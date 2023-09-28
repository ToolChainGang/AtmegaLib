////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      DigitalLogger.c
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Limit.h
//      //
//      ...Choose a port                   (Default: PortC)
//      ...Choose bits within a port       (Default: bits 0 and 1)
//
//      Connect up to 8 switches to PORTC and a serial monitor (ie - PC running
//        hyperterm) to the serial port. 
//
//      Compile, load, and run this module. When any switch changes state, the
//        system will report the time (since reset) and the new state.
//
//  DESCRIPTION
//
//      This program is a simple binary data logger with up to 8 inputs.
//
//      You can easily change the pin and port assignments to suit your needs,
//        see Limit.h for details.
//
//      The input can be easily captured on a PC for later analysis.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "UART.h"
#include "Serial.h"
#include "Limit.h"
#include "TimerB.h"
#include "PortMacros.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DigitalLogger - Report switch changes with timestamp
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

    LimitInit();                        // Initialize debounce system
    TimerBInit();                       // For timestamp and updates
    UARTInit();                         // For serial I/O

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    sei();                              // Enable interrupts

    PrintString("Reset.\r\n");

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1)
        sleep_cpu();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LimitISR - Called by the button system at each change
//
// Inputs:      Current state of buttons
//
// Outputs:     None.
//
void LimitISR(uint8_t Limits) {
    TIME_T Time = TimerBGetSeconds();
    uint8_t Secs, Mins, Hrs, Days;

    Secs = Time % 60;
    Time = Time / 60;

    Mins = Time % 60;
    Time = Time / 60;

    Hrs  = Time % 24;
    Time = Time / 24;

    Days = Time % 365;

    //
    // Print out the timestamp
    //
    PrintD(Days,103);           // => printf("%03d",Value);
    PutUARTByteW('.');
    PrintD(Hrs,102);            // => printf("%02d",Value);
    PutUARTByteW('.');
    PrintD(Mins,102);           // => printf("%02d",Value);
    PutUARTByteW('.');
    PrintD(Secs,102);           // => printf("%02d",Value);
    PutUARTByteW(':');
    PutUARTByteW(' ');

    //
    // Print out the new state
    //
    PrintH(Limits);
    PrintCRLF();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TimerBISR - Called by the timer section once a tick
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerBISR(void) {
    LimitUpdate(MS_PER_TICK);
    }
