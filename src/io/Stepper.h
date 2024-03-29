////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      Stepper.h
//
//  SYNOPSIS
//
//      //////////////////////////////////////
//      //
//      // In Stepper.h
//      //
//      #define NUM_STEPPERS  3
//
//      #define MOTOR1_TYPE FULL2WIRE   // Type of motor port connection
//              :
//      (Continue for NUM_MOTORS)
//
//      //////////////////////////////////////
//      //
//      // In Main
//      //
//      StepperInit();
//          :
//
//      StepperMaxSpeed    (1,200.0);   // Max speed   , stepper 1
//      StepperAcceleration(1,100.0);   // Acceleration, stepper 1
//    
//      while(1) {
//
//        StepperStep(1,400);           // Move 400 steps FWD
//        while( !StepperDone(1) );     // Wait for complete
//
//        StepperStep(1,-400);          // Move 400 steps REV
//        while( !StepperDone(1) );     // Wait for complete
//        }
//
//  DESCRIPTION
//
//      Low-level stepper control
//
//  EXAMPLE
//
//      StepperInit();
//
//      sei();                          // Enable interrupts
//
//      StepperMaxSpeed    (1,200.0);   // Max speed   , stepper 1
//      StepperAcceleration(1,100.0);   // Acceleration, stepper 1
//    
//      while(1) {
//
//        Stepper(1, 400);              // Move 400 steps FWD
//        while( !StepperDone(1) );     // Wait to complete
//
//        Stepper(1,-400);              // move 400 steps REV
//        while( !StepperDone(1) );     // Wait to complete
//        }
//
//  NOTES:
//
//      Based on "Generate stepper-motor speed profiles in real time" by David Austin
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

#ifndef STEPPER_H
#define STEPPER_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Specify number of steppers to use.
//
// For each stepper, define the port it is connected to, the type, and bit shift
//
// SHIFT is the number of bits to shift the step command to align with the port control
//   pins. For example, FULL4WIRE requires 4 pins. If the motor is controlled by pins 
//   4..7, then SHIFT should be 4.
//
#define NUM_STEPPERS  2

#if NUM_STEPPERS > 0
#define STEPPER0_PORT     D             // Port d
#define STEPPER0_TYPE     FULL4WIRE     // Motor type and wiring
#define STEPPER0_SHIFT    4             // Pins 4, 5, 6, 7
#endif

#if NUM_STEPPERS > 1
#define STEPPER1_PORT     D             // Port d
#define STEPPER1_TYPE     FULL2WIRE     // Motor type and wiring
#define STEPPER1_SHIFT    2             // Pins 2, 3
#endif

#if NUM_STEPPERS > 2
#error "Too many motors defined!"
#endif
//
// End of user configurable options
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type of motor interface, per motor
//
typedef enum {
    FUNCTION  = 0,  // Functional interface, implementing your own driver functions
    DRIVER    = 1,  // Stepper Driver, 2 driver pins required
    FULL2WIRE = 2,  // 2 wire stepper, 2 motor pins required
    FULL3WIRE = 3,  // 3 wire stepper, such as HDD spindle, 3 motor pins required
    FULL4WIRE = 4,  // 4 wire full stepper, 4 motor pins required
    HALF3WIRE = 6,  // 3 wire half stepper, such as HDD spindle, 3 motor pins required
    HALF4WIRE = 8   // 4 wire half stepper, 4 motor pins required
    } InterfaceType;


