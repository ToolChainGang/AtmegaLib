////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ESP8266Cmd.c
//
//  SYNOPSIS
//
//  DESCRIPTION
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
#include "UART.h"
#include "Serial.h"

#define HEARTBEAT_MS    1000            // mS between each hearbeat

static const char Heartbeat[] PROGMEM = "Lub-Dub\r\n";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SerialTest - Test the Serial port with continuous output & input
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {
    int CharCount = 0;
    int HBCount   = 0;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();

    sei();                              // Enable interrupts

    PrintString("Reset\r\n");           // String is in RAM

//  while(1) {
//      PutUARTByteW('K');    
//      _delay_ms(10);
//      }

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        char InChar;

        //
        // Print received characters in hex
        //
        while( (InChar = GetUARTByte()) ) {
            CharCount++;

            PrintD(CharCount,3);        // ==> %3d
            PrintString(": ");
            PrintH(InChar);             // ==> %2X
            PrintChar('(');
            PrintChar(InChar);          // ==> %c
            PrintChar(')');
            PrintCRLF();
            }

        //
        // Send heartbeat, once a second.
        //
        // The heartbeat text lives in the PROGRAM space. It
        //   occupies NO ram memory, but requires a special
        //   access method.
        //
        PrintD(HBCount++,5);
        PrintString(": ");
        PrintStringP(Heartbeat);

        _delay_ms(HEARTBEAT_MS);        // Wait a bit
        } 
    }
