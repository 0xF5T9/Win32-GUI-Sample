#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <Dwmapi.h>

class BA_CaptionBar
{
private:
	bool IsReady = 0;									// Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 150;			// Hover animation duration
	unsigned short LBDownAnimationDuration = 150;			// Left-button down animation duration
	HBRUSH hBrush_ButtonColor_Default = nullptr;			// Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;			// Hovering button color
	HBRUSH hBrush_ButtonColor_HoverClose = nullptr;		// Hovering button color (Close)
	HBRUSH hBrush_ButtonColor_HoverMinimize = nullptr;		// Hovering button color (Minimize)
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;			// Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;		// Button background color
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

class BA_Standard
{
private:
	bool IsReady = 0;									// Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 150;			// Hover animation duration
	unsigned short LBDownAnimationDuration = 150;			// Left-button down animation duration
	Gdiplus::Color CL_ButtonColor_Default;					// Default button color
	Gdiplus::Color CL_ButtonColor_Hover;					// Hovering button color
	Gdiplus::Color CL_ButtonColor_LBDown;					// Left-button down button color
	Gdiplus::Color CL_ButtonBackgroundColor;				// Button background color
	Gdiplus::Color CL_ButtonBorderColor_NonFocus;			// Button border color (Non-Focus)
	Gdiplus::Color CL_ButtonBorderColor_OnFocus;			// Button border color (On-Focus)
	HBRUSH hBrush_ButtonColor_Default = nullptr;			// Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;			// Hovering button color
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;			// Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;		// Button background color
	HFONT* hFont_ButtonFont = nullptr;					// Button text font
	COLORREF CLR_DefaultTextColor = RGB(0, 0, 0);			// Default button text color
	COLORREF CLR_HighlightTextColor = RGB (0, 0, 0);		// Highlight button text color
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

class BA_Radio2
{
private:
	bool IsReady = 0;									// Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 100;			// Hover animation duration
	unsigned short LBDownAnimationDuration = 300;			// Left-button down animation duration
	Gdiplus::Color CL_ButtonColor_Default;					// Default button color
	Gdiplus::Color CL_ButtonColor_Hover;					// Hovering button color
	Gdiplus::Color CL_ButtonColor_LBDown;					// Left-button down button color
	Gdiplus::Color CL_ButtonBackgroundColor;				// Button background color
	Gdiplus::Color CL_ButtonBorderColor_NonFocus;			// Button border color (Non-Focus)
	Gdiplus::Color CL_ButtonBorderColor_OnFocus;			// Button border color (On-Focus)
	HBRUSH hBrush_ButtonColor_Default = nullptr;			// Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;			// Hovering button color
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;			// Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;		// Button background color
	HFONT* hFont_ButtonFont = nullptr;					// Button text font
	COLORREF CLR_DefaultTextColor = RGB(0, 0, 0);			// Default button text color
	COLORREF CLR_HighlightTextColor = RGB(0, 0, 0);			// Highlight button text color
public:
	BA_Radio2() {}
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

class BA_Radio3
{
private:
	bool IsReady = 0;									// Track whether the required objects is initialized
	unsigned short HoverAnimationDuration = 100;			// Hover animation duration
	unsigned short LBDownAnimationDuration = 300;			// Left-button down animation duration
	Gdiplus::Color CL_ButtonColor_Default;					// Default button color
	Gdiplus::Color CL_ButtonColor_Hover;					// Hovering button color
	Gdiplus::Color CL_ButtonColor_LBDown;					// Left-button down button color
	Gdiplus::Color CL_ButtonBackgroundColor;				// Button background color
	Gdiplus::Color CL_ButtonBorderColor_NonFocus;			// Button border color (Non-Focus)
	Gdiplus::Color CL_ButtonBorderColor_OnFocus;			// Button border color (On-Focus)
	HPEN hPen_ButtonColor_Default = nullptr;				// Default button color
	HBRUSH hBrush_ButtonColor_Default = nullptr;			// Default button color
	HBRUSH hBrush_ButtonColor_Hover = nullptr;			// Hovering button color
	HBRUSH hBrush_ButtonColor_LBDown = nullptr;			// Left-button down button color
	HBRUSH hBrush_ButtonBackgroundColor = nullptr;		// Button background color
	HFONT* hFont_ButtonFont = nullptr;					// Button text font
	COLORREF CLR_DefaultTextColor = RGB(0, 0, 0);			// Default button text color
	COLORREF CLR_HighlightTextColor = RGB(0, 0, 0);			// Highlight button text color
public:
	BA_Radio3() {}
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