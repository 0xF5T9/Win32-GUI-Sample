/*
*  File: subclasses.cpp
*  Subclasses class definitions
*/

#include "./Headers/c_resources.h"
#include <iostream>
#include <string>
#include <locale>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <map>
#include <set>
#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <Dwmapi.h>
#include "./Headers./obj_manager.h"
#include "./Headers/subclasses.h"
#include "./Headers/global.h"

using Gdiplus::Graphics, Gdiplus::GraphicsPath, Gdiplus::Rect, Gdiplus::Color, Gdiplus::Brush, Gdiplus::SolidBrush, Gdiplus::SmoothingMode,
Gdiplus::Pen, Gdiplus::PenAlignmentCenter, Gdiplus::Unit, Gdiplus::UnitPixel;

// FORWARD DECLARATIONS
namespace nSol
{
	void RemoveWindowStyle(HWND& hWnd, LONG Style);
	void GetRoundRectPath(GraphicsPath* pPath, Rect r, int dia);
	void DrawRoundRect(Graphics* pGraphics, Rect r, Color color, int radius, int width);
	void FillRoundRect(Graphics* pGraphics, Brush* pBrush, Rect r, Color border, int radius);
	void cCreateFont(HFONT* hFontPtr, std::wstring fName, int fSize, int fWeight = FW_DONTCARE, int fQuality = DEFAULT_QUALITY);
}

/*********************************************************
*    BUTTON ANIMATION CLASS DEFINITIONS (CAPTION BAR)    *
**********************************************************/
void BA_CaptionBar::ClearAnimationMap()
{
	this->AnimationMap.clear();
}
void BA_CaptionBar::InsertAnimationMap(HWND& hWnd, HICON& hIcon_Default, HICON& hIcon_Hover, HICON& hIcon_Inactive)
{
	this->AnimationMap.insert(std::make_pair(std::make_pair(&hWnd, &hIcon_Default), std::make_pair(&hIcon_Hover, &hIcon_Inactive)));
}
void BA_CaptionBar::UpdateObjects(COLORREF Default, COLORREF Hover, COLORREF Hover_Close, COLORREF Hover_Minimize, COLORREF LBDown, COLORREF Background)
{
	if (this->IsReady)
	{	
		// Release previously objects
		DeleteObject(this->hBrush_ButtonColor_Default);
		DeleteObject(this->hBrush_ButtonColor_Hover);
		DeleteObject(this->hBrush_ButtonColor_HoverClose);
		DeleteObject(this->hBrush_ButtonColor_HoverMinimize);
		DeleteObject(this->hBrush_ButtonColor_LBDown);
		DeleteObject(this->hBrush_ButtonBackgroundColor);
	}

	// Create new objects
	this->hBrush_ButtonColor_Default = CreateSolidBrush(Default);
	this->hBrush_ButtonColor_Hover = CreateSolidBrush(Hover);
	this->hBrush_ButtonColor_HoverClose = CreateSolidBrush(Hover_Close);
	this->hBrush_ButtonColor_HoverMinimize = CreateSolidBrush(Hover_Minimize);
	this->hBrush_ButtonColor_LBDown = CreateSolidBrush(LBDown);
	this->hBrush_ButtonBackgroundColor = CreateSolidBrush(Background);

	this->IsReady = true;
}
void BA_CaptionBar::SetSubclass(HWND hWnd)
{
	if (!this->IsReady)
	{
		MessageBoxW(NULL, L"Error occurred!\n(The required objects are not initialized)\n\nINFO: 'BA_CaptionBar' CLASS, 'SetSubclass()' FUNC", L"", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Pass this object pointer via 'dwRefData' while subclassing so the callback function can access non-static members using the pointer
	// READMORE: https://stackoverflow.com/questions/66784011/c-winapi-wrapping-up-a-callback-of-subclass-in-a-class
	SetWindowSubclass(hWnd, &SC_BA_CaptionBar, NULL, reinterpret_cast<DWORD_PTR>(this));
}
void BA_CaptionBar::StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated)
{
	nState = !cState;
	frames_Invalidated = 0;
	InvalidateRect(hWnd, NULL, TRUE);
	SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
}
void BA_CaptionBar::Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	static HICON hIcon, hIcon_H, hIcon_NF;
	for (const auto& x : this->AnimationMap)
	{
		if (*x.first.first == hWnd)
		{
			hIcon = *x.first.second;
			hIcon_H = *x.second.first;
			hIcon_NF = *x.second.second;
			if (GetActiveWindow() != MAIN_HWND) hIcon = hIcon_NF;
			break;
		}
	}
	HBRUSH hBrush_Hover = this->hBrush_ButtonColor_Hover;
	if (hWnd == BTN_Close) hBrush_Hover = this->hBrush_ButtonColor_HoverClose;
	else if (hWnd == BTN_Minimize) hBrush_Hover = this->hBrush_ButtonColor_HoverMinimize;

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
	if (state)
	{
		DrawIconEx(hdc, 19, 8, hIcon, 20, 20, NULL, NULL, DI_NORMAL);
	}
	else if (!state && hWnd == cHWND)
	{
		FillRect(hdc, &rc, hBrush_Hover);
		DrawIconEx(hdc, 19, 8, hIcon_H, 20, 20, NULL, NULL, DI_NORMAL);
	}
	else
	{
		DrawIconEx(hdc, 19, 8, hIcon, 20, 20, NULL, NULL, DI_NORMAL);
	}
}
void BA_CaptionBar::Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	static HICON hIcon, hIcon_H, hIcon_NF;
	for (const auto& x : this->AnimationMap)
	{
		if (*x.first.first == hWnd)
		{
			hIcon = *x.first.second;
			hIcon_H = *x.second.first;
			hIcon_NF = *x.second.second;
			if (GetActiveWindow() != MAIN_HWND) hIcon = hIcon_NF;
			break;
		}
	}
	HBRUSH hBrush_Hover = this->hBrush_ButtonColor_Hover;
	if (hWnd == BTN_Close) hBrush_Hover = this->hBrush_ButtonColor_HoverClose;
	else if (hWnd == BTN_Minimize) hBrush_Hover = this->hBrush_ButtonColor_HoverMinimize;

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
	if (state)
	{
		FillRect(hdc, &rc, hBrush_Hover);
		DrawIconEx(hdc, 19, 8, hIcon_H, 20, 20, NULL, NULL, DI_NORMAL);
	}
	else if (!state && hWnd == cHWND)
	{
		FillRect(hdc, &rc, hBrush_Hover);
		DrawIconEx(hdc, 19, 8, hIcon_H, 20, 20, NULL, NULL, DI_NORMAL);
	}
	else
	{
		DrawIconEx(hdc, 19, 8, hIcon, 20, 20, NULL, NULL, DI_NORMAL);
	}
}
void BA_CaptionBar::OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR; // Alternative: BPAS_NONE

			animParams.dwDuration = (cState_H != nState_H ? this->HoverAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_Hover(hWnd, cHWND, hdcFrom, cState_H);
				}
				if (hdcTo)
				{
					Paint_Hover(hWnd, cHWND, hdcTo, nState_H);
				}

				cState_H = nState_H;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_Hover(hWnd, cHWND, hdc, cState_H);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
