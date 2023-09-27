////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ServoTest.C
//
//  SYNOPSIS
//
//      Connect a single servoto your system per instructions in Servo.h
//
//      Compile, load, and run this module. The servo should:
//
//          Left   -> Middle
//          Wait 2 secs
//          Middle -> Right
//          Wait 2 secs
//          Right  -> Middle
//          Wait 2 secs
//          Middle -> Left
//          Wait 2 secs
//
//      Then repeat.
//
//  DESCRIPTION
//
//      This is a simple AVR program with servo control
//
//      It is used to verify and test servo connections.
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

#include "PortMacros.h"
#include "Servo.h"
#include "UART.h"
#include "Serial.h"

#define MOVE_DELAY_MS   25      // Delay between pct position
#define PAUSE_MS      2000      // Delay for stopping points

//
// The OFFSET and SPAN are defined by the caller to determine, since
//   they are different for each type of servo.
//
#define OFFSET          10
#define SPAN            220
#define MIN_POS         OFFSET          // Minimum position
#define MAX_POS         SPAN+OFFSET     // Maximum position
#define MID_POS         (SPAN/2)+OFFSET

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ServoTest - Test the servo control
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {
    int Round = 0;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    ServoInit();

    sei();                                          // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintString("Begin Servo control.\r\n");        // String is in RAM

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    Servo(0,0);                             // Start in right position

    while(1) {
        int i;

        PrintD(Round++,0);
        PrintString(": ");

        PrintString("Left -> Mid, ");
        for( i=MIN_POS; i<=MID_POS; i++ ) { // Move slowly to the middle
            Servo(0,i);
            _delay_ms(MOVE_DELAY_MS);
            }

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintString("Mid -> Right, ");
        for( i=MID_POS; i<=MAX_POS; i++ ) { // Move slowly to left position
            Servo(0,i);
            _delay_ms(MOVE_DELAY_MS);
            }

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintString("Right -> Mid, ");
        for( i=MAX_POS; i>=MID_POS; i-- ) { // Move slowly to neutral position
            Servo(0,i);
            _delay_ms(MOVE_DELAY_MS);
            }

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintString("Mid -> Left");
        for( i=MID_POS; i>=MIN_POS; i-- ) { // Move slowly to right position
            Servo(0,i);
            _delay_ms(MOVE_DELAY_MS);
            }

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintCRLF();
        }
    }
