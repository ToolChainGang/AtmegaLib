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

TargetExec(BlinkLED         ${AllLibs})
#TargetExec(Digital3PotCmd   ${AllLibs})
TargetExec(DigitalLogger    ${AllLibs})
TargetExec(DigitalPotCmd    ${AllLibs})
TargetExec(ESP8266Cmd       ${AllLibs})
TargetExec(I2CCmd           ${AllLibs})
TargetExec(MotorCmd         ${AllLibs})
TargetExec(MotorPWMCmd      ${AllLibs})
TargetExec(PortMonitor      ${AllLibs})
TargetExec(PulseGenerator   ${AllLibs})
TargetExec(ServoCmd         ${AllLibs})
TargetExec(SetOutput        ${AllLibs})
TargetExec(SqWaveCmd        ${AllLibs})
#TargetExec(StepperPulse     ${AllLibs})
