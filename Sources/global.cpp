/*
*  File: global.cpp
*  Global variables definitions
*/

#include <iostream>
#include <string>
#include <locale>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <map>
#include <vector>
#include <Windows.h>
#include <Windowsx.h>
#include <gdiplus.h>
#include "./Headers/obj_manager.h"
#include "./Headers/subclasses.h"
#include "./Headers/c_resources.h"

using std::wstring, std::map, std::vector, Gdiplus::GdiplusStartupInput;

/**************************
*    GLOBAL PARAMETERS    *
***************************/

extern const LPCWSTR WndClassName = L"Win32GUISample";          // Main window's class name
extern const std::wstring SettingFileName = L"settings.cfg";    // Setting file name
extern const std::wstring LogFileName = L"log.txt";             // Log file name
int APPLICATION_WIDTH = 519, APPLICATION_HEIGHT = 566;          // Default main window's size
wstring APPLICATION_THEME = L"Obisidan";                        // Default theme
WCHAR* APPLICATION_PATH;                                        // Application directory path (ptr)
int DEBUG_LEVEL = 1;                                            // Debug level

// HWND placeholders that keep track of which button from its group is selected
HWND CURRENT_SELECTEDRADIO2;
HWND CURRENT_SELECTEDRADIO3;

// Status bools (DO NOT CHANGE DEFAULT VALUES)
bool IS_APPREADY = 0,                               // App readiness check, set to true once the application is ready to display.
IS_APPTHEMESHOWSCROLLBAR = 0,                       // Check if current selected theme want scrollbar visible
IS_WINDOWMAXIMIZED = 0;                             // Check if the main window is maximized




/*************************************
*    API INITIALIZATION VARIABLES    *
**************************************/

// GDI+ initialization variables
GdiplusStartupInput API_StartupInput;
ULONG_PTR API_Token;




/*************************************
*    OBJECT AND SUBCLASS MANAGERS    *
**************************************/

// Key Toggles
KeyToggle ktEnterKey(VK_RETURN);

// Object managers
OBJ_Manager* OBJM_Main;

// Subclass managers
BA_CaptionBar* BA_CaptionBar_Manager;
BA_Standard* BA_Standard_Manager;
BA_Radio2* BA_Radio2_Manager;
BA_Radio3* BA_Radio3_Manager;
CBDL_CustomDraw* CBDL_CustomDraw1_Manager;




/***********************
*    WINDOW HANDLES    *
************************/

// Main window handles
HINSTANCE MAIN_HINSTANCE;
HWND MAIN_HWND;
HWND BTN_Close, BTN_Minimize, SS_Title;

// Container content handles (SSCTR_MAINCONTENT)
HWND SSCTR_MAINCONTENT, SB_SSCTR_MAINCONTENT;
HWND SS_Heading1,
	BTN_Standard, BTN_Radio2Left, BTN_Radio2Right, BTN_Radio3Left, BTN_Radio3Middle, BTN_Radio3Right;
HWND SS_Heading2,
	SS_TextNoteNormalEditbox,  SS_ED_Normal, ED_Normal, BTN_NormalEditboxOK,
	SS_TextNotePasswordEditbox, SS_ED_Password, ED_Password, BTN_PasswordEditboxOK,
	SS_TextNoteMultilineEditbox, SS_ED_Multiline, ED_Multiline, BTN_MultilineEditboxOK;
HWND SS_Heading3,
	SS_TextNoteCBSelectTheme, CB_SelectTheme;
vector<HWND*> VECTOR_SSCTR_MAINCONTENT;   // Contains SSCTR_MAINCONTENT childs hwnd that need to be updated with SetWindowTheme() upon theme changes



/**************************
*    RECTANGLES (RECT)    *
***************************/

RECT RECT_Caption = { BORDER_WIDTH, BORDER_WIDTH, APPLICATION_WIDTH - BORDER_WIDTH, BORDER_WIDTH + CAPTIONBAR_HEIGHT };
RECT RECT_SizeBorder_Top = { 0, 0, APPLICATION_WIDTH, BORDER_WIDTH },
RECT_SizeBorder_Bottom = { 0, APPLICATION_HEIGHT - BORDER_WIDTH, APPLICATION_WIDTH, APPLICATION_HEIGHT },
RECT_SizeBorder_Left = { 0, 1, BORDER_WIDTH, APPLICATION_HEIGHT - 1 },
RECT_SizeBorder_Right = { APPLICATION_WIDTH - BORDER_WIDTH, 1, APPLICATION_WIDTH, APPLICATION_HEIGHT - 1 };




/**********************
*    MISCELLANEOUS    *
***********************/

vector<HWND*> Vector_Subclasses;       // Contains HWNDs that is subclassed