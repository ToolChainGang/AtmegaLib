//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Limit.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Limit.h
//      //
//      ...Choose a port                   (Default: PortC)
//      ...Choose bits within a port       (Default: bits 0 and 1)
//      ...Choose debounce time            (Default: 45 ms)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      _CLR_BIT(MCUCR,PUD);                // Allow I/O pullups
//
//      TimerInit();                        // Needed for updates
//      LimitInit();                        // Called once at startup
//          :
//
//      while(1) {
//          while( !LimitChanged )
//              sleep_cpu();                // Wait for change
//
//          LimitChanged = false;           // Reset flag
//
//          CurrentLimits = LimitValue;     // Process changes
//              :               :
//          }
//
//      void TimerISR(void) {               // Periodic update
//          UpdateLimit(MS_PER_TICK);
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      _CLR_BIT(MCUCR,PUD);                // Allow I/O pullups
//
//      TimerInit();                        // Needed for updates
//      LimitInit();                        // Called once at startup
//          :
//
//      while(1)
//          sleep_cpu();
//
//      void LimitISR(uint8_t Limits) {     // Called at every change
//          ...do all update functions
//          }
//
//      void TimerISR(void) {               // Periodic update
//          UpdateLimit(MS_PER_TICK);
//          }
//
//  DESCRIPTION
//
//      Limit (ie - limit switch) debounce processing
//
//      This is a copy of the "Button.h" file, but referring to Limit in all cases.
//
//      This allows programs to process more than 8 switches, or have switches
//        attached to more than one port.
//
//      See Button.h for a complete description
//
//  EXAMPLE
//
//      See Button.h
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

#ifndef LIMIT_H
#define LIMIT_H

#include <stdint.h>
#include <avr/io.h>

//////////////////////////////////////////////////////////////////////////////////////////
//
// Limit connection map - Change this to reflect your switch connections
//
// All limit inputs must be in a single port, but can be any combination of pins
//   within the port.
//
// This system will enable the internal pullups. To use this feature, the caller
//   must also enable the global flag in MCUCR. Or not, if external pullups are
//   not needed.
//
// Example for ATTiny26, using PortB 0 and 1
//
//      #define LIMIT_PORT B               // Limits are on PORTB
//      #define LIMIT_MASK 0x03            // ...using lower 2 bits
//      
// Example for ATMega168, using PortD 2 and 3
//
//      #define LIMIT_PORT D               // Limits are on PORTD
//      #define LIMIT_MASK 0x0C            // ...using bits 2 and 3
//      
//////////////////////////////////////////////////////////////////////////////////////////

#define LIMIT_PORT     C                    // Limits are on PORTC
#define LIMIT_MASK     0x03                 // ...using bits 0 and 1


//////////////////////////////////////////////////////////////////////////////////////////
//
// Debounce timing - A limit input needs to be unchanged for this number of
//                     ms to be considered debounced.
//
#define DEBOUNCE_MSL    45                  // Stable for 45 ms ==> debounced

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call LimitISR. Undefined (commented out)
//   means don't.
// 
#define CALL_LimitISR

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LimitISR - User's limit update routine
//
// Inputs:      LimitValue - Current state of switch
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_LimitISR is #defined, see above.
//
#ifdef CALL_LimitISR

void LimitISR(uint8_t LimitValue);

#else

extern volatile uint8_t LimitValue;        // Limit states
extern volatile bool    LimitChange;       // TRUE if changed from last state

#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LimitInit - Initialize the limit processing
//
// Inputs:      None.
//
// Outputs:     None.
//
void LimitInit(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// LimitUpdate - Update the limit state
//
// Inputs:      Number of ms since last call
//
// Outputs:     None.
//
void LimitUpdate(uint8_t MS);

#endif  // LIMIT_H - entire file
