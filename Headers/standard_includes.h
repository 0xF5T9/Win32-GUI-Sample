/**
 * @file standard_includes.h
 * @brief Include standard library headers.
 */

#ifndef STANDARD_INCLUDES_H
#define STANDARD_INCLUDES_H

#define NOMINMAX    // Defines NOMINMAX to prevent the Windows header <Windows.h> from defining its own min and max macros.
#define OEMRESOURCE // Enable OEM resource marcos.

// Define WINVER and _WIN32_WINNT marco to oldest platform value the application want to support before including `sdkddkver.h` header.
// 0x0A00 - Windows 10
// Readmore: https://learn.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-170
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <sdkddkver.h>   // Prevent Windows API-incompatible functionality.
#include <exception>     // Exception handling.
#include <string>        // String handling.
#include <cwctype>       // Character classification and conversion.
#include <regex>         // Regular expressions (String conversions).
#include <cmath>         // Mathematical functions and constants.
#include <limits>        // Numeric limits.
#include <locale>        // Localization.
#include <exception>     // Exception handling.
#include <memory>        // Memory management utilities.
#include <fstream>       // File input/output.
#include <filesystem>    // Filesystem operations (File paths).
#include <map>           // Associative containers (Animations).
#include <set>           // Unique element containers (Store temporary timers).
#include <vector>        // Sequence containers (C Style array alternative).
#include <windows.h>     // Windows API: Essentials.
#include <windowsx.h>    // Windows API: Macro functions for handling window messages.
#include <uxtheme.h>     // Windows API: Visual themes and styles.
#include <commctrl.h>    // Windows API: Common controls for UI elements.
#include <dwmapi.h>      // Windows API: Desktop window manager functions.
#include <mmsystem.h>    // Windows API: Multimedia services and functions.
#include <uianimation.h> // Windows API: Windows Animation Manager (WAM).
#include <comdef.h>      // Windows API: Component Object Model (COM) error handling.
#include <powrprof.h>    // Windows API: Power management functions and settings.
#include <d2d1.h>        // Windows API: Direct2D (D2D1), hardware-accelerated drawing functions.
#include <dwrite.h>      // Windows API: DirectWrite (DWrite), hardware-accelerated text rendering.
#include <wincodec.h>    // Windows API: Windows Imaging Component (WIC), image processing.
#include <richedit.h>    // Windows API: Rich Edit control classes.
#include <shobjidl.h>    // Windows API: Working with shell objects.

#endif // STANDARD_INCLUDES_H