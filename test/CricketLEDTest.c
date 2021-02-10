////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2018 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      CricketBusTest.c
//
//  SYNOPSIS
//
//      Connect the white, "bus signal" of your cricket display to PORTD.7
//      (Also Black GND to ground, and red BUSV to +5V)
//
//      Compile, load, and run this module.
//
//      Cricket display will begin a demo program:
//
//          1) display a count in decimal
//          2) display a count in hex
//          3) cycle the display brightness
//          4) cycle the digit segments
//          5) repeat...
//
//      You can easily change the demo behaviour below.
//
//      The port and pin can be easily changed, see CrucketBus.h
//
//  DESCRIPTION
//
//      This is a simple AVR program to test the cricket display
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

#include "UART.h"
#include "Serial.h"
#include "CricketBus.h"

#define DELAY_MS  1000              // mS of on time between displayed frames

//
// If you have trouble, uncomment this and look at your bus line (default: PORTD.7)
//   on an oscilloscope. The bus protocol is available on the net.
//
//#define DEBUG

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// CricketDisplay.c - Test the Arduino interface to a cricket display
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int main(void) {

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize things
    //
    UARTInit();

    CricketBusInit();

    sei();                                          // Enable interrupts

    PrintString("Reset CricketBusTest\r\n");

#ifdef DEBUG
    while(1) {
        CricketBusPut(16,true);
        _delay_ms(10);
        }
#endif // DEBUG

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    uint16_t Number = 1025;

    CricketLEDBright(4,0);

    while(1) {
        int Count;

        for( Count = 0; Count < 16*4; Count++ ) {
            CricketLEDDec(Number,0);

            PrintString("Display dec ");
            PrintD(Number,0);
            PrintCRLF();
            
            Number++;
            _delay_ms(100);
            }

        _delay_ms(DELAY_MS);              // Wait 1/2 cycle

        for( Count = 0; Count < 16*4; Count++ ) {
            CricketLEDHex(Number,0);

            PrintString("Display hex ");
            PrintD(Number,0);
            PrintCRLF();
            _delay_ms(100);
            
            Number++;
            }

        _delay_ms(DELAY_MS);              // Wait 1/2 cycle

        for( Count = 0; Count < 16*4; Count++ ) {
            uint8_t Bright = Count & 0x07;

            CricketLEDBright(Bright,0)

            PrintString("Display bright ");
            PrintD(Bright,0);
            PrintCRLF();
            _delay_ms(100);
            }

        CricketLEDBright(4,0);

        for( Count = 0; Count < 4; Count++ ) {
            int Bit;

            PrintString("Cycle Pattern bits\r\n");

            for( Bit = 0; Bit < 8; Bit++ ) {
                _delay_ms(200);
                CricketLEDPat(1 << Bit,1 << Bit,1 << Bit,1 << Bit,0)
                }

            _delay_ms(100);
            }

        _delay_ms(DELAY_MS);              // Wait 1/2 cycle
        } 
    }

