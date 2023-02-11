/*
*  File: subclasses.h
*  Subclasses class header
*/

#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <Dwmapi.h>


/************************
*    KEYTOGGLE CLASS    *
*************************/
// An efficient GetKeyState() alternative, help capture keydown message once instead of continuously while holding the key.
// More info : https://stackoverflow.com/questions/10790502/c-getkeystate-has-to-run-once
class KeyToggle {
public:
	KeyToggle(int key) :mKey(key), mActive(false) {}
	operator bool() {
		if (GetAsyncKeyState(mKey)) {
			if (!mActive) {
				mActive = true;
				return true;
			}
		}
		else
			mActive = false;
		return false;
	}
private:
	int mKey;
	bool mActive;
};




/*********************************************
*    BUTTON ANIMATION CLASS (CAPTION BAR)    *
**********************************************/
// Button animation subclass, hard-customized for caption bar button controls.
class BA_CaptionBar
{
private:
	bool IsReady = 0;                                     // Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 150;          // Hover animation duration
	unsigned short LBDownAnimationDuration = 150;         // Left-button down animation duration
	HBRUSH hBrush_ButtonColor_Default = nullptr;          // Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;            // Hovering button color
	HBRUSH hBrush_ButtonColor_HoverClose = nullptr;       // Hovering button color (Close)
	HBRUSH hBrush_ButtonColor_HoverMinimize = nullptr;    // Hovering button color (Minimize)
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;           // Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;        // Button background color
	std::map<std::pair<HWND*, HICON*>, std::pair<HICON*, HICON*>> AnimationMap;
public:
	BA_CaptionBar() {}
	~BA_CaptionBar() 
	{
		DeleteObject(hBrush_ButtonColor_Default);
		DeleteObject(hBrush_ButtonColor_Hover);
		DeleteObject(hBrush_ButtonColor_HoverClose);
		DeleteObject(hBrush_ButtonColor_HoverMinimize);
		DeleteObject(hBrush_ButtonColor_LBDown);
		DeleteObject(hBrush_ButtonBackgroundColor);
	}
public:
	void ClearAnimationMap();
	void InsertAnimationMap(HWND& hWnd, HICON& hIcon_Default, HICON& hIcon_Hover, HICON& hIcon_Inactive);
	void UpdateObjects(COLORREF Default, COLORREF Hover, COLORREF Hover_Close, COLORREF Hover_Minimize, COLORREF LBDown, COLORREF Background);
public:
	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated);
	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H);
	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB);
};




/******************************************
*    BUTTON ANIMATION CLASS (STANDARD)    *
*******************************************/
// Standard button animation subclass, a rounded rectangle button.
// Each object have its own color objects and can be use for multiple button controls.
class BA_Standard
{
private:
	bool IsReady = 0;                                     // Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 150;          // Hover animation duration
	unsigned short LBDownAnimationDuration = 150;         // Left-button down animation duration
	Gdiplus::Color CL_ButtonColor_Default;                // Default button color
	Gdiplus::Color CL_ButtonColor_Hover;                  // Hovering button color
	Gdiplus::Color CL_ButtonColor_LBDown;                 // Left-button down button color
	Gdiplus::Color CL_ButtonBackgroundColor;              // Button background color
	Gdiplus::Color CL_ButtonBorderColor_NonFocus;         // Button border color (Non-Focus)
	Gdiplus::Color CL_ButtonBorderColor_OnFocus;          // Button border color (On-Focus)
	HBRUSH hBrush_ButtonColor_Default = nullptr;          // Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;            // Hovering button color
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;           // Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;        // Button background color
	HFONT* hFont_ButtonFont = nullptr;                    // Button text font
	COLORREF CLR_DefaultTextColor = RGB(0, 0, 0);         // Default button text color
	COLORREF CLR_HighlightTextColor = RGB (0, 0, 0);      // Highlight button text color
public:
	BA_Standard() {}
	~BA_Standard() 
	{
		DeleteObject(hBrush_ButtonColor_Default);
		DeleteObject(hBrush_ButtonColor_Hover);
		DeleteObject(hBrush_ButtonColor_LBDown);
		DeleteObject(hBrush_ButtonBackgroundColor);
	}
public:
	void UpdateObjects(Gdiplus::Color Default, Gdiplus::Color Hover, Gdiplus::Color LBDown, Gdiplus::Color Background, Gdiplus::Color BorderNonFocus, Gdiplus::Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor);
public:
	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated);
	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H);
	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB);
};




