////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ComparatorTest.c
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Comparator.h
//      //
//
//      Connect a source of pulses (such as a button) to PORTD.6 (AIN0) and 
//        PORTD.7 (AIN1), and a serial monitor (ie - PC running hyperterm) to
//        the serial port.
//
//      Compile, load, and run this module. The system will report the count
//        of pulses per second to the serial port.
//
//  DESCRIPTION
//
//      This program is a simple test of the analog comparator.
//
//  VERSION:    2016.02.04
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

#include <string.h>

#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <UART.h>
#include <Serial.h>
#include <PortMacros.h>
#include <TimerB.h>
#include <Comparator.h>

#define REPORT_TIME     1               // Seconds between reports

//
// Timer for reports
//
TIME_T  ReportTimer                 __attribute__ ((section (".noinit")));

//////////////////////////////////////////////////////////////////////////////////////////
//
// One minute worth of counts
//
#define NUM_SECS 60

static struct {
    int     SecCount;
    int     SecIndex;
    int     Secs[NUM_SECS];
    } Minute NOINIT;

static int Round NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//
// Output for clicker
//
#define CLICK_PORT  D               // Use PORTD
#define CLICK_PIN   2               // CLICK is on pin 2

#define CLICK_OFF   _CLR_BIT(_PORT(CLICK_PORT),CLICK_PIN)
#define CLICK_ON    _SET_BIT(_PORT(CLICK_PORT),CLICK_PIN)

//
// Output for clicker
//

#define DEBUG

#ifdef DEBUG
int Debug1;
int PrevDebug1;
int Debug2;
int PrevDebug2;
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ComparatorTest - Test of the comparator module
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int __attribute__((OS_main)) main(void) {

    //
    // Initialize
    //
    UARTInit();
    ReportTimer = SECONDSB(REPORT_TIME);// Init timer

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    TimerBInit();
    ComparatorInit();

    memset(&Minute,0,sizeof(Minute));
    Round = 0;

#ifdef DEBUG
    Debug1     = 0;
    PrevDebug1 = 0;
    Debug2     = 0;
    PrevDebug2 = 0;
#endif

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the CLICK port
    //
    _SET_BIT( _DDR(CLICK_PORT),CLICK_PIN);  // CLICK is an output
    CLICK_OFF;                              // Set off for now

    sei();                                  // Enable interrupts

    PrintString("Comparator Test.\r\n");

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        sleep_cpu();                    // Wait for timer
        } 
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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

    ReportTimer = SECONDSB(REPORT_TIME);// Reset report timer

    PrintD(Round++,0);
    PrintString(" ");

    PrintString("CPS: ");
    PrintD(Minute.SecCount,3);          // => printf "%5d"
    PrintChar(' ');

    Minute.Secs[Minute.SecIndex] = Minute.SecCount;
    Minute.SecCount = 0;
    Minute.SecIndex = (Minute.SecIndex + 1) % NUM_SECS;

    int SecTotal = 0;
    for( int i=0; i<NUM_SECS; i++ )
        SecTotal += Minute.Secs[i];

    PrintString("CPM: ");
    PrintD(SecTotal,5);                  // => printf "%5d"
    PrintCRLF();

#ifdef DEBUG
    if( Debug1 != PrevDebug1 ) {
        PrevDebug1 = Debug1;
        PrintCRLF();
        PrintString("Debug1: ");
        PrintD(PrevDebug1,0);
        PrintCRLF();
        }
    if( Debug2 != PrevDebug2 ) {
        PrevDebug2 = Debug2;
        PrintCRLF();
        PrintString("Debug2: ");
        PrintD(PrevDebug2,0);
        PrintCRLF();
        }
#endif
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ComparatorISR - Called by the comparator section when an event happens
//
// Inputs:      None.
//
// Outputs:     None.
//
void ComparatorISR(void) {

    Minute.SecCount++;
    CLICK_ON;
    _delay_us(500);
    CLICK_OFF;
    }
