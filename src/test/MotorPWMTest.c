////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MotorPWMTest.C
//
//  SYNOPSIS
//
//      Connect a single motor to your system per instructions in MotorPWM.h
//
//      Compile, load, and run this module. The motor should ramp up in 10%
//        increments, spin at 100% for 5 seconds, ramp down in 10% increments,
//        then turn off for 5 seconds.
//
//      Then repeat.
//
//  DESCRIPTION
//
//      This is a simple AVR program with PWM H-Bridge motor control.
//
//      It is used to verify and test motor connections.
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

#include "MotorPWM.h"
#include "UART.h"
#include "Serial.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MotorPWMTest - Test the PWM motor control
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    MotorPWMInit();

    sei();                                          // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintString("Begin Motor PWM control.\r\n");    // String is in RAM

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        int8_t Decade;
        int8_t Speed;

        PrintString("Ramp Up: ");
        for( Decade = 0; Decade <= 10; Decade++ ) {
            Speed = 10*Decade;
            PrintD(Speed,0);
            PrintString(", ");
            MotorPWM(0,Speed);          // Set motor speed
            _delay_ms(1000);            // Wait 1 sec
            }

        PrintString("Spin 5 secs\r\n");
        _delay_ms(5000);                // Wait 5 sec

        PrintString("Ramp Dn: ");
        for( Decade = 0; Decade <= 10; Decade++ ) {
            Speed = 100 - 10*Decade;
            PrintD(Speed,0);
            PrintString(", ");
            MotorPWM(0,Speed);          // Set motor speed
            _delay_ms(1000);            // Wait 1 sec
            }

        PrintString("Wait 5 secs\r\n");
        _delay_ms(5000);                // Wait 5 sec
        }
    }
