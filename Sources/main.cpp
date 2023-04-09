/*
*    Win32 GUI Sample
*/

#include "./Headers/func.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	// Terminate if application already running
	if (FindWindowW(WndClassName, NULL) != NULL)
	{
		MessageBoxW(NULL, L"Application is already running", L"", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	// Create custom window class
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = NULL;    // Manually handle "WM_ERASEBKGND" (reduce flickering)
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON7));
	wc.hInstance = hInstance;
	wc.lpszClassName = WndClassName;
	wc.lpfnWndProc = WindowProcedure;

	MAIN_HINSTANCE = hInstance;

	// Register window class
	if (!RegisterClassW(&wc))
	{
		MessageBoxW(NULL, L"Error occurred!\n(Failed to register window class)", L"", MB_OK | MB_ICONERROR);
		return -1;
	}

	{
		// Load application directory path
		bool IsAppDirPathLoaded = 0;
		WCHAR TextBuffer[MAX_PATH];
		APPLICATION_PATH = _wgetcwd(TextBuffer, MAX_PATH);
		std::wstring WSTR_TextBuffer(TextBuffer);
		if (!WSTR_TextBuffer.empty()) IsAppDirPathLoaded = 1;


		// Load application settings from file
		bool IsSettFileLoaded = 0;
		if (nApp::LoadSettingFile()) IsSettFileLoaded = 1;
		if (IsAppDirPathLoaded) nSol::cWriteLog(L"Loaded application path: ", (L"\"" + WSTR_TextBuffer + L"\"").c_str());
		else nSol::cWriteLog(L"Failed to load application path.", L"", L"ERROR");
		if (IsSettFileLoaded) nSol::cWriteLog(L"Loaded application settings from file.");
		else nSol::cWriteLog(L"Failed to load application settings from file, using default settings.", L"", L"ERROR");
	}


	// Create main window using registered window class
	int DesktopWidth = 0, DesktopHeight = 0; nSol::GetDesktopResolution(DesktopWidth, DesktopHeight);   // Get user desktop resolution
	nSol::cWriteLog(L"Current desktop resolution: ", (std::to_wstring(DesktopWidth) + L"x" + std::to_wstring(DesktopHeight)).c_str());
	MAIN_HWND = CreateWindowExW(WS_EX_LAYERED, WndClassName, L"Win32 GUI Sample", WS_MYSTYLE,
		(DesktopWidth / 2) - (int)((double)APPLICATION_WIDTH / 1.4), (DesktopHeight / 2) - (int)((double)APPLICATION_HEIGHT / 1.4),   // Semi-center application on start
		APPLICATION_WIDTH, APPLICATION_HEIGHT,   // Initial application window size
		NULL, NULL, hInstance, NULL);
	SetLayeredWindowAttributes(MAIN_HWND, RGB(141, 172, 160), 255, LWA_ALPHA | LWA_COLORKEY);   // Set transparency color (Make MAIN_HWND compability with WS_MYSTYLE, otherwise MAIN_HWND won't be visible)

	// Execute when the application's main window is fully initialized and ready to displays
	nApp::OnReady();

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

	nSol::cWriteLog(L"", L"", L"", 1, 1);

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
			else 
			{ 
				MessageBoxW(NULL, L"Error occurred!\n(Failed to initialize main window)", L"", MB_OK | MB_ICONERROR);
				nSol::cWriteLog(L"Failed to initialize main window.", L"", L"ERROR");
				exit(1); 
			}
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

			// Begin painting to the memory DC
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

			static HBRUSH* RET_CTLCOLORSTATIC = nullptr;

			if ((HWND)lp == SS_Title)
			{
				SetBkMode((HDC)wp, TRANSPARENT);
				SetBkColor((HDC)wp, OBJM_Main->CLR_Primary);
				if (GetActiveWindow() == hWnd) SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
				else SetTextColor((HDC)wp, OBJM_Main->CLR_InactiveText);
				RET_CTLCOLORSTATIC = &OBJM_Main->HBR_Primary;
			}
			else if ((HWND)lp == SSCTR_MAINCONTENT)
			{
				static bool OnFirst = 1;
				if (OnFirst)
				{
					SetBkMode((HDC)wp, TRANSPARENT);
					SetBkColor((HDC)wp, OBJM_Main->CLR_Secondary);
					SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
					OnFirst = 0;
				}
				RET_CTLCOLORSTATIC = &OBJM_Main->HBR_Secondary;
			}
			else RET_CTLCOLORSTATIC = &OBJM_Main->HBR_DEBUG; // Apply debug color to non-handled static controls

			return (LRESULT)*RET_CTLCOLORSTATIC;
		}

		case WM_CTLCOLORBTN:
		{
			// Handling button control messages

			static HBRUSH* RET_CTLCOLORBTN = nullptr;
			RET_CTLCOLORBTN = nullptr;

			if ((HWND)lp == BTN_Close || (HWND)lp == BTN_Minimize)
			{
				RET_CTLCOLORBTN = &OBJM_Main->HBR_Primary;
			}
				
			if (!RET_CTLCOLORBTN)
			{
				MessageBoxW(NULL, L"Error occurred!\n(Unresolved WM_CTLCOLORBTN message in WindowProcedure)", L"", MB_OK | MB_ICONERROR);
				nSol::cWriteLog(L"Unresolved WM_CTLCOLORBTN message in WindowProcedure.", L"", L"ERROR");
				auto temp = (HBRUSH)GetStockObject(NULL_BRUSH);
				RET_CTLCOLORBTN = &temp;
			}
			return (LRESULT)*RET_CTLCOLORBTN;
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
				case WA_CLICKACTIVE:   // Catch window activate via click message
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
			// Adjust client rect to not spill over monitor edges when maximized
			// SRC: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
			if (wp == TRUE) 
			{
			      auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lp);
			      nSol::cAdjustMaximizedClientRect(hWnd, params.rgrc[0]);
			      return (LRESULT)0;
			}

			// Return zero remove standard window frame (non-client areas)
			// * The application use custom-draw caption bar and borders, everything is draw in client-area
			return (LRESULT)0;
		}

		case WM_NCHITTEST:
		{
			// Handling caption bar and resize messages

			const LRESULT result = ::DefWindowProcW(hWnd, msg, wp, lp);   // Default WM_NCHITTEST response

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

			// Handling size border events (ignore if the window is maximized)
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_bottom, pt)) && !IS_WINDOWMAXIMIZED)
				return HTBOTTOM;
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_top, pt)) && !IS_WINDOWMAXIMIZED)
				return HTTOP;
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_left, pt)) && !IS_WINDOWMAXIMIZED)
				return HTLEFT;
			if ((result == HTCLIENT) && (PtInRect(&lrc_sizeborder_right, pt)) && !IS_WINDOWMAXIMIZED)
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

			if (wp == SIZE_MAXIMIZED)
			{
				RedrawWindow(SSCTR_MAINCONTENT, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
				IS_WINDOWMAXIMIZED = true;
			}
			else IS_WINDOWMAXIMIZED = false;

			BufferedPaintStopAllAnimations(hWnd);   // Stop all animations during resize

			// Caption
			RECT_Caption.right = LOWORD(lp) - BORDER_WIDTH;

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

			// Container (SSCTR_MAINCONTENT)
			if (!IS_APPTHEMESHOWSCROLLBAR)
			{
				SetWindowPos(SSCTR_MAINCONTENT, NULL, BORDER_WIDTH, RECT_Caption.bottom,
					LOWORD(lp) - (BORDER_WIDTH * 2), // W
					HIWORD(lp) - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top),   // H
					SWP_NOZORDER);
			}
			else 
			{
				SetWindowPos(SSCTR_MAINCONTENT, NULL, BORDER_WIDTH, RECT_Caption.bottom,
					LOWORD(lp) - (BORDER_WIDTH * 2) - STD_SCROLLBAR_WIDTH, // W
					HIWORD(lp) - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top),   // H
					SWP_NOZORDER);
			}

			// Container scrollbar (SSCTR_MAINCONTENT)
			static RECT sbRect; GetClientRect(SB_SSCTR_MAINCONTENT, &sbRect);
			static int sbWidth = 0, sbHeight = 0;
			sbWidth = sbRect.right - sbRect.left;   //
			sbHeight = sbRect.bottom - sbRect.top;  // Get the latest container scrollbar width & height

			if (HIWORD(lp) - (BORDER_WIDTH * 2) - CAPTIONBAR_HEIGHT > sbHeight || HIWORD(lp) - (BORDER_WIDTH * 2) - CAPTIONBAR_HEIGHT < sbHeight)
				// Update new scrollbar's height to fit new container size (height resize)
				SetWindowPos(SB_SSCTR_MAINCONTENT, NULL, LOWORD(lp) - BORDER_WIDTH - sbWidth, RECT_Caption.bottom, sbWidth, HIWORD(lp) - (BORDER_WIDTH * 2) - (RECT_Caption.bottom - RECT_Caption.top), SWP_NOZORDER);
			else
				// Update new scrollbar's posX (width resize)
				SetWindowPos(SB_SSCTR_MAINCONTENT, NULL, LOWORD(lp) - BORDER_WIDTH - sbWidth, RECT_Caption.bottom, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);

			// Container scrollbar info (SSCTR_MAINCONTENT)
			static RECT LRECT_SSCTR_MAINCONTENT; GetClientRect(SSCTR_MAINCONTENT, &LRECT_SSCTR_MAINCONTENT);
			{
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_ALL;

				// Get current scroll info
				if (SendMessageW(SB_SSCTR_MAINCONTENT, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
				{
					MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);
					nSol::cWriteLog(L"Failed to get scroll info.", L"", L"ERROR");
				}
				si.nPage = LRECT_SSCTR_MAINCONTENT.bottom; // Update new page size

				// Scroll the container back to the zero pos and reset scroll pos
				ScrollWindowEx(SSCTR_MAINCONTENT, NULL, si.nPos, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
				si.nPos = 0;

				// Set scrollbar info
				SendMessageW(SB_SSCTR_MAINCONTENT, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);

				// Show|Hide the scrollbar if needed
				if (!IS_APPTHEMESHOWSCROLLBAR)
				{
					ShowWindow(SB_SSCTR_MAINCONTENT, SW_HIDE);
				}
				else
				{
					if ((unsigned int)si.nPage > (unsigned int)si.nMax)
					{
						ShowWindow(SB_SSCTR_MAINCONTENT, SW_HIDE);
					}
					else
					{
						ShowWindow(SB_SSCTR_MAINCONTENT, SW_SHOW);
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
			// Handling vertical scrolling info (Scroll bar control)

			// Get scroll info
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			if (SendMessageW(SB_SSCTR_MAINCONTENT, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
			{
				MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);
				nSol::cWriteLog(L"Failed to get scroll info.", L"", L"ERROR");
			}

			// Process scroll message
			static short ScrollPixel = NULL;       // Total pixel per scroll
			static bool ScrollingDown = NULL;      // Check if scrolling up or down
			static bool IsThumbTrackScroll = NULL; // Check if using thumb track to scroll
			static auto PreviousPos = NULL;        // Previous scroll pos
			ScrollPixel = 10;                      // Default scroll pixel
			PreviousPos = si.nPos;                 // Update new previous pos
			switch ((int)LOWORD(wp))
			{
				case SB_LINEUP:
					IsThumbTrackScroll = 0;
					ScrollingDown = 0;
					si.nPos -= ScrollPixel;
					break;

				case SB_LINEDOWN:
					IsThumbTrackScroll = 0;
					ScrollingDown = 1;
					si.nPos += ScrollPixel;
					break;

				case SB_THUMBTRACK:
					IsThumbTrackScroll = 1;
					si.nPos = HIWORD(wp);
					if (si.nPos < PreviousPos) ScrollingDown = 0;
					else ScrollingDown = 1;
					break;
			}

			// If scroll pos is the same, no scrolling needed
			if (PreviousPos == si.nPos) return (LRESULT)0;

			if (!IsThumbTrackScroll)
			{
				if (si.nPos < si.nMin)
				{
					ScrollWindowEx(SSCTR_MAINCONTENT, NULL, ScrollPixel - (-(si.nPos)), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
					si.nPos = si.nMin;
				}
				else if (si.nPos > si.nMax - (int)si.nPage)
				{
					ScrollWindowEx(SSCTR_MAINCONTENT, NULL, -ScrollPixel + -((si.nMax - (int)si.nPage) - si.nPos), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
					si.nPos = si.nMax - (int)si.nPage;
				}
				else
				{
					ScrollWindowEx(SSCTR_MAINCONTENT, NULL, (ScrollingDown ? -ScrollPixel : ScrollPixel), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
				}
			}
			else
			{
				if (si.nPos < si.nMin) si.nPos = si.nMin;
				else if (si.nPos > si.nMax - (int)si.nPage) si.nPos = si.nMax - (int)si.nPage;
				ScrollWindowEx(SSCTR_MAINCONTENT, NULL, (PreviousPos - si.nPos),
					NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
			}

			// Update new page size
			static RECT LRECT_SSCTR_MAINCONTENT; GetClientRect(SSCTR_MAINCONTENT, &LRECT_SSCTR_MAINCONTENT);
			si.nPage = LRECT_SSCTR_MAINCONTENT.bottom;   

			// Set scroll info
			SendMessageW(SB_SSCTR_MAINCONTENT, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);

			return (LRESULT)0;
		}

		case WM_MOUSEWHEEL:
		{
			// Handling vertical scrolling info (Mouse Wheel)

			static bool ScrollingDown = NULL;  // Check if scrolling up or down
			static short WheelDelta   = NULL,  // Wheel Delta
			ScrollPixel               = NULL;  // Total pixel per scroll
			
			// Get scroll info
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			if (SendMessageW(SB_SSCTR_MAINCONTENT, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
			{
				MessageBoxW(NULL, L"Error occurred!\n(Failed to get scroll info)", L"", MB_OK | MB_ICONWARNING);
				nSol::cWriteLog(L"Failed to get scroll info.", L"", L"ERROR");
			}

			// No scrolling needed if the current container size is already big enough to show all the contents.
			if ((int)si.nPage > si.nMax) return (LRESULT)0;

			// Process scroll message
			WheelDelta = GET_WHEEL_DELTA_WPARAM(wp);  // Get wheel data
			ScrollPixel = 10;                         // Default scroll pixel (Alternative: ScrollPixel = (si.nMax - (int)si.nPage) / 5;) <- this will take 5 scroll to scroll from begin to end
			switch (WheelDelta)
			{
				case 120:
					ScrollingDown = 0;
					si.nPos -= ScrollPixel;
					break;

				case -120:
					ScrollingDown = 1;
					si.nPos += ScrollPixel;
					break;
			}

			if (si.nPos < si.nMin)
			{
				ScrollWindowEx(SSCTR_MAINCONTENT, NULL, ScrollPixel - (-(si.nPos)), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
				si.nPos = si.nMin;
			}
			else if (si.nPos > si.nMax - (int)si.nPage)
			{
				ScrollWindowEx(SSCTR_MAINCONTENT, NULL, -ScrollPixel + -((si.nMax - (int)si.nPage) - si.nPos), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
				si.nPos = si.nMax - (int)si.nPage;
			}
			else
			{
				ScrollWindowEx(SSCTR_MAINCONTENT, NULL, (ScrollingDown ? -ScrollPixel : ScrollPixel), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
			}

			// Update new page size
			static RECT LRECT_SSCTR_MAINCONTENT; GetClientRect(SSCTR_MAINCONTENT, &LRECT_SSCTR_MAINCONTENT);
			si.nPage = LRECT_SSCTR_MAINCONTENT.bottom; 

			// Set scroll info
			SendMessageW(SB_SSCTR_MAINCONTENT, SBM_SETSCROLLINFO, TRUE, (LPARAM)&si);

			return (LRESULT)0;
		}

		case WM_LBUTTONDOWN:
			SetFocus(hWnd);   // Redirect focus to main window on background click
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
					// Switch application theme

					bool IsSuccess = 0;

					if (APPLICATION_THEME == L"Light") { if (nApp::SetAppTheme(L"Dark")) IsSuccess = 1; }
					else if (APPLICATION_THEME == L"Dark") { if (nApp::SetAppTheme(L"Ristretto")) IsSuccess = 1; }
					else if (APPLICATION_THEME == L"Ristretto") { if (nApp::SetAppTheme(L"Obisidan")) IsSuccess = 1; }
					else if (APPLICATION_THEME == L"Obisidan") { if (nApp::SetAppTheme(L"Light")) IsSuccess = 1; }
					if (!IsSuccess) 
					{ 
						MessageBoxW(hWnd, L"Error occurred!\n(Failed to set application theme)", L"", MB_OK | MB_ICONERROR); 
						nSol::cWriteLog(L"Failed to set application theme.", L"", L"ERROR");
						exit(1); 
					}

					return (LRESULT)0;
				}

				case VK_F1:
				{
					// Show debug info

					RECT LRECT_SSCTR_MAINCONTENT; GetClientRect(SSCTR_MAINCONTENT, &LRECT_SSCTR_MAINCONTENT);
					WCHAR* wcaSelectedR2 = new WCHAR[(UINT64)GetWindowTextLengthW(CURRENT_SELECTEDRADIO2) + (UINT64)1]; GetWindowTextW(CURRENT_SELECTEDRADIO2, wcaSelectedR2, GetWindowTextLengthW(CURRENT_SELECTEDRADIO2) + 1);
					WCHAR* wcaSelectedR3 = new WCHAR[(UINT64)GetWindowTextLengthW(CURRENT_SELECTEDRADIO3) + (UINT64)1]; GetWindowTextW(CURRENT_SELECTEDRADIO3, wcaSelectedR3, GetWindowTextLengthW(CURRENT_SELECTEDRADIO3) + 1);
					std::wstring wstrSelectedR2(wcaSelectedR2);
					std::wstring wstrSelectedR3(wcaSelectedR3);
					delete[] wcaSelectedR2;
					delete[] wcaSelectedR3;

					MessageBoxW(MAIN_HWND, (L"Window size: " + std::to_wstring(APPLICATION_WIDTH) + L"x" + std::to_wstring(APPLICATION_HEIGHT) + L"\n"
											+ L"Caption size: " + std::to_wstring(RECT_Caption.bottom - RECT_Caption.top) + L"\n"
											+ L"Container size (Main content): " + std::to_wstring(LRECT_SSCTR_MAINCONTENT.bottom) + L"x" + std::to_wstring(LRECT_SSCTR_MAINCONTENT.right) + L"\n"
											+ L"Theme: " + APPLICATION_THEME + L" (F5)\n"
											+ L"IsReady: " + (IS_APPREADY ? L"Yes" : L"No") + L"\n"
											+ L"IsWindowMaximized: " + (IS_WINDOWMAXIMIZED ? L"Yes" : L"No") + L"\n"
											+ L"IsThemeShowScrollBar: " + (IS_APPTHEMESHOWSCROLLBAR ? L"Yes" : L"No") + L"\n"
											+ L"Selected R2: " + (wstrSelectedR2 == L"" ? L"None" : wstrSelectedR2) + L"\n"
											+ L"Selected R3: " + (wstrSelectedR3 == L"" ? L"None" : wstrSelectedR3)
						).c_str(), L"", MB_OK);

					return (LRESULT)0;
				}

				case VK_F2:
				{
					std::wstring WSTR_TextBuffer(APPLICATION_PATH);

					nSol::cWriteLog(L"Message description: ", L"result", L"ERROR_LEVEL");

					return (LRESULT)0;
				}

				case VK_F3:
				{
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

LRESULT CALLBACK WindowProcedure_SSCTR_MAINCONTENT(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	static bool bEditboxFocusInvalidateCheck = 0;

	switch (msg)
	{
		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_NCDESTROY:
		{
			RemoveWindowSubclass(hWnd, &WindowProcedure_SSCTR_MAINCONTENT, uIdSubclass);
			return (LRESULT)0;
		}

        case WM_MEASUREITEM:
        {
            // Set item metrics for comboboxes

            LPMEASUREITEMSTRUCT lpMeasureItem = (LPMEASUREITEMSTRUCT)lp;

            if (lpMeasureItem->CtlType == ODT_COMBOBOX)   // Only process combobox messages
            {
				{
					TEXTMETRIC tm;
					HWND cb_hWnd = CB_SelectTheme;  // Set metric for Select Theme Combobox
					HDC  hdc = GetDC(cb_hWnd);      // Get DC

					if (GetTextMetricsW(hdc, &tm))
						// Set the item height to that of the font + 10px padding
						lpMeasureItem->itemHeight = tm.tmInternalLeading +
						tm.tmHeight + tm.tmExternalLeading + 10;

					ReleaseDC(cb_hWnd, hdc);   // Release DC
				}

                return (LRESULT)TRUE;
            }

            break;
        }

        case WM_DRAWITEM:
        {
            // Handle owner-drawn control messages

            LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lp;

            if (lpDrawItem->hwndItem == CB_SelectTheme)   // Select Theme Combobox
            {
				SetBkMode(lpDrawItem->hDC, TRANSPARENT);
				SetBkColor(lpDrawItem->hDC, CBDL_CustomDraw1_Manager->CLR_ComboboxDropdownListColor_Background);
				SetTextColor(lpDrawItem->hDC, CBDL_CustomDraw1_Manager->CLR_InactiveTextColor);

				if (lpDrawItem->itemAction & ODA_DRAWENTIRE)
				{
					SetBkColor(lpDrawItem->hDC, CBDL_CustomDraw1_Manager->CLR_ComboboxDropdownListColor_Background);
					SetTextColor(lpDrawItem->hDC, CBDL_CustomDraw1_Manager->CLR_InactiveTextColor);
				}
				if (lpDrawItem->itemState & ODS_SELECTED)
				{
					SetBkColor(lpDrawItem->hDC, CBDL_CustomDraw1_Manager->CLR_ComboboxDropdownListColor_Selected);
					SetTextColor(lpDrawItem->hDC, CBDL_CustomDraw1_Manager->CLR_DefaultTextColor);
				}

                size_t text_length = (size_t)SendMessageW(lpDrawItem->hwndItem, CB_GETLBTEXTLEN, lpDrawItem->itemID, 0);   // Get item text length
				WCHAR* TextBuffer = new WCHAR[(int)(text_length + 1)];   // Allocate text buffer
                SendMessageW(lpDrawItem->hwndItem, CB_GETLBTEXT, lpDrawItem->itemID, (LPARAM)TextBuffer);   // Get item text into text buffer

                // Draw texts
                ExtTextOutW(lpDrawItem->hDC,
                    lpDrawItem->rcItem.left + 5, lpDrawItem->rcItem.top + 5,
                    ETO_OPAQUE | ETO_CLIPPED, &lpDrawItem->rcItem,
                    TextBuffer, (UINT)text_length, NULL);
                delete[] TextBuffer;   // Release text buffer

                // Get the dropdown hwnd to override border paintings
                COMBOBOXINFO cbi{};
                cbi.cbSize = sizeof(COMBOBOXINFO);
                GetComboBoxInfo(lpDrawItem->hwndItem, &cbi);
                HWND DDL = cbi.hwndList;

                // Painting the borders
                auto hdc = GetWindowDC(DDL);                       // The dropdown is non-client area so GetClientDC() won't work
                static RECT rc; GetClientRect(DDL, &rc);
                FrameRect(hdc, &rc, CBDL_CustomDraw1_Manager->HBR_ComboboxDropdownListColor_Border);  // Painting the borders

                /*
                * Get total items of the combobox to get the id of the last item
                * (To check if the last item is selected and make the "unpainted" 2-pixels match background color)
                */
                int total_cbitems = (int)SendMessageW(lpDrawItem->hwndItem, CB_GETCOUNT, NULL, NULL);
                rc.top = rc.bottom -= 1; rc.top -= 2; rc.left += 1; rc.right -= 1;
                FillRect(hdc, &rc, (lpDrawItem->itemID == (total_cbitems - 1) ? CBDL_CustomDraw1_Manager->HBR_ComboboxDropdownListColor_Selected : CBDL_CustomDraw1_Manager->HBR_ComboboxDropdownListColor_Background));
                ReleaseDC(DDL, hdc);

				RedrawWindow(lpDrawItem->hwndItem, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
                return (LRESULT)TRUE;
            }

            break;
        }

		case WM_COMMAND:
		{
            // Combobox messages
            if (HIWORD(wp) == CBN_SELCHANGE)
            {
                int iIndex = (int)SendMessageW((HWND)lp, CB_GETCURSEL, NULL, NULL);

                if ((HWND)lp == CB_SelectTheme)
                {
                    switch (iIndex)
                    {
						case 0:
							nApp::SetAppTheme(L"Light");
							nApp::UpdateSettingFile(L"THEME", L"Light");
							break;
						case 1:
							nApp::SetAppTheme(L"Dark");
							nApp::UpdateSettingFile(L"THEME", L"Dark");
							break;
						case 2:
							nApp::SetAppTheme(L"Ristretto");
							nApp::UpdateSettingFile(L"THEME", L"Ristretto");
							break;
						case 3:
							nApp::SetAppTheme(L"Obisidan");
							nApp::UpdateSettingFile(L"THEME", L"Obisidan");
							break;
                    }

					COMBOBOXINFO cbi{};
					cbi.cbSize = sizeof(COMBOBOXINFO);
					GetComboBoxInfo((HWND)lp, &cbi);
					RedrawWindow(cbi.hwndList, NULL, NULL, RDW_INVALIDATE);  // Redraw the drop-down list to match new theme

					return (LRESULT)0;
                }
            }

			// Other messages
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

				case BUTTON_EDNORMALOK:
				{
					WCHAR* TextBuffer = new WCHAR[(UINT64)GetWindowTextLengthW(ED_Normal) + (UINT64)1];
					GetWindowTextW(ED_Normal, TextBuffer, GetWindowTextLengthW(ED_Normal) + 1);
					std::wstring wstrTextBuffer(TextBuffer);
					delete[] TextBuffer;

					SetWindowTextW(ED_Normal, L"");
					if (wstrTextBuffer != L"")
						MessageBoxW(hWnd, (L"Entered:\n" + wstrTextBuffer).c_str(), L"", MB_OK | MB_ICONINFORMATION);
					else MessageBeep(MB_ICONERROR);

					return (LRESULT)0;
				}

				case BUTTON_EDPASSWORDOK:
				{
					WCHAR* TextBuffer = new WCHAR[(UINT64)GetWindowTextLengthW(ED_Password) + (UINT64)1];
					GetWindowTextW(ED_Password, TextBuffer, GetWindowTextLengthW(ED_Password) + 1);
					std::wstring wstrTextBuffer(TextBuffer);
					delete[] TextBuffer;

					SetWindowTextW(ED_Password, L"");
					if (wstrTextBuffer == L"")
					{
						MessageBeep(MB_ICONERROR);
						return (LRESULT)0;
					}
					if (wstrTextBuffer == L"P@ssw0rdtk")
						MessageBoxW(hWnd, L"The entered password is correct!", L"", MB_OK | MB_ICONINFORMATION);
					else MessageBoxW(hWnd, L"The entered password is incorrect!\n(The password is: P@ssw0rdtk)", L"", MB_OK | MB_ICONWARNING);

					return (LRESULT)0;
				}

				case BUTTON_EDMULTILINEOK:
				{
					WCHAR* TextBuffer = new WCHAR[(UINT64)GetWindowTextLengthW(ED_Multiline) + (UINT64)1];
					GetWindowTextW(ED_Multiline, TextBuffer, GetWindowTextLengthW(ED_Multiline) + 1);
					std::wstring wstrTextBuffer(TextBuffer);
					delete[] TextBuffer;

					if (wstrTextBuffer == L"")
					{
						return (LRESULT)0;
					}
					else { SetWindowTextW(ED_Multiline, L""); MessageBeep(MB_OK); }

					return (LRESULT)0;
				}

				default:
					break;
			}

			break;
		}

		case WM_CTLCOLORSTATIC:
		{
			static HBRUSH* RET_CTLCOLORSTATIC = nullptr;
			RET_CTLCOLORSTATIC = nullptr;

			if ((HWND)lp == SS_Heading1 || (HWND)lp == SS_Heading2 || (HWND)lp == SS_Heading3)
			{
				RET_CTLCOLORSTATIC = &OBJM_Main->HBR_Secondary;
				SetBkMode((HDC)wp, TRANSPARENT);
				SetBkColor((HDC)wp, OBJM_Main->CLR_Secondary);
				SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
			}
			else if ((HWND)lp == SS_TextNoteNormalEditbox || (HWND)lp == SS_TextNotePasswordEditbox || (HWND)lp == SS_TextNoteMultilineEditbox || (HWND)lp == SS_TextNoteCBSelectTheme)
			{
				RET_CTLCOLORSTATIC = &OBJM_Main->HBR_Secondary;
				SetBkMode((HDC)wp, TRANSPARENT);
				SetBkColor((HDC)wp, OBJM_Main->CLR_Secondary);
				SetTextColor((HDC)wp, OBJM_Main->CLR_InactiveText);
			}
			else if ((HWND)lp == SS_ED_Normal)
			{
				auto temp = (HBRUSH)GetStockObject(NULL_BRUSH);
				RET_CTLCOLORSTATIC = &temp;

				static RECT rc1;
				static RECT rc2;
				GetClientRect((HWND)lp, &rc1);
				GetClientRect((HWND)lp, &rc2);

				rc2.left += 1; rc2.right -= 1; rc2.top += 1; rc2.bottom -= 1;
				FrameRect((HDC)wp, &rc1, OBJM_Main->HBR_Secondary);
				FrameRect((HDC)wp, &rc2, OBJM_Main->HBR_EditBox);

				Graphics graphics((HDC)wp);
				graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
				Gdiplus::Rect grc(rc1.top, rc1.left, rc1.right, rc1.bottom);

				SolidBrush SB_EditCtrl(Color(0, OBJM_Main->CL_EditBox.GetRed(), OBJM_Main->CL_EditBox.GetGreen(), OBJM_Main->CL_EditBox.GetBlue()));

				nSol::FillRoundRect(&graphics, &SB_EditCtrl, grc,
					(GetFocus() == ED_Normal
						? Color(255, OBJM_Main->CL_EditBoxBorder.GetRed(), OBJM_Main->CL_EditBoxBorder.GetGreen(), OBJM_Main->CL_EditBoxBorder.GetBlue())
						: Color(175, OBJM_Main->CL_EditBoxBorder.GetRed(), OBJM_Main->CL_EditBoxBorder.GetGreen(), OBJM_Main->CL_EditBoxBorder.GetBlue())),
					1);
				if (GetFocus() != ED_Normal) bEditboxFocusInvalidateCheck = 0; else bEditboxFocusInvalidateCheck = 1;
			}
			else if ((HWND)lp == SS_ED_Password)
			{
				auto temp = (HBRUSH)GetStockObject(NULL_BRUSH);
				RET_CTLCOLORSTATIC = &temp;

				static RECT rc1;
				static RECT rc2;
				GetClientRect((HWND)lp, &rc1);
				GetClientRect((HWND)lp, &rc2);

				rc2.left += 1; rc2.right -= 1; rc2.top += 1; rc2.bottom -= 1;
				FrameRect((HDC)wp, &rc1, OBJM_Main->HBR_Secondary);
				FrameRect((HDC)wp, &rc2, OBJM_Main->HBR_EditBox);

				Graphics graphics((HDC)wp);
				graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
				Gdiplus::Rect grc(rc1.top, rc1.left, rc1.right, rc1.bottom);

				SolidBrush SB_EditCtrl(Color(0, OBJM_Main->CL_EditBox.GetRed(), OBJM_Main->CL_EditBox.GetGreen(), OBJM_Main->CL_EditBox.GetBlue()));

				nSol::FillRoundRect(&graphics, &SB_EditCtrl, grc,
					(GetFocus() == ED_Password
						? Color(255, OBJM_Main->CL_EditBoxBorder.GetRed(), OBJM_Main->CL_EditBoxBorder.GetGreen(), OBJM_Main->CL_EditBoxBorder.GetBlue())
						: Color(175, OBJM_Main->CL_EditBoxBorder.GetRed(), OBJM_Main->CL_EditBoxBorder.GetGreen(), OBJM_Main->CL_EditBoxBorder.GetBlue())),
					1);
				if (GetFocus() != ED_Password) bEditboxFocusInvalidateCheck = 0; else bEditboxFocusInvalidateCheck = 1;
			}
			else if ((HWND)lp == SS_ED_Multiline)
			{
				auto temp = (HBRUSH)GetStockObject(NULL_BRUSH);
				RET_CTLCOLORSTATIC = &temp;

				static RECT rc1;
				static RECT rc2;
				GetClientRect((HWND)lp, &rc1);
				GetClientRect((HWND)lp, &rc2);

				rc2.left += 1; rc2.right -= 1; rc2.top += 1; rc2.bottom -= 1;
				FrameRect((HDC)wp, &rc1, OBJM_Main->HBR_Secondary);
				FrameRect((HDC)wp, &rc2, OBJM_Main->HBR_EditBox);

				Graphics graphics((HDC)wp);
				graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
				Gdiplus::Rect grc(rc1.top, rc1.left, rc1.right, rc1.bottom);

				SolidBrush SB_EditCtrl(Color(0, OBJM_Main->CL_EditBox.GetRed(), OBJM_Main->CL_EditBox.GetGreen(), OBJM_Main->CL_EditBox.GetBlue()));

				nSol::FillRoundRect(&graphics, &SB_EditCtrl, grc,
					(GetFocus() == ED_Multiline
						? Color(255, OBJM_Main->CL_EditBoxBorder.GetRed(), OBJM_Main->CL_EditBoxBorder.GetGreen(), OBJM_Main->CL_EditBoxBorder.GetBlue())
						: Color(175, OBJM_Main->CL_EditBoxBorder.GetRed(), OBJM_Main->CL_EditBoxBorder.GetGreen(), OBJM_Main->CL_EditBoxBorder.GetBlue())),
					1);
				if (GetFocus() != ED_Multiline) bEditboxFocusInvalidateCheck = 0; else bEditboxFocusInvalidateCheck = 1;
			}
			else if ((HWND)lp == ED_Normal)
			{
				if (GetFocus() != (HWND)lp) bEditboxFocusInvalidateCheck = 0;

				RET_CTLCOLORSTATIC = &OBJM_Main->HBR_EditBox;
				SetTextColor((HDC)wp, OBJM_Main->CLR_InactiveText);
				SetBkColor((HDC)wp, OBJM_Main->CLR_EditBox);
				if (!bEditboxFocusInvalidateCheck)
					RedrawWindow(SS_ED_Normal, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
			}
			else if ((HWND)lp == ED_Password)
			{
				if (GetFocus() != (HWND)lp) bEditboxFocusInvalidateCheck = 0;

				RET_CTLCOLORSTATIC = &OBJM_Main->HBR_EditBox;
				SetTextColor((HDC)wp, OBJM_Main->CLR_InactiveText);
				SetBkColor((HDC)wp, OBJM_Main->CLR_EditBox);
				if (!bEditboxFocusInvalidateCheck)
					RedrawWindow(SS_ED_Password, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
			}
			else if ((HWND)lp == ED_Multiline)
			{
				if (GetFocus() != (HWND)lp) bEditboxFocusInvalidateCheck = 0;

				RET_CTLCOLORSTATIC = &OBJM_Main->HBR_EditBox;
				SetTextColor((HDC)wp, OBJM_Main->CLR_InactiveText);
				SetBkColor((HDC)wp, OBJM_Main->CLR_EditBox);
				if (!bEditboxFocusInvalidateCheck)
					RedrawWindow(SS_ED_Multiline, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
			}

			if (!RET_CTLCOLORSTATIC)
			{
				MessageBoxW(NULL, L"Error occurred!\n(Unresolved WM_CTLCOLORSTATIC message)", L"", MB_OK | MB_ICONERROR);
				nSol::cWriteLog(L"Unresolved WM_CTLCOLORSTATIC message.", L"", L"ERROR");
				exit(1);
			}
			return (LRESULT)*RET_CTLCOLORSTATIC;
		}

		case WM_CTLCOLORBTN:
		{
			static HBRUSH* RET_CTLCOLORBTN = nullptr;
			RET_CTLCOLORBTN = nullptr;

			if ((HWND)lp == BTN_Standard ||
				(HWND)lp == BTN_Radio2Left || (HWND)lp == BTN_Radio2Right || (HWND)lp == BTN_Radio3Left || (HWND)lp == BTN_Radio3Middle || (HWND)lp == BTN_Radio3Right ||
				(HWND)lp == BTN_NormalEditboxOK || (HWND)lp == BTN_PasswordEditboxOK || (HWND)lp == BTN_MultilineEditboxOK)
			{
				RET_CTLCOLORBTN = &OBJM_Main->HBR_Secondary;
			}

			if (!RET_CTLCOLORBTN)
			{
				MessageBoxW(NULL, L"Error occurred!\n(Unresolved WM_CTLCOLORBTN message in WindowProcedure_MainContentCTR)", L"", MB_OK | MB_ICONERROR);
				nSol::cWriteLog(L"Unresolved WM_CTLCOLORBTN message in WindowProcedure_MainContentCTR.", L"", L"ERROR");
				auto temp = (HBRUSH)GetStockObject(NULL_BRUSH);
				RET_CTLCOLORBTN = &temp;
			}
			return (LRESULT)*RET_CTLCOLORBTN;
		}

		case WM_CTLCOLOREDIT:
		{
			static HBRUSH* RET_CTLCOLOREDIT = nullptr;
			RET_CTLCOLOREDIT = nullptr;

			if ((HWND)lp == ED_Normal)
			{
				if (GetFocus() != (HWND)lp) bEditboxFocusInvalidateCheck = 0;

				RET_CTLCOLOREDIT = &OBJM_Main->HBR_EditBox;
				SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
				SetBkColor((HDC)wp, OBJM_Main->CLR_EditBox);
				if (!bEditboxFocusInvalidateCheck)
					RedrawWindow(SS_ED_Normal, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
			}
			else if ((HWND)lp == ED_Password)
			{
				if (GetFocus() != (HWND)lp) bEditboxFocusInvalidateCheck = 0;

				RET_CTLCOLOREDIT = &OBJM_Main->HBR_EditBox;
				SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
				SetBkColor((HDC)wp, OBJM_Main->CLR_EditBox);
				if (!bEditboxFocusInvalidateCheck)
					RedrawWindow(SS_ED_Password, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
			}
			else if ((HWND)lp == ED_Multiline)
			{
				if (GetFocus() != (HWND)lp) bEditboxFocusInvalidateCheck = 0;

				RET_CTLCOLOREDIT = &OBJM_Main->HBR_EditBox;
				SetTextColor((HDC)wp, OBJM_Main->CLR_DefaultText);
				SetBkColor((HDC)wp, OBJM_Main->CLR_EditBox);
				if (!bEditboxFocusInvalidateCheck)
					RedrawWindow(SS_ED_Multiline, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
			}

			else if ((HWND)lp == CB_SelectTheme)
			{
				RET_CTLCOLOREDIT = &CBDL_CustomDraw1_Manager->HBR_ComboboxBackgroundColor;
				SetBkMode((HDC)wp, TRANSPARENT);
				SetBkColor((HDC)wp, CBDL_CustomDraw1_Manager->CLR_ComboboxColor_Default);
				SetTextColor((HDC)wp, CBDL_CustomDraw1_Manager->CLR_DefaultTextColor);
			}

			if (!RET_CTLCOLOREDIT)
			{
				MessageBoxW(NULL, L"Error occurred!\n(Unresolved WM_CTLCOLOREDIT message in WindowProcedure_MainContentCTR)", L"", MB_OK | MB_ICONERROR);
				nSol::cWriteLog(L"Unresolved WM_CTLCOLOREDIT message in WindowProcedure_MainContentCTR.", L"", L"ERROR");
				auto temp = (HBRUSH)GetStockObject(NULL_BRUSH);
				RET_CTLCOLOREDIT = &temp;
			}
			return (LRESULT)*RET_CTLCOLOREDIT;
		}

		case WM_CTLCOLORLISTBOX:
		{
			static HBRUSH* RET_CTLCOLORLISTBOX = nullptr;
			RET_CTLCOLORLISTBOX = nullptr;

			COMBOBOXINFO cbi{};
			cbi.cbSize = sizeof(COMBOBOXINFO);
			
			GetComboBoxInfo(CB_SelectTheme, &cbi);
			auto CBLB_SelectTheme = cbi.hwndList;
			if ((HWND)lp == CBLB_SelectTheme)
			{
				RET_CTLCOLORLISTBOX = &CBDL_CustomDraw1_Manager->HBR_ComboboxBackgroundColor;
				SetBkMode((HDC)wp, TRANSPARENT);
				SetBkColor((HDC)wp, CBDL_CustomDraw1_Manager->CLR_ComboboxDropdownListColor_Background);
				SetTextColor((HDC)wp, CBDL_CustomDraw1_Manager->CLR_InactiveTextColor);
			}

			if (!RET_CTLCOLORLISTBOX)
			{
				MessageBoxW(NULL, L"Error occurred!\n(Unresolved WM_CTLCOLORLISTBOX message in WindowProcedure_MainContentCTR)", L"", MB_OK | MB_ICONERROR);
				nSol::cWriteLog(L"Unresolved WM_CTLCOLORLISTBOX message in WindowProcedure_MainContentCTR.", L"", L"ERROR");
				auto temp = (HBRUSH)GetStockObject(NULL_BRUSH);
				RET_CTLCOLORLISTBOX = &temp;
			}
			return (LRESULT)*RET_CTLCOLORLISTBOX;
		}
	}

	return DefSubclassProc(hWnd, msg, wp, lp);
}