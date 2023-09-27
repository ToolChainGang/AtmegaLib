////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved
//
//  FILE
//      Stepper.c
//
//  DESCRIPTION
//
//      Simple AVR program to interface stepper motors
//
//      See Stepper.h for a more complete description
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

#include "Stepper.h"

typedef enum {
    CCW = 0,    // Clockwise
    CW  = 1     // Counter-Clockwise
    } Direction;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct {
    //
    // Motor type and connection. Permits 2 or 4. 2 pins is bipolar, 4 is unipolar.
    //
    Interface   Type;
    long        CurrentPos;         // Current position in steps.
    long        TargetPos;          // Target position in steps.
    float       Speed;              // Steps per second
    float       MaxSpeed;           // Steps per second
    float       Acceleration;       // Steps per second per second
    float       SqrtTwoA;           // Precomputed sqrt(2*_acceleration)


    } Steppers[NUM_STEPPERS] NOINIT;



    /// Arduino pin number assignments for the 2 or 4 pins required to interface to the
    /// stepper motor or driver
    uint8_t        _pin[4];

    /// Whether the _pins is inverted or not
    uint8_t        _pinInverted[4];

    /// The current interval between steps in microseconds.
    /// 0 means the motor is currently stopped with _speed == 0
    unsigned long  _stepInterval;

    /// The last step time in microseconds
    unsigned long  _lastStepTime;

    /// The minimum allowed pulse width in microseconds
    unsigned int   _minPulseWidth;

    /// Is the enable pin inverted?
    bool           _enableInverted;

    /// Enable pin for stepper driver, or 0xFF if unused.
    uint8_t        _enablePin;

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













    /// Constructor. You can have multiple simultaneous steppers, all moving
    /// at different speeds and accelerations, provided you call their run()
    /// functions at frequent enough intervals. Current Position is set to 0, target
    /// position is set to 0. MaxSpeed and Acceleration default to 1.0.
    /// The motor pins will be initialised to OUTPUT mode during the
    /// constructor by a call to enableOutputs().
    /// /param[in] interface Number of pins to interface to. 1, 2, 4 or 8 are
    /// supported, but it is preferred to use the ef MotorInterfaceType symbolic names. 
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
    /// eturn true if the motor is still running to the target position.
    boolean run();

    /// Poll the motor and step it if a step is due, implementing a constant
    /// speed as set by the most recent call to setSpeed(). You must call this as
    /// frequently as possible, but at least once per step interval,
    /// eturn true if the motor was stepped.
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
    /// eturn the most recent speed in steps per second
    float   speed();

    /// The distance from the current position to the target position.
    /// eturn the distance from the current position to the target position
    /// in steps. Positive is clockwise from the current position.
    long    distanceToGo();

    /// The most recently set target position.
    /// eturn the target position
    /// in steps. Positive is clockwise from the 0 position.
    long    targetPosition();

    /// The currently motor position.
    /// eturn the current motor position
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
    /// eturn true if it stepped
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



void AccelStepper::moveTo(long absolute)
{
    if (_targetPos != absolute)
    {
	_targetPos = absolute;
	computeNewSpeed();
	// compute new n?
    }
}

void AccelStepper::move(long relative)
{
    moveTo(_currentPos + relative);
}

// Implements steps according to the current step interval
// You must call this at least once per step
// returns true if a step occurred
boolean AccelStepper::runSpeed()
{
    // Dont do anything unless we actually have a step interval
    if (!_stepInterval)
	return false;

    unsigned long time = micros();
    unsigned long nextStepTime = _lastStepTime + _stepInterval;
    // Gymnastics to detect wrapping of either the nextStepTime and/or the current time
//    if (   ((nextStepTime >= _lastStepTime) && ((time >= nextStepTime) || (time < _lastStepTime)))
//	|| ((nextStepTime < _lastStepTime) && ((time >= nextStepTime) && (time < _lastStepTime))))

    // If the current time is equal or larger to the next step time, a step should occur.
    if ((long)(time - nextStepTime) >= 0)
    {
	if (_direction == DIRECTION_CW)
	{
	    // Clockwise
	    _currentPos += 1;
	}
	else
	{
	    // Anticlockwise  
	    _currentPos -= 1;
	}
	step(_currentPos);

	_lastStepTime = time;
	return true;
    }
    else
    {
	return false;
    }
}

