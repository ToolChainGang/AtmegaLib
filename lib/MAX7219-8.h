//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2018 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MAX7219-8.h - Interface to eight MAX7219 8x8 LED matrix controllers
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In MAX7219-8.h
//      //
//      #define MAX7219_LOAD_PORT               D
//      #define MAX7219_LOAD_BIT                3
//      #define MAX7219_CLK_PORT                D
//      #define MAX7219_CLK_BIT                 4
//
//      #define MAX7219_DATA_PORT               C
//
//      //////////////////////////////////////
//      //
//      // In Circuit
//      //
//      Connect MAX7219 LOAD (pin 12) to PORT/BIT specified in MAX7219-8.h
//      Connect MAX7219 SCK  (pin 13) to PORT/BIT specified in MAX7219-8.h
//
//      Connect MAX7219.0 DIN  (pin  1) to PortA.0
//      Connect MAX7219.1 DIN  (pin  1) to PortA.1
//                  :               :
//      Connect MAX7219.7 DIN  (pin  1) to PortA.7
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      SPIInit;
//      MAX7219Init();                      // Called once at startup
//
//      //
//      // Calls which affect all displays simultaneously:
//      //
//      MAX7219DecodeMode(uint8_t);         // Set decode mode
//      MAX7219Intensity(uint8_t);          // Set intensity
//      MAX7219ScanLimit(uint8_t);          // Set scan limit
//      MAX7219Shutdown(bool);              // Shutdown/startup
//      MAX7219DisplayTest(bool);           // Turn test mode on/off
//
//      uint8_t Data[8];                    // Data to be sent to 8 digit registers
//      MAX7219Digit(Index,Data);           // Send digit data
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
//
//
//  DESCRIPTION
//
//      A simple driver module for controlling eight MAX7219 8x8 LED matrix controllers
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

#ifndef MAX7219_8_H
#define MAX7219_8_H

#include <stdint.h>
#include <stdbool.h>

#include "PortMacros.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the ENABLE output port and CLK for simultaneous transmission
//
#define MAX7219_LOAD_PORT   D
#define MAX7219_LOAD_PIN    3

#define MAX7219_CLK_PORT    D
#define MAX7219_CLK_PIN     4

#define MAX7219_DATA_PORT   C
//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

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

#define _NOP        __asm__ __volatile__ ("nop")

#define MAX_CLK_HI  _SET_BIT(_PORT(MAX7219_LOAD_PORT),MAX7219_LOAD_PIN)
#define MAX_CLK_LO  _CLR_BIT(_PORT(MAX7219_LOAD_PORT),MAX7219_LOAD_PIN)


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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
    _DDR(MAX7219_DATA_PORT)  = 0xFF;                                                    \
    _PORT(MAX7219_DATA_PORT) = 0x00;                                                    \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Put8 - Send 8 parallel bits by SPI
//
// Inputs:      Byte
//
// Outputs:     None.
//
#define MAX7219Put8(_Data_) {                                                           \
    _PORT(MAX7219_DATA_PORT) = _Data_;                                                  \
    MAX_CLK_HI;                                                                         \
    _NOP;                                                                               \
    MAX_CLK_LO;                                                                         \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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
    MAX7219Put8(_Addr_);                                                                \
    MAX7219Put8(_Data_);                                                                \
    MAX7219_LOAD;                                                                       \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219DecodeMode - Set decode mode
//
// Inputs:      Decode mode to set
//
// Outputs:     None.
//
#define MAX7219DecodeMode(_Mode_) {                                                     \
    MAX7219Send(MAX7219_DECODE_MODE,_Mode_);                                            \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Intensity - Set intensity
//
// Inputs:      Intensity (0 .. 15)
//
// Outputs:     None.
//
#define MAX7219Intensity(_Intensity_) {                                                 \
    MAX7219Send(MAX7219_INTENSITY,_Intensity_);                                         \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219ScanLimit - Set scan limit
//
// Inputs:      Scan limit (0 .. 7)
//
// Outputs:     None.
//
#define MAX7219ScanLimit(_Limit_) {                                                     \
    MAX7219Send(MAX7219_SCAN_LIMIT,_Limit_);                                            \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Shutdown - Shutdown/Run
//
// Inputs:      TRUE  if output should shut down
//              FALSE if normal operation
//
// Outputs:     None.
//
#define MAX7219Shutdown(_Shutdown_) {                                                   \
    MAX7219Send(MAX7219_SHUTDOWN,((_Shutdown_) ? 0 : 1));                               \
    }                                                                                   \


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219DisplayTest - Set display in test mode (all LEDs on)
//
// Inputs:      TRUE  if output should test display
//              FALSE if normal operation
//
// Outputs:     None.
//
#define MAX7219DisplayTest(_Test_) {                                                    \
    MAX7219Send(MAX7219_DISPLAY_TEST,((_Test_) ? 1 : 0));                               \
    }                                                                                   \

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MAX7219Digit - Send digit data
//
// Inputs:      Index of digit (column) t0 send (0 .. 7)
//              Ptr to 8 bytes of Digit (column) data to send
//
// Outputs:     None.
//
#define MAX7219Digit(_Digit_,_Data_) {                                                  \
    MAX7219_START;                                                                      \
    MAX7219Put8(_Digit_);                                                               \
    MAX7219Put8((_Data_)[0]);                                                           \
    MAX7219Put8((_Data_)[1]);                                                           \
    MAX7219Put8((_Data_)[2]);                                                           \
    MAX7219Put8((_Data_)[3]);                                                           \
    MAX7219Put8((_Data_)[4]);                                                           \
    MAX7219Put8((_Data_)[5]);                                                           \
    MAX7219Put8((_Data_)[6]);                                                           \
    MAX7219Put8((_Data_)[7]);                                                           \
    MAX7219_LOAD;                                                                       \
    }                                                                                   \

#endif  // MAX7219_8_H - entire file
