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

set(Sources Command.c CustomScreens.c Debug.c Parse2ch.c Parse.c Screen.c)
set(Headers Command.h CustomScreens.h Debug.h Parse2ch.h Parse.h Screen.h VT100.h)

list(APPEND Sources DEScreen.c Dump.c EEScreen.c GetLine.c HEScreen.c MAScreen.c MEScreen.c)
list(APPEND Headers DEScreen.h Dump.h EEScreen.h GetLine.h HEScreen.h MAScreen.h MEScreen.h)

TargetLib(Screen)