/// /par Operation
/// This module operates by computing a step time in microseconds. The step
/// time is recomputed after each step and after speed and acceleration
/// parameters are changed by the caller. The time of each step is recorded in
/// microseconds. The run() function steps the motor once if a new step is due.
/// The run() function must be called frequently until the motor is in the
/// desired position, after which time run() will do nothing.
///
/// /par Positioning
/// Positions are specified by a signed long integer. At
/// construction time, the current position of the motor is consider to be 0. Positive
/// positions are clockwise from the initial position; negative positions are
/// anticlockwise. The current position can be altered for instance after
/// initialization positioning.
///

    /// Constructor. You can have multiple simultaneous steppers, all moving
    /// at different speeds and accelerations, provided you call their run()
    /// functions at frequent enough intervals. Current Position is set to 0, target
    /// position is set to 0. MaxSpeed and Acceleration default to 1.0.
    /// The motor pins will be initialised to OUTPUT mode during the
    /// constructor by a call to enableOutputs().
    /// /param[in] interface Number of pins to interface to. 1, 2, 4 or 8 are
    /// supported, but it is preferred to use the ref MotorInterfaceType symbolic names. 
    /// AccelStepper::DRIVER (1) means a stepper driver (with Step and Direction pins).
    /// If an enable line is also needed, call setEnablePin() after construction.
    /// You may also invert the pins using setPinsInverted().
    /// AccelStepper::FULL2WIRE (2) means a 2 wire stepper (2 pins required). 
    /// AccelStepper::FULL3WIRE (3) means a 3 wire stepper, such as HDD spindle (3 pins required). 
    /// AccelStepper::FULL4WIRE (4) means a 4 wire stepper (4 pins required). 
    /// AccelStepper::HALF3WIRE (6) means a 3 wire half stepper, such as HDD spindle (3 pins required)
    /// AccelStepper::HALF4WIRE (8) means a 4 wire half stepper (4 pins required)
    /// Defaults to AccelStepper::FULL4WIRE (4) pins.
    /// /param[in] pin1 Arduino digital pin number for motor pin 1. Defaults
    /// to pin 2. For a AccelStepper::DRIVER (interface==1), 
    /// this is the Step input to the driver. Low to high transition means to step)
    /// /param[in] pin2 Arduino digital pin number for motor pin 2. Defaults
    /// to pin 3. For a AccelStepper::DRIVER (interface==1), 
    /// this is the Direction input the driver. High means forward.
    /// /param[in] pin3 Arduino digital pin number for motor pin 3. Defaults
    /// to pin 4.
    /// /param[in] pin4 Arduino digital pin number for motor pin 4. Defaults
    /// to pin 5.
    /// /param[in] enable If this is true (the default), enableOutputs() will be called to enable
    /// the output pins at construction time.
    AccelStepper(uint8_t interface = AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);

    /// Alternate Constructor which will call your own functions for forward and backward steps. 
    /// You can have multiple simultaneous steppers, all moving
    /// at different speeds and accelerations, provided you call their run()
    /// functions at frequent enough intervals. Current Position is set to 0, target
    /// position is set to 0. MaxSpeed and Acceleration default to 1.0.
    /// Any motor initialization should happen before hand, no pins are used or initialized.
    /// /param[in] forward void-returning procedure that will make a forward step
    /// /param[in] backward void-returning procedure that will make a backward step
    AccelStepper(void (*forward)(), void (*backward)());
    
    /// Set the target position. The run() function will try to move the motor (at most one step per call)
    /// from the current position to the target position set by the most
    /// recent call to this function. Caution: moveTo() also recalculates the speed for the next step. 
    /// If you are trying to use constant speed movements, you should call setSpeed() after calling moveTo().
    /// /param[in] absolute The desired absolute position. Negative is
    /// anticlockwise from the 0 position.
    void    moveTo(long absolute); 

    /// Set the target position relative to the current position
    /// /param[in] relative The desired position relative to the current position. Negative is
    /// anticlockwise from the current position.
    void    move(long relative);

    /// Poll the motor and step it if a step is due, implementing
    /// accelerations and decelerations to acheive the target position. You must call this as
    /// frequently as possible, but at least once per minimum step time interval,
    /// preferably in your main loop. Note that each call to run() will make at most one step, and then only when a step is due,
    /// based on the current speed and the time since the last step.
    /// return true if the motor is still running to the target position.
    boolean run();

    /// Poll the motor and step it if a step is due, implementing a constant
    /// speed as set by the most recent call to setSpeed(). You must call this as
    /// frequently as possible, but at least once per step interval,
    /// return true if the motor was stepped.
    boolean runSpeed();

    /// Sets the maximum permitted speed. The run() function will accelerate
    /// up to the speed set by this function.
    /// Caution: the maximum speed achievable depends on your processor and clock speed.
    /// /param[in] speed The desired maximum speed in steps per second. Must
    /// be > 0. Caution: Speeds that exceed the maximum speed supported by the processor may
    /// Result in non-linear accelerations and decelerations.
    void    setMaxSpeed(float speed);

    /// Sets the acceleration/deceleration rate.
    /// /param[in] acceleration The desired acceleration in steps per second
    /// per second. Must be > 0.0. This is an expensive call since it requires a square 
    /// root to be calculated. Dont call more ofthen than needed
    void    setAcceleration(float acceleration);

    /// Sets the desired constant speed for use with runSpeed().
    /// /param[in] speed The desired constant speed in steps per
    /// second. Positive is clockwise. Speeds of more than 1000 steps per
    /// second are unreliable. Very slow speeds may be set (eg 0.00027777 for
    /// once per hour, approximately. Speed accuracy depends on the Arduino
    /// crystal. Jitter depends on how frequently you call the runSpeed() function.
    void    setSpeed(float speed);

    /// The most recently set speed
    /// return the most recent speed in steps per second
    float   speed();

    /// The distance from the current position to the target position.
    /// return the distance from the current position to the target position
    /// in steps. Positive is clockwise from the current position.
    long    distanceToGo();

    /// The most recently set target position.
    /// return the target position
    /// in steps. Positive is clockwise from the 0 position.
    long    targetPosition();

    /// The currently motor position.
    /// return the current motor position
    /// in steps. Positive is clockwise from the 0 position.
    long    currentPosition();  

    /// Resets the current position of the motor, so that wherever the motor
    /// happens to be right now is considered to be the new 0 position. Useful
    /// for setting a zero position on a stepper after an initial hardware
    /// positioning move.
    /// Has the side effect of setting the current motor speed to 0.
    /// /param[in] position The position in steps of wherever the motor
    /// happens to be right now.
    void    setCurrentPosition(long position);  
    
    /// Moves the motor at the currently selected constant speed (forward or reverse) 
    /// to the target position and blocks until it is at
    /// position. Dont use this in event loops, since it blocks.
    void    runToPosition();

    /// Runs at the currently selected speed until the target position is reached
    /// Does not implement accelerations.
    /// return true if it stepped
    boolean runSpeedToPosition();

    /// Moves the motor to the new target position and blocks until it is at
    /// position. Dont use this in event loops, since it blocks.
    /// /param[in] position The new target position.
    void    runToNewPosition(long position);

    /// Sets a new target position that causes the stepper
    /// to stop as quickly as possible, using the current speed and acceleration parameters.
    void stop();

    /// Disable motor pin outputs by setting them all LOW
    /// Depending on the design of your electronics this may turn off
    /// the power to the motor coils, saving power.
    /// This is useful to support Arduino low power modes: disable the outputs
    /// during sleep and then reenable with enableOutputs() before stepping
    /// again.
    virtual void    disableOutputs();

    /// Enable motor pin outputs by setting the motor pins to OUTPUT
    /// mode. Called automatically by the constructor.
    virtual void    enableOutputs();

    /// Sets the minimum pulse width allowed by the stepper driver. The minimum practical pulse width is 
    /// approximately 20 microseconds. Times less than 20 microseconds
    /// will usually result in 20 microseconds or so.
    /// /param[in] minWidth The minimum pulse width in microseconds. 
    void    setMinPulseWidth(unsigned int minWidth);

    /// Sets the enable pin number for stepper drivers.
    /// 0xFF indicates unused (default).
    /// Otherwise, if a pin is set, the pin will be turned on when 
    /// enableOutputs() is called and switched off when disableOutputs() 
    /// is called.
    /// /param[in] enablePin Arduino digital pin number for motor enable
    /// /sa setPinsInverted
    void    setEnablePin(uint8_t enablePin = 0xff);

    /// Sets the inversion for stepper driver pins
    /// /param[in] directionInvert True for inverted direction pin, false for non-inverted
    /// /param[in] stepInvert      True for inverted step pin, false for non-inverted
    /// /param[in] enableInvert    True for inverted enable pin, false (default) for non-inverted
    void    setPinsInverted(bool directionInvert = false, bool stepInvert = false, bool enableInvert = false);

    /// Sets the inversion for 2, 3 and 4 wire stepper pins
    /// /param[in] pin1Invert True for inverted pin1, false for non-inverted
    /// /param[in] pin2Invert True for inverted pin2, false for non-inverted
    /// /param[in] pin3Invert True for inverted pin3, false for non-inverted
    /// /param[in] pin4Invert True for inverted pin4, false for non-inverted
    /// /param[in] enableInvert    True for inverted enable pin, false (default) for non-inverted
    void    setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert);

