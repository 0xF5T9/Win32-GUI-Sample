/**
 * @file global.h
 * @brief The distribution header of the global variables, forward declarations and my class declarations.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include "../../Includes/my_includes.h" // My class declarations.
#include "../../Includes/my_enums.h"    // My enum definitions.

/*****************************
 * GLOBAL VARIABLE EXTERNALS *
 *****************************/

// Application parameters:
extern const LPCWSTR g_WindowClassName;
extern const std::wstring g_AppSettingsFileName;
extern const std::wstring g_AppLogFileName;
extern DWORD g_WindowsMajorVersion;
extern DWORD g_WindowsMinorVersion;
extern DWORD g_WindowsBuildNumber;
extern INT g_CurrentWindowWidth;
extern INT g_CurrentWindowHeight;
extern INT g_AppLogLevel;
extern MyTheme g_CurrentAppTheme;
extern std::filesystem::path g_AppDirectoryPath;
extern UIElements *g_pUIElements;

// Application states:
extern bool g_IsWindowReady;
extern bool g_IsCurrentThemeWantScrollbarsVisible;
extern bool g_IsAppResizing;
extern bool g_IsWindowMinimized;
extern bool g_IsWindowMaximized;
extern bool g_IsWindows11BorderAttributeSupported;

// Key toggle (Capture keydown events):
extern KeyToggle g_KeyToggle_ENTER;

// WinAPI mains:
extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern std::vector<MyWindow *> g_VectorMyWindow_NonClient;

// WinAPI handles:
extern MyContainer *g_Container_MainContent;

// My class objects:
extern MyRadioGroup SampleRadio;

// Initialization variables:
extern Gdiplus::GdiplusStartupInput g_APIGDIStartupInput;
extern ULONG_PTR g_APIGDIToken;
extern IUIAnimationManager* g_pAnimationManager;
extern IUIAnimationTimer *g_pAnimationTimer;
extern IUIAnimationTransitionLibrary* g_pTransitionLibrary;

#endif // GLOBAL_H