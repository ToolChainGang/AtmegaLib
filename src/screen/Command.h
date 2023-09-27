////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Command.h
//
//  SYNOPSIS
//
//      ...MAScreen.c
//      ...HEScreen.c
//      (Other screens, as needed)
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//      TimerInit();
//
//      ///////////////////////////////////////////////////////////////////////////
//      //
//      // All done with init, OK to enable interrupts and go
//      // 
//      sei();                              // Enable interrupts
//
//      ScreenInit();                       // NOTE: After interrupts enabled!
//      CommandInit();                      // NOTE: After interrupts enabled!
//
//      while(1) {
//
//          //
//          // Process serial commands, if any
//          //
//          ProcessSerialInput(GetUARTByte());
//          } 
//
//  DESCRIPTION
//
//      Command processing
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

#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

//
// The user input prompt
//
#ifndef PROMPT
#define PROMPT      "Cmd> "
#endif

//
// Maximum size of an input token. In other words, the maximum number of characters in 
//   a command, or between two delimiters or the maximum number of digits in an
//   input number
//
#define MAX_TOKEN_LENGTH    10

//
// Maximum size of an input entry.  In other words, the maximum number of characters
//   that can be entered on a single line for input over the serial port.
//
#define MAX_CMD_LENGTH      20

//
// Any character in the following is a delimiter character. Delimiters come between
//   tokens in a command.
//
#define DELIMITERS  " /t"

//
// Define this next def and serial input will be echoed back to the user
//   (Debugging thingy.)
//
//#define ECHO_INPUT

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CommandInit - System initialization for command processing
//
// Inputs:      None.
//
// Outputs:     None.
//
void CommandInit(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PlotInput - Put cursor in correct position for screen input
//
// Inputs:      None.
//
// Outputs:     None.
//
void PlotInput(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PlotError - Print an error message
//
// Inputs:      Message to print
//
// Outputs:     None.
//
void PlotError(char *Error);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command - Process text command
//
// Inputs:      Line command to process
//
// Outputs:     None.
//
void Command(char *Buffer);

#endif  // COMMAND_H - entire file
