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
namespace NS_BA_Main
{
	extern HBRUSH hBrush_Background,
		hBrush_Background_H,
		hBrush_Background_H_Close,
		hBrush_Background_H_Minimize,
		hBrush_Background_F,
		hBrush_Background_S;
};

// CALLBACK FORWARD DECLARATIONS:
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WindowProcedure_MainContentCTR(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK SC_BA_Main(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

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

	// Quick-create winapi font object
	HFONT CreateHFONT(std::wstring fName, int fSize)
	{
		HFONT ret_hFONT = CreateFontW(fSize, 0, 0, 0x1,
			FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, (LPCWSTR)fName.c_str());

		return ret_hFONT;
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
		else if (Theme == L"Dark" || Theme == L"Dark_Obisidan")
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

	/* Set & Update application theme */
	void SetAppTheme(std::wstring Theme = L"Light", bool OnInit = 0)
	{
		// Release objects and clear vectors
		if (!OnInit)
		{
			for (auto& x : Vector_StaticObjects_Brushes) DeleteObject(x); // Static objects
			for (auto& x : Vector_MainObjects_Brushes) DeleteObject(x); // Main objects

			// Subclass objects
			DeleteObject(NS_BA_Main::hBrush_Background);
			DeleteObject(NS_BA_Main::hBrush_Background_H);
			DeleteObject(NS_BA_Main::hBrush_Background_H_Close);
			DeleteObject(NS_BA_Main::hBrush_Background_H_Minimize);
			DeleteObject(NS_BA_Main::hBrush_Background_F);
			DeleteObject(NS_BA_Main::hBrush_Background_S);

			// Icon objects
			for (auto& x : Vector_MainObjects_Icons) DestroyIcon(x);

			// Clear vectors as original elements has become invalid
			Vector_StaticObjects_Brushes.clear();
			Vector_MainObjects_Brushes.clear();
			Vector_MainObjects_Icons.clear();
			HoverMap_1.clear();
		}

		// Update COLORREFs, HBRUSHs
		if (Theme == L"Light")
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
			// Subclass HBR
			NS_BA_Main::hBrush_Background = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_H = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_H_Close = CreateSolidBrush(CLR_CloseBtnHover);
			NS_BA_Main::hBrush_Background_H_Minimize = CreateSolidBrush(CLR_MinimizeBtnHover);
			NS_BA_Main::hBrush_Background_F = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_S = CreateSolidBrush(CLR_Primary);

			// ICON HANDLE
			/*
			hIcon_Close = (HICON)LoadImageW(NULL, L"./Assets/cross_black.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Close_NF = (HICON)LoadImageW(NULL, L"./Assets/cross_grey.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Close_H = (HICON)LoadImageW(NULL, L"./Assets/cross_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize = (HICON)LoadImageW(NULL, L"./Assets/minus_black.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize_NF = (HICON)LoadImageW(NULL, L"./Assets/minus_grey.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize_H = (HICON)LoadImageW(NULL, L"./Assets/minus_black.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			*/
			hIcon_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON4), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON4), IMAGE_ICON, 20, 20, NULL);

			APPLICATION_THEME = L"Light"; // Update theme status
			SetAppThemeClass(APPLICATION_THEME); // Set window theme class
		}
		else if (Theme == L"Dark")
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
			// Subclass HBR
			NS_BA_Main::hBrush_Background = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_H = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_H_Close = CreateSolidBrush(CLR_CloseBtnHover);
			NS_BA_Main::hBrush_Background_H_Minimize = CreateSolidBrush(CLR_MinimizeBtnHover);
			NS_BA_Main::hBrush_Background_F = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_S = CreateSolidBrush(CLR_Primary);

			// ICON HANDLE
			/*
			hIcon_Close = (HICON)LoadImageW(NULL, L"./Assets/cross_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Close_NF = (HICON)LoadImageW(NULL, L"./Assets/cross_grey.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Close_H = (HICON)LoadImageW(NULL, L"./Assets/cross_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize = (HICON)LoadImageW(NULL, L"./Assets/minus_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize_NF = (HICON)LoadImageW(NULL, L"./Assets/minus_grey.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize_H = (HICON)LoadImageW(NULL, L"./Assets/minus_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			*/
			hIcon_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);

			APPLICATION_THEME = L"Dark"; // Update theme status
			SetAppThemeClass(APPLICATION_THEME); // Set window theme class
		}
		else if (Theme == L"Dark_Obisidan")
		{
			// Static CLR
			CLR_DEBUG = RGB(0, 0, 255);
			// Main CLR
			CLR_Primary = RGB(29, 34, 39);
			CLR_Secondary = RGB(48, 56, 65);
			CLR_BorderActive = RGB(45, 50, 54);
			CLR_BorderInactive = RGB(51, 56, 60);
			CLR_DefaultTextColor = RGB(216, 222, 233);
			CLR_DefaultInactiveTextColor = RGB(162, 162, 162);
			// Subclass CLR
			CLR_CloseBtnHover = RGB(232, 17, 35);
			CLR_MinimizeBtnHover = RGB(51, 56, 60);

			// Static HBR
			hBrush_DEBUG = CreateSolidBrush(CLR_DEBUG);
			// Main HBR
			hBrush_Primary = CreateSolidBrush(CLR_Primary);
			hBrush_Secondary = CreateSolidBrush(CLR_Secondary);
			hBrush_BorderActive = CreateSolidBrush(CLR_BorderActive);
			hBrush_BorderInactive = CreateSolidBrush(CLR_BorderInactive);
			// Subclass HBR
			NS_BA_Main::hBrush_Background = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_H = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_H_Close = CreateSolidBrush(CLR_CloseBtnHover);
			NS_BA_Main::hBrush_Background_H_Minimize = CreateSolidBrush(CLR_MinimizeBtnHover);
			NS_BA_Main::hBrush_Background_F = CreateSolidBrush(CLR_Primary);
			NS_BA_Main::hBrush_Background_S = CreateSolidBrush(CLR_Primary);

			// ICON HANDLE
			/*
			hIcon_Close = (HICON)LoadImageW(NULL, L"./Assets/cross_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Close_NF = (HICON)LoadImageW(NULL, L"./Assets/cross_grey.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Close_H = (HICON)LoadImageW(NULL, L"./Assets/cross_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize = (HICON)LoadImageW(NULL, L"./Assets/minus_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize_NF = (HICON)LoadImageW(NULL, L"./Assets/minus_grey.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			hIcon_Minimize_H = (HICON)LoadImageW(NULL, L"./Assets/minus_white.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			*/
			hIcon_Close = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON2), IMAGE_ICON, 20, 20, NULL);
			hIcon_Close_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_NF = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON5), IMAGE_ICON, 20, 20, NULL);
			hIcon_Minimize_H = (HICON)LoadImageW(MAIN_HINSTANCE, MAKEINTRESOURCEW(IDI_ICON6), IMAGE_ICON, 20, 20, NULL);

			APPLICATION_THEME = L"Dark_Obisidan"; // Update theme status
			SetAppThemeClass(APPLICATION_THEME); // Set window theme class
		}

		// Update vectors
		if (!OnInit)
		{
			Vector_StaticObjects_Brushes.push_back(hBrush_DEBUG);
			Vector_MainObjects_Brushes.push_back(hBrush_Primary);
			Vector_MainObjects_Brushes.push_back(hBrush_Secondary);
			Vector_MainObjects_Brushes.push_back(hBrush_BorderActive);
			Vector_MainObjects_Brushes.push_back(hBrush_BorderInactive);
			Vector_MainObjects_Icons.push_back(hIcon_Close);
			Vector_MainObjects_Icons.push_back(hIcon_Close_NF);
			Vector_MainObjects_Icons.push_back(hIcon_Close_H);
			Vector_MainObjects_Icons.push_back(hIcon_Minimize);
			Vector_MainObjects_Icons.push_back(hIcon_Minimize_NF);
			Vector_MainObjects_Icons.push_back(hIcon_Minimize_H);
			HoverMap_1.insert(std::make_pair(std::make_pair(BTN_Close, hIcon_Close), std::make_pair(hIcon_Close_H, hIcon_Close_NF)));
			HoverMap_1.insert(std::make_pair(std::make_pair(BTN_Minimize, hIcon_Minimize), std::make_pair(hIcon_Minimize_H, hIcon_Minimize_NF)));
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
		Stage 1 WM_CREATE:
		- Init APIs
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
		Stage 2 WM_CREATE:
		- Set application theme (Set colors, load assets, ..)
	*/
	bool InitTheme(HWND hWnd)
	{
		SetAppTheme(APPLICATION_THEME, true);

		if (hIcon_Close && hIcon_Close_NF && hIcon_Close_H && hIcon_Minimize && hIcon_Minimize_NF && hIcon_Minimize_H) return true;
		else return false;
	}

	/*
		Stage 3 WM_CREATE:
		- Create child controls
	*/
	bool InitControl(HWND hWnd)
	{
		// Close button
		BTN_Close = CreateWindowExW(NULL, L"BUTTON", L"",
			WS_CHILD | BS_OWNERDRAW, 0, 0, 58, 36, hWnd, (HMENU)BUTTON_CLOSE, NULL, NULL);
		SetWindowSubclass(BTN_Close, &SC_BA_Main, NULL, NULL);
		Vector_Subclasses.push_back(BTN_Close);

		// Minimize button
		BTN_Minimize = CreateWindowExW(NULL, L"BUTTON", L"",
			WS_CHILD | BS_OWNERDRAW, 0, 0, 58, 36, hWnd, (HMENU)BUTTON_MINIMIZE, NULL, NULL);
		SetWindowSubclass(BTN_Minimize, &SC_BA_Main, NULL, NULL);
		Vector_Subclasses.push_back(BTN_Minimize);

		// Window title
		WCHAR* TextBuffer_AppTitle = new WCHAR[(UINT64)GetWindowTextLengthW(hWnd) + (UINT64)1];
		GetWindowTextW(hWnd, TextBuffer_AppTitle, GetWindowTextLengthW(hWnd) + 1);
		SS_Title = CreateWindowW(L"STATIC", TextBuffer_AppTitle,
			WS_CHILD | SS_NOPREFIX | SS_LEFT, 13, 6, 300, 31, hWnd, NULL, NULL, NULL);
		delete[] TextBuffer_AppTitle;

		// Main content container (view port: borders & caption areas excluded)
		SS_MAINCONTENTCTR = CreateWindowExW(NULL, L"STATIC", L"", WS_CHILD | SS_NOPREFIX, BORDER_WIDTH, CAPTIONBAR_HEIGHT,
			0, 0, hWnd, NULL, NULL, NULL);
		SetWindowSubclass(SS_MAINCONTENTCTR, &WindowProcedure_MainContentCTR, NULL, NULL);
		Vector_Subclasses.push_back(SS_MAINCONTENTCTR);

		// Main content container's scrollbar
		SB_MAINCONTENTCTR = mSol::CreateVerticalSB(hWnd, STD_SCROLLBAR_WIDTH, CAPTIONBAR_HEIGHT);
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nPos = 0; // Initiate scrollbar pos
			si.nMin = 0; // Min scrollbar pos
			si.nMax = 340 + 10 + (MAINCONTENTCTR_PADDING * 2); // Max scrollbar pos
			si.nPage = 213; // Actual width|height of the container
			SendMessageW(SB_MAINCONTENTCTR, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);
			Vector_MAINCONTENTCTR.push_back(&SB_MAINCONTENTCTR);
		}

		// Main content container's childs:
		SS_Test1 = CreateWindowExW(NULL, L"STATIC", L"Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.",
			WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
			0, 0, 300, 300, SS_MAINCONTENTCTR, NULL, NULL, NULL);
		BTN_Test1 = CreateWindowExW(NULL, L"BUTTON", L"Lorem Ipsum",
			WS_VISIBLE | WS_CHILD, 0, 0, 130, 40, SS_MAINCONTENTCTR, (HMENU)BUTTON_LOREMIPSUM, NULL, NULL);
		Vector_MAINCONTENTCTR.push_back(&BTN_Test1);

		// Update theme class for controls
		SetAppThemeClass(APPLICATION_THEME);

		return true;
	}

	/*
		Stage 4 WM_CREATE
		- Create font objects and apply to child controls
		- Group objects, controls into vectors (Visibility control & Object deallocation)
		- Maping controls with asset objects for animations
	*/
	bool InitEnd(HWND hWnd)
	{
		// Create and apply fonts to controls
		hFont_Title = mSol::CreateHFONT(L"Segoe UI Light", 27);
		SendMessageW(SS_Title, WM_SETFONT, (WPARAM)hFont_Title, TRUE);
		SendMessageW(SS_Test1, WM_SETFONT, (WPARAM)hFont_Title, TRUE);
		SendMessageW(BTN_Test1, WM_SETFONT, (WPARAM)hFont_Title, TRUE);

		// Update vectors
		Vector_StaticObjects_Brushes.push_back(hBrush_DEBUG);

		Vector_MainObjects_Brushes.push_back(hBrush_Primary);
		Vector_MainObjects_Brushes.push_back(hBrush_Secondary);
		Vector_MainObjects_Brushes.push_back(hBrush_BorderActive);
		Vector_MainObjects_Brushes.push_back(hBrush_BorderInactive);

		Vector_MainObjects_Icons.push_back(hIcon_Close);
		Vector_MainObjects_Icons.push_back(hIcon_Close_NF);
		Vector_MainObjects_Icons.push_back(hIcon_Close_H);
		Vector_MainObjects_Icons.push_back(hIcon_Minimize);
		Vector_MainObjects_Icons.push_back(hIcon_Minimize_NF);
		Vector_MainObjects_Icons.push_back(hIcon_Minimize_H);

		Vector_MainObjects_Fonts.push_back(hFont_Default);
		Vector_MainObjects_Fonts.push_back(hFont_Title);

		// Update maps
		HoverMap_1.insert(std::make_pair(std::make_pair(BTN_Close, hIcon_Close), std::make_pair(hIcon_Close_H, hIcon_Close_NF)));
		HoverMap_1.insert(std::make_pair(std::make_pair(BTN_Minimize, hIcon_Minimize), std::make_pair(hIcon_Minimize_H, hIcon_Minimize_NF)));

		return true;
	}

	/*
		Execute when the application's main window is fully created:
		- Show main window, close & minimize buttons, main window's title, main content container.
	*/
	void OnReady()
	{
		ShowWindow(MAIN_HWND, SW_NORMAL);
		ShowWindow(BTN_Close, SW_NORMAL);
		ShowWindow(BTN_Minimize, SW_NORMAL);
		ShowWindow(SS_Title, SW_NORMAL);
		ShowWindow(SS_MAINCONTENTCTR, SW_NORMAL);

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
			int c = 0;

			// Release static objects
			for (auto& x : Vector_StaticObjects_Brushes)
				if (DeleteObject(x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" static objects").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION);
			c = 0;

			// Release main objects
			for (auto& x : Vector_MainObjects_Brushes)
				if (DeleteObject(x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" main objects (brushes)").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION);
			c = 0;
			for (auto& x : Vector_MainObjects_Fonts)
				if (DeleteObject(x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" main objects (fonts)").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION);
			c = 0;
			for (auto& x : Vector_MainObjects_Icons)
				if (DestroyIcon(x)) c++;
			MessageBoxW(NULL, (L"Deleted " + std::to_wstring(c) + L" main objects (icons)").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION);
			c = 0;

			// Release subclass objects
			for (auto& x : Vector_Subclasses)
				if (DestroyWindow(x)) c++;
			MessageBoxW(NULL, (L"Destroyed " + std::to_wstring(c) + L" subclasses").c_str(), L"DEBUG", MB_OK | MB_ICONINFORMATION);
			c = 0;
		}
		else
		{
			// Release static objects
			for (auto& x : Vector_StaticObjects_Brushes) DeleteObject(x);
			// Release main objects
			for (auto& x : Vector_MainObjects_Brushes) DeleteObject(x);
			for (auto& x : Vector_MainObjects_Fonts) DeleteObject(x);
			for (auto& x : Vector_MainObjects_Icons) DestroyIcon(x);
			// Release subclass objects
			for (auto& x : Vector_Subclasses) DestroyWindow(x);
		}

		mSol::UnInitAPI();
	}
}