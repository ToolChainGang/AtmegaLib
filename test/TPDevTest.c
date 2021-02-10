//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TPDevTest.c
//
//  SYNOPSIS
//
//      Compile, load, and run this module. The Serial port will show raw activity
//        from the TouchPad.
//
//  DESCRIPTION
//
//      This is a simple AVR program to test the touchpad raw interface
//
//      It is used to verify and test the touchpad driver.
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

#include <string.h>
#include <stdbool.h>

#include "UART.h"
#include "Serial.h"
#include "TPDev.h"

TPDEV   PrevTPDev;

int LineCount;

#define ABS(x)           (((x) < 0) ? -(x) : (x))

#define WITHIN(_x_,_y_,_r_)  ( ABS((int) (_x_) - (int) (_y_)) < (_r_) )

#define NOISE   5           // AtoD values +/- this much are considered the same

bool    Printed = false;
#define TAP_LEVEL   750

#ifdef DEBUG
int Debug1, Debug2, Debug3, Debug4;
int PrevDebug1, PrevDebug2, PrevDebug3, PrevDebug4;
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintTPDev - Print out TPDev settings, if they have changed
//
// Inputs:      None.
//
// Outputs:     None.
//
static void PrintTPDev(void) {

    //
    // Print nothing if we're not touching the screen.
    //
    if( TPDev.ZPos < 300 ) {
        Printed = false;
        return;
        }

    if( Printed )
        return;

    Printed = true;

    PrintD(LineCount++,5);
    PrintString(": ");
    PrintD(TPDev.XPos,5);
    PrintString(", ");
    PrintD(TPDev.YPos,5);
    PrintString(", ");
    PrintD(TPDev.ZPos,5);
    PrintCRLF();

    PrevTPDev = TPDev;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SerialTest - Test the Serial port with continuous output & input
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int __attribute__((OS_main)) main(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    TPDevInit();                        // Called once at startup

    sei();                              // Enable interrupts

    LineCount = 0;
    Printed   = false;

    memset(&TPDev,0,sizeof(TPDev));
    PrintString("TPDevTest Reset\r\n"); // String is in RAM

#ifdef DEBUG
    Debug1     = Debug2     = Debug3     = Debug4     = 0;
    PrevDebug1 = PrevDebug2 = PrevDebug3 = PrevDebug4 = 0;
#endif

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        TPDevUpdate();                  // Update touch panel status

        PrintTPDev();

#ifdef DEBUG
        if( PrevDebug1 != Debug1 ||
            PrevDebug2 != Debug2 ||
            PrevDebug3 != Debug3 ||
            PrevDebug4 != Debug4 ) {

            PrevDebug1 = Debug1;
            PrevDebug2 = Debug2;
            PrevDebug3 = Debug3;
            PrevDebug4 = Debug4;

            PrintString("Debug: ");
            PrintD(Debug1,5);
            PrintString(", ");
            PrintD(Debug2,5);
            PrintString(", ");
            PrintD(Debug3,5);
            PrintString(", ");
            PrintD(Debug4,5);
            PrintCRLF();
            }
#endif
        }
    }
