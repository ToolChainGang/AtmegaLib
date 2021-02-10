////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MotorPWMCmd.C
//
//  SYNOPSIS
//
//      Connect a DC motor to a driver via ports specified in Motor.h
//
//      Compile, load, and run this module. The programn will accept motor commands
//        via the serial port.
//
//      See the HELP_SCREEN definition below for a list of available commands
//
//  DESCRIPTION
//
//      This is a simple AVR program with PWM H-Bridge motor control.
//
//      It is used to verify and test motor connections.
//
//  VERSION:    2011.11.07
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

#include <string.h>
#include <ctype.h>
#include <stdlib.h> 
#include <stdint.h>

#include "MotorPWM.h"
#include "UART.h"
#include "Serial.h"
#include "GetLine.h"
#include "Parse.h"
#include "VT100.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define DEBUG

//
// Static layout of the help screen
//
#define HELP_SCREEN "\
MO # <pct>  Set motor direction   (<pct> being -100 -> 0 -> 100)\r\n\
RE          Reset (stop all motors)\r\n\
ST          Show motor status\r\n\
HE          Show this help panel\r\n\
?           Show this help panel\r\n\
"

#define BEEP    "\007"
#define ESC_CMD "\033"


#ifdef DEBUG
int Debug1;
int PrevDebug1;
int Debug2;
int PrevDebug2;
#endif


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintStatus - Print out the motor statuses
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
static void PrintStatus(void) {

    PrintCRLF();
    PrintString("Motors:\r\n");

    for( uint8_t ID = 0; ID < NUM_MOTORS; ID++ ) {
        PrintD(ID,1);
        PrintString(": ");
        PrintD(GetMotorPWM(ID),0);
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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MotorPWMCmd - Command the motor control software
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int __attribute__((OS_main))  main(void) {
    int LoopCount;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    MotorPWMInit();

    LoopCount = 0;

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

    PrintString("Motor PWM CMD\r\n");
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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SerialCommand - Manage command lines for this program
//
// Inputs:      Command line typed by user
//
// Outputs:     None.
//
void SerialCommand(char *Line) {
    int     MotorID;
    char    *Command;

    ParseInit(Line);
    Command = ParseToken();

    //
    // Universally accept <ESC> as stop all motors
    //
    if( StrEQ(Command,ESC_CMD) ) {
        for( MotorID = 0; MotorID < NUM_MOTORS; MotorID++ )
            MotorPWM(MotorID,0);
        PrintCRLF();
        PrintString("All off\r\n");
        return;
        }


    //
    // MO - Set motor state
    //
    if( StrEQ(Command,"MO") ) {
        char *MotorText = ParseToken();
        int MotorNum    = *MotorText - '0';

        if( strlen(MotorText) != 1          ||
            MotorNum          < 0           ||
            MotorNum          >= NUM_MOTORS ||
            !isdigit(*MotorText)             ) {
            PrintString("Unrecognized motor # (");
            PrintString(MotorText);
            PrintString("), must be 0 to ");
            PrintD(NUM_MOTORS,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        char   *PctText  = ParseToken();
        int8_t  MotorPct = atoi(PctText);

        if( MotorPct < -100 ||
            MotorPct >  100 ) {
            PrintString("Unrecognized motor dir (");
            PrintString(PctText);
            PrintString("), must be one of FWD, REV, or OFF\r\n");
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        MotorPWM(MotorNum,MotorPct);
        return;
        }


    //
    // RE - Reset (stop all motors)
    //
    if( StrEQ(Command,"RE") ) {
        for( uint8_t ID = 0; ID < NUM_MOTORS; ID++ )
            MotorPWM(ID,0);
        return;
        }


    //
    // ST - Print motor status
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
