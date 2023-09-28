/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Rajstennaj Barrabas, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ADNSTest.c
//
//  SYNOPSIS
//
//  DESCRIPTION
//
//      A simple AVR program to interface/explore the ADNS-2610 optical flow sensor.
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

#include <avr/interrupt.h>
#include <util/delay.h>

#include "ADNS2610.h"

#include "Timer.h"
#include "UART.h"
#include "Serial.h"

#define UPDATE_MS  1000             // mS of on time between update msgs

static uint16_t TotalX NOINIT;
static uint16_t TotalY NOINIT;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ADNS2610Test - Test the Serial port with continuous output & input
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int main(void) {

    //
    // Initialize the timer and button system
    //
    ADNS2610Init();                     // Initialize optical flow sensor
    UARTInit();                         // For serial I/O

    sei();                              // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintCRLF();
    PrintString("Reset ADNS2610Test.\r\n");
    PrintCRLF();
    PrintString("Move chip over surface to see values change.");
    PrintCRLF();
    PrintCRLF();

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    PutADNS2610Byte(0x00,0x01);             // Continuous (ie - non-sleep) mode

    TotalX = 0;
    TotalY = 0;

    //    ADNS_Config = 0,   // 0x00 Configuration 0x00 Reset, Power Down, Forced Awake, etc
    //    ADNS_Status,       // 0x01 Product ID, Mouse state of Asleep or Awake
    //    ADNS_DeltaY,       // 0x02 Delta_Y Y Movement
    //    ADNS_DeltaX,       // 0x03 Delta_X X Movement
    //    ADNS_SQUAL ,       // 0x04 Measure of the number of features visible by the sensor
    //    ADNS_MaxPx ,       // 0x05 Maximum_Pixel
    //    ADNS_MinPx ,       // 0x06 Minimum_Pixel
    //    ADNS_PxSum ,       // 0x07 Pixel_Sum 
    //    ADNS_Data  ,       // 0x08 Pixel Data Actual picture of surface
    //    ADNS_ShutUp,       // 0x09 Shutter_Upper
    //    ADNS_ShutLw,       // 0x0A Shutter_Lower
    //    ADNS_IProd  = 0x11 // 0x11 Inverse Product ID
    //
    while(1) {
        _delay_ms(UPDATE_MS);                // Wait 1/2 cycle

        uint8_t DeltaX = GetADNS2610Byte(ADNS2610_DeltaX);
        TotalX += DeltaX;

        uint8_t DeltaY = GetADNS2610Byte(ADNS2610_DeltaY);
        TotalY += DeltaY;

        PrintStringP(PSTR("("));
        PrintD(DeltaX,4);
        PrintChar(',');
        PrintD(DeltaY,4);
        PrintStringP(PSTR(")"));

        PrintCRLF();
        }
    }
