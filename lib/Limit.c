//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Limit.c
//
//  DESCRIPTION
//
//      Limit processing
//
//      Setup an I/O port for limit switch debounce.
//
//  VERSION:    2011.01.10
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

#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#include "Limit.h"
#include "PortMacros.h"

#ifndef CALL_LimitISR
volatile uint8_t LimitValue;        // Limit states
volatile bool    LimitChange;       // TRUE if changed from last state
#endif

static   uint8_t LimitMirror;       // Mirror of switch states
static   uint8_t DebounceTimer;     // MS to go for debounce

#define LIMIT_VALUE        (_PIN(LIMIT_PORT) & LIMIT_MASK)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LimitInit - Initialize the limit switch interface
//
// Inputs:      None.
//
// Outputs:     None.
//
// Note: The electrical interface is specified by #defines in Limit.h. See
//         that file if you need to change the port or pin designations.
//         
// Note: This function should be called at system init, with interrupts
//         disabled.
//
// Note: For internal pullups, caller must enable them globally in the MCUCR.
//
void LimitInit(void) {

    //
    // Limit inputs
    // 
    _CLR_MASK( _DDR(LIMIT_PORT),LIMIT_MASK);  // All switches are inputs
    _SET_MASK(_PORT(LIMIT_PORT),LIMIT_MASK);  // With internal pullup

    LimitMirror  = LIMIT_VALUE;
    DebounceTimer = 0;

#ifndef CALL_LimitISR
    LimitValue   = LimitMirror;
    LimitChange  = false;
#endif
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LimitUpdate - Update the limit switch state
//
// Inputs:      None.
//
// Outputs:     None.
//
void LimitUpdate(uint8_t MS) {
    
    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Whenever the switches are different from the mirror value, reset the
    //   debounce clock
    //
    if( LimitMirror != LIMIT_VALUE ) {
        LimitMirror  = LIMIT_VALUE;
        DebounceTimer = DEBOUNCE_MSL;
        return;
        }

    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //
    // IDLE - No changes, nothing going on
    //
    if( !DebounceTimer )
        return;

    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //
    // DEBOUNCE - If timer hasn't expired, keep waiting.
    //
    if( DebounceTimer > MS ) {
        DebounceTimer -= MS;
        return;
        }

    //
    // Debounce timed out. Report new state and go back to IDLE mode
    //
    DebounceTimer = 0;

#ifdef CALL_LimitISR
    LimitISR(LimitMirror);
#else
    LimitValue  = LimitMirror;
    LimitChange = true;
#endif
    }