void BA_CaptionBar::OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR;

			animParams.dwDuration = (cState_LB != nState_LB ? this->LBDownAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_LBDown(hWnd, cHWND, hdcFrom, cState_LB);
				}
				if (hdcTo)
				{
					Paint_LBDown(hWnd, cHWND, hdcTo, nState_LB);
				}

				cState_LB = nState_LB;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_LBDown(hWnd, cHWND, hdc, cState_LB);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
// * SUBCLASS CALLBACK PROCEDURE
LRESULT CALLBACK BA_CaptionBar::SC_BA_CaptionBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	BA_CaptionBar* pThis = reinterpret_cast<BA_CaptionBar*>(dwRefData); // Extract the poiner from dwRefData and use it to access non-static members

	static bool cState_H = 1;
	static bool nState_H = 1;
	static bool cState_LB = 1;
	static bool nState_LB = 1;
	static bool isHover = 0;
	static bool isLBDown = 0;
	static unsigned short frames_Invalidated = 0;
	static HWND cHWND;

	switch (uMsg)
	{
		case WM_NCDESTROY:
		{
			RemoveWindowSubclass(hWnd, &SC_BA_CaptionBar, uIdSubclass);
			return 0;
		}

		case WM_PAINT:
		{
			if (!isLBDown) pThis->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else pThis->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
			return 0;
		}

		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_TIMER:
		{
			static std::set<HWND*> TimerStack;
			switch (wParam)
			{
			case 1:
			{
				InvalidateRect(hWnd, NULL, TRUE);
				if (frames_Invalidated == 0) TimerStack.insert(&hWnd);

				frames_Invalidated++;
				if (frames_Invalidated == 60)
				{
					for (auto& x : TimerStack)
					{
						KillTimer(*x, 1);
						TimerStack.erase(x);
					}
					frames_Invalidated = 0;
				}

				return (LRESULT)0;
			}
			default:
				break;
			}

			break;
		}

		case WM_LBUTTONDOWN:
		{
			nState_LB = 1;
			cState_LB = 1;
			isLBDown = 1;
			BufferedPaintStopAllAnimations(hWnd);
			pThis->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

			SendMessageW(MAIN_HWND, WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL); // Forward WM_COMMAND messages to main window procedure
			return 0;
		}

		case WM_LBUTTONUP:
		{
			if (isLBDown)
			{
				cState_LB = 0;
				nState_LB = 1;
				cState_H = 0;
				nState_H = 1;
				BufferedPaintStopAllAnimations(hWnd);
				pThis->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);
				return 0;
			}

			break;
		}

		case WM_MOUSELEAVE:
		{
			cState_LB = 0;
			nState_LB = 1;
			cState_H = 0;
			nState_H = 1;
			isLBDown = 0;
			isHover = 0;
			pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

			return 0;
		}

		case WM_MOUSEHOVER:
			break;

		case WM_MOUSEMOVE:
		{
			if (!isHover)
			{
				cHWND = hWnd;
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}




/******************************************************
*    BUTTON ANIMATION CLASS DEFINITIONS (STANDARD)    *
*******************************************************/
void BA_Standard::UpdateObjects(Color Default, Color Hover, Color LBDown, Color Background, Color BorderNonFocus, Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor)
{
	if (this->IsReady)
	{
		// Release previously objects
		DeleteObject(this->HBR_ButtonColor_Default);
		DeleteObject(this->HBR_ButtonColor_Hover);
		DeleteObject(this->HBR_ButtonColor_LBDown);
		DeleteObject(this->HBR_ButtonBackgroundColor);
	}

	// Create new objects
	this->CL_ButtonColor_Default = Default;
	this->CL_ButtonColor_Hover = Hover;
	this->CL_ButtonColor_LBDown = LBDown;
	this->CL_ButtonBackgroundColor = Background;
	this->CL_ButtonBorderColor_NonFocus = BorderNonFocus;
	this->CL_ButtonBorderColor_OnFocus = BorderOnFocus;
	this->HBR_ButtonColor_Default = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->HBR_ButtonColor_Hover = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Hover.GetRed(), (int)this->CL_ButtonColor_Hover.GetGreen(), (int)this->CL_ButtonColor_Hover.GetBlue()));
	this->HBR_ButtonColor_LBDown = CreateSolidBrush(RGB((int)this->CL_ButtonColor_LBDown.GetRed(), (int)this->CL_ButtonColor_LBDown.GetGreen(), (int)this->CL_ButtonColor_LBDown.GetBlue()));
	this->HBR_ButtonBackgroundColor = CreateSolidBrush(RGB((int)this->CL_ButtonBackgroundColor.GetRed(), (int)this->CL_ButtonBackgroundColor.GetGreen(), (int)this->CL_ButtonBackgroundColor.GetBlue()));
	this->CLR_DefaultTextColor = DefaultTextColor;
	this->CLR_HighlightTextColor = HighlightTextColor;
	this->HFO_ButtonFont = &Font;

	this->IsReady = true;
}
void BA_Standard::SetSubclass(HWND hWnd)
{
	if (!this->IsReady)
	{
		MessageBoxW(NULL, L"Error occurred!\n(The required objects are not initialized)\n\nINFO: 'BA_Standard' CLASS, 'SetSubclass()' FUNC", L"", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Pass this object pointer via 'dwRefData' while subclassing so the callback function can access non-static members using the pointer
	// READMORE: https://stackoverflow.com/questions/66784011/c-winapi-wrapping-up-a-callback-of-subclass-in-a-class
	SetWindowSubclass(hWnd, &SC_BA_Standard, NULL, reinterpret_cast<DWORD_PTR>(this));
}
void BA_Standard::StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated)
{
	nState = !cState;
	frames_Invalidated = 0;
	InvalidateRect(hWnd, NULL, TRUE);
	SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
}
void BA_Standard::Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

	RECT rc;
	GetClientRect(hWnd, &rc);
	Gdiplus::Rect grc(rc.top, rc.left, rc.right, rc.bottom);
	WCHAR TextBuffer[15];
	GetWindowTextW(hWnd, TextBuffer, 15);
	SetTextColor(hdc, this->CLR_DefaultTextColor);
	SetBkMode(hdc, TRANSPARENT);

	SolidBrush SB_Background = this->CL_ButtonColor_Default;
	SolidBrush SB_Background_H = this->CL_ButtonColor_Hover;
	SelectObject(hdc, (HFONT)*this->HFO_ButtonFont);

	FillRect(hdc, &rc, this->HBR_ButtonBackgroundColor);
	if (state)
	{
		if (GetFocus() != hWnd)
		{
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonBorderColor_NonFocus, 4);
		}
		else
		{
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonBorderColor_OnFocus, 4);
		}
		DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (!state && hWnd == cHWND)
	{
		if (GetFocus() != hWnd)
		{
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonBorderColor_NonFocus, 4);
		}
		else
		{
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonBorderColor_OnFocus, 4); // *
		}
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (GetFocus() != hWnd)
		{
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonBorderColor_NonFocus, 4);
		}
		else
		{
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonBorderColor_OnFocus, 4);
		}
		DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}
