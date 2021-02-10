//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SqWave.c
//
//  DESCRIPTION
//
//      Square wave processing
//
//      Setup a timer as a real time timer, to generate a square wave output
//
//      See SqWave.h for an in-depth description
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

#include <string.h>

#include <avr\io.h>
#include <avr\interrupt.h>

#include <SqWave.h>
#include <TimerMacros.h>

//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define PRTIMx      _PRTIM(SW_TIMER)

#define TCCRAx      _TCCRA(SW_TIMER)
#define TCCRBx      _TCCRB(SW_TIMER)
#define TCNTx       _TCNT(SW_TIMER)

#define OCRAx       _OCRA(SW_TIMER)

#define TIMSKx      _TIMSK(SW_TIMER)

#define COMA0x      _COMA0(SW_TIMER)
#define COMA1x      _COMA1(SW_TIMER)

#if   SQ_SCALE == 1
#   define CTCx        (_PIN_MASK(_CS0(SW_TIMER)))
#elif SQ_SCALE == 8
#   define CTCx        (_PIN_MASK(_CS1(SW_TIMER)))
#elif SQ_SCALE == 64
#   define CTCx        (_PIN_MASK(_CS0(SW_TIMER)) | _PIN_MASK(_CS1(SW_TIMER)))
#elif SQ_SCALE == 256
#   define CTCx        (_PIN_MASK(_CS2(SW_TIMER)))
#elif SQ_SCALE == 1024
#   define CTCx        (_PIN_MASK(_CS2(SW_TIMER)) | _PIN_MASK(_CS1(SW_TIMER)))
#else
#   error "SQ_SCALE not defined!"
#endif

//
// The 16-bit timer uses different mode bits for CTC mode
//
#if SW_TIMER == 1
#   define TCCAVal     (_PIN_MASK(COMA0x))
#   define TCCBVal     (CTCx | _PIN_MASK(_WGM2(SW_TIMER)))
#else
#   define TCCAVal     (_PIN_MASK(COMA0x) | _PIN_MASK(_WGM1(SW_TIMER)))
#   define TCCBVal     (CTCx)
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveInit - Initialize timer system
//
// Inputs:      None.
//
// Outputs:     None.
//
void SqWaveInit(void) {

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running, with OCRA as top value
    //
    TCCRAx = TCCAVal;               // Toggle on compare
    TCCRBx = 0;                     // Clock is stopped for now
    TCNTx  = 0;                     // Reset clock value
    OCRAx  = 0;                     // Freq zero

    _SET_BIT(_DDR(SW_PORT),SW_PIN); // Square Wave is an output
    }


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
void SqWaveEnable(bool Enable) {

    if( !Enable ) TCCRBx = 0;
    else          TCCRBx = TCCBVal;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveIsEnabled - Tell if output is enabled
//
// Inputs:      None.
//
// Outputs:     None.
//
bool SqWaveIsEnabled(void) { return TCCRBx != 0; }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveSetFreq - Set the output frequency
//
// Inputs:      Frequency to set (in Hz)
//
// Outputs:     None.
//
void SqWaveSetFreq(uint16_t Freq) {

    uint16_t CMP = (F_TIMER+(Freq >> 1))/Freq;

    OCRAx = CMP;    
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveGetFreq - Return the output frequency
//
// Inputs:      None.
//
// Outputs:     Frequency currently set (in Hz)
//
uint16_t SqWaveGetFreq() {

    uint16_t    CMP = OCRAx;

    return (F_TIMER+(CMP>>1))/CMP;
    }
