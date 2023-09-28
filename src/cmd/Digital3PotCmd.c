////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Digital3PotCmd.C
//
//  SYNOPSIS
//
//      Connect three digital pots to the SPI port,
//
//      Compile, load, and run this module. The program will accept commands
//        to set the wipers via the serial port.
//
//      See the HELP_SCREEN definition below for a list of available commands
//
//  DESCRIPTION
//
//      This is a simple AVR program with digital potentiometer control.
//
//      It is used to verify and test digital potentiometer connections.
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

#include "MCP4161.h"
#include "MCP4131.h"
#include "AD8400.h"
#include "UART.h"
#include "Serial.h"
#include "GetLine.h"
#include "Parse.h"
#include "VT100.h"

#define MIN_DEVICE  0
#define MAX_DEVICE  2

#define POT0_PORT   D
#define POT0_PIN    0

#define POT1_PORT   D
#define POT1_PIN    1

#define POT2_PORT   D
#define POT2_PIN    2


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161 - Device 0
//
#define MAX_STEPS0          (MCP4161_STEPS-1)
#define MAX_RESIST0         MCP4161_MAX_RESISTANCE

#define PotInit0            MCP4161Init(POT0_PORT,POT0_PIN)
#define SetWiper0(_w_)      MCP4161SetWiper(POT0_PORT,POT0_PIN,_w_)
#define SetResist0(_r_)     MCP4161SetResist(POT0_PORT,POT0_PIN,MAX_RESIST0,_r_)
#define Incr0               MCP4161Incr(POT0_PORT,POT0_PIN)
#define Decr0               MCP4161Decr(POT0_PORT,POT0_PIN)
#define GetReg0             MCP4161GetReg
#define PutReg0             MCP4161PutReg

#define VWIPER00            MCP4161_VWIPER0
#define R2W0                MCP4161_R2W
#define W2R0                MCP4161_W2R

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131 - Device 1
//
#define MAX_STEPS1          (MCP4131_STEPS-1)
#define MAX_RESIST1         MCP4131_MAX_RESISTANCE

#define PotInit1            MCP4131Init(POT0_PORT,POT0_PIN)
#define SetWiper1(_w_)      MCP4131SetWiper(POT0_PORT,POT0_PIN,_w_)
#define SetResist1(_r_)     MCP4131SetResist(POT0_PORT,POT0_PIN,MAX_RESIST1,_r_)
#define Incr1               MCP4131Incr(POT0_PORT,POT0_PIN)
#define Decr1               MCP4131Decr(POT0_PORT,POT0_PIN)
#define GetReg1             MCP4131GetReg
#define PutReg1             MCP4131PutReg

#define VWIPER01            MCP4131_VWIPER0
#define R2W1                MCP4131_R2W
#define W2R1                MCP4131_W2R

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AD8400 - Device 2
//
#define MAX_STEPS2          (AD8400_STEPS-1)
#define MAX_RESIST2         AD8400_MAX_RESISTANCE

#define PotInit2            AD8400Init(POT0_PORT,POT0_PIN)
#define SetWiper2(_w_)      AD8400SetWiper(POT0_PORT,POT0_PIN,_w_)
#define SetResist2(_r_)     AD8400SetResist(POT0_PORT,POT0_PIN,MAX_RESIST2,_r_)
#define Incr2               PrintString("(Incr not supported by AD8400.)\r\n");
#define Decr2               PrintString("(Decr not supported by AD8400.)\r\n");
#define GetReg2             AD8400GetReg
#define PutReg2             AD8400PutReg

#define VWIPER02            AD8400_VWIPER0
#define R2W2                AD8400_R2W
#define W2R2                AD8400_W2R

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//#define DEBUG

//
// Static layout of the help screen
//
#define HELP_SCREEN(_W_,_R_) "\
Wd <setting>    Set wiper d setting\r\n\
Rd <ohms>       Set wiper d ohms\r\n\
+d              Increment wiper d value\r\n\
-d              Decrement wiper #d value\r\n\
       Where d is a single digit 0 .. 2\r\n\
