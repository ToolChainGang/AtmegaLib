////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MAX7219.h - SPI Interface to the MAX7219 8x8 LED matrix controller
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In MAX7219.h
//      //
//      #define MAX7219_LOAD_PORT               D
//      #define MAX7219_LOAD_BIT                3
//
//      //////////////////////////////////////
//      //
//      // In Circuit
//      //
//      Connect MAX7219 LOAD (pin 12) to PORT/BIT specified in MAX7219.h
//      Connect MAX7219 DIN  (pin  1) to PortB.3 (MOSI) of CPU
//      Connect MAX7219 SCK  (pin 13) to PortB.5 (SCK)  of CPU
//      
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      SPIInit;
//      MAX7219Init();                      // Called once at startup
//
//      MAX7219DecodeMode(uint8_t);         // Set decode mode
//      MAX7219Digit(Index,Data);           // Send digit data
//      MAX7219Intensity(uint8_t);          // Set intensity
//      MAX7219ScanLimit(uint8_t);          // Set scan limit
//      MAX7219Shutdown(bool);              // Shutdown/startup
//      MAX7219DisplayTest(bool);           // Turn test mode on/off
//
//      //////////////////////////////////////
//      //
//      // System will power up with random data in all registers.
//      //
//      // Suggested startup code:
//      //
//      SPIInit;
//      MAX7219Init();                      // Called once at startup
//
//      MAX7219DisplayTest(false);
//      MAX7219Shutdown(false);
//      MAX7219Intensity(15);
//      MAX7219DecodeMode(MAX7219_NO_DECODE);
//      MAX7219ScanLimit(7);
//      for( uint8_t i=0; i<8; i++ )
//          MAX7219Digit(i,rand() & 0xFF);
//
//  DESCRIPTION
//
//      A simple driver module for the MAX7219 8x8 LED matrix controller
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

#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>
#include <stdbool.h>

#include "PortMacros.h"
#include "SPIInline.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the ENABLE output port
//
#define MAX7219_LOAD_PORT   D
#define MAX7219_LOAD_PIN    7

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX7219_START       _CLR_BIT(_PORT(MAX7219_LOAD_PORT),MAX7219_LOAD_PIN);
#define MAX7219_LOAD        _SET_BIT(_PORT(MAX7219_LOAD_PORT),MAX7219_LOAD_PIN);

//
// Opcodes the chip understands (from datasheet)
//
#define MAX7219_NOP             0
#define MAX7219_DIGIT           1           // Digit offset
#define MAX7219_DECODE_MODE     9
#define MAX7219_INTENSITY      10
#define MAX7219_SCAN_LIMIT     11
#define MAX7219_SHUTDOWN       12
#define MAX7219_DISPLAY_TEST   15

#define MAX7219_NO_DECODE       0
#define MAX7219_CODE_B0         1
#define MAX7219_CODE_B3        15
#define MAX7219_CODE_B_ALL    255


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Init - Initialize the MAX7219 interface
//
// Inputs:      None.
//
// Outputs:     None.
//
#define MAX7219Init {                                                                   \
    _SET_BIT( _DDR(MAX7219_LOAD_PORT),MAX7219_LOAD_PIN);                                \
    _SET_BIT(_PORT(MAX7219_LOAD_PORT),MAX7219_LOAD_PIN);                                \
    }                                                                                   \


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Send - Send 16-bit value to MAX7219
//
// Inputs:      Address to send to
//              Value to send
//
// Outputs:     None.
//
#define MAX7219Send(_Addr_,_Data_) {                                                    \
    MAX7219_START;                                                                      \
    PutSPIByte(_Addr_);                                                                 \
    PutSPIByte(_Data_);                                                                 \
    MAX7219_LOAD;                                                                       \
    }                                                                                   \

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219DecodeMode - Set decode mode
//
// Inputs:      Decode mode to set
//
// Outputs:     None.
//
#define MAX7219DecodeMode(_Mode_) {                                                     \
    MAX7219Send(MAX7219_DECODE_MODE,_Mode_)                                             \
    }                                                                                   \


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Digit - Send digit data
//
// Inputs:      Index of digit (column) t0 send (0 .. 7)
//              Digit (column) data to send
//
// Outputs:     None.
//
#define MAX7219Digit(_Digit_,_Data_) {                                                  \
    MAX7219Send(MAX7219_DIGIT+(_Digit_),_Data_)                                         \
    }                                                                                   \


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Intensity - Set intensity
//
// Inputs:      Intensity (0 .. 15)
//
// Outputs:     None.
//
#define MAX7219Intensity(_Intensity_) {                                                 \
    MAX7219Send(MAX7219_INTENSITY,_Intensity_)                                          \
    }                                                                                   \


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219ScanLimit - Set scan limit
//
// Inputs:      Scan limit (0 .. 7)
//
// Outputs:     None.
//
#define MAX7219ScanLimit(_Limit_) {                                                     \
    MAX7219Send(MAX7219_SCAN_LIMIT,_Limit_)                                             \
    }                                                                                   \


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Shutdown - Shutdown/Run
//
// Inputs:      TRUE  if output should shut down
//              FALSE if normal operation
//
// Outputs:     None.
//
#define MAX7219Shutdown(_Shutdown_) {                                                   \
    MAX7219Send(MAX7219_SHUTDOWN,((_Shutdown_) ? 0 : 1))                                \
    }                                                                                   \


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219DisplayTest - Set display in test mode (all LEDs on)
//
// Inputs:      TRUE  if output should test display
//              FALSE if normal operation
//
// Outputs:     None.
//
#define MAX7219DisplayTest(_Test_) {                                                    \
    MAX7219Send(MAX7219_DISPLAY_TEST,((_Test_) ? 1 : 0))                                \
    }                                                                                   \

#endif  // MAX7219_H - entire file
