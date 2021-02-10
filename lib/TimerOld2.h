//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Timer.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Timer.h
//      //
//      ...Choose a timer                  (Default: Timer0)
//      ...Choose Polled or interrupt mode (Default: Interrupt)
//      ...Choose tick duration            (Default: 15ms)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      TimerInit();                        // Called once at startup
//          :
//
//      while(1) {
//          while( !TimerChanged )
//              sleep_cpu();                // Wait for tick to happen
//
//          TimerChanged = false;           // Reset flag
//              :           :               // Do local updates
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      void TimerISR(void) {               // Called once every tick
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
//      Timer processing
//
//      Setup a timer as a real time counter, and to manage periodic events such as
//        LEDs that need to flash at a fixed pace.
//
//      Many programs require timed events. Frequently, you just don't need all
//        the structure and complexity of linked-list scheduling. A simple solution
//        will suffice.
//
//      This code configures a timer as a real-time counter, with user-defined
//        "tick" duration. The interface is in the model of a timer device, which
//        may be polled on an "interrupt flag", or setup as an "ISR callback" every
//        tick.
//
//      Code that needs timed events can define an Update() function to be called
//        from the user's TimerISR(). Knowing the number of ticks in a second, it
//        is thus straightforward for any module to keep track of its own timing.
//
//      Additionally, the time (realtime) since reset may retrieved at any point.
//
//  EXAMPLE
//
//      //
//      // Blink an LED once every 3 seconds
//      //
//      static  TIME_T  LEDTimer;       // Local timer for LEDs
//
//      void TimerISR {
//          if( --LEDTimer > 0 )        // Time to change state?
//              return;                 // Nope - return
//
//          LEDTimer = SECONDS(3)       // Reset LED timer
//
//          _CHG_BIT(LED_PORT,LED_BIT); // Change state of LED
//          }
//
//      //
//      // In main.c
//      //
//      LEDTimer = SECONDS(3);          // Initialize the LED timer
//      InitTimer();                    // Start the Timer
//
//      sei();                          // Enable interrupts
//
//      while(1) {
//          :                           // Do main processing
//          }
//
//  NOTES
//
//      The user's TimerISR() function is called in interrupt context of the
//        timer (but with interrupts enabled).
//
//  VERSION:    2011.11.07
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

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

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
// CLOCK_COUNT is the number of clocks in one MS_PER_TICK at the specified
//   configuration.
//
// CLOCK_COUNT is MS_PER_TICK*F_CPU/SCALE_FACTOR/1000 rounded to the 
//   nearest integer. SCALE_FACTOR is the F_CPU divisor, as specified in
//   the CLOCK_BITS.
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
// For example, for a 15 mS tick on an arduino using timer 0:
//
//   #define TIMER_ID       0                       // use TIMER0
//   #define MS_PER_TICK    15
//   #define CLOCK_BITS     (1 << CS02) | (1 << CS00)
//   #define CTC_MODE       (1 << WGM01)
//   #define CLOCK_COUNT    234
//
// That same example using timer 2:
//
//   #define TIMER_ID       2                       // use TIMER2
//   #define MS_PER_TICK    15
//   #define CLOCK_BITS     (1 << CS22) | (1 << CS21) | (1 << CS20)
//   #define CTC_MODE       (1 << WGM21)
//   #define CLOCK_COUNT    234
//
// Notice the difference in CLOCK_BITS, even though both are 8-bit timers
//   using F_CPU/1024.
//
// The 16-bit timers allow for a larger count. Using timer 1 at 25ms/tick:
//
// #define TIMER_ID       1                         // use TIMER1
// #define MS_PER_TICK    25
// #define CLOCK_BITS     (1 << WGM12) | (1 << CS12) | (1 << CS10)
// #define CTC_MODE       0
// #define CLOCK_COUNT    391
//
// The 16-bit clock can handle a count of 391, more than the 8-bit timer
//   can handle. Also note the 16-bit counter uses different bits for 
//   CTC_MODE, in different registers.
//
// A more accurate 25 mS would be 6250 with an F_CPU/64. This results in
//   exactly 25 mS. The 391 example has roundoff errors.
//
// #define TIMER_ID       1                         // use TIMER1
// #define MS_PER_TICK    25
// #define CLOCK_BITS     (1 << WGM12) | (1 << CS11) | (1 << CS10)
// #define CTC_MODE       0
// #define CLOCK_COUNT    6250
//
#define TIMER_ID    0                       // use TIMER0
#define MS_PER_TICK 15
#define CLOCK_BITS  _PIN_MASK(_CS2(TIMER_ID)) | _PIN_MASK(_CS0(TIMER_ID))
#define CTC_MODE    _PIN_MASK(_WGM1(TIMER_ID))
#define CLOCK_COUNT 234

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call TimerISR. Undefined (commented out)
//   means don't.
// 
// If you only need the TimerGetSeconds() or TimerGetTicks() functions, select
//   polled mode & comment this out.
//
//#define CALL_TimerISR

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

#define TICKS_PER_SEC       (1000/MS_PER_TICK)

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
// TimerGetSeconds - Return seconds since TimerInit()
// TimerGetMS      - Get MS within second
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
TIME_T TimerGetSeconds(void);
TIME_T TimerGetMS(void);

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
#else
#include <stdbool.h>

extern volatile bool    TimerChanged;       // Set TRUE at each tick
#endif

#endif  // TIMER_H - entire file
