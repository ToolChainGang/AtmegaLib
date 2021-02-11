////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      DigitalPotTest.C
//
//  SYNOPSIS
//
//      Connect a digital pot to the SPI port,
//
//      Compile, load, and run this module. The pot should:
//
//          Set wiper to 0
//          Wait 2 secs
//          Set wiper to 50
//          Wait 2 secs
//          Set wiper to 100
//          Wait 2 secs
//          Set wiper to 150
//          Wait 2 secs
//
//      Then repeat.
//
//  DESCRIPTION
//
//      This is a simple AVR program with digital potentiometer control.
//
//      It is used to verify and test digital potentiometer connections.
//
//  VERSION:    2015.06.20
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

#include <PortMacros.h>

#include "UART.h"
#include "Serial.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4131
//
#if 0
#include "MCP4131.h"

#define TestPot_PORT                        D
#define TestPot_BIT                         3
#define TestPot_MAXR                    50000
#define TestPot_MAX_WIPER   (MCP4131_STEPS-1)

#define TestPotInit             MCP4131Init(TestPot_PORT,TestPot_BIT)
#define TestPotSetWiper(_w_)    MCP4131SetWiper(TestPot_PORT,TestPot_BIT,_w_)
#define TestPotSetResist(_r_)   MCP4131SetResist(TestPot_PORT,TestPot_BIT,TestPot_MAXR,_r_)
#define TestPotIncr             MCP4131Incr(TestPot_PORT,TestPot_BIT)
#define TestPotDecr             MCP4131Decr(TestPot_PORT,TestPot_BIT)

#define TestPotR2W(_r_)         MCP4131_R2W(TestPot_MAXR,_r_)
#define TestPotW2R(_w_)         MCP4131_W2R(TestPot_MAXR,_w_)
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MCP4161
//
#if 0
#include "MCP4161.h"

#define TestPot_PORT                        D
#define TestPot_BIT                         2
#define TestPot_MAXR                    10000
#define TestPot_MAX_WIPER   (MCP4161_STEPS-1)

#define TestPotInit             MCP4161Init(TestPot_PORT,TestPot_BIT)
#define TestPotSetWiper(_w_)    MCP4161SetWiper(TestPot_PORT,TestPot_BIT,_w_)
#define TestPotSetResist(_r_)   MCP4161SetResist(TestPot_PORT,TestPot_BIT,TestPot_MAXR,_r_)
#define TestPotIncr             MCP4161Incr(TestPot_PORT,TestPot_BIT)
#define TestPotDecr             MCP4161Decr(TestPot_PORT,TestPot_BIT)

#define TestPotR2W(_r_)         MCP4161_R2W(TestPot_MAXR,_r_)
#define TestPotW2R(_w_)         MCP4161_W2R(TestPot_MAXR,_w_)
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AD8400
//
#if 1
#include "AD8400.h"

#define TestPot_PORT                        D
#define TestPot_BIT                         6
#define TestPot_MAXR                     1000
#define TestPot_MAX_WIPER   (AD8400_STEPS-1)

#define TestPotInit             AD8400Init(TestPot_PORT,TestPot_BIT)
#define TestPotSetWiper(_w_)    AD8400SetWiper(TestPot_PORT,TestPot_BIT,_w_)
#define TestPotSetResist(_r_)   AD8400SetResist(TestPot_PORT,TestPot_BIT,TestPot_MAXR,_r_)
#define TestPotIncr             AD8400Incr(TestPot_PORT,TestPot_BIT)
#define TestPotDecr             AD8400Decr(TestPot_PORT,TestPot_BIT)

#define TestPotR2W(_r_)         AD8400_R2W(TestPot_MAXR,_r_)
#define TestPotW2R(_w_)         AD8400_W2R(TestPot_MAXR,_w_)
#endif


#define PAUSE_MS    2000

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// DigitalPotTest - Command the digital potentiometer software
//
// Inputs:      None. (Embedded program - no command line options)
//
// Outputs:     None. (Never returns)
//
int __attribute__((OS_main))  main(void) {

    int Round = 0;

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the UART
    //
    SPIInit;
    TestPotInit;

#if 0
while(1) {
    TestPotSetWiper(4);
    _delay_us(2);                // Wait a bit
    }


    TestPotSetWiper(0x70);

while(1) {


//    _CLR_BIT(_PORT(MCP4161_CS_PORT),MCP4161_CS_BIT);
//    PutSPIByte(MCP4161_WRITE);
//    PutSPIByte(0x05);
//    _SET_BIT(_PORT(MCP4161_CS_PORT),MCP4161_CS_BIT);

    _delay_us(2);                // Wait a bit
    }

#endif


    UARTInit();

    sei();                                          // Enable interrupts

    PrintCRLF();
    PrintCRLF();
    PrintString("Begin digital pot control.\r\n");  // String is in RAM

    //////////////////////////////////////////////////////////////////////////////////////
    //
    // All done with init,
    // 
    while(1) {

        PrintD(Round++,0);
        PrintString(": ");

        PrintString("Wiper 0, ");
        TestPotSetWiper(0);

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintString("Wiper 50, ");
        TestPotSetWiper(50);

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintString("Wiper 100, ");
        TestPotSetWiper(100);

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintString("Wiper 150");
        TestPotSetWiper(150);

        _delay_ms(PAUSE_MS);                // Wait a bit

        PrintCRLF();
        }
    }
