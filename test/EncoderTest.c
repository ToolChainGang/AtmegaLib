////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      EncoderTest.c
//
//  SYNOPSIS
//
//      Connect an encoder to PORTA.1 and PORTA.2 and a serial monitor (ie - PC running
//        hyperterm) to the serial port.
//
//      Compile, load, and run this module. The system will report all encoder
//        changes with a message to the serial port.
//
//  DESCRIPTION
//
//      This program tests the Encoder interface.
//
//      You can easily change the pin and port assignments to suit your needs,
//        see Encoder.h for details.
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

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "UART.h"
#include "Serial.h"
#include "Encoder.h"
#include "PortMacros.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LimitState - Report the state of a button
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //
    // Initialize the timer and button system
    //
    EncoderInit();                      // Initialize encoder system
    UARTInit();                         // For serial I/O

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    sei();                              // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintCRLF();
    PrintString("Reset EncoderTest.\r\n");
    PrintCRLF();
    PrintString("Twist encoder shaft to see values.");
    PrintCRLF();
    PrintCRLF();

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    ENCODER_T   PrevPos = 0;

    while(1) {
        sleep_cpu();

        ENCODER_T EncoderPos = GetEncoder();
        if( EncoderPos == PrevPos )
            continue;

        if( EncoderPos < 0 ) {
            PrintChar('-');
            PrintD(-EncoderPos,0);
            }
        else PrintD(EncoderPos,0);
        PrintCRLF();

        PrevPos = EncoderPos;
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// EncoderISR - Interrupt routine called when encoder changes state
//
// Inputs:      Pin state of encoder, shifted to low order bits
//
// Outputs:     None.
//
void EncoderISR(uint8_t EncoderBits) {

//    PrintB(EncoderBits);
//    PrintCRLF();
    }
