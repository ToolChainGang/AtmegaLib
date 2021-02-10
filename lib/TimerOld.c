//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Timer.c
//
//  DESCRIPTION
//
//      Timer processing
//
//      Setup a timer as a real time timer, to manage timed events.
//
//      See Timer.h for an in-depth description
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

#include <Timer.h>
#include <TimerMacros.h>

#ifndef CALL_TimerISR
volatile bool   TimerChanged   
    __attribute__ ((section (".noinit")));;     // Set TRUE at each tick
#endif

static struct {
    TIME_T  Seconds;                            // Seconds since init
    TIME_T  MS;                                 // MS within second
    } Timer NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define PRTIMx      _PRTIM(TIMER_ID)
#define TIMER_ISR   _TCOMPA_VECT(TIMER_ID)

#define TCNTx       _TCNT(TIMER_ID)
#define OCRAx       _OCRA(TIMER_ID)

#define TCCRAx      _TCCRA(TIMER_ID)
#define TCCRBx      _TCCRB(TIMER_ID)
#define TIMSKx      _TIMSK(TIMER_ID)
#define OCIEAx      _OCIEA(TIMER_ID)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerInit(void) {

    memset(&Timer,0,sizeof(Timer));

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running, with OCRA as top value
    //
    TCCRAx = CTC_MODE;              // No output compare functions
    TCCRBx = CLOCK_BITS;            // Set appropriate clock
    OCRAx  = CLOCK_COUNT;           // And clock count for ticks
    TCNTx  = 0;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerGetSeconds - Return seconds since TimerBInit()
// TimerGetMS      - Get milliseconds since previous second
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
TIME_T TimerGetSeconds(void) {
    TIME_T Rtnval;

    TIMSKx = 0;                     // Disable interrupts
    Rtnval = Timer.Seconds;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts

    return Rtnval;
    }

TIME_T TimerGetMS(void) {
    TIME_T Rtnval;

    TIMSKx = 0;                     // Disable interrupts
    Rtnval = Timer.MS;
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

    Timer.MS += MS_PER_TICK;

    if( Timer.MS >= 1000 ) {
        ++Timer.Seconds;
        Timer.MS -= 1000;
        }

    //
    // Call the user's function
    //
#ifdef CALL_TimerISR
    TimerISR();
#else
    TimerChanged = true;
#endif
    }
