///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      AtoDTest.c
//
//  SYNOPSIS
//
//  DESCRIPTION
//
//      A simple AVR program to test the AtoD system
//
//  VERSION:    2015.02.28
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

#include <avr/interrupt.h>
#include <util/delay.h>

#include "PortMacros.h"

#include "AtoD.h"
#include "UART.h"
#include "VT100.h"
#include "Serial.h"


//
// Static layout of the AtoDTest main screen
//
static const prog_char AtoDScreenText[] = "\
Chan 0:  --- | Chan 4:  ---\r\n\
Chan 1:  --- | Chan 5:  ---\r\n\
Chan 2:  --- | Chan 6:  ---\r\n\
Chan 3:  --- | Chan 7:  ---\r\n\
-------------+-------------\r\n\
\r\n\
Temp:    ---\r\n\
";

#define AD_COL1     8
#define AD_COL2    23

#define TEMP_ROW    7
#define TEMP_COL   10

#ifdef DEBUG
#define DEBUG_ROW   9

int Debug1;
int PrevDebug1;
int Debug2;
int PrevDebug2;
#endif

#define FREE_ROW   11

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// UpdateAtoDScreen - Display the AtoD screen
//
// Inputs:      None.
//
// Outputs:     None.
//
void UpdateAtoDScreen(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Screen-specific display fields
    //
#if NUM_ATOD > 0
    CursorPos(AD_COL1,1);
    PrintD(GetAtoD(0),5);
#endif

#if NUM_ATOD > 1
    CursorPos(AD_COL1,2);
    PrintD(GetAtoD(1),5);
#endif

#if NUM_ATOD > 2
    CursorPos(AD_COL1,3);
    PrintD(GetAtoD(2),5);
#endif

#if NUM_ATOD > 3
    CursorPos(AD_COL1,4);
    PrintD(GetAtoD(3),5);
#endif

    //////////////////////////////////////////////////////////////////////////////////////

#if NUM_ATOD > 4
    CursorPos(AD_COL2,1);
    PrintD(GetAtoD(4),5);
#endif

#if NUM_ATOD > 5
    CursorPos(AD_COL2,2);
    PrintD(GetAtoD(5),5);
#endif

#if NUM_ATOD > 6
    CursorPos(AD_COL2,3);
    PrintD(GetAtoD(6),5);
#endif

#if NUM_ATOD > 7
    CursorPos(AD_COL2,4);
    PrintD(GetAtoD(7),5);
#endif

    //////////////////////////////////////////////////////////////////////////////////////

#if NUM_ATOD > 8
    CursorPos(TEMP_COL,TEMP_ROW);
    PrintD(GetAtoD(7),5);
#endif

#ifdef DEBUG
    CursorPos(1,DEBUG_ROW);
    PrintCRLF();
    PrintString("Debug1: ");
    PrintH(PrevDebug1);
    PrintCRLF();
    PrintString("Debug2: ");
    PrintH(PrevDebug2);
#endif

    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////////////

    CursorPos(1,FREE_ROW);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AtoDTest - Test the AtoD system
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int __attribute__((OS_main))  main(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    AtoDInit();

    sei();                              // Enable interrupts

#ifdef DEBUG
    Debug1 = 0;
    Debug2 = 0;
#endif

    CursorHome;
    ClearScreen;
    PrintStringP(AtoDScreenText);

    UpdateAtoDScreen();

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {

        //
        // Do other system maintenance
        //
        StartAtoD();
        while( !AtoDComplete() );

        UpdateAtoDScreen();
        _delay_ms(1000);

#ifdef DEBUG
        if( Debug1 != PrevDebug1 ) PrevDebug1 = Debug1;
        if( Debug2 != PrevDebug2 ) PrevDebug2 = Debug2;
#endif
        }
    }
