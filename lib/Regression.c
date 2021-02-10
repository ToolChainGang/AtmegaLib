//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Regression.c
//
//  DESCRIPTION
//
//      Simple linear regression calculations, using no arrays
//
//      Useful when calibrating a series of steps (ie - an external digital pot
//        controlled by the micro).
//
//      See Regression.h for usage example
//
//  VERSION:    2015.03.04
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
#include <stdint.h>

#include <PortMacros.h>

#include "Regression.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static struct {
    int32_t     SumX;                   // Sample point totals
    int32_t     SumY;
    int32_t     SumXY;
    int32_t     SumX2;
    int32_t     M;                      // Calculated slope
    int32_t     B;                      // Calculated intercept
    uint16_t    N;
    } Regression NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// RegressionStart - Initialize regression system
//
// Inputs:      None.
//
// Outputs:     None.
//
void RegressionStart(void) {

    memset(&Regression,0,sizeof(Regression));
    }


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
void RegressionData(uint16_t SampleX,uint16_t SampleY) {

    Regression.SumX  += SampleX;
    Regression.SumY  += SampleY;
    Regression.SumXY += SampleX*SampleY;
    Regression.SumX2 += SampleX*SampleX;
    Regression.N++;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// RegressionCalc - Finish regression calculations
//
// Inputs:      None.
//
// Outputs:     None.
//
#include <SerialLong.h>

void RegressionCalc(void) {

PrintLH(Regression.SumX);
PrintCRLF();
PrintLH(Regression.SumY);
PrintCRLF();
PrintLH(Regression.SumXY);
PrintCRLF();
PrintLH(Regression.SumX2);
PrintCRLF();
PrintLH(Regression.N);
PrintCRLF();

    Regression.M = (Regression.N*Regression.SumXY - Regression.SumX*Regression.SumY)/
                   (Regression.N*Regression.SumX2 - Regression.SumX*Regression.SumX);

    Regression.B = (Regression.SumY - Regression.M*Regression.SumX)/Regression.N;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetM - Return linear regression slope
// GetB - Return linear regression intercept
//
// Inputs:      None.
//
// Outputs:     Calculated slope
// Outputs:     Calculated intercept
//
int32_t GetM(void) { return Regression.M; }
int32_t GetB(void) { return Regression.B; }
