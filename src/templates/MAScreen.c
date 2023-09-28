////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MAScreen.c
//
//  DESCRIPTION
//
//      Main screen
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

#include "MAScreen.h"
#include "Serial.h"
#include "VT100.h"
#include "Command.h"

#ifdef USE_MAIN_SCREEN

#include "ADNS2610.h"

#include "PortMacros.h"
#include "Debug.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
// Static layout of the main screen
//
static const PGM_P MAScreenText = "\
Config:      | Status:\r\n\
DeltaX: xxxx | MaxPx :\r\n\
Deltay: xxxx | MinPx :\r\n\
PxSum :      | SQUAL :\r\n\
ShutUp:      | ShutLw:\r\n\
-------------+-------------\r\n\
\r\n\
Total X: \r\n\
Total Y: \r\n\
";

#define MA_COL1      9
#define MA_COL2     24

#define TOTALX_COL  10
#define TOTALY_COL  10
#define TOT_ROW1     8
#define TOT_ROW2     9

#define FREE_ROW     10

static uint16_t TotalX NOINIT;
static uint16_t TotalY NOINIT;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ShowMAScreen - Display the main screen
//
// Inputs:      None.
//
// Outputs:     None.
//
void ShowMAScreen(void) {

    CursorHome;
    ClearScreen;
    PrintStringP(MAScreenText);

    TotalX = 0;
    TotalY = 0;

    UpdateMAScreen();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UpdateMAScreen - Display the main screen
//
// Inputs:      None.
//
// Outputs:     None.
//
void UpdateMAScreen(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Screen-specific display fields
    //
    PutADNSByte(0x00,0x01);             // Continuous (ie - non-sleep) mode

    CursorPos(MA_COL1,1);
    PrintStringP(PSTR("0x"));
    PrintH(GetADNSByte(ADNS_Config));

    uint8_t DeltaX = GetADNSByte(ADNS_DeltaX);
    TotalX += DeltaX;
    CursorPos(MA_COL1,2);
    PrintD(DeltaX,4);

    uint8_t DeltaY = GetADNSByte(ADNS_DeltaY);
    TotalY += DeltaY;
    CursorPos(MA_COL1,3);
    PrintD(DeltaY,4);

    CursorPos(MA_COL1,4);
    PrintD(GetADNSByte(ADNS_PxSum ),4);

    CursorPos(MA_COL1,5);
    PrintD(GetADNSByte(ADNS_ShutUp),4);

    //////////////////////////////////////////////////////////////////////////////////////

    CursorPos(MA_COL2,1);
    PrintStringP(PSTR("0x"));
    PrintH(GetADNSByte(ADNS_Status));

    CursorPos(MA_COL2,2);
    PrintD(GetADNSByte(ADNS_MaxPx  ),4);

    CursorPos(MA_COL2,3);
    PrintD(GetADNSByte(ADNS_MinPx  ),4);

    CursorPos(MA_COL2,4);
    PrintD(GetADNSByte(ADNS_SQUAL  ),4);

    CursorPos(MA_COL2,5);
    PrintD(GetADNSByte(ADNS_ShutLw ),4);

    //////////////////////////////////////////////////////////////////////////////////////

    CursorPos(TOTALX_COL,TOT_ROW1);
    PrintD(TotalX,5);
    CursorPos(TOTALY_COL,TOT_ROW2);
    PrintD(TotalY,5);

//    CursorPos(MA_COL1,4);
//    VAC_LIMIT Limit = VacLimit();
//    PrintStringP(Limit == VAC_NO_LIMIT   ? PSTR("-----") : 
//                 Limit == VAC_FAR_LIMIT  ? PSTR("Far  ") : 
//                 Limit == VAC_FAR_LIMIT1 ? PSTR("Far-1") : 
//                 Limit == VAC_FAR_LIMIT2 ? PSTR("Far-2") : 
//                 Limit == VAC_FAR_LIMIT3 ? PSTR("Far-3") : 
//                 Limit == VAC_FAR_LIMIT4 ? PSTR("Far-4") : 
//                                           PSTR("Near "));
//
//
//    //
//    //
//    //
//    CursorPos(MA_COL2,1);
//    PrintStringP(EngineCold() ? PSTR("Cold") : PSTR(" Hot"));
//
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////////////

    PlotInput();
    }

#endif // USE_MAIN_SCREEN
