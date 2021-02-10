////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SetOutput.c
//
//  SYNOPSIS
//
//      Connect a stepper motor as shown below
//          #define STP_PORT    D               // Use PORTD
//          #define STP_PIN     7               // Step is on pin 7
//
//          #define ENB_PORT    D
//          #define ENB_PIN     6
//
//          #define DIR_PORT    C
//          #define DIR_PIN     5
//
//      Compile, load, and run this module. Various commands allow you to
//        test the stepper connections.
//
//      Also, "?" will show the help panel
//
//      (Simple, but sometimes you need the ability to just generate step pulses.)
//
//  DESCRIPTION
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
#include <avr/sleep.h>
#include <util/delay.h>

#include <ctype.h>
#include <stdbool.h>

#include "PortMacros.h"
#include "UART.h"
#include "Serial.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define STP_PORT    F               // Step
#define STP_PIN     0

#define DIR_PORT    F               // Dir
#define DIR_PIN     1

#define ENB_PORT    D               // ~Enable
#define ENB_PIN     7

static const prog_char Help[] =
"+      Go faster (by 10%)\r\n"
"-      Go slower (by 10%)\r\n"
"\r\n"
"D      Disable motor drive\r\n"
"E      Enable  motor drive\r\n"
"\r\n"
"F      Set FWD direction\r\n"
"R      Set REV direction\r\n"
"\r\n"
"H      Set step output HIGH\r\n"
"L      Set step output LOW\r\n"
"\r\n"
"P      Send 1 step pulse\r\n"
"C      Send continuous pulses\r\n"
"<ESC>  Stop continuous output\r\n"
"\r\n"
"?      Show help panel\r\n"
"\r\n"
;

static bool Continuous = false;
#define ESC     '\033'

int StepDelay;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Pulse - Send 1 motor step pulse
//
// Inputs:      None.
//
// Outputs:     None.
//
void Pulse(void) {

    _SET_BIT(_PORT(STP_PORT),STP_PIN);
    _delay_ms(StepDelay);
    _CLR_BIT(_PORT(STP_PORT),STP_PIN);
    _delay_ms(StepDelay);
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintSettings - Print out the settings
//
// Inputs:      None.
//
// Outputs:     None.
//
void PrintSettings(void) {

    PrintString("ENB ");
    PrintString(_BIT_ON(_PORT(ENB_PORT),ENB_PIN) ? "Off" : "On");
    PrintString(", DIR ");
    PrintString(_BIT_ON(_PORT(DIR_PORT),DIR_PIN) ? "Rev" : "Fwd");
    PrintString(", Step delay ");
    PrintD(StepDelay,0);
    PrintString(" ms\r\n");
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ProcessInput - Process input char
//
// Inputs:      Received serial char
//
// Outputs:     None.
//
void ProcessInput(char InChar) {

    //
    // Echo RET as CR-LF
    //
    if( InChar == '\r' ) {
        PrintChar('\n');
        return;
        }

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

    switch(toupper(InChar)) {

        //
        // + - Go faster (by 10%)
        //
        case '+':
            StepDelay -= (StepDelay+9)/10;
            if( StepDelay == 0 )
                StepDelay = 1;
            PrintSettings();
            return;
            break;

        //
        // - - Go slower (by 10%)
        //
        case '-':
            StepDelay += (StepDelay+9)/10;
            PrintSettings();
            return;
            break;

        //
        // D - Disable motor output
        //
        case 'D':
            _SET_BIT(_PORT(ENB_PORT),ENB_PIN);
            PrintString("Disabled\r\n");
            return;
            break;

        //
        // E - Enable motor output
        //
        case 'E':
            _CLR_BIT(_PORT(ENB_PORT),ENB_PIN);
            PrintString("Enabled.\r\n");
            return;
            break;

        //
        // F - Go FWD
        //
        case 'F':
            _CLR_BIT(_PORT(DIR_PORT),DIR_PIN);
            PrintString("Going FWD\r\n");
            return;
            break;

        //
        // R - Go REV
        //
        case 'R':
            _SET_BIT(_PORT(DIR_PORT),DIR_PIN);
            PrintString("Going REV\r\n");
            return;
            break;

        //
        // H - Set output HIGH
        //
        case 'H':
            _SET_BIT(_PORT(STP_PORT),STP_PIN);
            PrintString("Output is now HIGH\r\n");
            return;
            break;

        //
        // L - Set output LOW
        //
        case 'L':
            _CLR_BIT(_PORT(STP_PORT),STP_PIN);
            PrintString("Output is now LOW\r\n");
            return;
            break;

        //
        // P - Send pulse
        //
        case 'P':
            Pulse();
            PrintString("Pulse\r\n");
            return;
            break;

        //
        // C - Set for continuous pulses
        //
        case 'C':
            Continuous = true;
            PrintSettings();
            PrintString("Continuous pulses (ESC to stop)\r\n\r\n");
            return;
            break;

        //
        // ESC - Escape from continuous mode
        //
        case ESC:
            Continuous = false;
            PrintString("Stop\r\n");
            return;
            break;

        //
        // ? - Show help panel
        //
        case '?':
            PrintStringP(Help);
            PrintSettings();
            PrintCRLF();
            return;
            break;

        default:
            break;
        }

    PrintString("Output is ");
    if( _BIT_ON(_PORT(STP_PORT),STP_PIN) ) PrintString("High");
    else                                   PrintString("Low");

    PrintCRLF();
    PrintCRLF();
    PrintStringP(Help);
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SetOutput - Set the output state
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the Motor port
    //
    _SET_BIT( _DDR(STP_PORT),STP_PIN);  // STP is an output
    _CLR_BIT(_PORT(STP_PORT),STP_PIN);  // Set low for now

    _SET_BIT( _DDR(DIR_PORT),DIR_PIN);  // DIR is an output
    _CLR_BIT(_PORT(DIR_PORT),DIR_PIN);  // Set low for now

    _SET_BIT( _DDR(ENB_PORT),ENB_PIN);  // ~ENB is an output
    _CLR_BIT(_PORT(ENB_PORT),ENB_PIN);  // Set low for now

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    UARTInit();

    Continuous = false;
    StepDelay  = 100;

    sei();                              // Enable interrupts

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    PrintString("Reset StepperPulse\r\n");
    PrintCRLF();
    PrintString("Press '?' for help\r\n");
    PrintCRLF();

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        char InChar;

        if( Continuous ) Pulse();
        else             sleep_cpu();      // Wait for typed char

        //
        // Process characters as received
        //
        while( (InChar = GetUARTByte()) )
            ProcessInput(InChar);

        } 
    }
