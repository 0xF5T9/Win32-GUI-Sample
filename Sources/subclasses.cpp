#include "./Headers/c_resources.h"
#include <iostream>
#include <string>
#include <map>
#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <Dwmapi.h>
#include "./Headers/global.h"

using namespace Gdiplus;

namespace mSol
{
	void GetRoundRectPath(GraphicsPath* pPath, Rect r, int dia);
	void DrawRoundRect(Graphics* pGraphics, Rect r, Color color, int radius, int width);
	void FillRoundRect(Graphics* pGraphics, Brush* pBrush, Rect r, Color border, int radius);
	void CreateHFONT(HFONT* hFontPtr, std::wstring fName, int fSize, int fWeight = FW_DONTCARE, int fQuality = DEFAULT_QUALITY);
}

////////////////////////////////////////////////////////////

namespace NS_BA_CaptionBar
{
	HBRUSH hBrush_Background; // Default color
	HBRUSH hBrush_Background_H; // Hover color
	HBRUSH hBrush_Background_H_Close; // Hover color (Close)
	HBRUSH hBrush_Background_H_Minimize; // Hover color (Minimize)
	HBRUSH hBrush_Background_F; // Down color
	HBRUSH hBrush_Background_S; // Background color

	// Release objects
	bool IsReleased = 0;
	void ReleaseObject()
	{
		DeleteObject(hBrush_Background);
		DeleteObject(hBrush_Background_H);
		DeleteObject(hBrush_Background_H_Close);
		DeleteObject(hBrush_Background_H_Minimize);
		DeleteObject(hBrush_Background_F);
		DeleteObject(hBrush_Background_S);
		IsReleased = 1;
	}

	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated)
	{
		nState = !cState;
		frames_Invalidated = 0;
		InvalidateRect(hWnd, NULL, TRUE);
		SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
	}

	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		static HICON hIcon, hIcon_H, hIcon_NF;
		for (const auto& x : HoverMap_1)
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
		HBRUSH hBrush_Hover = NS_BA_CaptionBar::hBrush_Background_H;
		if (hWnd == BTN_Close) hBrush_Hover = NS_BA_CaptionBar::hBrush_Background_H_Close;
		else if (hWnd == BTN_Minimize) hBrush_Hover = NS_BA_CaptionBar::hBrush_Background_H_Minimize;

		FillRect(hdc, &rc, NS_BA_CaptionBar::hBrush_Background_S);
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

	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		static HICON hIcon, hIcon_H, hIcon_NF;
		for (const auto& x : HoverMap_1)
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
		HBRUSH hBrush_Hover = NS_BA_CaptionBar::hBrush_Background_H;
		if (hWnd == BTN_Close) hBrush_Hover = NS_BA_CaptionBar::hBrush_Background_H_Close;
		else if (hWnd == BTN_Minimize) hBrush_Hover = NS_BA_CaptionBar::hBrush_Background_H_Minimize;

		FillRect(hdc, &rc, NS_BA_CaptionBar::hBrush_Background_S);
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

	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
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

				animParams.dwDuration = (cState_H != nState_H ? 150 : 0);

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

	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
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

				animParams.dwDuration = (cState_LB != nState_LB ? 150 : 0);

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
}

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
			if (!NS_BA_CaptionBar::IsReleased) NS_BA_CaptionBar::ReleaseObject();
			return 0;
		}

		case WM_PAINT:
		{
			if (!isLBDown) NS_BA_CaptionBar::OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else NS_BA_CaptionBar::OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
			return 0;
		}

		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_TIMER:
		{
			switch (wParam)
			{
			case 1:
			{
				InvalidateRect(hWnd, NULL, TRUE);

				frames_Invalidated++;
				if (frames_Invalidated == 60)
				{
					KillTimer(hWnd, 1);
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
			NS_BA_CaptionBar::StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

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
				NS_BA_CaptionBar::StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);
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
			NS_BA_CaptionBar::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

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
				NS_BA_CaptionBar::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////

namespace NS_BA_Standard
{
	Color CL_Background;
	Color CL_Background_H;
	Color CL_Background_F;
	Color CL_Background_S;
	Color CL_NonFocus;
	Color CL_Focus;
	HBRUSH hBrush_Background;
	HBRUSH hBrush_Background_H;
	HBRUSH hBrush_Background_F;
	HBRUSH hBrush_Background_S;
	HFONT* hFont_PDefault;
	COLORREF CLR_DefaultTextColor;
	COLORREF CLR_HighlightTextColor;

	// Release objects
	bool IsReleased = 0;
	void ReleaseObject()
	{
		DeleteObject(hBrush_Background);
		DeleteObject(hBrush_Background_H);
		DeleteObject(hBrush_Background_F);
		DeleteObject(hBrush_Background_S);
		IsReleased = 1;
	}

	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated)
	{
		nState = !cState;
		frames_Invalidated = 0;
		InvalidateRect(hWnd, NULL, TRUE);
		SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
	}

	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		Graphics graphics(hdc);
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

		RECT rc;
		GetClientRect(hWnd, &rc);
		Gdiplus::Rect grc(rc.top, rc.left, rc.right, rc.bottom);
		WCHAR TextBuffer[15];
		GetWindowTextW(hWnd, TextBuffer, 15);
		SetTextColor(hdc, NS_BA_Standard::CLR_DefaultTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SolidBrush SB_Background = NS_BA_Standard::CL_Background;
		SolidBrush SB_Background_H = NS_BA_Standard::CL_Background_H;
		SelectObject(hdc, (HFONT)*NS_BA_Standard::hFont_PDefault);

		FillRect(hdc, &rc, NS_BA_Standard::hBrush_Background_S);
		if (state)
		{
			if (GetFocus() != hWnd)
			{
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Standard::CL_NonFocus, 4);
			}
			else
			{
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Standard::CL_Focus, 4);
			}
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (!state && hWnd == cHWND)
		{
			if (GetFocus() != hWnd)
			{
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Standard::CL_NonFocus, 4);
			}
			else
			{
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Standard::CL_NonFocus, 4);
			}
			SetTextColor(hdc, NS_BA_Standard::CLR_HighlightTextColor);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			if (GetFocus() != hWnd)
			{
				mSol::FillRoundRect(&graphics, &SB_Background, grc, Color(255, 155, 155, 155), 4);
			}
			else
			{
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Standard::CL_Focus, 4);
			}
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		Graphics graphics(hdc);
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

		RECT rc;
		GetClientRect(hWnd, &rc);
		Gdiplus::Rect grc(rc.top, rc.left, rc.right, rc.bottom);
		WCHAR TextBuffer[15];
		GetWindowTextW(hWnd, TextBuffer, 15);
		SetTextColor(hdc, NS_BA_Standard::CLR_DefaultTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SolidBrush SB_Background = NS_BA_Standard::CL_Background;
		SolidBrush SB_Background_H = NS_BA_Standard::CL_Background_H;
		SolidBrush SB_Background_F = NS_BA_Standard::CL_Background_F;
		SelectObject(hdc, (HFONT)*NS_BA_Standard::hFont_PDefault);

		FillRect(hdc, &rc, NS_BA_Standard::hBrush_Background_S);
		if (state)
		{
			if (GetFocus() != hWnd)
			{
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Standard::CL_NonFocus, 4);
			}
			else
			{
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Standard::CL_Focus, 4);
			}
			SetTextColor(hdc, NS_BA_Standard::CLR_HighlightTextColor);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (!state && hWnd == cHWND)
		{
			if (GetFocus() != hWnd)
			{
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, NS_BA_Standard::CL_NonFocus, 4);
			}
			else
			{
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, NS_BA_Standard::CL_Focus, 4);
			}
			SetTextColor(hdc, NS_BA_Standard::CLR_HighlightTextColor);
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			if (GetFocus() != hWnd)
			{
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Standard::CL_NonFocus, 4);
			}
			else
			{
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Standard::CL_Focus, 4);
			}
			DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
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

				animParams.dwDuration = (cState_H != nState_H ? 150 : 0);

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

	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
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

				animParams.dwDuration = (cState_LB != nState_LB ? 150 : 0);

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
}

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
		switch (wParam)
		{
			case 1:
			{
				InvalidateRect(hWnd, NULL, TRUE);

				frames_Invalidated++;
				if (frames_Invalidated == 60) 
				{ 
					KillTimer(hWnd, 1);
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
		if (!NS_BA_Standard::IsReleased) NS_BA_Standard::ReleaseObject();
		return 0;
	}

	case WM_PAINT:
	{
		if (!isLBDown) NS_BA_Standard::OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
		else NS_BA_Standard::OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
		return 0;
	}


	case WM_LBUTTONDOWN:
	{
		nState_LB = 1;
		cState_LB = 1;
		isLBDown = 1;
		BufferedPaintStopAllAnimations(hWnd);
		NS_BA_Standard::StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

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
			NS_BA_Standard::StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);
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
		NS_BA_Standard::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

		return 0;
	}

	/*case WM_MOUSEHOVER:
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		TrackMouseEvent(&tme);
		NS_BA_Standard::StartAnimation(hWnd, nState_H, cState_H);
		isHover = 1;
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (!isHover)
		{
			cHWND = hWnd;
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER;
			tme.dwHoverTime = 1;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
			return 0;
		}

		break;
	}*/

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
			NS_BA_Standard::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
			isHover = 1;

			return 0;
		}

		break;
	}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////

