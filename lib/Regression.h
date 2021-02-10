//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Regression.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//
//      RegressionStart();
//
//      for( N = 1; N < NumSamples; N++ ) {
//          // Get samples
//              :           :
//          RegressionData(SampleX,SampleY);
//          }
//
//      RegressionCalc();
//
//      float M = GetSlope();        // Slope
//      float B = GetIntercept();    // Intercept
//
//  DESCRIPTION
//
//      Simple linear regression calculations, using no arrays
//
//      Useful when calibrating a series of steps (ie - an external digital pot
//        controlled by the micro).
//
//  NOTE
//
//      Uses floating point! Will be rather slow, and put a coding load on your build.
//
//  VERSION:    2015.06.27
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

#ifndef REGRESSION_H
#define REGRESSION_H

#include "Timer.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// No user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// RegressionStart - Initialize regression system
//
// Inputs:      None.
//
// Outputs:     None.
//
void RegressionStart(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// RegressionData - Log regression data to calculation
//
// Inputs:      SampleX
//              Sampley
//
// Outputs:     None.
//
void RegressionData(uint16_t SampleX,uint16_t SampleY);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// RegressionCalc - Finish regression calculations
//
// Inputs:      None.
//
// Outputs:     None.
//
void RegressionCalc(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetSlope     - Return linear regression slope
// GetIntercept - Return linear regression intercept
//
// Inputs:      None.
//
// Outputs:     Calculated slope
// Outputs:     Calculated intercept
//
uint16_t GetSlope    (void);
uint16_t GetIntercept(void);


#endif  // REGRESSION_H - entire file
