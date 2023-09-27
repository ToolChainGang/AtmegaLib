////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2016 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      TPDev.c
//
//  DESCRIPTION
//
//      See TPDev.h for complete description.
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

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "TPDev.h"
#include "AtoDInline.h"
#include "PortMacros.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TPDEV TPDev;

//
// Utility macros that make the code below a LOT less confusing!
//
#define XP_INPUT                                                                                                        \
    _CLR_BIT( _DDR(TPDigital_PORT),TPXP_PIN);                                                                           \
    _CLR_BIT(_PORT(TPDigital_PORT),TPXP_PIN);                                                                           \

#define XM_INPUT                                                                                                        \
    _CLR_BIT( _DDR(TPAnalog_PORT) ,TPXM_PIN);                                                                           \
    _CLR_BIT(_PORT(TPAnalog_PORT) ,TPXM_PIN);                                                                           \

#define YP_INPUT                                                                                                        \
    _CLR_BIT( _DDR(TPAnalog_PORT) ,TPYP_PIN);                                                                           \
    _CLR_BIT(_PORT(TPAnalog_PORT) ,TPYP_PIN);                                                                           \

#define YM_INPUT                                                                                                        \
    _CLR_BIT( _DDR(TPDigital_PORT),TPYM_PIN);                                                                           \
    _CLR_BIT(_PORT(TPDigital_PORT),TPYM_PIN);                                                                           \

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XP_HIGH                                                                                                         \
    _SET_BIT( _DDR(TPDigital_PORT),TPXP_PIN);                                                                           \
    _SET_BIT(_PORT(TPDigital_PORT),TPXP_PIN);                                                                           \

#define XM_HIGH                                                                                                         \
    _SET_BIT( _DDR(TPAnalog_PORT) ,TPXM_PIN);                                                                           \
    _SET_BIT(_PORT(TPAnalog_PORT) ,TPXM_PIN);                                                                           \

#define YP_HIGH                                                                                                         \
    _SET_BIT( _DDR(TPAnalog_PORT) ,TPYP_PIN);                                                                           \
    _SET_BIT(_PORT(TPAnalog_PORT) ,TPYP_PIN);                                                                           \

#define YM_HIGH                                                                                                         \
    _SET_BIT( _DDR(TPDigital_PORT),TPYM_PIN);                                                                           \
    _SET_BIT(_PORT(TPDigital_PORT),TPYM_PIN);                                                                           \

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XP_LOW                                                                                                          \
    _SET_BIT( _DDR(TPDigital_PORT),TPXP_PIN);                                                                           \
    _CLR_BIT(_PORT(TPDigital_PORT),TPXP_PIN);                                                                           \

#define XM_LOW                                                                                                          \
    _SET_BIT( _DDR(TPAnalog_PORT) ,TPXM_PIN);                                                                           \
    _CLR_BIT(_PORT(TPAnalog_PORT) ,TPXM_PIN);                                                                           \

#define YP_LOW                                                                                                          \
    _SET_BIT( _DDR(TPAnalog_PORT) ,TPYP_PIN);                                                                           \
    _CLR_BIT(_PORT(TPAnalog_PORT) ,TPYP_PIN);                                                                           \

