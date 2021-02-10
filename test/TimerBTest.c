////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TimerBTest.c
//
//  SYNOPSIS
//
//      Connect an LED and current-limiting resistor to PORTD.7
//
//      Compile, load, and run this module. The LED should blink on/off with
//        50% duty cycle at a frequency of 1 second.
//
//      You can easily change the output pin and frequency as shown below,
//        to suit your needs.
//
//  DESCRIPTION
//
//      This program uses the TimerB module for timing.
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

#include <PortMacros.h>
#include <TimerB.h>

#define LED_PORT    D               // Use PORTD
#define LED_PIN     7               // LED is on pin 7

#define LED_TIME    2               // Seconds on time and off time

//
// Timer for LED flashing
//
TIME_T  LEDTimer  __attribute__ ((section (".noinit")));

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// BlinkLED - Blink an LED connected to LED_PORT
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int main(void) {

    //
    // Initialize the LED port
    //
    _SET_BIT( _DDR(LED_PORT),LED_PIN);  // LED is an output
    _CLR_BIT(_PORT(LED_PORT),LED_PIN);  // Set low for now

    LEDTimer = SECONDSB(LED_TIME);      // Init LED timer

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    TimerBInit();

    sei();                              // Enable interrupts

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        sleep_cpu();                    // Wait for timer
        } 
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerBISR - Called by the timer section once a tick
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerBISR(void) {

    if( --LEDTimer > 0 )                // Time to change state?
        return;                         // Nope - return

    LEDTimer = SECONDSB(LED_TIME);      // Reset LED timer

    _CHG_BIT(_PORT(LED_PORT),LED_PIN);  // Change state of LED
    }
