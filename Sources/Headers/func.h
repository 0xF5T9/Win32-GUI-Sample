/*
	File: func.h
	Include libraries and contains most application function definitions
*/

#pragma once

#include "./c_resources.h"	// Contains application constants
#include <iostream>		// C++ Essential
#include <string>			// C++ Essential
#include <map>			// Mapping for animation objects
#include <vector>			// Group and manage objects
#include <Windows.h>		// WinAPI Essential
#include <Windowsx.h>		// WinAPI Essential
#include <Uxtheme.h>		// WinAPI Essential
#include <commctrl.h>		// WinAPI Essential
#include <Dwmapi.h>		// WinAPI Essential
#include <mmsystem.h>	// WinAPI Essential
#include <gdiplus.h>		// Use GDI+ for drawing
#include <comdef.h>		// Read results from HRESULT datatype
#include "./Ex_DarkMode.h"	// Reverse and access to undocumented window "Dark Mode" API
#include "./obj_manager.h"	// Object manager class
#include "./subclasses.h"	// Subclasses classes
#include "./global.h"		// Global variables distribution

// #pragma comment(lib, "UxTheme.lib")
// #pragma comment(lib, "Comctl32.lib")
// #pragma comment(lib, "Gdiplus.lib")
// #pragma comment(lib, "Dwmapi.lib")
// #pragma comment(lib, "Winmm.lib")

using namespace Gdiplus;

