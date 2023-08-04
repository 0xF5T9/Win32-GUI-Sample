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
extern const LPCWSTR g_WindowClassName = L"Win32GUISample";   // Main window's class name.
extern const wstring g_AppSettingsFileName = L"settings.cfg"; // Default setting file name.
extern const wstring g_AppLogFileName = L"log.log";           // Default log file name.
DWORD g_WindowsMajorVersion = 0;
DWORD g_WindowsMinorVersion = 0;
DWORD g_WindowsBuildNumber = 0;
INT g_CurrentWindowWidth = 519;                                   // Default application window's width.
INT g_CurrentWindowHeight = 566;                                  // Default application window's height.
INT g_AppLogLevel = 1;                                        // Default Application debug level.
MyTheme g_CurrentAppTheme = MyTheme::Monokai;                        // Default application theme.
path g_AppDirectoryPath;                                      // Application directory path.
UIElements *g_pUIElements; // Global UI objects.

// Application states:
bool g_IsWindowReady = false;                          // Indicate whether the application is initialized and ready.
bool g_IsCurrentThemeWantScrollbarsVisible = false;          // Indicate whether the current application theme have its scrollbar visible.
bool g_IsAppResizing = false;                       // Indicate whether the application is resizing.
bool g_IsWindowMinimized = false;                // Indicate whether the application window is maximized.
bool g_IsWindowMaximized = false;                // Indicate whether the application window is maximized.
bool g_IsWindows11BorderAttributeSupported = false; // Indicate whether the DWMWA_BORDER_COLOR attribute flag is supported on the current OS.

// Key toggle (Capture keydown events):
KeyToggle g_KeyToggle_ENTER(VK_RETURN);

// WinAPI mains:
HINSTANCE g_hInstance;                         // Handle to the application instance (HINSTANCE).
HWND g_hWnd;                                   // Handle to the main window.
vector<MyWindow *> g_VectorMyWindow_NonClient; // Vector that holds the handles of non-client window controls.

// WinAPI handles:
MyContainer *g_Container_MainContent = nullptr; // Main application container.

// My subclass class objects:
MyRadioGroup SampleRadio;

// Initialization variables:
GdiplusStartupInput g_APIGDIStartupInput; // GDI+
ULONG_PTR g_APIGDIToken;                  // GDI+

IUIAnimationManager* g_pAnimationManager;            // Global animation manager. (WAM)
IUIAnimationTimer *g_pAnimationTimer;                // Global animation timer. (WAM)
IUIAnimationTransitionLibrary* g_pTransitionLibrary; // Global standard transition library. (WAM)