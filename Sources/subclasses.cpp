#include "./Headers/c_resources.h"
#include <iostream>
#include <string>
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

using namespace Gdiplus;

namespace nSol
{
	void GetRoundRectPath(GraphicsPath* pPath, Rect r, int dia);
	void DrawRoundRect(Graphics* pGraphics, Rect r, Color color, int radius, int width);
	void FillRoundRect(Graphics* pGraphics, Brush* pBrush, Rect r, Color border, int radius);
	void CreateHFONT(HFONT* hFontPtr, std::wstring fName, int fSize, int fWeight = FW_DONTCARE, int fQuality = DEFAULT_QUALITY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * BUTTON ANIMATION CLASS DEFINITIONS (CAPTION BAR)
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
			animParams.style = BPAS_SINE; // Alternative: BPAS_NONE

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
			animParams.style = BPAS_SINE;

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
LRESULT CALLBACK SC_BA_CaptionBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
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
			if (!isLBDown) BA_CaptionBar_Manager->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else BA_CaptionBar_Manager->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
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
			BA_CaptionBar_Manager->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

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
				BA_CaptionBar_Manager->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);
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
			BA_CaptionBar_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

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
				BA_CaptionBar_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * BUTTON ANIMATION CLASS DEFINITIONS (STANDARD)
void BA_Standard::UpdateObjects(Color Default, Color Hover, Color LBDown, Color Background, Color BorderNonFocus, Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor)
{
	if (this->IsReady)
	{
		// Release previously objects
		DeleteObject(this->hBrush_ButtonColor_Default);
		DeleteObject(this->hBrush_ButtonColor_Hover);
		DeleteObject(this->hBrush_ButtonColor_LBDown);
		DeleteObject(this->hBrush_ButtonBackgroundColor);
	}

	// Create new objects
	this->CL_ButtonColor_Default = Default;
	this->CL_ButtonColor_Hover = Hover;
	this->CL_ButtonColor_LBDown = LBDown;
	this->CL_ButtonBackgroundColor = Background;
	this->CL_ButtonBorderColor_NonFocus = BorderNonFocus;
	this->CL_ButtonBorderColor_OnFocus = BorderOnFocus;
	this->hBrush_ButtonColor_Default = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->hBrush_ButtonColor_Hover = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Hover.GetRed(), (int)this->CL_ButtonColor_Hover.GetGreen(), (int)this->CL_ButtonColor_Hover.GetBlue()));
	this->hBrush_ButtonColor_LBDown = CreateSolidBrush(RGB((int)this->CL_ButtonColor_LBDown.GetRed(), (int)this->CL_ButtonColor_LBDown.GetGreen(), (int)this->CL_ButtonColor_LBDown.GetBlue()));
	this->hBrush_ButtonBackgroundColor = CreateSolidBrush(RGB((int)this->CL_ButtonBackgroundColor.GetRed(), (int)this->CL_ButtonBackgroundColor.GetGreen(), (int)this->CL_ButtonBackgroundColor.GetBlue()));
	this->CLR_DefaultTextColor = DefaultTextColor;
	this->CLR_HighlightTextColor = HighlightTextColor;
	this->hFont_ButtonFont = &Font;

	this->IsReady = true;
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
	SelectObject(hdc, (HFONT)*this->hFont_ButtonFont);

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
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
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonBorderColor_NonFocus, 4);
		}
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (GetFocus() != hWnd)
		{
			nSol::FillRoundRect(&graphics, &SB_Background, grc, Color(255, 155, 155, 155), 4);
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
	SelectObject(hdc, (HFONT)*this->hFont_ButtonFont);

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
	if (state)
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
LRESULT CALLBACK SC_BA_Standard(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
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

	case WM_NCDESTROY:
	{
		RemoveWindowSubclass(hWnd, &SC_BA_Standard, uIdSubclass);
		return 0;
	}

	case WM_PAINT:
	{
		if (!isLBDown) BA_Standard_Manager->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
		else BA_Standard_Manager->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
		return 0;
	}


	case WM_LBUTTONDOWN:
	{
		nState_LB = 1;
		cState_LB = 1;
		isLBDown = 1;
		BufferedPaintStopAllAnimations(hWnd);
		BA_Standard_Manager->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

		SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL); // Forward WM_COMMAND messages to main window procedure
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
			BA_Standard_Manager->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);
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
		BA_Standard_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

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
			BA_Standard_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
			isHover = 1;

			return 0;
		}

		break;
	}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * BUTTON ANIMATION CLASS DEFINITIONS (RADIO2)
