/*
*  File: global.h
*  Global variables distribution header
*/

#pragma once
#include <map>
#include <vector>
#include <gdiplus.h>

/**************************
*    GLOBAL PARAMETERS    *
***************************/

extern const LPCWSTR WndClassName;
extern const std::wstring SettingFileName;
extern const std::wstring LogFileName;
extern int APPLICATION_WIDTH, APPLICATION_HEIGHT;
extern std::wstring APPLICATION_THEME;
extern WCHAR* APPLICATION_PATH;
extern int DEBUG_LEVEL;

// HWND placeholders that keep track of which button from its group is selected
extern HWND CURRENT_SELECTEDRADIO2;
extern HWND CURRENT_SELECTEDRADIO3;

// Status bools
extern bool IS_APPREADY,
IS_APPTHEMESHOWSCROLLBAR,
IS_WINDOWMAXIMIZED;




/*************************************
*    API INITIALIZATION VARIABLES    *
**************************************/

// GDI+ initialization variables
extern Gdiplus::GdiplusStartupInput API_StartupInput;
extern ULONG_PTR API_Token;




/*************************************
*    OBJECT AND SUBCLASS MANAGERS    *
**************************************/

// Key Toggles
extern KeyToggle ktEnterKey;

// Object managers
extern OBJ_Manager* OBJM_Main;

// Subclass managers
extern BA_CaptionBar* BA_CaptionBar_Manager;
extern BA_Standard* BA_Standard_Manager;
extern BA_Radio2* BA_Radio2_Manager;
extern BA_Radio3* BA_Radio3_Manager;
extern CBDL_CustomDraw* CBDL_CustomDraw1_Manager;




/***********************
*    WINDOW HANDLES    *
************************/

// Main window handles
extern HINSTANCE MAIN_HINSTANCE;
extern HWND MAIN_HWND;
extern HWND BTN_Close, BTN_Minimize, SS_Title;

// Container window handles (MAINCONTENTCTR)
extern HWND SS_MAINCONTENTCTR, SB_MAINCONTENTCTR;
extern HWND SS_Heading1,
	BTN_Standard, BTN_Radio2Left, BTN_Radio2Right, BTN_Radio3Left, BTN_Radio3Middle, BTN_Radio3Right;
extern HWND SS_Heading2,
	SS_TextNoteNormalEditbox, SS_ED_Normal, ED_Normal, BTN_NormalEditboxOK,
	SS_TextNotePasswordEditbox, SS_ED_Password, ED_Password, BTN_PasswordEditboxOK,
	SS_TextNoteMultilineEditbox, SS_ED_Multiline, ED_Multiline, BTN_MultilineEditboxOK;
extern HWND SS_Heading3,
	SS_TextNoteCBSelectTheme, CB_SelectTheme;




/**************************
*    RECTANGLES (RECT)    *
***************************/

extern RECT RECT_Caption;
extern RECT RECT_SizeBorder_Top,
RECT_SizeBorder_Bottom,
RECT_SizeBorder_Left,
RECT_SizeBorder_Right;




/**********************
*    MISCELLANEOUS    *
***********************/

extern std::vector<HWND*> Vector_Subclasses;
extern std::vector<HWND*> Vector_MAINCONTENTCTR;