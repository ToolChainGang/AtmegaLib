//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      AUART.c
//
//  SYNOPSIS
//
//      AUARTInit();                        // Called once at startup
//
//      char InChar = GetAUARTByte();       // == 0 if no chars available
//
//      bool Success = PutAUARTByte('A');   // == FALSE if buffer was full
//
//      PutAUARTByteW('A');                 // Block until complete
//
//      If( AUARTBusy() ) ...               // TRUE if sending something
//
//  DESCRIPTION
//
//      An alternate serial driver which does not use the onboard UART.
//        on an AVR processor.
//
//      The baud rate and FIFO sizes can be set in the AltUART.h file
//
//  NOTES:
//
//      This interface WILL NOT receive a NUL character (ascii 0). This is on
//        purpose, to make for a simple interface.
//
//      These are not the putc() and getc() functions required for stdio
//        by WinAVR. See serial.h for those.
//
//  VERSION:    2015.11.07
//
//////////////////////////////////////////////////////////////////////////////////////////
//
//  MIT LICENSE
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//    this software and associated documentation files (the "Software"), to deal in
//    the Software without restriction, including without limitation the rights to
//    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//    of the Software, and to permit persons to whom the Software is furnished to do
//    so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//    all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//  HOW THIS WORKS
//
//  Clock an 8-bit timer at 2MHz and set the OCRA to match and reset every baud bit. For
//    example, at 9600 baud the bits come every 104uS. We set the timer input to be
//    F_CPU/8 == 2 MHz and OCRA to be 208.
//
//  Every time the OCRA interrupt happens we send out 1 BAUD bit as needed using a Tx
//    finite state machine that handles start, stop, and so on.
//
//  When we detect the Rx start bit we note the timer position and use it to set OCRB to
//    tell us when to read the input bits.
//
//  The OCRB timing synchronises to the leading edge of the start bit, then one and a
//    half bit times later a sample is taken in the middle of the first data bit. After
//    a further bit time delay a sample is taken in the middle of the second data bit
//    and so on until a sample has been taken in the middle of the eighth data bit.
//
//  For example, suppose a start bit comes in when the timer reads 38: we set 38+104 as
//    the compare interrupt in OCRB and use that to read input bits, using the receive
//    FSM.
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string.h>

#include <avr/interrupt.h>

#include "TimerMacros.h"
#include "AUART.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tx/Rx FSM states
//
//
typedef enum {
    START_BIT = 301,
    DATA_BITS,
    STOP_BIT,
    } SSTATE;

//////////////////////////////////////////////////////////////////////////////////////////

#define IFIFO_WRAP  (AIFIFO_SIZE-1)     // Wraparound mask for Rx
#define OFIFO_WRAP  (AOFIFO_SIZE-1)     // Wraparound mask for Tx

static struct {
    char    Rx_FIFO[AIFIFO_SIZE];
    char    Tx_FIFO[AOFIFO_SIZE];

    uint8_t Tx_FIFO_In;                 // FIFO input  pointer
    uint8_t Tx_FIFO_Out;                // FIFO output pointer
    uint8_t Rx_FIFO_In;                 // FIFO input  pointer
    uint8_t Rx_FIFO_Out;                // FIFO output pointer

    uint8_t TxChar;                     // Char currently sending
    uint8_t TxBits;                     // Number of remaining bits to send
    SSTATE  TxState;                    // State of transmit

    uint8_t RxChar;                     // Char currently sending
    uint8_t RxBits;                     // Number of remaining bits to send
    SSTATE  RxState;                    // State of transmit
    } AUART NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define PRTIMx          _PRTIM(ATIMER)

#define TCCRAx          _TCCRA(ATIMER)
#define TCCRBx          _TCCRB(ATIMER)
#define TCNTx           _TCNT(ATIMER)
#define TIMSKx          _TIMSK(ATIMER)

#define OCRAx           _OCRA(ATIMER)
#define OCIEAx          _OCIEA(ATIMER)
#define ATx_ISR         _TCOMPA_VECT(ATIMER)
#define DISABLE_Tx_INT  _CLR_BIT(TIMSKx,OCIEAx)
#define ENABLE_Tx_INT   _SET_BIT(TIMSKx,OCIEAx)

