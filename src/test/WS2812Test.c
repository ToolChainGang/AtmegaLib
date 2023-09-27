////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      UARTTest.c
//
//  SYNOPSIS
//
//      Connect your project to a host computer. For example, on a windows
//        system connect to one of the serial ports and view with hyperterm.
//
//      (For systems such as Arduino which use the USB->Serial connection,
//        connect to a USB port and determine the COM port to use from the
//        device manager. Then use hyperterm for that port.)
//
//      Compile, load, and run this module. The UART sends uppercase
//        characters once a second.
//
//      Additionally, chars received are incremented and echoed back ('a' is
//        echoed as 'b', 'M' is echoed as 'N', and so on).
//
//      You can easily change the baud rate in the UART.h file
//
//  DESCRIPTION
//
//      This is a simple AVR program with serial I/O.
//
//      It is used to verify and test the serial connection, null modem, 
//        baud rate, &c.
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
#include <util/delay.h>

#include "PortMacros.h"
#include "WS2812.h"

#define NUM_LEDS    16

struct cRGB LEDs[NUM_LEDS];

void March(void) {

    for( int i=NUM_LEDS-1; i>0; --i )
        LEDs[i] = LEDs[i-1];
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UARTTest - Test the UART with continuous output & input processing
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
MAIN main(void) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
//    WS2812Init();

#if 0
#define LED_PORT    B               // Use PORTD
#define LED_PIN     1               // LED is on pin 7

    _SET_BIT( _DDR(LED_PORT),LED_PIN);  // LED is an output
    _SET_BIT(_PORT(LED_PORT),LED_PIN);  // Set low for now
#endif

    sei();                              // Enable interrupts

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 

    while(1) {

        LEDs[0].r = 0;
        LEDs[0].g = 16;
        LEDs[0].b = 0;

        ws2812_setleds(LEDs,1);
        _delay_ms(500);

        March();

        LEDs[0].r = 0;
        LEDs[0].g = 0;
        LEDs[0].b = 0;

        ws2812_setleds(LEDs,NUM_LEDS);
        _delay_ms(500);

        March();
        }
    }