\r\n\
ST          Show pot status\r\n\
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
// PrintStatus - Print out the wiper statuses
//
// Inputs:      None.
//
// Outputs:     None.
//
static void PrintStatus(void) {

return;

//    uint16_t Wiper;
//    MCP4161GetReg(VWIPER0,Wiper);
//
//    uint16_t Resist = W2R(Wiper);
//
//    PrintCRLF();
//    PrintString("Wiper: ");
//    PrintD(Wiper,5);            // == printf("%5d");
//    PrintString(", ");
//    PrintD(Resist,5);           // == printf("%5d");
//
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
// Digital3PotCmd - Command three digital potentiometers
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
    SPIInit;
    PotInit0;
    PotInit1;
    PotInit2;
    UARTInit();

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

    PrintString("Digital3Pot CMD\r\n");
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
    char    *Command;

    ParseInit(Line);
    Command = ParseToken();

    //
    // Wd - Set wiper value directly
    //
    if( toupper(Command[0]) == 'W' ) {
        int   Device    = Command[1] - '0';
        char *WiperText = ParseToken();
        int   WiperNum  = atoi(WiperText);

        if( Device < MIN_DEVICE ||
            Device > MAX_DEVICE ) {
            PrintString("Bad or out of range device (");
            PrintString(Command);
            PrintString("), must be 0 to ");
            PrintD(MAX_DEVICE,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        uint16_t    MaxSteps;

        if( Device == 0 ) MaxSteps = MAX_STEPS0;
        if( Device == 1 ) MaxSteps = MAX_STEPS1;
        if( Device == 2 ) MaxSteps = MAX_STEPS2;

        if( WiperNum          < 0        ||
            WiperNum          > MaxSteps ) {
            PrintString("Bad or out of range wiper # (");
            PrintString(WiperText);
            PrintString("), must be 0 to ");
            PrintD(MaxSteps,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        if( Device == 0 ) SetWiper0(WiperNum);
        if( Device == 1 ) SetWiper1(WiperNum);
        if( Device == 2 ) SetWiper2(WiperNum);

        PrintStatus();
        return;
        }


    //
    // R - Set wiper value by resistance
    //
    if( toupper(Command[0]) == 'R' ) {
        int   Device     = Command[1] - '0';
        char *ResistText = ParseToken();
        int   ResistNum  = atoi(ResistText);

        if( Device < MIN_DEVICE ||
            Device > MAX_DEVICE ) {
            PrintString("Bad or out of range device (");
            PrintString(Command);
            PrintString("), must be 0 to ");
            PrintD(MAX_DEVICE,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        uint16_t    MaxResist;

        if( Device == 0 ) MaxResist = MAX_RESIST0;
        if( Device == 1 ) MaxResist = MAX_RESIST1;
        if( Device == 2 ) MaxResist = MAX_RESIST2;

        if( ResistNum          < 0           ||
            ResistNum          > MaxResist ) {
            PrintString("Bad or out of range resist # (");
            PrintString(ResistText);
            PrintString("), must be 0 to ");
            PrintD(MaxResist,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        PrintString("Set wiper to ");
        if( Device == 0 ) PrintD(R2W0(ResistNum),0);
        if( Device == 1 ) PrintD(R2W1(ResistNum),0);
        if( Device == 2 ) PrintD(R2W2(ResistNum),0);
        PrintCRLF();

        if( Device == 0 ) SetResist0(ResistNum);
        if( Device == 1 ) SetResist1(ResistNum);
        if( Device == 2 ) SetResist2(ResistNum);

        PrintStatus();
        return;
        }


    //
    // + - Increment wiper value
    //
    if( StrEQ(Command,"+") ) {
        int Device = Command[1] - '0';

        if( Device < MIN_DEVICE ||
            Device > MAX_DEVICE ) {
            PrintString("Bad or out of range device (");
            PrintString(Command);
            PrintString("), must be 0 to ");
            PrintD(MAX_DEVICE,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        if( Device == 0 ) Incr0;
        if( Device == 1 ) Incr1;
        if( Device == 2 ) Incr2;


        PrintStatus();
        return;
        }


    //
    // - - Decrement wiper value
    //
    if( StrEQ(Command,"-") ) {
        int Device = Command[1] - '0';

        if( Device < MIN_DEVICE ||
            Device > MAX_DEVICE ) {
            PrintString("Bad or out of range device (");
            PrintString(Command);
            PrintString("), must be 0 to ");
            PrintD(MAX_DEVICE,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        if( Device == 0 ) Decr0;
        if( Device == 1 ) Decr1;
        if( Device == 2 ) Decr2;

        PrintStatus();
        return;
        }


    //
    // ST - Print wiper status
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
        PrintString(HELP_SCREEN(MAX_WIPER,MAX_RESIST));
        PrintCRLF();
        return;
        }


    //
    // Not a recognized command. Let the user know he goofed.
    //
    PrintStringP(PSTR(BEEP));
    PrintStringP(PSTR("Unrecognized Command /""));
    PrintString (Command);
    PrintStringP(PSTR("/"\r\n"));
    PrintString("Type '?' for help\r\n");
    PrintCRLF();
    }
