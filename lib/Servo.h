//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      Servo.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Servo.h
//      //
//      #define NUM_Servos  3
//
//      #define Servo1_PORT d           // Port connected to Servo1
//      #define Servo1_BIT  0           // Bit  connected to Servo1 control
//              :
//      (Continue for NUM_Servos)
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//      Servo(1, 30);                   // Engage Servo1 to left   position
//      Servo(1, 80);                   // Engage Servo1 in middle position
//      Servo(1,130);                   // Engage Servo1 in right  position
//
//  DESCRIPTION
//
//      Servo positioning commands
//
//  EXAMPLE
//
//      #define OFFSET          30
//      #define SPAN            110
//      #define MIN_POS         OFFSET      // Minimum position
//      #define MAX_POS         SPAN+OFFSET  // Maximum position
//      #define MID_POS         (SPAN/2)+OFFSET
//
//      Servo(0,0);                             // Start in right position
//
//      while(1) {
//          int i;
//
//          for( i=MIN_POS; i<=MID_POS; i++ ) { // Move slowly to the middle
//              Servo(0,i);
//              _delay_ms(MOVE_DELAY_MS);
//              }
//  
//          _delay_ms(PAUSE_MS);                // Wait a bit
//  
//          for( i=MID_POS; i<=MAX_POS; i++ ) { // Move slowly to left position
//              Servo(0,i);
//              _delay_ms(MOVE_DELAY_MS);
//              }
//  
//          _delay_ms(PAUSE_MS);                // Wait a bit
//  
//          for( i=MAX_POS; i>=MID_POS; i-- ) { // Move slowly to neutral position
//              Servo(0,i);
//              _delay_ms(MOVE_DELAY_MS);
//              }
//  
//          _delay_ms(PAUSE_MS);                // Wait a bit
//  
//          for( i=MID_POS; i>=MIN_POS; i-- ) { // Move slowly to right position
//              Servo(0,i);
//              _delay_ms(MOVE_DELAY_MS);
//              }
//  
//          _delay_ms(PAUSE_MS);                // Wait a bit
//  
//          PrintCRLF();
//          }
//
//  NOTES
//
//      The OFFSET and SPAN are left to the caller to determine, since they are
//        different for each type of servo.
//
//      The assignment of LEFT and RIGHT in the example above is arbitrary.
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

#ifndef Servo_H
#define Servo_H

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Specify number of Servos to use.
//
// For each Servo, define the port and pin it is connected to.
//
#define NUM_SERVOS      2

#if NUM_SERVOS > 0
#define Servo0_PORT     D       // Port d
#define Servo0_BIT      7       // Control pin
//#define Servo0_NEG_LOGIC
#endif

#if NUM_SERVOS > 1
#define Servo1_PORT     D       // Port d
#define Servo1_BIT      6       // Control pin
#endif

#if NUM_SERVOS > 2
#define Servo2_PORT     D       // Port d
#define Servo2_BIT      5       // Control pin
//#define Servo2_NEG_LOGIC
#endif

#if NUM_SERVOS > 3
#define Servo3_PORT     D       // Port d
#define Servo3_BIT      4       // Control pin
//#define Servo3_NEG_LOGIC
#endif

#if NUM_SERVOS > 4
#error "Too many Servos defined!"
#endif

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Specify a Timer to use for servos
//
// See Timer.h for explanation and list of examples.
//
#define SERVO_TIMER     2                         // use TIMER2
#define SERVO_CCRA      (1 << WGM21)
#define SERVO_CCRB      (1 <<  CS22) | (1 <<  CS20)
#define SERVO_COUNTA    254

//
// The OFLO_MODULUS is the number of times the timer must overflow
//   between sending pulses; ie we send out a pulse every OFLO_MODULUS
//   overflows of the timer.
//
// This way we can send out short (1-2mS) pulses at longer (20 mS) intervals,
//   as required for proper servo control.
//
#define OFLO_MODULUS    10

//
// MIN_PULSE is the number of ticks for a minimum pulse width (1/2 mS)
//
#define MIN_PULSE       50

//
// End of user configurable options
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Servox_STRING    is of the form "PortD[6]", for debug messages and such
//                 
#define Servo_STRING(_p_,_b_)   "PORT" _STR(_p_) "(" _STR(_b_) ")"

#define Servo0_STRING   Servo_STRING(Servo0_PORT,Servo0_BIT)
#define Servo1_STRING   Servo_STRING(Servo1_PORT,Servo1_BIT)
#define Servo2_STRING   Servo_STRING(Servo2_PORT,Servo2_BIT)
#define Servo3_STRING   Servo_STRING(Servo3_PORT,Servo3_BIT)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// As a special case, define SERVO_OFF to mean "unpowered"
//
#define SERVO_OFF   255

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// ServoInit - Initialize Servo system
//
// Inputs:      None.
//
// Outputs:     None.
//
void ServoInit(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Servo - Set Servo state
//
// Inputs:      ID of Servo to control (integer index of Servo)
//              Position to set (0 -> 100), or SERVO_OFF
//
// Outputs:     None.
//
void Servo(uint8_t ID,uint8_t POS);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetServo - Return Servo state
//
// Inputs:      ID of Servo to control (integer index of Servo)
//
// Outputs:     Position (0 -> 100
//
uint8_t GetServo(uint8_t ID);

#endif  // Servo_H - entire file
