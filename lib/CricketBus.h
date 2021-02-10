//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      CricketBus.h - Interface to the Cricket bus
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In CricketBus.h
//      //
//      #define CRICKET_PORT                D
//      #define CRICKET_BIT                 7
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      CricketBusInit();                   // Called once at startup
//
//      CricketBusPut(0x100,true);          // Send 0x100 as command to cricket bus
//      CricketBusPut(0x203,false);         // Send 0x203 as data (ie - not command) to bus
//
//      //////////////////////////////////////
//      //
//      // Cricket LED specific calls
//      //
//      In the calls below, ID can be zero (for "any"), or 1 or 2 to identify
//        which of two LED displays are connected. See Crickey LED display documentation for
//        how to set the device ID (involves cutting a trace on the board).
//
//      CricketLEDDec(1025,ID)              // Display numeric "1025" on LED display
//      CricketLEDHex(0x1025,ID)            // Display numeric "1025" on LED display
//      CricketLEDBright(7,ID)              // Set brightness level to 7
//
//      CricketLEDPat(a,b,c,d,ID)           // Set segments based on pattern (see notes below)
//
//  DESCRIPTION
//
//      A simple driver module for the cricket bus
//
//  NOTES
//
//      The Cricket bus requires some specific timing that the arduino cannot guarantee.
//        due to interrupt latency. It's also fairly fast, so transmission is done
//        bit-bang with approprtiate wait states.
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

#ifndef CRICKETBUS_H
#define CRICKETBUS_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the cricket bus connection to the system
//
#define CRICKET_BUS_PORT    D
#define CRICKET_BUS_PIN     7

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Cricket bus device IDs
//
// NOTE: Author only has only tested LED display, no guarantee of other constants
//
#define CRICKET_BUS_LED     0x10            // 4-digit LED array
#define CRICKET_BUS_MOTOR   0x12            // Motor control board
#define CRICKET_BUS_RELAY   0x16            // Lamp/Relay board

#define CRICKET_LED_BRIGHT  0x80            // Next two  bytes are brightness (1-7)
#define CRICKET_LED_PAT     0x40            // Next four bytes are LED pattern
#define CRICKET_LED_HEX     0x20            // Next two  bytes show number in hex
#define CRICKET_LED_NUMBER  0x00            // Next two  bytes show number in decimal

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CricketBusInit - Initialize cricket bus interface
//
// This routine initializes the cricket bus based on the settings above. Called from
//   init.
//
// Inputs:      None.
//
// Outputs:     None.
//
void CricketBusInit(void);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// CricketBusPut - Send one byte and command bit out the cricket bus
//
// Send a byte out the serial port, and the command bit.
//
// Inputs:      Byte to send
//              TRUE if this is a command byte
//
// Outputs:     None.
//
void CricketBusPut(uint8_t Byte,bool Command);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// CricketLEDDec - Display one 16-bit number in decimal
//
// Inputs:      Number to display
//              Device ID (0 or 1)
//
// Outputs:     None.
//
#define CricketLEDDec(_Number_,_ID_) {                                                      \
    CricketBusPut(CRICKET_BUS_LED          ,true);                                          \
    CricketBusPut(CRICKET_LED_NUMBER+(_ID_),false);                                         \
    CricketBusPut(((_Number_) >> 8) & 0xFF ,false);                                         \
    CricketBusPut(((_Number_) >> 0) & 0xFF ,false);                                         \
    }

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// CricketLEDHex - Display one 16-bit number in hexadecimal
//
// Inputs:      Number to display
//              Device ID (0 or 1)
//
// Outputs:     None.
//
#define CricketLEDHex(_Number_,_ID_) {                                                      \
    CricketBusPut(CRICKET_BUS_LED          ,true);                                          \
    CricketBusPut(CRICKET_LED_HEX+(_ID_)   ,false);                                         \
    CricketBusPut(((_Number_) >> 8) & 0xFF ,false);                                         \
    CricketBusPut(((_Number_) >> 0) & 0xFF ,false);                                         \
    }

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// CricketLEDBright - Set the display brightness level (1-7)
//
// Inputs:      Brightness level 1-7
//              [Optional] Device ID (0 or 1)
//
// Outputs:     None.
//
#define CricketLEDBright(_Level_,_ID_) {                                                    \
    CricketBusPut(CRICKET_BUS_LED          ,true);                                          \
    CricketBusPut(CRICKET_LED_BRIGHT+(_ID_),false);                                         \
    CricketBusPut(                        0,false);                                         \
    CricketBusPut(         (_Level_) & 0x07,false);                                         \
    }

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// CricketLEDPat - Set the display based on a pattern
//
// Inputs:      Pattern, as four bytes, each bit representing a digit segment
//              [Optional] Device ID (0 or 1)
//
// Outputs:     None.
//
//                   bit 0
//                 ----------
//                |          |
//                |          |
//             5  |          |  1
//                |    6     |
//                 ----------
//                |          |
//                |          |
//             4  |          |  2
//                |    3     |
//                 ----------  
//                              x bit 7
//
#define CricketLEDPat(_Dig1_,_Dig2_,_Dig3_,_Dig4_,_ID_) {                                \
    CricketBusPut(CRICKET_BUS_LED       ,true);                                          \
    CricketBusPut(CRICKET_LED_PAT+(_ID_),false);                                         \
    CricketBusPut(_Dig1_,false);                                                         \
    CricketBusPut(_Dig2_,false);                                                         \
    CricketBusPut(_Dig3_,false);                                                         \
    CricketBusPut(_Dig4_,false);                                                         \
    }

#endif  // CRICKETBUS_H - entire file