#define OCRBx           _OCRB(ATIMER)
#define OCIEBx          _OCIEB(ATIMER)
#define ARx_ISR         _TCOMPB_VECT(ATIMER)
#define DISABLE_Rx_INT  _CLR_BIT(TIMSKx,OCIEBx)
#define ENABLE_Rx_INT   _SET_BIT(TIMSKx,OCIEBx)

#define ENB_START_INT
#define DIS_START_INT

#define ARx_START_ISR   _VECT(INT,1)

//////////////////////////////////////////////////////////////////////////////////////////
//
// Timer count for baud rate.
//
// This should be the number of half-microseconds per bit sent.
//
//      BAUD   Count
//     -----   -----
//      9600   208
//     19200   104
//        :     :
//
#define CLOCK_COUNT     ((uint8_t)(2000000+(ABAUD/2)/ABAUD))

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AUARTInit - Initialize serial port
//
// This routine initializes the UART to BAUDRATE/8,1,n. Called from init. Also
//   enables the Rx interrupts, disables the Tx interrupt and clears the FIFOs.
//
// Inputs:      None.
//
// Outputs:     None.
//
void AUARTInit(void) {

    memset(&AUART,0,sizeof(AUART));

    _CLR_BIT(PRR,PRTIMx);           // Powerup the timer

    //
    // Setup the baud timer as free running, with OCRA as top value
    //
    TCCRAx = _WGM1(ATIMER);         // CTC based on OCRA
    TCCRBx = _CS1(ATIMER);          // 16MHz/8 = 2MHz
    TCNTx  = 0;
    OCRAx  = CLOCK_COUNT-1;         // And clock count for ticks

    //
    // Setup Tx pin as output
    //
    _SET_BIT( _DDR(ATx_PORT),ATx_BIT);      // Tx  is an output
    _SET_BIT(_PORT(ATx_PORT),ATx_BIT);      // Set high (mark) for now

    _CLR_BIT(MCUCR,PUD);                    // Allow I/O pullups
    _CLR_BIT( _DDR(ARx_PORT),ARx_BIT);      // Rx  is an input
    _SET_BIT(_PORT(ARx_PORT),ARx_BIT);      // With internal pullup

    DISABLE_Tx_INT;
    AUART.TxState = START_BIT;

    //
    // Setup low-level input on INT1 as start-bit detect
    //
    _CLR_MASK(EICRA,_PIN_MASK(ISC10) | _PIN_MASK(ISC11));
    _SET_BIT(EIMSK,_PIN_MASK(INT1));
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PutAUARTByte - Send one char out the serial port
//
// Send a char out the serial port. We stuff the char into the FIFO and enable
//   interrupts - at some point the interrupt handler will get serviced and
//   send the char out for us.
//
// Inputs:      Byte to send
//
// Outputs:     TRUE  if char was sent OK,
//              FALSE if buffer full
//
bool PutAUARTByte(char OutChar) {
    uint8_t NewIn;
    bool    Success = false;

    DISABLE_Tx_INT;

    //
    // If there's room in the buffer, add the new char
    //
    NewIn = (AUART.Tx_FIFO_In+1) & OFIFO_WRAP;

    if( NewIn != AUART.Tx_FIFO_Out ) {
        AUART.Tx_FIFO[AUART.Tx_FIFO_In] = OutChar;
        AUART.Tx_FIFO_In                = NewIn;
        Success = true;
        }

    ENABLE_Tx_INT;

    return(Success);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetAUARTByte - Get one char from the serial port
//
// Get a char from the serial port. The interrupt handler already received the
//   character for us, so this just pulls the char out of the receive FIFO.
//
// Inputs:      None
//
// Outputs:     ASCII char, if one was available
//              NUL   (binary value = 0) if no chars available
//
char GetAUARTByte(void) {
    char    OutChar = 0;

    DISABLE_Rx_INT;

    if( AUART.Rx_FIFO_In != AUART.Rx_FIFO_Out ) {
        OutChar = AUART.Rx_FIFO[AUART.Rx_FIFO_Out];
        AUART.Rx_FIFO_Out = (AUART.Rx_FIFO_Out+1) & IFIFO_WRAP;
        }

    ENABLE_Rx_INT;

    return(OutChar);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AUARTBusy - Return TRUE if AUART is busy sending output
//
// Inputs:      None
//
// Outputs:     TRUE  if AUART is busy sending output
//              FALSE if AUART is idle
//
bool AUARTBusy(void) { 
    return( AUART.Tx_FIFO_In != AUART.Tx_FIFO_Out || AUART.TxState != START_BIT); 
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ATx_ISR - Send out characters
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(ATx_ISR) {

    switch(AUART.TxState) {

        case START_BIT:
            //
            // If no chars are available, turn off interrupts
            //
            if( AUART.Tx_FIFO_In == AUART.Tx_FIFO_Out ) {
                DISABLE_Tx_INT;
                break;
                }

            //
            // Otherwise, queue up the next char & set the start bit
            //
            _CLR_BIT(_PORT(ATx_PORT),ATx_BIT);      // Send start bit

            AUART.TxBits      = 8;
            AUART.TxState     = DATA_BITS;
            AUART.TxChar      = AUART.Tx_FIFO[AUART.Tx_FIFO_Out];
            AUART.Tx_FIFO_Out = (AUART.Tx_FIFO_Out+1) & OFIFO_WRAP;
            break;

        case DATA_BITS:
            if( AUART.TxChar & 0x80 ) { _SET_BIT(_PORT(ATx_PORT),ATx_BIT); }
            else                      { _CLR_BIT(_PORT(ATx_PORT),ATx_BIT); }
            AUART.TxChar <<= 1;

            if( --AUART.TxBits == 0 )
                AUART.TxState = STOP_BIT;
            break;

        case STOP_BIT:
            _SET_BIT(_PORT(ATx_PORT),ATx_BIT);      // Send stop bit
            AUART.TxState = START_BIT;
            break;
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ARx_START_ISR - Detect the start bit
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(ARx_START_ISR) {

    //
    // Set a timeout of 1/2 baud from now, which will put the Rx FSM in the
    //   middle of the start bit.
    //
    uint16_t OTIME = TCNTx + CLOCK_COUNT/2;
    if( OTIME >= CLOCK_COUNT )
        OTIME -= CLOCK_COUNT;

    OCRAx = (uint8_t) OTIME;
    AUART.RxState = START_BIT;

    //
    // Disable start bit detection and start the receive FSM.
    //
    DIS_START_INT;
    ENABLE_Rx_INT;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ARx_ISR - Handle input received chars
//
// Get the input character and place it into the Rx_FIFO.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(ARx_ISR) {

    switch(AUART.RxState) {

        //
        // Actually 1/2 bit into the start bit. We could test for proper
        //   value (ie - noise immunity and all that), but noise isn't
        //   generally a problem with serial interfaces.
        //
        case START_BIT:
            AUART.RxState = DATA_BITS;
            AUART.RxChar  = 0;
            AUART.RxBits  = 8;
            break;            

        case DATA_BITS:
            AUART.RxChar <<= 1;

            if( _BIT_ON(_PORT(ARx_PORT),ARx_BIT) )
                AUART.RxChar |= 1;

            if( --AUART.RxBits > 0 )
                break;

            //
            // Receive complete. If there's room in the buffer, add the new char
            //
            uint8_t NewIn = (AUART.Rx_FIFO_In+1) & IFIFO_WRAP;

            if( NewIn != AUART.Rx_FIFO_Out ) {
                AUART.Rx_FIFO[AUART.Rx_FIFO_In] = AUART.RxChar;
                AUART.Rx_FIFO_In                = NewIn;
                }
                //
                // No room - Drop the character
                //

            AUART.RxState = STOP_BIT;
            break;

        case STOP_BIT:
            DISABLE_Rx_INT;
            ENB_START_INT;
            break;
        }

    }

