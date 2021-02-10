//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SqWave.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In SqWave.h
//      //
//      ...Choose a timer                  (Default: Timer0)
//      ...Choose a frequency range
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      SqWaveInit();                       // Called once at startup
//
//      sei();                              // Enable interrupts
//          :
//
//      SqWaveFreq(uint16_t Freq);          // Set output frequency
//
//      SqWaveEnable(true);                 // Turn off/on output
//
//      SqWaveIsEnabled();                  // Tell if output enabled
//
//  DESCRIPTION
//
//      Squarewave output
//
//      Setup a timer as a real time counter, and generate a squarewave output.
//
//  VERSION:    2011.03.20
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

#ifndef SQWAVE_H
#define SQWAVE_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify a timer to use
//
#define SW_TIMER    1                       // use TIMER0

//
// Frequency range estimates
//
// The software restricts the output compare to between 2 and MAXINT-2, depending on the
//   width of timer used.
//
// Based on those numbers and the known widths of Timer0 (8-bit) and Timer1 (16 bit),
//   the software can expect to generate squarewaves in the following ranges:
//
// (Min = F_CPU/2, Max = F_CPU/(either 254 or 65534))
//
// If F_CPU = 16 MHz:
//
//
//      SETTING           MAX FREQ 16        MAX FREQ 8            MIN FREQ
//      -----------     -------------       -----------         -----------
//      SQ_SCALE 1      8000      KHz       31496.06 Hz           122.07 Hz
//      SQ_SCALE 8      1000      KHz        3937.00 Hz            15.26 Hz
//      SQ_SCALE 64      125      KHz         492.12 Hz             1.90 Hz
//      SQ_SCALE 256      31.25   KHz         123.03 Hz             0.47 Hz
//      SQ_SCALE 1024      7.8125 KHz          30.75 Hz
//
//////////////////////////////////////////////////////////////////////////////////////////
//
// Uncomment exactly one of these
//
#define SQ_SCALE      1
//#define SQ_SCALE      8
//#define SQ_SCALE     64
//#define SQ_SCALE      256
//#define SQ_SCALE   1024

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
// These are here (instead of hidden in SqWave.c) for use by the containing
//   program; for frequency limits and such.
//
#define F_TIMER         (F_CPU/(SQ_SCALE*2))        // Speed of timer, in Hz
#define SQ_MAX_FREQ     F_TIMER/2                   // Min reset is 2 timer ticks

//
// The port/bits showing SqWave output
//
#if   SW_TIMER == 0
#   define SW_PORT  D
#   define SW_PIN   6
#   define SQ_MIN_FREQ     (F_TIMER/254)
#elif SW_TIMER == 1
#   define SW_PORT  B
#   define SW_PIN   1
#   define SQ_MIN_FREQ     (F_TIMER/65534)
#elif SW_TIMER == 2
#   define SW_PORT  B
#   define SW_PIN   3
#   define SQ_MIN_FREQ     (F_TIMER/254)
#else
#   error "SW_TIMER is invalid!"
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveInit - Initialize squarewave system
//
// Inputs:      None.
//
// Outputs:     None.
//
void SqWaveInit(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveEnable - Turn the squarewave output on/off
//
// Inputs:      TRUE  if squarewave output is to be generated
//              FALSE if no output is desired
//
// Outputs:     None.
//
void SqWaveEnable(bool Enable);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveIsEnabled - Tell if output is enabled
//
// Inputs:      None.
//
// Outputs:     None.
//
bool SqWaveIsEnabled(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveSetFreq - Set the output frequency
//
// Inputs:      Frequency to set (in Hz)
//
// Outputs:     None.
//
void SqWaveSetFreq(uint16_t Freq);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveGetFreq - Return the output frequency
//
// Inputs:      None.
//
// Outputs:     Frequency currently set (in Hz)
//
uint16_t SqWaveGetFreq(void);


#endif  // SQWAVE_H - entire file
