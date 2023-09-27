////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      Servo.c
//
//  SYNOPSIS
//
//      See Servo.h for synopsis
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

#include "Servo.h"
#include "PortMacros.h"
#include "TimerMacros.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct {
             uint8_t  Pos;      // Position, per servo
             uint8_t  Mask;     // Bit (mask) within port, per servo
    volatile uint8_t *Port;     // Port for each servo
    } Servos[NUM_SERVOS] __attribute__ ((section (".noinit")));

static uint8_t MinPos    NOINIT;
static uint8_t OFloCount NOINIT; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define PRTIMx      _PRTIM(SERVO_TIMER)
#define TIMER_ISRA  _TCOMPA_VECT(SERVO_TIMER)
#define TIMER_ISRB  _TCOMPB_VECT(SERVO_TIMER)

#define TCNTx       _TCNT(SERVO_TIMER)
#define OCRAx       _OCRA(SERVO_TIMER)
#define OCRBx       _OCRB(SERVO_TIMER)

#define TCCRAx      _TCCRA(SERVO_TIMER)
#define TCCRBx      _TCCRB(SERVO_TIMER)
#define TIMSKx      _TIMSK(SERVO_TIMER)
#define OCIEAx      _OCIEA(SERVO_TIMER)
#define OCIEBx      _OCIEB(SERVO_TIMER)

#define OCIB_ON     _SET_BIT(TIMSKx,OCIEBx)
#define OCIB_OFF    _CLR_BIT(TIMSKx,OCIEBx)

#define SERVO_PIN_ON(_id_)  { *(Servos[_id_].Port) |=  Servos[_id_].Mask; }
#define SERVO_PIN_OFF(_id_) { *(Servos[_id_].Port) &= ~Servos[_id_].Mask; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ServoInit - Initialize servo system
//
// Inputs:      None.
//
// Outputs:     None.
//
void ServoInit(void) {

    MinPos = 0;

#if NUM_SERVOS > 0
        Servos[0].Pos  = SERVO_OFF;
        Servos[0].Mask = _PIN_MASK(Servo0_BIT);
        Servos[0].Port = &_PORT(Servo0_PORT);
        _SET_BIT(_DDR(Servo0_PORT),Servo0_BIT);
        SERVO_PIN_OFF(0);
#       endif

#if NUM_SERVOS > 1
        Servos[1].Pos  = SERVO_OFF;
        Servos[1].Mask = _PIN_MASK(Servo1_BIT);
        Servos[1].Port = &_PORT(Servo1_PORT);
        _SET_BIT(_DDR(Servo1_PORT),Servo1_BIT);
        SERVO_PIN_OFF(1);
#       endif

#if NUM_SERVOS > 2
        Servos[2].Pos  = SERVO_OFF;
        Servos[2].Mask = _PIN_MASK(Servo2_BIT);
        Servos[2].Port = &_PORT(Servo2_PORT);
        _SET_BIT(_DDR(Servo2_PORT),Servo2_BIT);
        SERVO_PIN_OFF(2);
#       endif

#if NUM_SERVOS > 3
        Servos[3].Pos  = SERVO_OFF;
        Servos[3].Mask = _PIN_MASK(Servo3_BIT);
        Servos[3].Port = &_PORT(Servo3_PORT);
        _SET_BIT(_DDR(Servo3_PORT),Servo3_BIT);
        SERVO_PIN_OFF(3);
#       endif

#if NUM_SERVOS > 4
#   error "Too many servos defined!"
#   endif

    OFloCount = OFLO_MODULUS;

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running, with OCRA as top value
    //
    TCCRAx = SERVO_CCRA;            // No output compare functions
    TCCRBx = SERVO_CCRB;            // Set appropriate clock
    OCRAx  = SERVO_COUNTA;          // And clock count for ticks
    TCNTx  = 0;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Servo - Set servo speed
//
// Inputs:      ID of servo to control (integer index of servo)
//              Position 0 -> 100 or SERVO_OFF
//
// Outputs:     None.
//
void Servo(uint8_t ID,uint8_t Pos) {

    Servos[ID].Pos = Pos;

    if( Pos == SERVO_OFF ) SERVO_PIN_OFF(ID);

    //
    // Calculate the first OCRB interrupt now, so the interrupt handler
    //   doesn't have to do it
    //
    uint8_t MinPos = SERVO_COUNTA;

    for( uint8_t ID = 0; ID < NUM_SERVOS; ID++ ) {
        if( Servos[ID].Pos != 255   &&
            Servos[ID].Pos < MinPos )
            MinPos = Servos[ID].Pos;
        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GetServo - Return servo state
//
// Inputs:      ID of servo (integer index of motor)
//
// Outputs:     Position 0 -> 100 or SERVO_OFF
//
uint8_t GetServo(uint8_t ID) { 

    return Servos[ID].Pos;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_COMPA_vect - Compare A causes a timer tick
//
// This defines the "tick" of the pwm wave. Engage all non-stopped servos and
//   set up the pulse width interrupt.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_ISRA,ISR_BLOCK) {

    OFloCount--;

    //
    // Return if it's not time yet for a pulse.
    //
    if( OFloCount > 1 )
        return;

    //
    // At the last of the MODULOs, start a MIN_PULSE
    //
    if( OFloCount == 1 ) {
        //
        // Turn on all enabled servos.
        //
        for( uint8_t ID = 0; ID < NUM_SERVOS; ID++ ) {
        
            if( Servos[ID].Pos == SERVO_OFF )
                continue;

            SERVO_PIN_ON(ID);
            }

        OCRAx = MIN_PULSE;              // Set time for next interrupt
        return;
        }

    OCRAx     = SERVO_COUNTA;           // Set time for A next interrupt
    OFloCount = OFLO_MODULUS;

    //
    // Set the compare to interrupt us when the MinPos is due
    //
    OCRBx = MinPos;                     // Set time for B interrupt

    if( MinPos < SERVO_COUNTA )
        OCIB_ON;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_COMPB_vect - Compare B causes a timer tick
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_ISRB,ISR_BLOCK) {
    uint8_t CurrentPos = TCNT2;
    uint8_t NextPos    = SERVO_COUNTA;

    //
    // For some reason, the match on OCRA seems to trigger a corresponding
    //   match on OCRB in all cases (with TCNT2 == 0).
    //
    // I don't know why this is,
    //
    if( TCNT2 == 0 )
        return;

    for( uint8_t ID = 0; ID < NUM_SERVOS; ID++ ) {
        uint8_t Pos = Servos[ID].Pos;

        if( Pos == SERVO_OFF )
            continue;

        if     ( Pos <= CurrentPos ) SERVO_PIN_OFF(ID)
        else if( Pos <     NextPos ) NextPos = Servos[ID].Pos;
        }

    //
    // Set the compare to interrupt us when the NextPct is due
    //
    OCRBx = NextPos;            // Add clock count for ticks

    //
    // If all motors full on (or off), no need to do anything.
    //  
    if( NextPos == SERVO_COUNTA )
        OCIB_OFF;
    }
