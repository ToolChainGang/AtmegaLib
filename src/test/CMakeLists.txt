########################################################################################################################
########################################################################################################################
##
##      Copyright (C) 2021 Rajstennaj Barrabas, Milford, NH 03055
##      All Rights Reserved under the MIT license as outlined below.
##
##  FILE
##
##      CMake script
##
########################################################################################################################
##
##  MIT LICENSE
##
##  Permission is hereby granted, free of charge, to any person obtaining a copy of
##    this software and associated documentation files (the "Software"), to deal in
##    the Software without restriction, including without limitation the rights to
##    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
##    of the Software, and to permit persons to whom the Software is furnished to do
##    so, subject to the following conditions:
##
##  The above copyright notice and this permission notice shall be included in
##    all copies or substantial portions of the Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
##    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
##    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
##    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
##    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
##    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##
########################################################################################################################
########################################################################################################################

#
# Traditionally, we should only load the individual libraries needed for any test program.
#   Right now I'm too lazy, and it doesn't matter: just load them all and let the linker
#   sort them out.
#
set(AllLibs Screen Chips IO Atmega)

TargetExec(TimerTest        ${AllLibs})


TargetExec(AD9834Test       ${AllLibs})
TargetExec(ADNS2610Test     ${AllLibs})
TargetExec(AtoDTest         ${AllLibs})
TargetExec(AUARTTest        ${AllLibs})
TargetExec(ButtonTest       ${AllLibs})
TargetExec(ComparatorTest   ${AllLibs})
#TargetExec(CounterTest      ${AllLibs})
TargetExec(CricketLEDTest   ${AllLibs})
TargetExec(DigitalPotTest   ${AllLibs})
TargetExec(EncoderTest      ${AllLibs})
TargetExec(LimitTest        ${AllLibs})
TargetExec(MAX7219Test      ${AllLibs})
TargetExec(MotorPWMTest     ${AllLibs})
TargetExec(MotorTest        ${AllLibs})
TargetExec(SerialTest       ${AllLibs})
TargetExec(ServoTest        ${AllLibs})
#TargetExec(StepperPulse     ${AllLibs})
#TargetExec(StepperTest      ${AllLibs})
TargetExec(TimerBTest       ${AllLibs})
TargetExec(TimerMSTest      ${AllLibs})
#TargetExec(TimerTest        ${AllLibs})
TargetExec(TPDevTest        ${AllLibs})
TargetExec(UARTTest         ${AllLibs})
TargetExec(WS2812Test       ${AllLibs})
TargetExec(ZCrossTest       ${AllLibs})
