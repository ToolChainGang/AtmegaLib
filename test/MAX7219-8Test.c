////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      MAX7219-8Test.c
//
//  SYNOPSIS
//
//      Connect eight MAX7219 LED driver DIN to PORTA
//      Connect all   MAX7219 LOAD pins to PORTD.3
//      Connect all   MAX7219 CLK  pins to PORTD.4
//      
//      Run this program. The LED matrix should scroll the alphabit
//
//  DESCRIPTION
//
//      This is a simple AVR program to test 8-parallel MAX7219 chips
//
//      It is used to verify and test the LED outputs, SPI interface, &c.
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
#include <util/delay.h>
#include <stdlib.h>

#include "UART.h"
#include "Serial.h"
#include "Max7219-8.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Max7219-8Test.c
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int main(void) {
    uint8_t i, j;
    int     Round = 0;

    uint8_t MaxBuffer[8];

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    UARTInit();
    MAX7219Init;


    sei();                              // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintCRLF();
    PrintCRLF();
    PrintString("Max7219-8 Test\r\n");
    PrintString("============\r\n");

    MAX7219DisplayTest(false);
    MAX7219Shutdown(false);
    MAX7219Intensity(16);
    MAX7219DecodeMode(MAX7219_NO_DECODE);
    MAX7219ScanLimit(7);

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Light the matrix ON and OFF a couple of times to see that the display is working,
    //   then show some random characters to show that the interface is working.
    //
    while(1) {

        PrintString("Round ");
        PrintD(Round++,0);
        PrintString(": ");

        PrintString("Blink");

        for( i=0; i<4; i++ ) {
            for( uint8_t i=0; i<8; i++ )    // Clear display to start
                MaxBuffer[i] = 0;

            MAX7219DisplayTest(true);       // Turn display all ON
            _delay_ms(500);                 // Wait a bit

            MAX7219DisplayTest(false);      // Turn display all OFF
            _delay_ms(500);                 // Wait a bit
            }

        PrintString(", Random");

        for( j=0; j<32; j++ ) {
            for( uint8_t i=0; i<8; i++ ) {
                uint8_t Digit = rand() & 0xFF;
                MaxBuffer[i] = Digit;
                }
            MAX7219Digit(1,MaxBuffer);
            _delay_ms(250);                 // Wait a bit
            }

        PrintCRLF();
        }
    }
