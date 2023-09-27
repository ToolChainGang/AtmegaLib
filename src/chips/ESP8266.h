////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ESP8266.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // On circuit board:
//      //
//      Pin D3 (INT1 == Rx) -> ESP8266 Tx (opposite GND pin on ESP8266)
//      Pin D4 (T0   == Tx) -> ESP8266 Rx (opposite VCC pin on ESP8266)
//
//      //////////////////////////////////////
//      //
//      // In ESP8266.h
//      //
//      #define ESP8266_BAUD    9600
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//
//      ESP8266Init();                      // Called once at startup
//
//      char     String1[] = "...";
//      uint8_t  Byte;
//      uint16_t Value;
//
//      ESP8266String(String);       // => printf("%s",String);
//      ESP8266Hex(Byte);            // => printf("%02X",Byte);
//      ESP8266Hex2(Value);          // => printf("%02X",Value);
//
//      char InChar = ESP8266Get();  // == 0 if no chars available
//
//  DESCRIPTION
//
//      A simple serial Rx/Tx driver module for interrupt driven communications
//        to an ESP8266 Wifi card
//
//  NOTES:
//
//      The software uses Timer2. This software can send or receive, but not both
//        at the same time.
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

#ifndef ESP8266_H
#define ESP8266_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ESP8266_BAUD
#define ESP8266_BAUD    9600
#endif

//
// The serial FIFO's must be a power of two long each, since the code
//   uses binary wraparounds to access. This system has NO error detection
//   and NO XON/XOFF processing.
//
#ifndef ESPI_FIFO_SIZE
#define ESPI_FIFO_SIZE  (1 << 3)        // == 8  char Rx FIFO
#endif

#ifndef ESPO_FIFO_SIZE
#define ESPO_FIFO_SIZE  (1 << 6)        // == 64 chars Tx FIFO
#endif

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ESP8266Init - Initialize UART system for ESP8266 communication
//
// This routine initializes the software UART based on the settings above. Called from
//   main. Also enables the Rx interrupts, disables the Tx interrupt and
//   clears the FIFOs.
//
// Inputs:      None.
//
// Outputs:     None.
//
void ESP8266Init(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ESP8266String - Send string to ESP8266
//
// Inputs:      Text string to send
//
// Outputs:     None.
//
void ESP8266String(const char *String);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ESP8266Hex  - Printf byte  with %x format (2 hex chars)
// ESP8266Hex2 - Printf short with %x format (4 hex chars)
//
// Inputs:      Datum to send
//
// Outputs:     None.
//
void PrintHex (uint8_t  Byte);
void PrintHex2(uint16_t Word);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ESP8266Get - Get one char from the ESP8266 device
//
// Get a char from the ESP8266. The interrupt handler already received the
//   character for us, so this just pulls the char out of the receive FIFO.
//
// Inputs:      None.
//
// Outputs:     ASCII char, if one was available
//              NUL   (binary value = 0) if no chars available
//
char ESP8266Get(void);

#endif // ESP8266_H - entire file
