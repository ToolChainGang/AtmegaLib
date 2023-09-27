////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Counter.c
//
//  DESCRIPTION
//
//      Counter processing
//
//      Setup a timer as an event counter
//
//      See Counter.h for an in-depth description
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

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Counter.h"
#include "TimerMacros.h"

#ifndef CALL_CounterHWM_ISR
extern volatile bool    CounterHWM;     // Set TRUE when reached
#endif

#ifndef CALL_CounterOFLO_ISR
extern volatile bool    CounterOFLO;    // Set TRUE when reached
#endif

///////////////////////////////////////////////////////////////////////////////////////////
//
// Be sure that one (and only one) RISING/FALLING is defined
//
#ifdef RISING_EDGE
    #ifdef FALLING_EDGE
        #error "Both RISING and FALLING are defined."
        #endif
    #define EDGE    0x01
    #endif

#ifdef FALLING_EDGE
    #define EDGE    0x00
    #endif

#ifndef EDGE
    #error "One of RISING/FALLING edge is not defined."
    #endif

///////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define TIMER_COMPA_ISR   _TCOMPA_VECT(TIMER_ID)
#define TIMER_OFLO_ISR    _TOVF_VECT(TIMER_ID)

#define TCNTx       _TCNT(TIMER_ID)
#define TCCRAx      _TCCRA(TIMER_ID)
#define TCCRBx      _TCCRB(TIMER_ID)

#define OCRAx       _OCRA(TIMER_ID)





#define PRTIMx      _PRTIM(TIMER_ID)


#define TIMSKx      _TIMSK(TIMER_ID)
#define OCIEAx      _OCIEA(TIMER_ID)

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// TimerInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerInit(void) {

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running, with OCRA as top value
    //
    TCCRAx = 0;                     // Normal counter mode
    TCCRBx = EDGE;                  // Count selected edge
    OCRAx  = CLOCK_COUNT;           // And clock count for ticks
    TCNTx  = 0;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CounterSetHWM - Set the "high water mark"
//
// Inputs:      The HWM value to set
//
// Outputs:     None.
//
void CounterSetHWM(unsigned HWM) { OCRAx = HWM; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CounterGetCount - Return timer count
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
TIME_T CounterGetCount(void) { return(TCNTx); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_COMPA_vect - Timer reaches COMPA
//
// This defines the high-water mark of the timer
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_COMPA_ISR,ISR_NOBLOCK) {

    //
    // Call the user's function
    //
#ifdef CALL_CounterHWM_ISR
    CounterHWM_ISR();
#else
    CounterHWM = true;
#endif
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_OFLO_vect - Timer overflows
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_OFLO_ISR,ISR_NOBLOCK) {

    //
    // Call the user's function
    //
#ifdef CALL_CounterOFLO_ISR
    CounterOFLO_ISR();
#else
    CounterOFLO = true;
#endif
    }
