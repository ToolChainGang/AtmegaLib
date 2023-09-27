////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TCD1304.c
//
//  SYNOPSIS
//
//      See TCD1304.h
//
//  DESCRIPTION
//
//      Interface to the TCD1304 linear CCD array
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

#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "TCD1304.h"

#include "PortMacros.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define TCDPORT_DDR _DDR(TCD_PORT)
#define TCDPORT     _PORT(TCD_PORT)
#define TCDMASK     (_PIN_MASK(TCD_GATE) | _PIN_MASK(TCD_CLK) | _PIN_MASK(TCD_SHUT))

#define TCD_GATE_UP _SET_BIT(TCDPORT,TCD_GATE)
#define TCD_GATE_DN _CLR_BIT(TCDPORT,TCD_GATE)
#define TCD_CLK_UP  _SET_BIT(TCDPORT,TCD_CLK)
#define TCD_CLK_DN  _CLR_BIT(TCDPORT,TCD_CLK)
#define TCD_SHUT_UP _SET_BIT(TCDPORT,TCD_SHUT)
#define TCD_SHUT_DN _CLR_BIT(TCDPORT,TCD_SHUT)

uint8_t TCD1304Data[TCD_BUFSIZE];

#define START_ATOD  { _SET_BIT(ADCSRA,ADSC); }  // Start the AtoD conversion
#define ATOD_WAIT   { while(_BIT_ON(ADCSRA,ADSC)); }
#define ADMUX_VAL   (_PIN_MASK(REFS0) | _PIN_MASK(ADLAR))
    
//#define _NOP        __asm__ __volatile__ ("nop");
//#define _NOP        do { __asm__ __volatile__ ("nop" ::); } while (0)

#define _NOP    asm volatile("nop")

#define CLOCK       \
    TCD_CLK_DN;     \
    _delay_us(1);   \
    TCD_CLK_UP;     \
    _delay_us(1);   \

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TCD1304Init - Initialize TCD1304 chip and prepare for reading
//
// Inputs:      None.
//
// Outputs:     None.
//
void TCD1304Init(void) {

    _SET_MASK(TCDPORT_DDR,TCDMASK);         // Make CCD control lines outputs
    TCD_GATE_UP;
    TCD_CLK_DN;
    TCD_SHUT_DN;

    //
    // Setup AtoD channels for input
    //
    _CLR_BIT(PRR,PRADC);                    // Powerup the A/D converter

    DIDR0  = 0;                             // Turn off digital outputs
    ADCSRB = 0;                             // Free running mode
    ADMUX  = ADMUX_VAL;                     // AVCC as ref, number of channels
//    ADCSRA = _PIN_MASK(ADEN);               // Enable, fastest conversion rate



//    ADCSRA = _PIN_MASK(ADPS2) | _PIN_MASK(ADPS1) | _PIN_MASK(ADPS0) |   // Prescale to 150 KHz
//             _PIN_MASK(ADEN);                        // Enable, Enable int



    ADCSRA = _PIN_MASK(ADPS1) |
             _PIN_MASK(ADEN);                        // Enable, Enable int
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Read1Data - Get one data point from the TCD3104 chip
//
// Inputs:      None.
//
// Outputs:     Data value read from chip
//
// NOTE: Private function, for internal use only
//
uint16_t Read1Data(void) {

    TCD_CLK_DN;
    START_ATOD;
    ATOD_WAIT;    
    TCD_CLK_UP;
    _delay_us(1);
    return ADCH;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TCD1304Read - Read one scanline from TCD1304 chip
//
// Inputs:      Number of NS to integrate signal
//              Ptr to appropriate buffer
//
// Outputs:     None.
//
void TCD1304Read(void) {

    memset(TCD1304Data,0,sizeof(TCD1304Data));

    cli();

    TCD_CLK_UP;
    _delay_us(1);
    TCD_GATE_DN;
    _delay_us(1);
    TCD_SHUT_UP;

    for( uint16_t Delay = 0; Delay < 1000; Delay++ ) {
        CLOCK;
        _delay_us(1);
        }

    //
    // Integrate for the specified time
    //
    TCD_SHUT_DN;            // Start integration
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;

    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;

    TCD_GATE_UP;

    //
    // Ignore first 32 elements: Dummy outputs (16) + Light shield (13) + extra (3)
    //
    for( uint8_t Dummy = 0; Dummy < 16; Dummy++ )
        Read1Data();

    //
    // Read the elements, and store them into the data array 4 at a time.
    //
    for( uint16_t Index = 0; Index < TCD_BUFSIZE; Index++ ) {
        uint16_t Total = 0;
        Total += Read1Data();
        Total += Read1Data();
        Total += Read1Data();
        Total += Read1Data();

        TCD1304Data[Index] = Total >> 2;
        }


    //
    // Ignore last 14 elements: Dummy outputs (14)
    //
    for( uint8_t Dummy = 0; Dummy < 16; Dummy++ )
        Read1Data();

    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;
    CLOCK;

    sei();
    }
