/**
 * @file main.cpp
 * @brief Define application entry and window procedure callback functions.
 */

#include "../Main/func.h" // Include libraries and define application functions.

/**
 * @brief Application entry point.
 * @brief READMORE: https://learn.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
 *
 * @param hInstance     Handle to an instance or handle to a module.
 * @param hPrevInstance It was used in 16-bit Windows, but is now always zero.
 * @param lpCmdLine     Contains the command-line arguments as a Unicode string.
 * @param nShowCmd      Flag that indicates whether the main application window is minimized, maximized, or shown normally.
 *
 * @return The function returns an int value. The operating system doesn't use the return value, but we can use the value to pass a status code to another program.
 */
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd)
{
    // Get the application directory path for log-writing operations.
    {
        WCHAR text_buffer[MAX_PATH];
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get the directory path to the text buffer.
            if (!_wgetcwd(text_buffer, MAX_PATH))
            {
                error_message = L"_wgetcwd() function failed.";
                break;
            }

            // Create the string from the text buffer.
            std::wstring directory_path(text_buffer);

            // Check if the string is empty.
            if (directory_path.empty())
            {
                error_message = L"The directory path string is empty.";
                break;
            }

            // Create the path from the string.
            g_AppDirectoryPath = directory_path;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            std::wstring message = L"";
            message.append(L"Error occurred!\n");
            message.append(L"Failed to get the application directory path.\n");
            message.append(L"Error Message: " + error_message);
            MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

            return -1;
        }
    }

    // Terminate if the application is already running.
    if (FindWindowW(g_WindowClassName, NULL) != NULL)
    {
        WriteLogEx(L"The application is already running.", L"", MyLogType::Debug, 1);
        return 0;
    }

    // Create my custom window class.
    WNDCLASSW MyClass = {0};
    MyClass.hbrBackground = NULL;
    MyClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    MyClass.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON7));
    MyClass.hInstance = hInstance;
    MyClass.lpszClassName = g_WindowClassName;
    MyClass.lpfnWndProc = WindowProcedure;
    g_hInstance = hInstance;

    // Register my window class.
    if (!RegisterClassW(&MyClass))
    {
        WriteLog(L"Failed to register the window class.", L"", MyLogType::Error);
        return -1;
    }

    // Load the application settings from file.
    if (!nApp::File::LoadSettingsFile(g_AppDirectoryPath, g_AppSettingsFileName))
    {
        WriteLog(L"Failed to load the application settings from file.", L"", MyLogType::Error);
        return -1;
    }
    WriteLog(L"Loaded the application directory path: ", (L"\"" + g_AppDirectoryPath.wstring() + L"\""), MyLogType::Debug);
    WriteLog(L"Loaded the application settings from file.", L"", MyLogType::Debug);

    // Create the application window using my registered window class.
    {
        // Get user desktop resolution.
        INT desktop_width = 0, desktop_height = 0;
        nSol::GetDesktopResolution(desktop_width, desktop_height);
        if (!desktop_width || !desktop_height)
        {
            WriteLog(L"Failed to retrieve the desktop resolution.", L"", MyLogType::Error);
            return -1;
        }
        WriteLog(L"Current desktop resolution: ", (L"\"" + std::to_wstring(desktop_width) + L"x" + std::to_wstring(desktop_height) + L"\"").c_str(), MyLogType::Debug);

        // Create the window.
        g_hWnd = CreateWindowExW(WS_EX_LAYERED,       // Layered window, allows window to be transparent, semi-transparent or have a custom opacity.
                                 g_WindowClassName,   // Window class name.
                                 L"Win32 GUI Sample", // Window title.
                                 WS_MYSTYLE,          // My window style.
                                 static_cast<INT>((static_cast<DOUBLE>(desktop_width) / 2) - (static_cast<DOUBLE>(g_CurrentWindowWidth) / 1.4)),
                                 static_cast<INT>((static_cast<DOUBLE>(desktop_height) / 2) - (static_cast<DOUBLE>(g_CurrentWindowHeight) / 1.4)), // Semi-center the application window on start.
                                 g_CurrentWindowWidth, g_CurrentWindowHeight,                                                                      // Initial application window dimensions.
                                 NULL, NULL, hInstance, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(g_hWnd))
        {
            WriteLog(L"Failed to create the application window.", L"", MyLogType::Error);
            return -1;
        }

        // Set window opacity and transparency color.
        // Remark: Set transparency color so the window manager will not render any pixels that match the transparency color.
        // This means that the standard window frames will also be rendered transparent.
        if (!SetLayeredWindowAttributes(g_hWnd, RGB(141, 172, 160), 255, LWA_ALPHA | LWA_COLORKEY))
        {
            WriteLog(L"Failed to set the application window attributes.", L"", MyLogType::Error);
            return -1;
        }
    }

    // Sets the application window and essential controls to be visible, indicating that the application is ready.
    if (!nApp::Window::Initialization::OnReady())
    {
        WriteLog(L"Failed to execute \"nApp::Window::Initialization::OnReady()\" function.", L"", MyLogType::Error);
        return -1;
    }

    // Enter the message loop.
    MSG msg = {0};
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        if (IsDialogMessageW(g_hWnd, &msg) == 0)
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    // Write a log record indicating that the application has exited.
    WriteLogEx(L"Finished exiting.", L"", MyLogType::Debug, 1, true);

    return 0;
}

