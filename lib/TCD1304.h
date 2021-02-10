//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TCD1304.h
//
//  SYNOPSIS
//
//      extern uint8_t TCD1304Data[TCD_BUFSIZE];
//
//      TCD1304Init();                      // Initialize the CCD chip
//
//      TCD1304Read(IntTime);               // Read data into buffer
//
//  DESCRIPTION
//
//      Interface to the TCD1304 linear CCD array
//
//  VERSION:    2016.06.13
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

#ifndef TCD1304_H
#define TCD1304_H

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the chip connections
//
#define TCD_PORT    D               // Use PORTD
#define TCD_GATE    7               // CCD Gate
#define TCD_CLK     6               // CCD Clock
#define TCD_SHUT    5               // CCD Shutter

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define TCD_BUFSIZE 912

extern uint8_t TCD1304Data[TCD_BUFSIZE];

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TCD1304Init - Initialize TCD1304 chip and prepare for reading
//
// Inputs:      None.
//
// Outputs:     None.
//
void TCD1304Init(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TCD1304Read - Read one scanline from TCD1304 chip
//
// Inputs:      Number of NS to integrate signal
//              Ptr to appropriate buffer
//
// Outputs:     None.
//
void TCD1304Read(void);

#endif  // LINEARCCD_H - whole file
