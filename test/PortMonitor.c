////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      PortMonitor.c
//
//  SYNOPSIS
//
//      Look at the port pins and report any changes.
//
//      Compile, load, and run this module. The system will report all pin
//        changes in hex and binary
//
//  DESCRIPTION
//
//      This program uses the limit module, but prints the values in hex and binary
//
//      You can easily change the pin and port assignments to suit your needs,
//        see Limit.h for details.
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
#include <util/delay.h>

#include <UART.h>
#include <Serial.h>
#include <PortMacros.h>

#define MON_PORT     K                  // Monitor PORTA
#define MON_MASK     0x0F               // ...using bits 0, 1, 2

uint8_t     PrevValue;
uint16_t    ChangeCount;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PortMonitor - Report the state of a port
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //
    // Limit inputs
    // 
    _CLR_MASK( _DDR(MON_PORT),MON_MASK);// All monitored are inputs
    _CLR_MASK(_PORT(MON_PORT),MON_MASK);// Without internal pullup

    UARTInit();                         // For serial I/O

    sei();                              // Enable interrupts

    PrintString("Reset Port Monitor.\r\n");

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        PrevValue = _GET_MASK(_PIN(MON_PORT),MON_MASK);

        PrintD(ChangeCount,-5);
        PrintString(": 0x");
        PrintH(PrevValue);
        PrintString(": ");
        PrintB(PrevValue);
        PrintCRLF();

        while( _GET_MASK(_PIN(MON_PORT),MON_MASK) == PrevValue );
//        _delay_ms(1000);

        ChangeCount++;
        } 
    }