// CALLBACK FORWARD DECLARATIONS:
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WindowProcedure_MainContentCTR(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_CaptionBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_Standard(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_Radio2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_Radio3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

namespace nSol
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

	// Check if the window handle is maximized
	auto cIsMaximized(HWND hWnd) -> bool 
	{
		// SRC: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
		WINDOWPLACEMENT placement;
		placement.length = sizeof(WINDOWPLACEMENT);
		if (!::GetWindowPlacement(hWnd, &placement)) 
			return false;

		return placement.showCmd == SW_MAXIMIZE;
	}

	// Adjust maximized client rect to fit the monitor (Taskbar excluded)
	auto cAdjustMaximizedClientRect(HWND window, RECT& rect) -> void 
	{
		// SRC: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
		if (!cIsMaximized(window))
			return;

		auto monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
		if (!monitor)
			return;

		MONITORINFO monitor_info{};
		monitor_info.cbSize = sizeof(monitor_info);
		if (!::GetMonitorInfoW(monitor, &monitor_info))
			return;

		rect = monitor_info.rcWork;
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

namespace nApp
{
	/* 
		Set window theme class for standard controls (Non-Custom/Owner-Drawn Controls) 
		Ex: Button, ScrollBar
	*/
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

	/* 
		Set or update current application theme
	*/
	bool SetAppTheme(std::wstring Theme = L"Light", bool OnInit = 0)
	{
		// Update new objects
		if (Theme == L"Light")	// Light theme
		{	
			// Main application objects
			if (!OBJM_Main->UpdateColorObjects(
				Color(255, 227, 228, 228),	// Primary color
				Color(255, 252, 253, 253),	// Secondary color
				Color(255, 64, 64, 64),		// Border active color
				Color(255, 192, 192, 192),	// Border inactive color
				Color(255, 0, 0, 0),			// Default text color
				Color(255, 162, 162, 162),	// Inactive text color
				Color(255, 0, 0, 255)))		// Debug color
				return false;
			if (!OnInit) if (!OBJM_Main->DestroyMediaObjects()) return false;
			OBJM_Main->HICO_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Close_Hover = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Close_Inactive = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON4), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize_Hover = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON4), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize_Inactive = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);

			// Subclass objects
			BA_CaptionBar_Manager->UpdateObjects(OBJM_Main->CLR_Primary, OBJM_Main->CLR_Primary, RGB(255, 89, 89), RGB(205, 206, 206), OBJM_Main->CLR_Primary,
				OBJM_Main->CLR_Primary);
			BA_Standard_Manager->UpdateObjects(Color(255, 225, 225, 225), Color(255, 229, 241, 251), Color(255, 204, 228, 247), Color(255, 252, 253, 253), Color(255, 173, 173, 173), 
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(0, 0, 0), RGB(0, 0, 0));
			BA_Radio2_Manager->UpdateObjects(Color(255, 225, 225, 225), Color(255, 229, 241, 251), Color(255, 204, 228, 247), Color(255, 252, 253, 253), Color(255, 173, 173, 173),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(0, 0, 0), RGB(0, 0, 0));
			BA_Radio3_Manager->UpdateObjects(Color(255, 225, 225, 225), Color(255, 229, 241, 251), Color(255, 204, 228, 247), Color(255, 252, 253, 253), Color(255, 173, 173, 173),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(0, 0, 0), RGB(0, 0, 0));

			APPLICATION_THEME = L"Light";
			IS_APPTHEMESHOWSCROLLBAR = true;
			SetAppThemeClass(APPLICATION_THEME);

			SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
				APPLICATION_WIDTH - (BORDER_WIDTH * 2) - STD_SCROLLBAR_WIDTH,					// W
				APPLICATION_HEIGHT - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top),	// H
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
		else if (Theme == L"Dark")	// Dark theme
		{
			// Main application objects
			if (!OBJM_Main->UpdateColorObjects(
				Color(255, 0, 0, 0),			// Primary color
				Color(255, 32, 32, 32),		// Secondary color
				Color(255, 48, 48, 48),		// Border active color
				Color(255, 57, 57, 57),		// Border inactive color
				Color(255, 216, 222, 233),	// Default text color
				Color(255, 162, 162, 162),	// Inactive text color
				Color(255, 0, 0, 255)))		// Debug color
				return false;
			if (!OnInit) if (!OBJM_Main->DestroyMediaObjects()) return false;
			OBJM_Main->HICO_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Close_Hover = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Close_Inactive = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize_Hover = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize_Inactive = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);

			// Subclass objects
			BA_CaptionBar_Manager->UpdateObjects(OBJM_Main->CLR_Primary, OBJM_Main->CLR_Primary, RGB(232, 17, 35), RGB(57, 57, 57), OBJM_Main->CLR_Primary,
				OBJM_Main->CLR_Primary);
			BA_Standard_Manager->UpdateObjects(Color(255, 51, 51, 51), Color(255, 69, 69, 69), Color(255, 102, 102, 102), Color(255, 32, 32, 32), Color(255, 155, 155, 155),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(255, 255, 255), RGB(255, 255, 255));
			BA_Radio2_Manager->UpdateObjects(Color(255, 51, 51, 51), Color(255, 69, 69, 69), Color(255, 102, 102, 102), Color(255, 32, 32, 32), Color(255, 155, 155, 155),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(255, 255, 255), RGB(255, 255, 255));
			BA_Radio3_Manager->UpdateObjects(Color(255, 51, 51, 51), Color(255, 69, 69, 69), Color(255, 102, 102, 102), Color(255, 32, 32, 32), Color(255, 155, 155, 155),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(255, 255, 255), RGB(255, 255, 255));

			APPLICATION_THEME = L"Dark";
			IS_APPTHEMESHOWSCROLLBAR = true;
			SetAppThemeClass(APPLICATION_THEME);

			SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
				APPLICATION_WIDTH - (BORDER_WIDTH * 2) - STD_SCROLLBAR_WIDTH,					// W
				APPLICATION_HEIGHT - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top),	// H
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
		else if (Theme == L"Ristretto")
		{
			// Main application objects
			if (!OBJM_Main->UpdateColorObjects(
				Color(255, 39, 29, 28),		// Primary color
				Color(255, 30, 24, 23),		// Secondary color
				Color(255, 68, 50, 48),		// Border active color
				Color(255, 76, 56, 54),		// Border inactive color
				Color(255, 215, 218, 222),	// Default text color
				Color(255, 129, 115, 114),		// Inactive text color
				Color(255, 0, 0, 255)))		// Debug color
				return false;
			if (!OnInit) if (!OBJM_Main->DestroyMediaObjects()) return false;
			OBJM_Main->HICO_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Close_Hover = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Close_Inactive = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize_Hover = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			OBJM_Main->HICO_Minimize_Inactive = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);

			// Subclass objects
			BA_CaptionBar_Manager->UpdateObjects(OBJM_Main->CLR_Primary, OBJM_Main->CLR_Primary, RGB(232, 17, 35), RGB(76, 56, 54), OBJM_Main->CLR_Primary,
				OBJM_Main->CLR_Primary);
			BA_Standard_Manager->UpdateObjects(Color(255, 94, 70, 68), Color(255, 84, 63, 61), Color(255, 89, 67, 65), Color(255, 32, 32, 32), Color(255, 68, 50, 48),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(255, 255, 255), RGB(255, 255, 255));
			BA_Radio2_Manager->UpdateObjects(Color(255, 58, 44, 42), Color(255, 84, 63, 61), Color(255, 94, 70, 68), Color(255, 32, 32, 32), Color(255, 68, 50, 48),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(255, 255, 255), RGB(255, 255, 255));
			BA_Radio3_Manager->UpdateObjects(Color(255, 58, 44, 42), Color(255, 84, 63, 61), Color(255, 94, 70, 68), Color(255, 32, 32, 32), Color(255, 68, 50, 48),
				Color(255, 181, 180, 185), OBJM_Main->HFO_Default, RGB(255, 255, 255), RGB(255, 255, 255));

			APPLICATION_THEME = L"Ristretto"; 
			IS_APPTHEMESHOWSCROLLBAR = false;
			SetAppThemeClass(APPLICATION_THEME);
			ShowWindow(SB_MAINCONTENTCTR, SW_HIDE);

			SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
				APPLICATION_WIDTH - (BORDER_WIDTH * 2),											// W
				APPLICATION_HEIGHT - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top),		// H
				SWP_NOZORDER);
		}

		// Redraw entire application window
		if (!OnInit)
		{
			OBJM_Main->HBRP_CURRENTBORDER = &OBJM_Main->HBR_BorderActive;
			RedrawWindow(MAIN_HWND, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
			SetActiveWindow(MAIN_HWND);
		}
		
		return true;
	}

	/*
		WM_CREATE (1/4):
		- Init global class objects from pointers
		- Init APIs (Dark mode, GDI buffered painting, GDI+)
		- Extend frame into client area (For frame shadow effects)
		- Set minimum resolution for periodic timers
	*/
	bool InitBegin(HWND hWnd)
	{
		// Initialize global objects
		OBJM_Main = new OBJ_Manager();
		BA_CaptionBar_Manager = new BA_CaptionBar();
		BA_CaptionBar_Manager->InsertAnimationMap(BTN_Close, OBJM_Main->HICO_Close, OBJM_Main->HICO_Close_Hover, OBJM_Main->HICO_Close_Inactive);
		BA_CaptionBar_Manager->InsertAnimationMap(BTN_Minimize, OBJM_Main->HICO_Minimize, OBJM_Main->HICO_Minimize_Hover, OBJM_Main->HICO_Minimize_Inactive);
		BA_Standard_Manager = new BA_Standard();
		BA_Radio2_Manager = new BA_Radio2();
		BA_Radio3_Manager = new BA_Radio3();
		if (!OBJM_Main || !BA_CaptionBar_Manager || !BA_Standard_Manager || !BA_Radio2_Manager || !BA_Radio3_Manager)
		{
			MessageBoxW(NULL, L"Error occurred!\n(Failed to initialize global objects)", L"", MB_OK | MB_ICONERROR);
			return false;
		}

		// Initialize DarkMode API
		if (!nSol::InitDarkModeAPI(hWnd))
		{
			MessageBoxW(NULL, L"Error occurred!\n(Failed to initialize DarkMode API)", L"", MB_OK | MB_ICONERROR);
			return false;
		}

		// Initialize GDI Animation & GDI+ APIs
		if (!nSol::InitAPI())
		{
			MessageBoxW(NULL, L"Error occurred!\n(Failed to initialize GDI Animation & GDI+ APIs)", L"", MB_OK | MB_ICONERROR);
			return false;
		}

		// Extend frame into client area
		MARGINS borders = { 1,1,1,1 };
		HRESULT hr = DwmExtendFrameIntoClientArea(hWnd, &borders);
		_com_error err(hr);
		if (!SUCCEEDED(hr))
		{
			MessageBoxW(NULL, L"Error occurred!\n(Failed to execute DwmExtendFrameIntoClientArea)", L"", MB_OK | MB_ICONERROR);
			return false;
		}

		// Set minimum resolution for periodic timers
		if (timeBeginPeriod(15) != TIMERR_NOERROR)
		{
			MessageBoxW(NULL, L"Error occurred!\n(Failed to set minimum resolution for periodic timers)", L"", MB_OK | MB_ICONERROR);
			return false;
		}

		return true;
	}

	/*
		WM_CREATE (2/4):
		- Set application theme
	*/
	bool InitTheme(HWND hWnd)
	{
		if (!SetAppTheme(APPLICATION_THEME, true))
		{
			MessageBoxW(NULL, L"Error occurred!\n(Failed to set application theme)", L"", MB_OK | MB_ICONERROR);
			return false;
		}

		return true;
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
		SB_MAINCONTENTCTR = nSol::CreateVerticalSB(hWnd, STD_SCROLLBAR_WIDTH, CAPTIONBAR_HEIGHT);
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nPos = 0; // Initiate scrollbar pos
			si.nMin = 0; // Min scrollbar pos
			si.nMax = 74 + 10 + (MAINCONTENTCTR_PADDING * 2); // Max scrollbar pos = (total content sizes + spaces between contents + paddings)
			si.nPage = 211; // Actual width|height of the container
			SendMessageW(SB_MAINCONTENTCTR, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);
			Vector_MAINCONTENTCTR.push_back(&SB_MAINCONTENTCTR);
		}

		// Main content container's childs:
		SS_Heading1 = CreateWindowExW(NULL, L"STATIC", L"Sample buttons:",
			WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
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

		nSol::RemoveWindowStyle(BTN_Standard, CS_DBLCLKS);

		// Update theme class for controls
		SetAppThemeClass(APPLICATION_THEME);

		return true;
	}

	/*
		WM_CREATE (4/4):
		- Apply font to window handles
	*/
	bool InitEnd(HWND hWnd)
	{
		// Apply font to window handles
		EnumChildWindows(hWnd, (WNDENUMPROC)nSol::cbSetFont, (LPARAM)OBJM_Main->HFO_Default);
		SendMessageW(SS_Title, WM_SETFONT, (WPARAM)OBJM_Main->HFO_Title, TRUE);
		SendMessageW(SS_Heading1, WM_SETFONT, (WPARAM)OBJM_Main->HFO_Heading, TRUE);

		return true;
	}

	/*
		Execute when the application's main window is created and ready to displays
	*/
	void OnReady()
	{
		ShowWindow(MAIN_HWND, SW_NORMAL);			// Show main window
		ShowWindow(BTN_Close, SW_NORMAL);			// Show close button
		ShowWindow(BTN_Minimize, SW_NORMAL);			// Show minimize button
		ShowWindow(SS_Title, SW_NORMAL);				// Show main window title
		ShowWindow(SS_MAINCONTENTCTR, SW_NORMAL);	// Show main content container

		IS_APPREADY = true;
	}

	/*
		Execute when the application about to exit:
		- Reverse DwmExtendFrameIntoClientArea() (Disable frame shadow effects)
		- Remove WS_EX_LAYERED ex-style from main window (Bring back close animation from visual styles)
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
		- Destroy all objects
		- Uninitialize APIs
		- Clears previously set minimum timer resolution
	*/
	void OnDestroy(bool Debug = 0)
	{
		if (Debug)
		{
			unsigned short c = 0;

			for (auto& x : Vector_Subclasses) if (DestroyWindow(*x)) c++;
			MessageBoxW(NULL, (L"Destroyed " + std::to_wstring(c) + L" subclasses").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION); c = 0;
		}
		else
		{
			for (auto& x : Vector_Subclasses) DestroyWindow(*x);
		}

		delete BA_Radio2_Manager;
		delete BA_Radio3_Manager;
		delete BA_Standard_Manager;
		delete BA_CaptionBar_Manager;
		delete OBJM_Main;
		nSol::UnInitAPI();
		timeEndPeriod(15);
	}
}