#define YM_LOW                                                                                                          \
    _SET_BIT( _DDR(TPDigital_PORT),TPYM_PIN);                                                                           \
    _CLR_BIT(_PORT(TPDigital_PORT),TPYM_PIN);                                                                           \

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TPDevInit - Initialize the touch panel device
//
// Inputs:      None.
//
// Outputs:     None.
//
void TPDevInit(void) {

    memset(&TPDev,0,sizeof(TPDev));

    AtoDInit;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TPDevUpdate - Read the touch panel and update the status
//
// Inputs:      None.
//
// Outputs:     None.
//
void TPDevUpdate(void) {
#define NUMSAMPLES 2

  int samples[NUMSAMPLES];
  uint8_t i, valid;
  
  valid = 1;

    YP_INPUT;   // Set Y+ to Hi-Z
    YM_INPUT;   // Set Y- to Hi-Z
    XP_HIGH;    // Set X+ to VCC
    XM_LOW;     // Set X- to GND

   for (i=0; i<NUMSAMPLES; i++) {
    AtoDRead(TPYP_PIN,samples[i]);
   }
   // Allow small amount of measurement noise, because capacitive
   // coupling to a TFT display's signals can induce some noise.
   if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4) {
     valid = 0;
   } else {
     samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
   }

   TPDev.XPos = (1023-samples[NUMSAMPLES/2]);

    XP_INPUT;   // Set X+ to Hi-Z
    XM_INPUT;   // Set X- to Hi-Z
    YP_HIGH;    // Set Y+ to VCC
    YM_LOW;     // Set Y- to GND




   for (i=0; i<NUMSAMPLES; i++) {
    AtoDRead(TPXM_PIN,samples[i]);
   }

   // Allow small amount of measurement noise, because capacitive
   // coupling to a TFT display's signals can induce some noise.
   if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4) {
     valid = 0;
   } else {
     samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
   }


   TPDev.YPos = (1023-samples[NUMSAMPLES/2]);



   if (! valid) TPDev.ZPos = 0;
   else         TPDev.ZPos = 600;


#if 0
    int     V1, V2;
    bool    Valid = true;

    //
    // Turn off interrupts and save the port directions and states
    //
#ifdef TPDEV_DISABLE_INTS
    cli();
#endif

    uint8_t SavedAnalogPort  = _PORT(TPAnalog_PORT);
    uint8_t SavedDigitalPort = _PORT(TPDigital_PORT);
    uint8_t SavedAnalogDDR   = _DDR(TPAnalog_PORT);
    uint8_t SavedDigitalDDR  = _DDR(TPDigital_PORT);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // X POSITION
    //

    YP_INPUT;   // Set Y+ to Hi-Z
    YM_INPUT;   // Set Y- to Hi-Z
    XP_HIGH;    // Set X+ to VCC
    XM_LOW;     // Set X- to GND

    AtoDRead(TPYP_PIN,V1);
    AtoDRead(TPYP_PIN,V2);

    TPDev.XPos = 1023 - ((V1 + V2) >> 1);

    Valid &= (V1 - V2) < 4 && (V1 - V2) > -4;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Y POSITION
    //
    XP_INPUT;   // Set X+ to Hi-Z
    XM_INPUT;   // Set X- to Hi-Z
    YP_HIGH;    // Set Y+ to VCC
    YM_LOW;     // Set Y- to GND

    AtoDRead(TPXM_PIN,V1);
    AtoDRead(TPXM_PIN,V2);

    TPDev.YPos = 1023 - ((V1 + V2) >> 1);

    Valid &= (V1 - V2) < 4 && (V1 - V2) > -4;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Z POSITION (pressure)
    //
    XP_LOW;     // Set X+ to GND
    YM_HIGH;    // Set Y- to VCC
    XM_INPUT;   // Set X- to Hi-Z
    YP_INPUT;   // Set Y+ to Hi-Z

    int Z1, Z2;
    
    AtoDRead(TPXM_PIN,Z1);
    AtoDRead(TPYP_PIN,Z2);

    TPDev.ZPos = 1023 - (Z2-Z1);

    if( !Valid )
        TPDev.ZPos = 0;

    //
    // Restore previous port state and direction, restore interrupts
    //
    _PORT(TPAnalog_PORT)  = SavedAnalogPort;
    _PORT(TPDigital_PORT) = SavedDigitalPort;
    _DDR(TPAnalog_PORT)   = SavedAnalogDDR;
    _DDR(TPDigital_PORT)  = SavedDigitalDDR;

#ifdef TPDEV_DISABLE_INTS
    sei();
#endif
#endif
    }
