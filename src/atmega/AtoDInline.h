////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      AtoDInline.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In AtoDInline.h
//      //
//      ...Choose prescale value           (Default: 150 KHz)
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      AtoDInit();                                 // Called once at startup
//          :
//          
//      uint16_t Val = AtoDRead(7);                 // Read from AtoD channel 7
//
//  DESCRIPTION
//
//      A simple AtoD module using polled inline code.
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

#ifndef AtoDInline_H
#define AtoDInline_H

//
// Convenience macros
//
#define WAIT_ATOD   { while(_BIT_ON(ADCSRA,ADSC)); }    // ADSC cleared when finished
#define START_ATOD  { _SET_BIT(ADCSRA,ADSC); }          // Start the AtoD conversion

#define ADMUX_VAL(_pin_)    (_PIN_MASK(REFS0) + (_pin_))

//
// Prescale to 150 KHz, enable
//
#define ADCSRA_VAL  _PIN_MASK(ADPS2) | _PIN_MASK(ADPS1) | _PIN_MASK(ADPS0) | _PIN_MASK(ADEN)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AtoDInit - Initialize AtoD (inline) reads
//
// This routine initializes the AtoD based on the settings above. Called from
//   init.
//
// Inputs:      None.
//
// Outputs:     None.
//
#define AtoDInit {                                                                      \
    _CLR_BIT(PRR,PRADC);                    /* Power up the ADC         */              \
    ADCSRA = ADCSRA_VAL;                    /* Configure for reads      */              \
    }                                                                                   \


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AtoDRead - Read 1 value from AtoD channel
//
// Inputs:      AtoD channel to read
//              Var  to receive result
//
// Outputs:     None. (Var is set to AtoD result)
//
#define AtoDRead(_Channel_,_Result_) {                                                  \
    ADMUX = ADMUX_VAL(_Channel_);                                                       \
    START_ATOD;                                                                         \
    WAIT_ATOD;                                                                          \
    _Result_ = ADCW;                                                                    \
    }                                                                                   \

#endif // AtoDInline_H - entire file
