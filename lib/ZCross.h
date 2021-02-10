//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ZCross.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In ZCross.h
//      //
//      ...Choose ZCross port and pin       (Default: PORTD.2)
//      ...Choose polled/interrupt mode     (Default: Interrupt)
//
//      //////////////////////////////////////
//      //
//      // In Main.c, polled mode
//      //
//      ZCrossInit();                       // Called once at startup
//          :
//
//      while(1) {
//          while( !ZCrossSeen )
//              sleep_cpu();                // Wait for tick to happen
//
//          ZCrossSeen = false;
//              :           :               // Do zcross updates
//          }
//
//      //////////////////////////////////////
//      //
//      // In Main.c, Interrupt mode
//      //
//      void ZCrossISR(void) {              // Called once every cross
//          ...do zerocross functions
//          }
//
//      ZCrossInit();                       // Called once at startup
//          :
//
//      while(1) {
//          sleep_cpu();
//          }
//
//  DESCRIPTION
//
//    60-cycle zero cross detection
//
//    The ZCross must be an external interrupt (INT0, INT1, &c)
//
//    Note1: These designations are from the computer's point of view; ie -
//             Xmit is a driven output, and Recv is an input.
//
//    Note2: The system enables the internal pullups, so no external resistors
//             are needed.
//
//    Once initialized, the ZCross var will be set nonzero whenever the AC input
//      signal crosses zero.
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

#ifndef ZCROSS_H
#define ZCROSS_H

#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ZCross pin and port.
//
// MUST be an external interrupt (ie - INT0, INT1, &c)
//
// Example for Atmega168, using INT0
//
//      #define ZCROSS_PORT   D             // ZCross is PORTD.2 (== INT0)
//      #define ZCROSS_PIN    2
//      
//      #define ZCROSS_INT    0             // (ZCross is also INT0)
//
//////////////////////////////////////////////////////////////////////////////////////////
//
// Example for ATTiny26, using PortB 4, 5, 6 and INT0
//
//      #define ZCROSS_PORT   B             // ZCross is PORTD.2 (== INT0)
//      #define ZCROSS_PIN    6
//      
//      #define ZCROSS_INT    0             // (ZCross is also INT0)
//
//////////////////////////////////////////////////////////////////////////////////////////

#define ZCROSS_PORT   D             // ZCross is PORTD.2 (== INT0)
#define ZCROSS_PIN    2
      
#define ZCROSS_INT    0             // (ZCross is also INT0)

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call ZCrossISR. Undefined (commented out)
//   means don't.
// 
#define CALL_ZCrossISR

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ZCrossISR - User's ZCross update routine
//
// Inputs:      ButtonValue - Current state of button press
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_ButtonISR is #defined, see above.
//
#ifdef CALL_ZCrossISR

void ZCrossISR(void);

#else

extern volatile bool    ZCrossSeen;         // TRUE if zero crossing seen

#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// ZCrossInit - Initialize the zero cross logic
//
// Inputs:      None.
//
// Outputs:     None.
//
void ZCrossInit(void);

#endif  // ZCross_H - entire file
