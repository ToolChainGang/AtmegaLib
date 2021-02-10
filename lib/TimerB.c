//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TimerB.c
//
//  DESCRIPTION
//
//      Timer processing
//
//      This is a copy of the "Timer.c" file, but referring to TimerB in all cases.
//
//      This allows programs to have more than one timer running at different speeds.
//
//      See Timer.h for a complete description
//
//  VERSION:    2010.12.19
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

#include <avr\io.h>
#include <avr\interrupt.h>

#include <TimerB.h>
#include <TimerMacros.h>

///////////////////////////////////////////////////////////////////////////////////////////

#ifndef CALL_TimerBISR
volatile bool    TimerBChanged;     // Set TRUE at each tick
#endif

static struct {
    TIME_T  Seconds;                // Seconds since init
    TIME_T  MS;                     // MS sincs last second
    } TimerB NOINIT;

///////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define PRTIMx      _PRTIM(TIMERB_ID)
#define TIMER_ISR   _TCOMPA_VECT(TIMERB_ID)

#define TCNTx       _TCNT(TIMERB_ID)
#define OCRAx       _OCRA(TIMERB_ID)

#define TCCRAx      _TCCRA(TIMERB_ID)
#define TCCRBx      _TCCRB(TIMERB_ID)
#define TIMSKx      _TIMSK(TIMERB_ID)
#define OCIEAx      _OCIEA(TIMERB_ID)

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// TimerBInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerBInit(void) {

    memset(&TimerB,0,sizeof(TimerB));

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running, with OCRA as top value
    //
    TCCRAx = CTC_MODEB;             // No output compare functions
    TCCRBx = CLOCK_BITSB;           // Set appropriate clock
    OCRAx  = CLOCK_COUNTB;          // And clock count for ticks
    TCNTx  = 0;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerBGetSeconds - Return seconds since TimerBInit()
// TimerBGetMS      - Get milliseconds since previous second
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
TIME_T TimerBGetSeconds(void) {
    TIME_T Rtnval;

    TIMSKx = 0;                     // Disable interrupts
    Rtnval = TimerB.Seconds;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts

    return Rtnval;
    }

TIME_T TimerBGetMS(void) {
    TIME_T Rtnval;

    TIMSKx = 0;                     // Disable interrupts
    Rtnval = TimerB.MS;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts

    return Rtnval;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_COMPA_vect - Compare A causes a timer tick
//
// This defines the "tick" of the system clock. Increment the global
//   clock counter, call the user's function (if defined) and return.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_ISR,ISR_NOBLOCK) {

    TimerB.MS += MS_PER_TICKB;

    if( TimerB.MS >= 1000 ) {
        ++TimerB.Seconds;
        TimerB.MS -= 1000;
        }

    //
    // Call the user's function
    //
#ifdef CALL_TimerBISR
    TimerBISR();
#else
    TimerBChanged = true;
#endif
    }
