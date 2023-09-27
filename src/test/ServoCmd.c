////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      ServoCmd.C
//
//  SYNOPSIS
//
//      Connect a servo to ports specified in Servo.h
//
//      Compile, load, and run this module. The program will accept servo commands
//        via the serial port.
//
//      See the HELP_SCREEN definition below for a list of available commands
//
//  DESCRIPTION
//
//      This is a simple AVR program with servo control.
//
//      It is used to verify and test servo connections.
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
#include "Servo.h"
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

//#define DEBUG

//
// Static layout of the help screen
//
#define HELP_SCREEN "\
SE # <amt>  Set servo direction  (<amt> being 0 -> 254)\r\n\
RE          Reset (stop all servos)\r\n\
ST          Show servo status\r\n\
HE          Show this help panel\r\n\
?           Show this help panel\r\n\
"

#ifdef DEBUG
int Debug1;
int PrevDebug1;
int Debug2;
int PrevDebug2;
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrintStatus - Print out the Servo statuses
//
// Inputs:      None.
//
// Outputs:     None.
//
static void PrintStatus(void) {

    PrintCRLF();
    PrintString("Servos:\r\n");

    for( uint8_t ID = 0; ID < NUM_SERVOS; ID++ ) {
        PrintD(ID,1);
        PrintString(": ");
        PrintD(GetServo(ID),0);
        PrintCRLF();
        }

#ifdef DEBUG
    PrintCRLF();
    PrintString("Debug1: ");
    PrintD(PrevDebug1,0);
    PrintCRLF();
    PrintString("Debug2: ");
    PrintD(PrevDebug2,0);
#endif

    PrintCRLF();
    PrintCRLF();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ServoCmd - Command the servo control software
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
    ServoInit();

    sei();                              // Enable interrupts

    ClearScreen;

#ifdef DEBUG
    Debug1 = 0;
    Debug2 = 0;
#endif

    //
    // End of init
    //
    //////////////////////////////////////////////////////////////////////////////////////

    PrintString("Servo CMD\r\n");
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

#ifdef DEBUG
        if( Debug1 != PrevDebug1 ) {
            PrevDebug1 = Debug1;
            PrintCRLF();
            PrintString("Debug1: ");
            PrintD(PrevDebug1,0);
            PrintCRLF();
            }
        if( Debug2 != PrevDebug2 ) {
            PrevDebug2 = Debug2;
            PrintCRLF();
            PrintString("Debug2: ");
            PrintD(PrevDebug2,0);
            PrintCRLF();
            }
#endif
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
    int     ServoID;
    char    *Command;

    ParseInit(Line);
    Command = ParseToken();

    //
    // Universally accept <ESC> as stop all Servos
    //
    if( StrEQ(Command,ESC_CMD) ) {
        for( ServoID = 0; ServoID < NUM_SERVOS; ServoID++ )
            Servo(ServoID,SERVO_OFF);
        PrintCRLF();
        PrintString("All off\r\n");
        return;
        }


    //
    // SE - Set Servo state
    //
    if( StrEQ(Command,"SE") ) {
        char *ServoText = ParseToken();
        int ServoNum    = *ServoText - '0';

        if( strlen(ServoText) != 1          ||
            ServoNum          < 0           ||
            ServoNum          >= NUM_SERVOS ||
            !isdigit(*ServoText)             ) {
            PrintString("Unrecognized Servo # (");
            PrintString(ServoText);
            PrintString("), must be 0 to ");
            PrintD(NUM_SERVOS,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        char    *AmtText  = ParseToken();
        uint8_t  ServoAmt = atoi(AmtText);

        if( ServoAmt <    0 ||
            ServoAmt >  255 ) {
            PrintString("Unrecognized Servo amt (");
            PrintString(AmtText);
            PrintString("), must be 0 .. 255\r\n");
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        Servo(ServoNum,ServoAmt);
        return;
        }


    //
    // RE - Reset (stop all Servos)
    //
    if( StrEQ(Command,"RE") ) {
        for( uint8_t ID = 0; ID < NUM_SERVOS; ID++ )
            Servo(ID,SERVO_OFF);
        return;
        }


    //
    // ST - Print Servo status
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