void BA_Standard::Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

	RECT rc;
	GetClientRect(hWnd, &rc);
	Gdiplus::Rect grc(rc.top, rc.left, rc.right, rc.bottom);
	WCHAR TextBuffer[15];
	GetWindowTextW(hWnd, TextBuffer, 15);
	SetTextColor(hdc, this->CLR_DefaultTextColor);
	SetBkMode(hdc, TRANSPARENT);

	SolidBrush SB_Background = this->CL_ButtonColor_Default;
	SolidBrush SB_Background_H = this->CL_ButtonColor_Hover;
	SolidBrush SB_Background_F = this->CL_ButtonColor_LBDown;
	SelectObject(hdc, (HFONT)*this->HFO_ButtonFont);

	FillRect(hdc, &rc, this->HBR_ButtonBackgroundColor);
	if (state && hWnd == cHWND)
	{
		if (GetFocus() != hWnd)
		{
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonBorderColor_NonFocus, 4);
		}
		else
		{
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonBorderColor_OnFocus, 4);
		}
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (!state && hWnd == cHWND)
	{
		if (GetFocus() != hWnd)
		{
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonBorderColor_NonFocus, 4);
		}
		else
		{
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonBorderColor_OnFocus, 4);
		}
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (GetFocus() != hWnd)
		{
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonBorderColor_NonFocus, 4);
		}
		else
		{
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonBorderColor_OnFocus, 4);
		}
		DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}
void BA_Standard::OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR; // Alternative: BPAS_NONE

			animParams.dwDuration = (cState_H != nState_H ? this->HoverAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_Hover(hWnd, cHWND, hdcFrom, cState_H);
				}
				if (hdcTo)
				{
					Paint_Hover(hWnd, cHWND, hdcTo, nState_H);
				}

				cState_H = nState_H;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_Hover(hWnd, cHWND, hdc, cState_H);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
void BA_Standard::OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR;

			animParams.dwDuration = (cState_LB != nState_LB ? this->LBDownAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_LBDown(hWnd, cHWND, hdcFrom, cState_LB);
				}
				if (hdcTo)
				{
					Paint_LBDown(hWnd, cHWND, hdcTo, nState_LB);
				}

				cState_LB = nState_LB;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_LBDown(hWnd, cHWND, hdc, cState_LB);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
// * SUBCLASS CALLBACK PROCEDURE
LRESULT CALLBACK BA_Standard::SC_BA_Standard(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	BA_Standard* pThis = reinterpret_cast<BA_Standard*>(dwRefData); // Extract the poiner from dwRefData and use it to access non-static members

	static bool cState_H = 1;
	static bool nState_H = 1;
	static bool cState_LB = 1;
	static bool nState_LB = 1;
	static bool isHover = 0;
	static bool isLBDown = 0;
	static unsigned short frames_Invalidated = 0;
	static HWND cHWND;

	if (GetFocus() == hWnd && ktEnterKey)
	{
		SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL);
	}
	else if (GetFocus() == hWnd && GetKeyState(VK_TAB) & 0x8000)
	{
		InvalidateRect(hWnd, NULL, FALSE);
	}

	switch (uMsg)
	{
	case WM_NCDESTROY:
	{
		RemoveWindowSubclass(hWnd, &SC_BA_Standard, uIdSubclass);
		return (LRESULT)0;
	}

	case WM_ERASEBKGND:
		return (LRESULT)1;

	case WM_TIMER:
	{
		static std::set<HWND*> TimerStack;
		switch (wParam)
		{
		case 1:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			if (frames_Invalidated == 0) TimerStack.insert(&hWnd);

			frames_Invalidated++;
			if (frames_Invalidated == 60)
			{
				for (auto& x : TimerStack)
				{
					KillTimer(*x, 1);
					TimerStack.erase(x);
				}
				frames_Invalidated = 0;
			}

			return (LRESULT)0;
		}
		default:
			break;
		}

		break;
	}

	case WM_PAINT:
	{
		if (!isLBDown) pThis->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
		else pThis->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
		return (LRESULT)0;
	}

	case WM_LBUTTONDOWN:
	{
		nState_LB = 1;
		cState_LB = 1;
		isLBDown = 1;
		BufferedPaintStopAllAnimations(hWnd);
		pThis->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

		SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL); // Forward WM_COMMAND messages to main window procedure
		return (LRESULT)0;
	}

	case WM_LBUTTONUP:
	{
		if (isLBDown)
		{
			cState_LB = 0;
			nState_LB = 1;
			cState_H = 0;
			nState_H = 1;
			BufferedPaintStopAllAnimations(hWnd);
			pThis->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);
			return (LRESULT)0;
		}

		break;
	}

	case WM_MOUSELEAVE:
	{
		cState_LB = 0;
		nState_LB = 1;
		cState_H = 0;
		nState_H = 1;
		isLBDown = 0;
		isHover = 0;
		pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

		return (LRESULT)0;
	}

	case WM_MOUSEHOVER:
		break;

	case WM_MOUSEMOVE:
	{
		if (!isHover)
		{
			cHWND = hWnd;
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
			pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
			isHover = 1;

			return (LRESULT)0;
		}

		break;
	}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}




