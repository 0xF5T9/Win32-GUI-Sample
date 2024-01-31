/**
 * @file main.cpp
 * @brief Define the application entry and window procedures.
 * @todo Implement scroll focus.
 */

#include "../Headers/standard_includes.h"
#include "../Headers/my_includes.h"

/**
 * @brief Application entry point.
 * @note READMORE: https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
 * @param hInstance Handle to an instance or handle to a module.
 * @param hPrevInstance It was used in 16-bit Windows, but is now always zero.
 * @param lpCmdLine Contains the command-line arguments as a Unicode string.
 * @param nShowCmd Flag that indicates whether the main application window is minimized, maximized, or shown normally.
 * @return The function returns an int value. The operating system doesn't use the return value, but we can use the value to pass a status code to another program.
 */
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd)
{
    if (FindWindowW(g_pApp->windowClassName, NULL))
    {
        g_pApp->logger.writeLog("The application is already running.", "", MyLogType::Info);
        return -1;
    }

    if (!g_pApp->loadConfigurationFile())
    {
        g_pApp->logger.writeLog("Failed to load the application configuration file.", "", MyLogType::Error);
        return -1;
    }

    if (!g_pApp->initialize(hInstance))
    {
        g_pApp->logger.writeLog("Failed to initialize the application.", "", MyLogType::Error);
        return -1;
    }

    {
        // Get the user desktop resolution to calculate the appropriate application window positions.
        INT desktop_width = 0, desktop_height = 0;
        if (!MyUtility::GetDesktopResolution(desktop_width, desktop_height))
        {
            g_pApp->logger.writeLog("Failed to retrieve the desktop resolution.", "", MyLogType::Error);
            return -1;
        }
        g_pApp->logger.writeLog("Current desktop resolution:", ("'" + std::to_string(desktop_width) + "x" + std::to_string(desktop_height) + "'").c_str(), MyLogType::Debug);

        // Create the application window.
        g_pApp->hWnd = CreateWindowExW(0,                       // No extended style.
                                       g_pApp->windowClassName, // Window class name.
                                       L"Win32 GUI Sample",     // Window title.
                                       WS_MYSTYLE,              // My window style.
                                       static_cast<INT>((static_cast<DOUBLE>(desktop_width) / 2) - (static_cast<DOUBLE>(g_pApp->windowWidth) / 1.4)),
                                       static_cast<INT>((static_cast<DOUBLE>(desktop_height) / 2) - (static_cast<DOUBLE>(g_pApp->windowHeight) / 1.4)), // Semi-center the application window on start.
                                       g_pApp->windowWidth, g_pApp->windowHeight,                                                                       // Initial application window dimensions.
                                       NULL, NULL, NULL, NULL);
        if (!IsWindow(g_pApp->hWnd))
        {
            g_pApp->logger.writeLog("Failed to create the application window.", "", MyLogType::Error);
            return -1;
        }
    }

    // Sets the application window and essential controls to be visible, indicating that the application is ready.
    if (!g_pApp->onReady())
    {
        g_pApp->logger.writeLog("The application window is ready, but an error occurred when attempting to show the window.", "", MyLogType::Error);
        return -1;
    }

    // Enter the message loop.
    MSG message = {0};
    while (GetMessageW(&message, NULL, 0, 0))
    {
        // Capture WM_KEYDOWN messages to check if they are dialog messages
        // and make sure `IsDialogMessage()` is called on the correct window.
        if (message.message == WM_KEYDOWN)
        {
            switch (message.wParam)
            {
            case VK_TAB:
            {
                if (IsDialogMessageW(g_pApp->hWnd, &message))
                    continue;

                break;
            }
            case VK_RETURN:
            {
                HWND parent = GetParent(message.hwnd);
                if (IsDialogMessageW((parent == g_pApp->hWnd ? g_pApp->hWnd : parent), &message))
                    continue;

                break;
            }
            default:
                break;
            }
        }

        // Keep the animation manager time up-to-date.
        if (g_pApp->pGraphic && g_pApp->pGraphic->isInitialized())
            g_pApp->pGraphic->wamEngine().updateAnimationManager();

        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    if (!g_pApp->uninitialize())
    {
        g_pApp->logger.writeLog("Failed to uninitialize the application.", "", MyLogType::Error);
        return -1;
    }

    // Write a log record indicating that the application has exited successfully.
    g_pApp->logger.writeLog("Finished exiting.", true);

    return 0;
}

/**
 * @brief Application window procedure.
 * @param hWnd The handle to the window.
 * @param message The message identifier.
 * @param wParam The first message parameter.
 * @param lParam The second message parameter.
 * @return The result of the message processing.
 */
LRESULT CALLBACK ApplicationWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "Failed to retrieve the window client rect.";
                break;
            }

            auto &p_rendertarget = *g_pApp->pUIManager->pRenderTarget;
            hr = p_rendertarget->BindDC(hdc, &rect_window);
            if (FAILED(hr))
            {
                error_message = "Failed to bind the render target to the window device context.";
                break;
            }

            D2D1_RECT_F d2d1_rect_window = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window.right), static_cast<FLOAT>(rect_window.bottom));
            D2D1_RECT_F d2d1_rect_caption = D2D1::RectF(static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.left), static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.top), static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.right), static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.bottom));
            auto &d2d1_color_background = g_pApp->pUIManager->colors.background.getD2D1Color();
            auto &p_d2d1_solidcolorbrush_caption = *g_pApp->pUIManager->pSolidColorBrushCaptionBackground;
            auto &p_d2d1_solidcolorbrush_border = (g_pApp->isWindowActive ? *g_pApp->pUIManager->pSolidColorBrushBorderActive : *g_pApp->pUIManager->pSolidColorBrushBorderInactive);

            p_rendertarget->BeginDraw();

            {
                // Draw the window background.
                p_rendertarget->Clear(d2d1_color_background);

                // Draw the caption background.
                g_pApp->pGraphic->d2d1Engine().drawFillRectangle(p_rendertarget, d2d1_rect_caption, p_d2d1_solidcolorbrush_caption);

                // Draw the window border.
                g_pApp->pGraphic->d2d1Engine().drawRectangle(p_rendertarget, d2d1_rect_window, p_d2d1_solidcolorbrush_border);
            }

            hr = p_rendertarget->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!g_pApp->pUIManager->createDeviceResources())
                {
                    error_message = "Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue;
            }
            else if (FAILED(hr))
            {
                error_message = "Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[MESSAGE: 'WM_PAINT' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
    {
        // If the application is not ready yet, erase the background manually.
        // This effectively reduces unwanted visual artifacts during resizing and makes the application window appear more smoothly when being opened.
        if (!g_pApp->isWindowReady)
        {
            USHORT paint_attempts = 1;
            HDC hdc = reinterpret_cast<HDC>(wParam);

            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                HRESULT hr;

                // Get the window client rect.
                RECT rect_window;
                if (!GetClientRect(hWnd, &rect_window))
                {
                    error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                    break;
                }

                auto &p_rendertarget = *g_pApp->pUIManager->pRenderTarget;
                hr = p_rendertarget->BindDC(hdc, &rect_window);
                if (FAILED(hr))
                {
                    error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                    break;
                }

                D2D1_RECT_F d2d1_rect_window = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window.right), static_cast<FLOAT>(rect_window.bottom));
                D2D1_RECT_F d2d1_rect_caption = D2D1::RectF(static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.left), static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.top), static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.right), static_cast<FLOAT>(g_pApp->pUIManager->miscs.rectCaption.bottom));
                auto &d2d1_color_background = g_pApp->pUIManager->colors.background.getD2D1Color();
                auto &p_d2d1_solidcolorbrush_caption = *g_pApp->pUIManager->pSolidColorBrushCaptionBackground;
                auto &p_d2d1_solidcolorbrush_border = (g_pApp->isWindowActive ? *g_pApp->pUIManager->pSolidColorBrushBorderActive : *g_pApp->pUIManager->pSolidColorBrushBorderInactive);

                // Begin drawing.
                p_rendertarget->BeginDraw();

                {
                    // Draw the window background.
                    p_rendertarget->Clear(d2d1_color_background);

                    // Draw the caption background.
                    g_pApp->pGraphic->d2d1Engine().drawFillRectangle(p_rendertarget, d2d1_rect_caption, p_d2d1_solidcolorbrush_caption);

                    // Draw the window border.
                    g_pApp->pGraphic->d2d1Engine().drawRectangle(p_rendertarget, d2d1_rect_window, p_d2d1_solidcolorbrush_border);
                }

                hr = p_rendertarget->EndDraw();
                if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
                {
                    if (!g_pApp->pUIManager->createDeviceResources())
                    {
                        error_message = "Failed to create the device resources.";
                        break;
                    }
                    if (paint_attempts > 10)
                    {
                        error_message = "Failed to create the device resources after 10 attempts.";
                        break;
                    }
                    paint_attempts++;
                    continue;
                }
                else if (FAILED(hr))
                {
                    error_message = "Failed to end drawing.";
                    break;
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
                g_pApp->logger.writeLog(error_message, "[MESSAGE: 'WM_ERASEBKGND' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
        }

        // Always return non-zero to indicate that no further erasing is required, avoid flickering.
        return 1;
    }
    case WM_CTLCOLORSTATIC:
    {
        const HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        // INT window_id = GetDlgCtrlID(window);

        if (MyStandardTextSubclass::getSubclassPointer(window))
        {
            p_background_brush = &g_pApp->pUIManager->colors.nullBrush;
        }
        else
        {
            // Default processing.
            SetBkColor(hdc, g_pApp->pUIManager->colors.background.getCOLORREF());
            SetTextColor(hdc, g_pApp->pUIManager->colors.textActive.getCOLORREF());
            p_background_brush = &g_pApp->pUIManager->colors.background.getSolidBrush();
        }

        if (!p_background_brush)
            break;
        return reinterpret_cast<LRESULT>(*p_background_brush);
    }
    case WM_CTLCOLORBTN:
    {
        HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        // INT window_id = GetDlgCtrlID(window);

        if (MyStandardButtonSubclass::getSubclassPointer(window) ||
            MyImageButtonSubclass::getSubclassPointer(window) ||
            MyRadioButtonSubclass::getSubclassPointer(window))
        {
            p_background_brush = &g_pApp->pUIManager->colors.nullBrush;
        }
        else
        {
            SetBkColor(hdc, g_pApp->pUIManager->colors.background.getCOLORREF());
            SetTextColor(hdc, g_pApp->pUIManager->colors.textActive.getCOLORREF());
        }

        if (!p_background_brush)
            break;
        return reinterpret_cast<LRESULT>(*p_background_brush);
    }
    case WM_CREATE:
    {
        // Create application window controls.

        if (!g_pApp->onCreate(hWnd))
        {
            g_pApp->logger.writeLog("Unexpected error occurred.", "[MESSAGE: 'WM_CREATE' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
            return -1;
        }

        return 0;
    }
    case WM_COMMAND:
    {
        // Process command messages.

        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
        {
            switch (LOWORD(wParam))
            {
            case IDC_NONCLIENT_CLOSE_BUTTON:
            {
                if (!g_pApp->onExit())
                    g_pApp->logger.writeLog("Unexpected error occurred.", "[MESSAGE: 'WM_COMMAND' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
                DestroyWindow(hWnd);
                return 0;
            }
            case IDC_NONCLIENT_MAXIMIZE_BUTTON:
            {
                if (!g_pApp->isWindowMaximized)
                {
                    g_pApp->isWindowMaximized = true;
                    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
                }
                else
                {
                    g_pApp->isWindowMaximized = false;
                    ShowWindow(hWnd, SW_RESTORE);
                }
                return 0;
            }
            case IDC_NONCLIENT_MINIMIZE_BUTTON:
            {
                g_pApp->isWindowMinimized = true;
                ShowWindow(hWnd, SW_SHOWMINIMIZED);
                return 0;
            }
            }
            break;
        }
        }

        break;
    }
    case WM_ACTIVATE:
    {
        // Update window minimized state.
        g_pApp->isWindowMinimized = (HIWORD(wParam) ? true : false);

        switch (LOWORD(wParam))
        {
        case WA_CLICKACTIVE:
        case WA_ACTIVE:
        {
            // Update the appropriate border color.
            g_pApp->isWindowActive = true;

            // If supported, set the border color attribute.
            if (g_pApp->isWindows11BorderAttributeSupported)
            {
                COLORREF border_color = g_pApp->pUIManager->colors.borderActive.getCOLORREF();
                HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
                if (FAILED(hr))
                    g_pApp->logger.writeLog("Failed to set the window border attribute.", "[MESSAGE: 'WM_ACTIVATE/WA_CLICKACTIVE||WA_ACTIVE' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
            }

            // Update window title color.
            for (auto &window : g_pApp->vNonClientWindows)
            {
                if (GetDlgCtrlID(window->hWnd()) == IDC_NONCLIENT_CAPTIONTITLE_STATIC)
                {
                    auto p_subclass = MyStandardTextSubclass::getSubclassPointer(window->hWnd());
                    if (p_subclass)
                        p_subclass->textConfig.pTextColor = &g_pApp->pUIManager->colors.textActive;
                    break;
                }
            }

            break;
        }

        case WA_INACTIVE:
        {
            // Update the appropriate border color.
            g_pApp->isWindowActive = false;

            // If supported, set the border color attribute.
            if (g_pApp->isWindows11BorderAttributeSupported)
            {
                COLORREF border_color = g_pApp->pUIManager->colors.borderInactive.getCOLORREF();
                HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
                if (FAILED(hr))
                    g_pApp->logger.writeLog("Failed to set the window border attribute.", "[MESSAGE: 'WM_ACTIVATE/WA_INACTIVE' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
            }

            // Update window title color.
            for (auto &window : g_pApp->vNonClientWindows)
            {
                if (GetDlgCtrlID(window->hWnd()) == IDC_NONCLIENT_CAPTIONTITLE_STATIC)
                {
                    auto p_subclass = MyStandardTextSubclass::getSubclassPointer(window->hWnd());
                    if (p_subclass)
                        p_subclass->textConfig.pTextColor = &g_pApp->pUIManager->colors.textInactive;
                    break;
                }
            }

            break;
        }
        }

        // Redraw the borders.
        RedrawWindow(hWnd, &g_pApp->pUIManager->miscs.rectSizeBorderLeft, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(hWnd, &g_pApp->pUIManager->miscs.rectSizeBorderTop, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(hWnd, &g_pApp->pUIManager->miscs.rectSizeBorderRight, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(hWnd, &g_pApp->pUIManager->miscs.rectSizeBorderBottom, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

        // Redraw non-client window controls.
        for (const auto &window : g_pApp->vNonClientWindows)
            RedrawWindow(window->hWnd(), nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

        return 0;
    }
    case WM_NCCALCSIZE:
    {
        // Process WM_NCCALCSIZE messages to perform additional operations to adjust the maximize rect.
        // Prevent the application window from spilling over monitor edges when maximized.
        // Source: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
        if (wParam == TRUE)
        {
            auto &parameters = *reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
            g_pApp->adjustMaximizedClientRect(hWnd, parameters.rgrc[0]);
            return 0;
        }

        // Always return zero to remove the standard window frames.
        // Remark: The application uses custom-drawn caption bars and borders. Everything is drawn in the client area.
        return 0;
    }
    case WM_NCHITTEST:
    {
        // Handle appropriated events for custom-drawn caption bars and borders.

        // Get the default WM_NCHITTEST response.
        const LRESULT default_result = ::DefWindowProcW(hWnd, message, wParam, lParam);
        const LONG extra_sizeborder_thickness = 10;

        // Get caption bar and size border rects.
        RECT rect_caption = g_pApp->pUIManager->miscs.rectCaption,
             rect_border_left = g_pApp->pUIManager->miscs.rectSizeBorderLeft,
             rect_border_top = g_pApp->pUIManager->miscs.rectSizeBorderTop,
             rect_border_right = g_pApp->pUIManager->miscs.rectSizeBorderRight,
             rect_border_bottom = g_pApp->pUIManager->miscs.rectSizeBorderBottom,
             rect_border_bottom_right = rect_border_bottom,
             rect_border_bottom_left = rect_border_bottom,
             rect_border_top_left = rect_border_top,
             rect_border_top_right = rect_border_top;

        // Calculate the grab rectangle areas.
        rect_border_bottom.right -= 50;
        rect_border_bottom.left += 50;
        rect_border_right.bottom -= 50;
        rect_border_right.top += 75;
        rect_border_left.bottom -= 50;
        rect_border_top.left += 50;
        rect_border_top.right -= 116;
        rect_border_left.top += 37;
        rect_border_bottom_right.left = rect_border_bottom.right;
        rect_border_bottom_right.top = rect_border_right.bottom;
        rect_border_bottom_left.right = rect_border_bottom.left;
        rect_border_bottom_left.top = rect_border_left.bottom;
        rect_border_top_left.right = rect_border_top.left;
        rect_border_top_left.bottom = rect_border_left.top;
        rect_border_top_right.left = rect_border_top.right;
        rect_border_top_right.bottom = rect_border_right.top;

        // Add extra size border thickness, make the size borders easier to grab.
        rect_border_right.left -= extra_sizeborder_thickness;
        rect_border_bottom.top -= extra_sizeborder_thickness;
        rect_border_left.right += extra_sizeborder_thickness;
        rect_border_top.bottom += extra_sizeborder_thickness;

        // Map the window points.
        POINT point;
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_caption), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_top), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_bottom), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_left), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_right), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_bottom_right), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_bottom_left), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_top_left), (sizeof(RECT) / sizeof(POINT)));
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT *>(&rect_border_top_right), (sizeof(RECT) / sizeof(POINT)));

        // Handling size border events. (ignore if the application window is maximized)
        if (!g_pApp->isWindowMaximized)
        {
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_bottom_right, point)))
                return HTBOTTOMRIGHT;
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_bottom_left, point)))
                return HTBOTTOMLEFT;
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_top_left, point)))
                return HTTOPLEFT;
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_top_right, point)))
                return HTTOPRIGHT;
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_left, point)))
                return HTLEFT;
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_top, point)))
                return HTTOP;
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_right, point)))
                return HTRIGHT;
            if ((default_result == HTCLIENT) && (PtInRect(&rect_border_bottom, point)))
                return HTBOTTOM;
        }

        // Handling caption bar events.
        if ((default_result == HTCLIENT) && (PtInRect(&rect_caption, point)))
            return HTCAPTION;

        return default_result;
    }
    case WM_SIZE:
    {
        // Update the global dimension variables and rectangles.
        {
            // Global dimension variables.
            g_pApp->windowWidth = static_cast<INT>(LOWORD(lParam));
            g_pApp->windowHeight = static_cast<INT>(HIWORD(lParam));

            // Global dimension rectangles.
            g_pApp->pUIManager->miscs.rectCaption.right = g_pApp->windowWidth - WINDOW_BORDER_DEFAULTWIDTH;
            g_pApp->pUIManager->miscs.rectSizeBorderLeft.bottom = g_pApp->windowHeight - WINDOW_BORDER_DEFAULTWIDTH;
            g_pApp->pUIManager->miscs.rectSizeBorderTop.right = g_pApp->windowWidth;
            g_pApp->pUIManager->miscs.rectSizeBorderRight.left = g_pApp->windowWidth - WINDOW_BORDER_DEFAULTWIDTH;
            g_pApp->pUIManager->miscs.rectSizeBorderRight.right = g_pApp->windowWidth;
            g_pApp->pUIManager->miscs.rectSizeBorderRight.bottom = g_pApp->windowHeight - WINDOW_BORDER_DEFAULTWIDTH;
            g_pApp->pUIManager->miscs.rectSizeBorderBottom.right = g_pApp->windowWidth;
            g_pApp->pUIManager->miscs.rectSizeBorderBottom.top = g_pApp->windowHeight - WINDOW_BORDER_DEFAULTWIDTH;
            g_pApp->pUIManager->miscs.rectSizeBorderBottom.bottom = g_pApp->windowHeight;
        }

        // Stop all animations during resize event.
        {
            HRESULT hr;

            // Windows Animation Manager animations.
            if (g_pApp && g_pApp->pGraphic && g_pApp->pGraphic->isInitialized())
            {
                hr = g_pApp->pGraphic->wamEngine().manager()->FinishAllStoryboards(0);
                if (FAILED(hr))
                    g_pApp->logger.writeLog("Failed to finish all the active storyboards.", "[MESSAGE: 'WM_SIZE' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
            }
        }

        // Update window states.
        g_pApp->isWindowMaximized = (wParam == SIZE_MAXIMIZED ? true : false);
        g_pApp->isWindowMinimized = (wParam == SIZE_MINIMIZED ? true : false);

        // If the application window is maximized, perform an additional redraw on the containers.
        if (wParam == SIZE_MAXIMIZED && g_pApp && g_pApp->isWindowReady)
        {
            for (auto &[id, p_container] : g_pApp->mContainers)
                RedrawWindow(p_container->container()->hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
        }

        // Redraw the application window and trigger WM_WINDOWPOSCHANGING message to update application controls dimensions.
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

        return 0;
    }
    case WM_WINDOWPOSCHANGING:
    {
        if (!g_pApp)
            break;

        // Get the pointer to the WINDOWPOS structure and check if the application window size is changed.
        WINDOWPOS *p_window_pos = reinterpret_cast<WINDOWPOS *>(lParam);
        bool is_window_size_changed = (((p_window_pos->cx != g_pApp->windowWidth) && p_window_pos->cx) || ((p_window_pos->cy != g_pApp->windowHeight) && p_window_pos->cy) ? true : false);
        bool reset_containers_scroll_position = is_window_size_changed && !IsIconic(hWnd) && !g_pApp->isWindowMinimized;

        // Update the dimensions of the containers.
        if (g_pApp->isWindowReady)
        {
            for (auto &[id, p_container] : g_pApp->mContainers)
            {
                switch (id)
                {
                case IDC_DC_CONTAINER:
                {
                    if (!p_container->updateContainerDimensions(WINDOW_BORDER_DEFAULTWIDTH,
                                                                g_pApp->pUIManager->miscs.rectCaption.bottom,
                                                                g_pApp->windowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2),
                                                                g_pApp->windowHeight - (WINDOW_BORDER_DEFAULTWIDTH * 2) - (g_pApp->pUIManager->miscs.rectCaption.bottom - g_pApp->pUIManager->miscs.rectCaption.top), g_pApp->isAlwaysHideScrollbars, reset_containers_scroll_position))
                    {
                        g_pApp->logger.writeLog("Failed to update the test container's dimensions.", "[MESSAGE: 'WM_SIZE' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
                    }

                    auto p_scrollbar = p_container->verticalScrollbar();
                    if (p_scrollbar)
                    {
                        auto p_subclass = MyVerticalScrollbarSubclass::getSubclassPointer(p_scrollbar->hWnd());
                        if (p_subclass && IsWindowVisible(p_subclass->getStaticHandle()))
                            RedrawWindow(p_subclass->getStaticHandleRef(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }

        // Update the non-client controls dimensions all at once.
        HDWP hdwp_nonclient_controls = BeginDeferWindowPos(3);
        DeferWindowPos(hdwp_nonclient_controls, g_pApp->pUIManager->miscs.pCloseButton->hWnd(), NULL, g_pApp->windowWidth - WINDOW_BORDER_DEFAULTWIDTH - 58, WINDOW_BORDER_DEFAULTWIDTH, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        DeferWindowPos(hdwp_nonclient_controls, g_pApp->pUIManager->miscs.pMaximizeButton->hWnd(), NULL, g_pApp->windowWidth - WINDOW_BORDER_DEFAULTWIDTH - 58 - 58, WINDOW_BORDER_DEFAULTWIDTH, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        DeferWindowPos(hdwp_nonclient_controls, g_pApp->pUIManager->miscs.pMinimizeButton->hWnd(), NULL, g_pApp->windowWidth - WINDOW_BORDER_DEFAULTWIDTH - 58 - 58 - 58, WINDOW_BORDER_DEFAULTWIDTH, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        if (!EndDeferWindowPos(hdwp_nonclient_controls))
            g_pApp->logger.writeLog("Failed to update the non-client controls dimensions", "[MESSAGE: 'WM_SIZE' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);

        break;
    }
    case WM_ENTERSIZEMOVE:
    {
        // Keep track of whether the application is entering a resizing event.
        if (!g_pApp->isWindowResizing)
            g_pApp->isWindowResizing = true;

        break;
    }
    case WM_EXITSIZEMOVE:
    {
        // keep track of whether the application is exiting a resizing event.
        if (g_pApp->isWindowResizing)
            g_pApp->isWindowResizing = false;

        RedrawWindow(g_pApp->pUIManager->miscs.pCloseButton->hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(g_pApp->pUIManager->miscs.pMaximizeButton->hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(g_pApp->pUIManager->miscs.pMinimizeButton->hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        break;
    }
    case WM_GETMINMAXINFO:
    {
        // Set the minimum dimensions that the application window can be resized to.

        LPMINMAXINFO p_minmax_info = reinterpret_cast<LPMINMAXINFO>(lParam);

        p_minmax_info->ptMinTrackSize.x = 527;
        p_minmax_info->ptMinTrackSize.y = 200;

        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        // Handle mouse scrolling events.

        if (!g_pApp || !g_pApp->isWindowReady)
            break;

        auto p_defaultcontainer = g_pApp->findContainer(IDC_DC_CONTAINER);
        if (p_defaultcontainer && p_defaultcontainer->verticalScrollbar())
        {
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                bool is_scrolling_downward = false;                                     // Indicate whether scrolling is occurring in a downward direction.
                int scroll_pixel = (p_defaultcontainer->enableSmoothScroll ? 100 : 50), // Number of pixels to scroll per scroll.
                    scroll_delta = 0,                                                   // The actual number of pixels that will be scrolled per scroll event.
                    wheel_delta = 0;                                                    // Wheel Delta.
                auto p_subclass_scrollbar_window = static_cast<MyVerticalScrollbarSubclass *>(p_defaultcontainer->verticalScrollbar()->data());

                // Get the container scroll information.
                SCROLLINFO scroll_info;
                if (!p_subclass_scrollbar_window->getScrollInfo(scroll_info))
                {
                    error_message = "Failed to retrieve the container scroll information.";
                    break;
                }

                // If the container's vertical dimension is big enough to show all the container contents, no scrolling needed.
                if (static_cast<INT>(scroll_info.nPage) > scroll_info.nMax)
                    return 0;

                // Get wheel data.
                wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);

                // Get the scroll parameters.
                if (wheel_delta < 0)
                {
                    // The scrolling is occurring in a downward direction.
                    is_scrolling_downward = true;
                    scroll_delta = scroll_pixel * -(wheel_delta / WHEEL_DELTA);
                }
                else
                {
                    // The scrolling is occurring in an upward direction.
                    is_scrolling_downward = false;
                    scroll_delta = scroll_pixel * (wheel_delta / WHEEL_DELTA);
                }

                // Scroll the container window.
                if (p_defaultcontainer->enableSmoothScroll)
                {
                    if (!p_subclass_scrollbar_window->scrollWindowByAmountSmooth(is_scrolling_downward ? scroll_delta : -scroll_delta))
                    {
                        error_message = "Failed to scroll the container window.";
                        break;
                    }
                }
                else
                {
                    if (!p_subclass_scrollbar_window->scrollWindowByAmount(is_scrolling_downward ? scroll_delta : -scroll_delta))
                    {
                        error_message = "Failed to scroll the container window.";
                        break;
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
                g_pApp->logger.writeLog(error_message, "[MESSAGE: 'WM_MOUSEWHEEL' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);

            return 0;
        }

        break;
    }
    case WM_LBUTTONDOWN:
    {
        // Set focus to the main window on background click.
        SetFocus(hWnd);
        return 0;
    }
    case WM_KEYUP:
    {
        switch (wParam)
        {
        // Change application theme.
        case VK_F9:
        {
            // Change to the next theme.
            bool is_theme_changed = false;
            std::string error_message = "";
            INT change_theme_combobox_index = 0;
            switch (g_pApp->windowTheme)
            {
            case MyTheme::Light:
            {
                // Update application theme.
                if (!g_pApp->setAppTheme(MyTheme::Dark))
                {
                    error_message = "Failed to update application theme.";
                    break;
                }

                // Update the settings file.
                if (!g_pApp->updateConfigurationFile("theme", "Dark"))
                {
                    error_message = "Failed to update the settings file.";
                    break;
                }

                change_theme_combobox_index = 1;
                is_theme_changed = true;
                break;
            }
            case MyTheme::Dark:
            {
                // Update application theme.
                if (!g_pApp->setAppTheme(MyTheme::Monokai))
                {
                    error_message = "Failed to update application theme.";
                    break;
                }

                // Update the settings file.
                if (!g_pApp->updateConfigurationFile("theme", "Monokai"))
                {
                    error_message = "Failed to update the settings file.";
                    break;
                }

                change_theme_combobox_index = 2;
                is_theme_changed = true;
                break;
            }
            case MyTheme::Monokai:
            {
                // Update application theme.
                if (!g_pApp->setAppTheme(MyTheme::Light))
                {
                    error_message = "Failed to update application theme.";
                    break;
                }

                // Update the settings file.
                if (!g_pApp->updateConfigurationFile("theme", "Light"))
                {
                    error_message = "Failed to update the settings file.";
                    break;
                }

                change_theme_combobox_index = 0;
                is_theme_changed = true;
                break;
            }
            }

            // Check if the theme has changed successfully.
            if (!is_theme_changed)
            {
                g_pApp->logger.writeLog(error_message, "[MESSAGE: 'WM_KEYUP/F5' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
            }
            else
            {
                auto p_container = g_pApp->findContainer(IDC_DC_CONTAINER);
                if (p_container)
                {
                    auto p_combobox_window = p_container->findWindow(IDC_DC_COMBOBOXSELECTTHEME);
                    if (p_combobox_window)
                        ComboBox_SetCurSel(p_combobox_window->hWnd(), change_theme_combobox_index);
                }
            }

            return 0;
        }
        }
        break;
    }
    case WM_CLOSE:
    {
        g_pApp->onExit();
        DestroyWindow(hWnd);
        return 0;
    }
    case WM_DESTROY:
    {
        if (!g_pApp->onDestroy())
            g_pApp->logger.writeLog("The application failed to terminate properly.", "[MESSAGE: 'WM_DESTROY' | CALLBACK: 'ApplicationWindowProcedure()']", MyLogType::Error);
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

/**
 * @brief Default container window procedure.
 * @param hWnd The handle to the window.
 * @param message The message identifier.
 * @param wParam The first message parameter.
 * @param lParam The second message parameter.
 * @param idSubclass The subclass id.
 * @param refData The reference data.
 * @return The result of the message processing.
 */
LRESULT CALLBACK DefaultContainerProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR idSubclass, DWORD_PTR refData)
{
    switch (message)
    {
    case WM_ERASEBKGND:
        return 1;
    case WM_CTLCOLORSTATIC:
    {
        const HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        // INT window_id = GetDlgCtrlID(window);

        if (MyUtility::GetWindowClassNameWideString(window) == WC_EDIT)
        {
            SetBkColor(hdc, g_pApp->pUIManager->colors.background.getCOLORREF());
            SetTextColor(hdc, g_pApp->pUIManager->colors.textInactive.getCOLORREF());
            p_background_brush = &g_pApp->pUIManager->colors.background.getSolidBrush();
        }
        else if (MyStandardTextSubclass::getSubclassPointer(window))
        {
            p_background_brush = &g_pApp->pUIManager->colors.nullBrush;
        }
        else
        {
            SetBkColor(hdc, g_pApp->pUIManager->colors.background.getCOLORREF());
            SetTextColor(hdc, g_pApp->pUIManager->colors.textActive.getCOLORREF());
            p_background_brush = &g_pApp->pUIManager->colors.background.getSolidBrush();
        }

        if (!p_background_brush)
            break;
        return reinterpret_cast<LRESULT>(*p_background_brush);
    }
    case WM_MEASUREITEM:
    {
        // Set the dimensions of owner-drawn controls and menus.

        // Get the pointer to the measure item structure.
        LPMEASUREITEMSTRUCT measure_items_struct = reinterpret_cast<LPMEASUREITEMSTRUCT>(lParam);

        // Check if the message is sent by a menu.
        if (!wParam && measure_items_struct->CtlType == ODT_MENU)
        {
            // The application doesn't have any menu, always use default processing.
            g_pApp->logger.writeLog("Unexpected message.", "[MESSAGE: 'WM_MEASUREITEM/ODT_MENU' | CALLBACK: 'testContainerProcedure()']", MyLogType::Warn);
            break;
        }

        // Check if the message is sent by other types.
        switch (measure_items_struct->CtlType)
        {
        // Set item metrics for comboboxes.
        case ODT_COMBOBOX:
        {
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                // Set to zero by default since this will be overridden by combobox subclass classes.
                measure_items_struct->itemHeight = 0;

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                g_pApp->logger.writeLog(error_message, "[MESSAGE: 'WM_MEASUREITEM/ODT_COMBOBOX' | CALLBACK: 'testContainerProcedure()']", MyLogType::Error);
            }

            return TRUE;
        }

        // Set item metrics for listboxes/listviews.
        case ODT_LISTBOX:
        case ODT_LISTVIEW:
        {
            // The application doesn't have any listbox/listview samples yet, always use default processing.
            g_pApp->logger.writeLog("Unexpected message.", "[MESSAGE: 'WM_MEASUREITEM/ODT_LISTBOX||ODT_LISTVIEW' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Warn);
            break;
        }
        }

        break;
    }
    case WM_DRAWITEM:
    {
        // Draw the content of owner-drawn controls and menus.

        // Get the pointer to the draw items structure.
        LPDRAWITEMSTRUCT draw_items_struct = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

        switch (wParam)
        {
        // Check if the message is sent by a menu.
        case 0:
        {
            // The application doesn't have any menu samples, always use default processing.
            g_pApp->logger.writeLog("Unexpected message.", "[MESSAGE: 'WM_DRAWITEM/0' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Warn);
            break;
        }
        // Priority processings.
        // ...

        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                bool message_processed_successfully = false;
                switch (draw_items_struct->CtlType)
                {
                // Comboboxes.
                case ODT_COMBOBOX:
                {
                    // Subclassed controls.
                    if (MyDDLComboboxSubclass::getSubclassPointer(draw_items_struct->hwndItem))
                    {
                        // Check if the combobox is empty or system run out of memory (very unlikely).
                        if (draw_items_struct->itemID == static_cast<UINT>(CB_ERR))
                            return TRUE;

                        // Get the combobox item text length.
                        INT text_length = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXTLEN, draw_items_struct->itemID, 0));
                        if (text_length == CB_ERR)
                        {
                            error_message = "Failed to retrieve the combobox item text length.";
                            break;
                        }

                        // Get the combobox item text.
                        WCHAR *p_text_buffer = new WCHAR[static_cast<size_t>(text_length) + 1U];
                        if (SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXT, draw_items_struct->itemID, reinterpret_cast<LPARAM>(p_text_buffer)) == CB_ERR)
                        {
                            delete[] p_text_buffer;
                            error_message = "Failed to retrieve the combobox item text.";
                            break;
                        }
                        std::wstring text_string(p_text_buffer);
                        delete[] p_text_buffer;

                        // Prepare COLORREF variables for the background and text colors.
                        COLORREF background_color = g_pApp->pUIManager->colors.ddlComboboxItemBackground.getCOLORREF();
                        COLORREF text_color = g_pApp->pUIManager->colors.ddlComboboxItemTextDefault.getCOLORREF();

                        if (draw_items_struct->itemAction & ODA_DRAWENTIRE)
                        {
                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = g_pApp->pUIManager->colors.ddlComboboxItemBackground.getCOLORREF();
                                text_color = g_pApp->pUIManager->colors.ddlComboboxItemTextDefault.getCOLORREF();
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = g_pApp->pUIManager->colors.ddlComboboxSelectedItemBackground.getCOLORREF();
                                text_color = g_pApp->pUIManager->colors.ddlComboboxItemTextSelected.getCOLORREF();
                            }

                            SetBkMode(draw_items_struct->hDC, OPAQUE);
                            SetBkColor(draw_items_struct->hDC, background_color);
                            SetTextColor(draw_items_struct->hDC, text_color);

                            ExtTextOutW(draw_items_struct->hDC,
                                        draw_items_struct->rcItem.left + 5, draw_items_struct->rcItem.top + 5,
                                        ETO_OPAQUE | ETO_CLIPPED, &draw_items_struct->rcItem,
                                        text_string.c_str(), static_cast<UINT>(text_length), NULL);
                        }
                        else if (draw_items_struct->itemAction & ODA_SELECT)
                        {
                            // If new item is selected (ODA_SELECT) and the combobox is dropped down, redraw the combobox to reflect the new selection.
                            if (ComboBox_GetDroppedState(draw_items_struct->hwndItem))
                                RedrawWindow(draw_items_struct->hwndItem, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = g_pApp->pUIManager->colors.ddlComboboxItemBackground.getCOLORREF();
                                text_color = g_pApp->pUIManager->colors.ddlComboboxItemTextDefault.getCOLORREF();
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = g_pApp->pUIManager->colors.ddlComboboxSelectedItemBackground.getCOLORREF();
                                text_color = g_pApp->pUIManager->colors.ddlComboboxItemTextSelected.getCOLORREF();
                            }

                            SetBkMode(draw_items_struct->hDC, OPAQUE);
                            SetBkColor(draw_items_struct->hDC, background_color);
                            SetTextColor(draw_items_struct->hDC, text_color);

                            ExtTextOutW(draw_items_struct->hDC,
                                        draw_items_struct->rcItem.left + 5, draw_items_struct->rcItem.top + 5,
                                        ETO_OPAQUE | ETO_CLIPPED, &draw_items_struct->rcItem,
                                        text_string.c_str(), static_cast<UINT>(text_length), NULL);
                        }
                        else if (draw_items_struct->itemAction & ODA_FOCUS)
                        {
                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = g_pApp->pUIManager->colors.ddlComboboxItemBackground.getCOLORREF();
                                text_color = g_pApp->pUIManager->colors.ddlComboboxItemTextDefault.getCOLORREF();
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = g_pApp->pUIManager->colors.ddlComboboxSelectedItemBackground.getCOLORREF();
                                text_color = g_pApp->pUIManager->colors.ddlComboboxItemTextSelected.getCOLORREF();
                            }

                            SetBkMode(draw_items_struct->hDC, OPAQUE);
                            SetBkColor(draw_items_struct->hDC, background_color);
                            SetTextColor(draw_items_struct->hDC, text_color);

                            ExtTextOutW(draw_items_struct->hDC,
                                        draw_items_struct->rcItem.left + 5, draw_items_struct->rcItem.top + 5,
                                        ETO_OPAQUE | ETO_CLIPPED, &draw_items_struct->rcItem,
                                        text_string.c_str(), static_cast<UINT>(text_length), NULL);
                        }

                        // Get the combobox drop down list HWND.
                        COMBOBOXINFO combobox_info{};
                        combobox_info.cbSize = sizeof(COMBOBOXINFO);
                        if (!GetComboBoxInfo(draw_items_struct->hwndItem, &combobox_info))
                        {
                            error_message = "Failed to retrieve the combobox info.";
                            break;
                        }
                        HWND hwnd_dropdownlist = combobox_info.hwndList;

                        // Get the combobox drop down list's client rect.
                        RECT rect_dropdownlist;
                        if (!hwnd_dropdownlist || !GetClientRect(hwnd_dropdownlist, &rect_dropdownlist))
                        {
                            error_message = "Failed to retrieve the combobox drop down list's client rect.";
                            break;
                        }

                        // Get the combobox drop down list's device context.
                        HDC hdc = GetWindowDC(hwnd_dropdownlist);
                        if (!hdc)
                        {
                            error_message = "Failed to retrieve the combobox drop down list's device context.";
                            break;
                        }

                        // Get the total items of the combobox to get the ID of the last item.
                        INT total_combobox_items = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETCOUNT, 0, 0));
                        if (total_combobox_items == CB_ERR)
                        {
                            error_message = "Failed to retrieve the total combobox items.";
                            break;
                        }

                        // Get current selectet combobox item index.
                        INT current_selected_combobox_item = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETCURSEL, 0, 0));
                        if (current_selected_combobox_item == CB_ERR)
                        {
                            error_message = "Failed to retrieve the current selected combobox item index.";
                            break;
                        }

                        // Fill the 2-pixels gap to match the color of the last item background color.
                        RECT rect_dropdownlist_2 = rect_dropdownlist;
                        rect_dropdownlist_2.top = --rect_dropdownlist_2.bottom - 1;
                        FillRect(hdc, &rect_dropdownlist_2,
                                 (current_selected_combobox_item == (total_combobox_items - 1)
                                      ? g_pApp->pUIManager->colors.ddlComboboxSelectedItemBackground.getSolidBrush()
                                      : g_pApp->pUIManager->colors.ddlComboboxItemBackground.getSolidBrush()));

                        // Draw the drop down list borders.
                        FrameRect(hdc, &rect_dropdownlist, g_pApp->pUIManager->colors.ddlComboboxDropdownlistBorder.getSolidBrush());

                        // Release the combobox drop down list's device context.
                        if (!ReleaseDC(hwnd_dropdownlist, hdc))
                        {
                            error_message = "Failed to release the combobox drop down list's device context.";
                            break;
                        }

                        message_processed_successfully = true;
                    }
                    // Standard controls.
                    // Note: Add the processing of non-subclassed controls anyway for the sake of demonstration.
                    else
                    {
                        // Check if item ID is valid.
                        if (draw_items_struct->itemID == static_cast<UINT>(CB_ERR))
                        {
                            error_message = "Invalid item ID or system ran out of memory.";
                            break;
                        }

                        // Get the combobox item text length.
                        INT text_length = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXTLEN, draw_items_struct->itemID, 0));
                        if (text_length == CB_ERR)
                        {
                            error_message = "Failed to retrieve the combobox item text length.";
                            break;
                        }

                        // Get the combobox item text.
                        WCHAR *p_text_buffer = new WCHAR[static_cast<size_t>(text_length) + 1U];
                        if (SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXT, draw_items_struct->itemID, reinterpret_cast<LPARAM>(p_text_buffer)) == CB_ERR)
                        {
                            delete[] p_text_buffer;
                            error_message = "Failed to retrieve the combobox item text.";
                            break;
                        }
                        std::wstring text_string(p_text_buffer);
                        delete[] p_text_buffer;

                        // Prepare COLORREF variables for the background and text colors.
                        COLORREF background_color = RGB(255, 255, 255);
                        COLORREF text_color = GetSysColor(COLOR_WINDOWTEXT);

                        if (draw_items_struct->itemAction & ODA_DRAWENTIRE)
                        {
                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = RGB(255, 255, 255);
                                text_color = RGB(0, 0, 0);
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = RGB(0, 120, 215);
                                text_color = RGB(255, 255, 255);
                            }

                            SetBkMode(draw_items_struct->hDC, OPAQUE);
                            SetBkColor(draw_items_struct->hDC, background_color);
                            SetTextColor(draw_items_struct->hDC, text_color);

                            ExtTextOutW(draw_items_struct->hDC,
                                        draw_items_struct->rcItem.left + 5, draw_items_struct->rcItem.top + 5,
                                        ETO_OPAQUE | ETO_CLIPPED, &draw_items_struct->rcItem,
                                        text_string.c_str(), static_cast<UINT>(text_length), NULL);
                        }
                        else if (draw_items_struct->itemAction & ODA_SELECT)
                        {
                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = RGB(255, 255, 255);
                                text_color = RGB(0, 0, 0);
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = RGB(0, 120, 215);
                                text_color = RGB(255, 255, 255);
                            }

                            SetBkMode(draw_items_struct->hDC, OPAQUE);
                            SetBkColor(draw_items_struct->hDC, background_color);
                            SetTextColor(draw_items_struct->hDC, text_color);

                            ExtTextOutW(draw_items_struct->hDC,
                                        draw_items_struct->rcItem.left + 5, draw_items_struct->rcItem.top + 5,
                                        ETO_OPAQUE | ETO_CLIPPED, &draw_items_struct->rcItem,
                                        text_string.c_str(), static_cast<UINT>(text_length), NULL);
                        }
                        else if (draw_items_struct->itemAction & ODA_FOCUS)
                        {
                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = RGB(255, 255, 255);
                                text_color = RGB(0, 0, 0);
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = RGB(0, 120, 215);
                                text_color = RGB(255, 255, 255);
                            }

                            SetBkMode(draw_items_struct->hDC, OPAQUE);
                            SetBkColor(draw_items_struct->hDC, background_color);
                            SetTextColor(draw_items_struct->hDC, text_color);

                            ExtTextOutW(draw_items_struct->hDC,
                                        draw_items_struct->rcItem.left + 5, draw_items_struct->rcItem.top + 5,
                                        ETO_OPAQUE | ETO_CLIPPED, &draw_items_struct->rcItem,
                                        text_string.c_str(), static_cast<UINT>(text_length), NULL);
                        }

                        message_processed_successfully = true;
                    }

                    break;
                }
                // Other types, the application doesn't have any samples yet.
                case ODT_BUTTON:
                case ODT_LISTBOX:
                case ODT_LISTVIEW:
                case ODT_STATIC:
                case ODT_TAB:
                {
                    error_message = "Unexpected control type.";
                    break;
                }
                }
                if (!message_processed_successfully)
                    break;

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                g_pApp->logger.writeLog(error_message, "[MESSAGE: 'WM_DRAWITEM' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                break;
            }

            return TRUE;
        }
        }

        break;
    }
    case WM_COMMAND:
    {
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
        {
            switch (LOWORD(wParam))
            {
            case IDC_DC_STANDARDBUTTON1:
            {
                g_pApp->logger.writeLog("Standard button 1 clicked.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_STANDARDBUTTON2:
            {
                g_pApp->logger.writeLog("Standard button 2 clicked.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_STANDARDBUTTON3:
            {
                g_pApp->logger.writeLog("Standard button 3 clicked.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_RADIOBUTTON1:
            {
                g_pApp->logger.writeLog("Radio button 1 selected.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_RADIOBUTTON2:
            {
                g_pApp->logger.writeLog("Radio button 2 selected.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_RADIOBUTTON3:
            {
                g_pApp->logger.writeLog("Radio button 3 selected.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_IMAGEBUTTON1:
            {
                g_pApp->logger.writeLog("Image button 1 clicked.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_IMAGEBUTTON2:
            {
                g_pApp->logger.writeLog("Image button 2 clicked.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_IMAGEBUTTON3:
            {
                g_pApp->logger.writeLog("Image button 3 clicked.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_IMAGEBUTTON4:
            {
                g_pApp->logger.writeLog("Image button 4 clicked.", true);
                MessageBeep(MB_OK);
                return 0;
            }
            case IDC_DC_OPENLOGFILE:
            {
                // Get current directory.
                std::wstring command_line = GetCommandLineW();
                if (command_line.size() <= 2)
                    throw std::runtime_error("Unexpected command line string.");
                command_line.erase(0, 1);
                command_line.pop_back();
                std::filesystem::path path(command_line);

                if ((ULONGLONG)ShellExecuteW(NULL, L"OPEN", (path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + L"log.log").c_str(), NULL, NULL, SW_SHOW) == ERROR_FILE_NOT_FOUND)
                {
                    // If the file doesn't exists, create one.
                    std::ofstream log_file((path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + L"log.log").c_str());
                    log_file.close();

                    // Open the file again.
                    ShellExecuteW(NULL, L"OPEN", (path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + L"log.log").c_str(), NULL, NULL, SW_SHOW);
                }

                return 0;
            }
            case IDC_DC_SELECTFILE:
            {
                std::vector<std::wstring> paths;
                if (MyUtility::OpenFileDialog(paths, false, true))
                {
                    if (!paths.empty())
                    {
                        g_pApp->logger.writeLog("Total selected items: " + std::to_string(paths.size()), true);
                        for (int i = 0; i < static_cast<int>(paths.size()); ++i)
                            g_pApp->logger.writeLog("Path " + std::to_string(i + 1) + ": " + MyUtility::ConvertWideStringToString(paths[i]), true);
                    }
                    else
                        g_pApp->logger.writeLog("No items were selected.", true);
                }

                return 0;
            }
            }

            break;
        }
        case CBN_SELCHANGE:
        {
            switch (LOWORD(wParam))
            {
            case IDC_DC_COMBOBOXNORMAL:
            {
                HWND combobox_window = reinterpret_cast<HWND>(lParam);
                INT combobox_index = static_cast<INT>(SendMessageW(combobox_window, CB_GETCURSEL, 0, 0));
                if (combobox_index == CB_ERR)
                {
                    g_pApp->logger.writeLog("No combobox item was selected.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                    break;
                }

                // Get the combobox value (in this case a string) of the current selected item.
                // INT item_text_length = static_cast<INT>(SendMessageW(combobox_window, CB_GETLBTEXTLEN, combobox_index, 0));
                // if (item_text_length == CB_ERR)
                // {
                //     error_message = "Failed to retrieve the combobox item text length.";
                //     break;
                // }
                // WCHAR *text_buffer = new WCHAR[item_text_length + 1];
                // if (SendMessageW(combobox_window, CB_GETLBTEXT, combobox_index, reinterpret_cast<LPARAM>(text_buffer)) == CB_ERR)
                // {
                //     delete[] text_buffer;
                //     error_message = "Failed to retrieve the combobox item text.";
                //     break;
                // }
                // std::wstring item_text(text_buffer);
                // delete[] text_buffer;

                switch (combobox_index)
                {
                case 0:
                {
                    g_pApp->logger.writeLog("Combobox option 1 selected.", true);
                    break;
                }
                case 1:
                {
                    g_pApp->logger.writeLog("Combobox option 2 selected.", true);
                    break;
                }
                case 2:
                {
                    g_pApp->logger.writeLog("Combobox option 3 selected.", true);
                    break;
                }
                case 3:
                {
                    g_pApp->logger.writeLog("Combobox option 4 selected.", true);
                    break;
                }
                case 4:
                {
                    g_pApp->logger.writeLog("Combobox option 5 selected.", true);
                    break;
                }
                }

                return 0;
            }
            case IDC_DC_COMBOBOXSELECTTHEME:
            {
                HWND combobox_window = reinterpret_cast<HWND>(lParam);
                INT combobox_index = static_cast<INT>(SendMessageW(combobox_window, CB_GETCURSEL, 0, 0));
                if (combobox_index == CB_ERR)
                {
                    g_pApp->logger.writeLog("No combobox item was selected.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                    break;
                }

                // Get the combobox value (in this case a string) of the current selected item.
                // INT item_text_length = static_cast<INT>(SendMessageW(combobox_window, CB_GETLBTEXTLEN, combobox_index, 0));
                // if (item_text_length == CB_ERR)
                // {
                //     error_message = "Failed to retrieve the combobox item text length.";
                //     break;
                // }
                // WCHAR *text_buffer = new WCHAR[item_text_length + 1];
                // if (SendMessageW(combobox_window, CB_GETLBTEXT, combobox_index, reinterpret_cast<LPARAM>(text_buffer)) == CB_ERR)
                // {
                //     delete[] text_buffer;
                //     error_message = "Failed to retrieve the combobox item text.";
                //     break;
                // }
                // std::wstring item_text(text_buffer);
                // delete[] text_buffer;

                bool is_switch_success = false;
                switch (combobox_index)
                {
                case 0:
                {
                    if (!g_pApp->setAppTheme(MyTheme::Light))
                        break;

                    if (!g_pApp->updateConfigurationFile("theme", "Light"))
                        break;

                    is_switch_success = true;
                    break;
                }
                case 1:
                {
                    if (!g_pApp->setAppTheme(MyTheme::Dark))
                        break;

                    if (!g_pApp->updateConfigurationFile("theme", "Dark"))
                        break;

                    is_switch_success = true;
                    break;
                }
                case 2:
                {
                    if (!g_pApp->setAppTheme(MyTheme::Monokai))
                        break;

                    if (!g_pApp->updateConfigurationFile("theme", "Monokai"))
                        break;

                    is_switch_success = true;
                    break;
                }
                }
                if (!is_switch_success)
                {
                    g_pApp->logger.writeLog("Failed to change the application theme.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                    break;
                }
                return 0;
            }
            case IDC_DC_COMBOBOXSELECTFONT:
            {
                HWND combobox_window = reinterpret_cast<HWND>(lParam);
                INT combobox_index = static_cast<INT>(SendMessageW(combobox_window, CB_GETCURSEL, 0, 0));
                if (combobox_index == CB_ERR)
                {
                    g_pApp->logger.writeLog("No combobox item was selected.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                    break;
                }

                // Get the combobox value (in this case a string) of the current selected item.
                // INT item_text_length = static_cast<INT>(SendMessageW(combobox_window, CB_GETLBTEXTLEN, combobox_index, 0));
                // if (item_text_length == CB_ERR)
                // {
                //     error_message = "Failed to retrieve the combobox item text length.";
                //     break;
                // }
                // WCHAR *text_buffer = new WCHAR[item_text_length + 1];
                // if (SendMessageW(combobox_window, CB_GETLBTEXT, combobox_index, reinterpret_cast<LPARAM>(text_buffer)) == CB_ERR)
                // {
                //     delete[] text_buffer;
                //     error_message = "Failed to retrieve the combobox item text.";
                //     break;
                // }
                // std::wstring item_text(text_buffer);
                // delete[] text_buffer;

                bool is_switch_success = false;
                switch (combobox_index)
                {
                case 0:
                {
                    if (g_pApp->pUIManager->fonts.defaultFamily == L"Bahnschrift")
                        return 0;

                    g_pApp->pUIManager->fonts.updateFonts(L"Bahnschrift");

                    // Refresh non-client windows.
                    for (auto &p_nonclient_window : g_pApp->vNonClientWindows)
                    {
                        auto p_subclass = MyStandardTextSubclass::getSubclassPointer(p_nonclient_window->hWnd());
                        if (p_subclass)
                            p_nonclient_window->refresh();
                    }

                    // Refresh the containers.
                    for (auto &[id, container] : g_pApp->mContainers)
                    {
                        container->refresh();
                    }

                    g_pApp->logger.writeLog("Application font selected:", "'Bahnschrift'", MyLogType::Info);

                    is_switch_success = true;
                    break;
                }
                case 1:
                {
                    if (g_pApp->pUIManager->fonts.defaultFamily == L"Ubuntu")
                        return 0;

                    g_pApp->pUIManager->fonts.updateFonts(L"Ubuntu");

                    // Refresh non-client windows.
                    for (auto &p_nonclient_window : g_pApp->vNonClientWindows)
                    {
                        auto p_subclass = MyStandardTextSubclass::getSubclassPointer(p_nonclient_window->hWnd());
                        if (p_subclass)
                            p_nonclient_window->refresh();
                    }

                    // Refresh the containers.
                    for (auto &[id, container] : g_pApp->mContainers)
                    {
                        container->refresh();
                    }

                    g_pApp->logger.writeLog("Application font selected:", "'Ubuntu'", MyLogType::Info);

                    is_switch_success = true;
                    break;
                }
                }
                if (!is_switch_success)
                {
                    g_pApp->logger.writeLog("Failed to change the application font.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                    break;
                }
                return 0;
            }
            case IDC_DC_COMBOBOXSELECTSCROLLMODE:
            {
                HWND combobox_window = reinterpret_cast<HWND>(lParam);
                INT combobox_index = static_cast<INT>(SendMessageW(combobox_window, CB_GETCURSEL, 0, 0));
                if (combobox_index == CB_ERR)
                {
                    g_pApp->logger.writeLog("No combobox item was selected.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                    break;
                }

                // Get the combobox value (in this case a string) of the current selected item.
                // INT item_text_length = static_cast<INT>(SendMessageW(combobox_window, CB_GETLBTEXTLEN, combobox_index, 0));
                // if (item_text_length == CB_ERR)
                // {
                //     error_message = "Failed to retrieve the combobox item text length.";
                //     break;
                // }
                // WCHAR *text_buffer = new WCHAR[item_text_length + 1];
                // if (SendMessageW(combobox_window, CB_GETLBTEXT, combobox_index, reinterpret_cast<LPARAM>(text_buffer)) == CB_ERR)
                // {
                //     delete[] text_buffer;
                //     error_message = "Failed to retrieve the combobox item text.";
                //     break;
                // }
                // std::wstring item_text(text_buffer);
                // delete[] text_buffer;

                bool is_switch_success = false;
                switch (combobox_index)
                {
                case 0:
                {
                    auto p_container = g_pApp->findContainer(IDC_DC_CONTAINER);
                    if (p_container)
                    {
                        p_container->enableSmoothScroll = false;
                        g_pApp->logger.writeLog("Scroll mode selected:", "'Instant'", MyLogType::Info);
                    }
                    else
                        g_pApp->logger.writeLog("Failed to change the container scroll mode.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);

                    is_switch_success = true;
                    break;
                }
                case 1:
                {
                    auto p_container = g_pApp->findContainer(IDC_DC_CONTAINER);
                    if (p_container)
                    {
                        p_container->enableSmoothScroll = true;
                        g_pApp->logger.writeLog("Scroll mode selected:", "'Smooth'", MyLogType::Info);
                    }
                    else
                        g_pApp->logger.writeLog("Failed to change the container scroll mode.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);

                    is_switch_success = true;
                    break;
                }
                }
                if (!is_switch_success)
                {
                    g_pApp->logger.writeLog("Failed to change the application font.", "[MESSAGE: 'WM_COMMAND/CBN_SELCHANGE' | CALLBACK: 'DefaultContainerProcedure()']", MyLogType::Error);
                    break;
                }
                return 0;
            }
            default:
                break;
            }
        }
        }
        break;
    }
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &DefaultContainerProcedure, idSubclass))
            g_pApp->logger.writeLog("Failed to remove the window subclass callback.", "[MESSAGE: 'WM_DESTROY' | FUNC: 'DefaultContainerProcedure()']", MyLogType::Error);
        return 0;
    }
    }

    return DefSubclassProc(hWnd, message, wParam, lParam);
}