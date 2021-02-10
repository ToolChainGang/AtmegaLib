//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      PulseGenerator.c
//
//  SYNOPSIS
//
//      Connect an LED and current-limiting resistor to PORTD.7
//
//      Compile, load, and run this module. The LED should pulse with a frequency
//        and pulse width determined by serial commands, for example:
//
//      > P 1000        # Sets the pulse to repeat every 1000 ms
//      > O 10          # Sets the pulse ON for 10 ms
//
//      As an additional interface, you can set the pulse characteristics based on
//        frequency and duty cycle.
//
//      > F 10          # Sets the pulse frequency to 10 cycles per second
//      > D 30          # Sets the duty cycle to 30%
//
//      You can freely mix and match these commands.
//
//      Also, "H" will show a help text,
//            "?" will show current pulse settings
//
//  DESCRIPTION
//
//      This program uses the Timer module for timing.
//
//  VERSION:    2010.12.05
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

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <ctype.h>

#include <PortMacros.h>
#include <Timer.h>
#include <UART.h>
#include <Serial.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define LED_PORT    D               // Use PORTD
#define LED_PIN     5               // LED is on pin 7

//
// Timer for LED flashing
//
uint16_t  OnTicks     __attribute__ ((section (".noinit")));
uint16_t  PulseTicks  __attribute__ ((section (".noinit")));
bool    LEDOn       __attribute__ ((section (".noinit")));

uint16_t LEDTimer    __attribute__ ((section (".noinit")));

static const prog_char Prompt[] = "> ";
static char     Command;
static uint32_t Argument;

enum {
    START_CMD   = 0,
    GET_NUMBER,
    } State;

static const prog_char Help[] =
"P 1000  # Sets the pulse to repeat every 1000 ms\r\n"
"O 10    # Sets the pulse ON for 10 ms\r\n"
"F 10    # Sets the pulse frequency to 10 cycles per second\r\n"
"D 30    # Sets the duty cycle to 30%\r\n"
"\r\n"
"?       # Show current setting\r\n"
"\r\n"
;

static const prog_char Hint[] = "Type 'H' for help.\r\n";

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PrintSettings - Print current settings
//
// Inputs:      None.
//
// Outputs:     None.
//
void PrintSettings(void) {

    PrintString("Pulse = ");
    PrintD(PulseTicks,0);
    PrintString(" On = ");
    PrintD(OnTicks,0);
    PrintCRLF();
    PrintCRLF();
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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
        // P - Set pulse width in ms
        //
        case 'P':
            PulseTicks = Argument;
            break;

        //
        // O - Set pulse "on" time in ms
        //
        case 'O':
            OnTicks = Argument;
            break;

        //
        // F - Set pulse frequency
        //
        case 'F':
            PulseTicks = (1000+Argument/2)/Argument;
            break;

        //
        // D - Set pulse duty cycle
        //
        case 'D':
            if( Argument > 100 ) Argument = 100;
            OnTicks = (100*PulseTicks)/Argument;
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

            if( InChar != 'P' &&
                InChar != 'O' &&
                InChar != 'F' &&
                InChar != 'D' &&
                InChar != 'p' &&
                InChar != 'o' &&
                InChar != 'f' &&
                InChar != 'd' &&
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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PulseGenerator - Generate pulses on command
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    //
    // Initialize the LED port
    //
    _SET_BIT( _DDR(LED_PORT),LED_PIN);  // LED is an output
    _SET_BIT(_PORT(LED_PORT),LED_PIN);  // Set high for now

    PulseTicks = 16000;                 // Init pulse timer
    OnTicks    = 1; 
    LEDTimer   = OnTicks;
    LEDOn      = true;                  // Pulse is currently on

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    TimerInit();
    UARTInit();

    sei();                              // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintCRLF();
    PrintString("Pulse Generator\r\n"); // String is in RAM
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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerISR - Called by the timer section once a tick
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerISR(void) {

    if( --LEDTimer > 0 )                // Time to change state?
        return;                         // Nope - return

    _CHG_BIT(_PORT(LED_PORT),LED_PIN);  // Change state of LED

    LEDOn    = !LEDOn;
    LEDTimer = LEDOn ? OnTicks : (PulseTicks-OnTicks);
    }
