////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Command.c
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

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "PortMacros.h"
#include "VT100.h"

#include "Command.h"
#include "Screen.h"
#include "Parse.h"
#include "GetLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// IsDelimiter() macro
//
#define IsDelimiter(__char__)   strchr(DELIMITERS,__char__)

static char     CommandBuffer[MAX_CMD_LENGTH+1] NOINIT;
static uint8_t  nChars                          NOINIT;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmultichar"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitCommandBuffer - Initialize command buffer for next command
//
// Inputs:      None.
//
// Outputs:     None.
//
static void InitCommandBuffer(void) {

    memset(CommandBuffer,0,sizeof(CommandBuffer));
    nChars = 0;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CommandInit - Command initialization
//
// Inputs:      None.
//
// Outputs:     None.
//
void CommandInit(void) {

    InitCommandBuffer();
    Prompt();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command - Process text command
//
// Inputs:      Line command to process
//
// Outputs:     None.
//
void Command(char *Buffer) {

    ParseInit(Buffer);

    char *Command = ParseToken();

#ifdef USE_MAIN_SCREEN
    //
    // MA - Show the main screen
    //
    if( StrEQ(Command,"MA") ) {
        ShowScreen('MA');
        return;
        }

#ifdef USE_MAIN_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'MA' ) {
        if( MAScreenCommand(Command) )
            return;
        }

    //
    // Universally accept <ESC> as an escape to the main screen.
    //
    if( StrEQ(Command,ESC_CMD) ) {
        ShowScreen('MA');
        return;
        }
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_HELP_SCREEN
    //
    // HE - Help screen
    //
    if( StrEQ(Command,"HE") ||
        StrEQ(Command,"?" ) ) {
        ShowScreen('HE');
        return;
        }

#ifdef USE_HELP_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'HE' ) {
        if( HEScreenCommand(Command) )
            return;
        }
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_DEBUG_SCREEN
    //
    // DE - Show the debug screen
    //
    if( StrEQ(Command,"DE") ) {
        ShowScreen('DE');
        return;
        }

#ifdef USE_DEBUG_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'DE' ) {
        if( DEScreenCommand(Command) )
            return;
        }
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_MEMORY_SCREEN
    //
    // ME - Show the memory screen
    //
    if( StrEQ(Command,"ME") ) {
        ShowScreen('ME');
        return;
        }

#ifdef USE_MEMORY_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'ME' ) {
        if( MEScreenCommand(Command) )
            return;
        }
#endif
#endif

#ifdef USE_EEPROM_SCREEN
    //
    // EE - Show the memory screen
    //
    if( StrEQ(Command,"EE") ) {
        ShowScreen('EE');
        return;
        }

#ifdef USE_EEPROM_SCREEN_CMDS
    //
    // See if the local screen can manage the command
    //
    if( SelectedScreen == 'EE' ) {
        if( EEScreenCommand(Command) )
            return;
        }
#endif
#endif

    //
    // Not a recognized command. Let the user know he goofed.
    //
    CursorPos(1,ERROR_ROW);
    ClearEOL;
    PrintString("Unrecognized command (");
    PrintString(Command);
    PrintString(")\r\n");

    PrintStringP(PSTR(BEEP));
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PlotInput - Put cursor in correct position for input
//
// Inputs:      None.
//
// Outputs:     None.
//
void PlotInput(void) {

    PrintStringP(PSTR("/033["));
    PrintD(INPUT_ROW,0);
    PrintStringP(PSTR(";"));
    PrintD(INPUT_COL+strlen(CommandBuffer)+strlen(PROMPT),0);
    PrintStringP(PSTR("H"));
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PlotError - Print an error message
//
// Inputs:      Message to print
//
// Outputs:     None.
//
void PlotError(char *Error) {

    CursorPos(ERROR_COL,ERROR_ROW);

    PrintString(Error);
    PrintStringP(PSTR(")" BEEP));
    }

#pragma GCC diagnostic pop
