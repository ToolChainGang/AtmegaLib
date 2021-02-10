//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Counter.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Counter.h
//      //
//      ...Choose a timer                  (Default: Timer0)
//      ...Choose Polled or interrupt mode (Default: Interrupt)
//      ...Choose rising/falling edge      (Default: Rising)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      CounterInit();                      // Called once at startup
//      CounterSetHWM(200);                 // Set high-water mark
//          :
//
//      while(1) {
//          while( !CounterHWM )
//              sleep_cpu();                // Wait for High-Water-Mark
//
//          CounterHWM = false;             // Reset flag
//              :           :               // Process High-Water event
//
//          while( !CounterOFLO );          // Wait for overflow
//
//          CounterOFLO = false;            // Reset flag
//              :           :               // Process overflow
//
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      void CounterHWM_ISR(void) {         // Called at high-water
//          ...process high-water event
//          }
//
//      void CounterOFLO_ISR(void) {        // Called at overflow
//          ...process overflow event
//          }
//
//
//      CounterInit();                      // Called once at startup
//      CounterSetHWM(200);                 // Set high-water mark
//          :
//
//      while(1) {
//          sleep_cpu();
//          }
//
//      //////////////////////////////////////
//      //
//      // General - Counter management
//      //
//      CurrentCount = CounterGetCount();   // Get current count
//      TimerSetCount(0);                   // Reset count to zero
//      CounterSetHWM(200);                 // Set high-water mark
//
//  DESCRIPTION
//
//      Counter processing
//
//      Setup a timer as a counter, which counts external events.
//
//      This code configures a timer as an external event counter, with optional
//        high-water mark. The interface is in the model of a counter device, which
//        may be polled on an "interrupt flag", or setup as an "ISR callback" when
//        the high water mark is reached, or when the timer overflows.
//
//  EXAMPLE1
//
//      //
//      // Log the number of events every minute
//      //
//      static  TIME_T  ReportTimer;    // Timer for periodic output
//      static  uint8_t PrevCount;      // Previous count
//
//      void TimerBISR {
//          if( --ReportTimer > 0 )     // Time to report count?
//              return;                 // Nope - return
//
//          ReportTimer = SECONDS(60)   // Reset timer
//
//          uint8_t NewCount = Timer.GetCount();
//          PrintD(NewCount-PrevCount,3);   // => printf("%3d"
//          PrevCount = NewCount;
//          }
//
//      ReportTimer = SECONDS(60);      // Initialize the ReportTimer
//      InitTimerB();                   // Start the Timer
//      InitCounter();                  // Initialize the Counter
//
//      sei();                          // Enable interrupts
//
//      while(1) {
//          :                           // Do main processing
//          }
//
//  EXAMPLE2
//
//      //
//      // Print an alarm if more than 200 events per minute
//      //
//      static  TIME_T  AlarmTimer;     // Timer for alarm
//
//      void TimerBISR {
//          if( --ReportTimer > 0 )     // Time to reset alarm?
//              return;                 // Nope - return
//
//          ReportTimer = SECONDS(60)   // Reset timer
//
//          CounterSetCount(0);         // Reset alarm counter
//          }
//
//      void CounterHWM_ISR(void) {
//          void PrintString("200 Events/Min Exceeded!\r\n");
//          }
//
//      //
//      // In main.c
//      //
//      AlarmTimer = SECONDS(60);       // Initialize the AlarmTimer
//      InitTimerB();                   // Start the Timer
//      InitCounter();                  // Initialize the Counter
//      CounterSetHWM(200);             // Set the alarm mark
//
//      sei();                          // Enable interrupts
//
//      while(1) {
//          :                           // Do main processing
//          }
//
//  NOTES
//
//      The user's CounterXXX_ISR() function is called in interrupt context of
//        the timer (but with interrupts enabled).
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

#ifndef COUNTER_H
#define COUNTER_H

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Specify a timer to use
//
// If RISING_EDGEs are to be counted, uncomment the RISING_EDGE definition.
//   Otherwise, uncomment the FALLING_EDGE definition.
//
#define COUNTER_ID   0                       // use TIMER0

#define RISING_EDGE  1                       // Count rising edges
//#define FALLING_EDGE 1

//
// Polled mode/ISR mode depends on the next definitions.
//
// Defined (ie - uncommented) means call the ISR. Undefined (commented out)
//   means don't.
// 
// If you only need the CounterGetCount(), use polled mode & comment this out.
//
//
#define CALL_CounterHWM_ISR
#define CALL_CounterOFLO_ISR

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// CounterInit - Initialize counter system
//
// Inputs:      None.
//
// Outputs:     None.
//
void CounterInit(void);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// CounterGetCount - Return events since CounterInit()
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
TIME_T CounterGetCount(void);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//  CounterHWM_ISR  - User's "High Water Mark" update routine
//  CounterOFLO_ISR - User's "Overflow" update routine
//
// Inputs:      None.
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_CounterXXX_ISR is #defined, see above.
//
#ifdef CALL_CounterHWM_ISR
void CounterHWM_ISR(void);
#else 
extern volatile bool    CounterHWM;     // Set TRUE when reached
#endif

#ifdef CALL_CounterOFLO_ISR
void CounterOFLO_ISR(void);
#else 
extern volatile bool    CounterOFLO;    // Set TRUE when reached
#endif

#endif  // COUNTER_H - entire file
