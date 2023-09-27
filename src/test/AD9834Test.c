////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      AD9834Test.c
//
//  SYNOPSIS
//
//      Connect an AD9834 sin/square generator board to an Arduino using the SPI bus
//
//      Compile, load, and run this module. The AD9834 board should generate frequency
//        and phase according to the commands below
//
//      > F 1000        # Sets the frequency to 1000 Hz
//
//      You can freely mix and match these commands.
//
//      Also, "H" will show a help text,
//            "?" will show current settings
//
//  DESCRIPTION
//
//      This program uses the Timer module for timing.
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

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <ctype.h>

#include "PortMacros.h"
#include "UART.h"
#include "Serial.h"
#include "SerialLong.h"

#include "AD9834.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const char Prompt[] PROGMEM = "> ";
static char     Command;
static uint32_t Argument;

enum {
    START_CMD   = 0,
    GET_NUMBER,
    } State;

static const char Help[] PROGMEM =
"F 1000000  # Sets the frequency to 1MHz\r\n"
"\r\n"
"?       # Show current settinh\r\n"
"\r\n"
;

static const char Hint[] PROGMEM = "Type 'H' for help.\r\n";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrintSettings - Print current settings
//
// Inputs:      None.
//
// Outputs:     None.
//
void PrintSettings(void) {

    PrintString("Freq = ");
    PrintLD(AD9834GetFreq(),0);
    PrintString(" On = ");
    PrintD(AD9834IsEnabled()?1:0,0);
    PrintCRLF();
    PrintCRLF();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ProcessCommand - Process typed command
//
// Inputs:      None.
//
// Outputs:     None.
//
void ProcessCommand(void) {

    switch(Command) {

        //
        // F - Set frequency
        //
        case 'P':
            AD9834SetFreq(Argument);
            break;

        //
        // H - Show help text.
        //
        case 'H':
            PrintStringP(Help);
            break;

        //
        // ? - Show current settings.
        //
        case '?':
            PrintSettings();
            break;

        default:
            PrintString("Unrecognized Command?\r\n");
            break;
        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ProcessInput - Process input char
//
// Inputs:      Received serial char
//
// Outputs:     None.
//
void ProcessInput(char InChar) {

    //
    // LF characters are ignored.
    //
    if( InChar == '\n' )
        return;

    //
    // Whitespace is ignored
    //
    if( InChar == ' ' )
        return;

    if( InChar == '\t' )
        return;

    //
    // Otherwise, process char according to state
    //
    switch(State) {

        //
        // START_CMD - Wait for the command char
        //
        case START_CMD:
            //
            // Blank lines are ignored
            //
            if( InChar == '\r' )
                return;

            if( InChar != 'F' &&
                InChar != 'h' &&
                InChar != 'H' &&
                InChar != '?' ) {
                PrintString("Unrecognized command: ");
                PrintChar(InChar);
                PrintCRLF();
                PrintStringP(Hint);
                PrintStringP(Prompt);
                return;
                }

            Command  = toupper(InChar);
            Argument = 0;
            State    = GET_NUMBER;
            break;


        //
        // GetNumber - Get number from input stream
        //
        case GET_NUMBER:
            //
            // EOL means process command
            //
            if( InChar == '\r' ) {
                ProcessCommand();
                State = START_CMD;
                PrintStringP(Prompt);
                return;
                }

            //
            // Otherwise, only allow digits
            //
            if( !isdigit(InChar) ) {
                PrintString("Not a digit: ");
                PrintChar(InChar);
                PrintCRLF();
                PrintStringP(Hint);
                PrintStringP(Prompt);
                State = START_CMD;
                return;
                }

            Argument = Argument*10 + (InChar - '0');
            break;

        default:
            PrintString("Bad input state?\r\n");
            State = START_CMD;
            break;
        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TestAD9834 - Generate frequency on command
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //
    // Initialize the AD9834
    //
    AD9834Init();

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    UARTInit();

    sei();                              // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintCRLF();
    PrintString("AD9834 Test\r\n"); // String is in RAM
    PrintStringP(Hint);
    PrintCRLF();
    PrintSettings();
    PrintStringP(Prompt);
    State = START_CMD;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        char InChar;

        sleep_cpu();                    // Wait for timer

        //
        // Processed characters as received
        //
        while( (InChar = GetUARTByte()) ) {
            PrintChar(InChar);
            if( InChar == '\r' )
                PrintChar('\n');
            ProcessInput(InChar);
            }
        } 
    }
