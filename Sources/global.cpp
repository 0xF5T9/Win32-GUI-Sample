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
#include "./Headers/c_resources.h"

using std::wstring, std::map, std::vector, Gdiplus::GdiplusStartupInput;

// APPLICATION PARAMETERS
LPCWSTR WndClassName = L"Win32GUISample";
int APPLICATION_WIDTH = 500, APPLICATION_HEIGHT = 250; // Initiate application window size
bool IS_APPREADY = 0, SHOW_SCROLLBAR = 0;
wstring APPLICATION_THEME = L"Ristretto";

// API OBJECTS
GdiplusStartupInput API_StartupInput;
ULONG_PTR API_Token;

// MAIN WINDOW HANDLES
HINSTANCE MAIN_HINSTANCE;
HWND MAIN_HWND;
HWND BTN_Close, BTN_Minimize, SS_Title;
HWND SS_MAINCONTENTCTR, SB_MAINCONTENTCTR;
HWND SS_Test1, BTN_Test1;

// RECTS
RECT RECT_Caption = { BORDER_WIDTH, BORDER_WIDTH, APPLICATION_WIDTH - BORDER_WIDTH, BORDER_WIDTH + CAPTIONBAR_HEIGHT };
RECT RECT_SizeBorder_Top = { 0, 0, APPLICATION_WIDTH, BORDER_WIDTH },
RECT_SizeBorder_Bottom = { 0, APPLICATION_HEIGHT - BORDER_WIDTH, APPLICATION_WIDTH, APPLICATION_HEIGHT },
RECT_SizeBorder_Left = { 0, 1, BORDER_WIDTH, APPLICATION_HEIGHT - 1 },
RECT_SizeBorder_Right = { APPLICATION_WIDTH - BORDER_WIDTH, 1, APPLICATION_WIDTH, APPLICATION_HEIGHT - 1 };

// BRUSHES | PENS | FONTS
COLORREF CLR_DEBUG,
CLR_Primary, CLR_Secondary, CLR_BorderActive, CLR_BorderInactive,
CLR_DefaultTextColor, CLR_DefaultInactiveTextColor, CLR_CloseBtnHover, CLR_MinimizeBtnHover;
HBRUSH hBrush_CTLCOLORSTATIC, hBrush_CTLCOLORBTN, hBrush_CURBORDER, hBrush_DEBUG,
hBrush_Primary, hBrush_Secondary, hBrush_BorderActive, hBrush_BorderInactive;
HFONT hFont_Default, hFont_Title;

// VECTORS/MAPS (Manage objects for memory deallocation or visibility of control handles)
vector<HBRUSH*> Vector_MainObjects_Brushes;				// Contains main application brush objects
vector<HFONT*> Vector_MainObjects_Fonts;				// Contains main application font objects
vector<HICON*> Vector_MainObjects_Icons;				// Contains main application icon objects
vector<HBRUSH*> Vector_StaticObjects_Brushes;			// Contains static and debug objects
vector<HWND*> Vector_Subclasses;						// Contains HWNDs that is subclassed
vector<HWND*> Vector_MAINCONTENTCTR;					// Viewport container (Main content)
vector<HBRUSH*> Vector_Subclasses_BAStandard_Brushes;
vector<HBRUSH*> Vector_Subclasses_BACaptionBar_Brushes;	// Contains "Caption bar button animation subclass" brush objects
map<std::pair<HWND*, HICON*>, std::pair<HICON*, HICON*>> HoverMap_1; // "Caption bar button animation subclass" map

// ICONS & BITMAPS HANDLES
HICON hIcon_Close, hIcon_Close_NF, hIcon_Close_H, hIcon_Minimize, hIcon_Minimize_NF, hIcon_Minimize_H;