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
}

namespace NS_BA_Main
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

	void StartAnimation(HWND hWnd, bool& nState, bool& cState)
	{
		nState = !cState;
		// RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		InvalidateRect(hWnd, NULL, TRUE);
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
		HBRUSH hBrush_Hover = NS_BA_Main::hBrush_Background_H;
		if (hWnd == BTN_Close) hBrush_Hover = NS_BA_Main::hBrush_Background_H_Close;
		else if (hWnd == BTN_Minimize) hBrush_Hover = NS_BA_Main::hBrush_Background_H_Minimize;

		FillRect(hdc, &rc, NS_BA_Main::hBrush_Background_S);
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
		HBRUSH hBrush_Hover = NS_BA_Main::hBrush_Background_H;
		if (hWnd == BTN_Close) hBrush_Hover = NS_BA_Main::hBrush_Background_H_Close;
		else if (hWnd == BTN_Minimize) hBrush_Hover = NS_BA_Main::hBrush_Background_H_Minimize;

		FillRect(hdc, &rc, NS_BA_Main::hBrush_Background_S);
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
				animParams.style = BPAS_SINE;

				animParams.dwDuration = (cState_LB != nState_LB ? 100 : 0);

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

LRESULT CALLBACK SC_BA_Main(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	static bool cState_H = 1;
	static bool nState_H = 1;
	static bool cState_LB = 1;
	static bool nState_LB = 1;
	static bool isHover = 0;
	static bool isLBDown = 0;
	static HWND cHWND;

	switch (uMsg)
	{
		case WM_NCDESTROY:
		{
			RemoveWindowSubclass(hWnd, &SC_BA_Main, uIdSubclass);
			if (!NS_BA_Main::IsReleased) NS_BA_Main::ReleaseObject();
			return 0;
		}

		case WM_PAINT:
		{
			if (!isLBDown) NS_BA_Main::OnPaint_Hover(hWnd, cHWND, nState_H, cState_H);
			else NS_BA_Main::OnPaint_LBDown(hWnd, cHWND, nState_LB, cState_LB);
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			nState_LB = 1;
			cState_LB = 1;
			isLBDown = 1;
			BufferedPaintStopAllAnimations(hWnd);
			NS_BA_Main::StartAnimation(hWnd, nState_LB, cState_LB);

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
				NS_BA_Main::StartAnimation(hWnd, nState_LB, cState_LB);
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
			NS_BA_Main::StartAnimation(hWnd, nState_H, cState_H);

			return 0;
		}

		case WM_MOUSEHOVER:
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
			NS_BA_Main::StartAnimation(hWnd, nState_H, cState_H);
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
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}