////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Encoder.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Encoder.h
//      //
//      ...Choose a port                   (Default: PortA)
//      ...Choose bits within a port       (Default: bits 0 and 1)
//      ...Choose Polled or interrupt mode (Default: Polled)
//      ...Choose encoder resolution       (Default: int16_t)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      EncoderInit();                      // Called once at startup
//          :
//
//      while(1) {
//          sleep_cpu();                    // Wait for changes
//
//          sint16_t Pos = GetEncoder();    // Get new encoder position
//          SetEncoder(0);                  // Set position to zero
//              :           :
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      EncoderInit();                      // Called once at startup
//          :
//
//      while(1) {
//          sleep_cpu();                    // Wait for changes
//
//          ENCODER_T Pos = GetEncoder();   // Get new encoder position
//          SetEncoder(0);                  // Set position to zero
//
//          ENCODER_T Pos = GetEncoderIndex();  // Return pos of index mark
//          SetEncoderIndex(Pos);               // Set encoder pos at next mark transision
//          }
//
//      void EncoderISR(ENCODER_T Pos) {    // Called at every change
//          ...do stuff
//          }
//
//  DESCRIPTION
//
//      Encoder processing
//
//      Keep track of encoder positioning.
//
//      The Index pulse is a 3rd signal that happens once per revolution. If enabled (see
//        below), then whenever the encoder pulse goes high the current encoder value is
//        saved. GetEncoderIndex() then returns the encoder index that was current when the
//        index tripped.
//
//      Alternately, if SetEncoderIndex() is called with an encoder value, then the encoder
//        position is SET when the index mark is seen. This allows a system to keep an
//        absolute position referenced to the index mark, and saved to the EEPROM.
//
//      The Modulus is the maximum unsigned value that the encoder can see, and the system
//        will force the encoder to wrap around to this value. This imposes a slight loss of
//        efficiency in the encoder read method, if enabled.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Encoder configuration - Change this to reflect your connections
//
// Encoder inputs should be in a single port, on adjacent pins. The ENCODER_SHIFT macro
//   will be used to move the bits to the low order position. If the bits are already the
//   low order bits, then this macro can be set to do nothing.
//
// Example for PortA 0 and 1 on Atmega1024P:
//
//      #define ENCODER_PORT    A       // Encoder is on PORTB
//      #define ENCODER_SHIFT   0       // ...shifting is not needed
//      
// Example for PortB 3 and 4 on Atmega328P
//
//      #define ENCODER_PORT    B       // Encoder is on PORTB
//      #define ENCODER_SHIFT   3       // ...shift right 3 bits to examine
//      
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ENCODER_PORT    D               // Encoder is on PORTD
#define ENCODER_SHIFT   1               // ...using bits 1 and 2
#define _PCI_NUM        2               // PCI stack associated with port

//
// Uncomment this next if you want to use the index mark functions (3rd signal on a
//   grey-code encoder, usually pulses once per rev)
//
#define ENCODER_INDEX                      

//
// Uncomment this next if the encoder needs to use the micro pullups (most encoders don't).
//
//#define ENCODER_NEEDS_PULLUP

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call EncoderISR. Undefined (commented out)
//   means don't.
// 
//#define CALL_EncoderISR

//
// Encoder resolution. Usually 16 bits is enough, but it's easy enough to change to
//   int32_t if needed. If ENCODER_USE_MODULUS is NOT set, then you might need 32
//   bits to prevent wrap-around. (At 1000 pulses per rev, 32 revs would wrap around
//   using 16 bits.)
//
#define ENCODER_T int16_t
//#define ENCODER_T int32_t

//
// Uncomment this next to force the encoder values to be within the modulus when read.
//
//#define ENCODER_USE_MODULUS

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EncoderInit - Initialize the encoder processing
//
// Inputs:      Encoder modulus (max unsigned encoder value)
//
// Outputs:     None.
//
#ifdef ENCODER_USE_MODULUS
void EncoderInit(ENCODER_T Modulus);
#else
void EncoderInit(void);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GetEncoder - Return encoder position
//
// Inputs:      None.
//
// Outputs:     EncoderValue
//
ENCODER_T GetEncoder(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SetEncoder - Set encoder position
//
// Inputs:      EncoderValue - 
//
// Outputs:     None.
//
void SetEncoder(ENCODER_T NewValue);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GetEncoderIndex - Return encoder index position
//
// Inputs:      None.
//
// Outputs:     EncoderPos when index flagged
//
#ifdef ENCODER_INDEX
ENCODER_T GetEncoderIndex(void);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SetEncoderIndex - Set encoder value at next index mark
//
// Inputs:      Encoder pos to set at next index mark
//
// Outputs:     None.
//
#ifdef ENCODER_INDEX
void SetEncoderIndex(ENCODER_T Pos);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EncoderISR - User's encoder update routine
//
// Inputs:      EncoderValue - Current encoder value
//
// Outputs:     None.
//
// NOTE: Only defined if CALL_EncoderISR is #defined, see above.
//
#ifdef CALL_EncoderISR

void EncoderISR(uint8_t EncoderBits);

#endif

#endif  // ENCODER_H - entire file
