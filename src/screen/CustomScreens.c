////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Copyright (C) 2010 Peter Walsh, Milford, NH 03055
//      All Rights Reserved under the MIT license as outlined below.
//
//  FILE
//      CustomScreens.c
//
//  DESCRIPTION
//
//      Custom screens for the application
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

#include "CustomScreens.h"
#include "Command.h"
#include "VT100.h"

#ifdef USE_CUSTOM_SCREENS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ShowCustomScreens - Display the user-defined screens
//
// Inputs:      Two char abbreviation of screen (ie - 'DE')
//
// Outputs:     TRUE  if screen correctly shown
//              FALSE if screen ID unknown
//
bool ShowCustomScreens(int ScreenType) {

    switch(ScreenType) {

#ifdef USE_XX_SCREEN
        //
        // XX - Show the XX screen
        //
        case 'XX':
            ShowXXScreen();
            return true;
#endif
        }

    return false;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UpdateCustomScreens - Update the user-defined screens
//
// Inputs:      Two char abbreviation of screen (ie - 'DE')
//
// Outputs:     TRUE  if screen correctly updated
//              FALSE if screen ID unknown
//
bool UpdateCustomScreens(int ScreenType) {

    switch(ScreenType) {

#ifdef USE_XXSCREEN
        //
        // XX - Update the debug screen
        //
        case 'XX':
            UpdateXXScreen();
            return true;
#endif
        }

    return false;
    }

#endif // USE_CUSTOM_SCREENS