protected:

    /// /brief Direction indicator
    /// Symbolic names for the direction the motor is turning
    typedef enum
    {
    DIRECTION_CCW = 0,  ///< Clockwise
        DIRECTION_CW  = 1   ///< Counter-Clockwise
    } Direction;

    /// Forces the library to compute a new instantaneous speed and set that as
    /// the current speed. It is called by
    /// the library:
    /// /li  after each step
    /// /li  after change to maxSpeed through setMaxSpeed()
    /// /li  after change to acceleration through setAcceleration()
    /// /li  after change to target position (relative or absolute) through
    /// move() or moveTo()
    void           computeNewSpeed();

    /// Low level function to set the motor output pins
    /// bit 0 of the mask corresponds to _pin[0]
    /// bit 1 of the mask corresponds to _pin[1]
    /// You can override this to impment, for example serial chip output insted of using the
    /// output pins directly
    virtual void   setOutputPins(uint8_t mask);

    /// Called to execute a step. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default calls step1(), step2(), step4() or step8() depending on the
    /// number of pins defined for the stepper.
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step(long step);

    /// Called to execute a step using stepper functions (pins = 0) Only called when a new step is
    /// required. Calls _forward() or _backward() to perform the step
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step0(long step);

    /// Called to execute a step on a stepper driver (ie where pins == 1). Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of Step pin1 to step, 
    /// and sets the output of _pin2 to the desired direction. The Step pin (_pin1) is pulsed for 1 microsecond
    /// which is the minimum STEP pulse width for the 3967 driver.
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step1(long step);

    /// Called to execute a step on a 2 pin motor. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of pin1 and pin2
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step2(long step);

    /// Called to execute a step on a 3 pin motor, such as HDD spindle. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of pin1, pin2,
    /// pin3
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step3(long step);

    /// Called to execute a step on a 4 pin motor. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of pin1, pin2,
    /// pin3, pin4.
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step4(long step);

    /// Called to execute a step on a 3 pin motor, such as HDD spindle. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of pin1, pin2,
    /// pin3
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step6(long step);

    /// Called to execute a step on a 4 pin half-steper motor. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of pin1, pin2,
    /// pin3, pin4.
    /// /param[in] step The current step phase number (0 to 7)
    virtual void   step8(long step);

