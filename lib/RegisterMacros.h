//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      RegisterMacros.h
//
//  SYNOPSIS
//
//      //
//      // Bits within registers
//      //
//      #define PCI_NUM 0
//
//      _PCI_EN(_num_)      => PCIE0
//      _PCI_MSK(_num_)     => PCIMSK0
//      _PCI_VECT(_num_)    => SIG_PIN_CHANGE0
//
//  DESCRIPTION
//      
//      These macros allow interface code to depend on #included definitions 
//        for miscellaneous register assignments
//
//      Instead of writing:
//
//          TCCRA0 = (1 << WGM01);
//
//      Use this:
//
//          #define TIMER_ID   0                    // Use Timer 0
//
//          TCCRA(TIMER_ID) = (1 << WGM1(TIMER_ID));
//
//      Now changing the timer assignment will cause code to automatically
//        adjust.
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

#ifndef REGISTERMACROS_H
#define REGISTERMACROS_H

#include <PortMacros.h>

//
// Bits within registers
//
//      #define PCI_NUM 0
//
//      _PCIE(_num_)    => PCIE0
//      _PCIMSK(_num_)  => PCIMSK0
//
#define _PCIE(_num_)    _JOIN(PCIE,_num_)
#define _PCIMSK(_num_)  _JOIN(PCMSK,_num_)


//
// _PCI_VECT - Generate PCI ISR vector
//
// Example:
//
//      #define _PCI_NUM    0                   // use PCI on first port
//
//      ISR(_PCI_VECT(_PCI_NUM)) { ... }        // == PCINT0_vect
//
#define _PCI_VECT(_num_)    _JOIN3(PCINT,_num_,_vect)

#endif  // REGISTERMACROS_H - whole file