namespace NS_BA_Radio2
{
	Color CL_Background;
	Color CL_Background_H;
	Color CL_Background_F;
	Color CL_Background_S;
	Color CL_NonFocus;
	Color CL_Focus;
	HBRUSH hBrush_Background;
	HBRUSH hBrush_Background_H;
	HBRUSH hBrush_Background_F;
	HBRUSH hBrush_Background_S;
	HFONT* hFont_PDefault;
	COLORREF CLR_DefaultTextColor;
	COLORREF CLR_HighlightTextColor;

	// Release objects
	bool IsReleased = 0;
	void ReleaseObject()
	{
		DeleteObject(hBrush_Background);
		DeleteObject(hBrush_Background_H);
		DeleteObject(hBrush_Background_F);
		DeleteObject(hBrush_Background_S);
		IsReleased = 1;
	}

	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short& frames_Invalidated)
	{
		nState = !cState;
		frames_Invalidated = 0;
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
	}

	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		Graphics graphics(hdc);
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

		RECT rc;
		GetClientRect(hWnd, &rc);
		WCHAR TextBuffer[15];
		GetWindowTextW(hWnd, TextBuffer, 15);
		SetTextColor(hdc, CLR_DefaultTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SolidBrush SB_Background = CL_Background;
		SolidBrush SB_Background_H = CL_Background_H;
		SolidBrush SB_Background_F = CL_Background_F;
		SelectObject(hdc, (HFONT)*hFont_PDefault);

		FillRect(hdc, &rc, hBrush_Background_S);
		if (state)
		{
			if (hWnd == BTN_Radio2Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, hBrush_Background);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, hBrush_Background);
			}
		}
		else if (!state && hWnd == cHWND)
		{
			SetTextColor(hdc, CLR_HighlightTextColor);
			if (hWnd == BTN_Radio2Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, hBrush_Background_H);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, hBrush_Background_H);
			}
		}
		else
		{
			if (hWnd == BTN_Radio2Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, hBrush_Background);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, hBrush_Background);
			}
		}

		if (hWnd == CURRENT_SELECTEDRADIO2)
		{
			SetTextColor(hdc, CLR_HighlightTextColor);
			if (hWnd == BTN_Radio2Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, hBrush_Background_F);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, hBrush_Background_F);
			}
		}
	}

	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		Graphics graphics(hdc);
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

		RECT rc;
		GetClientRect(hWnd, &rc);
		WCHAR TextBuffer[15];
		GetWindowTextW(hWnd, TextBuffer, 15);
		SetTextColor(hdc, CLR_DefaultTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SolidBrush SB_Background = CL_Background;
		SolidBrush SB_Background_H = CL_Background_H;
		SolidBrush SB_Background_F = CL_Background_F;
		SelectObject(hdc, (HFONT)*hFont_PDefault);

		FillRect(hdc, &rc, hBrush_Background_S);
		if (state)
		{
			SetTextColor(hdc, CLR_HighlightTextColor);
			if (hWnd == BTN_Radio2Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, hBrush_Background_H);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, hBrush_Background_H);
			}
		}
		else if (!state && hWnd == cHWND)
		{
			SetTextColor(hdc, CLR_HighlightTextColor);
			if (hWnd == BTN_Radio2Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, hBrush_Background_F);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, hBrush_Background_F);
			}
		}
		else
		{
			if (hWnd == BTN_Radio2Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, hBrush_Background);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, hBrush_Background);
			}
		}
	}

	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
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

				animParams.dwDuration = (cState_H != nState_H ? 100 : 0);

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

	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
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

				animParams.dwDuration = (cState_LB != nState_LB ? 300 : 0);

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
}

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
			if (!NS_BA_Radio2::IsReleased) NS_BA_Radio2::ReleaseObject();
			return 0;
		}

		case WM_PAINT:
		{
			if (!isLBDown) NS_BA_Radio2::OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else NS_BA_Radio2::OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
			return 0;
		}

		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_TIMER:
		{
			switch (wParam)
			{
			case 1:
			{
				InvalidateRect(hWnd, NULL, TRUE);

				frames_Invalidated++;
				if (frames_Invalidated == 60)
				{
					KillTimer(hWnd, 1);
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
			NS_BA_Radio2::StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

			SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL); // Forward WM_COMMAND messages to main window procedure
			return 0;
		}

		case WM_MOUSELEAVE:
		{
			isLBDown = 0;
			isHover = 0;
			NS_BA_Radio2::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

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
				NS_BA_Radio2::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////

namespace NS_BA_Radio3
{
	Color CL_Background;
	Color CL_Background_H;
	Color CL_Background_F;
	Color CL_Background_S;
	HPEN hPen_Background;
	HBRUSH hBrush_Background;
	HBRUSH hBrush_Background_H;
	HBRUSH hBrush_Background_F;
	HBRUSH hBrush_Background_S;
	HFONT* hFont_PDefault;
	COLORREF CLR_DefaultTextColor;
	COLORREF CLR_HighlightTextColor;

	// Release objects
	bool IsReleased = 0;
	void ReleaseObject()
	{
		DeleteObject(hPen_Background);
		DeleteObject(hBrush_Background);
		DeleteObject(hBrush_Background_H);
		DeleteObject(hBrush_Background_F);
		DeleteObject(hBrush_Background_S);
		IsReleased = 1;
	}

	void StartAnimation(HWND hWnd, bool& nState, bool& cState, unsigned short frames_Invalidated)
	{
		nState = !cState;
		frames_Invalidated = 0;
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		SetTimer(hWnd, 1, ANIMATION_INVALIDATE_TICK, NULL);
	}

	void Paint_Hover(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		Graphics graphics(hdc);
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

		RECT rc;
		GetClientRect(hWnd, &rc);
		WCHAR TextBuffer[15];
		GetWindowTextW(hWnd, TextBuffer, 15);
		SetTextColor(hdc, NS_BA_Radio3::CLR_DefaultTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SolidBrush SB_Background = NS_BA_Radio3::CL_Background;
		SolidBrush SB_Background_H = NS_BA_Radio3::CL_Background_H;
		SolidBrush SB_Background_F = NS_BA_Radio3::CL_Background_F;
		SelectObject(hdc, (HFONT)*NS_BA_Radio3::hFont_PDefault);

		FillRect(hdc, &rc, NS_BA_Radio3::hBrush_Background_S);
		if (state)
		{
			if (hWnd == BTN_Radio3Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Radio3::CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background);
			}
			else if (hWnd == BTN_Radio3Middle)
			{
				SelectObject(hdc, (HBRUSH)NS_BA_Radio3::hBrush_Background);
				SelectObject(hdc, (HPEN)NS_BA_Radio3::hPen_Background);
				RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Radio3::CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background);
			}
		}
		else if (!state && hWnd == cHWND)
		{
			SetTextColor(hdc, NS_BA_Radio3::CLR_HighlightTextColor);
			if (hWnd == BTN_Radio3Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Radio3::CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_H);
			}
			else if (hWnd == BTN_Radio3Middle)
			{
				SelectObject(hdc, (HBRUSH)NS_BA_Radio3::hBrush_Background_H);
				SelectObject(hdc, (HPEN)NS_BA_Radio3::hPen_Background);
				RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Radio3::CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_H);
			}
		}
		else
		{
			if (hWnd == BTN_Radio3Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Radio3::CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background);
			}
			else if (hWnd == BTN_Radio3Middle)
			{
				SelectObject(hdc, (HBRUSH)NS_BA_Radio3::hBrush_Background);
				SelectObject(hdc, (HPEN)NS_BA_Radio3::hPen_Background);
				RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Radio3::CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background);
			}
		}

		if (hWnd == CURRENT_SELECTEDRADIO3)
		{
			SetTextColor(hdc, NS_BA_Radio3::CLR_HighlightTextColor);
			if (hWnd == BTN_Radio3Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, NS_BA_Radio3::CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_F);
			}
			else if (hWnd == BTN_Radio3Middle)
			{
				SelectObject(hdc, (HBRUSH)NS_BA_Radio3::hBrush_Background_F);
				SelectObject(hdc, (HPEN)NS_BA_Radio3::hPen_Background);
				RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, NS_BA_Radio3::CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_F);
			}
		}
	}

	void Paint_LBDown(HWND& hWnd, HWND& cHWND, HDC hdc, bool state)
	{
		Graphics graphics(hdc);
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

		RECT rc;
		GetClientRect(hWnd, &rc);
		WCHAR TextBuffer[15];
		GetWindowTextW(hWnd, TextBuffer, 15);
		SetTextColor(hdc, NS_BA_Radio3::CLR_DefaultTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SolidBrush SB_Background = NS_BA_Radio3::CL_Background;
		SolidBrush SB_Background_H = NS_BA_Radio3::CL_Background_H;
		SolidBrush SB_Background_F = NS_BA_Radio3::CL_Background_F;
		SelectObject(hdc, (HFONT)*NS_BA_Radio3::hFont_PDefault);

		FillRect(hdc, &rc, NS_BA_Radio3::hBrush_Background_S);
		if (state)
		{
			SetTextColor(hdc, NS_BA_Radio3::CLR_HighlightTextColor);
			if (hWnd == BTN_Radio3Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Radio3::CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_H);
			}
			else if (hWnd == BTN_Radio3Middle)
			{
				SelectObject(hdc, (HBRUSH)NS_BA_Radio3::hBrush_Background_H);
				SelectObject(hdc, (HPEN)NS_BA_Radio3::hPen_Background);
				RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_H, grc, NS_BA_Radio3::CL_Background_H, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_H);
			}
		}
		else if (!state && hWnd == cHWND)
		{
			SetTextColor(hdc, NS_BA_Radio3::CLR_HighlightTextColor);
			if (hWnd == BTN_Radio3Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, NS_BA_Radio3::CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_F);
			}
			else if (hWnd == BTN_Radio3Middle)
			{
				SelectObject(hdc, (HBRUSH)NS_BA_Radio3::hBrush_Background_F);
				SelectObject(hdc, (HPEN)NS_BA_Radio3::hPen_Background);
				RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background_F, grc, NS_BA_Radio3::CL_Background_F, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background_F);
			}
		}
		else
		{
			if (hWnd == BTN_Radio3Left)
			{
				Gdiplus::Rect grc(rc.top, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Radio3::CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.left = lerc.right - 10;
				lerc.right -= 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background);
			}
			else if (hWnd == BTN_Radio3Middle)
			{
				SelectObject(hdc, (HBRUSH)NS_BA_Radio3::hBrush_Background);
				SelectObject(hdc, (HPEN)NS_BA_Radio3::hPen_Background);
				RoundRect(hdc, rc.left, rc.top - 1, rc.right, rc.bottom + 1, 0, 0);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				Gdiplus::Rect grc(rc.top + 5, rc.left, rc.right - 5, rc.bottom);
				mSol::FillRoundRect(&graphics, &SB_Background, grc, NS_BA_Radio3::CL_Background, 4);
				DrawTextW(hdc, TextBuffer, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT lerc = rc;
				lerc.right = lerc.left + 10;
				lerc.left += 1;
				FillRect(hdc, &lerc, NS_BA_Radio3::hBrush_Background);
			}
		}
	}

	void OnPaint_Hover(HWND hWnd, HWND& cHWND, bool& nState_H, bool& cState_H)
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

				animParams.dwDuration = (cState_H != nState_H ? 100 : 0);

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

	void OnPaint_LBDown(HWND hWnd, HWND& cHWND, bool& nState_LB, bool& cState_LB)
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

				animParams.dwDuration = (cState_LB != nState_LB ? 300 : 0);

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
}

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
			if (!NS_BA_Radio3::IsReleased) NS_BA_Radio3::ReleaseObject();
			return 0;
		}

		case WM_PAINT:
		{
			if (!isLBDown) NS_BA_Radio3::OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else NS_BA_Radio3::OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
			return 0;
		}

		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_TIMER:
		{
			switch (wParam)
			{
			case 1:
			{
				InvalidateRect(hWnd, NULL, TRUE);

				frames_Invalidated++;
				if (frames_Invalidated == 60)
				{
					KillTimer(hWnd, 1);
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
			NS_BA_Radio3::StartAnimation(hWnd, nState_LB, cState_LB, frames_Invalidated);

			SendMessageW(GetParent(hWnd), WM_COMMAND, (WPARAM)GetDlgCtrlID(hWnd), NULL);
			return 0;
		}

		case WM_MOUSELEAVE:
		{
			isLBDown = 0;
			isHover = 0;
			NS_BA_Radio3::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);

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
				NS_BA_Radio3::StartAnimation(hWnd, nState_H, cState_H, frames_Invalidated);
				isHover = 1;

				return 0;
			}

			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}