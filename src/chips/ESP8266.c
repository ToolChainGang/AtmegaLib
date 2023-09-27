////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ESP8266.c
//
//  SYNOPSIS
//
//      See ESP8266.h
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <avr/interrupt.h>

#include "PortMacros.h"
#include "ESP8266.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IFIFO_WRAP  (ESPI_FIFO_SIZE-1)  // Wraparound mask for Rx
#define OFIFO_WRAP  (ESPO_FIFO_SIZE-1)  // Wraparound mask for Tx

static struct {
    char    Rx_FIFO[ESPI_FIFO_SIZE];
    char    Tx_FIFO[ESPO_FIFO_SIZE];

    uint8_t Tx_FIFO_In;                 // FIFO input  pointer
    uint8_t Tx_FIFO_Out;                // FIFO output pointer
    uint8_t Rx_FIFO_In;                 // FIFO input  pointer
    uint8_t Rx_FIFO_Out;                // FIFO output pointer
    } ESP8266 NOINIT;


#define CYCLES_PER_BIT  (F_CPU+ESP8266_BAUD/2)/ESP8266_BAUD)

#define ESP8266_PORT    D               // Port on which I/O happens
#define RxBIT           3               // Bit for Rx
#define TxBIT           3               // Bit for Tx

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ESP8266Init - Initialize ESP8266 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
void ESP8266Init(void) {

    memset(&UART,0,sizeof(UART));

    _CLR_BIT(MCUCR,PUD);                    // Enable internal pullups

    _CLR_BIT( _DDR(ESP8266_Port),RxBIT);    // Rx is an input
    _SET_BIT(_PORT(ESP8266_Port),RxBIT);    // With internal pullup


  sssStopAll();
  
  //  prepare Rx and Tx pins
  pinMode(sssRxPin, INPUT_PULLUP);
  pinMode(sssTxPin, OUTPUT);

  // prepare Timer2
  TCCR2A = B00000000; // set normal mode (not fast PWM)
                       // with fast PWM the clock resets when it matches
                       // and gives the wrong timing
                       
  TCCR2B = B00000011;  // set the prescaler to 32 - this gives counts at 2usec intervals
  
  // set baud rate timing
  sssBaudTimerCount = 1000000UL / sssBaudRate / 2; // number of counts per baud at 2usecs per count
  sssBaudTimerFirstCount = sssBaudTimerCount; // I thought a longer period might be needed to get into
                                              // the first bit after the start bit - but apparently not
  // normally an interrupt occurs after sssBaudTimerCount * 2 usecs (104usecs for 9600 baud)

  // length of required idle period to synchronize start bit detection
  sssIdleIntervalMicros = 1000000UL / 9600 * 25 ; // 2.5 byte lengths
  
  sssDbg("idleCount ", sssIdleIntervalMicros);
  sssDbg("baudTimer ",sssBaudTimerCount);
  
  sssPrepareToListen();
  sssReady = true;


























    TIMSK1 = 0;

#if CYCLES_PER_BIT < 7085



  #define CONFIG_TIMER_NOPRESCALE()	(TIMSK1 = 0, TCCR1A = 0, TCCR1B = (1<<ICNC1) | (1<<CS10))
  #define CONFIG_TIMER_NOPRESCALE()	(TIMSK3 = 0, TCCR3A = 0, TCCR3B = (1<<ICNC3) | (1<<CS30))
#endif



void AltSoftSerial::init(uint32_t cycles_per_bit) {


	if (cycles_per_bit < 7085) {
		CONFIG_TIMER_NOPRESCALE();
	} else {
		cycles_per_bit /= 8;
		if (cycles_per_bit < 7085) {
			CONFIG_TIMER_PRESCALE_8();
		} else {
			return; // minimum 283 baud at 16 MHz clock
		}
	}
	ticks_per_bit = cycles_per_bit;
	rx_stop_ticks = cycles_per_bit * 37 / 4;
	pinMode(INPUT_CAPTURE_PIN, INPUT_PULLUP);
	digitalWrite(OUTPUT_COMPARE_A_PIN, HIGH);
	pinMode(OUTPUT_COMPARE_A_PIN, OUTPUT);
	rx_state = 0;
	rx_buffer_head = 0;
	rx_buffer_tail = 0;
	tx_state = 0;
	tx_buffer_head = 0;
	tx_buffer_tail = 0;
	ENABLE_INT_INPUT_CAPTURE();
}






















    _CLR_BIT(PRR,PRUSART0);             // Power up the UART

    //
    // Set the baud rate
    //
#include <util/setbaud.h>

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    _SET_BIT(UCSR0A,U2X0);
#else
    _CLR_BIT(UCSR0A,U2X0);
#endif

    //
    // Enable port I/O and Rx interrupt
    //
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);    // 8,n,1
    UCSR0B = (1<< RXEN0) | (1<< TXEN0) | (1<<RXCIE0);

    //
    // Enable internal pull-up resistor on pin D0 (RX), to supress line noise
    //
    _CLR_BIT( DDRD,PIND0);
    _SET_BIT(PORTD,PIND0);
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PutUARTByte - Send one char out the serial port
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
bool PutUARTByte(char OutChar) {
    uint8_t NewIn;
    bool    Success = false;

    _CLR_BIT(UCSR0B,UDRIE0);                    // Disable UART interrupts

    //
    // If there's room in the buffer, add the new char
    //
    NewIn = (UART.Tx_FIFO_In+1) & OFIFO_WRAP;

    if( NewIn != UART.Tx_FIFO_Out ) {
        UART.Tx_FIFO[UART.Tx_FIFO_In] = OutChar;
        UART.Tx_FIFO_In               = NewIn;
        Success = true;
        }

    _SET_BIT(UCSR0B,UDRIE0);                    // Enable UART interrupts

    return(Success);
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GetUARTByte - Get one char from the serial port
//
// Get a char from the serial port. The interrupt handler already received the
//   character for us, so this just pulls the char out of the receive FIFO.
//
// Inputs:      None
//
// Outputs:     ASCII char, if one was available
//              NUL   (binary value = 0) if no chars available
//
char GetUARTByte(void) {
    char    OutChar = 0;

    _CLR_BIT(UCSR0B,RXCIE0);                     // Disable UART interrupts

    if( UART.Rx_FIFO_In != UART.Rx_FIFO_Out ) {
        OutChar = UART.Rx_FIFO[UART.Rx_FIFO_Out];
        UART.Rx_FIFO_Out = (UART.Rx_FIFO_Out+1) & IFIFO_WRAP;
        }

    _SET_BIT(UCSR0B,RXCIE0);                    // Enable UART interrupts

    return(OutChar);
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UARTBusy - Return TRUE if UART is busy sending output
//
// Inputs:      None
//
// Outputs:     TRUE  if UART is busy sending output
//              FALSE if UART is idle
//
bool UARTBusy(void) { return( UART.Tx_FIFO_In != UART.Tx_FIFO_Out ); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// USART_RX_vect - Handle input received chars
//
// Get the input character and place it into the Rx_FIFO.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(USART_RX_vect) {
    uint8_t NewIn;
    char    NewChar;

    NewChar = UDR0;                         // Get data, clear errors

    //
    // If there's room in the buffer, add the new char
    //
    NewIn = (UART.Rx_FIFO_In+1) & IFIFO_WRAP;

    if( NewIn != UART.Rx_FIFO_Out ) {
        UART.Rx_FIFO[UART.Rx_FIFO_In] = NewChar;
        UART.Rx_FIFO_In               = NewIn;
        }

    //
    // No room - Drop the character
    //
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// USART_UDRE_vect - Queue up another char to be transmitted
//
// Pull the next character to be sent from the TX_FIFO and send it. If no
//   more, turn off interrupt.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(USART_UDRE_vect) {

    //
    // If more chars are available, queue one up.
    //
    if( UART.Tx_FIFO_In != UART.Tx_FIFO_Out ) {
        UDR0             = UART.Tx_FIFO[UART.Tx_FIFO_Out];
        UART.Tx_FIFO_Out = (UART.Tx_FIFO_Out+1) & OFIFO_WRAP;
        }

    //
    // Else turn off interrupts, for now.
    //
    else _CLR_BIT(UCSR0B,UDRIE0);       // Disable buffer empty interrupt
    }
