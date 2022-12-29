/*
	File: func.h
	Include libraries and contains most application function definitions
*/

#pragma once

#include "./c_resources.h"	// Contains application constants
#include <iostream>
#include <string>
#include <map>				// Maping for animations
#include <vector>			// Group and manage objects
#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>		// Use GDI+ for drawings
#include <Dwmapi.h>
#include <comdef.h>			// Read errors from HRESULT
#include "./Ex_DarkMode.h"	// Reverse and access to undocumented window "Dark Mode" API
#include "./global.h"			// Global variables distribution

// #pragma comment(lib, "UxTheme.lib")
// #pragma comment(lib, "Comctl32.lib")
// #pragma comment(lib, "Gdiplus.lib")
// #pragma comment(lib, "Dwmapi.lib")

using namespace Gdiplus;

// Forward declaration variables from subclasses definition file
namespace NS_BA_CaptionBar
{
	extern HBRUSH hBrush_Background,
		hBrush_Background_H,
		hBrush_Background_H_Close,
		hBrush_Background_H_Minimize,
		hBrush_Background_F,
		hBrush_Background_S;
};
namespace NS_BA_Standard
{
	extern Color CL_Background,
		CL_Background_H,
		CL_Background_F,
		CL_Background_S,
		CL_NonFocus,
		CL_Focus;
	extern HBRUSH hBrush_Background,
		hBrush_Background_H,
		hBrush_Background_F,
		hBrush_Background_S;
	extern HFONT* hFont_PDefault;
	extern COLORREF CLR_DefaultTextColor,
		CLR_HighlightTextColor;
};
namespace NS_BA_Radio2
{
	extern Color CL_Background,
		CL_Background_H,
		CL_Background_F,
		CL_Background_S,
		CL_NonFocus,
		CL_Focus;
	extern HBRUSH hBrush_Background,
		hBrush_Background_H,
		hBrush_Background_F,
		hBrush_Background_S;
	extern HFONT* hFont_PDefault;
	extern COLORREF CLR_DefaultTextColor,
		CLR_HighlightTextColor;
};
namespace NS_BA_Radio3
{
	extern Color CL_Background,
		CL_Background_H,
		CL_Background_F,
		CL_Background_S;
	extern HPEN hPen_Background;
	extern HBRUSH hBrush_Background,
		hBrush_Background_H,
		hBrush_Background_F,
		hBrush_Background_S;
	extern HFONT* hFont_PDefault;
	extern COLORREF CLR_DefaultTextColor,
		CLR_HighlightTextColor;
};

