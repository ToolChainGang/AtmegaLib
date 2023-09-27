////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      AtoD.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In AtoD.h
//      //
//      #define NUM_ATOD  3
//
//      ...Choose Polled or interrupt mode   (Default: Interrupt)
//      ...Choose continuous or command mode (Default: Continuous)
//
//      //////////////////////////////////////
//      //
//      // Polled mode
//      //
//      AtoDInit();                         // Called once at startup
//          :
//
//      while(1) {
//
//          AtoDStart();                    // Start conversions
//          while( !AtoDReady )
//              sleep_cpu();                // Wait for completion
//
//          AtoDReady = false;              // Reset flag
//          int Val = AtoD(1);              // Get AtoD value
//              :           :               // Do local updates
//          }
//
//      //////////////////////////////////////
//      //
//      // Interrupt mode
//      //
//      AtoDInit();                         // Called once at startup
//
//      AtoDStart();                        // Start conversions
//
//      void AtoDISR(void) {                // Called once every tick
//          int Val = AtoD(1);              // Get AtoD value
//          ...do all update functions
//          }
//
//
//  DESCRIPTION
//
//      Simple AtoD processing
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

#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>

#include "PortMacros.h"
#include "AtoD.h"

///////////////////////////////////////////////////////////////////////////////

static struct {
    uint8_t     CurrentChannel;                 // Current conversion
    uint16_t    Channels[NUM_ATOD];             // Value of conversions
    } AtoD NOINIT;

#define START_ATOD  { _SET_BIT(ADCSRA,ADSC); }  // Start the AtoD conversion
#define ADMUX_VAL   (_PIN_MASK(REFS0))
#define TEMP_CHANNEL    9
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AtoDInit - Initialize AtoD engine
//
// Inputs:      None.
//
// Outputs:     None.
//
void AtoDInit(void) {

    memset(&AtoD,0,sizeof(AtoD));

    //
    // Setup AtoD channels for input
    //
    _CLR_BIT(PRR,PRADC);                    // Powerup the A/D converter

    DIDR0  = 0;                             // Turn off digital outputs
    ADCSRB = 0;                             // Free running mode
    ADMUX  = ADMUX_VAL;                     // AVCC as ref, number of channels
    ADCSRA = _PIN_MASK(ADPS2) | _PIN_MASK(ADPS1) | _PIN_MASK(ADPS0) |   // Prescale to 150 KHz
             _PIN_MASK(ADEN)  | _PIN_MASK(ADIE);                        // Enable, Enable int

    AtoD.CurrentChannel = NUM_ATOD;

    //
    // If user wants continuous outputs, start the conversion
    //
#ifdef  ContinuousAtoD
    START_ATOD;
#endif
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// StartAtoD - Start the AtoD conversion sequence
//
// Inputs:      None.
//
// Outputs:     None.
//
void StartAtoD(void) { 

    if( !AtoDComplete() )                   // Return if already in progress
        return;

    AtoD.CurrentChannel = 0;

    ADMUX = ADMUX_VAL | 0;                  // Start at channel 0
    START_ATOD;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AtoDComplete - Check to see if AtoD processing is done
//
// Inputs:      None.
//
// Outputs:     TRUE  if all AtoD channels have been read
//              FALSE if caller whould wait some more
//
bool AtoDComplete(void) { return(AtoD.CurrentChannel == NUM_ATOD); }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GetAtoD - Return AtoD Channel
//
// Inputs:      AtoD channel to return
//
// Outputs:     Value of measured AtoD
//
uint16_t GetAtoD(uint8_t Index) {

    return(AtoD.Channels[Index]);
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ADC_vect - A/D interrupt processing
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(ADC_vect,ISR_NOBLOCK) {

    if( AtoD.CurrentChannel == NUM_ATOD )       // Ignore spurious interrupts
        return;

    //
    // Grab the current conversion
    //
    AtoD.Channels[AtoD.CurrentChannel++] = ADCW;

    //
    // If not complete, start the next conversion
    //
    if( AtoD.CurrentChannel < NUM_ATOD ) {

        ADMUX = ADMUX_VAL | AtoD.CurrentChannel;
        //
        // Temp sensor requires internal 1.1v ref
        //
        if( AtoD.CurrentChannel == TEMP_CHANNEL )
            _SET_BIT(ADMUX,REFS1);

        START_ATOD;
        return;
        }

    //
    // Call the user's function
    //
#ifdef CALL_AtoDISR
    AtoDISR();
#endif

#ifdef  ContinuousAtoD
    StartAtoD();
#endif
    }