/****************************************
*    BUTTON ANIMATION CLASS (RADIO2)    *
*****************************************/
// Radio button animation subclass, left and right buttons.
// Each object have its own color objects and can only be used for a single button control set.
class BA_Radio2
{
private:
	bool IsReady = 0;                                     // Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 100;          // Hover animation duration
	unsigned short LBDownAnimationDuration = 300;         // Left-button down animation duration
	Gdiplus::Color CL_ButtonColor_Default;                // Default button color
	Gdiplus::Color CL_ButtonColor_Hover;                  // Hovering button color
	Gdiplus::Color CL_ButtonColor_LBDown;                 // Left-button down button color
	Gdiplus::Color CL_ButtonBackgroundColor;              // Button background color
	Gdiplus::Color CL_ButtonBorderColor_NonFocus;         // Button border color (Non-Focus)
	Gdiplus::Color CL_ButtonBorderColor_OnFocus;          // Button border color (On-Focus)
	HBRUSH hBrush_ButtonColor_Default = nullptr;          // Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;            // Hovering button color
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;           // Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;        // Button background color
	HFONT* hFont_ButtonFont = nullptr;                    // Button text font
	COLORREF CLR_DefaultTextColor = RGB(0, 0, 0);         // Default button text color
	COLORREF CLR_HighlightTextColor = RGB(0, 0, 0);       // Highlight button text color
	HWND* hWnd_CurrentSelectedButton = nullptr;           // Current selected button window handle (the subclass use this variable to check which button is currently selected)
	HWND* hWnd_LeftButton = nullptr;                      // Left button window handle (the subclass use this variable to distinguish between left and right buttons)
public:
	BA_Radio2(HWND& hWnd_CurrentSelectedButton, HWND& hWnd_LeftButton) 
	{
		this->hWnd_CurrentSelectedButton = &hWnd_CurrentSelectedButton;
		this->hWnd_LeftButton = &hWnd_LeftButton;
	}
	~BA_Radio2() 
	{
		DeleteObject(hBrush_ButtonColor_Default);
		DeleteObject(hBrush_ButtonColor_Hover);
		DeleteObject(hBrush_ButtonColor_LBDown);
		DeleteObject(hBrush_ButtonBackgroundColor);
	}
public:
	void UpdateObjects(Gdiplus::Color Default, Gdiplus::Color Hover, Gdiplus::Color LBDown, Gdiplus::Color Background, Gdiplus::Color BorderNonFocus, Gdiplus::Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor);
public:
	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated);
	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H);
	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB);
};




/****************************************
*    BUTTON ANIMATION CLASS (RADIO3)    *
*****************************************/
// Radio button animation subclass, left & middle and right buttons.
// Each object have its own color objects and can only be used for a single button control set.
class BA_Radio3
{
private:
	bool IsReady = 0;                                     // Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 100;          // Hover animation duration
	unsigned short LBDownAnimationDuration = 300;         // Left-button down animation duration
	Gdiplus::Color CL_ButtonColor_Default;                // Default button color
	Gdiplus::Color CL_ButtonColor_Hover;                  // Hovering button color
	Gdiplus::Color CL_ButtonColor_LBDown;                 // Left-button down button color
	Gdiplus::Color CL_ButtonBackgroundColor;              // Button background color
	Gdiplus::Color CL_ButtonBorderColor_NonFocus;         // Button border color (Non-Focus)
	Gdiplus::Color CL_ButtonBorderColor_OnFocus;          // Button border color (On-Focus)
	HPEN hPen_ButtonColor_Default = nullptr;              // Default button color
	HBRUSH hBrush_ButtonColor_Default = nullptr;          // Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;            // Hovering button color
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;           // Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;        // Button background color
	HFONT* hFont_ButtonFont = nullptr;                    // Button text font
	COLORREF CLR_DefaultTextColor = RGB(0, 0, 0);         // Default button text color
	COLORREF CLR_HighlightTextColor = RGB(0, 0, 0);       // Highlight button text color
	HWND* hWnd_CurrentSelectedButton = nullptr;           // Current selected button window handle (the subclass use this variable to check which button is currently selected)
	HWND* hWnd_LeftButton = nullptr;                      // Left button window handle (the subclass use this variable to distinguish between left, middle and right buttons)
	HWND* hWnd_MiddleButton = nullptr;                    // Middle button window handle (the subclass use this variable to distinguish between left, middle and right buttons)
public:
	BA_Radio3(HWND& hWnd_CurrentSelectedButton, HWND& hWnd_LeftButton, HWND& hWnd_MiddleButton) 
	{
		this->hWnd_CurrentSelectedButton = &hWnd_CurrentSelectedButton;
		this->hWnd_LeftButton = &hWnd_LeftButton;
		this->hWnd_MiddleButton = &hWnd_MiddleButton;
	}
	~BA_Radio3()
	{
		DeleteObject(hPen_ButtonColor_Default);
		DeleteObject(hBrush_ButtonColor_Default);
		DeleteObject(hBrush_ButtonColor_Hover);
		DeleteObject(hBrush_ButtonColor_LBDown);
		DeleteObject(hBrush_ButtonBackgroundColor);
	}
public:
	void UpdateObjects(Gdiplus::Color Default, Gdiplus::Color Hover, Gdiplus::Color LBDown, Gdiplus::Color Background, Gdiplus::Color BorderNonFocus, Gdiplus::Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor);
public:
	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated);
	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state);
	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H);
	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB);
};