/****************************************************
*    BUTTON ANIMATION CLASS DEFINITIONS (RADIO2)    *
*****************************************************/
void BA_Radio2::UpdateObjects(Color Default, Color Hover, Color LBDown, Color Background, Color BorderNonFocus, Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor)
{
	if (this->IsReady)
	{
		// Release previously objects
		DeleteObject(this->HBR_ButtonColor_Default);
		DeleteObject(this->HBR_ButtonColor_Hover);
		DeleteObject(this->HBR_ButtonColor_LBDown);
		DeleteObject(this->HBR_ButtonBackgroundColor);
	}

	// Create new objects
	this->CL_ButtonColor_Default = Default;
	this->CL_ButtonColor_Hover = Hover;
	this->CL_ButtonColor_LBDown = LBDown;
	this->CL_ButtonBackgroundColor = Background;
	this->CL_ButtonBorderColor_NonFocus = BorderNonFocus;
	this->CL_ButtonBorderColor_OnFocus = BorderOnFocus;
	this->HBR_ButtonColor_Default = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->HBR_ButtonColor_Hover = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Hover.GetRed(), (int)this->CL_ButtonColor_Hover.GetGreen(), (int)this->CL_ButtonColor_Hover.GetBlue()));
	this->HBR_ButtonColor_LBDown = CreateSolidBrush(RGB((int)this->CL_ButtonColor_LBDown.GetRed(), (int)this->CL_ButtonColor_LBDown.GetGreen(), (int)this->CL_ButtonColor_LBDown.GetBlue()));
	this->HBR_ButtonBackgroundColor = CreateSolidBrush(RGB((int)this->CL_ButtonBackgroundColor.GetRed(), (int)this->CL_ButtonBackgroundColor.GetGreen(), (int)this->CL_ButtonBackgroundColor.GetBlue()));
	this->CLR_DefaultTextColor = DefaultTextColor;
	this->CLR_HighlightTextColor = HighlightTextColor;
	this->HFO_ButtonFont = &Font;

	this->IsReady = true;
}
void BA_Radio2::SetSubclass(HWND hWnd)
{
	if (!this->IsReady)
	{
		MessageBoxW(NULL, L"Error occurred!\n(The required objects are not initialized)\n\nINFO: 'BA_Radio2' CLASS, 'SetSubclass()' FUNC", L"", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Pass this object pointer via 'dwRefData' while subclassing so the callback function can access non-static members using the pointer
	// READMORE: https://stackoverflow.com/questions/66784011/c-winapi-wrapping-up-a-callback-of-subclass-in-a-class
	SetWindowSubclass(hWnd, &SC_BA_Radio2, NULL, reinterpret_cast<DWORD_PTR>(this));
}
void BA_Radio2::StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated)
{
	nState = !cState;
	frames_Invalidated = 0;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
}
void BA_Radio2::Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

	RECT rc;
	GetClientRect(hWnd, &rc);
	WCHAR TextBuffer[15];
	GetWindowTextW(hWnd, TextBuffer, 15);
	SetTextColor(hdc, CLR_DefaultTextColor);
	SetBkMode(hdc, TRANSPARENT);

	SolidBrush SB_Background = this->CL_ButtonColor_Default;
	SolidBrush SB_Background_H = this->CL_ButtonColor_Hover;
	SolidBrush SB_Background_F = this->CL_ButtonColor_LBDown;
	SelectObject(hdc, (HFONT)*this->HFO_ButtonFont);

	FillRect(hdc, &rc, this->HBR_ButtonBackgroundColor);
	if (state)
	{
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
	}
	else
	{
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
	}

	if (hWnd == *this->hWnd_CurrentSelectedButton)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
	}
}
void BA_Radio2::Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

	RECT rc;
	GetClientRect(hWnd, &rc);
	WCHAR TextBuffer[15];
	GetWindowTextW(hWnd, TextBuffer, 15);
	SetTextColor(hdc, CLR_DefaultTextColor);
	SetBkMode(hdc, TRANSPARENT);

	SolidBrush SB_Background = this->CL_ButtonColor_Default;
	SolidBrush SB_Background_H = this->CL_ButtonColor_Hover;
	SolidBrush SB_Background_F = this->CL_ButtonColor_LBDown;
	SelectObject(hdc, (HFONT)*this->HFO_ButtonFont);

	FillRect(hdc, &rc, this->HBR_ButtonBackgroundColor);
	if (state)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
	}
	else
	{
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
	}
}
void BA_Radio2::OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR; // Alternative: BPAS_NONE

			animParams.dwDuration = (cState_H != nState_H ? this->HoverAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_Hover(hWnd, cHWND, hdcFrom, cState_H);
				}
				if (hdcTo)
				{
					Paint_Hover(hWnd, cHWND, hdcTo, nState_H);
				}

				cState_H = nState_H;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_Hover(hWnd, cHWND, hdc, cState_H);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
void BA_Radio2::OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR;

			animParams.dwDuration = (cState_LB != nState_LB ? this->LBDownAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_LBDown(hWnd, cHWND, hdcFrom, cState_LB);
				}
				if (hdcTo)
				{
					Paint_LBDown(hWnd, cHWND, hdcTo, nState_LB);
				}

				cState_LB = nState_LB;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_LBDown(hWnd, cHWND, hdc, cState_LB);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
