//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2013 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ADNS2610.c
//
//  SYNOPSIS
//
//      Connect:    ADNS-SCK  to SCK    (== PortB.5)
//                  ADNS-SDIO to MOSI   (== PortB.3)
//            *and* ADNS-SDIO to MISO   (== PortB.4)
//
//      //////////////////////////////////////
//      //
//      // In ADNS.h
//      //
//      ...Choose a speed                  (Default: FOsc/8 == 2 MHz)
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      ADNSInit();                         // Called once at startup
//
//      PutADNSByte('A');                   // Blocks until complete
//
//      uint8_t InChar = GetUARTByte();     // Blocks until complete
//
//  DESCRIPTION
//
//      A simple driver module for blocking communications with ADNS-xxxx
//        optical flow sensors on an AVR processor.
//
//      The baud rate is set in the ADNS.h file.
//
//  NOTE
//
//      Due to the speed of the ADNS chip (2 MHz) and time needed to take an
//        interrupt on an ATMega (7 cycles, plus register saves and restores),
//        this module is implemented as a BLOCKING transciever.
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//  VERSION:    2013.02.28
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

#include <util/delay.h>

#include "PortMacros.h"
#include "ADNS2610.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Some port designations
//
#define ADNS_PORT   B                   // Port containing MISO, MOSI, &c

#define SS_BIT      2                   // Bits in ADNS port
#define MOSI_BIT    3
#define MISO_BIT    4
#define SCK_BIT     5

#define MOSI_WRITE  _SET_BIT(_DDR(ADNS_PORT),MOSI_BIT);     // MOSI is an output
#define MOSI_READ   _CLR_BIT(_DDR(ADNS_PORT),MOSI_BIT);     // MOSI is an input

#define ADNS_WRITE  0x80                // "Write" bit in addr data
#define ADNS_WAIT   while(!(SPSR & (1<<SPIF)))

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ADNSInit - Initialize ADNS
//
// This routine initializes the ADNS based on the settings above. Called from
//   init.
//
// Inputs:      None.
//
// Outputs:     None.
//
void ADNSInit(void) {

    _CLR_BIT(PRR,PRSPI);                        // Power up the SPI

    //
    // Enable port I/O and Tx interrupt
    //
    // NOTE: SS is REQUIRED to be set and low. Don't forget this!!!
    //
    _SET_BIT( _DDR(ADNS_PORT), SCK_BIT);     // SCK  is an output
    _SET_BIT( _DDR(ADNS_PORT),  SS_BIT);     // SS   is an output
    _CLR_BIT(_PORT(ADNS_PORT),  SS_BIT);     // Set low for now

    SPCR = (1 << SPE)  | 
           (1 << MSTR) | 
           (1 << CPOL) | 
           (1 << CPHA) | 
           (1 << SPR0) |
           (1 << SPR1);

#ifdef SPEED2X
    SPSR = (1 << ADNS2X);
#endif
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PutADNSByte - Send one byte out the ADNS port
//
// Send a byte out the ADNS port, block until completion.
//
// Inputs:      Address of register to write
//              Data for register
//
// Outputs:     None.
//
void PutADNSByte(ADNS_REG_T Addr,uint8_t Data) {

    MOSI_WRITE;         // Enable MOSI

    SPDR = Addr | ADNS_WRITE;
    ADNS_WAIT;

    SPDR = Data;
    ADNS_WAIT;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetADNSByte - Get one byte from the ADNS port
//
// Get a byte from the ADNS port, block until completion.
//
// Inputs:      Address of register to read
//
// Outputs:     Received byte
//
uint8_t GetADNSByte(ADNS_REG_T Addr) {
    //
    // Output 1 byte while keeping MOSI offline. This generates the clock and shifts
    //   data into the SPI, while keeping the output data offline.
    //
    MOSI_WRITE;         // Enable MOSI

    SPDR = Addr & (~ADNS_WRITE);
    ADNS_WAIT;

    MOSI_READ;          // MOSI High-Z
//JDJJDJ    _delay_us(100);     // Wait for ADNS to process address
    _delay_us(50);     // Wait for ADNS to process address

    SPDR = 0;
    ADNS_WAIT;

    return SPDR;
    }
