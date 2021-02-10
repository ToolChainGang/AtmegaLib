//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      MotorPWM.c
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Motor.h
//      //
//      #define NUM_MOTORS  3
//
//      #define MOTOR1_PORT d           // Port connected to motor
//      #define MOTOR1_POS  0           // Bit connected to pos wire
//      #define MOTOR1_NEG  0           // Bit connected to neg wire
//      #define MOTOR1_NEG_LOGIC        // Define as needed
//              :
//      (Continue for NUM_MOTORS)
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//      Motor(1, 50);                   // Engage MOTOR1 FWD at 50% speed
//      Motor(1,-50);                   // Engage MOTOR1 REV at 50% speed
//      Motor(1,  0);                   // Turn   MOTOR1 OFF
//
//  DESCRIPTION
//
//      Motor PWM processing for simple H-bridge motors
//
//  EXAMPLE
//
//      MotorPWMInit();                 // Initialize the motor system
//
//      sei();                          // Enable interrupts
//
//      while(1) {
//          MotorPWM(1,50);             // Engage MOTOR1 in FWD dir at half speed
//          _delay_ms(500);             // Wait 1/2 sec
//          MotorPWM(1,-50);            // Engage MOTOR1 in REV dir at half speed
//          _delay_ms(500);             // Wait 1/2 sec
//          MotorPWM(1,0);              // Rutn off MOTOR1
//          _delay_ms(500);             // Wait 1/2 sec
//          }
//
//  NOTES
//
//      Which wire (of the motor) are POS and NEG are arbitrary.
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

#include <string.h>
#include <avr/interrupt.h>

#include <PortMacros.h>
#include <TimerMacros.h>

#include "MotorPWM.h"

//////////////////////////////////////////////////////////////////////////////////////////

static struct {
    uint8_t AbsPct[NUM_MOTORS];         // Speed percentage, per motor
    uint8_t NegPct[NUM_MOTORS];         // == 1 if speed is reverse
    uint8_t MinPct;                     // == Min non-zero speed
    } MotorData NOINIT;

//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some port designations
//
#define PRTIMx      _PRTIM(TIMERPWM_ID)
#define TIMER_ISRA  _TCOMPA_VECT(TIMERPWM_ID)
#define TIMER_ISRB  _TCOMPB_VECT(TIMERPWM_ID)

#define TCNTx       _TCNT(TIMERPWM_ID)
#define OCRAx       _OCRA(TIMERPWM_ID)
#define OCRBx       _OCRB(TIMERPWM_ID)

#define TCCRAx      _TCCRA(TIMERPWM_ID)
#define TCCRBx      _TCCRB(TIMERPWM_ID)
#define TIMSKx      _TIMSK(TIMERPWM_ID)
#define OCIEAx      _OCIEA(TIMERPWM_ID)
#define OCIEBx      _OCIEB(TIMERPWM_ID)

#define OCIB_ON     _SET_BIT(TIMSKx,OCIEBx)
#define OCIB_OFF    _CLR_BIT(TIMSKx,OCIEBx)

#define ABS(x)           (((x) < 0) ? -(x) : (x))
#define NEG(x)           (((x) < 0) ?    1 :   0)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MotorPWMInit - Initialize motor system
//
// Inputs:      None.
//
// Outputs:     None.
//
void MotorPWMInit(void) {

    memset(&MotorData,0,sizeof(MotorData));

    MotorInit();

    for( uint8_t ID = 0; ID < NUM_MOTORS; ID++ )
        Motor(ID,MOTOR_OFF);

    _CLR_BIT(PRR,PRTIMx);           // Powerup the clock

    //
    // Setup the timer as free running, with OCRA as top value
    //
    TCCRAx = PWM_CCRA;              // No output compare functions
    TCCRBx = PWM_CCRB;              // Set appropriate clock
    OCRAx  = CLOCK_COUNTA;          // And clock count for ticks
    TCNTx  = 0;
    TIMSKx = _PIN_MASK(OCIEAx);     // Allow interrupts
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MotorPWM - Set motor control speed
//
// Inputs:      ID of motor to control (integer index of motor)
//              Percent speed -100 -> 0 -> +100
//
// Outputs:     None.
//
void MotorPWM(uint8_t ID,int8_t SpeedPct) {

    MotorData.AbsPct[ID] = 2*ABS(SpeedPct);
    MotorData.NegPct[ID] =   NEG(SpeedPct);

    //
    // Handle [0,100] now as needed, so that the interrupt handler doesn't have
    //   to do it
    //
    if( MotorData.AbsPct[ID] ==   0 )
        Motor(ID,MOTOR_OFF); 

    if( MotorData.AbsPct[ID] == 200 )
        Motor(ID,MOTOR_FWD+MotorData.NegPct[ID]);

    //
    // Calculate the first OCRB interrupt now, so the interrupt handler
    //   doesn't have to do it
    //
    uint8_t MinPct = 200;

    for( uint8_t ID = 0; ID < NUM_MOTORS; ID++ ) {
        if( MotorData.AbsPct[ID] > 0      &&
            MotorData.AbsPct[ID] < MinPct )
            MinPct = MotorData.AbsPct[ID];
        }

    MotorData.MinPct = MinPct;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetMotorPWM - Return motor state
//
// Inputs:      ID of motor to control (integer index of motor)
//
// Outputs:     Speed of motor
//
uint8_t GetMotorPWM(uint8_t ID) { 

    int8_t Speed = MotorData.AbsPct[ID] << 1;

    if( MotorData.NegPct[ID] )
        Speed = -Speed;

    return Speed;
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_COMPA_vect - Compare A causes a timer tick
//
// This defines the "tick" of the pwm wave. Engage all non-stopped motors and
//   set up the pulse width interrupt.
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_ISRA,ISR_BLOCK) {
    //
    // Turn all the motors on, in the appropriate direction. Keep
    //   track of the smallest non-zero speed.
    //
    for( uint8_t ID = 0; ID < NUM_MOTORS; ID++ ) {

        if( MotorData.AbsPct[ID] == 0 ||
            MotorData.AbsPct[ID] == 200 )
            continue;

        Motor(ID,MOTOR_FWD+MotorData.NegPct[ID]);
        }

    //
    // Set the compare to interrupt us when the WaitForPct is due
    //
    OCRBx = MotorData.MinPct;           // Add clock count for ticks

    if( MotorData.MinPct < 200 )
        OCIB_ON;
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// TIMERx_COMPB_vect - Compare B causes a timer tick
//
// Inputs:      None. (ISR)
//
// Outputs:     None.
//
ISR(TIMER_ISRB,ISR_BLOCK) {
    uint8_t CurrentPct = TCNT2;
    uint8_t NextPct    = 200;

    //
    // For some reason, the match on OCRA seems to trigger a corresponding
    //   match on OCRB in all cases (with TCNT2 == 0).
    //
    // I don't know why this is,
    //
    if( TCNT2 == 0 )
        return;

    for( uint8_t ID = 0; ID < NUM_MOTORS; ID++ ) {
        uint8_t AbsPct = MotorData.AbsPct[ID];

        if( AbsPct == 0   ||
            AbsPct == 200 )
            continue;

        if     ( AbsPct <= CurrentPct ) Motor(ID,MOTOR_OFF);
        else if( AbsPct <     NextPct ) NextPct = MotorData.AbsPct[ID];
        }

    //
    // Set the compare to interrupt us when the NextPct is due
    //
    OCRBx = NextPct;            // Add clock count for ticks

    //
    // If all motors full on (or off), no need to do anything.
    //  
    if( NextPct == 200 )
        OCIB_OFF;
    }
