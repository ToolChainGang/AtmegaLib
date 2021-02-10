//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TimerB.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In TimerB.h
//      //
//      ...Choose a timer                  (Default: Timer1)
//      ...Choose Polled or interrupt mode (Default: Interrupt)
//      ...Choose tick duration            (Default: 25ms)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      TimerBInit();                       // Called once at startup
//          :
//
//      while(1) {
//          while( !TimerBChanged )
//              sleep_cpu();                // Wait for tick to happen
//
//          TimerBChanged = false;          // Reset flag
//              :           :               // Do local updates
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      void TimerBISR(void) {              // Called once every tick
//          ...do all update functions
//          }
//
//
//      TimerBInit();                       // Called once at startup
//          :
//
//      while(1) {
//          sleep_cpu();
//          }
//
//      //////////////////////////////////////
//      //
//      // General - time since reset
//      //
//      TIME_T CurrentSecs = TimerBGetSeconds();    // == Realtime seconds
//      TIME_T CurrentMS   = TimerBGetMS();         // == MS since second
//
//  DESCRIPTION
//
//      TimerB processing
//
//      This is a copy of the "Timer.h" file, but referring to TimerB in all cases.
//
//      This allows programs to have more than one timer running at different speeds.
//
//      See Timer.h for a complete description
//
//  EXAMPLE
//
//      See Timer.h
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

#ifndef TIMERB_H
#define TIMERB_H

#include <Timer.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify a TimerB to use.
//
// See Timer.h for explanation and list of examples.
//
//
#define TIMERB_ID      1                         // use TIMER1
#define MS_PER_TICKB   25
#define CLOCK_BITSB    (1 << WGM12) | (1 << CS12) | (1 << CS10)
#define CTC_MODEB      0
#define CLOCK_COUNTB   391

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call TimerBISR. Undefined (commented out)
//   means don't.
// 
// If you only need the TimerBGetSeconds() or TimerBGetTicks() functions, select
//   polled mode & comment this out.
//
#define CALL_TimerBISR

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data definitions and macros
//
#define SECONDSB(_t_)       ((TIME_T) (((_t_)*1000+MS_PER_TICKB/2)/MS_PER_TICKB) )
#define MINUTESB(_t_)       ((TIME_T) (60*SECONDSB(_t_)) )
#define HOURSB(_t_)         ((TIME_T) (60*MINUTESB(_t_)) )

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerBInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerBInit(void);

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
TIME_T TimerBGetSeconds(void);
TIME_T TimerBGetMS     (void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerBISR - User's timer update routine
//
// Inputs:      None.
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_TimerBISR is #defined, see above.
//
#ifdef CALL_TimerBISR
void TimerBISR(void);
#else
extern volatile bool    TimerBChanged;      // Set TRUE at each tick
#endif

#endif  // TIMERB_H - entire file
