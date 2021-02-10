//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      AUART.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In AltUART.h
//      //
//      ...Choose a timer                   (Default: Timer2)
//      ...Choose baud rate                 (Default: 9600)
//      ...Choose an Rx port/pin            (Default: PortD.2)
//      ...Choose a  Tx port/pin            (Default: PortD.3)
//
//      //////////////////////////////////////
//      //
//      // In Main.c
//      //
//      AUARTInit();                        // Called once at startup
//
//      char InChar = GetAUARTByte();       // == 0 if no chars available
//
//      bool Success = PutAUARTByte('A');   // == FALSE if buffer was full
//
//      PutAUARTByteW('A');                 // Block until complete
//
//      If( AUARTBusy() ) ...               // TRUE if sending something
//
//  DESCRIPTION
//
//      An alternate serial driver which does not use the onboard UART.
//        on an AVR processor.
//
//      The baud rate and FIFO sizes can be set in the AltUART.h file
//
//  NOTES:
//
//      This interface WILL NOT receive a NUL character (ascii 0). This is on
//        purpose, to make for a simple interface.
//
//      These are not the putc() and getc() functions required for stdio
//        by WinAVR. See serial.h for those.
//
//  VERSION:    2015.11.07
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

#ifndef AUART_H
#define AUART_H

#include <stdbool.h>
#include <avr/wdt.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ABAUD
#define ABAUD           9600
#endif

//
// Specify the Rx/Tx pins
//
#define ARx_PORT        D
#define ARx_BIT         2

#define ATx_PORT        D
#define ATx_BIT         3

//
// Specify the timer to use.
//
#define ATIMER          2

//
// The serial FIFO's must be a power of two long each, since the code
//   uses binary wraparounds to access. This system has NO error detection
//   and NO XON/XOFF processing.
//
#ifndef AIFIFO_SIZE
#define AIFIFO_SIZE     (1 << 3)        // == 8  char Rx FIFO
#endif

#ifndef AOFIFO_SIZE
#define AOFIFO_SIZE     (1 << 6)        // == 64 chars Tx FIFO
#endif

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// AUARTInit - Initialize UART
//
// This routine initializes the AUART based on the settings above. Called from
//   init. Also enables the Rx interrupts, disables the Tx interrupt and
//   clears the FIFOs.
//
// Inputs:      None.
//
// Outputs:     None.
//
void AUARTInit(void);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// PutAUARTByte - Send one char out the serial port
//
// Send a char out the serial port. We stuff the char into the FIFO and enable
//   interrupts - at some point the interrupt handler will get serviced and
//   send the char out for us.
//
// Inputs:      Byte to send
//
// Outputs:     TRUE  if char was sent OK,
//              FALSE if buffer full
//
bool PutAUARTByte(char OutChar);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// PutAUARTByteW - Send one char out the serial port, wait for completion
//
// Like PutUARTByte, but will block [if no FIFO space] until complete.
//
// Inputs:      Byte to send
//
// Outputs:     None.
//
#define PutAUARTByteW(_OutChar_) { while(!PutAUARTByte(_OutChar_)); }

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// GetAUARTByte - Get one char from the serial port
//
// Get a char from the UART. The interrupt handler already received the
//   character for us, so this just pulls the char out of the receive FIFO.
//
// Inputs:      None.
//
// Outputs:     ASCII char, if one was available
//              NUL   (binary value = 0) if no chars available
//
char GetAUARTByte(void);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// AUARTBusy - Return TRUE if UART is busy sending output
//
// Inputs:      None.
//
// Outputs:     TRUE  if UART is busy sending output
//              FALSE if UART is idle
//
bool AUARTBusy(void);

#endif // AUART_H - entire file
