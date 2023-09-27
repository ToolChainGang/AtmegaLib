////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
#include <avr/interrupt.h>

#include "ZCross.h"
#include "PortMacros.h"

volatile bool    ZCrossSeen;         // TRUE if zero crossing seen

#define EICRA_MASKx      _JOIN(ISC,ZCROSS_INT)
#define EICRA_MASK       _JOIN(EICRA_MASKx,1)



//(((1 << ISC01)+(1 << ISC00)) << ZCROSS_INT)
//#define EICRA_CTRL      ((             (1 << ISC00)) << ZCROSS_INT)

//#define INT0_BIT(_x_)   _JOIN(INT,_x_)      // == INTx bit #

//
// When debugging, uncomment this next bit to cause a brieg pulse at the
//   specified port/pin at each zero crossing.
//
// This can be shown on a scope, along with the 6-cycle input, and used for
//   timing adjustments.
//
#define DEBUG_ZCROSS

#ifdef DEBUG_ZCROSS
#define ZDEBUG_PORT   D             // Debug output is on PORTD.5
#define ZDEBUG_PIN    5
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ZCrossInit - Initialize the ZCross interface
//
// Inputs:      None.
//
// Outputs:     None.
//
void ZCrossInit(void) {

    _CLR_BIT( _DDR(ZCROSS_PORT),ZCROSS_PIN);    // ZCross is an input
    _CLR_BIT(_PORT(ZCROSS_PORT),ZCROSS_PIN);    // With no internal pullup

    _CLR_BIT(EICRA,1+(2*ZCROSS_INT));           // Any change generates interrupt
    _SET_BIT(EICRA,0+(2*ZCROSS_INT));           // Any change generates interrupt
    _SET_BIT(EIMSK,ZCROSS_INT);

    ZCrossSeen = false;

#ifdef DEBUG
    //
    // Hardware debugging: Toggle the Xmit bit at each zero crossing
    //
    _SET_BIT( _DDR(ZDEBUG_PORT),ZCROSS_PIN);    // ZDEBUG is an output
    _CLR_BIT(_PORT(ZDEBUG_PORT),ZCROSS_PIN);    // Set to zero for now

#endif
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ZCross_vect - Zero cross interrupt processing
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(_VECT(INT,ZCROSS_INT)) {

#ifdef DEBUG
    //
    // Hardware debugging: Toggle the Xmit bit at each zero crossing
    //
    _CHG_BIT(_PORT(ZDEBUG_PORT),ZDEBUG_PIN);    // Toggle bit
    _delay_us(100);                             // Wait a bit
    _CHG_BIT(_PORT(ZDEBUG_PORT),ZDEBUG_PIN);    // Toggle bit

    return;
#endif

#ifdef CALL_ZCrossISR
    ZCrossISR();
#else
    ZCrossSeen = true;
#endif
    }
