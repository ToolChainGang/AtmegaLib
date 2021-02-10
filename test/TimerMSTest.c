////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TimerMSTest.c
//
//  SYNOPSIS
//
//      Serial port timer testing
//
//      Compile, load, and run this module. The serial port should pop up a
//        heartbeat message at a frequency of 1 second.
//
//      You can easily change the output message and frequency as shown below,
//        to suit your needs.
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
#include <stdbool.h>

#include <PortMacros.h>
#include <UART.h>
#include <Serial.h>

#include "Timer.h"

//
// Timer for LED flashing
//
#define HEARTBEAT_SECS  1               // Seconds between heartbeat msg
TIME_T  HeartbeatTimer  NOINIT;

bool    SendHeartbeat;

static const prog_char Heartbeat[] = ": Lub-Dub\r\n";

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
    int HeartbeatCount = 0;

    //
    // Initialize the LED port
    //
    UARTInit();
    TimerInit();

    HeartbeatTimer = SECONDS(HEARTBEAT_SECS);
    SendHeartbeat  = false;

//   set_sleep_mode(SLEEP_MODE_IDLE);
//    sleep_enable();

    sei();                              // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintCRLF();
    PrintString("TimerMS Test\r\n");    // String is in RAM

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {
        TimerUpdate();
//        sleep_cpu();                    // Wait for timer

        if( SendHeartbeat ) {
            PrintD(HeartbeatCount++,0);
            PrintStringP(Heartbeat);
            SendHeartbeat = false;
            }
        } 
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TimerISR - Called by the timer section once a tick
//
// Don't send msgs directly from an ISR, set a flag that the main routine can see.
//
// Inputs:      None.
//
// Outputs:     None.
//
void TimerISR(void) {

    if( --HeartbeatTimer > 0 )          // Time to change state?
        return;                         // Nope - return

    HeartbeatTimer = SECONDS(HEARTBEAT_SECS);
    SendHeartbeat  = true;              // Set flag - time for new heartbeat
    }
