/*
	Win32 GUI Sample
*/

#include "./Headers/func.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	if (FindWindowW(WndClassName, NULL) != NULL)	// Terminate if application already running
	{
		MessageBoxW(NULL, L"Application is already running", L"", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	// Create custom window class
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = NULL;	// Manually handle "WM_ERASEBKGND" (reduce flickering)
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON7));
	wc.hInstance = hInstance;
	wc.lpszClassName = WndClassName;
	wc.lpfnWndProc = WindowProcedure;

	MAIN_HINSTANCE = hInstance;

	if (!RegisterClassW(&wc))	// Register window class
	{
		MessageBoxW(NULL, L"Error occurred!\n(Failed to register window class)", L"", MB_OK | MB_ICONERROR);
		return -1;
	}

	int DesktopWidth = 0, DesktopHeight = 0; nSol::GetDesktopResolution(DesktopWidth, DesktopHeight);	// Get user desktop resolution
	MAIN_HWND = CreateWindowExW(WS_EX_LAYERED, WndClassName, L"Win32 GUI Sample", WS_MYSTYLE,
		(DesktopWidth / 2) - (int)((double)APPLICATION_WIDTH / 1.4), (DesktopHeight / 2) - (int)((double)APPLICATION_HEIGHT / 1.4),	// Semi-center application on start
		APPLICATION_WIDTH, APPLICATION_HEIGHT, // Initial application window size
		NULL, NULL, hInstance, NULL);
	SetLayeredWindowAttributes(MAIN_HWND, RGB(141, 172, 160), 255, LWA_ALPHA | LWA_COLORKEY);	// Set transparency color (Make MAIN_HWND compability with WS_MYSTYLE, otherwise MAIN_HWND won't be visible)

	nApp::OnReady();	// Execute when the application's main window is created and ready to displays

	// Begin message loop
	MSG msg = { 0 };
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		if (IsDialogMessageW(MAIN_HWND, &msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			// Initialize main window
			if (nApp::InitBegin(hWnd) && nApp::InitTheme(hWnd) && nApp::InitControl(hWnd) && nApp::InitEnd(hWnd))
				return (LRESULT)0;
			else { MessageBoxW(NULL, L"Error occurred!\n(Failed to initialize main window)", L"", MB_OK | MB_ICONERROR); exit(1); }
		}

		case WM_COMMAND:
		{
			// Process WM_COMMAND messages

			switch (wp)
			{
				case BUTTON_CLOSE:
					nApp::OnExit();
					DestroyWindow(hWnd);
					return (LRESULT)0;

				case BUTTON_MINIMIZE:
					ShowWindow(hWnd, SW_SHOWMINIMIZED);
					return (LRESULT)0;

				default:
					break;
			}

			break;
		}

		case WM_PAINT:
		{
			// Painting main window

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			// Create DCs & BMPs for double buffering (reduce flickering)
			HDC mem_hdc = CreateCompatibleDC(hdc);
			static RECT rAppClient; GetClientRect(hWnd, &rAppClient);
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc,
				rAppClient.right - rAppClient.left,
				rAppClient.bottom - rAppClient.top);
			HBITMAP hBitmap_Old = (HBITMAP)SelectObject(mem_hdc, hBitmap);

			// Set GDI+ Smoothing Mode
			Graphics graphics(mem_hdc);
			graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

			// Begin painting to memory DC
			FillRect(mem_hdc, &rAppClient, OBJM_Main->HBR_Secondary);	// Main background color
			FillRect(mem_hdc, &RECT_Caption, OBJM_Main->HBR_Primary);	// Caption bar color
			static RECT rFrames_Paint; GetClientRect(hWnd, &rFrames_Paint);
			FrameRect(mem_hdc, &rFrames_Paint, *OBJM_Main->HBRP_CURRENTBORDER);	// Border color

			// Draw contents from memory DC to target DC
			BitBlt(hdc, 0, 0, rAppClient.right - rAppClient.left,
				rAppClient.bottom - rAppClient.top, mem_hdc, 0, 0, SRCCOPY);

			// Release DCs
			SelectObject(mem_hdc, hBitmap_Old);
			DeleteObject(hBitmap);
			DeleteDC(mem_hdc);

			EndPaint(hWnd, &ps);
			return (LRESULT)0;
		}

		case WM_ERASEBKGND:
		{
			// Manually handle "WM_ERASEBKGND" (reduce flickering)
			return (LRESULT)1;
		}

		case WM_CTLCOLORSTATIC:
		{
			// Handling static control & read-only edit control messages

			static HBRUSH RET_CTLCOLORSTATIC = (HBRUSH)GetStockObject(BLACK_BRUSH);

			if ((HWND)lp == SS_Title)
			{
				SetBkMode((HDC)wp, TRANSPARENT);
				SetBkColor((HDC)wp, OBJM_Main->CLR_Primary);
				if (GetActiveWindow() == hWnd) SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
				else SetTextColor((HDC)wp, OBJM_Main->CLR_InactiveText);
				RET_CTLCOLORSTATIC = OBJM_Main->HBR_Primary;
			}
			else if ((HWND)lp == SS_MAINCONTENTCTR)
			{
				static bool OnFirst = 1;
				if (OnFirst)
				{
					SetBkMode((HDC)wp, TRANSPARENT);
					SetBkColor((HDC)wp, OBJM_Main->CLR_Secondary);
					SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
					OnFirst = 0;
				}
				RET_CTLCOLORSTATIC = OBJM_Main->HBR_Secondary;
			}
			else RET_CTLCOLORSTATIC = OBJM_Main->HBR_DEBUG; // Apply debug color to non-handled static controls

			return (LRESULT)RET_CTLCOLORSTATIC;
		}

		case WM_CTLCOLORBTN:
		{
			// Handling button control messages

			static HBRUSH RET_CTLCOLORBTN = (HBRUSH)GetStockObject(BLACK_BRUSH);

			if ((HWND)lp == BTN_Close || (HWND)lp == BTN_Minimize)
				RET_CTLCOLORBTN = OBJM_Main->HBR_Primary;
			else RET_CTLCOLORBTN = OBJM_Main->HBR_DEBUG; // Apply debug color to non-handled button controls

			return (LRESULT)RET_CTLCOLORBTN;
		}

		case WM_SETFOCUS:
		{
			break;
		}

		case WM_KILLFOCUS:
		{
			break;
		}

		case WM_ACTIVATE:
		{
			// Update window border colors on active state changes

			switch (wp)
			{
				case WA_CLICKACTIVE: // Catch window activate via click message
				case WA_ACTIVE:
					OBJM_Main->HBRP_CURRENTBORDER = &OBJM_Main->HBR_BorderActive;
					InvalidateRect(hWnd, &RECT_SizeBorder_Left, FALSE);
					InvalidateRect(hWnd, &RECT_SizeBorder_Top, FALSE);
					InvalidateRect(hWnd, &RECT_SizeBorder_Right, FALSE);
					InvalidateRect(hWnd, &RECT_SizeBorder_Bottom, FALSE);
					InvalidateRect(SS_Title, NULL, FALSE);
					InvalidateRect(BTN_Close, NULL, FALSE);
					InvalidateRect(BTN_Minimize, NULL, FALSE);
					break;
				case WA_INACTIVE:
					OBJM_Main->HBRP_CURRENTBORDER = &OBJM_Main->HBR_BorderInactive;
					InvalidateRect(hWnd, &RECT_SizeBorder_Left, FALSE);
					InvalidateRect(hWnd, &RECT_SizeBorder_Top, FALSE);
					InvalidateRect(hWnd, &RECT_SizeBorder_Right, FALSE);
					InvalidateRect(hWnd, &RECT_SizeBorder_Bottom, FALSE);
					InvalidateRect(SS_Title, NULL, FALSE);
					InvalidateRect(BTN_Close, NULL, FALSE);
					InvalidateRect(BTN_Minimize, NULL, FALSE);
					break;
			}
			break;
		}

		case WM_NCCALCSIZE:
		{
			// Return zero remove standard window frame (non-client areas)
			// * The application use custom-draw caption bar and borders, everything is draw in client-area
			return (LRESULT)0;
		}

		case WM_NCHITTEST:
		{
			// Handling caption bar and resize messages

			const LRESULT result = ::DefWindowProcW(hWnd, msg, wp, lp); // Default WM_NCHITTEST response

			RECT lrc = RECT_Caption;
			RECT lrc_sizeborder_top = RECT_SizeBorder_Top,
				lrc_sizeborder_left = RECT_SizeBorder_Left,
				lrc_sizeborder_right = RECT_SizeBorder_Right,
				lrc_sizeborder_bottom = RECT_SizeBorder_Bottom;
			lrc_sizeborder_top.bottom += 2;
			lrc_sizeborder_bottom.top -= 2;
			lrc_sizeborder_left.right += 2;
			lrc_sizeborder_right.left -= 2;

			POINT pt;
			pt.x = GET_X_LPARAM(lp);
			pt.y = GET_Y_LPARAM(lp);
			MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT*>(&lrc), (sizeof(RECT) / sizeof(POINT)));
			MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT*>(&lrc_sizeborder_top), (sizeof(RECT) / sizeof(POINT)));
			MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT*>(&lrc_sizeborder_bottom), (sizeof(RECT) / sizeof(POINT)));
			MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT*>(&lrc_sizeborder_left), (sizeof(RECT) / sizeof(POINT)));
			MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT*>(&lrc_sizeborder_right), (sizeof(RECT) / sizeof(POINT)));

			// Handling size border events
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_bottom, pt)))
				return HTBOTTOM;
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_top, pt)))
				return HTTOP;
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_left, pt)))
				return HTLEFT;
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_right, pt)))
				return HTRIGHT;

			// Handling caption events
			if ((result == HTCLIENT) && (PtInRect(&lrc, pt)))
			{
				return HTCAPTION;
			}

			return result;
		}

		case WM_SIZE:
		{
			// Handling WM_SIZE messages

			BufferedPaintStopAllAnimations(hWnd); // Stop all animations during resize

			// Caption
			RECT_Caption.right = LOWORD(lp) - BORDER_WIDTH;

			// Main content container
			if (!IS_APPTHEMESHOWSCROLLBAR)
			{
				SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
					LOWORD(lp) - (BORDER_WIDTH * 2), // W
					HIWORD(lp) - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top), // H
					SWP_NOZORDER);
			}
			else 
			{
				SetWindowPos(SS_MAINCONTENTCTR, NULL, BORDER_WIDTH, RECT_Caption.bottom,
					LOWORD(lp) - (BORDER_WIDTH * 2) - STD_SCROLLBAR_WIDTH, // W
					HIWORD(lp) - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top), // H
					SWP_NOZORDER);
			}

			// Top size border
			RECT_SizeBorder_Top.right = LOWORD(lp);

			// Bottom size border
			RECT_SizeBorder_Bottom.right = LOWORD(lp);
			RECT_SizeBorder_Bottom.top = HIWORD(lp) - BORDER_WIDTH;
			RECT_SizeBorder_Bottom.bottom = HIWORD(lp);

			// Left size border
			RECT_SizeBorder_Left.bottom = HIWORD(lp) - BORDER_WIDTH;

			// Right size border
			RECT_SizeBorder_Right.left = LOWORD(lp) - BORDER_WIDTH;
			RECT_SizeBorder_Right.right = LOWORD(lp);
			RECT_SizeBorder_Right.bottom = HIWORD(lp) - BORDER_WIDTH;

			// Scrollbar (Size)
			static RECT sbRect; GetClientRect(SB_MAINCONTENTCTR, &sbRect);
			static int sbWidth = 0, sbHeight = 0;
			sbWidth = sbRect.right - sbRect.left;	//
			sbHeight = sbRect.bottom - sbRect.top;	// Get latest mcctr_scrollbar's width & height

			if (HIWORD(lp) - (BORDER_WIDTH * 2) - CAPTIONBAR_HEIGHT > sbHeight || HIWORD(lp) - (BORDER_WIDTH * 2) - CAPTIONBAR_HEIGHT < sbHeight)
				// Update new scrollbar's height to fit new container size (height resize)
				SetWindowPos(SB_MAINCONTENTCTR, NULL, LOWORD(lp) - BORDER_WIDTH - sbWidth, RECT_Caption.bottom, sbWidth, HIWORD(lp) - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top), SWP_NOZORDER);
			else
				// Update new scrollbar's posX (width resize)
				SetWindowPos(SB_MAINCONTENTCTR, NULL, LOWORD(lp) - BORDER_WIDTH - sbWidth, RECT_Caption.bottom, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);

			// Main content container & its scrollbar info
			static RECT rMCCTR; GetClientRect(SS_MAINCONTENTCTR, &rMCCTR);
			{
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_ALL;

				// Get current scroll info
				if (SendMessageW(SB_MAINCONTENTCTR, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
					MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);
				si.nPage = rMCCTR.bottom; // Update new page size

				// Reset container's childs positions & current scroll pos
				SetWindowPos(SS_Heading1, NULL, MAINCONTENTCTR_PADDING, MAINCONTENTCTR_PADDING, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(BTN_Standard, NULL, MAINCONTENTCTR_PADDING, MAINCONTENTCTR_PADDING + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(BTN_Radio2Left, NULL, MAINCONTENTCTR_PADDING + 140, MAINCONTENTCTR_PADDING + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(BTN_Radio2Right, NULL, MAINCONTENTCTR_PADDING + 206, MAINCONTENTCTR_PADDING + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(BTN_Radio3Left, NULL, MAINCONTENTCTR_PADDING + 281, MAINCONTENTCTR_PADDING + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(BTN_Radio3Middle, NULL, MAINCONTENTCTR_PADDING + 347, MAINCONTENTCTR_PADDING + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
				SetWindowPos(BTN_Radio3Right, NULL, MAINCONTENTCTR_PADDING + 413, MAINCONTENTCTR_PADDING + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
				si.nPos = 0;

				// Set scrollbar info
				SendMessageW(SB_MAINCONTENTCTR, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);

				// Show|Hide the scrollbar if needed
				if (!IS_APPTHEMESHOWSCROLLBAR)
				{
					ShowWindow(SB_MAINCONTENTCTR, SW_HIDE);
				}
				else
				{
					if ((unsigned int)si.nPage > (unsigned int)si.nMax)
					{
						ShowWindow(SB_MAINCONTENTCTR, SW_HIDE);
					}
					else
					{
						ShowWindow(SB_MAINCONTENTCTR, SW_SHOW);
					}
				}
			}

			// Close & Minimize buttons
			SetWindowPos(BTN_Close, NULL, LOWORD(lp) - BORDER_WIDTH - 58, BORDER_WIDTH, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Minimize, NULL, LOWORD(lp) - BORDER_WIDTH - 116, BORDER_WIDTH, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);

			// Extra invalidate
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			APPLICATION_WIDTH = (int)LOWORD(lp);
			APPLICATION_HEIGHT = (int)HIWORD(lp);
			return (LRESULT)0;
		}

		case WM_WINDOWPOSCHANGING:
		{
			// Keep close and minimize button drawn correctly during resize

			RedrawWindow(BTN_Close, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			RedrawWindow(BTN_Minimize, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return (LRESULT)0;
		}

		case WM_WINDOWPOSCHANGED:
		{
			break;
		}

		case WM_GETMINMAXINFO:
		{
			// Set minimum window size depending on currently selected TAB

			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lp;

			lpMMI->ptMinTrackSize.x = 519;
			lpMMI->ptMinTrackSize.y = 200;
			return (LRESULT)0;
		}

		case WM_VSCROLL:
		{
			// Handling vertical scroll messages

			static short ScrollPixel = 10;
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;

			if (SendMessageW(SB_MAINCONTENTCTR, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
				MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);

			switch ((int)LOWORD(wp))
			{
				case SB_LINEUP:
					si.nPos -= ScrollPixel;
					break;

				case SB_LINEDOWN:
					si.nPos += ScrollPixel;
					break;

				case SB_THUMBTRACK:
					si.nPos = HIWORD(wp);
					break;
			}
			if (si.nPos < si.nMin) si.nPos = si.nMin;
			else if (si.nPos > si.nMax - (int)si.nPage) si.nPos = si.nMax - (int)si.nPage;

			static int scroll_distance = 0; scroll_distance = -si.nPos;
			static RECT rMCCTR; GetClientRect(SS_MAINCONTENTCTR, &rMCCTR);
			si.nPage = rMCCTR.bottom; // Update new page size

			// Update container's childs positions
			SetWindowPos(SS_Heading1, NULL, MAINCONTENTCTR_PADDING, MAINCONTENTCTR_PADDING + scroll_distance, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Standard, NULL, MAINCONTENTCTR_PADDING, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio2Left, NULL, MAINCONTENTCTR_PADDING + 140, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio2Right, NULL, MAINCONTENTCTR_PADDING + 206, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio3Left, NULL, MAINCONTENTCTR_PADDING + 281, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio3Middle, NULL, MAINCONTENTCTR_PADDING + 347, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio3Right, NULL, MAINCONTENTCTR_PADDING + 413, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			// Set scroll info
			SendMessageW(SB_MAINCONTENTCTR, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);

			// Extra invalidates
			static bool NeedInvalidate = 1;
			if (NeedInvalidate)
			{
				RedrawWindow(SS_MAINCONTENTCTR, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
				if (si.nPos == si.nMin || si.nPos == si.nMax - (int)si.nPage)
					NeedInvalidate = 0;
			}
			else
			{
				if (si.nPos != si.nMin && si.nPos != si.nMax - (int)si.nPage)
				{
					RedrawWindow(SS_MAINCONTENTCTR, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
					NeedInvalidate = 1;
				}
			}

			return (LRESULT)0;
		}

		case WM_MOUSEWHEEL:
		{
			// Handling mouse wheel scroll messages

			static short WheelDelta = 0, ScrollPixel = 20;

			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;

			if (SendMessageW(SB_MAINCONTENTCTR, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
				MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);

			if ((int)si.nPage > si.nMax) return 0; // Scrolling not needed

			WheelDelta = GET_WHEEL_DELTA_WPARAM(wp);
			switch (WheelDelta)
			{
			case 120:
				si.nPos -= ScrollPixel;
				break;
			case -120:
				si.nPos += ScrollPixel;
				break;
			}
			if (si.nPos < si.nMin) si.nPos = si.nMin;
			else if (si.nPos > si.nMax - (int)si.nPage) si.nPos = si.nMax - (int)si.nPage;

			static int scroll_distance = 0; scroll_distance = -si.nPos;
			static RECT rMCCTR; GetClientRect(SS_MAINCONTENTCTR, &rMCCTR);
			si.nPage = rMCCTR.bottom; // Update new page size

			// Update container's childs positions
			SetWindowPos(SS_Heading1, NULL, MAINCONTENTCTR_PADDING, MAINCONTENTCTR_PADDING + scroll_distance, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Standard, NULL, MAINCONTENTCTR_PADDING, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio2Left, NULL, MAINCONTENTCTR_PADDING + 140, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio2Right, NULL, MAINCONTENTCTR_PADDING + 206, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio3Left, NULL, MAINCONTENTCTR_PADDING + 281, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio3Middle, NULL, MAINCONTENTCTR_PADDING + 347, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			SetWindowPos(BTN_Radio3Right, NULL, MAINCONTENTCTR_PADDING + 413, MAINCONTENTCTR_PADDING + scroll_distance + 44, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
			// Set scroll info
			SendMessageW(SB_MAINCONTENTCTR, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);

			// Extra invalidates
			static bool NeedInvalidate = 1;
			if (NeedInvalidate)
			{
				RedrawWindow(SS_MAINCONTENTCTR, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
				if (si.nPos == si.nMin || si.nPos == si.nMax - (int)si.nPage)
					NeedInvalidate = 0;
			}
			else
			{
				if (si.nPos != si.nMin && si.nPos != si.nMax - (int)si.nPage)
				{
					RedrawWindow(SS_MAINCONTENTCTR, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
					NeedInvalidate = 1;
				}
			}

			return (LRESULT)0;
		}

		case WM_LBUTTONDOWN:
			SetFocus(hWnd);
			return (LRESULT)0;

		case WM_KEYDOWN:
		{
			break;
		}

		case WM_KEYUP:
		{
			switch (wp)
			{
				case VK_F5:
				{
					// Switch theme

					bool IsSuccess = 0;

					if (APPLICATION_THEME == L"Light") { if (nApp::SetAppTheme(L"Dark")) IsSuccess = 1; }
					else if (APPLICATION_THEME == L"Dark") { if (nApp::SetAppTheme(L"Ristretto")) IsSuccess = 1; }
					else if (APPLICATION_THEME == L"Ristretto") { if (nApp::SetAppTheme(L"Light")) IsSuccess = 1; }
					if (!IsSuccess) { MessageBoxW(hWnd, L"Error occurred!\n(Failed to set application theme)", L"", MB_OK | MB_ICONERROR); exit(1); }

					return (LRESULT)0;
				}

				case VK_F1:
				{
					// Show debug info

					RECT rMCCTR; GetClientRect(SS_MAINCONTENTCTR, &rMCCTR);
					WCHAR* wcaSelectedR2 = new WCHAR[(UINT64)GetWindowTextLengthW(CURRENT_SELECTEDRADIO2) + (UINT64)1]; GetWindowTextW(CURRENT_SELECTEDRADIO2, wcaSelectedR2, GetWindowTextLengthW(CURRENT_SELECTEDRADIO2) + 1);
					WCHAR* wcaSelectedR3 = new WCHAR[(UINT64)GetWindowTextLengthW(CURRENT_SELECTEDRADIO3) + (UINT64)1]; GetWindowTextW(CURRENT_SELECTEDRADIO3, wcaSelectedR3, GetWindowTextLengthW(CURRENT_SELECTEDRADIO3) + 1);
					std::wstring wstrSelectedR2(wcaSelectedR2);
					std::wstring wstrSelectedR3(wcaSelectedR3);

					MessageBoxW(MAIN_HWND, (L"Window size: " + std::to_wstring(APPLICATION_WIDTH) + L"x" + std::to_wstring(APPLICATION_HEIGHT) + L"\n"
											+ L"Caption size: " + std::to_wstring(RECT_Caption.bottom - RECT_Caption.top) + L"\n"
											+ L"Container size (Main content): " + std::to_wstring(rMCCTR.bottom) + L"x" + std::to_wstring(rMCCTR.right) + L"\n"
											+ L"Theme: " + APPLICATION_THEME + L" (F5)\n"
											+ L"IsReady: " + (IS_APPREADY ? L"Yes" : L"No") + L"\n"
											+ L"IsThemeShowScrollBar: " + (IS_APPTHEMESHOWSCROLLBAR ? L"Yes" : L"No") + L"\n"
											+ L"Selected R2: " + (wstrSelectedR2 == L"" ? L"None" : wstrSelectedR2) + L"\n"
											+ L"Selected R3: " + (wstrSelectedR3 == L"" ? L"None" : wstrSelectedR3)
						).c_str(), L"", MB_OK);

					delete[] wcaSelectedR2;
					delete[] wcaSelectedR3;
					return (LRESULT)0;
				}

				case VK_F2:
				{
					// ...
					return (LRESULT)0;
				}
			}

			break;
		}

		case WM_CLOSE:
		{
			nApp::OnExit();
			DestroyWindow(hWnd);
			return (LRESULT)0;
		}

		case WM_DESTROY:
		{
			nApp::OnDestroy();
			PostQuitMessage(0);
			return (LRESULT)0;
		}

		/*
		case WM_CTLCOLORSCROLLBAR:
		{
			if ((HWND)lp == SB_MAINCONTENTCTR)
			{
				...
			}

			break;
		}
		*/

		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
	}

	return DefWindowProcW(hWnd, msg, wp, lp);
}

LRESULT CALLBACK WindowProcedure_MainContentCTR(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{
		case WM_NCDESTROY:
		{
			RemoveWindowSubclass(hWnd, &WindowProcedure_MainContentCTR, uIdSubclass);
			return (LRESULT)0;
		}

		case WM_COMMAND:
		{
			switch (wp)
			{
				case BUTTON_STANDARD:
					MessageBoxW(MAIN_HWND, L"STANDARD BUTTON CLICKED", L"", MB_OK | MB_ICONINFORMATION);
					return (LRESULT)0;

				case BUTTON_R2LEFT:
					CURRENT_SELECTEDRADIO2 = BTN_Radio2Left;
					RedrawWindow(BTN_Radio2Right, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					return (LRESULT)0;

				case BUTTON_R2RIGHT:
					CURRENT_SELECTEDRADIO2 = BTN_Radio2Right;
					RedrawWindow(BTN_Radio2Left, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					return (LRESULT)0;

				case BUTTON_R3LEFT:
					CURRENT_SELECTEDRADIO3 = BTN_Radio3Left;
					RedrawWindow(BTN_Radio3Middle, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					RedrawWindow(BTN_Radio3Right, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					return (LRESULT)0;

				case BUTTON_R3MIDDLE:
					CURRENT_SELECTEDRADIO3 = BTN_Radio3Middle;
					RedrawWindow(BTN_Radio3Left, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					RedrawWindow(BTN_Radio3Right, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					return (LRESULT)0;

				case BUTTON_R3RIGHT:
					CURRENT_SELECTEDRADIO3 = BTN_Radio3Right;
					RedrawWindow(BTN_Radio3Left, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					RedrawWindow(BTN_Radio3Middle, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
					return (LRESULT)0;

				default:
					break;
			}

			break;
		}

		case WM_CTLCOLORSTATIC:
		{
			static HBRUSH RET_CTLCOLORSTATIC = (HBRUSH)GetStockObject(BLACK_BRUSH);
			if ((HWND)lp == SS_Heading1)
			{
				SetBkMode((HDC)wp, TRANSPARENT);
				SetBkColor((HDC)wp, OBJM_Main->CLR_Secondary);
				SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
				RET_CTLCOLORSTATIC = OBJM_Main->HBR_Secondary;
			}
			else RET_CTLCOLORSTATIC = OBJM_Main->HBR_DEBUG;
			return (LRESULT)RET_CTLCOLORSTATIC;
		}

		case WM_CTLCOLORBTN:
		{
			static HBRUSH RET_CTLCOLORBTN = (HBRUSH)GetStockObject(BLACK_BRUSH);
			if ((HWND)lp == BTN_Standard)
			{
				RET_CTLCOLORBTN = OBJM_Main->HBR_Secondary;
			}
			else RET_CTLCOLORBTN = OBJM_Main->HBR_DEBUG;
			return (LRESULT)RET_CTLCOLORBTN;
		}
	}

	return DefSubclassProc(hWnd, msg, wp, lp);
}