long AccelStepper::distanceToGo()
{
    return _targetPos - _currentPos;
}

long AccelStepper::targetPosition()
{
    return _targetPos;
}

long AccelStepper::currentPosition()
{
    return _currentPos;
}

// Useful during initialisations or after initial positioning
// Sets speed to 0
void AccelStepper::setCurrentPosition(long position)
{
    _targetPos = _currentPos = position;
    _n = 0;
    _stepInterval = 0;
}

void AccelStepper::computeNewSpeed()
{
    long distanceTo = distanceToGo(); // +ve is clockwise from curent location

    long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration)); // Equation 16

    if (distanceTo == 0 && stepsToStop <= 1)
    {
	// We are at the target and its time to stop
	_stepInterval = 0;
	_speed = 0.0;
	_n = 0;
	return;
    }

    if (distanceTo > 0)
    {
	// We are anticlockwise from the target
	// Need to go clockwise from here, maybe decelerate now
	if (_n > 0)
	{
	    // Currently accelerating, need to decel now? Or maybe going the wrong way?
	    if ((stepsToStop >= distanceTo) || _direction == DIRECTION_CCW)
		_n = -stepsToStop; // Start deceleration
	}
	else if (_n < 0)
	{
	    // Currently decelerating, need to accel again?
	    if ((stepsToStop < distanceTo) && _direction == DIRECTION_CW)
		_n = -_n; // Start accceleration
	}
    }
    else if (distanceTo < 0)
    {
	// We are clockwise from the target
	// Need to go anticlockwise from here, maybe decelerate
	if (_n > 0)
	{
	    // Currently accelerating, need to decel now? Or maybe going the wrong way?
	    if ((stepsToStop >= -distanceTo) || _direction == DIRECTION_CW)
		_n = -stepsToStop; // Start deceleration
	}
	else if (_n < 0)
	{
	    // Currently decelerating, need to accel again?
	    if ((stepsToStop < -distanceTo) && _direction == DIRECTION_CCW)
		_n = -_n; // Start accceleration
	}
    }

    // Need to accelerate or decelerate
    if (_n == 0)
    {
	// First step from stopped
	_cn = _c0;
	_direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    else
    {
	// Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
	_cn = _cn - ((2.0 * _cn) / ((4.0 * _n) + 1)); // Equation 13
	_cn = max(_cn, _cmin); 
    }
    _n++;
    _stepInterval = _cn;
    _speed = 1000000.0 / _cn;
    if (_direction == DIRECTION_CCW)
	_speed = -_speed;

#if 0
    Serial.println(_speed);
    Serial.println(_acceleration);
    Serial.println(_cn);
    Serial.println(_c0);
    Serial.println(_n);
    Serial.println(_stepInterval);
    Serial.println(distanceTo);
    Serial.println(stepsToStop);
    Serial.println("-----");
#endif
}

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if the motor is still running to the target position.
boolean AccelStepper::run()
{
    if (runSpeed())
	computeNewSpeed();
    return _speed != 0.0 || distanceToGo() != 0;
}

AccelStepper::AccelStepper(uint8_t interface, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, bool enable)
{
    _interface = interface;
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _maxSpeed = 1.0;
    _acceleration = 0.0;
    _sqrt_twoa = 1.0;
    _stepInterval = 0;
    _minPulseWidth = 1;
    _enablePin = 0xff;
    _lastStepTime = 0;
    _pin[0] = pin1;
    _pin[1] = pin2;
    _pin[2] = pin3;
    _pin[3] = pin4;

    // NEW
    _n = 0;
    _c0 = 0.0;
    _cn = 0.0;
    _cmin = 1.0;
    _direction = DIRECTION_CCW;

    int i;
    for (i = 0; i < 4; i++)
	_pinInverted[i] = 0;
    if (enable)
	enableOutputs();
    // Some reasonable default
    setAcceleration(1);
}

AccelStepper::AccelStepper(void (*forward)(), void (*backward)())
{
    _interface = 0;
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _maxSpeed = 1.0;
    _acceleration = 0.0;
    _sqrt_twoa = 1.0;
    _stepInterval = 0;
    _minPulseWidth = 1;
    _enablePin = 0xff;
    _lastStepTime = 0;
    _pin[0] = 0;
    _pin[1] = 0;
    _pin[2] = 0;
    _pin[3] = 0;
    _forward = forward;
    _backward = backward;

    // NEW
    _n = 0;
    _c0 = 0.0;
    _cn = 0.0;
    _cmin = 1.0;
    _direction = DIRECTION_CCW;

    int i;
    for (i = 0; i < 4; i++)
	_pinInverted[i] = 0;
    // Some reasonable default
    setAcceleration(1);
}

