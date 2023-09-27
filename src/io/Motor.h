////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      Motor.h
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
//      Motor(1,MOTOR_FWD);             // Engage MOTOR1 in FWD direction
//      Motor(1,MOTOR_REV);             // Engage MOTOR1 in REV direction
//      Motor(1,MOTOR_OFF);             // Engage MOTOR1 in OFF direction
//
//  DESCRIPTION
//
//      Motor processing for simple H-bridge motors
//
//      Note that POS and NEG wires on motor are somewhat arbitrary.
//
//      MOTOR_FWD ==>  POS + ~NEG
//      MOTOR_REV ==> ~POS +  NEG
//      MOTOR_OFF ==> ~POS + ~NEG
//
//  EXAMPLE
//
//      //
//      // GO FWD for 1/2 sec, OFF for 1/2 sec, REV for 1/2 sec, &c
//      //
//      MotorInit();                    // Initialize the motor system
//
//      sei();                          // Enable interrupts
//
//      while(1) {
//          Motor(1,MOTOR_FWD);         // Engage MOTOR1 in FWD direction
//          _delay_ms(500);             // Wait 1/2 sec
//          Motor(1,MOTOR_OFF);         // Engage MOTOR1 in OFF direction
//          _delay_ms(500);             // Wait 1/2 sec
//          Motor(1,MOTOR_REV);         // Engage MOTOR1 in REV direction
//          _delay_ms(500);             // Wait 1/2 sec
//          Motor(1,MOTOR_OFF);         // Engage MOTOR1 in OFF direction
//          _delay_ms(500);             // Wait 1/2 sec
//          }
//
//  NOTES
//
//      Which wire (of the motor) are POS and NEG are arbitrary.
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

#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#include "PortMacros.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define MOTOR_NEG_LOGIC when using an inverter for logic control; ie - both outputs
//   are SET for OFF, and so on.
//
// Specify number of motors to use.
//
// For each motor, define the port it is connected to, and the POS and NEG pins
//
#define NUM_MOTORS  2

#if NUM_MOTORS > 0
#define MOTOR0_PORT     D       // Port d
#define MOTOR0_POS      6       // Pin for POS wire
#define MOTOR0_NEG      7       // Pin for NEG wire
//#define MOTOR0_NEG_LOGIC
#endif

#if NUM_MOTORS > 1
#define MOTOR1_PORT     D       // Port d
#define MOTOR1_POS      2       // Pin for POS wire
#define MOTOR1_NEG      3       // Pin for NEG wire
#define MOTOR1_NEG_LOGIC
#endif

#if NUM_MOTORS > 2
#define MOTOR2_PORT     D       // Port d
#define MOTOR2_POS      4       // Pin for POS wire
#define MOTOR2_NEG      5       // Pin for NEG wire
//#define MOTOR2_NEG_LOGIC
#endif

#if NUM_MOTORS > 3
#define MOTOR3_PORT     D       // Port d
#define MOTOR3_POS      0       // Pin for POS wire
#define MOTOR3_NEG      1       // Pin for NEG wire
//#define MOTOR3_NEG_LOGIC
#endif

#if NUM_MOTORS > 4
#error "Too many motors defined!"
#endif

//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MOTORx_STRING    is of the form "PortD[6,7]", for debug messages and such
//                 
#define MOTOR_STRING(_p_,_f_,_r_)   "PORT" _STR(_p_) "(" _STR(_f_) "," _STR(_r_) ")"

#define MOTOR0_STRING   MOTOR_STRING(MOTOR0_PORT,MOTOR0_POS,MOTOR0_NEG)
#define MOTOR1_STRING   MOTOR_STRING(MOTOR1_PORT,MOTOR1_POS,MOTOR1_NEG)
#define MOTOR2_STRING   MOTOR_STRING(MOTOR2_PORT,MOTOR2_POS,MOTOR2_NEG)
#define MOTOR3_STRING   MOTOR_STRING(MOTOR3_PORT,MOTOR3_POS,MOTOR3_NEG)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Data definitions and macros
//
// NOTE: The MotorPWM system requires that MOTOR_REV = MOTOR_FWD+1
//
typedef enum {
    MOTOR_FWD   = 201,
    MOTOR_REV,
    MOTOR_OFF
    } MOTOR_CTL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MotorInit - Initialize motor system
//
// Inputs:      None.
//
// Outputs:     None.
//
void MotorInit(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Motor - Set motor state
//
// Inputs:      ID of motor to control (integer index of motor)
//              Direction to set
//
// Outputs:     None.
//
void Motor(uint8_t ID,MOTOR_CTL Ctl);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GetMotor - Return motor state
//
// Inputs:      ID of motor to control (integer index of motor)
//
// Outputs:     Direction of motor
//
MOTOR_CTL GetMotor(uint8_t ID);

#endif  // MOTOR_H - entire file
