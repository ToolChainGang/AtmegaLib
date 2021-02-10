//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      MotorPWM.h
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
//  VERSION:    2015.01.05
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

#ifndef MOTORPWM_H
#define MOTORPWM_H

#include <Motor.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify a Timer to use for PWM
//
// See Timer.h for explanation and list of examples.
//
// NOTE: Useful to have CLOCK_COUNTB be a multiple of 100 for pct calculations
//
#define TIMERPWM_ID     2                         // use TIMER2
#define PWM_CCRA        (1 << WGM21)
#define PWM_CCRB        (1 <<  CS22) | (1 <<  CS21)
#define CLOCK_COUNTA    200

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MotorPWMInit - Initialize motor system
//
// Inputs:      None.
//
// Outputs:     None.
//
void MotorPWMInit(void);

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
void MotorPWM(uint8_t ID,int8_t SpeedPct);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetMotorPWM - Return motor state
//
// Inputs:      ID of motor to control (integer index of motor)
//
// Outputs:     Speed of motor
//
uint8_t GetMotorPWM(uint8_t ID);


#endif  // MOTORPWM_H - entire file
