////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ZCrossTest.c
//
//  SYNOPSIS
//
//      Connect a ZCross signal to the port specified in ZCross.h
//
//      Compile, load, and run this module. The system will count the zero crossings
//        and report the totals once a second
//
//  DESCRIPTION
//
//      This program tests the Zero crossing interface.
//
//      You can easily change the pin and port assignments to suit your needs,
//        see ZCross.h for details.
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

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "UART.h"
#include "Serial.h"
#include "Timer.h"
#include "ZCross.h"

uint16_t    ZCrossCount;
uint8_t     TickTimer;

#define MSG_SECONDS 1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ZCrossTest - Test the Zero crossing interface
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //
    // Initialize the serial and zero cross system
    //
    ZCrossInit();                       // Initialize button system
    UARTInit();                         // For serial I/O

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    ZCrossCount = 0;
    TickTimer   = SECONDS(MSG_SECONDS);

    sei();                              // Enable interrupts

    PrintString("Begin ZCross test.\r\n");

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        while( TickTimer > 0 )
            sleep_cpu();

        TickTimer = SECONDS(MSG_SECONDS);

        PrintD(ZCrossCount,ZCrossCount);  // ==> %3d
        PrintCRLF();

        ZCrossCount = 0;
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    // Test using polled mode
//
//    while( !ZCrossSeen )
//        sleep_cpu();
//
//    ZCrossSeen = false;
//
//    ZCrossCount++;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ZCrossISR - Called by the zero cross system at each change
//
// Inputs:      None.
//
// Outputs:     None.
//
void ZCrossISR(void) {

    ZCrossCount++;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TimerISR - Called by the timer section once a tick
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerISR(void) {

    TickTimer--;    
    }
