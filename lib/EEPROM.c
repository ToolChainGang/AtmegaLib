//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2015 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      EEPROM.c
//
//  SYNOPSIS
//
//      See EEPROM.h for a complete description
//
//  VERSION:    2010.12.06
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

#include <avr/eeprom.h>

#include <PortMacros.h>

#include "EEPROM.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// Data declarations
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

EEPROM_T    EEPROM NOINIT;

PROGMEM EEPROM_T EEPROMDefaults = EEPROM_DEFAULTS;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// EEPROMInit - Initialize RAM copy of EEPROM 
//
// Inputs:      None.
//
// Outputs:     None.
//
void EEPROMInit(void) {

    eeprom_read_block(&EEPROM,0,sizeof(EEPROM));

    //
    // If uninitialized, or if version mismatch (we're the newer version), initialize
    //   with defaults
    //
    if( EEPROM.Version != EEPROM_CURR_VERSION ) {
        memcpy_P(&EEPROM,&EEPROMDefaults,sizeof(EEPROM));
        EEPROMWrite();
        }
    }


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
// EEPROMWrite - Write new EEPROM values from RAM
//
// Inputs:      None.
//
// Outputs:     None.
//
void EEPROMWrite(void) {

    eeprom_write_block(&EEPROM,0,sizeof(EEPROM));
    }
