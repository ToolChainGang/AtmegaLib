//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TimerMS.h - Millisecond timer configuration
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In TimerMS.h
//      //
//      ...Choose a timer                  (Default: Timer0)
//      ...Choose Polled or interrupt mode (Default: Interrupt)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      TimerInit();                        // Called once at startup
//          :
//
//      while(1) {
//          while( !TimerUpdate() )
//              sleep_cpu();                // Wait for tick to happen
//
//              :           :               // Do local updates 1/ms
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      void TimerISR(void) {               // Called once every ms
//          ...do all update functions
//          }
//
//
//      TimerInit();                        // Called once at startup
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
//      TIME_T CurrentSecs = TimerGetSeconds(); // == Realtime seconds
//      TIME_T CurrentMS   = TimerGetMS();      // == MS since second
//
//  DESCRIPTION
//
//      Timer with a 1 millisecond update rate.
//
//      See Timer.h for full description.
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

#ifndef TIMERMS_H
#define TIMERMS_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify a timer to use
//
// In addition to the timer, the user needs to define CLOCK_BITS, CLOCK_COUNT,
//   and CTC_MODE specific to the selected timer.
//
// This is because the clock-select bits in TCCRB are *different*, depending
//   on which timer you use. Also, some timers are 8 bits wide and some are
//   16 bits wide, and there's no easy way for the program to tell.
//
// CTC_MODE is the bit combination needed to put the timer in CTC mode, as
//   specified in the datasheet. It's a combination of the WGMxx bits.
//
// CLOCK_BITS is the F_CPU clock divisor you choose, as specified in the
//   TCCRxB register. It's a combination of CSxx bits.
//
// CSx2 CSx1 CSx0
//    0    0    0  No clock source (Timer/Counter stopped).
//    0    0    1  clk I/O /1    (No prescaling)
//    0    1    0  clk I/O /8    (From prescaler)
//    0    1    1  clk I/O /64   (From prescaler)
//    1    0    0  clk I/O /256  (From prescaler)
//    1    0    1  clk I/O /1024 (From prescaler)
//    1    1    0  External clock on Tx pin. Clock on falling edge.
//    1    1    1  External clock on Tx pin. Clock on rising edge.
//
// CLOCK_COUNT is the number of clocks in one cycle of the counter (ie - the
//   OCRA value to use)
//
// TIMER_COUNT is the number of timer OCRA counts that make up 1 tick
//
// For example, a 40 mS tick on an arduino using timer 2:
//
//   #define TIMER_ID        2                           // use TIMER2
//   #define TICKS_PER_SEC  25
//   #define CLOCK_BITS      _PIN_MASK(_CS2(TIMER_ID)) |
//                           _PIN_MASK(_CS1(TIMER_ID)) |
//                           _PIN_MASK(_CS0(TIMER_ID))
//   #define CTC_MODE        _PIN_MASK(_WGM1(TIMER_ID))
//   #define CLOCK_COUNT     125
//   #define TIMER_COUNT     5
//
#define TIMER_ID        1                           // use TIMER1
#define CLOCK_BITS      _PIN_MASK(_CS1(TIMER_ID)) | \
                        _PIN_MASK(_CS0(TIMER_ID))
#define CTC_MODE        _PIN_MASK(_WGM2(TIMER_ID))
#define CLOCK_COUNT     250
#define TIMER_COUNT     1
#define TICKS_PER_SEC   1000

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call TimerISR. Undefined (commented out)
//   means don't.
// 
// If you only need the TimerGetSeconds() or TimerGetTicks() functions, select
//   polled mode & comment this out.
//
#define CALL_TimerISR

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data definitions and macros
//
#define SECONDS(_t_)        ((TIME_T) (((_t_)*1000+MS_PER_TICK/2)/MS_PER_TICK) )
#define MINUTES(_t_)        ((TIME_T) (60*SECONDS(_t_)) )
#define HOURS(_t_)          ((TIME_T) (60*MINUTES(_t_)) )

#define MS_PER_TICK         (1000/TICKS_PER_SEC)

//
// The canonical datatype to be used when dealing with times.
//
// Based on the TICKS_PER_SECOND below, starting at zero this will overflow
//   in 828 days of continuous processing.
//
typedef uint32_t    TIME_T;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerInit(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerUpdate - Update timer, tell if tick has passed
//
// Inputs:      None.
//
// Outputs:     TRUE  if tick has elapsed since last call
//              FALSE otherwise
//
bool TimerUpdate(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerGetSeconds - Return seconds since TimerInit()
// TimerGetMS      - Get MS within second
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
TIME_T      TimerGetSeconds(void);
uint16_t    TimerGetMS(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerISR - User's timer update routine
//
// Inputs:      None.
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_TimerISR is #defined, see above.
//
#ifdef CALL_TimerISR
void TimerISR(void);
#endif

#endif  // TIMERMS_H - entire file
