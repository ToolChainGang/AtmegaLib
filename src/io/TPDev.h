////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TPDev.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In TPDev.h
//      //
//      ...Choose port connections         (Default: Timer0)
//      ...Choose Polled or interrupt mode (Default: Interrupt)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      TPDevInit();                        // Called once at startup
//
//      while(1) {
//          TPDevUpdate();                  // Update touch panel status
//
//          if( TPDev.IsTouched )           // Do something whrn 
//              if( TPDev.XPos > BtnLeft  &&
//                  TPDev.XPos < BtnRight &&
//                  TPDev.YPos > BtnBot   &&
//                  TPDev.YPos < BtnTop  ) PrintString("Box Selected!\n");
//              else                       PrintString("Box Unselected!\n");
//              }
//          }
//
//  DESCRIPTION
//
//      A simple touchpad device driver for AVR processors
//
//      Check whether the user is touching the touchpad, and update the XPos, YPos,
//        and ZPos accordingly.
//
//  NOTES:
//
//      Because the interface assumes the application uses the touchpad digital I/O pins
//        for other purposes (which is true for the Ardiono LCD shield), it saves and
//        restores the DDRx interface ports at each update.
//
//      This is done with interrupts disabled, to prevent this from causing problems with
//        other interrupt routines.
//
//      If you are calling the TPDevUpdate function from an interrupt routine, set the
//        #ifdef below to disable the interrupt lock.
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

#ifndef TPDEV_H
#define TPDEV_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Specify the touch panel connections
//
// The following configuration is correct for the Arduino LCD shield
//
//#define TPAnalog_PORT   C               // Analog  port on processor
//#define TPAnX_PIN       3               // Analog  pin that reads X values
//#define TPAnY_PIN       2               // Analog  pin that reads Y values
//#define TPDigital_PORT  B               // Digital port connected to touchpad
//#define TPDiX_PIN       0               // Digital pin that energises X values
//#define TPDiY_PIN       0               // Digital pin that energises Y values

#define TPAnalog_PORT   C               // Analog  port on processor
#define TPYP_PIN        2               // Y plus
#define TPXM_PIN        3               // X minus
#define TPDigital_PORT  B               // Digital port connected to touchpad
#define TPYM_PIN        0               // Y minus
#define TPXP_PIN        1               // X plus

//
// Comment this next to disable the CLI/SPI interrupt lock in update
//
#define TPDEV_DISABLE_INTS

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int    XPos;
    int    YPos;
    int    ZPos;
    } TPDEV;

extern TPDEV TPDev;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TPDevInit - Initialize the touch panel device
//
// Inputs:      None.
//
// Outputs:     None.
//
void TPDevInit(void);

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
// TPDevUpdate - Read the touch panel and update the status
//
// Inputs:      None.
//
// Outputs:     None.
//
void TPDevUpdate(void);

#endif // TPDEV_H - entire file
