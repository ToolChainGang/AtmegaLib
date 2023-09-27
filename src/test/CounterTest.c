////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      CounterTest.c
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Counter.h
//      //
//      ...Choose a timer input            (Default: Timer0)
//      ...Choose input configuration      (Default: rising edge)
//
//      Connect a source of pulses (such as a button) to PORTD.4 (T0) and a
//        serial monitor (ie - PC running hyperterm) to the serial port.
//
//      Compile, load, and run this module. The system will report the count
//        of pulses per second to the serial port.
//
//      Additionally, the program will print a message when the high-water mark
//        is reached (currently 50), and when the timer overflows.
//
//  DESCRIPTION
//
//      This program is a simple test of the counter input.
//
//      You can easily change the timer assignment and input details (rising
//        edge, falling edge, &c). See Counter.h for details.
//
//      The input can be easily captured on a PC for later analysis.
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
#include "PortMacros.h"
#include "TimerB.h"
#include "Counter.h"

#define REPORT_TIME     1       // Seconds between reports
#define HIGH_WATER     50       // High water mark


//
// Timer for reports
//
TIME_T  ReportTimer         __attribute__ ((section (".noinit")));
//
// Previous count reported
//
static  uint8_t PrevCount   __attribute__ ((section (".noinit")));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CounterTest - Test of the counter module
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //
    // Initialize the report counter
    //
    ReportTimer = SECONDS(REPORT_TIME); // Init timer

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    TimerBInit();
    CounterInit();
    CounterSetHWM(HIGH_WATER);          // Set high-water mark
    PrevCount = 0;

    sei();                              // Enable interrupts

    PrintString("Reset.\r\n");

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        sleep_cpu();                    // Wait for timer
        } 
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

    if( --ReportTimer > 0 )             // Time to report?
        return;                         // Nope - return

    ReportTimer = SECONDS(REPORT_TIME); // Reset report timer

    uint8_t NewCount = TimerGetCount();
    PrintD(NewCount-PrevCount,3);       // => printf "%3d"
    PrevCount = NewCount;

    PrintCRLF();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CounterHWM_ISR() - Called by the counter section at the high water mark
//
// Inputs:      None.
//
// Outputs:     None.
//
void CounterHWM_ISR() {

    PrintString("High Water: ");
    PrintD(HIGH_WATER,3);               // => printf "%3d"
    PrintCRLF();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CounterOFLO_ISR() - Called by the counter section when the timer overflows
//
// Inputs:      None.
//
// Outputs:     None.
//
void CounterOFLO_ISR() { 

    PrintString("Overflow!\r\n");
    }