// CALLBACK FORWARD DECLARATIONS:
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WindowProcedure_MainContentCTR(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_CaptionBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_Standard(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_Radio2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_Radio3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

namespace mSol
{
	// Init undocumented dark mode API introduced in Windows 10 1809
	bool InitDarkModeAPI(HWND hWnd)
	{
		InitDarkMode(true); // Bypass winver check, may broke the application

		if (!g_darkModeSupported)
		{
			MessageBoxW(NULL, L"Error occurred!\n(Failed to init dark mode)", L"", MB_OK | MB_ICONERROR);
			return false;
		}
		_AllowDarkModeForWindow(hWnd, true);
		RefreshTitleBarThemeColor(hWnd);

		return true;
	}

	// Init GDI Animation & GDI+ APIs
	bool InitAPI()
	{
		HRESULT hr = BufferedPaintInit();
		_com_error err(hr);

		if (Gdiplus::GdiplusStartup(&API_Token, &API_StartupInput, NULL) == 0 && SUCCEEDED(hr))
			return true;
		else return false;
	}

	// Uninit GDI Animation & GDI+ APIs
	void UnInitAPI()
	{
		Gdiplus::GdiplusShutdown(API_Token);
		BufferedPaintUnInit();
	}

	// Get current window client size infos
	bool cShowSizeInfo(HWND hWnd)
	{
		RECT tRect;
		int cWidth = 0, wWidth = 0;
		int cHeight = 0, wHeight = 0;

		if (!GetClientRect(hWnd, &tRect))
			return false;
		cWidth = tRect.right - tRect.left;
		cHeight = tRect.bottom - tRect.top;

		if (!GetWindowRect(hWnd, &tRect))
			return false;
		wWidth = tRect.right - tRect.left;
		wHeight = tRect.bottom - tRect.top;

		MessageBoxW(NULL,
			(((std::wstring)L"Client Width: " + std::to_wstring(cWidth) + L" Client Height: " + std::to_wstring(cHeight)
				+ L"\nWindow Width: " + std::to_wstring(wWidth) + L" Window Height: " + std::to_wstring(wHeight)).c_str()), L"", MB_OK);
		return true;
	}

	// Get current user desktop resolution
	void GetDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();

		GetWindowRect(hDesktop, &desktop);
		
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}

	// Quick-create winapi font object
	void CreateHFONT(HFONT* hFontPtr, std::wstring fName, int fSize, int fWeight = FW_DONTCARE, int fQuality = DEFAULT_QUALITY)
	{
		*hFontPtr = CreateFontW(fSize, 0, 0, 0x1,
			fWeight, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, fQuality,
			DEFAULT_PITCH | FF_DONTCARE, (LPCWSTR)fName.c_str());
	}

	// Set font callback function
	bool CALLBACK cbSetFont(HWND child_hWnd, LPARAM hFont)
	{
		SendMessageW(child_hWnd, WM_SETFONT, hFont, TRUE);
		return true;
	}

	// Remove specific window style from control
	void RemoveWindowStyle(HWND& hWnd, LONG Style)
	{
		DWORD dwStyle = ::GetClassLongW(hWnd, GCL_STYLE);
		dwStyle &= ~Style;
		::SetClassLongW(hWnd, GCL_STYLE, dwStyle);
	}

	// Draw GDI+ Round Rect
	void GetRoundRectPath(GraphicsPath* pPath, Rect r, int dia)
	{
		// diameter can't exceed width or height
		if (dia > r.Width)    dia = r.Width;
		if (dia > r.Height)    dia = r.Height;

		// define a corner 
		Rect Corner(r.X, r.Y, dia, dia);

		// begin path
		pPath->Reset();

		// top left
		pPath->AddArc(Corner, 180, 90);

		// tweak needed for radius of 10 (dia of 20)
		if (dia == 20)
		{
			Corner.Width += 1;
			Corner.Height += 1;
			r.Width -= 1; r.Height -= 1;
		}

		// top right
		Corner.X += (r.Width - dia - 1);
		pPath->AddArc(Corner, 270, 90);

		// bottom right
		Corner.Y += (r.Height - dia - 1);
		pPath->AddArc(Corner, 0, 90);

		// bottom left
		Corner.X -= (r.Width - dia - 1);
		pPath->AddArc(Corner, 90, 90);

		// end path
		pPath->CloseFigure();
	}
	void DrawRoundRect(Graphics* pGraphics, Rect r, Color color, int radius, int width)
	{
		int dia = 2 * radius;

		// set to pixel mode
		int oldPageUnit = pGraphics->SetPageUnit(UnitPixel);

		// define the pen
		Pen pen(color, 1);
		pen.SetAlignment(PenAlignmentCenter);

		// get the corner path
		GraphicsPath path;

		// get path
		GetRoundRectPath(&path, r, dia);

		// draw the round rClient
		pGraphics->DrawPath(&pen, &path);

		// if width > 1
		for (int i = 1; i < width; i++)
		{
			// left stroke
			r.Inflate(-1, 0);
			// get the path
			GetRoundRectPath(&path, r, dia);

			// draw the round rClient
			pGraphics->DrawPath(&pen, &path);

			// up stroke
			r.Inflate(0, -1);

			// get the path
			GetRoundRectPath(&path, r, dia);

			// draw the round rClient
			pGraphics->DrawPath(&pen, &path);
		}

		// restore page unit
		pGraphics->SetPageUnit((Unit)oldPageUnit);
	}
	void FillRoundRect(Graphics* pGraphics, Brush* pBrush, Rect r, Color border, int radius)
	{
		int dia = 2 * radius;

		// set to pixel mode
		int oldPageUnit = pGraphics->SetPageUnit(UnitPixel);

		// define the pen
		Pen pen(border, 1);
		pen.SetAlignment(PenAlignmentCenter);

		// get the corner path
		GraphicsPath path;

		// get path
		GetRoundRectPath(&path, r, dia);

		// fill
		pGraphics->FillPath(pBrush, &path);

		// draw the border last so it will be on top
		pGraphics->DrawPath(&pen, &path);

		// restore page unit
		pGraphics->SetPageUnit((Unit)oldPageUnit);
	}

	// Quick-create winapi standalone scrollbar control
	HWND CreateHorizontalSB(HWND hParent, int sbHeight)
	{
		RECT rClient;
		if (!GetClientRect(hParent, &rClient))
			return NULL;

		return (CreateWindowExW(0, L"SCROLLBAR", NULL,
			WS_CHILD | WS_VISIBLE | SBS_VERT,
			rClient.left + BORDER_WIDTH, rClient.bottom - BORDER_WIDTH - sbHeight,
			rClient.right - (BORDER_WIDTH * 2), sbHeight,
			hParent, NULL, MAIN_HINSTANCE, NULL));
	}
	HWND CreateVerticalSB(HWND hParent, int sbWidth, int sbPosX)
	{
		RECT rClient;
		if (!GetClientRect(hParent, &rClient))
			return NULL;

		return (CreateWindowExW(0, L"SCROLLBAR", NULL,
			WS_CHILD | WS_VISIBLE | SBS_VERT,
			rClient.right - sbWidth - BORDER_WIDTH, sbPosX,
			sbWidth, rClient.bottom - BORDER_WIDTH - sbPosX,
			hParent, NULL, MAIN_HINSTANCE, NULL));
	}
}

namespace mApp
{
	/* Set window theme class for controls */
	void SetAppThemeClass(std::wstring Theme)
	{
		if (Theme == L"Light")
		{
			WCHAR TextBuffer[256];
			for (auto& x : Vector_MAINCONTENTCTR)
			{
				GetClassNameW(*x, TextBuffer, 256);
				std::wstring lwstr(TextBuffer);
				if (lwstr == L"ScrollBar")
					SetWindowTheme(*x, L"Explorer", L"ScrollBar");
				else if (lwstr == L"Button")
					SetWindowTheme(*x, L"Explorer", L"Button");
			}
		}
		else if (Theme == L"Dark" || Theme == L"Ristretto")
		{
			WCHAR TextBuffer[256];
			for (auto& x : Vector_MAINCONTENTCTR)
			{
				GetClassNameW(*x, TextBuffer, 256);
				std::wstring lwstr(TextBuffer);
				if (lwstr == L"ScrollBar")
					SetWindowTheme(*x, L"DarkMode_Explorer", L"ScrollBar");
				else if (lwstr == L"Button")
					SetWindowTheme(*x, L"DarkMode_Explorer", L"Button");
			}
		}
	}

	/* Set & update application theme */
	void SetAppTheme(std::wstring Theme = L"Light", bool OnInit = 0)
	{
		// Release current drawing objects (Brushes, icons, ..)
		if (!OnInit)
		{
			for (auto& x : Vector_StaticObjects_Brushes) DeleteObject(*x); // Static brush objects
			for (auto& x : Vector_MainObjects_Brushes) DeleteObject(*x); // Main brush objects
			for (auto& x : Vector_MainObjects_Icons) DestroyIcon(*x);	// Icon objects
			for (auto& x : Vector_Subclasses_BACaptionBar_Brushes) DeleteObject(*x); // Subclass objects
			for (auto& x : Vector_Subclasses_BAStandard_Brushes) DeleteObject(*x); // Subclass objects
			for (auto& x : Vector_Subclasses_BARadio2_Brushes) DeleteObject(*x); // Subclass objects
			for (auto& x : Vector_Subclasses_BARadio3_Brushes) DeleteObject(*x);  // Subclass objects
			for (auto& x : Vector_Subclasses_BARadio3_Miscs) DeleteObject(*x); // Subclass objects
		}

		// Update new COLORREFs and other drawing objects
		if (Theme == L"Light") // Light theme
		{
			// Static CLR
			CLR_DEBUG = RGB(0, 0, 255);
			// Main CLR
			CLR_Primary = RGB(227, 228, 228);
			CLR_Secondary = RGB(252, 253, 253);
			CLR_BorderActive = RGB(64, 64, 64);
			CLR_BorderInactive = RGB(192, 192, 192);
			CLR_DefaultTextColor = RGB(0, 0, 0);
			CLR_DefaultInactiveTextColor = RGB(162, 162, 162);
			// Subclass CLR
			CLR_CloseBtnHover = RGB(255, 89, 89);
			CLR_MinimizeBtnHover = RGB(205, 206, 206);

			// Static HBR
			hBrush_DEBUG = CreateSolidBrush(CLR_DEBUG);
			// Main HBR
			hBrush_Primary = CreateSolidBrush(CLR_Primary);
			hBrush_Secondary = CreateSolidBrush(CLR_Secondary);
			hBrush_BorderActive = CreateSolidBrush(CLR_BorderActive);
			hBrush_BorderInactive = CreateSolidBrush(CLR_BorderInactive);
			// Subclass Objects
			{
				NS_BA_CaptionBar::hBrush_Background = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_H = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_H_Close = CreateSolidBrush(CLR_CloseBtnHover);
				NS_BA_CaptionBar::hBrush_Background_H_Minimize = CreateSolidBrush(CLR_MinimizeBtnHover);
				NS_BA_CaptionBar::hBrush_Background_F = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_S = CreateSolidBrush(CLR_Primary);
			}
			{
				NS_BA_Standard::CL_Background = Color(255, 225, 225, 225); // Default color
				NS_BA_Standard::CL_Background_H = Color(255, 229, 241, 251); // Hover color
				NS_BA_Standard::CL_Background_F = Color(255, 204, 228, 247); // Down color
				NS_BA_Standard::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Standard::CL_NonFocus = Color(255, 173, 173, 173); // Default border color
				NS_BA_Standard::CL_Focus = Color(255, 181, 180, 185); // Highlight border color
				NS_BA_Standard::hBrush_Background = CreateSolidBrush(RGB(225, 225, 225)); // Default color
				NS_BA_Standard::hBrush_Background_H = CreateSolidBrush(RGB(229, 241, 251)); // Hover color
				NS_BA_Standard::hBrush_Background_F = CreateSolidBrush(RGB(204, 228, 247)); // Down color
				NS_BA_Standard::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Standard::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Standard::CLR_DefaultTextColor = RGB(0, 0, 0); // Default text color
				NS_BA_Standard::CLR_HighlightTextColor = RGB(0, 0, 0); // Highlight text color
			}
			{
				NS_BA_Radio2::CL_Background = Color(255, 58, 44, 42); // Default color
				NS_BA_Radio2::CL_Background_H = Color(255, 84, 63, 61); // Hover color
				NS_BA_Radio2::CL_Background_F = Color(255, 94, 70, 68); // Down color
				NS_BA_Radio2::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Radio2::CL_NonFocus = Color(255, 68, 50, 48); // Default border color
				NS_BA_Radio2::CL_Focus = Color(255, 181, 180, 185); // Highlight border color
				NS_BA_Radio2::hBrush_Background = CreateSolidBrush(RGB(58, 44, 42)); // Default color
				NS_BA_Radio2::hBrush_Background_H = CreateSolidBrush(RGB(84, 63, 61)); // Hover color
				NS_BA_Radio2::hBrush_Background_F = CreateSolidBrush(RGB(94, 70, 68)); // Down color
				NS_BA_Radio2::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Radio2::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Radio2::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Radio2::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}
			{
				NS_BA_Radio3::CL_Background = Color(255, 58, 44, 42); // Default color
				NS_BA_Radio3::CL_Background_H = Color(255, 84, 63, 61); // Hover color
				NS_BA_Radio3::CL_Background_F = Color(255, 94, 70, 68); // Down color
				NS_BA_Radio3::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Radio3::hPen_Background = CreatePen(PS_SOLID, NULL, RGB(32, 32, 32)); // Default border color
				NS_BA_Radio3::hBrush_Background = CreateSolidBrush(RGB(58, 44, 42)); // Default color
				NS_BA_Radio3::hBrush_Background_H = CreateSolidBrush(RGB(84, 63, 61)); // Hover color
				NS_BA_Radio3::hBrush_Background_F = CreateSolidBrush(RGB(94, 70, 68)); // Down color
				NS_BA_Radio3::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Radio3::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Radio3::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Radio3::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}

			// ICON HANDLE
			hIcon_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON4), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON4), IMAGE_ICON, 20, 20, NULL);

			APPLICATION_THEME = L"Light"; // Update theme status
			SHOW_SCROLLBAR = true;
			SetAppThemeClass(APPLICATION_THEME); // Set window theme class

			SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
				APPLICATION_WIDTH - (BORDER_WIDTH * 2) - STD_SCROLLBAR_WIDTH, // W
				APPLICATION_HEIGHT - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top), // H
				SWP_NOZORDER);
			
			{	// Show scrollbar if needed
				RECT rMCCTR; GetClientRect(SS_MAINCONTENTCTR, &rMCCTR);
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_ALL;

				if (SendMessageW(SB_MAINCONTENTCTR, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
					MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);

				if ((unsigned int)rMCCTR.bottom > (unsigned int)si.nMax)
				{
					ShowWindow(SB_MAINCONTENTCTR, SW_HIDE);
				}
				else
				{
					ShowWindow(SB_MAINCONTENTCTR, SW_SHOW);
				}
			}
		}
		else if (Theme == L"Dark") // Dark theme
		{
			// Static CLR
			CLR_DEBUG = RGB(0, 0, 255);
			// Main CLR
			CLR_Primary = RGB(0, 0, 0);
			CLR_Secondary = RGB(32, 32, 32);
			CLR_BorderActive = RGB(48, 48, 48);
			CLR_BorderInactive = RGB(57, 57, 57);
			CLR_DefaultTextColor = RGB(216, 222, 233);
			CLR_DefaultInactiveTextColor = RGB(162, 162, 162);
			// Subclass CLR
			CLR_CloseBtnHover = RGB(232, 17, 35);
			CLR_MinimizeBtnHover = RGB(57, 57, 57);

			// Static HBR
			hBrush_DEBUG = CreateSolidBrush(CLR_DEBUG);
			// Main HBR
			hBrush_Primary = CreateSolidBrush(CLR_Primary);
			hBrush_Secondary = CreateSolidBrush(CLR_Secondary);
			hBrush_BorderActive = CreateSolidBrush(CLR_BorderActive);
			hBrush_BorderInactive = CreateSolidBrush(CLR_BorderInactive);
			// Subclass Objects
			{
				NS_BA_CaptionBar::hBrush_Background = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_H = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_H_Close = CreateSolidBrush(CLR_CloseBtnHover);
				NS_BA_CaptionBar::hBrush_Background_H_Minimize = CreateSolidBrush(CLR_MinimizeBtnHover);
				NS_BA_CaptionBar::hBrush_Background_F = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_S = CreateSolidBrush(CLR_Primary);
			}
			{
				NS_BA_Standard::CL_Background = Color(255, 51, 51, 51); // Default color
				NS_BA_Standard::CL_Background_H = Color(255, 69, 69, 69); // Hover color
				NS_BA_Standard::CL_Background_F = Color(255, 102, 102, 102); // Down color
				NS_BA_Standard::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Standard::CL_NonFocus = Color(255, 155, 155, 155); // Default border color
				NS_BA_Standard::CL_Focus = Color(255, 181, 180, 185); // Highlight border color
				NS_BA_Standard::hBrush_Background = CreateSolidBrush(RGB(51, 51, 51)); // Default color
				NS_BA_Standard::hBrush_Background_H = CreateSolidBrush(RGB(69, 69, 69)); // Hover color
				NS_BA_Standard::hBrush_Background_F = CreateSolidBrush(RGB(102, 102, 102)); // Down color
				NS_BA_Standard::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Standard::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Standard::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Standard::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}
			{
				NS_BA_Radio2::CL_Background = Color(255, 58, 44, 42); // Default color
				NS_BA_Radio2::CL_Background_H = Color(255, 84, 63, 61); // Hover color
				NS_BA_Radio2::CL_Background_F = Color(255, 94, 70, 68); // Down color
				NS_BA_Radio2::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Radio2::CL_NonFocus = Color(255, 68, 50, 48); // Default border color
				NS_BA_Radio2::CL_Focus = Color(255, 181, 180, 185); // Highlight border color
				NS_BA_Radio2::hBrush_Background = CreateSolidBrush(RGB(58, 44, 42)); // Default color
				NS_BA_Radio2::hBrush_Background_H = CreateSolidBrush(RGB(84, 63, 61)); // Hover color
				NS_BA_Radio2::hBrush_Background_F = CreateSolidBrush(RGB(94, 70, 68)); // Down color
				NS_BA_Radio2::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Radio2::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Radio2::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Radio2::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}
			{
				NS_BA_Radio3::CL_Background = Color(255, 58, 44, 42); // Default color
				NS_BA_Radio3::CL_Background_H = Color(255, 84, 63, 61); // Hover color
				NS_BA_Radio3::CL_Background_F = Color(255, 94, 70, 68); // Down color
				NS_BA_Radio3::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Radio3::hPen_Background = CreatePen(PS_SOLID, NULL, RGB(32, 32, 32)); // Default border color
				NS_BA_Radio3::hBrush_Background = CreateSolidBrush(RGB(58, 44, 42)); // Default color
				NS_BA_Radio3::hBrush_Background_H = CreateSolidBrush(RGB(84, 63, 61)); // Hover color
				NS_BA_Radio3::hBrush_Background_F = CreateSolidBrush(RGB(94, 70, 68)); // Down color
				NS_BA_Radio3::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Radio3::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Radio3::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Radio3::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}

			// ICON HANDLE
			hIcon_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);

			APPLICATION_THEME = L"Dark"; // Update theme status
			SHOW_SCROLLBAR = true;
			SetAppThemeClass(APPLICATION_THEME); // Set window theme class

			SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
				APPLICATION_WIDTH - (BORDER_WIDTH * 2) - STD_SCROLLBAR_WIDTH, // W
				APPLICATION_HEIGHT - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top), // H
				SWP_NOZORDER);

			{	// Show scrollbar if needed
				RECT rMCCTR; GetClientRect(SS_MAINCONTENTCTR, &rMCCTR);
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_ALL;

				if (SendMessageW(SB_MAINCONTENTCTR, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
					MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);

				if ((unsigned int)rMCCTR.bottom > (unsigned int)si.nMax)
				{
					ShowWindow(SB_MAINCONTENTCTR, SW_HIDE);
				}
				else
				{
					ShowWindow(SB_MAINCONTENTCTR, SW_SHOW);
				}
			}
		}
		else if (Theme == L"Ristretto") // Dark obisidan theme
		{
			// Static CLR
			CLR_DEBUG = RGB(0, 0, 255);
			// Main CLR
			CLR_Primary = RGB(39, 29, 28);
			CLR_Secondary = RGB(30, 24, 23);
			CLR_BorderActive = RGB(68, 50, 48);
			CLR_BorderInactive = RGB(76, 56, 54);
			CLR_DefaultTextColor = RGB(215, 218, 222);
			CLR_DefaultInactiveTextColor = RGB(129, 115, 114);
			// Subclass CLR
			CLR_CloseBtnHover = RGB(232, 17, 35);
			CLR_MinimizeBtnHover = RGB(76, 56, 54);

			// Static HBR
			hBrush_DEBUG = CreateSolidBrush(CLR_DEBUG);
			// Main HBR
			hBrush_Primary = CreateSolidBrush(CLR_Primary);
			hBrush_Secondary = CreateSolidBrush(CLR_Secondary);
			hBrush_BorderActive = CreateSolidBrush(CLR_BorderActive);
			hBrush_BorderInactive = CreateSolidBrush(CLR_BorderInactive);
			// Subclass Objects
			{
				NS_BA_CaptionBar::hBrush_Background = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_H = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_H_Close = CreateSolidBrush(CLR_CloseBtnHover);
				NS_BA_CaptionBar::hBrush_Background_H_Minimize = CreateSolidBrush(CLR_MinimizeBtnHover);
				NS_BA_CaptionBar::hBrush_Background_F = CreateSolidBrush(CLR_Primary);
				NS_BA_CaptionBar::hBrush_Background_S = CreateSolidBrush(CLR_Primary);
			}
			{
				NS_BA_Standard::CL_Background = Color(255, 94, 70, 68); // Default color
				NS_BA_Standard::CL_Background_H = Color(255, 84, 63, 61); // Hover color
				NS_BA_Standard::CL_Background_F = Color(255, 89, 67, 65); // Down color
				NS_BA_Standard::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Standard::CL_NonFocus = Color(255, 68, 50, 48); // Default border color
				NS_BA_Standard::CL_Focus = Color(255, 181, 180, 185); // Highlight border color
				NS_BA_Standard::hBrush_Background = CreateSolidBrush(RGB(94, 70, 68)); // Default color
				NS_BA_Standard::hBrush_Background_H = CreateSolidBrush(RGB(84, 63, 61)); // Hover color
				NS_BA_Standard::hBrush_Background_F = CreateSolidBrush(RGB(89, 67, 65)); // Down color
				NS_BA_Standard::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Standard::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Standard::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Standard::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}
			{
				NS_BA_Radio2::CL_Background = Color(255, 58, 44, 42); // Default color
				NS_BA_Radio2::CL_Background_H = Color(255, 84, 63, 61); // Hover color
				NS_BA_Radio2::CL_Background_F = Color(255, 94, 70, 68); // Down color
				NS_BA_Radio2::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Radio2::CL_NonFocus = Color(255, 68, 50, 48); // Default border color
				NS_BA_Radio2::CL_Focus = Color(255, 181, 180, 185); // Highlight border color
				NS_BA_Radio2::hBrush_Background = CreateSolidBrush(RGB(58, 44, 42)); // Default color
				NS_BA_Radio2::hBrush_Background_H = CreateSolidBrush(RGB(84, 63, 61)); // Hover color
				NS_BA_Radio2::hBrush_Background_F = CreateSolidBrush(RGB(94, 70, 68)); // Down color
				NS_BA_Radio2::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Radio2::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Radio2::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Radio2::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}
			{
				NS_BA_Radio3::CL_Background = Color(255, 58, 44, 42); // Default color
				NS_BA_Radio3::CL_Background_H = Color(255, 84, 63, 61); // Hover color
				NS_BA_Radio3::CL_Background_F = Color(255, 94, 70, 68); // Down color
				NS_BA_Radio3::CL_Background_S = Color(255, 32, 32, 32); // Background color
				NS_BA_Radio3::hPen_Background = CreatePen(PS_SOLID, NULL, RGB(32, 32, 32)); // Default border color
				NS_BA_Radio3::hBrush_Background = CreateSolidBrush(RGB(58, 44, 42)); // Default color
				NS_BA_Radio3::hBrush_Background_H = CreateSolidBrush(RGB(84, 63, 61)); // Hover color
				NS_BA_Radio3::hBrush_Background_F = CreateSolidBrush(RGB(94, 70, 68)); // Down color
				NS_BA_Radio3::hBrush_Background_S = CreateSolidBrush(CLR_Secondary); // Background color
				NS_BA_Radio3::hFont_PDefault = &hFont_Default; // Font
				NS_BA_Radio3::CLR_DefaultTextColor = RGB(255, 255, 255); // Default text color
				NS_BA_Radio3::CLR_HighlightTextColor = RGB(255, 255, 255); // Highlight text color
			}

			// ICON HANDLE
			hIcon_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);

			APPLICATION_THEME = L"Ristretto"; // Update theme status
			SHOW_SCROLLBAR = false;
			SetAppThemeClass(APPLICATION_THEME); // Set window theme class
			ShowWindow(SB_MAINCONTENTCTR, SW_HIDE);

			SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
				APPLICATION_WIDTH - (BORDER_WIDTH * 2), // W
				APPLICATION_HEIGHT - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top), // H
				SWP_NOZORDER);
		}

		// Redraw application window
		if (!OnInit)
		{
			hBrush_CURBORDER = hBrush_BorderActive;
			RedrawWindow(MAIN_HWND, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
			SetActiveWindow(MAIN_HWND);
		}
	}

	/*
		WM_CREATE (1/4):
		- Init APIs (Dark mode, GDI buffered painting, GDI+)
		- Extend frame into client area (For frame shadow effects)
	*/
	bool InitBegin(HWND hWnd)
	{
		MARGINS borders = { 1,1,1,1 };
		HRESULT hr = DwmExtendFrameIntoClientArea(hWnd, &borders);
		_com_error err(hr);

		if (mSol::InitDarkModeAPI(hWnd) && mSol::InitAPI() && SUCCEEDED(hr))
			return true;
		else return false;
	}

	/*
		WM_CREATE (2/4):
		- Set application theme (Set color parameters and create drawing objects)
	*/
	bool InitTheme(HWND hWnd)
	{
		SetAppTheme(APPLICATION_THEME, true);

		if (hIcon_Close && hIcon_Close_NF && hIcon_Close_H && hIcon_Minimize && hIcon_Minimize_NF && hIcon_Minimize_H) return true;
		else return false;
	}

	/*
		WM_CREATE (3/4):
		- Create application hwnd controls
	*/
	bool InitControl(HWND hWnd)
	{
		// Close button
		BTN_Close = CreateWindowExW(NULL, L"BUTTON", L"",
			WS_CHILD | BS_OWNERDRAW, 0, 0, 58, 37, hWnd, (HMENU)BUTTON_CLOSE, NULL, NULL);
		SetWindowSubclass(BTN_Close, &SC_BA_CaptionBar, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Close);

		// Minimize button
		BTN_Minimize = CreateWindowExW(NULL, L"BUTTON", L"",
			WS_CHILD | BS_OWNERDRAW, 0, 0, 58, 37, hWnd, (HMENU)BUTTON_MINIMIZE, NULL, NULL);
		SetWindowSubclass(BTN_Minimize, &SC_BA_CaptionBar, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Minimize);

		// Window title
		WCHAR* TextBuffer_AppTitle = new WCHAR[(UINT64)GetWindowTextLengthW(hWnd) + (UINT64)1];
		GetWindowTextW(hWnd, TextBuffer_AppTitle, GetWindowTextLengthW(hWnd) + 1);
		SS_Title = CreateWindowW(L"STATIC", TextBuffer_AppTitle,
			WS_CHILD | SS_NOPREFIX | SS_LEFT, 13, 7, 300, 28, hWnd, NULL, NULL, NULL);
		delete[] TextBuffer_AppTitle;

		// Main content container (borders & caption areas excluded)
		SS_MAINCONTENTCTR = CreateWindowExW(NULL, L"STATIC", L"", WS_CHILD | SS_NOPREFIX, BORDER_WIDTH, BORDER_WIDTH + CAPTIONBAR_HEIGHT,
			0, 0, hWnd, NULL, NULL, NULL);
		SetWindowSubclass(SS_MAINCONTENTCTR, &WindowProcedure_MainContentCTR, NULL, NULL);
		Vector_Subclasses.push_back(&SS_MAINCONTENTCTR);

		// Main content container's scrollbar
		SB_MAINCONTENTCTR = mSol::CreateVerticalSB(hWnd, STD_SCROLLBAR_WIDTH, CAPTIONBAR_HEIGHT);
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nPos = 0; // Initiate scrollbar pos
			si.nMin = 0; // Min scrollbar pos
			si.nMax = 340 + 10 + (MAINCONTENTCTR_PADDING * 2); // Max scrollbar pos
			si.nPage = 211; // Actual width|height of the container
			SendMessageW(SB_MAINCONTENTCTR, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);
			Vector_MAINCONTENTCTR.push_back(&SB_MAINCONTENTCTR);
		}

		// Main content container's childs:
		SS_Heading1 = CreateWindowExW(NULL, L"STATIC", L"Sample buttons:",
			/*WS_BORDER |*/ WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
			0, 0, 155, 34, SS_MAINCONTENTCTR, NULL, NULL, NULL);
		
		BTN_Standard = CreateWindowExW(NULL, L"BUTTON", L"Standard",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 0, 0, 130, 40, SS_MAINCONTENTCTR, (HMENU)BUTTON_STANDARD, NULL, NULL);
		SetWindowSubclass(BTN_Standard, &SC_BA_Standard, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Standard);

		BTN_Radio2Left = CreateWindowEx(NULL, L"BUTTON", L"Left",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, MAINCONTENTCTR_PADDING + 140, MAINCONTENTCTR_PADDING + 44, 65, 40, SS_MAINCONTENTCTR, (HMENU)BUTTON_R2LEFT, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Radio2Left);
		SetWindowSubclass(BTN_Radio2Left, &SC_BA_Radio2, NULL, NULL);

		BTN_Radio2Right = CreateWindowEx(NULL, L"BUTTON", L"Right",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, MAINCONTENTCTR_PADDING + 206, MAINCONTENTCTR_PADDING + 44, 65, 40, SS_MAINCONTENTCTR, (HMENU)BUTTON_R2RIGHT, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Radio2Right);
		SetWindowSubclass(BTN_Radio2Right, &SC_BA_Radio2, NULL, NULL);

		BTN_Radio3Left = CreateWindowEx(NULL, L"BUTTON", L"Left",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, MAINCONTENTCTR_PADDING + 281, MAINCONTENTCTR_PADDING + 44, 65, 40, SS_MAINCONTENTCTR, (HMENU)BUTTON_R3LEFT, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Radio3Left);
		SetWindowSubclass(BTN_Radio3Left, &SC_BA_Radio3, NULL, NULL);

		BTN_Radio3Middle = CreateWindowEx(NULL, L"BUTTON", L"Middle",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, MAINCONTENTCTR_PADDING + 347, MAINCONTENTCTR_PADDING + 44, 65, 40, SS_MAINCONTENTCTR, (HMENU)BUTTON_R3MIDDLE, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Radio3Middle);
		SetWindowSubclass(BTN_Radio3Middle, &SC_BA_Radio3, NULL, NULL);

		BTN_Radio3Right = CreateWindowEx(NULL, L"BUTTON", L"Right",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, MAINCONTENTCTR_PADDING + 413, MAINCONTENTCTR_PADDING + 44, 65, 40, SS_MAINCONTENTCTR, (HMENU)BUTTON_R3RIGHT, NULL, NULL);
		Vector_Subclasses.push_back(&BTN_Radio3Right);
		SetWindowSubclass(BTN_Radio3Right, &SC_BA_Radio3, NULL, NULL);

		mSol::RemoveWindowStyle(BTN_Standard, CS_DBLCLKS); // *

		// Update theme class for controls
		SetAppThemeClass(APPLICATION_THEME);

		return true;
	}

	/*
		WM_CREATE (4/4):
		- Create font objects and apply to hwnd controls
		- Group objects, hwnd controls into vectors (Visibility control & Object deallocation)
		- Maping controls with asset objects for animations
	*/
	bool InitEnd(HWND hWnd)
	{
		// Create and apply fonts to controls
		mSol::CreateHFONT(&hFont_Default, L"Segoe UI", 24, FW_LIGHT, CLEARTYPE_QUALITY);
		mSol::CreateHFONT(&hFont_Title, L"Segoe UI", 24, FW_LIGHT, CLEARTYPE_QUALITY);
		mSol::CreateHFONT(&hFont_Heading, L"Segoe UI", 28, FW_BOLD, CLEARTYPE_QUALITY);

		EnumChildWindows(hWnd, (WNDENUMPROC)mSol::cbSetFont, (LPARAM)hFont_Default);
		SendMessageW(SS_Title, WM_SETFONT, (WPARAM)hFont_Title, TRUE);
		SendMessageW(SS_Heading1, WM_SETFONT, (WPARAM)hFont_Heading, TRUE);

		// Update vectors:
		{
			// Static and debug objects
			Vector_StaticObjects_Brushes.push_back(&hBrush_DEBUG);
		}

		{
			// Main application brush/icon/font objects
			Vector_MainObjects_Brushes.push_back(&hBrush_Primary);
			Vector_MainObjects_Brushes.push_back(&hBrush_Secondary);
			Vector_MainObjects_Brushes.push_back(&hBrush_BorderActive);
			Vector_MainObjects_Brushes.push_back(&hBrush_BorderInactive);

			Vector_MainObjects_Icons.push_back(&hIcon_Close);
			Vector_MainObjects_Icons.push_back(&hIcon_Close_NF);
			Vector_MainObjects_Icons.push_back(&hIcon_Close_H);
			Vector_MainObjects_Icons.push_back(&hIcon_Minimize);
			Vector_MainObjects_Icons.push_back(&hIcon_Minimize_NF);
			Vector_MainObjects_Icons.push_back(&hIcon_Minimize_H);

			Vector_MainObjects_Fonts.push_back(&hFont_Default);
			Vector_MainObjects_Fonts.push_back(&hFont_Title);
		}

		{
			// "Caption bar button animation subclass" objects
			Vector_Subclasses_BACaptionBar_Brushes.push_back(&NS_BA_CaptionBar::hBrush_Background);
			Vector_Subclasses_BACaptionBar_Brushes.push_back(&NS_BA_CaptionBar::hBrush_Background_H);
			Vector_Subclasses_BACaptionBar_Brushes.push_back(&NS_BA_CaptionBar::hBrush_Background_H_Close);
			Vector_Subclasses_BACaptionBar_Brushes.push_back(&NS_BA_CaptionBar::hBrush_Background_H_Minimize);
			Vector_Subclasses_BACaptionBar_Brushes.push_back(&NS_BA_CaptionBar::hBrush_Background_F);
			Vector_Subclasses_BACaptionBar_Brushes.push_back(&NS_BA_CaptionBar::hBrush_Background_S);
			// "Caption bar button animation subclass" maps
			HoverMap_1.insert(std::make_pair(std::make_pair(&BTN_Close, &hIcon_Close), std::make_pair(&hIcon_Close_H, &hIcon_Close_NF)));
			HoverMap_1.insert(std::make_pair(std::make_pair(&BTN_Minimize, &hIcon_Minimize), std::make_pair(&hIcon_Minimize_H, &hIcon_Minimize_NF)));

			// "Standard button animation subclass" objects
			Vector_Subclasses_BAStandard_Brushes.push_back(&NS_BA_Standard::hBrush_Background);
			Vector_Subclasses_BAStandard_Brushes.push_back(&NS_BA_Standard::hBrush_Background_H);
			Vector_Subclasses_BAStandard_Brushes.push_back(&NS_BA_Standard::hBrush_Background_F);
			Vector_Subclasses_BAStandard_Brushes.push_back(&NS_BA_Standard::hBrush_Background_S);

			// "Radio2 button animation subclass" objects
			Vector_Subclasses_BARadio2_Brushes.push_back(&NS_BA_Radio2::hBrush_Background);
			Vector_Subclasses_BARadio2_Brushes.push_back(&NS_BA_Radio2::hBrush_Background_H);
			Vector_Subclasses_BARadio2_Brushes.push_back(&NS_BA_Radio2::hBrush_Background_F);
			Vector_Subclasses_BARadio2_Brushes.push_back(&NS_BA_Radio2::hBrush_Background_S);

			// "Radio3 button animation subclass" objects
			Vector_Subclasses_BARadio3_Brushes.push_back(&NS_BA_Radio3::hBrush_Background);
			Vector_Subclasses_BARadio3_Brushes.push_back(&NS_BA_Radio3::hBrush_Background_H);
			Vector_Subclasses_BARadio3_Brushes.push_back(&NS_BA_Radio3::hBrush_Background_F);
			Vector_Subclasses_BARadio3_Brushes.push_back(&NS_BA_Radio3::hBrush_Background_S);
			Vector_Subclasses_BARadio3_Miscs.push_back(&NS_BA_Radio3::hPen_Background);
		}

		return true;
	}

	/*
		Execute when the application's main window is created and ready to displays
	*/
	void OnReady()
	{
		ShowWindow(MAIN_HWND, SW_NORMAL);			// Show main window
		ShowWindow(BTN_Close, SW_NORMAL);			// Show close button
		ShowWindow(BTN_Minimize, SW_NORMAL);		// Show minimize button
		ShowWindow(SS_Title, SW_NORMAL);			// Show main window title
		ShowWindow(SS_MAINCONTENTCTR, SW_NORMAL);	// Show main content container

		IS_APPREADY = true;
	}

	/*
		Execute when the application about to exit:
		- Reverse DwmExtendFrameIntoClientArea() (Disable frame shadow effects)
		- Remove WS_EX_LAYERED ex-style from main window (Bring back close animation)
	*/
	void OnExit()
	{
		DWORD exStyles = GetWindowLongW(MAIN_HWND, GWL_EXSTYLE);
		SetWindowLongW(MAIN_HWND, GWL_EXSTYLE, exStyles & ~WS_EX_LAYERED);
		MARGINS borders = { 0,0,0,0 };
		HRESULT hr = DwmExtendFrameIntoClientArea(MAIN_HWND, &borders);
	}

	/*
		Execute before the application is fully destroyed:
		- Release objects
		- Uninitialize APIs
	*/
	void OnDestroy(bool Debug = 0)
	{
		if (Debug)
		{
			unsigned short c = 0;

			// Release static objects
			for (auto& x : Vector_StaticObjects_Brushes) if (DeleteObject(*x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" static objects").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION); c = 0;

			// Release main objects
			for (auto& x : Vector_MainObjects_Brushes) if (DeleteObject(*x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" main objects (brushes)").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION); c = 0;

			for (auto& x : Vector_MainObjects_Fonts) if (DeleteObject(*x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" main objects (fonts)").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION); c = 0;
			
			for (auto& x : Vector_MainObjects_Icons) if (DestroyIcon(*x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" main objects (icons)").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION); c = 0;

			// Release subclass objects
			for (auto& x : Vector_Subclasses) if (DestroyWindow(*x)) c++;
			MessageBoxW(NULL, (L"Destroyed " + std::to_wstring(c) + L" subclasses").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION); c = 0;
		}
		else
		{
			// Release static objects
			for (auto& x : Vector_StaticObjects_Brushes) DeleteObject(*x);
			// Release main objects
			for (auto& x : Vector_MainObjects_Brushes) DeleteObject(*x);
			for (auto& x : Vector_MainObjects_Fonts) DeleteObject(*x);
			for (auto& x : Vector_MainObjects_Icons) DestroyIcon(*x);
			// Release subclass objects
			for (auto& x : Vector_Subclasses) DestroyWindow(*x);
		}

		mSol::UnInitAPI();
	}
}