// * SUBCLASS CALLBACK PROCEDURE
LRESULT CALLBACK BA_Radio2::SC_BA_Radio2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	BA_Radio2* pThis = reinterpret_cast<BA_Radio2*>(dwRefData); // Extract the poiner from dwRefData and use it to access non-static members

	static bool cState_H = 1;
	static bool nState_H = 1;
	static bool cState_LB = 1;
	static bool nState_LB = 1;
	static bool isHover = 0;
	static bool isLBDown = 0;
	static unsigned short frames_Invalidated = 0;
	static HWND cHWND;

	switch (uMsg)
	{
		case WM_NCDESTROY:
		{
			RemoveWindowSubclass(hWnd, &SC_BA_Radio2, uIdSubclass);
			return 0;
		}

		case WM_PAINT:
		{
			if (!isLBDown) pThis->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else pThis->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
			return 0;
		}

		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_TIMER:
		{
			static std::set<HWND*> TimerStack;
			switch (wParam)
			{
			case 1:
			{
				InvalidateRect(hWnd, NULL, TRUE);
				if (frames_Invalidated == 0) TimerStack.insert(&hWnd);

				frames_Invalidated++;
				if (frames_Invalidated == 60)
				{
					for (auto& x : TimerStack)
					{
						KillTimer(*x, 1);
						TimerStack.erase(x);
					}
					frames_Invalidated = 0;
				}

				return (LRESULT)0;
			}
			default:
				break;
			}

			break;
		}

		case WM_LBUTTONDOWN:
		{
			nState_H = 1; // *
			cState_H = 0; // * Solve invalid hover state? to be tested
			nState_LB = 1;
			cState_LB = 1;
			isLBDown = 1;
			BufferedPaintStopAllAnimations(hWnd);
			pThis->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

			SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL); // Forward WM_COMMAND messages to main window procedure
			return 0;
		}

		case WM_MOUSELEAVE:
		{
			isLBDown = 0;
			isHover = 0;
			pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

			return 0;
		}

		case WM_MOUSEHOVER:
			break;

		case WM_MOUSEMOVE:
		{
			if (!isHover)
			{
				cHWND = hWnd;
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}




/****************************************************
*    BUTTON ANIMATION CLASS DEFINITIONS (RADIO3)    *
*****************************************************/
void BA_Radio3::UpdateObjects(Color Default, Color Hover, Color LBDown, Color Background, Color BorderNonFocus, Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor)
{
	if (this->IsReady)
	{
		// Release previously objects
		DeleteObject(this->HPEN_ButtonColor_Default);
		DeleteObject(this->HBR_ButtonColor_Default);
		DeleteObject(this->HBR_ButtonColor_Hover);
		DeleteObject(this->HBR_ButtonColor_LBDown);
		DeleteObject(this->HBR_ButtonBackgroundColor);
	}

	// Create new objects
	this->CL_ButtonColor_Default = Default;
	this->CL_ButtonColor_Hover = Hover;
	this->CL_ButtonColor_LBDown = LBDown;
	this->CL_ButtonBackgroundColor = Background;
	this->CL_ButtonBorderColor_NonFocus = BorderNonFocus;
	this->CL_ButtonBorderColor_OnFocus = BorderOnFocus;
	this->HPEN_ButtonColor_Default = CreatePen(PS_NULL, NULL, RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->HBR_ButtonColor_Default = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->HBR_ButtonColor_Hover = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Hover.GetRed(), (int)this->CL_ButtonColor_Hover.GetGreen(), (int)this->CL_ButtonColor_Hover.GetBlue()));
	this->HBR_ButtonColor_LBDown = CreateSolidBrush(RGB((int)this->CL_ButtonColor_LBDown.GetRed(), (int)this->CL_ButtonColor_LBDown.GetGreen(), (int)this->CL_ButtonColor_LBDown.GetBlue()));
	this->HBR_ButtonBackgroundColor = CreateSolidBrush(RGB((int)this->CL_ButtonBackgroundColor.GetRed(), (int)this->CL_ButtonBackgroundColor.GetGreen(), (int)this->CL_ButtonBackgroundColor.GetBlue()));
	this->CLR_DefaultTextColor = DefaultTextColor;
	this->CLR_HighlightTextColor = HighlightTextColor;
	this->HFO_ButtonFont = &Font;

	this->IsReady = true;
}
void BA_Radio3::SetSubclass(HWND hWnd)
{
	if (!this->IsReady)
	{
		MessageBoxW(NULL, L"Error occurred!\n(The required objects are not initialized)\n\nINFO: 'BA_Radio3' CLASS, 'SetSubclass()' FUNC", L"", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Pass this object pointer via 'dwRefData' while subclassing so the callback function can access non-static members using the pointer
	// READMORE: https://stackoverflow.com/questions/66784011/c-winapi-wrapping-up-a-callback-of-subclass-in-a-class
	SetWindowSubclass(hWnd, &SC_BA_Radio3, NULL, reinterpret_cast<DWORD_PTR>(this));
}
void BA_Radio3::StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated)
{
	nState = !cState;
	frames_Invalidated = 0;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
}
void BA_Radio3::Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

	RECT rc;
	GetClientRect(hWnd, &rc);
	WCHAR TextBuffer[15];
	GetWindowTextW(hWnd, TextBuffer, 15);
	SetTextColor(hdc, this->CLR_DefaultTextColor);
	SetBkMode(hdc, TRANSPARENT);

	SolidBrush SB_Background = this->CL_ButtonColor_Default;
	SolidBrush SB_Background_H = this->CL_ButtonColor_Hover;
	SolidBrush SB_Background_F = this->CL_ButtonColor_LBDown;
	SelectObject(hdc, (HFONT)*this->HFO_ButtonFont);

	FillRect(hdc, &rc, this->HBR_ButtonBackgroundColor);
	if (state)
	{
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
		else if (hWnd == *this->hWnd_MiddleButton)
		{
			SelectObject(hdc, (HBRUSH)this->HBR_ButtonColor_Default);
			SelectObject(hdc, (HPEN)this->HPEN_ButtonColor_Default);
			RoundRect(hdc, rc.left + 1, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
		else if (hWnd == *this->hWnd_MiddleButton)
		{
			SelectObject(hdc, (HBRUSH)this->HBR_ButtonColor_Hover);
			SelectObject(hdc, (HPEN)this->HPEN_ButtonColor_Default);
			RoundRect(hdc, rc.left + 1, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
	}
	else
	{
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
		else if (hWnd == *this->hWnd_MiddleButton)
		{
			SelectObject(hdc, (HBRUSH)this->HBR_ButtonColor_Default);
			SelectObject(hdc, (HPEN)this->HPEN_ButtonColor_Default);
			RoundRect(hdc, rc.left + 1, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
	}

	if (hWnd == *this->hWnd_CurrentSelectedButton)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
		else if (hWnd == *this->hWnd_MiddleButton)
		{
			SelectObject(hdc, (HBRUSH)this->HBR_ButtonColor_LBDown);
			SelectObject(hdc, (HPEN)this->HPEN_ButtonColor_Default);
			RoundRect(hdc, rc.left + 1, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
	}
}
void BA_Radio3::Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

	RECT rc;
	GetClientRect(hWnd, &rc);
	WCHAR TextBuffer[15];
	GetWindowTextW(hWnd, TextBuffer, 15);
	SetTextColor(hdc, this->CLR_DefaultTextColor);
	SetBkMode(hdc, TRANSPARENT);

	SolidBrush SB_Background = this->CL_ButtonColor_Default;
	SolidBrush SB_Background_H = this->CL_ButtonColor_Hover;
	SolidBrush SB_Background_F = this->CL_ButtonColor_LBDown;
	SelectObject(hdc, (HFONT)*this->HFO_ButtonFont);

	FillRect(hdc, &rc, this->HBR_ButtonBackgroundColor);
	if (state)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
		else if (hWnd == *this->hWnd_MiddleButton)
		{
			SelectObject(hdc, (HBRUSH)this->HBR_ButtonColor_Hover);
			SelectObject(hdc, (HPEN)this->HPEN_ButtonColor_Default);
			RoundRect(hdc, rc.left + 1, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Hover);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
		else if (hWnd == *this->hWnd_MiddleButton)
		{
			SelectObject(hdc, (HBRUSH)this->HBR_ButtonColor_LBDown);
			SelectObject(hdc, (HPEN)this->HPEN_ButtonColor_Default);
			RoundRect(hdc, rc.left + 1, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_LBDown);
		}
	}
	else
	{
		if (hWnd == *this->hWnd_LeftButton)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
		else if (hWnd == *this->hWnd_MiddleButton)
		{
			SelectObject(hdc, (HBRUSH)this->HBR_ButtonColor_Default);
			SelectObject(hdc, (HPEN)this->HPEN_ButtonColor_Default);
			RoundRect(hdc, rc.left + 1, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->HBR_ButtonColor_Default);
		}
	}
}
void BA_Radio3::OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR; // Alternative: BPAS_NONE

			animParams.dwDuration = (cState_H != nState_H ? this->HoverAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_Hover(hWnd, cHWND, hdcFrom, cState_H);
				}
				if (hdcTo)
				{
					Paint_Hover(hWnd, cHWND, hdcTo, nState_H);
				}

				cState_H = nState_H;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_Hover(hWnd, cHWND, hdc, cState_H);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
void BA_Radio3::OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (hdc)
	{
		if (!BufferedPaintRenderAnimation(hWnd, hdc))
		{
			BP_ANIMATIONPARAMS animParams;
			ZeroMemory(&animParams, sizeof(animParams));
			animParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
			animParams.style = BPAS_LINEAR;

			animParams.dwDuration = (cState_LB != nState_LB ? this->LBDownAnimationDuration : 0);

			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hdcFrom, hdcTo;
			HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hWnd, hdc, &rc,
				BPBF_COMPATIBLEBITMAP, NULL, &animParams, &hdcFrom, &hdcTo);

			if (hbpAnimation)
			{
				if (hdcFrom)
				{
					Paint_LBDown(hWnd, cHWND, hdcFrom, cState_LB);
				}
				if (hdcTo)
				{
					Paint_LBDown(hWnd, cHWND, hdcTo, nState_LB);
				}

				cState_LB = nState_LB;
				EndBufferedAnimation(hbpAnimation, TRUE);
			}
			else
			{
				Paint_LBDown(hWnd, cHWND, hdc, cState_LB);
			}
		}

		EndPaint(hWnd, &ps);
	}
}
// * SUBCLASS CALLBACK PROCEDURE
LRESULT CALLBACK BA_Radio3::SC_BA_Radio3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	BA_Radio3* pThis = reinterpret_cast<BA_Radio3*>(dwRefData); // Extract the poiner from dwRefData and use it to access non-static members

	static bool cState_H = 1;
	static bool nState_H = 1;
	static bool cState_LB = 1;
	static bool nState_LB = 1;
	static bool isHover = 0;
	static bool isLBDown = 0;
	static unsigned short frames_Invalidated = 0;
	static HWND cHWND;

	switch (uMsg)
	{
	case WM_NCDESTROY:
	{
		RemoveWindowSubclass(hWnd, &SC_BA_Radio3, uIdSubclass);
		return 0;
	}

	case WM_PAINT:
	{
		if (!isLBDown) pThis->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
		else pThis->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
		return 0;
	}

	case WM_ERASEBKGND:
		return (LRESULT)1;

	case WM_TIMER:
	{
		static std::set<HWND*> TimerStack;
		switch (wParam)
		{
		case 1:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			if (frames_Invalidated == 0) TimerStack.insert(&hWnd);

			frames_Invalidated++;
			if (frames_Invalidated == 60)
			{
				for (auto& x : TimerStack)
				{
					KillTimer(*x, 1);
					TimerStack.erase(x);
				}
				frames_Invalidated = 0;
			}

			return (LRESULT)0;
		}
		default:
			break;
		}

		break;
	}

	case WM_LBUTTONDOWN:
	{
		nState_H = 1;
		cState_H = 0;
		nState_LB = 1;
		cState_LB = 1;
		isLBDown = 1;
		BufferedPaintStopAllAnimations(hWnd);
		pThis->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

		SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL);
		return 0;
	}

	case WM_MOUSELEAVE:
	{
		isLBDown = 0;
		isHover = 0;
		pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

		return 0;
	}

	case WM_MOUSEHOVER:
		break;

	case WM_MOUSEMOVE:
	{
		if (!isHover)
		{
			cHWND = hWnd;
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
			pThis->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
			isHover = 1;

			return 0;
		}

		break;
	}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}




/*************************************************************************
*    CUSTOM DRAW COMBOBOX SUBCLASS CLASS DEFINITIONS (DROP-DOWN LIST)    *
**************************************************************************/
void CBDL_CustomDraw::UpdateObjects(Gdiplus::Color ComboboxColor, Gdiplus::Color ComboboxBackgroundColor, Gdiplus::Color CBDL_Background, Gdiplus::Color CBDL_Selected, Gdiplus::Color CBDL_Border, HFONT& Font, COLORREF DefaultTextColor, COLORREF InactiveTextColor)
{
	// Release previously objects if required
	if (this->IsReady)
	{
		DeleteObject(this->HBR_ComboboxColor_Default);
		DeleteObject(this->HBR_ComboboxBackgroundColor);
		DeleteObject(this->HBR_ComboboxDropdownListColor_Background);
		DeleteObject(this->HBR_ComboboxDropdownListColor_Selected);
		DeleteObject(this->HBR_ComboboxDropdownListColor_Border);
		this->IsReady = false;
	}

	// Create new objects
	{
		this->CL_ComboboxColor_Default = ComboboxColor;
		this->CLR_ComboboxColor_Default = RGB((int)this->CL_ComboboxColor_Default.GetRed(), (int)this->CL_ComboboxColor_Default.GetGreen(), (int)this->CL_ComboboxColor_Default.GetBlue());
		this->HBR_ComboboxColor_Default = CreateSolidBrush(this->CLR_ComboboxColor_Default);

		this->CL_ComboboxBackgroundColor = ComboboxBackgroundColor;
		this->CLR_ComboboxBackgroundColor = RGB((int)this->CL_ComboboxBackgroundColor.GetRed(), (int)this->CL_ComboboxBackgroundColor.GetGreen(), (int)this->CL_ComboboxBackgroundColor.GetBlue());
		this->HBR_ComboboxBackgroundColor = CreateSolidBrush(this->CLR_ComboboxBackgroundColor);

		this->CL_ComboboxDropdownListColor_Background = CBDL_Background;
		this->CLR_ComboboxDropdownListColor_Background = RGB((int)this->CL_ComboboxDropdownListColor_Background.GetRed(), (int)this->CL_ComboboxDropdownListColor_Background.GetGreen(), (int)this->CL_ComboboxDropdownListColor_Background.GetBlue());
		this->HBR_ComboboxDropdownListColor_Background = CreateSolidBrush(this->CLR_ComboboxDropdownListColor_Background);

		this->CL_ComboboxDropdownListColor_Selected = CBDL_Selected;
		this->CLR_ComboboxDropdownListColor_Selected = RGB((int)this->CL_ComboboxDropdownListColor_Selected.GetRed(), (int)this->CL_ComboboxDropdownListColor_Selected.GetGreen(), (int)this->CL_ComboboxDropdownListColor_Selected.GetBlue());
		this->HBR_ComboboxDropdownListColor_Selected = CreateSolidBrush(this->CLR_ComboboxDropdownListColor_Selected);

		this->CL_ComboboxDropdownListColor_Border = CBDL_Border;
		this->CLR_ComboboxDropdownListColor_Border = RGB((int)this->CL_ComboboxDropdownListColor_Border.GetRed(), (int)this->CL_ComboboxDropdownListColor_Border.GetGreen(), (int)this->CL_ComboboxDropdownListColor_Border.GetBlue());
		this->HBR_ComboboxDropdownListColor_Border = CreateSolidBrush(this->CLR_ComboboxDropdownListColor_Border);
		
		this->HFO_ComboboxFont = &Font;

		this->CLR_DefaultTextColor = DefaultTextColor;
		this->CLR_InactiveTextColor = InactiveTextColor;
	}

	// Check for invalid objects
	{
		if (!this->HBR_ComboboxColor_Default || !this->HBR_ComboboxBackgroundColor || !this->HBR_ComboboxDropdownListColor_Background ||
			!this->HBR_ComboboxDropdownListColor_Selected || !this->HBR_ComboboxDropdownListColor_Border)
			MessageBoxW(NULL, L"Error occurred!\n(Failed to create HBRUSH object(s))\n\nINFO: 'CBDL_CustomDraw' CLASS,'UpdateObjects()' FUNC", L"", MB_OK | MB_ICONERROR);

		if (!*(this->HFO_ComboboxFont))
			MessageBoxW(NULL, L"Error occurred!\n(Invalid font object)\n\nINFO: 'CBDL_CustomDraw' CLASS, 'UpdateObjects()' FUNC", L"", MB_OK | MB_ICONERROR);
	}

	this->IsReady = true;
}
void CBDL_CustomDraw::SetComboboxSubclass(HWND hWnd)
{
	if (!this->IsReady)
	{
		MessageBoxW(NULL, L"Error occurred!\n(The required objects are not initialized)\n\nINFO: 'CBDL_CustomDraw' CLASS, 'SetComboboxSubclass()' FUNC", L"", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Pass this object pointer via 'dwRefData' while subclassing so the callback function can access non-static members using the pointer
	// READMORE: https://stackoverflow.com/questions/66784011/c-winapi-wrapping-up-a-callback-of-subclass-in-a-class
	SetWindowSubclass(hWnd, &ComboboxCtrl, NULL, reinterpret_cast<DWORD_PTR>(this));

	// Get the drop-down list hwnd from COMBOBOXINFO struct
	COMBOBOXINFO cbi{};
	cbi.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(hWnd, &cbi);

	HWND DDL = cbi.hwndList;
	if (DDL)
	{
		SetComboboxDDLSubclass(DDL);                                                                        // Set subclass for drop-down list hwnd
		SetClassLongPtrW(DDL, GCLP_HBRBACKGROUND, (LONG_PTR)NULL);                                          // Set class background brush to none
		nSol::RemoveWindowStyle(DDL, CS_DBLCLKS);                                                           // Don't want double click message
		nSol::RemoveWindowStyle(DDL, CS_DROPSHADOW);                                                        // Don't want drop shadow effect
		SendMessageW(DDL, WM_SETFONT, (WPARAM)*this->HFO_ComboboxFont, TRUE);  // Set font for drop-down list hwnd
	}
}
void CBDL_CustomDraw::SetComboboxDDLSubclass(HWND hWnd)
{
	// Pass this object pointer via 'dwRefData' while subclassing so the callback function can access non-static members using the pointer
	// READMORE: https://stackoverflow.com/questions/66784011/c-winapi-wrapping-up-a-callback-of-subclass-in-a-class
	SetWindowSubclass(hWnd, &ComboboxCtrl_DDL, NULL, reinterpret_cast<DWORD_PTR>(this));
}
void CBDL_CustomDraw::ComboboxCtrl_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	// Create DCs & BMPs for double buffering (reduce flickering)
	HDC mem_hdc = CreateCompatibleDC(hdc);
	RECT rc; GetClientRect(hWnd, &rc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc,
		rc.right - rc.left,
		rc.bottom - rc.top);
	HBITMAP hBitmap_Old = (HBITMAP)SelectObject(mem_hdc, hBitmap);

	// Set GDI+ Smoothing Mode
	Graphics graphics(mem_hdc);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
	Gdiplus::Rect grc(rc.top, rc.left, rc.right, rc.bottom);

	// Begin painting to the memory DC
	SelectObject(mem_hdc, (HFONT)*this->HFO_ComboboxFont);
	SetBkColor(mem_hdc, this->CLR_ComboboxColor_Default);
	SetTextColor(mem_hdc, this->CLR_DefaultTextColor);

	FillRect(mem_hdc, &rc, this->HBR_ComboboxBackgroundColor);  // Background color
	SolidBrush SB_Background = this->CL_ComboboxColor_Default;  // Combobox color
	nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ComboboxColor_Default, 4);  // Draw round rect

	// Get and paint current selected combobox item text
	int current_index = (int)SendMessageW(hWnd, CB_GETCURSEL, NULL, NULL);
	if (current_index >= 0)
	{
		size_t text_length = (size_t)SendMessageW(hWnd, CB_GETLBTEXTLEN, current_index, 0);
		WCHAR* text_buffer = new WCHAR[text_length + 1];
		SendMessageW(hWnd, CB_GETLBTEXT, current_index, (LPARAM)text_buffer);
		DrawTextW(mem_hdc, text_buffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		delete[] text_buffer;
	}

	// Draw contents from memory DC to target DC
	BitBlt(hdc, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top, mem_hdc, 0, 0, SRCCOPY);

	// Release DCs
	SelectObject(mem_hdc, hBitmap_Old);
	DeleteObject(hBitmap);
	DeleteDC(mem_hdc);

	EndPaint(hWnd, &ps);
}
void CBDL_CustomDraw::ComboboxCtrl_DDL_OnErase(HWND hWnd, HDC hdc)
{
	// Get total items of the combobox to get the id of the last item ('last item id' = 'total items' - 1)
	// (To check if the last item is selected and make the "unpainted" 2-pixels match background color)
	COMBOBOXINFO cbi;
	cbi.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(hWnd, &cbi);
	int total_cbitems = (int)SendMessageW(cbi.hwndCombo, CB_GETCOUNT, NULL, NULL);
	int curselected_cbitem = (int)SendMessageW(cbi.hwndCombo, CB_GETCURSEL, NULL, NULL);

	RECT rc; GetClientRect(hWnd, &rc);
	FillRect(hdc, &rc, (curselected_cbitem == (total_cbitems - 1) ? this->HBR_ComboboxDropdownListColor_Selected : this->HBR_ComboboxDropdownListColor_Background));
}
void CBDL_CustomDraw::ComboboxCtrl_DDL_OnNCPaint(HWND hWnd)
{
	auto hdc = GetWindowDC(hWnd);                                     // The dropdown is non-client area
	RECT rc; GetClientRect(hWnd, &rc);
	FrameRect(hdc, &rc, this->HBR_ComboboxDropdownListColor_Border);  // Paint the borders

	
	// Get total items of the combobox to get the id of the last item ('last item id' = 'total items' - 1)
	// (To check if the last item is selected and make the "unpainted" 2-pixels match background color)
	COMBOBOXINFO cbi;
	cbi.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(hWnd, &cbi);

	int total_cbitems = (int)SendMessageW(cbi.hwndCombo, CB_GETCOUNT, NULL, NULL);
	int curselected_cbitem = (int)SendMessageW(cbi.hwndCombo, CB_GETCURSEL, NULL, NULL);
	rc.top = rc.bottom -= 1; rc.top -= 2; rc.left += 1; rc.right -= 1;
	FillRect(hdc, &rc, (curselected_cbitem == (total_cbitems - 1) ? this->HBR_ComboboxDropdownListColor_Selected : this->HBR_ComboboxDropdownListColor_Background));

	ReleaseDC(hWnd, hdc);  // Release DC
}
// * SUBCLASS CALLBACK PROCEDURE
LRESULT CALLBACK CBDL_CustomDraw::ComboboxCtrl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CBDL_CustomDraw* pThis = reinterpret_cast<CBDL_CustomDraw*>(dwRefData); // Extract the poiner from dwRefData and use it to access non-static members

	static bool IsDDLInvalidated = 0;

    switch (uMsg)
    {
        case WM_NCDESTROY:
        {
            RemoveWindowSubclass(hWnd, &ComboboxCtrl, uIdSubclass);
            return (LRESULT)0;
        }

        case WM_ERASEBKGND:
        {
            return (LRESULT)1;
        }

		// Override combobox paint messages
		case WM_PAINT:
		{
			DWORD style = (DWORD)GetWindowLongPtrW(hWnd, GWL_STYLE);
			if (!(style & CBS_DROPDOWNLIST))  // The combobox must have CBS_DROPDOWNLIST style
				break;

			pThis->ComboboxCtrl_OnPaint(hWnd);

			return (LRESULT)0;
		}

		case WM_NCPAINT:
		{
			return (LRESULT)0; // Always return zero to prevent unwanted non-client paints
		}

		// Extra invalidate upon opening the drop-down list
		case WM_MOUSEMOVE:
		{
			if (wParam == MK_LBUTTON && !IsDDLInvalidated)
			{
				COMBOBOXINFO cbi;
				cbi.cbSize = sizeof(COMBOBOXINFO);
				GetComboBoxInfo(hWnd, &cbi);
				InvalidateRect(cbi.hwndList, NULL, FALSE);

				IsDDLInvalidated = 1;
			}

			return (LRESULT)0;
		}

		case WM_MOUSELEAVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			IsDDLInvalidated = 0;
			break;
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK CBDL_CustomDraw::ComboboxCtrl_DDL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CBDL_CustomDraw* pThis = reinterpret_cast<CBDL_CustomDraw*>(dwRefData);

    switch (uMsg)
    {
		case WM_NCDESTROY:
		{
			RemoveWindowSubclass(hWnd, &ComboboxCtrl_DDL, uIdSubclass);
			return (LRESULT)0;
		}

		case WM_ERASEBKGND:
		{
			pThis->ComboboxCtrl_DDL_OnErase(hWnd, (HDC)wParam);
			return (LRESULT)1;
		}
		
		case WM_NCPAINT:
		{
			pThis->ComboboxCtrl_DDL_OnNCPaint(hWnd);
			return (LRESULT)0;
		}
		
		case WM_NCCALCSIZE:
		{
			return (LRESULT)0;  // Return zero to kill standard frames
		}
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}




/******************************************
*    EDIT CONTROL SUBCLASS DEFINITIONS    *
*******************************************/
// * SUBCLASS CALLBACK PROCEDURE
LRESULT CALLBACK SC_EditControl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	// Capture enter key message of the edit control and do something with it
	if (ktEnterKey)
	{
		auto cf = GetFocus();
		if (cf == ED_Normal)
		{
			SendMessageW(GetParent(hWnd), WM_COMMAND, BUTTON_EDNORMALOK, NULL);
		}
		else if (cf == ED_Password)
		{
			SendMessageW(GetParent(hWnd), WM_COMMAND, BUTTON_EDPASSWORDOK, NULL);
		}
		/*
		else if (cf == ED_Multiline)
		{
			// Do something ...
		}
		*/
	}

	switch (uMsg)
	{
	case WM_NCDESTROY:
	{
		RemoveWindowSubclass(hWnd, &SC_EditControl, uIdSubclass);
		return (LRESULT)0;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_TAB: // Capture VK_TAB and forward the focus to the next tab stop
		{
			/*
			// Uncomment to skip processing VK_TAB message for ED_Multiline
			if (hWnd == ED_Multiline) break;
			*/
			HWND NextFocus = GetNextDlgTabItem(MAIN_HWND, hWnd, (int)(GetKeyState(VK_SHIFT) & 0x8000));
			SetFocus(NextFocus);
			break;
		}

		default:
			break;
		}

		break;
	}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}