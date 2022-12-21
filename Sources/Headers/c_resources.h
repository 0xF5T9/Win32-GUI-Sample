/*
    File: c_resource.h
    Application constants and project resource header
*/

#include "../Resources/resource.h"

/*
    IDI_ICON1                       cross_black
    IDI_ICON2                       cross_grey
    IDI_ICON3                       cross_white
    IDI_ICON4                       minus_black
    IDI_ICON5                       minus_grey
    IDI_ICON6                       minus_white
*/

#pragma once

// Button identifiers
#define BUTTON_CLOSE 101
#define BUTTON_MINIMIZE 102
#define BUTTON_LOREMIPSUM 103

// Miscs
#define BORDER_WIDTH 1
#define CAPTIONBAR_HEIGHT 37
#define STD_SCROLLBAR_WIDTH 20
#define MAINCONTENTCTR_PADDING 10

// Custom window style
#define WS_MYSTYLE (    WS_POPUP          | \
                        WS_THICKFRAME     | \
                        WS_CAPTION        | \
                        WS_MINIMIZEBOX    | \
                        WS_MAXIMIZEBOX    | \
                        WS_CLIPCHILDREN)