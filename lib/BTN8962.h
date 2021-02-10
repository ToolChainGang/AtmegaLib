//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      BTN8962.h - Interface to the BTN8962 Half H-bridge driver
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In BTN8962.h
//      //
//      #define BTN8962_ENB_PORT                D
//      #define BTN8962_ENB_BIT                 7
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      BTN8962Init();                      // Called once at startup
//
//      BTN8962Enable(bool);                // Turn output on/off
//
//  DESCRIPTION
//
//      A simple driver module for the BTN8962 class D amplifier
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

#ifndef BTN8962_H
#define BTN8962_H

#include <stdbool.h>

#include "PortMacros.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the ENABLE output port
//
#define BTN8962_ENB_PORT    D
#define BTN8962_ENB_PIN     7

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define BTN8962_ENB()      _SET_BIT(_PORT(BTN8962_ENB_PORT),BTN8962_ENB_PIN)
#define BTN8962_DIS()      _CLR_BIT(_PORT(BTN8962_ENB_PORT),BTN8962_ENB_PIN)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// BTN8962Init - Initialize the BTN8962 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
#define BTN8962Init {                                                                   \
    _SET_BIT( _DDR(BTN8962_ENB_PORT),BTN8962_ENB_PIN);                                  \
    BTN8962Enable(false);                                                               \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// BTN8962Enable - Enable/Disable the BTN8962 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
#define BTN8962Enable(_enb_) {                                                          \
    if( _enb_ ) { _SET_BIT(_PORT(BTN8962_ENB_PORT),BTN8962_ENB_PIN); }                  \
    else        { _CLR_BIT(_PORT(BTN8962_ENB_PORT),BTN8962_ENB_PIN); }                  \
    }                                                                                   \

#endif  // BTN8962_H - entire file
