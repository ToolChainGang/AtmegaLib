////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      Parse2ch.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Parse2ch.h
//      //
//      ...set    user prompt
//      ...select local echo (or not)
//      ...select maximum argument
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      Parse2chInit();                     // Called once at startup
//          :
//
//      sei();                              // Enable interrupts
//
//
//      while(1) {
//          char InChar;
//
//          sleep_cpu();                    // Wait for timer
//
//          //
//          // Processed characters as received
//          //
//          while( (InChar = GetUARTByte()) )
//              Parse2ch(InChar);
//
//          }
//
//      //////////////////////////////////////
//      //
//      // Command Processing
//      //
//      void Parse2Command(uint16_t Command,char *Argument) {
//
//          switch(Command) {
//
//              case 'PU':
//                  if( strlen(Argument) ) {
//                      int Argument = atoi(Argument);
//                      ...     // Process "PU ###" command
//                      }
//
//                  ...         // Process "PU" command
//                  break;
//
//              }
//          }
//
//
//  DESCRIPTION
//
//      Command parsing of 2-char commands.
//
//      A simplified version of the parse module that manages serial input 
//        (echo, LF and CR) and parses 2-character commands and an optional argument.
//
//  NOTE
//
//      DEL and BS are not processed.
//
//      Commands are truncated to 2 chars. Thus "OFF" will be parsed as "OF", and so
//        on. This is on purpose, for a simple command system.
//
//      Add -Wno-multichar to the compile options to get rid of the multichar compile
//        warnings.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

#ifndef PARSE2CH_H
#define PARSE2CH_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// The user input prompt
//
#define PROMPT  "*> "

//
// Maximum allowed length of argument string
//
#define MAX_ARG2CH  8

//
// Echo chars as they are typed (comment out for local echo)
//
#define ECHO_2CH

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parse2chInit - Initialize command line parsing
//
// Inputs:      TRUE  if should print prompt
//              FALSE if caller will print prompt
//
// Outputs:     None.
//
void Parse2chInit(bool Prompt);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parse2ch - Process serial input char
//
// Inputs:      Next typed serial char
//
// Outputs:     None. User's Parse2Command is called when entry complete
//
void Parse2ch(char InChar);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parse2Command - User's command processing function
//
// Inputs:      2-character command, as integer
//
// Outputs:     TRUE  if should reprompt the user
//              FALSE if caller responsible for prompt
//
// NOTE: TO be supplied by the user
//
extern bool Parse2Command(uint16_t Command, char *Argument);

#endif  // PARSE2CH_H - Entire file 
