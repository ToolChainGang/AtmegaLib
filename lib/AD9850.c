//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      AD9850.c
//
//  SYNOPSIS
//
//  DESCRIPTION
//
//      Simple AtoD processing
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

#include <avr/io.h>
#include <string.h>

#include "PortMacros.h"
#include "AD9850.h"
#include "SPIInline.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
// Note: I normally avoid unions, but the generated code for shift/mask the Div
//         value is excessive. The compiler isn't smart enough to optimize bite-wise
//         shifts, so I do it here with a union.
//
static struct {
    uint16_t    Freq;               // Currently set frequency
    union {
        uint32_t    Value;          // Clock divisor, sent to chip
        uint8_t     Bytes[4];       // Referenced as bytes
        } Div;
    bool        IsEnabled;          // TRUE if output is currently enabled
    } AD9850 NOINIT;


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9850Init - Initialize AD9850 system
//
// Inputs:      None.
//
// Outputs:     None.
//
void AD9850Init(void) {

    memset(&AD9850,0,sizeof(AD9850));

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the control outputs
    //
    _SET_BIT( _DDR(AD9850_RESET_PORT),AD9850_RESET_PIN);    // RESET is an output
    _SET_BIT(_PORT(AD9850_RESET_PORT),AD9850_RESET_PIN);    // Set high for now

    _SET_BIT( _DDR(AD9850_FQ_UD_PORT),AD9850_FQ_UD_PIN);    // FQ_UD is an output
    _CLR_BIT(_PORT(AD9850_FQ_UD_PORT),AD9850_FQ_UD_PIN);    // Set low  for now

    //
    // Since the AD9850 runs at 125 MHz, we've already met the minimum RESET time
    //   requirement. Bring the system out of RESET and power down for now.
    //
    _CLR_BIT(_PORT(AD9850_RESET_PORT),AD9850_RESET_PIN);    // Bring out of reset
    
    AD9850Enable(false);
    }

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
void AD9850Enable(bool Enable) { 

    AD9850.IsEnabled = Enable;

#ifndef LSB_FIRST
    SPI_LSB_FIRST;
#endif

    PutSPIByte(AD9850.Div.Bytes[0]);
    PutSPIByte(AD9850.Div.Bytes[1]);
    PutSPIByte(AD9850.Div.Bytes[2]);
    PutSPIByte(AD9850.Div.Bytes[3]);

    if( AD9850.IsEnabled ) { PutSPIByte(AD9850_POWER_UP);   }
    else                   { PutSPIByte(AD9850_POWER_DOWN); }

    AD9850_LOAD;

#ifndef LSB_FIRST
    SPI_MSB_FIRST; 
#endif
    }


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
bool AD9850IsEnabled(void) { return AD9850.IsEnabled; }


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
void AD9850SetFreq(uint16_t Freq) {

    AD9850.Freq = Freq;

    //
    // Per the AD9850 datasheet:
    //
    //   Freq    = (Divisor*CLKIN)/(2^32)
    //   Divisor = Freq*(2^32)/CLKIN
    //
    // CLKIN in our application is 125,000,000
    //
    // (2^32)/CLKIN = (2^26)/(5^9)      (Factor out 2^6 in both numbers)
    //
    // The shift/div below works for frequencies to 32768 without
    //   overflowing 32-bit arithmetic.
    //
    AD9850.Div.Value = Freq;

    //
    // For temporary purposes.
    //
    // The dev board SG3525 chip runs at twice the output speed.
    //
#warning "Doubled frequency"
//    AD9850.Div = (AD9850.Div << 10)/125;
    AD9850.Div.Value = (AD9850.Div.Value << 11)/125;

    AD9850.Div.Value = (AD9850.Div.Value << 10)/125;
    AD9850.Div.Value = (AD9850.Div.Value <<  6)/125;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9850GetFreq - Return current frequency
//
// Inputs:      None
//
// Outputs:     Currently set frequency
//
uint16_t AD9850GetFreq(void) { return AD9850.Freq; }
