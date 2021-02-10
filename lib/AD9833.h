//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      AD9833.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In AD9833.h
//      //
//      ...Choose FSYNC port and pin        (Default: PORTD.5)
//      
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      AD9833Init();                       // Called once at startup
//          :
//
//      AD9833Enable(bool ON);              // Enable/disable output
//      bool AD9833IsEnabled(bool ON);      // Return whether output is on/off
//
//      AD9833SetFreq(uint16_t Freq);       // Set output frequency
//      uint16_t Freq = AD9833GetFreq();    // Get output frequency
//
//
//  DESCRIPTION
//
//      Simple AD9833 frequency output control
//
//  SYNOPSIS
//
//  DESCRIPTION
//
//  VERSION:    2015.03.15
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

#ifndef AD9833_H
#define AD9833_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the FSYNC output port
//
#define AD9833_FSYNC_PORT   D
#define AD9833_FSYNC_PIN    5

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833Init - Initialize AD9833 system
//
// Inputs:      None.
//
// Outputs:     None.
//
void AD9833Init(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833Enable - Enable/disable output
//
// Inputs:      TRUE  if output should be enabled
//              FALSE otherwise
//
// Outputs:     None.
//
void AD9833Enable(bool Enable);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833IsEnabled - Return whether output is enabled
//
// Inputs:      None.
//
// Outputs:     TRUE  if output enabled
//              FALSE otherwise
//
bool AD9833IsEnabled(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833SetFreq - Set output frequency
//
// Inputs:      Frequency to set
//
// Outputs:     None.
//
// NOTE: This does NOT change the output frequency! A subsequent call to enable
//         output is still required.
//
void AD9833SetFreq(uint16_t Freq);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9833GetFreq - Return current frequency
//
// Inputs:      None
//
// Outputs:     Currently set frequency
//
uint16_t AD9833GetFreq(void);

#endif  // AD9833_H - entire file
