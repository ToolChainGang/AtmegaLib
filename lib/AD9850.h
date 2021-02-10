//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      AD9850.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In AD9850.h
//      //
//      ...Choose RESET port and pin        (Default: PORTD.4)
//      ...Choose FQ_UD port and pin        (Default: PORTD.5)
//      
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      AD9850Init();                       // Called once at startup
//          :
//
//      AD9850Enable(bool ON);              // Enable/disable output
//      bool AD9850IsEnabled(bool ON);      // Return whether output is on/off
//
//      AD9850SetFreq(uint16_t Freq);       // Set output frequency
//      uint16_t Freq = AD9850GetFreq();    // Get output frequency
//
//
//  DESCRIPTION
//
//      Simple AD9850 frequency output control
//
//  SYNOPSIS
//
//  DESCRIPTION
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

#ifndef AD9850_H
#define AD9850_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the RESET output port
// Specify the FQ_UD output port
//
#define AD9850_RESET_PORT   D
#define AD9850_RESET_PIN    4

#define AD9850_FQ_UD_PORT   D
#define AD9850_FQ_UD_PIN    5

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
// The last 2 bits of the control word are the command, and all are reserved except 00
//
#define AD9850_PHASE_MASK   0b11111000
#define AD9850_POWER_DOWN   0b00000100
#define AD9850_POWER_UP     0b00000000

//
// Strobe the FQ line to load the new frequency
//
#define AD9850_LOAD {                                                                       \
    _SET_BIT(_PORT(AD9850_FQ_UD_PORT),AD9850_FQ_UD_PIN);                                    \
    _CLR_BIT(_PORT(AD9850_FQ_UD_PORT),AD9850_FQ_UD_PIN);                                    \
    }                                                                                       \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9850Init - Initialize AD9850 system
//
// Inputs:      None.
//
// Outputs:     None.
//
void AD9850Init(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9850Enable - Enable/disable output
//
// Inputs:      TRUE  if output should be enabled
//              FALSE otherwise
//
// Outputs:     None.
//
void AD9850Enable(bool Enable);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9850IsEnabled - Return whether output is enabled
//
// Inputs:      None.
//
// Outputs:     TRUE  if output enabled
//              FALSE otherwise
//
bool AD9850IsEnabled(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9850SetFreq - Set output frequency
//
// Inputs:      Frequency to set
//
// Outputs:     None.
//
// NOTE: This does NOT change the output frequency! A subsequent call to enable
//         output is still required.
//
void AD9850SetFreq(uint16_t Freq);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9850GetFreq - Return current frequency
//
// Inputs:      None
//
// Outputs:     Currently set frequency
//
uint16_t AD9850GetFreq(void);

#endif  // AD9850_H - entire file