private:
    /// Number of pins on the stepper motor. Permits 2 or 4. 2 pins is a
    /// bipolar, and 4 pins is a unipolar.
    uint8_t        _interface;          // 0, 1, 2, 4, 8, See MotorInterfaceType

    /// Arduino pin number assignments for the 2 or 4 pins required to interface to the
    /// stepper motor or driver
    uint8_t        _pin[4];

    /// Whether the _pins is inverted or not
    uint8_t        _pinInverted[4];

    /// The current absolution position in steps.
    long           _currentPos;    // Steps

    /// The target position in steps. The AccelStepper library will move the
    /// motor from the _currentPos to the _targetPos, taking into account the
    /// max speed, acceleration and deceleration
    long           _targetPos;     // Steps

    /// The current motos speed in steps per second
    /// Positive is clockwise
    float          _speed;         // Steps per second

    /// The maximum permitted speed in steps per second. Must be > 0.
    float          _maxSpeed;

    /// The acceleration to use to accelerate or decelerate the motor in steps
    /// per second per second. Must be > 0
    float          _acceleration;
    float          _sqrt_twoa; // Precomputed sqrt(2*_acceleration)

    /// The current interval between steps in microseconds.
    /// 0 means the motor is currently stopped with _speed == 0
    unsigned long  _stepInterval;

    /// The last step time in microseconds
    unsigned long  _lastStepTime;

    /// The minimum allowed pulse width in microseconds
    unsigned int   _minPulseWidth;

    /// Is the direction pin inverted?
    ///bool           _dirInverted; /// Moved to _pinInverted[1]

    /// Is the step pin inverted?
    ///bool           _stepInverted; /// Moved to _pinInverted[0]

    /// Is the enable pin inverted?
    bool           _enableInverted;

    /// Enable pin for stepper driver, or 0xFF if unused.
    uint8_t        _enablePin;

    /// The pointer to a forward-step procedure
    void (*_forward)();

    /// The pointer to a backward-step procedure
    void (*_backward)();

    /// The step counter for speed calculations
    long _n;

    /// Initial step size in microseconds
    float _c0;

    /// Last step size in microseconds
    float _cn;

    /// Min step size in microseconds based on maxSpeed
    float _cmin; // at max speed

    /// Current direction motor is spinning in
    boolean _direction; // 1 == CW

};

