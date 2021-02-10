//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Encoder.c
//
//  DESCRIPTION
//
//      Encoder processing
//
//      Setup an I/O port for encoder processing
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

#include <stdint.h>
#include <stdbool.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "PortMacros.h"
#include "RegisterMacros.h"
#include "Encoder.h"

//////////////////////////////////////////////////////////////////////////////////////////

static  ENCODER_T   EncoderPos;         // Current encoder position
static  uint8_t     PrevBits;           // Previous encoder bits

#ifdef ENCODER_USE_MODULUS
static  ENCODER_T   EncoderModulus;     // Max unsigned value for encoder
#endif

#ifdef ENCODER_INDEX
static  ENCODER_T   EncoderIndexPos;    // Position when index flagged
static  bool        SetEncoderAtIndex;  // TRUE if should set encoder value at index
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//
// Some useful macros
//
#define _PCIEx      _PCIE(_PCI_NUM)
#define _PCIMSKx    _PCIMSK(_PCI_NUM)
#define _PCIVECTx   _PCI_VECT(_PCI_NUM)

#define ENABLE_INT  _SET_BIT(PCICR,_PCIEx);     // Enable  PCI interrupts
#define DISABLE_INT _CLR_BIT(PCICR,_PCIEx);     // Disable PCI interrupts

#define ENC_MASK    (0x03 << (ENCODER_SHIFT  ))
#define INDEX_MASK  (0x01 << (ENCODER_SHIFT+2))

//
// List of state transitions for Grey code.
//
// Officially, a transition that makes no change, or which changes 2 bits
//   simultaneously is an error.
//
// The code currently ignores those cases, but it wouldn't be very hard
//   to test for zero in the code (below) and take action
//
static int8_t GreyCode[16] PROGMEM = { 
     0, // 00 -> 00
    -1, // 00 -> 01
     1, // 00 -> 10
     0, // 00 -> 11
     1, // 01 -> 00
     0, // 01 -> 01
     0, // 01 -> 10
    -1, // 01 -> 11
    -1, // 10 -> 00
     0, // 10 -> 01
     0, // 10 -> 10
     1, // 10 -> 11
     0, // 11 -> 00
     1, // 11 -> 01
    -1, // 11 -> 10
     0, // 11 -> 11
     };

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// EncoderInit - Initialize the encoder processing
//
// Inputs:      None.
//
// Outputs:     None.
//
#ifdef ENCODER_USE_MODULUS
void EncoderInit(ENCODER_T Modulus) {
#else
void EncoderInit(void) {
#endif

    //
    // Encoder inputs
    // 
    _CLR_MASK( _DDR(ENCODER_PORT),ENC_MASK);    // A/B lines are inputs

#ifdef ENCODER_NEEDS_PULLUP
    _CLR_BIT(MCUCR,PUD);                        // Allow I/O pullups
    _SET_MASK(_PORT(ENCODER_PORT),ENC_MASK);    // Add internal pullup
#else
    _CLR_MASK(_PORT(ENCODER_PORT),ENC_MASK);    // Without internal pullup
#endif

    EncoderPos     = 0;
    PrevBits       = (_PORT(ENCODER_PORT) >> ENCODER_SHIFT) & 0x03;

#ifdef ENCODER_USE_MODULUS
    EncoderModulus = Modulus;
#endif

    //
    // Enable the pin change interrupt
    //
    _SET_MASK(_PCIMSKx,ENC_MASK);               // A/B lines will interrupt

#ifdef ENCODER_INDEX
    _CLR_MASK( _DDR(ENCODER_PORT),INDEX_MASK);  // Index is input
#ifdef ENCODER_NEEDS_PULLUP
    _SET_MASK(_PORT(ENCODER_PORT),INDEX_MASK);  // Add internal pullup
#else
    _CLR_MASK(_PORT(ENCODER_PORT),INDEX_MASK);  // Without internal pullup
#endif
    EncoderIndexPos   = 0;
    SetEncoderAtIndex = false;

    _SET_MASK(_PCIMSKx,INDEX_MASK);             // Index mark will interrupt
#endif

    ENABLE_INT;                                 // Enable PCI interrupts
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetEncoder - Return encoder position
//
// Inputs:      None.
//
// Outputs:     EncoderValue
//
ENCODER_T GetEncoder(void) {

    //
    // Mutex this access against the interrupt handler
    //
    DISABLE_INT;                                        // Disable PCI interrupts
    ENCODER_T RtnVal = EncoderPos;

#ifdef ENCODER_USE_MODULUS
    while( RtnVal <               0 ) RtnVal += EncoderModulus;
    while( RtnVal >= EncoderModulus ) RtnVal -= EncoderModulus;

    EncoderPos = RtnVal;
#endif

    ENABLE_INT;                                         // Enable PCI interrupts

    return RtnVal;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetEncoderIndex - Return encoder index position
//
// Inputs:      None.
//
// Outputs:     EncoderPos when index flagged
//
ENCODER_T GetEncoderIndex(void) {

    //
    // Mutex this access against the interrupt handler
    //
    DISABLE_INT;                                        // Enable PCI interrupts
    ENCODER_T   RtnVal = EncoderIndexPos;

#ifdef ENCODER_USE_MODULUS
    while( RtnVal <               0 ) RtnVal += EncoderModulus;
    while( RtnVal >= EncoderModulus ) RtnVal -= EncoderModulus;

    EncoderIndexPos = RtnVal;
#endif

    ENABLE_INT;                                         // Enable PCI interrupts

    return RtnVal;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SetEncoder - Set encoder position
//
// Inputs:      EncoderValue - 
//
// Outputs:     None.
//
void SetEncoder(ENCODER_T NewValue) {

    //
    // Mutex this access against the interrupt handler
    //
    DISABLE_INT;                                        // Enable PCI interrupts
    EncoderPos = NewValue;
    ENABLE_INT;                                         // Enable PCI interrupts
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SetEncoderIndex - Set encoder value at next index mark
//
// Inputs:      Encoder pos to set at next index mark
//
// Outputs:     None.
//
#ifdef ENCODER_INDEX
void SetEncoderIndex(ENCODER_T Pos) {

    EncoderIndexPos   = Pos;
    SetEncoderAtIndex = true;
    }
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SIG_PIN_CHANGEx - Handle pin change interrupt
//
// Manage the encoder position, based on which bits have changed
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(_PCIVECTx) {

    //
    // NOTE: Reading the pin port apparently resets it, so in order to handle both
    //         encoder bits and index bit use 1 hardware access (line below) for both.
    //
    uint8_t PinPort = _PIN(ENCODER_PORT);

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Count the encoder pin change
    //
    uint8_t EncoderBits = (PinPort >> ENCODER_SHIFT) & 0x03;

    //
    // Array lookup is *very* fast, no bimbling about with if statements
    //
    EncoderPos += (int8_t) pgm_read_byte(&GreyCode[EncoderBits | (PrevBits << 2)]);

#ifdef CALL_EncoderISR
    EncoderISR(EncoderBits);
#endif

    PrevBits = EncoderBits;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Make note of the index position, if needed
    //
#ifdef ENCODER_INDEX
    if( (PinPort & INDEX_MASK) == 0 )
        return;

    //
    // If SetEncoderAtIndex is set, then set the encoder value when we
    //   reach the index.
    //
    if( SetEncoderAtIndex ) {
        EncoderPos = EncoderIndexPos;
        SetEncoderAtIndex = false;
        return;
        }

    //
    // Otherwise, just note the encoder value seen at index
    //
    EncoderIndexPos = EncoderPos;
#endif
    }