/**
 * @brief Application main window procedure.
 *
 * @param hWnd   The handle to the window.
 * @param uMsg   The message identifier.
 * @param wParam The first message parameter.
 * @param lParam The second message parameter.
 *
 * @return The result of the message processing.
 */
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    // Initialize the application window.
    case WM_CREATE:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Initialize the application global objects, APIs, and set up basic WinAPI settings.
            if (!nApp::Window::Initialization::InitBegin(hWnd, g_hInstance))
            {
                error_message = L"Failed to execute function \"InitBegin()\" function.";
                break;
            }

            // Initialize the application theme and perform related operations.
            if (!nApp::Window::Initialization::InitTheme(hWnd))
            {
                error_message = L"Failed to execute function \"InitTheme()\" function.";
                break;
            }

            // Create the application controls.
            if (!nApp::Window::Initialization::InitControl(hWnd, g_hInstance))
            {
                error_message = L"Failed to execute function \"InitControl()\" function.";
                break;
            }

            // Perform additional operations on the application window objects.
            if (!nApp::Window::Initialization::InitEnd(hWnd))
            {
                error_message = L"Failed to execute function \"InitEnd()\" function.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [MESSAGE: \"WM_CREATE\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            return -1;
        }

        return 0;
    }

    // Process WM_COMMAND messages.
    case WM_COMMAND:
    {
        // Use of the wParam and lParam parameters are summarized here.
        // +----------------+-----------------------------------+------------------------+-------------------------------------+
        // | Message Source | wParam (high word)                | wParam (low word)      | lParam                              |
        // +----------------+-----------------------------------+------------------------+-------------------------------------+
        // | Menu           | 0                                 | Menu identifier        | 0                                   |
        // | Accelerator    | 1                                 | Accelerator identifier | 0                                   |
        // | Control        | Control-defined notification code | Control identifier     | Handle to the control window        |
        // +----------------+-----------------------------------+------------------------+-------------------------------------+
        // Return: 0 if the application processed the message.

        // Process WM_COMMAND messages sent by controls.
        switch (LOWORD(wParam))
        {
        case IDC_NONCLIENT_CLOSE_BUTTON:
            nApp::Window::Deinitialization::OnExit();
            DestroyWindow(hWnd);
            return 0;

        case IDC_NONCLIENT_MINIMIZE_BUTTON:
            g_IsWindowMinimized = true;
            ShowWindow(hWnd, SW_SHOWMINIMIZED);
            return 0;
        }

        break;
    }

    // Paint the main window.
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = NULL;
        HDC mem_hdc = NULL;
        HBITMAP bitmap = NULL;
        HBITMAP bitmap_old = NULL;
        bool is_paint_begined = false;
        bool is_bitmap_created = false;
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get the application client rect.
            RECT rect_application;
            if (!GetClientRect(hWnd, &rect_application))
            {
                error_message = L"Failed to retrieve the application window's client rect.";
                break;
            }

            // Begin the paintings.
            hdc = BeginPaint(hWnd, &ps);
            if (!hdc)
            {
                error_message = L"Failed to execute \"BeginPaint()\".";
                break;
            }
            is_paint_begined = true;

            // Create memory device context and bitmap object for double buffering.
            mem_hdc = CreateCompatibleDC(hdc);
            if (!mem_hdc)
            {
                error_message = L"Failed to create the compatible memory device context.";
                break;
            }
            bitmap = CreateCompatibleBitmap(hdc, rect_application.right - rect_application.left, rect_application.bottom - rect_application.top);
            if (!bitmap)
            {
                error_message = L"Failed to create the compatible bitmap object.";
                break;
            }
            is_bitmap_created = true;
            bitmap_old = reinterpret_cast<HBITMAP>(SelectObject(mem_hdc, bitmap));
            if (!bitmap_old)
            {
                error_message = L"Failed to select the bitmap object.";
                break;
            }

            // Begin painting to the memory device context.
            {
                // Draw the window background.
                FillRect(mem_hdc, &rect_application, g_pUIElements->colors.background.getHBRUSH());

                // Draw the window caption background.
                FillRect(mem_hdc, &g_pUIElements->rectangles.rectCaption, g_pUIElements->colors.captionBackground.getHBRUSH());

                // Draw the window borders.
                FrameRect(mem_hdc, &rect_application, *g_pUIElements->pointers.pCurrentBorderBrush);
            }

            // Draw contents from memory device context to target device context.
            if (!BitBlt(hdc, 0, 0, rect_application.right - rect_application.left, rect_application.bottom - rect_application.top, mem_hdc, 0, 0, SRCCOPY))
            {
                error_message = L"Failed to draw contents from memory device context to target device context.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [MESSAGE: \"WM_PAINT\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (is_paint_begined)
        {
            // Delete the bitmap object.
            if (is_bitmap_created)
            {
                if (!SelectObject(mem_hdc, bitmap_old))
                {
                    WriteLog(L"Failed to select the bitmap object.", L" [MESSAGE: \"WM_PAINT\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
                }
                if (!DeleteObject(bitmap))
                {
                    WriteLog(L"Failed to delete the bitmap object.", L" [MESSAGE: \"WM_PAINT\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
                }
                bitmap = NULL;
                bitmap_old = NULL;
            }

            // Delete the memory device context.
            if (mem_hdc)
            {
                if (!DeleteDC(mem_hdc))
                {
                    WriteLog(L"Failed to delete the memory device context.", L" [MESSAGE: \"WM_PAINT\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
                }
                mem_hdc = NULL;
            }

            // End the paintings.
            EndPaint(hWnd, &ps);
            return 0;
        }

        break;
    }

    // Process background erase requests.
    case WM_ERASEBKGND:
    {
        // If the application is not ready yet or is resizing, erase the background manually.
        // This effectively reduces unwanted visual artifacts during resizing and makes the application window appear more smoothly when being opened.
        if (!g_IsWindowReady || g_IsAppResizing)
        {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            HDC mem_hdc = NULL;
            HBITMAP bitmap = NULL;
            HBITMAP bitmap_old = NULL;
            bool is_bitmap_created = false;
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the application client rect.
                RECT rect_application;
                if (!GetClientRect(hWnd, &rect_application))
                {
                    error_message = L"Failed to retrieve the application window's client rect.";
                    break;
                }

                // Create memory device context and bitmap object for double buffering.
                mem_hdc = CreateCompatibleDC(hdc);
                if (!mem_hdc)
                {
                    error_message = L"Failed to create the compatible memory device context.";
                    break;
                }
                bitmap = CreateCompatibleBitmap(hdc, rect_application.right - rect_application.left, rect_application.bottom - rect_application.top);
                if (!bitmap)
                {
                    error_message = L"Failed to create the compatible bitmap object.";
                    break;
                }
                is_bitmap_created = true;
                bitmap_old = reinterpret_cast<HBITMAP>(SelectObject(mem_hdc, bitmap));
                if (!bitmap_old)
                {
                    error_message = L"Failed to select the bitmap object.";
                    break;
                }

                // Begin painting to the memory device context.
                {
                    // Draw the window background.
                    FillRect(mem_hdc, &rect_application, g_pUIElements->colors.background.getHBRUSH());
                    // Draw the window caption background.
                    RECT rect_caption = rect_application;
                    rect_caption.bottom = WINDOW_CAPTIONBAR_DEFAULTHEIGHT;
                    FillRect(mem_hdc, &rect_caption, g_pUIElements->colors.captionBackground.getHBRUSH());
                }

                // Draw contents from memory device context to target device context.
                if (!BitBlt(hdc, 0, 0, rect_application.right - rect_application.left, rect_application.bottom - rect_application.top, mem_hdc, 0, 0, SRCCOPY))
                {
                    error_message = L"Failed to draw contents from memory device context to target device context.";
                    break;
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_ERASEBKGND\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            // Perform necesscary clean up operations.
            {
                // Delete the bitmap object.
                if (is_bitmap_created)
                {
                    if (!SelectObject(mem_hdc, bitmap_old))
                    {
                        WriteLog(L"Failed to select the bitmap object.", L" [MESSAGE: \"WM_ERASEBKGND\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
                    }
                    if (!DeleteObject(bitmap))
                    {
                        WriteLog(L"Failed to delete the bitmap object.", L" [MESSAGE: \"WM_ERASEBKGND\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
                    }
                    bitmap = NULL;
                    bitmap_old = NULL;
                }

                // Delete the memory device context.
                if (mem_hdc)
                {
                    if (!DeleteDC(mem_hdc))
                    {
                        WriteLog(L"Failed to delete the memory device context.", L" [MESSAGE: \"WM_ERASEBKGND\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
                    }
                    mem_hdc = NULL;
                }
            }
        }

        // Always return non-zero to indicate that no further erasing is required, avoid flickering.
        return 1;
    }

    // Process WM_CTLCOLORSTATIC messages to set the text foreground and background colors of the static control, read-only/disabled edit control.
    case WM_CTLCOLORSTATIC:
    {
        HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        INT window_id = GetDlgCtrlID(window);

        switch (window_id)
        {
        // Priority processings.

        // Container: MainContent
        case IDC_MAINCONTENT_CONTAINER:
        {
            SetBkColor(hdc, g_pUIElements->colors.background.getCOLORREF());
            SetTextColor(hdc, g_pUIElements->colors.textActive.getCOLORREF());
            p_background_brush = &g_pUIElements->colors.background.getHBRUSH();
            break;
        }

        // Caption title.
        case IDC_NONCLIENT_CAPTIONTITLE_STATIC:
        {
            SetBkColor(hdc, g_pUIElements->colors.captionBackground.getCOLORREF());
            SetTextColor(hdc, (GetActiveWindow() == hWnd ? g_pUIElements->colors.textActive.getCOLORREF() : g_pUIElements->colors.textInactive.getCOLORREF()));
            p_background_brush = &g_pUIElements->colors.captionBackground.getHBRUSH();
            break;
        }

        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the window class name.
                std::wstring class_name = nSol::GetWindowClassName(window);
                if (class_name.empty())
                {
                    error_message = L"Failed to retrieve the window class name.";
                    break;
                }

                SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                p_background_brush = &default_background_brush;
                WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                         L" [MESSAGE: \"WM_CTLCOLORSTATIC\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Debug);

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                p_background_brush = &default_background_brush;
                WriteLog(error_message, L" [MESSAGE: \"WM_CTLCOLORSTATIC\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            break;
        }
        }

        return reinterpret_cast<LRESULT>(*p_background_brush);
    }

    // Process WM_CTLCOLORBTN messages to set the text foreground and background colors of the button control.
    case WM_CTLCOLORBTN:
    {
        HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        INT window_id = GetDlgCtrlID(window);

        switch (window_id)
        {
        // Priority processings.
        // Expect no priority messages now, use 6969 as a placeholder to suppress IDE warnings.
        case 6969:
            break;
        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the window class name.
                std::wstring class_name = nSol::GetWindowClassName(window);
                if (class_name.empty())
                {
                    error_message = L"Failed to retrieve the window class name.";
                    break;
                }

                // Default processing for button controls.
                if (class_name == WC_BUTTON)
                {
                    // Subclassed controls.
                    void *p_subclass = nullptr;
                    if (MyStandardButton::isSubclassed(window, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    else if (MyImageButton::isSubclassed(window, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    else if (MyRadioButton::isSubclassed(window, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    // Standard controls.
                    else
                    {
                        SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                        auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                        p_background_brush = &default_background_brush;
                        WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                                 L" [MESSAGE: \"WM_CTLCOLORBTN\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Debug);
                    }
                }
                // Default processing for unexpected class name.
                else
                {
                    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                    auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                    p_background_brush = &default_background_brush;
                    WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                             L" [MESSAGE: \"WM_CTLCOLORBTN\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Debug);
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                p_background_brush = &default_background_brush;
                WriteLog(error_message, L" [MESSAGE: \"WM_CTLCOLORBTN\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            break;
        }
        }

        return reinterpret_cast<LRESULT>(*p_background_brush);
    }

    // Updates the application window border colors when the window is activated or deactivated.
    case WM_ACTIVATE:
    {
        // Update window minimized state.
        g_IsWindowMinimized = (HIWORD(wParam) ? true : false);

        switch (LOWORD(wParam))
        {
        case WA_CLICKACTIVE:
        case WA_ACTIVE:
        {
            // Update the appropriate border color.
            g_pUIElements->pointers.pCurrentBorderBrush = &g_pUIElements->colors.borderActive.getHBRUSH();

            // If supported, set the border color attribute.
            if (g_IsWindows11BorderAttributeSupported)
            {
                COLORREF border_color = g_pUIElements->colors.borderActive.getCOLORREF();
                HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
                if (FAILED(hr))
                    WriteLog(L"Failed to set the window border attribute.", L" [MESSAGE: \"WM_ACTIVATE/WA_CLICKACTIVE||WA_ACTIVE\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            break;
        }

        case WA_INACTIVE:
        {
            // Update the appropriate border color.
            g_pUIElements->pointers.pCurrentBorderBrush = &g_pUIElements->colors.borderInactive.getHBRUSH();

            // If supported, set the border color attribute.
            if (g_IsWindows11BorderAttributeSupported)
            {
                COLORREF border_color = g_pUIElements->colors.borderInactive.getCOLORREF();
                HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
                if (FAILED(hr))
                    WriteLog(L"Failed to set the window border attribute.", L" [MESSAGE: \"WM_ACTIVATE/WA_INACTIVE\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            break;
        }
        }
        
        // Invalidate the borders.
        InvalidateRect(hWnd, &g_pUIElements->rectangles.rectSizeBorderLeft, FALSE);
        InvalidateRect(hWnd, &g_pUIElements->rectangles.rectSizeBorderTop, FALSE);
        InvalidateRect(hWnd, &g_pUIElements->rectangles.rectSizeBorderRight, FALSE);
        InvalidateRect(hWnd, &g_pUIElements->rectangles.rectSizeBorderBottom, FALSE);

        // Invalidate non-client window controls.
        for (const auto &window : g_VectorMyWindow_NonClient)
        {
            InvalidateRect(window->hWnd, NULL, FALSE);
        }

        return 0;
    }

    // Suppress WM_NCACTIVATE messages to prevent unnecessary non-client paints.
    case WM_NCACTIVATE:
    {
        if (!g_IsWindowReady)
            return 0;

        break;
    }

    // Process WM_NCCALCSIZE messages to perform additional operations to adjust the maximize rect.
    // Prevent the application window from spilling over monitor edges when maximized.
    // Source: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
    case WM_NCCALCSIZE:
    {
        if (wParam == TRUE)
        {
            auto &parameters = *reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
            nApp::Window::Utility::AdjustMaximizedClientRect(hWnd, parameters.rgrc[0]);
            return 0;
        }

        // Always return zero to remove the standard window frames.
        // Remark: The application uses custom-drawn caption bars and borders. Everything is drawn in the client area.
        return 0;
    }

    // Process WM_NCHITTEST messages to handle appropriated events for custom-drawn caption bars and borders.
    case WM_NCHITTEST:
    {
        // Get the default WM_NCHITTEST response.
        const LRESULT default_result = ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
        const LONG extra_sizeborder_thickness = 10;

        // Get caption bar and size border rects.
        RECT rect_caption = g_pUIElements->rectangles.rectCaption,
             rect_border_left = g_pUIElements->rectangles.rectSizeBorderLeft,
             rect_border_top = g_pUIElements->rectangles.rectSizeBorderTop,
             rect_border_right = g_pUIElements->rectangles.rectSizeBorderRight,
             rect_border_bottom = g_pUIElements->rectangles.rectSizeBorderBottom,
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
        if (!g_IsWindowMaximized)
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

    // Process WM_SIZE messages to update global dimension variables and rectangles.
    case WM_SIZE:
    {
        // Update the global dimension variables and rectangles.
        {
            // Global dimension variables.
            g_CurrentWindowWidth = static_cast<INT>(LOWORD(lParam));
            g_CurrentWindowHeight = static_cast<INT>(HIWORD(lParam));

            // Global dimension rectangles.
            g_pUIElements->rectangles.rectCaption.right = g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH;
            g_pUIElements->rectangles.rectSizeBorderLeft.bottom = g_CurrentWindowHeight - WINDOW_BORDER_DEFAULTWIDTH;
            g_pUIElements->rectangles.rectSizeBorderTop.right = g_CurrentWindowWidth;
            g_pUIElements->rectangles.rectSizeBorderRight.left = g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH;
            g_pUIElements->rectangles.rectSizeBorderRight.right = g_CurrentWindowWidth;
            g_pUIElements->rectangles.rectSizeBorderRight.bottom = g_CurrentWindowHeight - WINDOW_BORDER_DEFAULTWIDTH;
            g_pUIElements->rectangles.rectSizeBorderBottom.right = g_CurrentWindowWidth;
            g_pUIElements->rectangles.rectSizeBorderBottom.top = g_CurrentWindowHeight - WINDOW_BORDER_DEFAULTWIDTH;
            g_pUIElements->rectangles.rectSizeBorderBottom.bottom = g_CurrentWindowHeight;
        }

        // Stop all animations during resize event.
        {
            HRESULT hr;

            // GDI Animations.
            hr = BufferedPaintStopAllAnimations(hWnd);
            if (FAILED(hr))
                WriteLog(L"Failed to execute \"BufferedPaintStopAllAnimations()\" function.", L" [MESSAGE: \"WM_SIZE\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);

            // Windows Animation Manager animations.
            if (g_pAnimationManager)
            {
                hr = g_pAnimationManager->FinishAllStoryboards(0);
                if (FAILED(hr))
                    WriteLog(L"Failed to finish all the active storyboards.", L" [MESSAGE: \"WM_SIZE\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }
        }

        // Update window states.
        g_IsWindowMaximized = (wParam == SIZE_MAXIMIZED ? true : false);
        g_IsWindowMinimized = (wParam == SIZE_MINIMIZED ? true : false);

        // If the application window is maximized, perform an additional redraw on the containers.
        if (wParam == SIZE_MAXIMIZED && IsWindow(g_Container_MainContent->pContainerWindow->hWnd))
            RedrawWindow(g_Container_MainContent->pContainerWindow->hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

        // Redraw the application window and trigger WM_WINDOWPOSCHANGING message to update application controls dimensions.
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

        return 0;
    }

    // Process WM_WINDOWPOSCHANGING messages to update application controls dimensions.
    case WM_WINDOWPOSCHANGING:
    {
        // Get the pointer to the WINDOWPOS structure and check if the application window size is changed.
        WINDOWPOS *p_window_pos = reinterpret_cast<WINDOWPOS *>(lParam);
        bool is_window_size_changed = ((p_window_pos->cx != g_CurrentWindowWidth) && p_window_pos->cx || (p_window_pos->cy != g_CurrentWindowHeight) && p_window_pos->cy ? true : false);
        bool reset_containers_scroll_position = is_window_size_changed && !IsIconic(hWnd) && !g_IsWindowMinimized;

        // Update the dimensions of the containers.
        // Container: MainContent
        if (IsWindow(g_Container_MainContent->pContainerWindow->hWnd))
        {
            if (!g_Container_MainContent->updateContainerDimensions(WINDOW_BORDER_DEFAULTWIDTH,
                                                                    g_pUIElements->rectangles.rectCaption.bottom,
                                                                    g_CurrentWindowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2),
                                                                    g_CurrentWindowHeight - (WINDOW_BORDER_DEFAULTWIDTH * 2) - (g_pUIElements->rectangles.rectCaption.bottom - g_pUIElements->rectangles.rectCaption.top), g_IsCurrentThemeWantScrollbarsVisible, reset_containers_scroll_position))
            {
                WriteLog(L"Failed to update the MainContent container's dimensions.", L" [MESSAGE: \"WM_SIZE\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }
        }

        // Update the non-client controls dimensions all at once.
        HDWP hdwp_nonclient_controls = BeginDeferWindowPos(2);
        DeferWindowPos(hdwp_nonclient_controls, g_pUIElements->miscs.hWndNonClientCloseButton, NULL, g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH - 58, WINDOW_BORDER_DEFAULTWIDTH, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
        DeferWindowPos(hdwp_nonclient_controls, g_pUIElements->miscs.hWndNonClientMinimizeButton, NULL, g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH - 58 - 58, WINDOW_BORDER_DEFAULTWIDTH, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
        if (!EndDeferWindowPos(hdwp_nonclient_controls))
            WriteLog(L"Failed to update the non-client controls' dimensions", L" [MESSAGE: \"WM_SIZE\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);

        break;
    }

    // Process WM_ENTERSIZEMOVE messages to keep track of whether the application is entering a resizing event.
    case WM_ENTERSIZEMOVE:
    {
        if (!g_IsAppResizing)
            g_IsAppResizing = true;

        break;
    }

    // Process WM_EXITSIZEMOVE messages to keep track of whether the application is exiting a resizing event.
    case WM_EXITSIZEMOVE:
    {
        if (g_IsAppResizing)
            g_IsAppResizing = false;

        RedrawWindow(g_pUIElements->miscs.hWndNonClientCloseButton, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(g_pUIElements->miscs.hWndNonClientMinimizeButton, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        break;
    }

    // Process WM_GETMINMAXINFO messages to set the minimum dimensions that the application window can be resized to.
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO p_minmax_info = reinterpret_cast<LPMINMAXINFO>(lParam);

        p_minmax_info->ptMinTrackSize.x = 519;
        p_minmax_info->ptMinTrackSize.y = 200;

        return 0;
    }

    // Process WM_VSCROLL messages to handle scrolling events.
    case WM_VSCROLL:
    {
        HWND hwnd_scrollbar = reinterpret_cast<HWND>(lParam);
        INT scrollbar_id = GetDlgCtrlID(hwnd_scrollbar);

        switch (scrollbar_id)
        {
        // MainContent container scrollbar.
        case IDC_MAINCONTENT_CONTAINER_SCROLLBAR:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                bool is_scrolling_downward = false, // Indicate whether scrolling is occurring in a downward direction.
                    is_thumb_dragging = false;      // Indicates if the user is dragging the scroll bar thumb.
                INT scroll_pixel = 10,              // Number of pixels to scroll per scroll.
                    previous_pos = 0;               // Previous message's scroll position.
                HWND scroll_container = g_Container_MainContent->pContainerWindow->hWnd;

                // Check if the scroll container window is valid.
                if (!IsWindow(scroll_container))
                {
                    error_message = L"The scroll container window is invalid.";
                    break;
                }

                // Get the container scroll information struct.
                SCROLLINFO scroll_info;
                scroll_info.cbSize = sizeof(SCROLLINFO);
                scroll_info.fMask = SIF_ALL;
                if (!SendMessageW(hwnd_scrollbar, SBM_GETSCROLLINFO, NULL, reinterpret_cast<LPARAM>(&scroll_info)))
                {
                    error_message = L"Failed to retrieve the container scroll information struct.";
                    break;
                }

                // Stores previous position.
                previous_pos = scroll_info.nPos;

                // Get the scroll parameters.
                switch (LOWORD(wParam))
                {
                case SB_LINEUP:
                    is_thumb_dragging = false;
                    is_scrolling_downward = false;
                    scroll_info.nPos -= scroll_pixel;
                    break;

                case SB_LINEDOWN:
                    is_thumb_dragging = false;
                    is_scrolling_downward = true;
                    scroll_info.nPos += scroll_pixel;
                    break;

                case SB_THUMBTRACK:
                    is_thumb_dragging = true;
                    scroll_info.nPos = HIWORD(wParam);
                    if (scroll_info.nPos < previous_pos)
                        is_scrolling_downward = false;
                    else
                        is_scrolling_downward = true;
                    break;

                case SB_PAGEUP:
                    is_thumb_dragging = false;
                    is_scrolling_downward = false;
                    scroll_pixel *= 4;
                    scroll_info.nPos -= scroll_pixel;
                    break;

                case SB_PAGEDOWN:
                    is_thumb_dragging = false;
                    is_scrolling_downward = true;
                    scroll_pixel *= 4;
                    scroll_info.nPos += scroll_pixel;
                    break;
                }

                // If the scroll position has not changed, no scrolling is needed.
                if (previous_pos == scroll_info.nPos)
                    return 0;

                // If the thumb is not being dragged, check if the scroll position exceeds the minimum or maximum values and adjust it if necessary.
                if (!is_thumb_dragging)
                {
                    if (scroll_info.nPos < scroll_info.nMin)
                    {
                        ScrollWindowEx(scroll_container, NULL, scroll_pixel - (-(scroll_info.nPos)), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                        scroll_info.nPos = scroll_info.nMin;
                    }
                    else if (scroll_info.nPos > scroll_info.nMax - static_cast<int>(scroll_info.nPage))
                    {
                        ScrollWindowEx(scroll_container, NULL, -scroll_pixel + -((scroll_info.nMax - (int)scroll_info.nPage) - scroll_info.nPos), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                        scroll_info.nPos = scroll_info.nMax - static_cast<int>(scroll_info.nPage);
                    }
                    else
                    {
                        ScrollWindowEx(scroll_container, NULL, (is_scrolling_downward ? -scroll_pixel : scroll_pixel), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                    }
                }
                // Otherwise, if the thumb is being dragged, keep the scroll position within
                // the allowed range and scroll the window by the difference between the previous and current position.
                else
                {
                    if (scroll_info.nPos < scroll_info.nMin)
                        scroll_info.nPos = scroll_info.nMin;
                    else if (scroll_info.nPos > scroll_info.nMax - static_cast<int>(scroll_info.nPage))
                        scroll_info.nPos = scroll_info.nMax - static_cast<int>(scroll_info.nPage);
                    ScrollWindowEx(scroll_container, NULL, (previous_pos - scroll_info.nPos), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                }

                // Get the container dimensions.
                RECT rect_container;
                if (!GetClientRect(scroll_container, &rect_container))
                {
                    error_message = L"Failed to retrieve the container's client rect.";
                    break;
                }

                // New container height dimension equivalent to new page size.
                scroll_info.nPage = rect_container.bottom - rect_container.top;

                // Set scroll info.
                SendMessageW(hwnd_scrollbar, SBM_SETSCROLLINFO, TRUE, reinterpret_cast<LPARAM>(&scroll_info));

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_VSCROLL\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            WriteLog(L"Unexpected message.", L" [MESSAGE: \"WM_VSCROLL\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Warn);
            break;
        }
        }

        break;
    }

    // Process WM_MOUSEWHEEL messages to handle mouse scrolling events.
    case WM_MOUSEWHEEL:
    {
        // Remark: For now, the application only have 1 scrollable container: MainContent.
        if (g_Container_MainContent && IsWindow(g_Container_MainContent->pContainerWindow->hWnd) && IsWindowVisible(g_Container_MainContent->pContainerWindow->hWnd) && IsWindow(g_Container_MainContent->pVerticalScrollbarWindow->hWnd))
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                bool is_scrolling_downward = false; // Indicate whether scrolling is occurring in a downward direction.
                INT scroll_pixel = 10,              // Number of pixels to scroll per scroll.
                    scroll_delta = 0,               // The actual number of pixels that will be scrolled per scroll event.
                    wheel_delta = 0;                // Wheel Delta.
                HWND scroll_container = g_Container_MainContent->pContainerWindow->hWnd,
                     hwnd_scrollbar = g_Container_MainContent->pVerticalScrollbarWindow->hWnd;

                // Get the container scroll information struct.
                SCROLLINFO scroll_info;
                scroll_info.cbSize = sizeof(SCROLLINFO);
                scroll_info.fMask = SIF_ALL;
                if (!SendMessageW(hwnd_scrollbar, SBM_GETSCROLLINFO, NULL, reinterpret_cast<LPARAM>(&scroll_info)))
                {
                    error_message = L"Failed to retrieve the container scroll information struct.";
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
                    scroll_info.nPos += scroll_delta;
                }
                else
                {
                    // The scrolling is occurring in an upward direction.
                    is_scrolling_downward = false;
                    scroll_delta = scroll_pixel * (wheel_delta / WHEEL_DELTA);
                    scroll_info.nPos -= scroll_delta;
                }

                // Check for boundary conditions and update the position.
                if (scroll_info.nPos < scroll_info.nMin)
                {
                    // Update scroll position and the window with a scroll of size specified by the ScrollDelta.
                    ScrollWindowEx(scroll_container, NULL, (scroll_delta) - (-(scroll_info.nPos)), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                    scroll_info.nPos = scroll_info.nMin;
                }
                else if (scroll_info.nPos > scroll_info.nMax - (int)scroll_info.nPage)
                {
                    // Update scroll position and the window with a scroll of size specified by the ScrollDelta.
                    ScrollWindowEx(scroll_container, NULL, -(scroll_delta) + -((scroll_info.nMax - (int)scroll_info.nPage) - scroll_info.nPos), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                    scroll_info.nPos = scroll_info.nMax - (int)scroll_info.nPage;
                }
                else
                {
                    // Update scroll position and the window with a scroll of size specified by the ScrollDelta.
                    ScrollWindowEx(scroll_container, NULL, (is_scrolling_downward ? -(scroll_delta) : scroll_delta), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                }

                // Get the container dimensions.
                RECT rect_container;
                if (!GetClientRect(scroll_container, &rect_container))
                {
                    error_message = L"Failed to retrieve the container's client rect.";
                    break;
                }

                // New container height dimension equivalent to new page size.
                scroll_info.nPage = rect_container.bottom - rect_container.top;

                // Set scroll info.
                SendMessageW(hwnd_scrollbar, SBM_SETSCROLLINFO, TRUE, reinterpret_cast<LPARAM>(&scroll_info));

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_MOUSEWHEEL\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            return 0;
        }

        break;
    }

    // Set focus to the main window on background click.
    case WM_LBUTTONDOWN:
    {
        SetFocus(hWnd);
        return 0;
    }

    // Process WM_KEYUP messages for key press events such as change theme, show debug message box..
    case WM_KEYUP:
    {
        switch (wParam)
        {
        // Switch application theme.
        case VK_F5:
        {
            // Change to the next theme.
            bool is_theme_changed = false;
            std::wstring error_message = L"";
            INT change_theme_combobox_index = 0;
            switch (g_CurrentAppTheme)
            {
            case MyTheme::Light:
            {
                // Update application theme.
                if (!nApp::Theme::SetAppTheme(MyTheme::Dark))
                {
                    error_message = L"Failed to update application theme.";
                    break;
                }

                // Update the settings file.
                if (!nApp::File::UpdateSettingsFile(g_AppDirectoryPath, g_AppSettingsFileName, L"theme", L"Dark"))
                {
                    error_message = L"Failed to update the settings file.";
                    break;
                }

                change_theme_combobox_index = 1;
                is_theme_changed = true;
                break;
            }

            case MyTheme::Dark:
            {
                // Update application theme.
                if (!nApp::Theme::SetAppTheme(MyTheme::Monokai))
                {
                    error_message = L"Failed to update application theme.";
                    break;
                }

                // Update the settings file.
                if (!nApp::File::UpdateSettingsFile(g_AppDirectoryPath, g_AppSettingsFileName, L"theme", L"Monokai"))
                {
                    error_message = L"Failed to update the settings file.";
                    break;
                }

                change_theme_combobox_index = 2;
                is_theme_changed = true;
                break;
            }

            case MyTheme::Monokai:
            {
                // Update application theme.
                if (!nApp::Theme::SetAppTheme(MyTheme::Light))
                {
                    error_message = L"Failed to update application theme.";
                    break;
                }

                // Update the settings file.
                if (!nApp::File::UpdateSettingsFile(g_AppDirectoryPath, g_AppSettingsFileName, L"theme", L"Light"))
                {
                    error_message = L"Failed to update the settings file.";
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
                WriteLog(error_message, L" [MESSAGE: \"WM_KEYUP/F5\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }
            else
            {
                // The MainContent container have a change theme combobox, update its current selected index.
                if (g_Container_MainContent && IsWindow(g_Container_MainContent->pContainerWindow->hWnd))
                {
                    HWND change_theme_combobox = g_Container_MainContent->findMyWindowByID(IDC_MAINCONTENT_SELECTTHEME_COMBOBOX);
                    if (IsWindow(change_theme_combobox))
                        SendMessageW(change_theme_combobox, CB_SETCURSEL, change_theme_combobox_index, NULL);
                    else
                        WriteLog(L"The combobox window is not found.", L" [MESSAGE: \"WM_KEYUP/F5\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
                }
            }

            return 0;
        }

        // Show window debug informations.
        case VK_F1:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the container dimensions (MainContent).
                INT container_maincontent_width = 0, container_maincontent_height = 0;
                if (!g_Container_MainContent || !IsWindow(g_Container_MainContent->pContainerWindow->hWnd))
                {
                    error_message = L"The MainContent container window is invalid.";
                    break;
                }
                RECT rect_container_maincontent;
                if (!GetClientRect(g_Container_MainContent->pContainerWindow->hWnd, &rect_container_maincontent))
                {
                    error_message = L"Failed to retrieve the MainContent container's client rect.";
                    break;
                }
                container_maincontent_width = rect_container_maincontent.right - rect_container_maincontent.left;
                container_maincontent_height = rect_container_maincontent.bottom - rect_container_maincontent.top;

                // Get the log level.
                std::wstring log_level_string = std::to_wstring(g_AppLogLevel);

                // Get the current theme name string.
                std::wstring theme_string = L"";
                switch (g_CurrentAppTheme)
                {
                case MyTheme::Light:
                {
                    theme_string = L"Light";
                    break;
                }

                case MyTheme::Dark:
                {
                    theme_string = L"Dark";
                    break;
                }

                case MyTheme::Monokai:
                {
                    theme_string = L"Monokai";
                    break;
                }
                }

                // Get the sample radio group state string.
                std::wstring sample_radio_state_string = L"";
                switch (SampleRadio.getRadioState())
                {
                case IDC_MAINCONTENT_RADIO_TEST_FIRST_BUTTON:
                    sample_radio_state_string = L"First";
                    break;
                case IDC_MAINCONTENT_RADIO_TEST_SECOND_BUTTON:
                    sample_radio_state_string = L"Second";
                    break;
                case IDC_MAINCONTENT_RADIO_TEST_THIRD_BUTTON:
                    sample_radio_state_string = L"Third";
                    break;
                default:
                    sample_radio_state_string = L"None";
                    break;
                }

                // Get application window state strings.
                std::wstring is_window_ready_string = (g_IsWindowReady ? L"Yes" : L"No");
                std::wstring is_window_minimized_string = (g_IsWindowMinimized ? L"Yes" : L"No");
                std::wstring is_window_maximized_string = (g_IsWindowMaximized ? L"Yes" : L"No");
                std::wstring is_current_theme_want_scrollbar_hidden_string = (g_IsCurrentThemeWantScrollbarsVisible ? L"Yes" : L"No");
                std::wstring is_windows11_border_attribute_supported = (g_IsWindows11BorderAttributeSupported ? L"Yes" : L"No");

                // Show the debug message.
                std::wstring debug_message = L"";
                debug_message.append(L"Window dimensions: " + std::to_wstring(g_CurrentWindowWidth) + L"x" + std::to_wstring(g_CurrentWindowHeight) + L"\n");
                debug_message.append(L"Caption dimensions: " + std::to_wstring(g_pUIElements->rectangles.rectCaption.bottom - g_pUIElements->rectangles.rectCaption.top) + L"\n");
                debug_message.append(L"Container dimensions (MainContent): " + std::to_wstring(container_maincontent_width) + L"x" + std::to_wstring(container_maincontent_height) + L"\n");
                debug_message.append(L"Log level: " + log_level_string + L"\n");
                debug_message.append(L"Theme: " + theme_string + L" (Hotkey: F5)\n");
                debug_message.append(L"IsWindowReady: " + is_window_ready_string + L"\n");
                debug_message.append(L"IsWindowMinimized: " + is_window_minimized_string + L"\n");
                debug_message.append(L"IsWindowMaximized: " + is_window_maximized_string + L"\n");
                debug_message.append(L"IsCurrentThemeWantScrollbarsVisible: " + is_current_theme_want_scrollbar_hidden_string + L"\n");
                debug_message.append(L"IsWindows11BorderAttributeSupported: " + is_windows11_border_attribute_supported + L"\n");
                debug_message.append(L"Sample radio state: " + sample_radio_state_string);
                MessageBoxW(hWnd, debug_message.c_str(), L"", MB_OK | MB_ICONINFORMATION);

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_KEYUP/F1\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Error);
            }

            return 0;
        }

        case VK_F2:
        {   
            MessageBeep(MB_OK);
            g_pUIElements->showTotalInstances();
            return 0;
        }
        }

        break;
    }

    // Process WM_TIMER messages.
    case WM_TIMER:
    {
        switch (wParam)
        {
        // Window active check timer to make sure the main window drop-shadow effects are drawn correctly.
        case IDT_ACTIVE_CHECK:
        {
            if (GetActiveWindow() == hWnd)
                SendMessageW(g_hWnd, WM_NCACTIVATE, 1, 0);
            KillTimer(hWnd, IDT_ACTIVE_CHECK);

            return 0;
        }
        default:
        {
            WriteLog(L"Unexpected message.", L" [MESSAGE: \"WM_TIMER\" | CALLBACK: \"WindowProcedure()\"]", MyLogType::Warn);
            break;
        }
        }
        break;
    }

    // Process WM_CLOSE message to perform additional operations on the application window before destroy the window.
    case WM_CLOSE:
    {
        nApp::Window::Deinitialization::OnExit();
        DestroyWindow(hWnd);
        return 0;
    }

    // Process WM_DESTROY message to perform necessary deallocations before exiting the application.
    case WM_DESTROY:
    {
        // Perform necessary deallocations and deinitialize global objects, subclasses, and APIs to properly conclude the application.
        nApp::Window::Deinitialization::OnDestroy();
        PostQuitMessage(0);
        return 0;
    }

    default:
        break;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

/**
 * @brief Container: MainContent
 * @brief Container window procedure.
 *
 * @param hWnd        The handle to the window.
 * @param uMsg        The message identifier.
 * @param wParam      The first message parameter.
 * @param lParam      The second message parameter.
 * @param uIdSubclass The subclass ID.
 * @param dwRefData   DWORD_PTR to reference data.
 *
 * @return The result of the message processing.
 */
LRESULT CALLBACK WindowProcedure_Container_MainContent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
    // Remove the window subclass callback.
    case WM_NCDESTROY:
    {
        // Removes installed subclass callback from the window.
        if (!RemoveWindowSubclass(hWnd, &WindowProcedure_Container_MainContent, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Process WM_COMMAND messages.
    case WM_COMMAND:
    {
        // Use of the wParam and lParam parameters are summarized here.
        // +----------------+-----------------------------------+------------------------+-------------------------------------+
        // | Message Source | wParam (high word)                | wParam (low word)      | lParam                              |
        // +----------------+-----------------------------------+------------------------+-------------------------------------+
        // | Menu           | 0                                 | Menu identifier        | 0                                   |
        // | Accelerator    | 1                                 | Accelerator identifier | 0                                   |
        // | Control        | Control-defined notification code | Control identifier     | Handle to the control window        |
        // +----------------+-----------------------------------+------------------------+-------------------------------------+
        // Return: 0 if the application processed the message.

        // Process WM_COMMAND messages sent by combobox controls.
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            switch (LOWORD(wParam))
            {
            case IDC_MAINCONTENT_SELECTTHEME_COMBOBOX:
            {
                bool are_all_operation_success = false;
                std::wstring error_message = L"";
                while (!are_all_operation_success)
                {
                    // Get the combobox index id of the current selected item.
                    HWND hwnd_combobox = reinterpret_cast<HWND>(lParam);
                    INT combobox_index = static_cast<INT>(SendMessageW(hwnd_combobox, CB_GETCURSEL, NULL, NULL));
                    if (combobox_index == CB_ERR)
                    {
                        error_message = L"No combobox item was selected.";
                        break;
                    }

                    // Get the combobox value (in this case a string) of the current selected item.
                    // INT item_text_length = static_cast<INT>(SendMessageW(hwnd_combobox, CB_GETLBTEXTLEN, combobox_index, 0));
                    // if (item_text_length == CB_ERR)
                    // {
                    //     error_message = L"Failed to retrieve the combobox item text length.";
                    //     break;
                    // }
                    // WCHAR *text_buffer = new WCHAR[item_text_length + 1];
                    // if (SendMessageW(hwnd_combobox, CB_GETLBTEXT, combobox_index, reinterpret_cast<LPARAM>(text_buffer)) == CB_ERR)
                    // {
                    //     delete[] text_buffer;
                    //     error_message = L"Failed to retrieve the combobox item text.";
                    //     break;
                    // }
                    // std::wstring item_text(text_buffer);
                    // delete[] text_buffer;

                    // Process the combobox item index.
                    bool are_all_operation_success_2 = false;
                    switch (combobox_index)
                    {
                    case 0:
                    {
                        if (!nApp::Theme::SetAppTheme(MyTheme::Light))
                        {
                            error_message = L"Failed to set the application theme.";
                            break;
                        }

                        if (!nApp::File::UpdateSettingsFile(g_AppDirectoryPath, g_AppSettingsFileName, L"theme", L"Light", true))
                        {
                            error_message = L"Failed to update the settings file.";
                        }

                        are_all_operation_success_2 = true;
                        break;
                    }
                    case 1:
                    {
                        if (!nApp::Theme::SetAppTheme(MyTheme::Dark))
                        {
                            error_message = L"Failed to set the application theme.";
                            break;
                        }

                        if (!nApp::File::UpdateSettingsFile(g_AppDirectoryPath, g_AppSettingsFileName, L"theme", L"Dark", true))
                        {
                            error_message = L"Failed to update the settings file.";
                        }

                        are_all_operation_success_2 = true;
                        break;
                    }
                    case 2:
                    {
                        if (!nApp::Theme::SetAppTheme(MyTheme::Monokai))
                        {
                            error_message = L"Failed to set the application theme.";
                            break;
                        }

                        if (!nApp::File::UpdateSettingsFile(g_AppDirectoryPath, g_AppSettingsFileName, L"theme", L"Monokai", true))
                        {
                            error_message = L"Failed to update the settings file.";
                        }

                        are_all_operation_success_2 = true;
                        break;
                    }

                    default:
                    {
                        error_message = L"Unexpected combobox index message.";
                        break;
                    }
                    }

                    if (!are_all_operation_success_2)
                        break;

                    are_all_operation_success = true;
                }

                if (!are_all_operation_success)
                {
                    WriteLog(error_message, L" [MESSAGE: \"WM_COMMAND/CBN_SELCHANGE/IDC_MAINCONTENT_SELECTTHEME_COMBOBOX\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
                }

                return 0;
            }

            default:
            {
                WriteLog(L"Unexpected notification code message.", L" [MESSAGE: \"WM_COMMAND/CBN_SELCHANGE\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }
            }

            return 0;
        }

        // Process WM_COMMAND messages sent by controls.
        switch (LOWORD(wParam))
        {
        case IDC_MAINCONTENT_STANDARDBUTTON_BUTTON:
        {
            WriteLog(L"Standard button clicked.", L"", MyLogType::Info);
            MessageBeep(MB_OK);
            return 0;
        }

        case IDC_MAINCONTENT_RADIO_TEST_FIRST_BUTTON:
        {
            WriteLog(L"First button of the sample radio group selected.", L"", MyLogType::Info);
            return 0;
        }

        case IDC_MAINCONTENT_RADIO_TEST_SECOND_BUTTON:
        {
            WriteLog(L"Second button of the sample radio group selected.", L"", MyLogType::Info);
            return 0;
        }

        case IDC_MAINCONTENT_RADIO_TEST_THIRD_BUTTON:
        {
            WriteLog(L"Third button of the sample radio group selected.", L"", MyLogType::Info);
            return 0;
        }

        case IDC_MAINCONTENT_STANDARDEDITBOX_BUTTON:
        {
            WriteLog(L"Standard editbox's OK button clicked.", L"", MyLogType::Info);

            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the editbox window.
                HWND hwnd_editbox = GetDlgItem(hWnd, IDC_MAINCONTENT_STANDARDEDITBOX_EDIT);
                if (!IsWindow(hwnd_editbox))
                {
                    error_message = L"Failed to retrieve the editbox window object.";
                    break;
                }

                // Get the editbox text length.
                INT text_length = GetWindowTextLengthW(hwnd_editbox);
                SetLastError(NULL);
                if (!text_length && GetLastError() != NULL)
                {
                    error_message = L"Failed to get the editbox text length.";
                    break;
                }

                // Get the editbox text.
                WCHAR *text_buffer = new WCHAR[text_length + 1];
                if (!GetWindowTextW(hwnd_editbox, text_buffer, text_length + 1) && text_length != NULL)
                {
                    delete[] text_buffer;
                    error_message = L"Failed to get the editbox text.";
                    break;
                }
                std::wstring editbox_text(text_buffer);
                delete[] text_buffer;

                // Set the editbox text to empty string.
                if (!SetWindowTextW(hwnd_editbox, L""))
                {
                    error_message = L"Failed to set the editbox text to empty string.";
                    break;
                }

                // Check if the edit box is empty. If it is, play an warning sound. Otherwise, display the entered text in a message box.
                if (editbox_text.empty())
                    MessageBeep(MB_ICONWARNING);
                else
                    MessageBoxW(hWnd, (L"Entered:\n" + editbox_text).c_str(), L"", MB_OK | MB_ICONINFORMATION);

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_COMMAND/IDC_MAINCONTENT_STANDARDEDITBOX_BUTTON\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }

            return 0;
        }

        case IDC_MAINCONTENT_PASSWORDEDITBOX_BUTTON:
        {
            WriteLog(L"Password editbox's OK button clicked.", L"", MyLogType::Info);

            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the editbox window.
                HWND hwnd_editbox = GetDlgItem(hWnd, IDC_MAINCONTENT_PASSWORDEDITBOX_EDIT);
                if (!IsWindow(hwnd_editbox))
                {
                    error_message = L"Failed to retrieve the editbox window object.";
                    break;
                }

                // Get the editbox text length.
                INT text_length = GetWindowTextLengthW(hwnd_editbox);
                SetLastError(NULL);
                if (!text_length && GetLastError() != NULL)
                {
                    error_message = L"Failed to get the editbox text length.";
                    break;
                }

                // Get the editbox text.
                WCHAR *text_buffer = new WCHAR[text_length + 1];
                if (!GetWindowTextW(hwnd_editbox, text_buffer, text_length + 1) && text_length != NULL)
                {
                    delete[] text_buffer;
                    error_message = L"Failed to get the editbox text.";
                    break;
                }
                std::wstring editbox_text(text_buffer);
                delete[] text_buffer;

                // Set the editbox text to empty string.
                if (!SetWindowTextW(hwnd_editbox, L""))
                {
                    error_message = L"Failed to set the editbox text to empty string.";
                    break;
                }

                // Check if the edit box is empty. If it is, play a warning sound.
                // If it's not empty and the entered text is "12345678", display a message box saying that the password is correct.
                // Otherwise, display a message box saying that the password is incorrect and show the correct password.
                if (editbox_text.empty())
                    MessageBeep(MB_ICONWARNING);
                else if (editbox_text == L"12345678")
                    MessageBoxW(hWnd, L"Password is correct.", L"", MB_OK | MB_ICONINFORMATION);
                else
                    MessageBoxW(hWnd, L"Password is incorrect.\n(The password is: 12345678)", L"", MB_OK | MB_ICONWARNING);

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_COMMAND/IDC_MAINCONTENT_PASSWORDEDITBOX_BUTTON\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }

            return 0;
        }

        case IDC_MAINCONTENT_MULTILINEEDITBOX_BUTTON:
        {
            WriteLog(L"Multiline editbox's OK button clicked.", L"", MyLogType::Info);

            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the editbox window.
                HWND hwnd_editbox = GetDlgItem(hWnd, IDC_MAINCONTENT_MULTILINEEDITBOX_EDIT);
                if (!IsWindow(hwnd_editbox))
                {
                    error_message = L"Failed to retrieve the editbox window object.";
                    break;
                }

                // Get the editbox text length.
                INT text_length = GetWindowTextLengthW(hwnd_editbox);
                SetLastError(NULL);
                if (!text_length && GetLastError() != NULL)
                {
                    error_message = L"Failed to get the editbox text length.";
                    break;
                }

                // Get the editbox text.
                WCHAR *text_buffer = new WCHAR[text_length + 1];
                if (!GetWindowTextW(hwnd_editbox, text_buffer, text_length + 1) && text_length != NULL)
                {
                    delete[] text_buffer;
                    error_message = L"Failed to get the editbox text.";
                    break;
                }
                std::wstring editbox_text(text_buffer);
                delete[] text_buffer;

                // Set the editbox text to empty string.
                if (!SetWindowTextW(hwnd_editbox, L""))
                {
                    error_message = L"Failed to set the editbox text to empty string.";
                    break;
                }

                // Check if the edit box is not empty. If it's not empty, set the edit box text to an empty string and play an OK sound.
                if (!editbox_text.empty())
                {
                    if (!SetWindowTextW(hwnd_editbox, L""))
                    {
                        error_message = L"Failed to set the editbox text to empty string.";
                        break;
                    }
                    MessageBeep(MB_OK);
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_COMMAND/IDC_MAINCONTENT_MULTILINEEDITBOX_EDIT\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }

            return 0;
        }
        }

        break;
    }

    // Process background erase requests.
    case WM_ERASEBKGND:
    {
        // Always return non-zero to indicate that no further erasing is required, avoid flickering.
        return 1;
    }

    // Process WM_CTLCOLORSTATIC messages to set the text foreground and background colors of the static control, read-only/disabled edit control.
    case WM_CTLCOLORSTATIC:
    {
        HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        INT window_id = GetDlgCtrlID(window);

        switch (window_id)
        {
        // Priority processings.

        // Heading static texts.
        case IDC_MAINCONTENT_SAMPLEBUTTONS_STATIC:
        case IDC_MAINCONTENT_SAMPLEEDITBOXES_STATIC:
        case IDC_MAINCONTENT_SAMPLECOMBOBOXES_STATIC:
        case IDC_MAINCONTENT_TESTZONE_STATIC:
        {
            SetBkColor(hdc, g_pUIElements->colors.background.getCOLORREF());
            SetTextColor(hdc, g_pUIElements->colors.textActive.getCOLORREF());
            p_background_brush = &g_pUIElements->colors.background.getHBRUSH();
            break;
        }

        // Note static texts.
        case IDC_MAINCONTENT_STANDARDEDITBOXNOTE_STATIC:
        case IDC_MAINCONTENT_PASSWORDEDITBOXNOTE_STATIC:
        case IDC_MAINCONTENT_MULTILINEEDITBOXNOTE_STATIC:
        case IDC_MAINCONTENT_SELECTTHEMENOTE_STATIC:
        {
            SetBkColor(hdc, g_pUIElements->colors.background.getCOLORREF());
            SetTextColor(hdc, g_pUIElements->colors.textInactive.getCOLORREF());
            p_background_brush = &g_pUIElements->colors.background.getHBRUSH();
            break;
        }

        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the window class name.
                std::wstring class_name = nSol::GetWindowClassName(window);
                if (class_name.empty())
                {
                    error_message = L"Failed to retrieve the window class name.";
                    break;
                }

                // Default processing for static controls.
                if (class_name == WC_STATIC)
                {
                    // Subclassed controls.
                    void *p_subclass = nullptr;
                    if (MyEdit::isSubclassed(window, true, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    // Standard controls.
                    else
                    {
                        SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                        auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                        p_background_brush = &default_background_brush;
                        WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                                 L" [MESSAGE: \"WM_CTLCOLORSTATIC\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                    }
                }
                // Default processing for read-only/disabled edit controls.
                else if (class_name == WC_EDIT)
                {
                    // Subclassed controls.
                    void *p_subclass = nullptr;
                    if (MyEdit::isSubclassed(window, false, &p_subclass))
                    {
                        SetBkMode(hdc, OPAQUE);
                        SetBkColor(hdc, g_pUIElements->colors.editbox.getCOLORREF());
                        SetTextColor(hdc, g_pUIElements->colors.textInactive.getCOLORREF());
                        p_background_brush = &g_pUIElements->colors.editbox.getHBRUSH();
                    }
                    // Standard controls.
                    else
                    {
                        SetBkMode(hdc, OPAQUE);
                        SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                        auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                        p_background_brush = &default_background_brush;
                        WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                                 L" [MESSAGE: \"WM_CTLCOLORSTATIC\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                    }
                }
                // Default processing for unexpected class name.
                else
                {
                    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                    auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                    p_background_brush = &default_background_brush;
                    WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                             L" [MESSAGE: \"WM_CTLCOLORSTATIC\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                p_background_brush = &default_background_brush;
                WriteLog(error_message, L" [MESSAGE: \"WM_CTLCOLORSTATIC\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }

            break;
        }
        }

        return reinterpret_cast<LRESULT>(*p_background_brush);
    }

    // Process WM_CTLCOLORBTN messages to set the text foreground and background colors of the button control.
    case WM_CTLCOLORBTN:
    {
        HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        INT window_id = GetDlgCtrlID(window);

        switch (window_id)
        {
        // Priority processings.
        // Expect no priority messages now, use 6969 as a placeholder to suppress IDE warnings.
        case 6969:
            break;
        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the window class name.
                std::wstring class_name = nSol::GetWindowClassName(window);
                if (class_name.empty())
                {
                    error_message = L"Failed to retrieve the window class name.";
                    break;
                }

                // Default processing for button controls.
                if (class_name == WC_BUTTON)
                {
                    // Subclassed controls.
                    void *p_subclass = nullptr;
                    if (MyStandardButton::isSubclassed(window, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    else if (MyImageButton::isSubclassed(window, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    else if (MyRadioButton::isSubclassed(window, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    // Standard controls.
                    else
                    {
                        SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                        auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                        p_background_brush = &default_background_brush;
                        WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                                 L" [MESSAGE: \"WM_CTLCOLORBTN\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                    }
                }
                // Default processing for unexpected class name.
                else
                {
                    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                    auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                    p_background_brush = &default_background_brush;
                    WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                             L" [MESSAGE: \"WM_CTLCOLORBTN\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                p_background_brush = &default_background_brush;
                WriteLog(error_message, L" [MESSAGE: \"WM_CTLCOLORBTN\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }

            break;
        }
        }

        return reinterpret_cast<LRESULT>(*p_background_brush);
    }

    // Process WM_CTLCOLOREDIT messages to set the text foreground and background colors of the edit control.
    case WM_CTLCOLOREDIT:
    {
        HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        INT window_id = GetDlgCtrlID(window);

        switch (window_id)
        {
        // Priority processings.
        // Expect no priority messages now, use 6969 as a placeholder to suppress IDE warnings.
        case 6969:
            break;
        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the window class name.
                std::wstring class_name = nSol::GetWindowClassName(window);
                if (class_name.empty())
                {
                    error_message = L"Failed to retrieve the window class name.";
                    break;
                }

                // Default processing for edit controls.
                if (class_name == WC_EDIT)
                {
                    // Subclassed controls.
                    void *p_subclass = nullptr;
                    if (MyEdit::isSubclassed(window, false, &p_subclass))
                    {
                        SetBkMode(hdc, OPAQUE);
                        SetBkColor(hdc, g_pUIElements->colors.editbox.getCOLORREF());
                        SetTextColor(hdc, g_pUIElements->colors.textActive.getCOLORREF());
                        p_background_brush = &g_pUIElements->colors.editbox.getHBRUSH();
                    }
                    // Standard controls.
                    else
                    {
                        SetBkMode(hdc, OPAQUE);
                        SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                        auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                        p_background_brush = &default_background_brush;
                        WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                                 L" [MESSAGE: \"WM_CTLCOLOREDIT\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                    }
                }
                // Default processing for combobox controls.
                else if (class_name == WC_COMBOBOX)
                {
                    // Subclassed controls.
                    void *p_subclass = nullptr;
                    if (MyDDLCombobox::isSubclassed(window, &p_subclass))
                    {
                        p_background_brush = &g_pUIElements->colors.nullBrush;
                    }
                    // Standard controls.
                    else
                    {
                        // This message shouldn't be tempered with. Otherwise the non-subclassed CBS_DROPDOWNLIST combobox may misbehave.
                        // Set background brush pointer to nullptr to use default system processing.
                        p_background_brush = nullptr;
                    }
                }
                // Default processing for unexpected class name.
                else
                {
                    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                    auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                    p_background_brush = &default_background_brush;
                    WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                             L" [MESSAGE: \"WM_CTLCOLOREDIT\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                p_background_brush = &default_background_brush;
                WriteLog(error_message, L" [MESSAGE: \"WM_CTLCOLOREDIT\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }

            break;
        }
        }

        if (!p_background_brush)
            break;
        return reinterpret_cast<LRESULT>(*p_background_brush);
    }

    // Process WM_CTLCOLORLISTBOX messages to set the text and background colors of the list box control.
    case WM_CTLCOLORLISTBOX:
    {
        HBRUSH *p_background_brush = nullptr;
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND window = reinterpret_cast<HWND>(lParam);
        INT window_id = GetDlgCtrlID(window);

        switch (window_id)
        {
        // Priority processings.
        // Expect no priority messages now, use 6969 as a placeholder to suppress IDE warnings.
        case 6969:
            break;
        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the window class name.
                std::wstring class_name = nSol::GetWindowClassName(window);
                if (class_name.empty())
                {
                    error_message = L"Failed to retrieve the window class name.";
                    break;
                }

                // Check if the window is a drop-down list of a CBS_DROPDOWNLIST combobox.
                if (class_name == L"ComboLBox")
                {
                    // This message shouldn't be tempered with. Otherwise the non-subclassed CBS_DROPDOWNLIST combobox may misbehave.
                    // Set background brush pointer to nullptr to use default system processing.
                    p_background_brush = nullptr;
                }
                // The application doesn't have any listbox samples yet, always use default processing.
                else
                {
                    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                    auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                    p_background_brush = &default_background_brush;
                    WriteLog(L"The message is handled by default processing. (Window class: " + class_name + L" | Window id: " + std::to_wstring(window_id) + L")",
                             L" [MESSAGE: \"WM_CTLCOLORLISTBOX\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Debug);
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                auto default_background_brush = GetSysColorBrush(COLOR_WINDOW);
                p_background_brush = &default_background_brush;
                WriteLog(error_message, L" [MESSAGE: \"WM_CTLCOLORLISTBOX\" | CALLBACK: \"WindowProcedure_Container_MainContent\"]", MyLogType::Error);
            }

            break;
        }
        }

        if (!p_background_brush)
            break;
        return reinterpret_cast<LRESULT>(*p_background_brush);
    }

    // Process WM_MEASUREITEM messages to set the dimensions of owner-drawn controls and menus.
    case WM_MEASUREITEM:
    {
        // Get the pointer to the measure item structure.
        LPMEASUREITEMSTRUCT measure_items_struct = reinterpret_cast<LPMEASUREITEMSTRUCT>(lParam);

        // Check if the message is sent by a menu.
        if (!wParam && measure_items_struct->CtlType == ODT_MENU)
        {
            // The application doesn't have any menu samples yet, always use default processing.
            WriteLog(L"Unexpected message.", L" [MESSAGE: \"WM_MEASUREITEM/ODT_MENU\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Warn);
            break;
        }

        // Check if the message is sent by other types.
        switch (measure_items_struct->CtlType)
        {
        // Set item metrics for comboboxes.
        case ODT_COMBOBOX:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Set the combobox item height to the height of the font plus 10.
                measure_items_struct->itemHeight = g_pUIElements->fonts.ddlCombobox.getSize() + 10;

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_MEASUREITEM/ODT_COMBOBOX\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Error);
            }

            return TRUE;
        }

        // Set item metrics for listboxes/listviews.
        case ODT_LISTBOX:
        case ODT_LISTVIEW:
        {
            // The application doesn't have any listbox/listview samples yet, always use default processing.
            WriteLog(L"Unexpected message.", L" [MESSAGE: \"WM_MEASUREITEM/ODT_LISTBOX||ODT_LISTVIEW\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Warn);
            break;
        }
        }

        break;
    }

    // Process WM_DRAWITEM messages to draw the content of owner-drawn controls and menus.
    case WM_DRAWITEM:
    {
        // Get the pointer to the draw items structure.
        LPDRAWITEMSTRUCT draw_items_struct = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

        switch (wParam)
        {
        // Check if the message is sent by a menu.
        case 0:
        {
            // The application doesn't have any menu samples yet, always use default processing.
            WriteLog(L"Unexpected message.", L" [MESSAGE: \"WM_DRAWITEM/0\" | CALLBACK: \"WindowProcedure_Container_MainContent()\"]", MyLogType::Warn);
            break;
        }
        // Priority processings.
        // ...

        // Default processing.
        default:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                bool message_processed_successfully = false;
                switch (draw_items_struct->CtlType)
                {
                // Comboboxes.
                case ODT_COMBOBOX:
                {
                    // Subclassed controls.
                    if (MyDDLCombobox::isSubclassed(draw_items_struct->hwndItem))
                    {
                        // Check if item ID is valid.
                        if (draw_items_struct->itemID == CB_ERR)
                        {
                            error_message = L"Invalid item ID or system ran out of memory.";
                            break;
                        }

                        // Get the combobox item text length.
                        INT text_length = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXTLEN, draw_items_struct->itemID, 0));
                        if (text_length == CB_ERR)
                        {
                            error_message = L"Failed to retrieve the combobox item text length.";
                            break;
                        }

                        // Get the combobox item text.
                        WCHAR *text_buffer = new WCHAR[static_cast<size_t>(text_length) + static_cast<size_t>(1)];
                        if (SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXT, draw_items_struct->itemID, reinterpret_cast<LPARAM>(text_buffer)) == CB_ERR)
                        {
                            delete[] text_buffer;
                            error_message = L"Failed to retrieve the combobox item text.";
                            break;
                        }
                        std::wstring text_string(text_buffer);
                        delete[] text_buffer;

                        // Prepare COLORREF variables for the background and text colors.
                        COLORREF background_color = g_pUIElements->colors.ddlComboboxItemBackground.getCOLORREF();
                        COLORREF text_color = g_pUIElements->colors.ddlComboboxItemTextDefault.getCOLORREF();

                        if (draw_items_struct->itemAction & ODA_DRAWENTIRE)
                        {
                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = g_pUIElements->colors.ddlComboboxItemBackground.getCOLORREF();
                                text_color = g_pUIElements->colors.ddlComboboxItemTextDefault.getCOLORREF();
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = g_pUIElements->colors.ddlComboboxSelectedItemBackground.getCOLORREF();
                                text_color = g_pUIElements->colors.ddlComboboxItemTextSelected.getCOLORREF();
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
                            {
                                RedrawWindow(draw_items_struct->hwndItem, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
                            }

                            if (draw_items_struct->itemState & ODS_COMBOBOXEDIT)
                            {
                                background_color = g_pUIElements->colors.ddlComboboxItemBackground.getCOLORREF();
                                text_color = g_pUIElements->colors.ddlComboboxItemTextDefault.getCOLORREF();
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = g_pUIElements->colors.ddlComboboxSelectedItemBackground.getCOLORREF();
                                text_color = g_pUIElements->colors.ddlComboboxItemTextSelected.getCOLORREF();
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
                                background_color = g_pUIElements->colors.ddlComboboxItemBackground.getCOLORREF();
                                text_color = g_pUIElements->colors.ddlComboboxItemTextDefault.getCOLORREF();
                            }
                            else if (draw_items_struct->itemState & ODS_SELECTED)
                            {
                                background_color = g_pUIElements->colors.ddlComboboxSelectedItemBackground.getCOLORREF();
                                text_color = g_pUIElements->colors.ddlComboboxItemTextSelected.getCOLORREF();
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
                            error_message = L"Failed to retrieve the combobox info.";
                            break;
                        }
                        HWND hwnd_dropdownlist = combobox_info.hwndList;

                        // Get the combobox drop down list's client rect.
                        RECT rect_dropdownlist;
                        if (!GetClientRect(hwnd_dropdownlist, &rect_dropdownlist))
                        {
                            error_message = L"Failed to retrieve the combobox drop down list's client rect.";
                            break;
                        }

                        // Get the combobox drop down list's device context.
                        HDC hdc = GetWindowDC(hwnd_dropdownlist);
                        if (!hdc)
                        {
                            error_message = L"Failed to retrieve the combobox drop down list's device context.";
                            break;
                        }

                        // Get the total items of the combobox to get the ID of the last item.
                        INT total_combobox_items = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETCOUNT, NULL, NULL));
                        if (total_combobox_items == CB_ERR)
                        {
                            error_message = L"Failed to retrieve the total combobox items.";
                            break;
                        }

                        // Get current selectet combobox item index.
                        INT current_selected_combobox_item = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETCURSEL, NULL, NULL));
                        if (current_selected_combobox_item == CB_ERR)
                        {
                            error_message = L"Failed to retrieve the current selected combobox item index.";
                            break;
                        }

                        // Fill the 2-pixels gap to match the color of the last item background color.
                        RECT rect_dropdownlist_2 = rect_dropdownlist;
                        rect_dropdownlist_2.top = --rect_dropdownlist_2.bottom - 1;
                        FillRect(hdc, &rect_dropdownlist_2,
                                 (current_selected_combobox_item == (total_combobox_items - 1)
                                      ? g_pUIElements->colors.ddlComboboxSelectedItemBackground.getHBRUSH()
                                      : g_pUIElements->colors.ddlComboboxItemBackground.getHBRUSH()));

                        // Draw the drop down list borders.
                        FrameRect(hdc, &rect_dropdownlist, g_pUIElements->colors.ddlComboboxDropdownlistBorder.getHBRUSH());

                        // Release the combobox drop down list's device context.
                        if (!ReleaseDC(hwnd_dropdownlist, hdc))
                        {
                            error_message = L"Failed to release the combobox drop down list's device context.";
                            break;
                        }

                        message_processed_successfully = true;
                    }
                    // Standard controls.
                    // Note: Add the processing of non-subclassed controls anyway for the sake of demonstration.
                    else
                    {
                        // Check if item ID is valid.
                        if (draw_items_struct->itemID == CB_ERR)
                        {
                            error_message = L"Invalid item ID or system ran out of memory.";
                            break;
                        }

                        // Get the combobox item text length.
                        INT text_length = static_cast<INT>(SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXTLEN, draw_items_struct->itemID, 0));
                        if (text_length == CB_ERR)
                        {
                            error_message = L"Failed to retrieve the combobox item text length.";
                            break;
                        }

                        // Get the combobox item text.
                        WCHAR *text_buffer = new WCHAR[static_cast<size_t>(text_length) + static_cast<size_t>(1)];
                        if (SendMessageW(draw_items_struct->hwndItem, CB_GETLBTEXT, draw_items_struct->itemID, reinterpret_cast<LPARAM>(text_buffer)) == CB_ERR)
                        {
                            delete[] text_buffer;
                            error_message = L"Failed to retrieve the combobox item text.";
                            break;
                        }
                        std::wstring text_string(text_buffer);
                        delete[] text_buffer;

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
                    error_message = L"Unexpected control type.";
                    break;
                }
                }
                if (!message_processed_successfully)
                    break;

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [MESSAGE: \"WM_DRAWITEM\" | CALLBACK: \"WindowProcedure_Container_MainContent\"]", MyLogType::Error);
                break;
            }

            return TRUE;
        }
        }

        break;
    }

    // Process WM_SETFOCUS message to redirect the container focus to the main window.
    case WM_SETFOCUS:
    {
        SetFocus(g_hWnd);
        break;
    }

    // Process WM_UPDATEUISTATE & WM_CHANGEUISTATE messages.
    case WM_UPDATEUISTATE:
    case WM_CHANGEUISTATE:
    {
        // For now the application doesn't have any UI elements that require these messages.
        // Always return 0 to prevent unnecessary messages.
        return 0;
    }

    default:
        return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}