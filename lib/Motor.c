//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      Motor.c
//
//  DESCRIPTION
//
//      Simple AVR program to interface H-bridge DC motor controllers.
//
//      See Motor.h for a more complete description
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

#include <PortMacros.h>

#include "Motor.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup some helpful macros
//
#define _OFF_MASK(_id_)                                                             \
    (_PIN_MASK(MOTOR##_id_##_POS) | _PIN_MASK(MOTOR##_id_##_NEG))                   \

#define _FWD_MASK(_id_)                                                             \
    (_PIN_MASK(MOTOR##_id_##_POS))                                                  \

#define _REV_MASK(_id_)                                                             \
    (_PIN_MASK(MOTOR##_id_##_NEG))                                                  \

#ifndef MOTOR0_NEG_LOGIC
#   define _MOTOR0_OFF                                                              \
    _CLR_MASK(_PORT(MOTOR0_PORT),_OFF_MASK(0));                                     \

#   define _MOTOR0_FWD                                                              \
    _SET_MASK(_PORT(MOTOR0_PORT),_FWD_MASK(0));                                     \

#   define _MOTOR0_REV                                                              \
    _SET_MASK(_PORT(MOTOR0_PORT),_REV_MASK(0));                                     \

#   define MOTOR0_GET   (_GET_MASK(_PORT(MOTOR0_PORT),_OFF_MASK(0)) ^ _OFF_MASK(0))
#else
#   define _MOTOR0_OFF                                                              \
    _SET_MASK(_PORT(MOTOR0_PORT),_OFF_MASK(0));                                     \

#   define _MOTOR0_FWD                                                              \
    _CLR_MASK(_PORT(MOTOR0_PORT),_FWD_MASK(0));                                     \

#   define _MOTOR0_REV                                                              \
    _CLR_MASK(_PORT(MOTOR0_PORT),_REV_MASK(0));                                     \

#   define MOTOR0_GET   _GET_MASK(_PORT(MOTOR0_PORT),_OFF_MASK(0))
#endif


#ifndef MOTOR1_NEG_LOGIC
#   define _MOTOR1_OFF                                                              \
    _CLR_MASK(_PORT(MOTOR1_PORT),_OFF_MASK(1));                                     \

#   define _MOTOR1_FWD                                                              \
    _SET_MASK(_PORT(MOTOR1_PORT),_FWD_MASK(1));                                     \

#   define _MOTOR1_REV                                                              \
    _SET_MASK(_PORT(MOTOR1_PORT),_REV_MASK(1));                                     \

#   define MOTOR1_GET   (_GET_MASK(_PORT(MOTOR1_PORT),_OFF_MASK(1)) ^ _OFF_MASK(1))
#else
#   define _MOTOR1_OFF                                                              \
    _SET_MASK(_PORT(MOTOR1_PORT),_OFF_MASK(1));                                     \

#   define _MOTOR1_FWD                                                              \
    _CLR_MASK(_PORT(MOTOR1_PORT),_FWD_MASK(1));                                     \

#   define _MOTOR1_REV                                                              \
    _CLR_MASK(_PORT(MOTOR1_PORT),_REV_MASK(1));                                     \

#   define MOTOR1_GET   _GET_MASK(_PORT(MOTOR1_PORT),_OFF_MASK(1))
#endif


#ifndef MOTOR2_NEG_LOGIC
#   define _MOTOR2_OFF                                                              \
    _CLR_MASK(_PORT(MOTOR2_PORT),_OFF_MASK(2));                                     \

#   define _MOTOR2_FWD                                                              \
    _SET_MASK(_PORT(MOTOR2_PORT),_FWD_MASK(2));                                     \

#   define _MOTOR2_REV                                                              \
    _SET_MASK(_PORT(MOTOR2_PORT),_REV_MASK(2));                                     \

#   define MOTOR2_GET   (_GET_MASK(_PORT(MOTOR2_PORT),_OFF_MASK(2)) ^ _OFF_MASK(2))
#else
#   define _MOTOR2_OFF                                                              \
    _SET_MASK(_PORT(MOTOR2_PORT),_OFF_MASK(2));                                     \

#   define _MOTOR2_FWD                                                              \
    _CLR_MASK(_PORT(MOTOR2_PORT),_FWD_MASK(2));                                     \

#   define _MOTOR2_REV                                                              \
    _CLR_MASK(_PORT(MOTOR2_PORT),_REV_MASK(2));                                     \

#   define MOTOR2_GET   _GET_MASK(_PORT(MOTOR2_PORT),_OFF_MASK(2))
#endif


#ifndef MOTOR3_NEG_LOGIC
#   define _MOTOR3_OFF                                                              \
    _CLR_MASK(_PORT(MOTOR3_PORT),_OFF_MASK(3));                                     \

#   define _MOTOR3_FWD                                                              \
    _SET_MASK(_PORT(MOTOR3_PORT),_FWD_MASK(3));                                     \

#   define _MOTOR3_REV                                                              \
    _SET_MASK(_PORT(MOTOR3_PORT),_REV_MASK(3));                                     \

#   define MOTOR3_GET   (_GET_MASK(_PORT(MOTOR3_PORT),_OFF_MASK(3)) ^ _OFF_MASK(3))
#else
#   define _MOTOR3_OFF                                                              \
    _SET_MASK(_PORT(MOTOR3_PORT),_OFF_MASK(3));                                     \

#   define _MOTOR3_FWD                                                              \
    _CLR_MASK(_PORT(MOTOR3_PORT),_FWD_MASK(3));                                     \

#   define _MOTOR3_REV                                                              \
    _CLR_MASK(_PORT(MOTOR3_PORT),_REV_MASK(3));                                     \

#   define MOTOR3_GET   _GET_MASK(_PORT(MOTOR3_PORT),_OFF_MASK(3))
#endif

#define _MOTOR_INIT(_id_)                                                           \
    _MOTOR##_id_##_OFF                                                              \
    _SET_MASK(_DDR(MOTOR##_id_##_PORT),_OFF_MASK(_id_));                            \

#define _MOTOR_SET(_id_)                                                            \
    case _id_:                                                                      \
        _MOTOR##_id_##_OFF;                                                         \
        switch (Ctl) {                                                              \
                                                                                    \
            case MOTOR_FWD:                                                         \
                _MOTOR##_id_##_FWD;                                                 \
                break;                                                              \
                                                                                    \
            case MOTOR_REV:                                                         \
                _MOTOR##_id_##_REV;                                                 \
                break;                                                              \
                                                                                    \
            case MOTOR_OFF:                                                         \
                break;                                                              \
            }                                                                       \
    break;                                                                          \

#define _MOTOR_GET(_id_)                                                             \
    case _id_:                                                                      \
        MotorBits = MOTOR##_id_##_GET;                                              \
        if( MotorBits == _FWD_MASK(_id_) ) return(MOTOR_FWD);                       \
        if( MotorBits == _REV_MASK(_id_) ) return(MOTOR_REV);                       \
        return MOTOR_OFF;                                                           \
        break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// MotorInit - Initialize motor system
//
// Inputs:      None.
//
// Outputs:     None.
//
void MotorInit(void) {
 
#if NUM_MOTORS > 0
    _MOTOR_INIT(0);
#   endif

#if NUM_MOTORS > 1
    _MOTOR_INIT(1);
#   endif

#if NUM_MOTORS > 2
    _MOTOR_INIT(2);
#   endif

#if NUM_MOTORS > 3
    _MOTOR_INIT(3);
#   endif

#if NUM_MOTORS > 4
#   error "Too many motors defined!"
#   endif
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Motor - Set motor state
//
// Inputs:      ID of motor to control (integer index of motor)
//              Direction to set
//
// Outputs:     None.
//
void Motor(uint8_t ID,MOTOR_CTL Ctl) {

    switch(ID) {
#if NUM_MOTORS > 0
        _MOTOR_SET(0);
#       endif

#if NUM_MOTORS > 1
        _MOTOR_SET(1);
#       endif

#if NUM_MOTORS > 2
        _MOTOR_SET(2);
#       endif

#if NUM_MOTORS > 3
        _MOTOR_SET(3);
#       endif

#if NUM_MOTORS > 4
#   error "Too many motors defined!"
#   endif
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// GetMotor - Return motor state
//
// Inputs:      ID of motor to control (integer index of motor)
//
// Outputs:     Direction of motor
//
MOTOR_CTL GetMotor(uint8_t ID) {
    uint8_t MotorBits;

    switch(ID) {
#if NUM_MOTORS > 0
        _MOTOR_GET(0);
#       endif

#if NUM_MOTORS > 1
        _MOTOR_GET(1);
#       endif

#if NUM_MOTORS > 2
        _MOTOR_GET(2);
#       endif

#if NUM_MOTORS > 3
        _MOTOR_GET(3);
#       endif

#if NUM_MOTORS > 4
#   error "Too many motors defined!"
#   endif
        }

    return MOTOR_OFF;
    }
