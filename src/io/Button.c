////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Button.c
//
//  DESCRIPTION
//
//      Button processing
//
//      Setup an I/O port for button debounce.
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

#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#include "Button.h"
#include "PortMacros.h"

#ifndef CALL_ButtonISR
volatile uint8_t ButtonValue;       // Button states
volatile bool    ButtonChange;      // TRUE if changed from last state
#endif

static   uint8_t ButtonMirror;      // Mirror of button states
static   uint8_t DebounceTimer;     // MS to go for debounce

#define BUTTON_VALUE        (_PIN(BUTTON_PORT) & BUTTON_MASK)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ButtonInit - Initialize the button interface
//
// Inputs:      None.
//
// Outputs:     None.
//
// Note: The electrical interface is specified by #defines in Button.h. See
//         that file if you need to change the port or pin designations.
//         
// Note: This function should be called at system init, with interrupts
//         disabled.
//
// Note: For internal pullups, caller must enable them globally in the MCUCR.
//
void ButtonInit(void) {

    //
    // Button inputs
    // 
    _CLR_MASK( _DDR(BUTTON_PORT),BUTTON_MASK);  // All buttons are inputs
    _SET_MASK(_PORT(BUTTON_PORT),BUTTON_MASK);  // With internal pullup

    ButtonMirror  = BUTTON_VALUE;
    DebounceTimer = 0;

#ifndef CALL_ButtonISR
    ButtonValue   = ButtonMirror;
    ButtonChange  = false;
#endif
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ButtonUpdate - Update the button state
//
// Inputs:      None.
//
// Outputs:     None.
//
void ButtonUpdate(uint8_t MS) {
    
    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Whenever the buttons are different from the mirror value, reset the
    //   debounce clock
    //
    if( ButtonMirror != BUTTON_VALUE ) {
        ButtonMirror  = BUTTON_VALUE;
        DebounceTimer = DEBOUNCE_MS;
        return;
        }

    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //
    // IDLE - No button changes, nothing going on
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

#ifdef CALL_ButtonISR
    ButtonISR(ButtonMirror);
#else
    ButtonValue  = ButtonMirror;
    ButtonChange = true;
#endif
    }

