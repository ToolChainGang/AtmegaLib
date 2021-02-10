//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      AtoD.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In AtoD.h
//      //
//      #define NUM_ATOD  3
//
//      ...Choose Polled or interrupt mode   (Default: Interrupt)
//      ...Choose continuous or command mode (Default: Continuous)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      AtoDInit();                         // Called once at startup
//          :
//
//      while(1) {
//
//          AtoDStart();                    // Start conversions
//          while( !AtoDComplete() )
//              sleep_cpu();                // Wait for completion
//
//          int Val = AtoD(1);              // Get AtoD value
//              :           :               // Do local updates
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      AtoDInit();                         // Called once at startup
//
//      AtoDStart();                        // Start conversions
//
//      void AtoDISR(void) {                // Called once every tick
//          int Val = AtoD(1);              // Get AtoD value
//          ...do all update functions
//          }
//
//
//  DESCRIPTION
//
//      Simple AtoD processing
//
//  VERSION:    2015.03.15
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

#ifndef ATOD_H
#define ATOD_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify number of AtoD inputs to use.
//
// NOTE: This value can range from 1 .. 9, where the ninth channel represents the
//         internal temperature sensor.
//
#define NUM_ATOD        1

//
// Continuous mode/Command depends on the next definition.
//
// Defined (ie - uncommented) means always start new conversion when previous
//   conversions complete
// 
//#define ContinuousAtoD

//
// Polled mode/ISR mode depends on the next definition.
//
// Defined (ie - uncommented) means call AtoDISR. Undefined (commented out)
//   means don't.
// 
//#define CALL_AtoDISR

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AtoDInit - Initialize AtoD system
//
// Inputs:      None.
//
// Outputs:     None.
//
void AtoDInit(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// StartAtoD - Start the AtoD capture
//
// Inputs:      None.
//
// Outputs:     None.
//
// NOTE: Not needed in continuous mode
//
void StartAtoD(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AtoDComplete - Return TRUE if AtoD has completed
//
// Inputs:      None.
//
// Outputs:     None.
//
bool AtoDComplete(void);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AtoD - Get last AtoD value
//
// Inputs:      Index of AtoD value to get
//
// Outputs:     None.
//
uint16_t GetAtoD(uint8_t Index);

#endif  // ATOD_H - entire file