void AccelStepper::setMaxSpeed(float speed)
{
    if (_maxSpeed != speed)
    {
	_maxSpeed = speed;
	_cmin = 1000000.0 / speed;
	// Recompute _n from current speed and adjust speed if accelerating or cruising
	if (_n > 0)
	{
	    _n = (long)((_speed * _speed) / (2.0 * _acceleration)); // Equation 16
	    computeNewSpeed();
	}
    }
}

void AccelStepper::setAcceleration(float acceleration)
{
    if (acceleration == 0.0)
	return;
    if (_acceleration != acceleration)
    {
	// Recompute _n per Equation 17
	_n = _n * (_acceleration / acceleration);
	// New c0 per Equation 7, with correction per Equation 15
	_c0 = 0.676 * sqrt(2.0 / acceleration) * 1000000.0; // Equation 15
	_acceleration = acceleration;
	computeNewSpeed();
    }
}

void AccelStepper::setSpeed(float speed)
{
    if (speed == _speed)
        return;
    speed = constrain(speed, -_maxSpeed, _maxSpeed);
    if (speed == 0.0)
	_stepInterval = 0;
    else
    {
	_stepInterval = fabs(1000000.0 / speed);
	_direction = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    _speed = speed;
}

float AccelStepper::speed()
{
    return _speed;
}

// Subclasses can override
void AccelStepper::step(long step)
{
    switch (_interface)
    {
        case FUNCTION:
            step0(step);
            break;

	case DRIVER:
	    step1(step);
	    break;
    
	case FULL2WIRE:
	    step2(step);
	    break;
    
	case FULL3WIRE:
	    step3(step);
	    break;  

	case FULL4WIRE:
	    step4(step);
	    break;  

	case HALF3WIRE:
	    step6(step);
	    break;  
		
	case HALF4WIRE:
	    step8(step);
	    break;  
    }
}

// You might want to override this to implement eg serial output
// bit 0 of the mask corresponds to _pin[0]
// bit 1 of the mask corresponds to _pin[1]
// ....
void AccelStepper::setOutputPins(uint8_t mask)
{
    uint8_t numpins = 2;
    if (_interface == FULL4WIRE || _interface == HALF4WIRE)
	numpins = 4;
    else if (_interface == FULL3WIRE || _interface == HALF3WIRE)
	numpins = 3;
    uint8_t i;
    for (i = 0; i < numpins; i++)
	digitalWrite(_pin[i], (mask & (1 << i)) ? (HIGH ^ _pinInverted[i]) : (LOW ^ _pinInverted[i]));
}

// 0 pin step function (ie for functional usage)
void AccelStepper::step0(long step)
{
  if (_speed > 0)
    _forward();
  else
    _backward();
}

// 1 pin step function (ie for stepper drivers)
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step1(long step)
{
    // _pin[0] is step, _pin[1] is direction
    setOutputPins(_direction ? 0b10 : 0b00); // Set direction first else get rogue pulses
    setOutputPins(_direction ? 0b11 : 0b01); // step HIGH
    // Caution 200ns setup time 
    // Delay the minimum allowed pulse width
    delayMicroseconds(_minPulseWidth);
    setOutputPins(_direction ? 0b10 : 0b00); // step LOW

}


// 2 pin step function
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step2(long step)
{
    switch (step & 0x3)
    {
	case 0: /* 01 */
	    setOutputPins(0b10);
	    break;

	case 1: /* 11 */
	    setOutputPins(0b11);
	    break;

	case 2: /* 10 */
	    setOutputPins(0b01);
	    break;

	case 3: /* 00 */
	    setOutputPins(0b00);
	    break;
    }
}
// 3 pin step function
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step3(long step)
{
    switch (step % 3)
    {
	case 0:    // 100
	    setOutputPins(0b100);
	    break;

	case 1:    // 001
	    setOutputPins(0b001);
	    break;

	case 2:    //010
	    setOutputPins(0b010);
	    break;
	    
    }
}

// 4 pin step function for half stepper
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step4(long step)
{
    switch (step & 0x3)
    {
	case 0:    // 1010
	    setOutputPins(0b0101);
	    break;

	case 1:    // 0110
	    setOutputPins(0b0110);
	    break;

	case 2:    //0101
	    setOutputPins(0b1010);
	    break;

	case 3:    //1001
	    setOutputPins(0b1001);
	    break;
    }
}

// 3 pin half step function
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step6(long step)
{
    switch (step % 6)
    {
	case 0:    // 100
	    setOutputPins(0b100);
            break;
	    
        case 1:    // 101
	    setOutputPins(0b101);
            break;
	    
	case 2:    // 001
	    setOutputPins(0b001);
            break;
	    
        case 3:    // 011
	    setOutputPins(0b011);
            break;
	    
	case 4:    // 010
	    setOutputPins(0b010);
            break;
	    
	case 5:    // 011
	    setOutputPins(0b110);
            break;
	    
    }
}

// 4 pin half step function
// This is passed the current step number (0 to 7)
// Subclasses can override
void AccelStepper::step8(long step)
{
    switch (step & 0x7)
    {
	case 0:    // 1000
	    setOutputPins(0b0001);
            break;
	    
        case 1:    // 1010
	    setOutputPins(0b0101);
            break;
	    
	case 2:    // 0010
	    setOutputPins(0b0100);
            break;
	    
        case 3:    // 0110
	    setOutputPins(0b0110);
            break;
	    
	case 4:    // 0100
	    setOutputPins(0b0010);
            break;
	    
        case 5:    //0101
	    setOutputPins(0b1010);
            break;
	    
	case 6:    // 0001
	    setOutputPins(0b1000);
            break;
	    
        case 7:    //1001
	    setOutputPins(0b1001);
            break;
    }
}
    
// Prevents power consumption on the outputs
void    AccelStepper::disableOutputs()
{   
    if (! _interface) return;

    setOutputPins(0); // Handles inversion automatically
    if (_enablePin != 0xff)
        digitalWrite(_enablePin, LOW ^ _enableInverted);
}

void    AccelStepper::enableOutputs()
{
    if (! _interface) 
	return;

    pinMode(_pin[0], OUTPUT);
    pinMode(_pin[1], OUTPUT);
    if (_interface == FULL4WIRE || _interface == HALF4WIRE)
    {
        pinMode(_pin[2], OUTPUT);
        pinMode(_pin[3], OUTPUT);
    }
    else if (_interface == FULL3WIRE || _interface == HALF3WIRE)
    {
        pinMode(_pin[2], OUTPUT);
    }

    if (_enablePin != 0xff)
    {
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, HIGH ^ _enableInverted);
    }
}

