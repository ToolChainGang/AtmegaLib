//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      PAM8403.h - Interface to the PAM8403 class D amplifier
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In PAM8403.h
//      //
//      #define PAM8403_PORT                    D
//      #define PAM8403_BIT                     7
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      PAM8403Init();                      // Called once at startup
//
//      PAM8403Enable(bool);                // Turn output on/off
//
//  DESCRIPTION
//
//      A simple driver module for the PAM8403 class D amplifier
//
//  NOTE:
//
//      Enable/Disable *must* be accomplished through a buffer amplifier, because
//        the PAM8403 internal pullups are too small for the Arduino to overcome.
//
//  VERSION:    2015.05.18
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

#ifndef PAM8403_H
#define PAM8403_H

#include <stdbool.h>

#include "PortMacros.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the ~SHUTDOWN output port
//
#define PAM8403_ENB_PORT    D
#define PAM8403_ENB_PIN     7

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define PAM8403_ENB()      _SET_BIT(_PORT(PAM8403_ENB_PORT),PAM8403_ENB_PIN)
#define PAM8403_DIS()      _CLR_BIT(_PORT(PAM8403_ENB_PORT),PAM8403_ENB_PIN)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PAM8403Init - Initialize the PAM8403 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
#define PAM8403Init() {                                                                 \
    _SET_BIT( _DDR(PAM8403_ENB_PORT),PAM8403_ENB_PIN);                                  \
    PAM8403Enable(false);                                                               \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PAM8403Enable - Enable/Disable the PAM8403 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
#define PAM8403Enable(_enb_) {                                                          \
    if( _enb_ ) { _SET_BIT(_PORT(PAM8403_ENB_PORT),PAM8403_ENB_PIN); }                  \
    else        { _CLR_BIT(_PORT(PAM8403_ENB_PORT),PAM8403_ENB_PIN); }                  \
    }                                                                                   \

#endif  // PAM8403_H - entire file
