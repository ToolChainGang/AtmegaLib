////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Button.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Button.h
//      //
//      ...Choose a port                   (Default: PortD)
//      ...Choose bits within a port       (Default: bits 2 and 3)
//      ...Choose debounce time            (Default: 45 ms)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      _CLR_BIT(MCUCR,PUD);                // Allow I/O pullups
//
//      TimerInit();                        // Needed for updates
//      ButtonInit();                       // Called once at startup
//          :
//
//      while(1) {
//          while( !ButtonChanged )
//              sleep_cpu();                // Wait for change
//
//          ButtonChanged = false;          // Reset flag
//
//          CurrentButtons = ButtonValue;   // Process button change
//              :               :
//          }
//
//      void TimerISR(void) {               // Periodic update
//          UpdateButton(MS_PER_TICK);
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      _CLR_BIT(MCUCR,PUD);                // Allow I/O pullups
//
//      TimerInit();                        // Needed for updates
//      ButtonInit();                       // Called once at startup
//          :
//
//      while(1)
//          sleep_cpu();
//
//      void ButtonISR(uint8_t Buttons) {   // Called at every change
//          ...do all update functions
//          }
//
//      void TimerISR(void) {               // Periodic update
//          UpdateButton(MS_PER_TICK);
//          }
//
//  DESCRIPTION
//
//      Button debounce processing
//
//      This code configures bits of an I/O port as physical switch inputs, with
//        debounce. The interface is in the model of a Button device, which
//        may be polled on an "interrupt flag", or setup as an "ISR callback"
//        every button change event.
//
//      NOTE: This module requires periodic calls to an "update" function, such
//        as provided by the Timer module (see Timer.h).
//
//      NOTE: This module automatically enables the port internal pullup resistor
//        for each button. This must be enabled globally (or not) by the calling
//        program as needed.
//
//      NOTE: When using polled mode, the caller must reset the "ButtonChanged"
//        flag when the event is processed.
//
//  EXAMPLE
//
//      //
//      // Print a message when a door is opened/closed
//      //
//      #define DOOR_SWITCH    0x01         // Bit within port of door switch
//
//      _CLR_BIT(MCUCR,PUD);                // Allow I/O pullups
//
//      TimerInit();                        // Needed for updates
//      ButtonInit();                       // Called once at startup
//          :
//
//      while(1)
//          sleep_cpu();
//
//      void ButtonISR(uint8_t Buttons) {   // Called at every change
//
//          if( _BIT_ON(Buttons,DOOR_SWITCH) )
//              PrintString("Door 1 open.\n");
//
//          if( _BIT_OFF(Buttons,DOOR_SWITCH) )
//              PrintString("Door 1 closed.\n");
//          }
//
//      void TimerISR(void) {               // Periodic update
//          UpdateButton(MS_PER_TICK);
//          }
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

#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <avr/io.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Button connection map - Change this to reflect your button connections
//
// All button inputs must be in a single port, but can be any combination of pins
//   within the port.
//
// This system will enable the internal pullups. To use this feature, the caller
//   must also enable the global flag in MCUCR. Or not, if external pullups are
//   not needed.
//
// Example for ATTiny26, using PortB 0 and 1
//
//      #define BUTTON_PORT B               // Buttons are on PORTB
//      #define BUTTON_MASK 0x03            // ...using lower 2 bits
//      
// Example for ATMega168, using PortD 2 and 3
//
//      #define BUTTON_PORT D               // Buttons are on PORTD
//      #define BUTTON_MASK 0x0C            // ...using bits 2 and 3
//      
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BUTTON_PORT     D                   // Buttons are on PORTB
#define BUTTON_MASK     0x0C                // ...using bits 2 and 3

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Debounce timing - A button input needs to be unchanged for this number of
//                     ms to be considered debounced.
//
#define DEBOUNCE_MS     45                  // Stable for 45 ms ==> debounced

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call ButtonISR. Undefined (commented out)
//   means don't.
// 
#define CALL_ButtonISR

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ButtonISR - User's button update routine
//
// Inputs:      ButtonValue - Current state of button press
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_ButtonISR is #defined, see above.
//
#ifdef CALL_ButtonISR

void ButtonISR(uint8_t ButtonValue);

#else

extern volatile uint8_t ButtonValue;        // Button states
extern volatile bool    ButtonChange;       // TRUE if changed from last state

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ButtonInit - Initialize the button processing
//
// Inputs:      None.
//
// Outputs:     None.
//
void ButtonInit(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ButtonUpdate - Update the button state
//
// Inputs:      Number of ms since last call
//
// Outputs:     None.
//
void ButtonUpdate(uint8_t MS);

#endif  // BUTTON_H - entire file

