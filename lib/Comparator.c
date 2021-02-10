//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Comparator.c
//
//  DESCRIPTION
//
//      Comparator processing
//
//      Setup the comparator to timestamp events
//
//      See Comparator.h for an in-depth description
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

#include <avr\io.h>
#include <avr\interrupt.h>

#include <Comparator.h>
#include <PortMacros.h>

#ifndef CALL_ComparatorISR
bool ComparatorHit;
#endif

///////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
// Be sure that one (and only one) RISING/FALLING is defined
//
#if defined RISING_EDGE
    #define EDGE    (_PIN_MASK(ACIS1) | _PIN_MASK(ACIS0))

#elif defined FALLING_EDGE
    #define EDGE    (_PIN_MASK(ACIS1))

#elif defined TOGGLE_EDGE
    #define EDGE    (0)

#else
    #error "One of RISING/FALLING/TOGGLE edge is not defined."
#endif

/////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_BANDGAP
    #define BANDGAP (_PIN_MASK(ACBG))
#else
    #define BANDGAP (0)
#endif

/////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_T1CAPTIRE
    #define T1CAPTURE (_PIN_MASK(ACIC))
#else
    #define T1CAPTURE (0)
#endif

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// ComparatorInit - Initialize comparator system
//
// Inputs:      None.
//
// Outputs:     None.
//
void ComparatorInit(void) {

    _CLR_BIT(PRR,PRADC);            // Powerup the A/D (includes the comparator)

    //
    // Setup the comparator to interrupt based on selected edge
    //
    _CLR_BIT(ADCSRB,ACME);
    ACSR = BANDGAP | _PIN_MASK(ACIE) | T1CAPTURE | EDGE;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ANALOG_COMP_vect - Analog comparator interrupt handler
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(ANALOG_COMP_vect,ISR_NOBLOCK) {

    //
    // Call the user's function
    //
#ifdef CALL_ComparatorISR
    ComparatorISR();
#else
    ComparatorHit = true;
#endif
    }