/// @example Random.pde
/// Make a single stepper perform random changes in speed, position and acceleration

/// @example Overshoot.pde
///  Check overshoot handling
/// which sets a new target position and then waits until the stepper has 
/// achieved it. This is used for testing the handling of overshoots

/// @example MultiStepper.pde
/// Shows how to multiple simultaneous steppers
/// Runs one stepper forwards and backwards, accelerating and decelerating
/// at the limits. Runs other steppers at the same time

/// @example ConstantSpeed.pde
/// Shows how to run AccelStepper in the simplest,
/// fixed speed mode with no accelerations

/// @example Blocking.pde 
/// Shows how to use the blocking call runToNewPosition
/// Which sets a new target position and then waits until the stepper has 
/// achieved it.

/// @example AFMotor_MultiStepper.pde
/// Control both Stepper motors at the same time with different speeds
/// and accelerations. 

/// @example AFMotor_ConstantSpeed.pde
/// Shows how to run AccelStepper in the simplest,
/// fixed speed mode with no accelerations

/// @example ProportionalControl.pde
/// Make a single stepper follow the analog value read from a pot or whatever
/// The stepper will move at a constant speed to each newly set posiiton, 
/// depending on the value of the pot.

/// @example Bounce.pde
/// Make a single stepper bounce from one limit to another, observing
/// accelrations at each end of travel

/// @example Quickstop.pde
/// Check stop handling.
/// Calls stop() while the stepper is travelling at full speed, causing
/// the stepper to stop as quickly as possible, within the constraints of the
/// current acceleration.

/// @example MotorShield.pde
/// Shows how to use AccelStepper to control a 3-phase motor, such as a HDD spindle motor
/// using the Adafruit Motor Shield http://www.ladyada.net/make/mshield/index.html.

/// @example DualMotorShield.pde
/// Shows how to use AccelStepper to control 2 x 2 phase steppers using the 
/// Itead Studio Arduino Dual Stepper Motor Driver Shield
/// model IM120417015

#endif // STEPPER_H, whole file