void AccelStepper::setMinPulseWidth(unsigned int minWidth)
{
    _minPulseWidth = minWidth;
}

void AccelStepper::setEnablePin(uint8_t enablePin)
{
    _enablePin = enablePin;

    // This happens after construction, so init pin now.
    if (_enablePin != 0xff)
    {
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, HIGH ^ _enableInverted);
    }
}

void AccelStepper::setPinsInverted(bool directionInvert, bool stepInvert, bool enableInvert)
{
    _pinInverted[0] = stepInvert;
    _pinInverted[1] = directionInvert;
    _enableInverted = enableInvert;
}

void AccelStepper::setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert)
{    
    _pinInverted[0] = pin1Invert;
    _pinInverted[1] = pin2Invert;
    _pinInverted[2] = pin3Invert;
    _pinInverted[3] = pin4Invert;
    _enableInverted = enableInvert;
}

// Blocks until the target position is reached and stopped
void AccelStepper::runToPosition()
{
    while (run())
	;
}

boolean AccelStepper::runSpeedToPosition()
{
    if (_targetPos == _currentPos)
	return false;
    if (_targetPos >_currentPos)
	_direction = DIRECTION_CW;
    else
	_direction = DIRECTION_CCW;
    return runSpeed();
}

// Blocks until the new target position is reached
void AccelStepper::runToNewPosition(long position)
{
    moveTo(position);
    runToPosition();
}

void AccelStepper::stop()
{
    if (_speed != 0.0)
    {    
	long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration)) + 1; // Equation 16 (+integer rounding)
	if (_speed > 0)
	    move(stepsToStop);
	else
	    move(-stepsToStop);
    }
}
