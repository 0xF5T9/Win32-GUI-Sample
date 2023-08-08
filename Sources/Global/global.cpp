/**
 * @file global.cpp
 * @brief Define global variables.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../../Includes/my_includes.h"       // My class declarations.
#include "../../Includes/my_enums.h"          // My enum declarations.

using std::wstring, std::map, std::vector, std::filesystem::path, Gdiplus::GdiplusStartupInput;

/*******************************
 * GLOBAL VARIABLE DEFINITIONS *
 *******************************/

// Application parameters:
extern const wstring g_AppSettingsFileName = L"settings.cfg"; // Setting file name.
extern const wstring g_AppLogFileName = L"log.log";           // Log file name.
INT g_CurrentWindowWidth = 519;                               // Current application window's width.
INT g_CurrentWindowHeight = 566;                              // Current application window's height.
INT g_AppLogLevel = 1;                                        // Current application log level.
MyTheme g_CurrentAppTheme = MyTheme::Monokai;                 // Current application theme.
path g_AppDirectoryPath;                                      // Application directory path.

// Application states:
bool g_IsWindowReady = false;                       // Indicate whether the application is initialized and ready.
bool g_IsCurrentThemeWantScrollbarsVisible = false; // Indicate whether the current application theme have its scrollbar visible.
bool g_IsAppResizing = false;                       // Indicate whether the application is resizing.
bool g_IsWindowMinimized = false;                   // Indicate whether the application window is maximized.
bool g_IsWindowMaximized = false;                   // Indicate whether the application window is maximized.
bool g_IsWindows11BorderAttributeSupported = false; // Indicate whether the DWMWA_BORDER_COLOR attribute flag is supported on the current OS.

// Graphic-related variables:
MyD2D1Engine *g_pD2D1Engine; // My Direct2D engine object.

// UI-related variables:
UIElements *g_pUIElements; // UI manager object.

// WinAPI mains:
HINSTANCE g_hInstance;                       // Handle to the application instance (HINSTANCE).
HWND g_hWnd;                                 // Handle to the main window.
vector<MyWindow *> g_VectorNonClientWindows; // Vector that holds the handles of non-client window controls.

// WinAPI handles:
MyContainer *g_ContainerMainContent = nullptr; // Main application container.

// My subclass class objects:
MyRadioGroup SampleRadio; // Sample radio group object.

// Key toggle (Capture keydown events):
KeyToggle g_KeyToggleENTER(VK_RETURN);

// Initialization variables:
extern const LPCWSTR g_WindowClassName = L"Win32GUISample"; // Main window's class name.

GdiplusStartupInput g_APIGDIStartupInput; // GDI+ startup input. (GDI+ initialization variable)
ULONG_PTR g_APIGDIToken;                  // GDI+ token.         (GDI+ initialization variable)

IUIAnimationManager *g_pAnimationManager;            // WAM global animation manager.
IUIAnimationTimer *g_pAnimationTimer;                // WAM global animation timer.
IUIAnimationTransitionLibrary *g_pTransitionLibrary; // WAM global standard transition library.

// Miscellaneous variables:
DWORD g_WindowsMajorVersion = 0; // Windows major version.
DWORD g_WindowsMinorVersion = 0; // Windows minor version.
DWORD g_WindowsBuildNumber = 0;  // Windows build number.