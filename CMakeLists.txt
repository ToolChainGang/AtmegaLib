########################################################################################################################
########################################################################################################################
##
##      Copyright (C) 2021 Rajstennaj Barrabas, Milford, NH 03055
##      All Rights Reserved under the MIT license as outlined below.
##
##  FILE
##
##      CMakeLists.txt - Top level CMake setup for personal library
##
##      Top level CMake script to compile all C library files and apps, and move the
##        results to the correct locations.
##
##  FUNCTIONS
##
##  NOTES:
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

cmake_minimum_required(VERSION 3.16)

project(ArduinoLib)

include(src/etc/CMakeMacros.txt)

SetAVR(atmega168 16000000)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 20)

#add_definitions(-DDEBUG_ASSERT)

include_directories(BEFORE src)
add_subdirectory(src)
