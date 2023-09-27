////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Debug.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//      DebugInit();
//
//      ///////////////////////////////////////////////////////////////////////////
//      //
//      // All done with init, OK to enable interrupts and go
//      // 
//      sei();                              // Enable interrupts
//
//      while(1) {
//
//          if( interesting-state ) {
//              SET_MAX(Debug1,Var1);
//              SET_MAX(Debug2,Var2);
//              SET_MIN(Debug3,Var1);
//              SET_MIN(Debug4,Var2);
//              }
//
//          //
//          // Dump debug values on command
//          //
//          if( Some-Trigger ) 
//              PrintDebug();
//          } 
//
//  DESCRIPTION
//
//      Functions to aid debug
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

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

#define SET_MAX(_v1_,_v2_)      { if( (_v2_) > (_v1_) ) _v1_ = _v2_; }
#define SET_MIN(_v1_,_v2_)      { if( (_v2_) < (_v1_) ) _v1_ = _v2_; }

extern uint16_t Debug1;                 // Utility probes
extern uint16_t Debug2;
extern uint16_t Debug3;
extern uint16_t Debug4;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DebugArray is used for internal data logging
//
#define USE_DEBUG_ARRAY

//
// Size of debug array.
//
// NOTE1: Must be a power of 2
// NOTE2: Due to GCC bug, must also be set in Debug.c
//
#define DEBUG_SIZE  0x80                // Power of 2, and match definition in Debug.c

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CPUCount is used for timing
//
//#define DEBUG_CPU_COUNT

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DEBUG_ARRAY
extern uint16_t DebugIndex;
extern uint16_t DebugArray[DEBUG_SIZE];

#define DEBUG_TRIG_SET      -1
#define DEBUG_TRIG_OFF       0
#define DEBUG_TRIG_ON       (DEBUG_SIZE/2)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DebugInit - Initialize debugging code
//
// Inputs:      None.
//
// Outputs:     None.
//
void DebugInit(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DebugPrint - Print out the debugging vars
//
// Inputs:      None.
//
// Outputs:     None.
//
void DebugPrint(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DEBUG_ARRAY

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DebugLog - Log data value to debugging array
//
// Inputs:      Value to log
//
// Outputs:     None.
//
void DebugLog(uint16_t Value);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DebugTrigger - Trigger the debugging array and stop logging
//
// Inputs:      -1 Trigger is reset, waiting for trigger
//               0 Trigger is complete, data logging is stopped
//              >0 Triggered, waiting for final log values
//
// Outputs:     None.
//
void DebugTrigger(void);


#endif  // USE_DEBUG_ARRAY

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG_CPU_COUNT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DebugCPUCount - Count CPU cycles per call
//
// Inputs:      None.
//
// Outputs:     None.
//
void DebugCPUCount(void);

#else

#define DebugCPUCount
#endif  // DEBUG_CPU_COUNT

#endif  // DEBUG_H - entire file
