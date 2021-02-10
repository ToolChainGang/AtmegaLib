//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      UARTTest.c
//
//  SYNOPSIS
//
//      Connect your project to a host computer. For example, on a windows
//        system connect to one of the serial ports and view with hyperterm.
//
//      (For systems such as Arduino which use the USB->Serial connection,
//        connect to a USB port and determine the COM port to use from the
//        device manager. Then use hyperterm for that port.)
//
//      Compile, load, and run this module. The UART sends uppercase
//        characters once a second.
//
//      Additionally, chars received are incremented and echoed back ('a' is
//        echoed as 'b', 'M' is echoed as 'N', and so on).
//
//      You can easily change the baud rate in the UART.h file
//
//  DESCRIPTION
//
//      This is a simple AVR program with serial I/O.
//
//      It is used to verify and test the serial connection, null modem, 
//        baud rate, &c.
//
//  VERSION:    2010.12.06
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

#include "UART.h"

#define CHAR_MS   1000              // mS between each char sent

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// UARTTest - Test the UART with continuous output & input processing
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int main(void) {
    char OutChar = 'A';

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    InitUART();

    sei();                              // Enable interrupts

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        char InChar;

        //
        // Echo characters incremented by 1.
        //
        // This is not terribly useful, but it tests
        //   the UART connection - baud rate, null modem, &c
        //
        while( (InChar = GetUARTByte()) )
            PutUARTByte(InChar+1);

        //
        // Send out continuous chars, once a second.
        //
        PutUARTByte(OutChar);

        if( ++OutChar > 'Z' ) 
            OutChar = 'A';

        _delay_ms(CHAR_MS);                 // Wait a bit
        } 
    }
