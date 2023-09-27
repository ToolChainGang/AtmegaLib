////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      StepperTest.C
//
//  SYNOPSIS
//
//      Connect a single Stepperto your system per instructions in Stepper.h
//
//      Compile, load, and run this module. The Stepper should:
//
//          Left  2 revolutions
//          Wait  2 secs
//          Right 2 revolutions
//          Wait  2 secs
//
//      Then repeat.
//
//  DESCRIPTION
//
//      This is a simple AVR program with Stepper control
//
//      It is used to verify and test Stepper connections.
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

#include "UART.h"
#include "Serial.h"

#include "Stepper.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// StepperTest - Test the Stepper control
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {
    int Round = 0;

    AccelStepper stepper1; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5


    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();

    sei();                                          // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintString("Begin Stepper control.\r\n");        // String is in RAM

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    stepper1.setMaxSpeed(200.0);
    stepper1.setAcceleration(100.0);
    
    while(1) {
        int i;

        PrintD(Round++,0);
        PrintString(": ");

        PrintString("Fwd 2 revs, ");

        stepper1.moveTo(400);
        while( stepper1.distanceToGo() != 0 )
            stepper1.run();

        PrintString("Wait 2 secs, ");
        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintString("Rev 2 revs, ");
        stepper1.moveTo(-400);
        while( stepper1.distanceToGo() != 0 )
            stepper1.run();

        PrintString("Wait 2 secs, ");
        _delay_ms(PAUSE_MS);                // Wait a bit


        PrintCRLF();
        }
    }