void BA_Radio2::UpdateObjects(Color Default, Color Hover, Color LBDown, Color Background, Color BorderNonFocus, Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor)
{
	if (this->IsReady)
	{
		// Release previously objects
		DeleteObject(this->hBrush_ButtonColor_Default);
		DeleteObject(this->hBrush_ButtonColor_Hover);
		DeleteObject(this->hBrush_ButtonColor_LBDown);
		DeleteObject(this->hBrush_ButtonBackgroundColor);
	}

	// Create new objects
	this->CL_ButtonColor_Default = Default;
	this->CL_ButtonColor_Hover = Hover;
	this->CL_ButtonColor_LBDown = LBDown;
	this->CL_ButtonBackgroundColor = Background;
	this->CL_ButtonBorderColor_NonFocus = BorderNonFocus;
	this->CL_ButtonBorderColor_OnFocus = BorderOnFocus;
	this->hBrush_ButtonColor_Default = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->hBrush_ButtonColor_Hover = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Hover.GetRed(), (int)this->CL_ButtonColor_Hover.GetGreen(), (int)this->CL_ButtonColor_Hover.GetBlue()));
	this->hBrush_ButtonColor_LBDown = CreateSolidBrush(RGB((int)this->CL_ButtonColor_LBDown.GetRed(), (int)this->CL_ButtonColor_LBDown.GetGreen(), (int)this->CL_ButtonColor_LBDown.GetBlue()));
	this->hBrush_ButtonBackgroundColor = CreateSolidBrush(RGB((int)this->CL_ButtonBackgroundColor.GetRed(), (int)this->CL_ButtonBackgroundColor.GetGreen(), (int)this->CL_ButtonBackgroundColor.GetBlue()));
	this->CLR_DefaultTextColor = DefaultTextColor;
	this->CLR_HighlightTextColor = HighlightTextColor;
	this->hFont_ButtonFont = &Font;

	this->IsReady = true;
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
	SelectObject(hdc, (HFONT)*this->hFont_ButtonFont);

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
	if (state)
	{
		if (hWnd == BTN_Radio2Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == BTN_Radio2Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
	}
	else
	{
		if (hWnd == BTN_Radio2Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
	}

	if (hWnd == CURRENT_SELECTEDRADIO2)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == BTN_Radio2Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
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
	SelectObject(hdc, (HFONT)*this->hFont_ButtonFont);

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
	if (state)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == BTN_Radio2Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, CLR_HighlightTextColor);
		if (hWnd == BTN_Radio2Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
		}
	}
	else
	{
		if (hWnd == BTN_Radio2Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
		else
		{
			Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.right = lerc.left + 10;
			lerc.left += 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
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
LRESULT CALLBACK SC_BA_Radio2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
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
			if (!isLBDown) BA_Radio2_Manager->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else BA_Radio2_Manager->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
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
			BA_Radio2_Manager->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

			SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL); // Forward WM_COMMAND messages to main window procedure
			return 0;
		}

		case WM_MOUSELEAVE:
		{
			isLBDown = 0;
			isHover = 0;
			BA_Radio2_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

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
				BA_Radio2_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * BUTTON ANIMATION CLASS DEFINITIONS (RADIO3)
void BA_Radio3::UpdateObjects(Color Default, Color Hover, Color LBDown, Color Background, Color BorderNonFocus, Color BorderOnFocus, HFONT& Font, COLORREF DefaultTextColor, COLORREF HighlightTextColor)
{
	if (this->IsReady)
	{
		// Release previously objects
		DeleteObject(this->hPen_ButtonColor_Default);
		DeleteObject(this->hBrush_ButtonColor_Default);
		DeleteObject(this->hBrush_ButtonColor_Hover);
		DeleteObject(this->hBrush_ButtonColor_LBDown);
		DeleteObject(this->hBrush_ButtonBackgroundColor);
	}

	// Create new objects
	this->CL_ButtonColor_Default = Default;
	this->CL_ButtonColor_Hover = Hover;
	this->CL_ButtonColor_LBDown = LBDown;
	this->CL_ButtonBackgroundColor = Background;
	this->CL_ButtonBorderColor_NonFocus = BorderNonFocus;
	this->CL_ButtonBorderColor_OnFocus = BorderOnFocus;
	this->hPen_ButtonColor_Default = CreatePen(PS_SOLID, NULL, RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->hBrush_ButtonColor_Default = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Default.GetRed(), (int)this->CL_ButtonColor_Default.GetGreen(), (int)this->CL_ButtonColor_Default.GetBlue()));
	this->hBrush_ButtonColor_Hover = CreateSolidBrush(RGB((int)this->CL_ButtonColor_Hover.GetRed(), (int)this->CL_ButtonColor_Hover.GetGreen(), (int)this->CL_ButtonColor_Hover.GetBlue()));
	this->hBrush_ButtonColor_LBDown = CreateSolidBrush(RGB((int)this->CL_ButtonColor_LBDown.GetRed(), (int)this->CL_ButtonColor_LBDown.GetGreen(), (int)this->CL_ButtonColor_LBDown.GetBlue()));
	this->hBrush_ButtonBackgroundColor = CreateSolidBrush(RGB((int)this->CL_ButtonBackgroundColor.GetRed(), (int)this->CL_ButtonBackgroundColor.GetGreen(), (int)this->CL_ButtonBackgroundColor.GetBlue()));
	this->CLR_DefaultTextColor = DefaultTextColor;
	this->CLR_HighlightTextColor = HighlightTextColor;
	this->hFont_ButtonFont = &Font;

	this->IsReady = true;
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
	SelectObject(hdc, (HFONT)*this->hFont_ButtonFont);

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
	if (state)
	{
		if (hWnd == BTN_Radio3Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
		else if (hWnd == BTN_Radio3Middle)
		{
			SelectObject(hdc, (HBRUSH)this->hBrush_ButtonColor_Default);
			SelectObject(hdc, (HPEN)this->hPen_ButtonColor_Default);
			RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
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
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == BTN_Radio3Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
		else if (hWnd == BTN_Radio3Middle)
		{
			SelectObject(hdc, (HBRUSH)this->hBrush_ButtonColor_Hover);
			SelectObject(hdc, (HPEN)this->hPen_ButtonColor_Default);
			RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
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
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
	}
	else
	{
		if (hWnd == BTN_Radio3Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
		else if (hWnd == BTN_Radio3Middle)
		{
			SelectObject(hdc, (HBRUSH)this->hBrush_ButtonColor_Default);
			SelectObject(hdc, (HPEN)this->hPen_ButtonColor_Default);
			RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
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
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
	}

	if (hWnd == CURRENT_SELECTEDRADIO3)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == BTN_Radio3Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
		}
		else if (hWnd == BTN_Radio3Middle)
		{
			SelectObject(hdc, (HBRUSH)this->hBrush_ButtonColor_LBDown);
			SelectObject(hdc, (HPEN)this->hPen_ButtonColor_Default);
			RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
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
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
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
	SelectObject(hdc, (HFONT)*this->hFont_ButtonFont);

	FillRect(hdc, &rc, this->hBrush_ButtonBackgroundColor);
	if (state)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == BTN_Radio3Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_H, grc, this->CL_ButtonColor_Hover, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
		else if (hWnd == BTN_Radio3Middle)
		{
			SelectObject(hdc, (HBRUSH)this->hBrush_ButtonColor_Hover);
			SelectObject(hdc, (HPEN)this->hPen_ButtonColor_Default);
			RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
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
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Hover);
		}
	}
	else if (!state && hWnd == cHWND)
	{
		SetTextColor(hdc, this->CLR_HighlightTextColor);
		if (hWnd == BTN_Radio3Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background_F, grc, this->CL_ButtonColor_LBDown, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
		}
		else if (hWnd == BTN_Radio3Middle)
		{
			SelectObject(hdc, (HBRUSH)this->hBrush_ButtonColor_LBDown);
			SelectObject(hdc, (HPEN)this->hPen_ButtonColor_Default);
			RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
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
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_LBDown);
		}
	}
	else
	{
		if (hWnd == BTN_Radio3Left)
		{
			Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
			nSol::FillRoundRect(&graphics, &SB_Background, grc, this->CL_ButtonColor_Default, 4);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			RECT lerc = rc;
			lerc.left = lerc.right - 10;
			lerc.right -= 1;
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
		}
		else if (hWnd == BTN_Radio3Middle)
		{
			SelectObject(hdc, (HBRUSH)this->hBrush_ButtonColor_Default);
			SelectObject(hdc, (HPEN)this->hPen_ButtonColor_Default);
			RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
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
			FillRect(hdc, &lerc, this->hBrush_ButtonColor_Default);
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
LRESULT CALLBACK SC_BA_Radio3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
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
			if (!isLBDown) BA_Radio3_Manager->OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else BA_Radio3_Manager->OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
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
			BA_Radio3_Manager->StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

			SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL);
			return 0;
		}

		case WM_MOUSELEAVE:
		{
			isLBDown = 0;
			isHover = 0;
			BA_Radio3_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

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
				BA_Radio3_Manager->StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////