# ArduinoLib

A bog-standard set of Arduino hardware interface libraries...

With test programs!

You can test hardware sections in isolation by running test programs found in this project.

The interfaces can be port-configured to conform to your specific hardware. For example,
the stepper interface can be configured to use any three GPIO pins (step, dir, enb).

Many test programs are supplied that accept serial commands to exercise the hardware. For example,
the stepper motor test program "SteperPulse.c" accepts the following commands:

````bash
+      # Go faster (by 10%)
-      # Go slower (by 10%)

D      # Disable motor drive
E      # Enable  motor drive

F      # Set FWD direction
R      # Set REV direction

H      # Set step output HIGH
L      # Set step output LOW

P      # Send 1 step pulse
C      # Send continuous pulses

<ESC>  # Stop continuous output
?      # Show help panel
````

Other test programs are automatic, allowing the engineer to debug the hardware while the
device is put through its paces. For example, the program "StepperTest.c" loops
continuously:

````bash
    while(1) {
        Left  2 revolutions
        Wait  2 secs
        Right 2 revolutions
        Wait  2 secs
        }
````

## List of interfaces

````bash
## Standard Arduino/Atmega I/O

AtoD            # Interrupt       AtoD interface
AtoDInline      # Inline/blocking AtoD interface
AUART           # Alt UART, for devices that have one
BadInt          # Bad interrupt
Comparator      # Comparator
Counter         # Counter/timer as counter
EEPROM          # Read/Write to EEPROM
I2C             # I2C interface
PortMacros      # Macros for portable port and pin
PWM             # PWM output using timer
RegisterMacros  # Macros for portable registers
Serial          # Replacement for most printf conversions
SerialLong      # More (lesser used)   printf conversions
SPI             # Interrupt       SPI interface
SPIInline       # inline/blocking SPI
Timer           # Timer
TimerB          # Timer B
TimerMacros     # Macros for portable timer
TimerMS         # Millisecond timer, for devices that have one
UART            # Buffered UART interface

## Common useful hardware

Button          # Simple button with debounce
Encoder         # Quadrature encoder
CricketBus      # Cricket bus
ESP8266         # ESP8266 serial commands
Freq            # Generate sq wave frequencies
Limit           # Limit switch
Motor           # On/Off control of motors
MotorPWM        # PWM control of motors
Regression      # Linear regression w/o using arrays
Servo           # RC Servo motors
SqWave          # Square waves using timer
Stepper         # Control steppers
TPDev           # Touchpad
VT100           # VT100 screen graphics control
ZCross          # 60 Hz zero-cross for X11 interface

## Specific chip interfaces

ACS712          # Current sensor
AD8400          # 10K Digital Pot
AD9833          # Sin/Sq/Triangle generator
AD9834          # Sin/Sq/Triangle generator
AD9850          # Sin/Sq          generator
ADNS2610        # Optical flow sensor
BTN8962         # Half bridge
MAX7219         # Control one   8-digit LED
MAX7219-8       # Control eight 8-digit LED
MCP4131         # 50K digital pot
MCP4161         # 10K digital pot
PAM8403         # Stereo amplifier
TCD1304         # Linear image sensor

## Generic useful functions

GetLine         # Terminal input, with BS and CR/LF processing
Parse           # Tokenize an input string
Parse2ch        # Generic parse of 2ch command with arguments
````

## Installation and usage

All files are intended to be included in your project, and are free to use without attribution.

Clone the project and copy whichever files you need into your project source directory. Generally
speaking, each interface consists of one .c and one .h file named after the hardware. For example,
the files [Stepper.c, Stepper.h] are the files for the stepper motor interface.

Some of the interfaces are standalone, and some are built on top of the standalone functions. For
example, the "Serial" interface requires the "UART" interface. Serial deals with formatting and
printing, while UART manages the hardware.

Dependencies are easy to find - just compile your project (or one of the test programs) and see which .h
files are missing, then copy over those files as well.

## Test programs

The library includes programs to isolate and test specific hardware. For example, you can
compile and a Servo test program to see if your servo hardware is working properly.

Input device test programs report status and changes via the serial port. For example, the touchpad
test program simply prints (to the serial port) coordinates when the user touches the pad.

Output device test programs of the form xxxCmd take commands over the serial port to control the
specific hardware. Programs of the form xxxTest continuously run a demo program of some sort that
exercises the hardware, allowing the engineer to focus on the electrical interface.

For example:

* ServoCmd allows the user to set the servo to a specific position by serial command.
* ServoTest continuously moves the servo Left->Middle->Right->Middle, pausing
at each position.

Test programs in the project:

````bash
AD9834Test          # Generate sin/sq/ frequencies by command
AtoDTest            # Continuously show a screen of all AtoD inputs
AUARTTest           # Continuously send/receive serial text
BlinkLED            # Continuously blinks an LED
ButtonTest          # Report all button presses
ComparatorTest      # Report transitions seen
CounterTest         # Report pulses counted each second
CricketLEDTest      # Run demo pattern on Cricket LED
Digital3PotCmd      # Control 3 digital pots
DigitalLogger       # Show state changes for up to 8 GPIOs
DigitalPotCmd       # Control 1 digital pot
DigitalPotTest      # Continuously change digital pot values
EncoderTest         # Continuously report encoder changes
ESP8266Cmd          # I don't know what this does
I2CCmd              # Explore I2C devices form command line
LimitTest           # Report limit switch transitions
MAX7219-8Test       # Scroll the alphabet across 8 LED arrays
MAX7219Test         # Run demo program on LED array
MotorCmd            # Control H-bridge motor on/off and dir
MotorPWMCmd         # Control motor speed by PWM command
MotorPWMTest        # Run PWM demo motor control program
MotorTest           # Run on/off demo motor control
PortMonitor         # Report pin changes in hex and binary
PulseGenerator      # Generate pulses by freq and width
SerialTest          # Run demo program testing serial port
ServoCmd            # Command RC servos
ServoTest           # Run RC servo demo
SetOutput           # Set GPIO outputs high/low by command
SqWaveCmd           # Generate square waves by command
StepperPulse        # Control stepper motor by command
StepperTest         # Run stepper motor demo program
TimerBTest          # Blink an LED using the timer
TimerMSTest         # Write serial msg once/sec using MS timer
TimerTest           # Blink an LED using timer
TPDevTest           # Report touchpad changes
UARTTest            # Simple test of serial port hardware
ZCrossTest          # Count and report zero crossings
````

## Documentation

Comprehensive documentation is included in the .h file for each device.

Documentation for the test programs is at the top of the source for that program.

Here's an example, from Stepper.h:

````C
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
//  VERSION:    2015.01.05
//
//  NOTES:
//
//      Based on "Generate stepper-motor speed profiles in real time" by David Austin
//
````


## System requirements

All files depend on AVR libc. These are standard, but it shouldn't be too hard to port to a
different development environment if needed. Most of the dependence is in .h files
like <stdint.h> and <string.h> and <stdbool.h> that you should be using anyway.
