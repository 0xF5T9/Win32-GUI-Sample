/**
 * @file global.cpp
 * @brief Define the global variables
 */

#include "../Headers/standard_includes.h" // Include standard libraries.
#include "../Headers/my_includes.h"       // Include my user-defined type headers.

/*******************************
 * GLOBAL VARIABLE DEFINITIONS *
 *******************************/

std::unique_ptr<MyApp> g_pApp = std::make_unique<MyApp>();
MyRadioGroup SampleRadio;