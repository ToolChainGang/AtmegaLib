////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MotorCMD.c
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
//      This is a simple AVR program to test H-bridge DC motors.
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
#include <string.h>
#include <ctype.h>

#include "UART.h"
#include "Serial.h"
#include "Motor.h"
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
MO # <dir>  Set motor direction   (<dir> being FWD, REV, or OFF)\r\n\
ST          Show motor status\r\n\
HE          Show this help panel\r\n\
?           Show this help panel\r\n\
"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MotorTest - Test the motor control software
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
static char *CTLText(MOTOR_CTL Ctl) {

    if( Ctl == MOTOR_FWD ) return ("Fwd");
    if( Ctl == MOTOR_REV ) return ("Rev");
    if( Ctl == MOTOR_OFF ) return ("Off");
    return("???");
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MotorTest - Test the motor control software
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
static void PrintStatus(void) {

    PrintCRLF();
    PrintString("Motors:\r\n");

#if NUM_MOTORS > 0
    PrintString("  0: ");
    PrintString(MOTOR0_STRING " ");
    PrintString(CTLText(GetMotor(0)));
#ifdef MOTOR0_NEG_LOGIC
    PrintString(" (inverted logic)");
#endif
    PrintCRLF();
#endif

#if NUM_MOTORS > 1
    PrintString("  1: ");
    PrintString(MOTOR1_STRING " ");
    PrintString(CTLText(GetMotor(1)));
#ifdef MOTOR1_NEG_LOGIC
    PrintString(" (inverted logic)");
#endif
    PrintCRLF();
#endif

#if NUM_MOTORS > 2
    PrintString("  2: ");
    PrintString(MOTOR2_STRING " ");
    PrintString(CTLText(GetMotor(2)));
#ifdef MOTOR2_NEG_LOGIC
    PrintString(" (inverted logic)");
#endif
    PrintCRLF();
#endif

#if NUM_MOTORS > 3
    PrintString("  3: ");
    PrintString(MOTOR3_STRING " ");
    PrintString(CTLText(GetMotor(3)));
#ifdef MOTOR3_NEG_LOGIC
    PrintString(" (inverted logic)");
#endif
    PrintCRLF();
#endif

    PrintCRLF();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MotorTest - Test the motor control software
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
    MotorInit();

    sei();                              // Enable interrupts

    ClearScreen;
    //
    // End of init
    //
    //////////////////////////////////////////////////////////////////////////////////////

    PrintString("Motor CMD\r\n");
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
    int     MotorID;
    char    *Command;

    ParseInit(Line);
    Command = ParseToken();

    //
    // Universally accept <ESC> as stop all motors
    //
    if( StrEQ(Command,ESC_CMD) ) {
        for( MotorID = 0; MotorID < NUM_MOTORS; MotorID++ )
            Motor(MotorID,MOTOR_OFF);
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

        char       *DirText = ParseToken();
        MOTOR_CTL   MotorDir;

        if(      StrEQ(DirText,"FWD") ) MotorDir = MOTOR_FWD;
        else if( StrEQ(DirText,"REV") ) MotorDir = MOTOR_REV;
        else if( StrEQ(DirText,"OFF") ) MotorDir = MOTOR_OFF;
        else {
            PrintString("Unrecognized motor dir (");
            PrintString(DirText);
            PrintString("), must be one of FWD, REV, or OFF\r\n");
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        Motor(MotorNum,MotorDir);
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

