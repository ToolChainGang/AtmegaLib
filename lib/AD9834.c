//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2013 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      AD9834.c
//
//  SYNOPSIS
//
//      See AD9834.h for details
//
//  DESCRIPTION
//
//      Simple AD9834 interface
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
#include "AD9834.h"
#include "SPIInline.h"

#ifdef DEBUG
#include "Serial.h"
#include "SerialLong.h"
#endif // DEBUG

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
// Note: I normally avoid unions, but the generated code for shift/mask the Div
//         value is excessive. The compiler isn't smart enough to optimize byte-wise
//         shifts, so I do it here with a union.
//
static struct {
    uint32_t    Freq;               // Currently set frequency
    union {
        uint32_t    Long;           // Clock divisor, sent to chip
        uint16_t    Words[2];       // Referenced as words
        uint8_t     Bytes[4];       // Referenced as bytes
        } Div;
    union {
        uint16_t    Word;
        uint8_t     Bytes[2];
        } FreqLow;                  // Low  order 14 bits of freq
    union {
        uint16_t    Word;
        uint8_t     Bytes[2];
        } FreqHigh;                 // High order 14 bits of freq
    bool        IsEnabled;          // TRUE if output is currently enabled
    } AD9834 NOINIT;


uint64_t    froboz;

//
// See the datasheet for description
//
#define FREQ0       0b0100000000000000      // Frequency register 0
#define FREQ1       0b1000000000000000      // Frequency register 1

#define PHASE0      0b11000000      // Phase     register 0
#define PHASE1      0b11100000      // Phase     register 1

#define UNUSED15    0b10000000      // ...unused
#define UNUSED14    0b01000000      // ...unused
#define B28         0b00100000      // Use full 28-bit registers
#define HLB         0b00010000
#define FSELECT     0b00001000      // Select frequency register
#define PSELECT     0b00000100      // Select phase register
#define UNUSED9     0b00000010      // ...unused
#define RESET       0b00000001      // Reset registers to zero, output is midscale

#define SLEEP       0b10000000
#define SLEEP12     0b01000000
#define OPBITEN     0b00100000
#define UNUSED4     0b00010000      // ...unused
#define DIV2        0b00001000
#define UNUSED2     0b00000100      // ...unused
#define MODE        0b00000010
#define UNUSED0     0b00000001      // ...unused

#define SEND_2BYTES(_x_,_y_) {                                                              \
    _CLR_BIT(_PORT(AD9834_FSYNC_PORT),AD9834_FSYNC_PIN);/* Clr FSYNC for data */            \
    PutSPIByte(_x_);                                    /* Send 1st byte      */            \
    PutSPIByte(_y_);                                    /* Send 2nd byte      */            \
    _SET_BIT(_PORT(AD9834_FSYNC_PORT),AD9834_FSYNC_PIN);/* Set to strobe data */            \
    }                                                                                       \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9834Init - Initialize AD9834 system
//
// Inputs:      None.
//
// Outputs:     None.
//
void AD9834Init(void) {

    memset(&AD9834,0,sizeof(AD9834));

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the control outputs
    //
    _SET_BIT( _DDR(AD9834_FSYNC_PORT),AD9834_FSYNC_PIN);    // FSYNC is an output
    _SET_BIT(_PORT(AD9834_FSYNC_PORT),AD9834_FSYNC_PIN);    // Set high  for now

    AD9834Enable(false);
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9834Enable - Enable/disable output
//
// Inputs:      TRUE  if output should be enabled
//              FALSE otherwise
//
// Outputs:     None.
//
void AD9834Enable(bool Enable) { 

    AD9834.IsEnabled = Enable;

    //
    // The 9834 chip uses an unusual CPOL/CPHA combination. Since it's unusual, we
    //   encapsulate it here, rather than complicate the SPIInline module.
    //
    uint8_t SavedSPCR = SPCR;
    _SET_BIT(SPCR,CPOL);
    _CLR_BIT(SPCR,CPHA);

    SEND_2BYTES(B28 | RESET,0);                                     // Use 28-bit regs, reset

    if( AD9834.IsEnabled ) {
        SEND_2BYTES(AD9834.FreqLow .Bytes[1],AD9834.FreqLow .Bytes[0]); // Low  14 bits frequency
        SEND_2BYTES(AD9834.FreqHigh.Bytes[1],AD9834.FreqHigh.Bytes[0]); // High 14-bits frequency
        SEND_2BYTES(PHASE0,0);                                          // Set phase to zero
        SEND_2BYTES(B28,0);                                             // Undo RESET
        }

    SPCR = SavedSPCR;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9834IsEnabled - Return whether output is enabled
//
// Inputs:      None.
//
// Outputs:     TRUE  if output enabled
//              FALSE otherwise
//
bool AD9834IsEnabled(void) { return AD9834.IsEnabled; }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9834SetFreq - Set output frequency
//
// Inputs:      Frequency to set
//
// Outputs:     None.
//
// NOTE: This does NOT change the output frequency! A subsequent call to enable
//         output is still required.
//
void AD9834SetFreq(uint32_t Freq) {

    //
    // Per the AD9834 datasheet:
    //
    //   Freq    = (Divisor*CLKIN)/(2^28)
    //   Divisor = Freq*(2^28)/CLKIN
    //
    // CLKIN in our application is 75,000,000
    //
    // (2^28)/CLKIN = (2^22)/(3*5^8)      (Factor out 2^6 in both numbers)
    //
    // The shift/div below works without overflowing 32-bit arithmetic.
    //
    AD9834.Div.Long = Freq;

    AD9834.Div.Long = (AD9834.Div.Long << 14)/625;
    AD9834.Div.Long = (AD9834.Div.Long <<  8)/625;

    AD9834.FreqLow .Word = FREQ0 | (AD9834.Div.Words[0] & 0x3FFF);
    AD9834.FreqHigh.Word = FREQ0 | (AD9834.Div.Words[1] << 2) | (AD9834.Div.Bytes[2] >> 6);

#ifdef DEBUG
    PrintString("Freq: ");
    PrintD(Freq,0);
    PrintString(" = ");
    PrintLH(AD9834.Div.Long);
    PrintString(" (");
    PrintLD(AD9834.Div.Long,0);
    PrintChar(')');
    PrintCRLF();
#endif // DEBUG
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD9834GetFreq - Return current frequency
//
// Inputs:      None
//
// Outputs:     Currently set frequency
//
uint32_t AD9834GetFreq(void) { return AD9834.Freq; }
