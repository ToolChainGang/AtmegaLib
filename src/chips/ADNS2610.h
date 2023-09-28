////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2013 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ADNS2610.h
//
//  SYNOPSIS
//
//      Connect:    ADNS-SCK  to SCK    (== PortB.5)
//                  ADNS-SDIO to MOSI   (== PortB.3)
//            *and* ADNS-SDIO to MISO   (== PortB.4)
//
//      //////////////////////////////////////
//      //
//      // In ADNS2610.h
//      //
//      ...Choose a speed                  (Default: FOsc/8 == 2 MHz)
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      ADNS2610Init();                     // Called once at startup
//
//      PutADNS2610Byte('A');               // Blocks until complete
//
//      uint8_t InChar = GetUARTByte();     // Blocks until complete
//
//  DESCRIPTION
//
//      A simple driver module for blocking communications with ADNS2610
//        optical flow sensors on an AVR processor.
//
//      The baud rate is set in the ADNS2610.h file.
//
//  NOTE
//
//      Due to the speed of the ADNS chip (2 MHz) and time needed to take an
//        interrupt on an ATMega (7 cycles, plus register saves and restores),
//        this module is implemented as a BLOCKING transciever.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

#ifndef ADNS2610_H
#define ADNS2610_H

#include <stdint.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Choose the speed based on the table in the hardware manual
//
#ifndef ADNS2610_SPEED
#define ADNS2610_SPEED       (1 << SPR0)        // == 1 MHz serial clock
#endif

#define ADNS26102X                              // 1MHz => 2MHz clock

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
    ADNS2610_Config = 0,   // 0x00 Configuration 0x00 Reset, Power Down, Forced Awake, etc
    ADNS2610_Status,       // 0x01 Product ID, Mouse state of Asleep or Awake
    ADNS2610_DeltaY,       // 0x02 Delta_Y Y Movement
    ADNS2610_DeltaX,       // 0x03 Delta_X X Movement
    ADNS2610_SQUAL ,       // 0x04 Measure of the number of features visible by the sensor
    ADNS2610_MaxPx ,       // 0x05 Maximum_Pixel
    ADNS2610_MinPx ,       // 0x06 Minimum_Pixel
    ADNS2610_PxSum ,       // 0x07 Pixel_Sum 
    ADNS2610_Data  ,       // 0x08 Pixel Data Actual picture of surface
    ADNS2610_ShutUp,       // 0x09 Shutter_Upper
    ADNS2610_ShutLw,       // 0x0A Shutter_Lower
    ADNS2610_IProd  = 0x11 // 0x11 Inverse Product ID
    } ADNS2610_REG_T;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ADNS2610Init - Initialize ADNS2610
//
// This routine initializes the ADNS2610 based on the settings above. Called from init.
//
// Inputs:      None.
//
// Outputs:     None.
//
void ADNS2610Init(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PutADNS2610Byte - Send one byte out the ADNS2610 port
//
// Send a byte to the ADNS2610 device, and block until complete.
//
// Inputs:      Address of register to write
//              Data for register
//
// Outputs:     None.
//
void PutADNS2610Byte(ADNS2610_REG_T Addr,uint8_t Data);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GetADNS2610Byte - Get one char from the ADNS2610 device
//
// Get a char from the ADNS2610 device, and block until complete.
//
// Inputs:      Address of register to read
//
// Outputs:     Data in register
//
uint8_t GetADNS2610Byte(ADNS2610_REG_T Addr);

#endif // ADNS2610_H - entire file
