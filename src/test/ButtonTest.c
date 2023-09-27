////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ButtonTest.c
//
//  SYNOPSIS
//
//      Connect a button to PORTD.2 and a serial monitor (ie - PC running
//        hyperterm) to the serial port.
//
//      Compile, load, and run this module. The system will report all button
//        presses and releases with a message to the serial port.
//
//  DESCRIPTION
//
//      This program tests the Button debounce interface.
//
//      You can easily change the pin and port assignments to suit your needs,
//        see Button.h for details.
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
#include "Button.h"
#include "Timer.h"
#include "PortMacros.h"

#define SWITCH_BIT    2                 // Bit within port of switch

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ButtonState - Report the state of a button
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //
    // Initialize the timer and button system
    //
    _CLR_BIT(MCUCR,PUD);                // Allow I/O pullups

    ButtonInit();                       // Initialize button system
    TimerInit();                        // For updates
    UARTInit();                         // For serial I/O

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    sei();                              // Enable interrupts

    PrintString("Button test.\r\n");

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1)
        sleep_cpu();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    // Test using polled mode
//
//    while( !ButtonChange )
//        sleep_cpu();
//
//    ButtonChange = false;
//
//    if( _BIT_ON(ButtonValue,SWITCH_BIT) ) 
//        PrintString("Switch open.\r\n");
//
//    if( _BIT_OFF(ButtonValue,SWITCH_BIT) )
//        PrintString("Switch closed.\r\n");
//        }
//
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ButtonISR - Called by the button system at each change
//
// Inputs:      Current state of buttons
//
// Outputs:     None.
//
void ButtonISR(uint8_t Buttons) {

    if( _BIT_ON(Buttons,SWITCH_BIT) ) 
        PrintString("Switch open.\r\n");

    if( _BIT_OFF(Buttons,SWITCH_BIT) )
        PrintString("Switch closed.\r\n");
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
    ButtonUpdate(MS_PER_TICK);
    }
