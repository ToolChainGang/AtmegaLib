//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Parse2ch.c
//
//  SYNOPSIS
//      See Parse2ch.h for details.
//
//  DESCRIPTION
//
//      Command parsing of 2-char commands.
//
//      A simplified version of the parse module that manages serial input
//        (echo, del, LF and CR) and parses 2-character commands and an optional argument.
//
//////////////////////////////////////////////////////////////////////////////////////////
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

#include <ctype.h>
#include <string.h>

#include "PortMacros.h"
#include "Serial.h"
#include "Parse2ch.h"

static uint16_t Command                NOINIT;
static char     Argument[MAX_ARG2CH+1] NOINIT;

static enum {
    CMD_CHAR1 = 7,      // Expecting 1st char of command
    CMD_CHAR2,          // Expecting 2nd char of command
    CMD_CHAR3,          // Expecting 3rd char of command
    ARG_CHAR1,          // Expecting 1st (non-space) char of argument
    ARG_CHAR2,          // Expecting 2nd (non-space) char of argument
    } ParseState NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Parse2chInit - Initialize command line parsing
//
// Inputs:      TRUE  if should print prompt
//              FALSE if caller will haneld prompting
//
// Outputs:     None.
//
void Parse2chInit(bool Prompt) {

    ParseState = CMD_CHAR1;
    Command    = 0;
    memset(Argument,0,sizeof(Argument));

    if( Prompt )
        PrintString(PROMPT);
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Parse2ch - Process serial input char
//
// Inputs:      Next typed serial char
//
// Outputs:     None. User's Parse2Command is called when entry complete
//
void Parse2ch(char InChar) {

    //
    // NUL is the result of the UART returning no input.
    //
    // It's probably a bug on the user's side, but ignore it anyway.
    //
    if( InChar == 0 )
        return;

#ifdef ECHO_2CH
    PrintChar(InChar);

    //
    // Echo CR-LF when CR entered.
    //
    if( InChar == '\r' )
        PrintChar('\n');
#endif

    //
    // LF characters are universally ignored.
    //
    if( InChar == '\n' )
        return;

    //
    // Otherwise, process char according to state
    //
    switch(ParseState) {

        //
        // CMD_CHAR1 - Looking for the 1st command char
        //
        case CMD_CHAR1:
            //
            // Blank lines are ignored
            //
            if( InChar == '\r' )
                return;

            //
            // Whitespace (at beginning of line) is ignored
            //
            if( InChar == ' ' )
                return;

            Command    = toupper(InChar);
            ParseState = CMD_CHAR2;
            break;


        //
        // CMD_CHAR2 - Looking for the 2nd command char
        //
        case CMD_CHAR2:
            //
            // Single char command, no argument
            //
            if( InChar == '\r' ) {
                Parse2chInit(Parse2Command(Command,Argument));
                return;
                }

            //
            // Whitespace (after 1st char command) implies an argument
            //
            if( InChar == ' ' ) {
                ParseState = ARG_CHAR1;
                return;
                }


            //
            // Otherwise, grab 2nd char and look for argument spacer
            //
            Command    = (Command << 8) + toupper(InChar);
            ParseState = CMD_CHAR3;
            break;


        //
        // CMD_CHAR3 - Looking for the 3rd command char
        //
        case CMD_CHAR3:
            //
            // Completed command, no argument
            //
            if( InChar == '\r' ) {
                Parse2chInit(Parse2Command(Command,Argument));
                return;
                }

            //
            // Command chars after the 2nd are ignored. Anything except space.
            //
            if( InChar != ' ' )
                return;

            ParseState = ARG_CHAR1;
            break;


        //
        // ARG_CHAR1 - Looking for the 1st argument char
        //
        case ARG_CHAR1:
            //
            // Whitespace (after command) ignored
            //
            if( InChar == ' ' )
                return;

            //
            // Start of argument.
            //
            ParseState = ARG_CHAR2;

            //
            // FALL THROUGH
            //   |     |
            //   V     V


        //
        // ARG_CHAR2 - Looking for the next argument char
        //
        case ARG_CHAR2:
            //
            // Completed command, no argument
            //
            if( InChar == '\r' ) {
                Parse2chInit(Parse2Command(Command,Argument));
                return;
                }

            //
            // Place new char at the end of the argument
            //
            if( strlen(Argument) < MAX_ARG2CH )
                Argument[strlen(Argument)] = InChar;

            break;
        }
    }

