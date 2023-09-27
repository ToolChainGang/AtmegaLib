////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      EEScreen.c
//
//  DESCRIPTION
//
//      EEPROM dump screen
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

#include "EEScreen.h"
#include "Command.h"
#include "VT100.h"
#include "Dump.h"

#ifdef USE_EEPROM_SCREEN

static  int StartDump =     0;
static  int EndDump   = 0x100;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ShowEEScreen - Display the EEPROM screen
//
// Inputs:      None.
//
// Outputs:     None.
//
void ShowEEScreen(void) {

    CursorHome;
    ClearScreen;

    PrintStringP(PSTR("0x"));
    PrintH2(E2END+1);
    PrintStringP(PSTR(" bytes EEPROM[0x"));
    PrintH2(StartDump);
    PrintStringP(PSTR("-0x"));
    PrintH2(EndDump);
    PrintStringP(PSTR("]:\r\n"));

    DumpEEPROM((uint8_t *) StartDump,EndDump -StartDump + 0x10);

    UpdateEEScreen();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UpdateEEScreen - Update the EEPROM dump screen
//
// Inputs:      None.
//
// Outputs:     None.
//
void UpdateEEScreen(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
//    CursorPos(1,FREE_ROW);
//    PrintDebug();

    //
    //
    //
    PlotInput();
    }

#endif // USE_EEPROM_SCREEN
