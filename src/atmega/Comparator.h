////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Comparator.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Comparator.h
//      //
//      ...Choose an edge (RISING/FALLING?TOGGLE)
//      ...Choose bandgap or AIN0
//      ...Choose whether to trigger input capture
//      ...Choose Polled or interrupt mode (Default: Interrupt)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      ComparatorInit();                   // Called once at startup
//          :
//
//      while(1) {
//          while( !ComparatorHit )
//              sleep_cpu();                // Wait for comparator hit
//
//          ComparatorHit = false;          // Reset flag
//              :           :               // Process comparator hit event
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      void ComparatorISR(void) {          // Called at event
//          ...process comparator event
//          }
//
//
//      ComparatorInit();                   // Called once at startup
//
//      while(1) {
//          sleep_cpu();
//          }
//
//  DESCRIPTION
//
//      Comparator processing
//
//      Setup the comparator, then process hits
//
//  EXAMPLE1
//
//      //
//      // Log the number of events every minute
//      //
//      static  TIME_T  ReportTimer;    // Timer for periodic output
//      static  int     Count;          // Previous count
//
//      void TimerBISR {
//          if( --ReportTimer > 0 )     // Time to report count?
//              return;                 // Nope - return
//
//          ReportTimer = SECONDS(60)   // Reset timer
//
//          PrintD(Count,3);            // => printf("%3d"
//          Count = 0;
//          }
//
//      void ComparatorISR(void) {
//      
//          Count++;
//          }
//
//      ReportTimer = SECONDS(60);      // Initialize the ReportTimer
//      InitTimerB();                   // Start the Timer
//      InitCounter();                  // Initialize the Counter
//
//      sei();                          // Enable interrupts
//
//      Count = 0;
//      while(1) {
//          :                           // Do main processing
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

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <stdbool.h>

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Specify which edge causes interrupt
//
//#define RISING_EDGE
#define FALLING_EDGE
//#define TOGGLE_EDGE

//
// Uncomment this next to use the bandgap instead of AIN0
//
#define USE_BANDGAP

//
// Uncomment this next to cause the event to trigger a T1 input capture
//
//#define USE_T1CAPTURE

//
// Polled mode/ISR mode depends on the next definitions.
//
// Defined (ie - uncommented) means call the ISR. Undefined (commented out)
//   means don't.
// 
#define CALL_ComparatorISR

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// ComparatorInit - Initialize comparator system
//
// Inputs:      None.
//
// Outputs:     None.
//
void ComparatorInit(void);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// ComparatorGetCount - Return events since last ComparatorGetCount()
//
// Inputs:      None.
//
// Outputs:     The value specified.
//
uint32_t ComparatorGetCount(void);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//  ComparatorISR  - User's comparator event handler
//
// Inputs:      None.
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_ComparatorISR is #defined, see above.
//
#ifdef CALL_ComparatorISR
extern void ComparatorISR(void);
#else
extern bool ComparatorHit;
#endif

#endif  // COMPARATOR_H - entire file
