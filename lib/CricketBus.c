////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      CricketBus.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In CricketBus.h
//      //
//      #define CRICKET_PORT                D
//      #define CRICKET_BIT                 7
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      CricketBusInit();                   // Called once at startup
//
//      CricketBusPut(0x100,true);          // Send 0x100 as command to cricket bus
//      CricketBusPut(0x203,false);         // Send 0x203 as data (ie - not command) to bus
//
//      //////////////////////////////////////
//      //
//      // Cricket LED specific calls
//      //
//      In the calls below, ID can be zero (for "any"), or 1 or 2 to identify
//        which of two LED displays are connected. See Crickey LED display documentation for
//        how to set the device ID (involves cutting a trace on the board).
//
//      CricketLEDDec(1025,ID)              // Display numeric "1025" on LED display
//      CricketLEDHex(0x1025,ID)            // Display numeric "1025" on LED display
//      CricketLEDBright(7,ID)              // Set brightness level to 7
//
//      CricketLEDPat(a,b,c,d,ID)           // Set segments based on pattern (see notes below)
//
//  DESCRIPTION
//      
//      Simple cricket bus interface
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

#include <avr/interrupt.h>
#include <util/delay.h>

#include "CricketBus.h"
#include "PortMacros.h"

#define CRICKET_HIGH    _SET_BIT(_PORT(CRICKET_BUS_PORT),CRICKET_BUS_PIN);  // Bus high
#define CRICKET_LOW     _CLR_BIT(_PORT(CRICKET_BUS_PORT),CRICKET_BUS_PIN);  // Bus low

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CricketBusInit - Initialize cricket bus interface
//
// This routine initializes the cricket bus based on the settings above. Called from
//   init.
//
// Inputs:      None.
//
// Outputs:     None.
//
void CricketBusInit(void) {
    
    _SET_BIT(_DDR(CRICKET_BUS_PORT),CRICKET_BUS_PIN);   // Bus line is an output

    CRICKET_HIGH;                                       // Set high until first data
    }


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// CricketBusPut - Send one byte and command bit out the cricket bus
//
// Send a byte out the serial port, and the command bit.
//
// Inputs:      Byte to send
//              TRUE if this is a command byte
//
// Outputs:     None.
//
void CricketBusPut(uint8_t Byte,bool Command) {

    //
    // The Cricket bus requires some specific timing, and the arduino interrupt latency
    //   cannot be guaranteed.
    //
    // Just bit-bang it and hope for the best
    //
    // Most Arduinos (nano, &c) run at 16MHz clock speed. The loop and if-statement
    //   below take up most of a microsecond, so the BIT_DELAY time is shortened
    //   from 10uS (per the cricket bus spec) to 9uS to compensate. This then shows
    //   each individual bit for almost exactly 10 uS.
    //
    // When running at a significantly slower clock speed, adjust the delay time as 
    //   appropriate; for example, when running at 8MHz the loop will take twice
    //   as long, so the delay should be 8uS instead of 9uS.
    //
    // When running at a significantly higher speed, such as 80MHz (an STM32, for
    //   example), the loop time will be negligible. In that case you should use
    //   the bus spec 10uS delay time.
    //
    cli();                                          // Disable interrupts

#define BIT_DELAY   9                               // Bit delay time, in uS. See comment above

    CRICKET_LOW;
    _delay_us(100);     // Pre-start
    CRICKET_HIGH;
    _delay_us(10);      // Start bit

    //
    // Send the byte data, lowest bit first
    //
    for( uint8_t Bit = 0; Bit < 8; Bit++ ) {
        if( Byte & 0x01 ) { CRICKET_HIGH; }
        else              { CRICKET_LOW;  }
        Byte >>= 1;
        _delay_us(BIT_DELAY);
        }

    if( Command ) { CRICKET_LOW;  }
    else          { CRICKET_HIGH; }
    _delay_us(10);      // Command bit
    CRICKET_HIGH;

    sei();                                          // Enable interrupts
    }

