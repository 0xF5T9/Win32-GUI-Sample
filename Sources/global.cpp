/*
	File: global.cpp
	Global variables definitions
*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <Windows.h>
#include <Windowsx.h>
#include <gdiplus.h>
#include "./Headers/obj_manager.h"
#include "./Headers/subclasses.h"
#include "./Headers/c_resources.h"

using std::wstring, std::map, std::vector, Gdiplus::GdiplusStartupInput;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * GLOBAL PARAMETERS

LPCWSTR WndClassName = L"Win32GUISample";				// Main window's class name
int APPLICATION_WIDTH = 519, APPLICATION_HEIGHT = 250;		// Default main window's size
bool IS_APPREADY = 0, IS_APPTHEMESHOWSCROLLBAR = 0;
wstring APPLICATION_THEME = L"Ristretto";
HWND CURRENT_SELECTEDRADIO2;
HWND CURRENT_SELECTEDRADIO3;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * API INITIALIZATION VARIABLES

// GDI+ initialization variables
GdiplusStartupInput API_StartupInput;
ULONG_PTR API_Token;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * OBJECT AND SUBCLASS MANAGERS

// Object managers
OBJ_Manager* OBJM_Main;

// Subclass managers
BA_CaptionBar* BA_CaptionBar_Manager;
BA_Standard* BA_Standard_Manager;
BA_Radio2* BA_Radio2_Manager;
BA_Radio3* BA_Radio3_Manager;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * WINDOW HANDLES

// Main window handles
HINSTANCE MAIN_HINSTANCE;
HWND MAIN_HWND;
HWND BTN_Close, BTN_Minimize, SS_Title;

// Container window handles (MAINCONTENTCTR)
HWND SS_MAINCONTENTCTR, SB_MAINCONTENTCTR;
HWND SS_Heading1, BTN_Standard, BTN_Radio2Left, BTN_Radio2Right, BTN_Radio3Left, BTN_Radio3Middle, BTN_Radio3Right;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * RECTS

RECT RECT_Caption = { BORDER_WIDTH, BORDER_WIDTH, APPLICATION_WIDTH - BORDER_WIDTH, BORDER_WIDTH + CAPTIONBAR_HEIGHT };
RECT RECT_SizeBorder_Top = { 0, 0, APPLICATION_WIDTH, BORDER_WIDTH },
RECT_SizeBorder_Bottom = { 0, APPLICATION_HEIGHT - BORDER_WIDTH, APPLICATION_WIDTH, APPLICATION_HEIGHT },
RECT_SizeBorder_Left = { 0, 1, BORDER_WIDTH, APPLICATION_HEIGHT - 1 },
RECT_SizeBorder_Right = { APPLICATION_WIDTH - BORDER_WIDTH, 1, APPLICATION_WIDTH, APPLICATION_HEIGHT - 1 };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * MISCELLANEOUS

vector<HWND*> Vector_Subclasses;						// Contains HWNDs that is subclassed
vector<HWND*> Vector_MAINCONTENTCTR;					// Contains MAINCONTENTCTR's child HWNDs that need to be updated with SetWindowTheme() upon theme changes