/*************************************************************
*    CUSTOM DRAW COMBOBOX SUBCLASS CLASS (DROP-DOWN LIST)    *
**************************************************************/
// Custom draw drop-down list combobox subclass.
// Each object have its own color objects and can be use for multiple combobox controls.
class CBDL_CustomDraw
{
private:
	bool IsReady = 0;                                           // Track whether the required objects is initialized
public:
	Gdiplus::Color CL_ComboboxColor_Default;                    // Default combobox color
	Gdiplus::Color CL_ComboboxBackgroundColor;                  // Combobox background color
	Gdiplus::Color CL_ComboboxDropdownListColor_Background;     // Combobox drop-down list background color
	Gdiplus::Color CL_ComboboxDropdownListColor_Selected;       // Combobox drop-down list selected item background color
	Gdiplus::Color CL_ComboboxDropdownListColor_Border;         // Combobox drop-down list border color

	COLORREF CLR_ComboboxColor_Default = NULL;                  // Default combobox color                                   (COLORREF)
	COLORREF CLR_ComboboxBackgroundColor = NULL;                // Combobox background color                                (COLORREF)
	COLORREF CLR_ComboboxDropdownListColor_Background = NULL;   // Combobox drop-down list background color                 (COLORREF)
	COLORREF CLR_ComboboxDropdownListColor_Selected = NULL;     // Combobox drop-down list selected item background color   (COLORREF)
	COLORREF CLR_ComboboxDropdownListColor_Border = NULL;       // Combobox drop-down list border color                     (COLORREF)

	HBRUSH HBR_ComboboxColor_Default = nullptr;                 // Default combobox color                                   (HBRUSH)
	HBRUSH HBR_ComboboxBackgroundColor = nullptr;               // Combobox background color                                (HBRUSH)
	HBRUSH HBR_ComboboxDropdownListColor_Background = nullptr;  // Combobox drop-down list background color                 (HBRUSH)
	HBRUSH HBR_ComboboxDropdownListColor_Selected = nullptr;    // Combobox drop-down list selected item background color   (HBRUSH)
	HBRUSH HBR_ComboboxDropdownListColor_Border = nullptr;      // Combobox drop-down list border color                     (HBRUSH)

	HFONT* HFO_ComboboxFont = nullptr;                          // Combobox text font                                       (POINTER)
	COLORREF CLR_DefaultTextColor = RGB(0, 0, 0);               // Default combobox text color
	COLORREF CLR_InactiveTextColor = RGB(0, 0, 0);              // Inactive combobox text color
public:
	CBDL_CustomDraw() {}
	~CBDL_CustomDraw() 
	{
		// Release objects
		DeleteObject(this->HBR_ComboboxColor_Default);
		DeleteObject(this->HBR_ComboboxBackgroundColor);
		DeleteObject(this->HBR_ComboboxDropdownListColor_Background);
		DeleteObject(this->HBR_ComboboxDropdownListColor_Selected);
		DeleteObject(this->HBR_ComboboxDropdownListColor_Border);
	}
public:
	void UpdateObjects(Gdiplus::Color ComboboxColor, Gdiplus::Color ComboboxBackgroundColor, Gdiplus::Color CBDL_Background, Gdiplus::Color CBDL_Selected, Gdiplus::Color CBDL_Border, HFONT& Font, COLORREF DefaultTextColor, COLORREF InactiveTextColor);
	void SetComboboxSubclass(HWND hWnd);
	void SetComboboxDDLSubclass(HWND hWnd);

public:
	void ComboboxCtrl_OnPaint(HWND hWnd);
	void ComboboxCtrl_DDL_OnErase(HWND hWnd, HDC hdc);
	void ComboboxCtrl_DDL_OnNCPaint(HWND hWnd);
public:
	static LRESULT CALLBACK ComboboxCtrl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK ComboboxCtrl_DDL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
};