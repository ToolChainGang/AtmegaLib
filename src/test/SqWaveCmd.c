////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SqWaveCmd.c
//
//  SYNOPSIS
//
//      Select a timer to use in SqWave.h.
//
//      Connect a scope to port specified in SqWave.h (ie - OCR1A, PortB.2)
//
//      Compile, load, and run this module. The program will accept frequency commands
//        via the serial port.
//
//      See the HELP_SCREEN definition below for a list of available commands
//
//  DESCRIPTION
//
//      This is a simple AVR program to generate squarewave outputs
//
//      It is used to verify and test the output functionality.
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

#include <string.h>
#include <ctype.h>
#include <stdlib.h> 
#include <stdint.h>

#include "PortMacros.h"
#include "SqWave.h"
#include "UART.h"
#include "Serial.h"
#include "GetLine.h"
#include "Parse.h"
#include "VT100.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Static layout of the help screen
//
#define HELP_SCREEN "\
FR <amt>    Set square wave frequency\r\n\
RE          Reset (stop squarewave)\r\n\
ON          Start square wave\r\n\
OFF         Stop square wave\r\n\
ST          Show status\r\n\
HE          Show this help panel\r\n\
?           Show this help panel\r\n\
"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrintStatus - Print out the SqWave status
//
// Inputs:      None.
//
// Outputs:     None.
//
static void PrintStatus(void) {

    PrintCRLF();

    PrintString("Freq = ");
    PrintD(SqWaveGetFreq(),0);
    PrintCRLF();

    PrintString("Output is ");
    if( !SqWaveIsEnabled() ) PrintString("off");
    else                     PrintString("on");
    PrintCRLF();

    PrintCRLF();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SqWaveCmd - Command the SqWave software
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    SqWaveInit();

    sei();                              // Enable interrupts

    ClearScreen;

    //
    // End of init
    //
    //////////////////////////////////////////////////////////////////////////////////////

    PrintString("SqWave CMD\r\n");
    PrintString("Type '?' for help");
    PrintCRLF();
    PrintStatus();

    GetLineInit();

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        //
        // Process user commands
        //
        ProcessSerialInput(GetUARTByte());
        } 
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SerialCommand - Manage command lines for this program
//
// Inputs:      Command line typed by user
//
// Outputs:     None.
//
void SerialCommand(char *Line) {
    char    *Command;

    ParseInit(Line);
    Command = ParseToken();

    //
    // Universally accept <ESC> as stop the SqWave
    //
    if( StrEQ(Command,ESC_CMD) ) {
        SqWaveEnable(false);
        PrintCRLF();
        PrintString("SqWave off\r\n");
        return;
        }


    //
    // FR - Set SqWave frequency
    //
    if( StrEQ(Command,"FR") ) {
        char *FreqText = ParseToken();

        //
        // If no arguments, print out frequency
        //
        if( FreqText == NULL ||
            strlen(FreqText) == 0 ) {
            PrintString("Freq = ");
            PrintD(SqWaveGetFreq(),0);
            PrintCRLF();
            return;
            }

        int FreqNum = atoi(FreqText);
        //
        // If freq out of range, print error and ignore
        //
        if( FreqNum < SQ_MIN_FREQ ||
            FreqNum > SQ_MAX_FREQ ) {
            PrintString("Freq out of range (");
            PrintString(FreqText);
            PrintString("), must be ");
            PrintD(SQ_MIN_FREQ,0);
            PrintString(" to ");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
            PrintD(SQ_MAX_FREQ,0);
#pragma GCC diagnostic pop

            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }


        SqWaveSetFreq(FreqNum);

        return;
        }


    //
    // ON - Turn squarewave on
    //
    if( StrEQ(Command,"ON") ) {
        SqWaveEnable(1);
        return;
        }


    //
    // OFF - Turn squarewave off
    //
    if( StrEQ(Command,"OFF") ) {
        SqWaveEnable(0);
        return;
        }


    //
    // RE - Reset (stop square wave)
    //
    if( StrEQ(Command,"RE") ) {
        SqWaveEnable(false);
        PrintCRLF();
        PrintString("SqWave off\r\n");
        return;
        }


    //
    // ST - Print status
    //
    if( StrEQ(Command,"ST") ) {
        PrintStatus();
        return;
        }


    /////////////////////////////////////////////////////////////////////////////////////
    //
    // Regular commands follow
    //

    //
    // HE - Help screen
    //
    if( StrEQ(Command,"HE") ||
        StrEQ(Command,"?" ) ) {
        PrintCRLF();
        PrintString(HELP_SCREEN);
        PrintCRLF();
        return;
        }


    //
    // Not a recognized command. Let the user know he goofed.
    //
    PrintStringP(PSTR(BEEP));
    PrintStringP(PSTR("Unrecognized Command \""));
    PrintString (Command);
    PrintStringP(PSTR("\"\r\n"));
    PrintString("Type '?' for help\r\n");
    PrintCRLF();
    }
