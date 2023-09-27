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

#set(        Sources AtoD.c AUART.c Comparator.c Counter.c EEPROM.c Freq.c I2C.c PWM.c)
#set(        Headers AtoD.h AUART.h Comparator.h Counter.h EEPROM.h Freq.h I2C.h PWM.h)

set(        Sources AtoD.c AUART.c Comparator.c EEPROM.c Freq.c I2C.c PWM.c)
set(        Headers AtoD.h AUART.h Comparator.h EEPROM.h Freq.h I2C.h PWM.h)

list(APPEND Sources Regression.c Serial.c SerialLong.c TimerB.c Timer.c UART.c)
list(APPEND Headers Regression.h Serial.h SerialLong.h TimerB.h Timer.h UART.h)

list(APPEND Sources BadInt.c)

list(APPEND Headers PortMacros.h RegisterMacros.h TimerMacros.h)
list(APPEND Headers AtoDInline.h SPIInline.h TimerOld2.h TimerMS.h)


TargetLib(Atmega)


