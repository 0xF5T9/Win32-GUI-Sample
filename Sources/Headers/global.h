/*
	File: global.h
	Global variables distribution header file
*/

#pragma once
#include <map>
#include <vector>
#include <gdiplus.h>

// APPLICATION PARAMETERS
extern LPCWSTR WndClassName;
extern int APPLICATION_WIDTH, APPLICATION_HEIGHT;
extern bool IS_APPREADY, IS_APPTHEMESHOWSCROLLBAR;
extern std::wstring APPLICATION_THEME;
extern HWND CURRENT_SELECTEDRADIO2;
extern HWND CURRENT_SELECTEDRADIO3;

// API OBJECTS
extern Gdiplus::GdiplusStartupInput API_StartupInput;
extern ULONG_PTR API_Token;

// MAIN WINDOW HANDLES
extern HINSTANCE MAIN_HINSTANCE;
extern HWND MAIN_HWND;
extern HWND BTN_Close, BTN_Minimize, SS_Title;
extern HWND SS_MAINCONTENTCTR, SB_MAINCONTENTCTR;
extern HWND SS_Heading1, BTN_Standard, BTN_Radio2Left, BTN_Radio2Right, BTN_Radio3Left, BTN_Radio3Middle, BTN_Radio3Right;

// RECTS
extern RECT RECT_Caption;
extern RECT RECT_SizeBorder_Top,
RECT_SizeBorder_Bottom,
RECT_SizeBorder_Left,
RECT_SizeBorder_Right;

// BRUSHES | PENS | FONTS
extern COLORREF CLR_DEBUG,
CLR_Primary, CLR_Secondary, CLR_BorderActive, CLR_BorderInactive,
CLR_DefaultTextColor, CLR_DefaultInactiveTextColor, CLR_CloseBtnHover, CLR_MinimizeBtnHover;
extern HBRUSH hBrush_CURBORDER, hBrush_DEBUG,
hBrush_Primary, hBrush_Secondary, hBrush_BorderActive, hBrush_BorderInactive;
extern HFONT hFont_Default, hFont_Title, hFont_Heading;

// VECTORS (Manage objects for memory deallocation or visibility of control handles)
extern std::vector<HBRUSH*> Vector_MainObjects_Brushes;
extern std::vector<HFONT*> Vector_MainObjects_Fonts;
extern std::vector<HICON*> Vector_MainObjects_Icons;
extern std::vector<HBRUSH*> Vector_StaticObjects_Brushes;
extern std::vector<HWND*> Vector_Subclasses;
extern std::vector<HWND*> Vector_MAINCONTENTCTR;
extern std::vector<HBRUSH*> Vector_Subclasses_BACaptionBar_Brushes;
extern std::vector<HBRUSH*> Vector_Subclasses_BAStandard_Brushes;
extern std::vector<HBRUSH*> Vector_Subclasses_BARadio2_Brushes;
extern std::vector<HBRUSH*> Vector_Subclasses_BARadio3_Brushes;		
extern std::vector<HPEN*> Vector_Subclasses_BARadio3_Miscs;
extern std::map<std::pair<HWND*, HICON*>, std::pair<HICON*, HICON*>> HoverMap_1;

// ICONS & BITMAPS HANDLES
extern HICON hIcon_Close, hIcon_Close_NF, hIcon_Close_H, hIcon_Minimize, hIcon_Minimize_NF, hIcon_Minimize_H;