////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      DigitalPotCmd.C
//
//  SYNOPSIS
//
//      Connect a digital pot to the SPI port,
//
//      Compile, load, and run this module. The program will accept commands
//        to set the wiper via the serial port.
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

#define POT_PORT    D
#define POT_PIN     0
#define POT_WIPER   0                                   // Which wiper to use

#define MAX_RESIST              10000                   // Configure for your specific part
#define MAX_WIPER               MCP4161_STEPS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PotInit                 MCP4161Init(POT_PORT,POT_PIN)
#define SetWiper(_w_)           MCP4161SetWiper(POT_PORT,POT_PIN,_w_)
#define SetResist(_r_)          MCP4161SetResist(POT_PORT,POT_PIN,MAX_RESIST,_r_)
#define Incr(_Addr_)            MCP4161Incr(POT_PORT,POT_PIN,_Addr_)
#define Decr(_Addr_)            MCP4161Decr(POT_PORT,POT_PIN,_Addr_)
#define GetReg(_Addr_,_Dest_)   MCP4161GetReg(POT_PORT,POT_PIN,_Addr_,_Dest_)
#define PutReg(_Addr_,_Value_)  MCP4161PutReg(POT_PORT,POT_PIN,_Addr_,_Value_)

#define VWIPER0                 MCP4161_VWIPER0
#define R2W(_r_)                MCP4161_R2W(MAX_RESIST,_r_)
#define W2R(_w_)                MCP4161_W2R(MAX_RESIST,_w_)

//#define DEBUG

//
// Static layout of the help screen
//
#define HELP_SCREEN(_W_,_R_) "\
W <setting> Set wiper setting (<amt> being 0 .. " #_W_  ")\r\n\
R <ohms>    Set wiper ohms    (<amt> being 0 .. " #_R_ ")\r\n\
+           Increment wiper value\r\n\
-           Decrement wiper value\r\n\
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

    uint16_t Wiper;
    GetReg(VWIPER0,Wiper);

    uint16_t Resist = W2R(Wiper);

    PrintCRLF();
    PrintString("Wiper: ");
    PrintD(Wiper,5);            // == printf("%5d");
    PrintString(", ");
    PrintD(Resist,5);           // == printf("%5d");

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
// DigitalPotCmd - Command the digital potentiometer software
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
    PotInit;
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

    PrintString("DigitalPot CMD\r\n");
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
    // W - Set wiper value directly
    //
    if( StrEQ(Command,"W") ) {
        char *WiperText = ParseToken();
        int   WiperNum  = atoi(WiperText);

        if( WiperNum          < 0           ||
            WiperNum          > MAX_WIPER ) {
            PrintString("Bad or out of range wiper # (");
            PrintString(WiperText);
            PrintString("), must be 0 to ");
            PrintD(MAX_WIPER,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        SetWiper(WiperNum);
        PrintStatus();
        return;
        }


    //
    // R - Set wiper value by resistance
    //
    if( StrEQ(Command,"R") ) {
        char *ResistText = ParseToken();
        int   ResistNum  = atoi(ResistText);

        if( ResistNum          < 0           ||
            ResistNum          > MAX_RESIST ) {
            PrintString("Bad or out of range resist # (");
            PrintString(ResistText);
            PrintString("), must be 0 to ");
            PrintD(MAX_RESIST,0);
            PrintCRLF();
            PrintString("Type '?' for help\r\n");
            PrintCRLF();
            return;
            }

        PrintString("Set wiper to ");
        PrintD(R2W(ResistNum),0);
        PrintCRLF();
        SetResist(ResistNum);
        PrintStatus();
        return;
        }


    //
    // + - Increment wiper value
    //
    if( StrEQ(Command,"+") ) {
        Incr(POT_WIPER);
        PrintStatus();
        return;
        }


    //
    // - - Decrement wiper value
    //
    if( StrEQ(Command,"-") ) {
        Decr(POT_WIPER);
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
    PrintStringP(PSTR("Unrecognized Command \n\""));
    PrintString (Command);
    PrintStringP(PSTR("\"\r\n"));
    PrintString("Type '?' for help\r\n");
    PrintCRLF();
    }
