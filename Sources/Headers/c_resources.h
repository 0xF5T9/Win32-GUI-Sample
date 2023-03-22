/*
*  File: c_resource.h
*  Application constants and project resource header
*/

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING    // Deprecated in C++17

#include "../Resources/resource.h"

/*****************************************************
*
*   IDI_ICON1                       cross_black
*   IDI_ICON2                       cross_grey
*   IDI_ICON3                       cross_white
*   IDI_ICON4                       minus_black
*   IDI_ICON5                       minus_grey
*   IDI_ICON6                       minus_white
*
******************************************************/

#pragma once

// Button identifiers
#define BUTTON_CLOSE 101
#define BUTTON_MINIMIZE 102
#define BUTTON_STANDARD 103
#define BUTTON_R2LEFT 104
#define BUTTON_R2RIGHT 105
#define BUTTON_R3LEFT 106
#define BUTTON_R3MIDDLE 107
#define BUTTON_R3RIGHT 108
#define BUTTON_EDNORMALOK 109
#define BUTTON_EDPASSWORDOK 110
#define BUTTON_EDMULTILINEOK 111

// miscellaneous
#define BORDER_WIDTH 1                 // Main window border with (pixel)
#define CAPTIONBAR_HEIGHT 37           // Main window caption bar height (pixel)
#define STD_SCROLLBAR_WIDTH 20         // Standard scrollbar width (pixel) 
#define MAINCONTENTCTR_PADDING 10      // "MAINCONTENT" container padding (pixel)
#define ANIMATION_INVALIDATE_TICK 16   // Animation invalidate tick in miliseconds (60FPS)

// Custom window style
#define WS_MYSTYLE (    WS_POPUP          | \
                        WS_THICKFRAME     | \
                        WS_CAPTION        | \
                        WS_MINIMIZEBOX    | \
                        WS_MAXIMIZEBOX    | \
                        WS_CLIPCHILDREN)