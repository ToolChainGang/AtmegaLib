////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MotorTest.c
//
//  SYNOPSIS
//
//      Connect a DC motor to a driver via PORTD.6 and PORTD.7 (per Motor.h)
//
//      Compile, load, and run this module. The program will spin the motor FWD for
//        1 sec, pause for 1 sec, REV for 1 sec, and pause for 1 sec.
//
//      Additionally, a count of the spin cycles is printed to the serial port.
//
//      You can easily change the motor settings in Motor.h
//
//  DESCRIPTION
//
//      This is a simple AVR program to test H-bridge DC motors.
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
#include <ctype.h>

#include "UART.h"
#include "Serial.h"
#include "Motor.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MotorTest - Test the motor control software
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {
    int LoopCount;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    MotorInit();

    LoopCount = 0;

    sei();                              // Enable interrupts

    //
    // End of init
    //
    //////////////////////////////////////////////////////////////////////////////////////

    PrintCRLF();
    PrintString("Motor Test\r\n");
    PrintString(MOTOR0_STRING);
#ifdef MOTOR0_NEG_LOGIC
    PrintString(" (inverted logic)\r\n");
#endif

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    //
    while(1) {

        while(1) {
            PrintD(LoopCount,3);        // ==> %3d
            PrintString(": ");

            PrintString("FWD 1 sec, ");
            Motor(0,MOTOR_FWD);         // Engage MOTOR0 in FWD direction
            _delay_ms(1000);            // Wait 1 sec

            PrintString("OFF 1 sec, ");
            Motor(0,MOTOR_OFF);         // Engage MOTOR0 in OFF direction
            _delay_ms(1000);            // Wait 1 sec

            PrintString("REV 1 sec, ");
            Motor(0,MOTOR_REV);         // Engage MOTOR0 in REV direction
            _delay_ms(1000);            // Wait 1 sec

            PrintString("OFF 1 sec");
            Motor(0,MOTOR_OFF);         // Engage MOTOR0 in OFF direction
            _delay_ms(1000);            // Wait 1 sec

            PrintCRLF();
            LoopCount++;
            }
        }
    }
