//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2013 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      SPI.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In SPI.h
//      //
//      ...Choose a speed                  (Default: FOsc/4)
//      ...Choose clock/data phase         (Default: Mode0)
//      ...Chose whether to use ISP_PreData or ISP_PostData
//
//      //////////////////////////////////////
//      //
//      // In main.c
//      //
//      SPIInit();                          // Called once at startup
//
//      bool Success = PutSPIByte('A');     // == FALSE if buffer was full
//
//      PutSPIByteW('A');                   // Block until complete
//
//      If( SPIBusy() ) ...                 // TRUE if sending something
//
//      char ISP_PreData (char Data) {...}   // Preprocess  before data send
//      void ISP_PostData(char Data) {...}   // Postprocess after  data send
//
//  DESCRIPTION
//
//      A simple serial SPI driver module for interrupt driven communications
//        on an AVR processor.
//
//      The baud rate and FIFO sizes are set in the SPI.h file.
//
//  VERSION:    2013.01.06
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

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Choose the speed based on the table in the hardware manual
//
//  SPI2X SPR1 SPR0     Frequency
//  ===== ==== ====     =========
//      0    0    0     F_osc/4
//      0    0    1     F_osc/16
//      0    1    0     F_osc/64
//      0    1    1     F_osc/128
//      1    0    0     F_osc/2
//      1    0    1     F_osc/8
//      1    1    0     F_osc/32
//      1    1    1     F_osc/64
//
#ifndef SPI_SPEED
//#define SPI_SPEED       ((1 << SPR0) | (1 << SPR1))
#define SPI_SPEED       0
#endif

//#define SPEED2X

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Choose the clock/data phase based on the table in the hardware manual
//
#ifndef MODE
//#define MODE            (CPOL | CPHA)
#define MODE            0
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// void ISP_PreData(void)  is supplied by the user (if needed) to perform
//   pre-data processing, such as toggling a latch output.
//
// void ISP_PostData(void) is supplied by the user (if needed) to perform
//    post-data processing, such as toggling a latch output.
//
// Defined (ie - uncommented) means call SPI_XXX Undefined (commented out)
//   means don't.
// 
//#define CALL_ISP_PREDATA
#define CALL_ISP_POSTDATA

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// The FIFO's must be a power of two long each, since the code
//   uses binary wraparounds to access.
//
#ifndef SPI_OFIFO_SIZE
#define SPI_OFIFO_SIZE  (1 << 6)        // == 64 bytes Tx FIFO
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Sigh. The standard Atmega 168 file doesn't define these.
//
#ifndef SPI_PORT

#define SPI_PORT    B                   // Port containing MISO, MOSI, &c

#define SS_BIT      2                   // Bits in SPI port
#define MOSI_BIT    3
#define MISO_BIT    4
#define SCK_BIT     5

#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SPIInit - Initialize SPI
//
// This routine initializes the SPI based on the settings above. Called from
//   init. Also disables the Tx interrupt and clears the FIFOs.
//
// Inputs:      None.
//
// Outputs:     None.
//
void SPIInit(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PutSPIByte - Send one byte out the SPI port
//
// Send a byte out the SPI port. We stuff the byte into the FIFO and enable
//   interrupts - at some point the interrupt handler will get serviced and
//   send the byte out for us.
//
// Inputs:      Byte to send
//
// Outputs:     TRUE  if byte was sent OK,
//              FALSE if buffer full
//
bool PutSPIByte(uint8_t OutByte);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// PutSPIByteW - Send one byte out the serial port, wait for completion
//
// Like PutSPIByte, but will block [if no FIFO space] until complete.
//
// Inputs:      Byte to send
//
// Outputs:     None.
//
#define PutSPIByteW(_OutByte_) { while(!PutSPIByte(_OutByte_)); }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// SPIBusy - Return TRUE if SPI is busy sending output
//
// Inputs:      None.
//
// Outputs:     TRUE  if SPI is busy sending output
//              FALSE if SPI is idle
//
bool SPIBusy(void);

#endif // SPI_H - entire file
