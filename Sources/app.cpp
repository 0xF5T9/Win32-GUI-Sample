/**
 * @file app.cpp
 * @brief Implementation of the user-defined types defined in app.h.
 */

#include "../Headers/standard_includes.h" // Include standard libraries.
#include "../Headers/my_includes.h"       // Include my user-defined type headers.

// [MyApp] class implementations:

MyApp::MyApp()
{
    if (!MyApp::instanceExists)
        MyApp::instanceExists = true;
    else
    {
        this->logger.writeLog("The MyApp instance already exists.", "[CLASS: 'MyApp' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The MyApp instance already exists.");
    }
}
MyApp::~MyApp()
{
    MyApp::instanceExists = false;
}
bool MyApp::isInitialized() const
{
    return this->initialized;
}
bool MyApp::initialize(HINSTANCE hInstance)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->initialized)
        {
            error_message = "The application is already initialized.";
            break;
        }

        // Enable visual style for the application.
        {
            INITCOMMONCONTROLSEX cts;
            cts.dwSize = sizeof(INITCOMMONCONTROLSEX);
            cts.dwICC = ICC_STANDARD_CLASSES;
            if (!InitCommonControlsEx(&cts))
            {
                error_message = "Failed to initialize the common controls.";
                break;
            }
            this->logger.writeLog("Visual style enabled.", "", MyLogType::Debug);
        }

        // Enable DPI Awareness.
        if (!SetProcessDPIAware())
        {
            error_message = "Failed to set DPI awareness.";
            break;
        }
        this->logger.writeLog("DPI awareness is set to system-aware.", "", MyLogType::Debug);

        // Enable the DWM MCSS.
        {
            HRESULT hr = DwmEnableMMCSS(TRUE);
            if (FAILED(hr))
            {
                error_message = "Failed to enable Multimedia Class Schedule Service (MMCSS).";
                break;
            }
            this->logger.writeLog("Multimedia Class Schedule Service (MMCSS) enabled.", "", MyLogType::Debug);
        }

        // Set the minimum resolution for periodic timers to increase the precision of application timers (WM_TIMER).
        {
            if (timeBeginPeriod(1) != TIMERR_NOERROR)
            {
                error_message = "Failed to set minimum resolution for periodic timers.";
                break;
            }
            this->logger.writeLog("Minimum resolution for periodic timers sets (1).", "", MyLogType::Debug);
        }

        // Register my window class.
        {
            WNDCLASSW my_class = {0};
            my_class.hbrBackground = NULL;
            my_class.hCursor = LoadCursorW(NULL, IDC_ARROW);
            my_class.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(MAINICON));
            my_class.hInstance = hInstance;
            my_class.lpszClassName = this->windowClassName;
            my_class.lpfnWndProc = ApplicationWindowProcedure;
            this->hInstance = hInstance;
            if (!RegisterClassW(&my_class))
            {
                error_message = "Failed to register the window class.";
                break;
            }
            this->logger.writeLog("Registered window class:", ("'" + MyUtility::ConvertWideStringToString(this->windowClassName) + "'").c_str(), MyLogType::Debug);
        }

        // Retrieve the user Windows version to ensure compatibility.
        {
            auto module_ntdll = GetModuleHandleW(L"ntdll.dll");
            auto rtl_get_version_number = reinterpret_cast<void(WINAPI *)(LPDWORD major, LPDWORD minor, LPDWORD build)>(GetProcAddress(module_ntdll, "RtlGetNtVersionNumbers"));
            if (!rtl_get_version_number)
            {
                error_message = "Failed to retrieve the windows version.";
                break;
            }
            rtl_get_version_number(&this->windowsMajorVersion, &this->windowsMinorVersion, &this->windowsBuildNumber);
            this->windowsBuildNumber &= ~0xF0000000;
            if (this->windowsBuildNumber >= 22000)
                this->isWindows11BorderAttributeSupported = true;
            this->logger.writeLog("Windows version:", ("'" + std::to_string(this->windowsMajorVersion) + "." + std::to_string(this->windowsMinorVersion) + "." + std::to_string(this->windowsBuildNumber) + " Build " + std::to_string(this->windowsBuildNumber) + "'").c_str(), MyLogType::Debug);
        }

        // Load WinAPI libraries.
        {
            // 'Msftedit.dll' : Rich edit window controls.
            if (!LoadLibraryW(L"Msftedit.dll"))
            {
                error_message = "Failed to load WinAPI libraries.";
                break;
            }
            this->logger.writeLog("WinAPI libraries loaded successfully.", "", MyLogType::Debug);
        }

        // Initialize the COM library.
        {
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
            if (FAILED(hr))
            {
                error_message = "Failed to initialize the COM library.";
                break;
            }
            this->logger.writeLog("COM library initialized successfully.", "", MyLogType::Debug);
        }

        // Initialize graphic engine.
        {
            this->pGraphic = std::make_unique<MyGraphicEngine>();
            if (!pGraphic->initialize())
            {
                error_message = "Failed to initialize the graphic engine.";
                break;
            }
            this->logger.writeLog("Graphic engine initialized successfully.", "", MyLogType::Debug);
        }

        // Initialize UI manager.
        {
            this->pUIManager = std::make_unique<UIManager>(this->pGraphic.get(), this->hWnd);
            this->logger.writeLog("UI manager initialized successfully.", "", MyLogType::Debug);
        }

        // Initialize my subclass classes.
        {
            if (!InitializeSubclassClasses(&this->pGraphic, &this->pUIManager, this->hWnd))
            {
                error_message = "Failed to initialize subclass classes.";
                break;
            }
            this->logger.writeLog("Subclass classes initialized successfully.", "", MyLogType::Debug);
        }

        this->initialized = true;
        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        this->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'initialize()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::uninitialize()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The application is not initialized yet.";
            break;
        }

        // Uninitialize subclass classes.
        {
            // Release left-over D2D1 resources of the subclass classes.
            MyStandardButtonSubclass::releaseSharedDeviceResources();
            MyImageButtonSubclass::releaseSharedDeviceResources();
            MyRadioButtonSubclass::releaseSharedDeviceResources();
            MyEditboxSubclass::releaseSharedDeviceResources();
            MyDDLComboboxSubclass::releaseSharedDeviceResources();
            MyVerticalScrollbarSubclass::releaseSharedDeviceResources();
            this->logger.writeLog("Subclass classes resources released.", "", MyLogType::Debug);
            if (!UninitializeSubclassClasses())
            {
                error_message = "Failed to uninitialize subclass classes.";
                break;
            }
            this->logger.writeLog("Subclass classes uninitialized.", "", MyLogType::Debug);
        }

        // Uninitialize UI manager.
        this->pUIManager.reset();
        this->logger.writeLog("UI manager uninitialized.", "", MyLogType::Debug);

        // Uninitialize the graphic engine.
        if (!this->pGraphic->uninitialize())
        {
            error_message = "Failed to uninitialize the graphic engine.";
            break;
        }
        this->pGraphic.reset();
        this->logger.writeLog("Graphic engine uninitialized.", "", MyLogType::Debug);

        // Uninitialize the COM library.
        CoUninitialize();
        this->logger.writeLog("COM library uninitialized.", "", MyLogType::Debug);

        // Unregister my window class.
        if (!UnregisterClassW(this->windowClassName, this->hInstance))
        {
            error_message = "Failed to unregister the window class.";
            break;
        }
        this->logger.writeLog("Window class unregistered.", "", MyLogType::Debug);

        // Clears previously set minimum timer resolution.
        {
            if (timeEndPeriod(1) != TIMERR_NOERROR)
            {
                error_message = "Failed to clear previously set minimum timer resolution.";
                break;
            }
            this->logger.writeLog("Previously set minimum timer resolution cleared.", "", MyLogType::Debug);
        }

        this->initialized = false;
        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        this->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'uninitialize()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::loadConfigurationFile()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        std::wstring command_line = GetCommandLineW();
        if (command_line.size() <= 2)
        {
            error_message = "Unexpected command line string.";
            throw std::runtime_error("Unexpected command line string.");
            break;
        }
        command_line.erase(0, 1);
        command_line.pop_back();
        std::filesystem::path path(command_line);

        std::ifstream config_file((path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + this->configurationFileName.c_str()).c_str()); // Create file reading stream.
        if (config_file.is_open())
        {
            std::string line;
            while (std::getline(config_file, line))
            {
                // line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end()); // Remove all whitespaces from the line.
                if (line[0] == '#' || line.empty())
                    continue; // Ignore lines starting with '#' or empty lines.

                auto delimiter_pos = line.find("=");
                auto name = line.substr(0, delimiter_pos);
                auto value = line.substr(delimiter_pos + 1);
                if (value[0] == ' ')
                    value = line.substr(delimiter_pos + 2);

                if (MyUtility::ContainsWord(name, "Debug")) // Debug level.
                {
                    // Perform additional check on the value string to avoid crashes from stoi() function.
                    std::string converted_value = MyUtility::RemoveNonDigitFromString(value);
                    if (!converted_value.empty() && MyUtility::ContainsDigit(converted_value))
                    {
                        int int_value = std::stoi(converted_value);
                        this->logger.setLogLevel(int_value);
                    }
                    else
                        this->logger.setLogLevel(1);
                }
                else if (MyUtility::ContainsWord(name, "Theme")) // Theme.
                {
                    if (MyUtility::ContainsWord(value, "Light"))
                        this->windowTheme = MyTheme::Light;
                    else if (MyUtility::ContainsWord(value, "Dark"))
                        this->windowTheme = MyTheme::Dark;
                    else if (MyUtility::ContainsWord(value, "Monokai"))
                        this->windowTheme = MyTheme::Monokai;
                    else
                        this->windowTheme = MyTheme::Monokai;
                }
            }
            config_file.close();
        }
        else
        {
            std::ofstream new_config_file((path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + this->configurationFileName.c_str()).c_str());
            if (!new_config_file.is_open())
            {
                error_message = "Failed to create the configuration file.";
                break;
            }

            new_config_file << "# Available themes: Light, Dark, Monokai.\n# Available settings: Debug\n\nTheme = Monokai\nDebug = 1";
            new_config_file.close();
            continue;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        this->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'loadConfigurationFile()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::updateConfigurationFile(std::string optionName, std::string optionValue)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        std::wstring command_line = GetCommandLineW();
        if (command_line.size() <= 2)
        {
            error_message = "Unexpected command line string.";
            throw std::runtime_error("Unexpected command line string.");
            break;
        }
        command_line.erase(0, 1);
        command_line.pop_back();
        std::filesystem::path path(command_line);

        std::string current_app_debug_level_string = "";
        std::string current_app_theme_string = "";
        std::string final_string = "";

        std::ofstream config_file((path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + this->configurationFileName.c_str()).c_str());
        if (!config_file.is_open())
        {
            error_message = "Failed to create the configuration file.";
            break;
        }

        if (MyUtility::ContainsWord(optionName, "debug"))
        {
            switch (this->windowTheme)
            {
            case MyTheme::Light:
                current_app_theme_string = "Light";
                break;
            case MyTheme::Dark:
                current_app_theme_string = "Dark";
                break;
            case MyTheme::Monokai:
                current_app_theme_string = "Monokai";
                break;
            }
            current_app_debug_level_string += optionValue;
        }
        else if (MyUtility::ContainsWord(optionName, "theme"))
        {
            current_app_theme_string += optionValue;
            current_app_debug_level_string += std::to_string(this->logger.getLogLevel());
        }
        else
        {
            error_message = "Invalid option name.";
            break;
        }

        final_string += "# Available themes: Light, Dark, Monokai.\n# Available settings: Debug\n\nTheme = ";
        final_string += current_app_theme_string;
        final_string += "\nDebug = ";
        final_string += current_app_debug_level_string;
        config_file << final_string;
        config_file.close();

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        this->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'updateConfigurationFile()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::setAppTheme(MyTheme theme)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->isWindowReady)
        {
            error_message = "The application window isn't ready yet.";
            break;
        }

        // Update UI resources.
        this->windowTheme = theme;
        if (!this->pUIManager->updateUITheme(this->windowTheme))
        {
            error_message = "Failed to update the UI resources.";
            break;
        }

        // Perform extra actions based on the theme if needed.
        // e.g isAlwaysHideScrollbars = true
        bool is_switch_success = false;
        std::string theme_name = "";
        switch (this->windowTheme)
        {
        case MyTheme::Light:
        {
            theme_name = "Light";
            is_switch_success = true;
            break;
        }

        case MyTheme::Dark:
        {
            theme_name = "Dark";
            is_switch_success = true;
            break;
        }

        case MyTheme::Monokai:
        {
            theme_name = "Monokai";
            is_switch_success = true;
            break;
        }
        default:
            error_message = "Unknown theme.";
            break;
        }
        if (!is_switch_success)
            break;

        // Release all subclass shared Direct2D resources so they can be recreated on next refresh calls.
        MyStandardButtonSubclass::releaseSharedDeviceResources();
        MyImageButtonSubclass::releaseSharedDeviceResources();
        MyRadioButtonSubclass::releaseSharedDeviceResources();
        MyEditboxSubclass::releaseSharedDeviceResources();
        MyDDLComboboxSubclass::releaseSharedDeviceResources();
        MyVerticalScrollbarSubclass::releaseSharedDeviceResources();

        // Create application window Direct2D resources.
        if (!this->pUIManager->createDeviceResources())
        {
            error_message = "Failed to create application window device-dependent resources.";
            break;
        }

        // Refresh the non-client windows.
        if (!this->vNonClientWindows.empty())
        {
            bool is_for_loop_failed = false;
            for (auto &p_window : this->vNonClientWindows)
            {
                if (!p_window->refresh())
                {
                    is_for_loop_failed = true;
                    break;
                }
            }
            if (is_for_loop_failed)
            {
                error_message = "Failed to refresh the non-client windows.";
                break;
            }
        }

        // Refresh the containers.
        if (!this->updateContainers(this->windowTheme))
        {
            error_message = "Failed to update containers.";
            break;
        }

        // Make the application window active, update its border and redraw the window.
        this->isWindowActive = true;
        if (this->isWindows11BorderAttributeSupported)
        {
            COLORREF border_color = this->pUIManager->colors.borderActive.getCOLORREF();
            HRESULT hr = DwmSetWindowAttribute(this->hWnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
            if (FAILED(hr))
            {
                error_message = "Failed to set the window border attribute.";
                break;
            }
        }
        RedrawWindow(this->hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
        SetActiveWindow(this->hWnd);

        this->logger.writeLog("Application theme selected:", ("'" + theme_name + "'").c_str(), MyLogType::Info);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        this->logger.writeLog(error_message, "[NAMESPACE: 'nApp::Theme' | FUNC: 'SetAppTheme()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::updateContainers(MyTheme theme)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->mContainers.empty())
        {
            bool is_for_loop_failed = false;
            for (auto &[id, p_container] : this->mContainers)
            {
                // Perform custom actions on specific container ...
                switch (id)
                {
                case IDC_DC_CONTAINER:
                {
                    if (!p_container->updateContainerDimensions(WINDOW_BORDER_DEFAULTWIDTH,
                                                                this->pUIManager->miscs.rectCaption.bottom,
                                                                this->windowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2),
                                                                this->windowHeight - (WINDOW_BORDER_DEFAULTWIDTH * 2) - (this->pUIManager->miscs.rectCaption.bottom - this->pUIManager->miscs.rectCaption.top),
                                                                this->isAlwaysHideScrollbars, false))
                    {
                        error_message = "Failed to update the container dimensions.";
                        is_for_loop_failed = true;
                        break;
                    }

                    break;
                }
                default:
                    break;
                }
                if (is_for_loop_failed)
                    break;

                if (!p_container->refresh())
                {
                    error_message = "Failed to refresh the container.";
                    is_for_loop_failed = true;
                    break;
                }
            }
            if (is_for_loop_failed)
                break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        this->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'updateContainers()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::addContainer(std::shared_ptr<MyContainer> pContainerWindow)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!pContainerWindow || !pContainerWindow->container())
        {
            error_message = "The window pointer is invalid. (null or empty window)";
            break;
        }

        auto insert_result = this->mContainers.insert(std::make_pair(GetDlgCtrlID(pContainerWindow->container()->hWnd()), pContainerWindow));
        if (!insert_result.second)
        {
            error_message = "A container with the same ID already exists in the application. All windows IDs should be unique.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'addContainer()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::removeContainer(INT containerWindowID)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        auto it = this->mContainers.find(containerWindowID);
        if (it != this->mContainers.end())
        {
            this->mContainers.erase(it);
            are_all_operation_success = true;
            break;
        }

        error_message = "Target container not found.";
        break;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'removeContainer()']", MyLogType::Error);

    return are_all_operation_success;
}
std::shared_ptr<MyContainer> MyApp::findContainer(INT containerWindowID)
{
    for (const auto &[id, p_container] : this->mContainers)
    {
        if (containerWindowID == GetDlgCtrlID(p_container->container()->hWnd()))
            return p_container;
    }

    return nullptr;
}
bool MyApp::onCreate(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Extend the window frames into the client area. (Enabling the window drop-shadow effect)
        MARGINS borders = {1, 1, 1, 1};
        HRESULT hr = DwmExtendFrameIntoClientArea(hWnd, &borders);
        if (FAILED(hr))
        {
            error_message = "Failed to extend the window frames into the client area.";
            break;
        }

        // By default, all the UI resources are null.
        // Update the application parameters and create UI resources for the first time.
        {
            if (!this->pUIManager->updateUITheme(this->windowTheme))
            {
                error_message = "Failed to update the UI resources.";
                break;
            }

            bool is_switch_success = false;
            std::string theme_name = "";
            switch (this->windowTheme)
            {
            case MyTheme::Light:
            {
                theme_name = "Light";
                is_switch_success = true;
                break;
            }
            case MyTheme::Dark:
            {
                theme_name = "Dark";
                is_switch_success = true;
                break;
            }
            case MyTheme::Monokai:
            {
                theme_name = "Monokai";
                is_switch_success = true;
                break;
            }
            default:
                error_message = "Unknown theme.";
                break;
            }
            if (!is_switch_success)
                break;
            this->logger.writeLog("Default application theme:", ("'" + theme_name + "'").c_str(), MyLogType::Info);
        }

        // Create non-client window controls.
        {
            // Window title.
            std::wstring text_title = MyUtility::GetWindowTextWideString(hWnd);
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 0;
            subclass_config.posX = WINDOW_BORDER_DEFAULTWIDTH + 10;
            subclass_config.posY = WINDOW_BORDER_DEFAULTWIDTH + 6;
            subclass_config.fontScale = 0.55f;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textActive;
            MyStandardTextWindowConfig window_config(hWnd, 0, 0, 330, 37,
                                                     (HMENU)IDC_NONCLIENT_CAPTIONTITLE_STATIC, &subclass_config, true, text_title);
            std::shared_ptr<MyWindow> p_window_title(new MyWindow());
            if (!p_window_title->createStandardText(window_config))
            {
                error_message = "Failed to create the window title.";
                break;
            }
            this->vNonClientWindows.push_back(p_window_title);
            auto p_subclass_window_title = static_cast<MyStandardTextSubclass *>(p_window_title->data());
            p_subclass_window_title->config.pBackground = &g_pApp->pUIManager->colors.captionBackground;

            // Minimize button.
            MyImageButtonSubclassConfig button_minimize_subclass_config(this->pUIManager->images.pWicBitmapMinimizeImageDefault,
                                                                        this->pUIManager->images.pWicBitmapMinimizeImageHover,
                                                                        this->pUIManager->images.pWicBitmapMinimizeImageActive,
                                                                        &this->pUIManager->colors.captionBackground,
                                                                        &this->pUIManager->colors.minimizeButtonBackgroundOnHover,
                                                                        &this->pUIManager->colors.minimizeButtonBackgroundOnActive,
                                                                        0, 0, 20, 20);
            button_minimize_subclass_config.alwaysDrawDefaultImage = false;
            button_minimize_subclass_config.centering = true;
            MyImageButtonWindowConfig button_minimize_window_config(hWnd, 0, 0, 58, 37, (HMENU)IDC_NONCLIENT_MINIMIZE_BUTTON, &button_minimize_subclass_config, false, true, L"");
            std::shared_ptr<MyWindow> p_button_minimize(new MyWindow());
            if (!p_button_minimize->createImageButton(button_minimize_window_config))
            {
                error_message = "Failed to create the minimize button.";
                break;
            }
            this->vNonClientWindows.push_back(p_button_minimize);
            this->pUIManager->miscs.pMinimizeButton = &*p_button_minimize;

            // Maximize button.
            MyImageButtonSubclassConfig button_maximize_subclass_config(this->pUIManager->images.pWicBitmapMaximizeImageDefault,
                                                                        this->pUIManager->images.pWicBitmapMaximizeImageHover,
                                                                        this->pUIManager->images.pWicBitmapMaximizeImageActive,
                                                                        &this->pUIManager->colors.captionBackground,
                                                                        &this->pUIManager->colors.maximizeButtonBackgroundOnHover,
                                                                        &this->pUIManager->colors.maximizeButtonBackgroundOnActive,
                                                                        0, 0, 20, 20);
            button_maximize_subclass_config.alwaysDrawDefaultImage = false;
            button_maximize_subclass_config.centering = true;
            MyImageButtonWindowConfig button_maximize_window_config(hWnd, 0, 0, 58, 37, (HMENU)IDC_NONCLIENT_MAXIMIZE_BUTTON, &button_maximize_subclass_config, false, true, L"");
            std::shared_ptr<MyWindow> p_button_maximize(new MyWindow());
            if (!p_button_maximize->createImageButton(button_maximize_window_config))
            {
                error_message = "Failed to create the maximize button.";
                break;
            }
            this->vNonClientWindows.push_back(p_button_maximize);
            this->pUIManager->miscs.pMaximizeButton = &*p_button_maximize;

            // Close button.
            MyImageButtonSubclassConfig button_close_subclass_config(this->pUIManager->images.pWicBitmapCloseImageDefault,
                                                                     this->pUIManager->images.pWicBitmapCloseImageActive,
                                                                     this->pUIManager->images.pWicBitmapCloseImageHover,
                                                                     &this->pUIManager->colors.captionBackground,
                                                                     &this->pUIManager->colors.closeButtonBackgroundOnHover,
                                                                     &this->pUIManager->colors.closeButtonBackgroundOnActive,
                                                                     0, 0, 20, 20);
            button_close_subclass_config.alwaysDrawDefaultImage = false;
            button_close_subclass_config.centering = true;
            MyImageButtonWindowConfig button_close_window_config(hWnd, 0, 0, 58, 37, (HMENU)IDC_NONCLIENT_CLOSE_BUTTON, &button_close_subclass_config, false, true, L"");
            std::shared_ptr<MyWindow> p_button_close(new MyWindow());
            if (!p_button_close->createImageButton(button_close_window_config))
            {
                error_message = "Failed to create the close button.";
                break;
            }
            this->vNonClientWindows.push_back(p_button_close);
            this->pUIManager->miscs.pCloseButton = &*p_button_close;
        }

        // Create client window controls.
        // Container window.
        error_message = "Failed to create and add the container window to application window.";
        MyContainerWindowConfig container_config(hWnd,
                                                 WINDOW_BORDER_DEFAULTWIDTH, g_pApp->pUIManager->miscs.rectCaption.bottom,
                                                 g_pApp->windowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2) - WINDOW_SCROLLBAR_DEFAULTWIDTH,
                                                 g_pApp->windowHeight - (g_pApp->pUIManager->miscs.rectCaption.bottom - g_pApp->pUIManager->miscs.rectCaption.top) - (WINDOW_BORDER_DEFAULTWIDTH * 2),
                                                 (HMENU)IDC_DC_CONTAINER, false, DefaultContainerProcedure);
        std::shared_ptr<MyContainer> p_container(new MyContainer());
        if (!p_container->createContainerWindow(container_config))
            break;
        if (!g_pApp->addContainer(p_container))
            break;
        error_message = "";

        // Scrollbar window.
        {
            error_message = "Failed to create the container scrollbar window.";
            int container_x = 0, container_y = 0, container_width = 0, container_height = 0;
            MyUtility::GetWindowRelativePosition(p_container->container()->hWnd(), container_x, container_y);
            MyUtility::GetWindowDimensions(p_container->container()->hWnd(), container_width, container_height);
            if (!p_container->createVerticalScrollbarWindow(container_x + container_width, container_y,
                                                            WINDOW_SCROLLBAR_DEFAULTWIDTH, container_height,
                                                            (HMENU)IDC_DC_SCROLLBAR, false))
                break;
            if (!p_container->setScrollBottomPadding(10))
                break;
            error_message = "";
        }

        error_message = "Failed to create standard button sample windows.";
        // Header text: 'STANDARD BUTTONS'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10, 10, 300, 20, (HMENU)IDC_DC_HEADING1, &subclass_config, true, L"STANDARD BUTTONS");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        // Standard button windows.
        {
            MyStandardButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                       10, 40,
                                                       130, 40, (HMENU)IDC_DC_STANDARDBUTTON1, true, true, L"Standard 1");
            std::shared_ptr<MyWindow> p_button_window(new MyWindow());
            if (!p_button_window.get()->createStandardButton(window_config))
                break;
            if (!p_container->addWindow(p_button_window))
                break;
        }
        {
            MyStandardButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                       150, 40,
                                                       130, 40, (HMENU)IDC_DC_STANDARDBUTTON2, true, true, L"Standard 2");
            std::shared_ptr<MyWindow> p_button_window(new MyWindow());
            if (!p_button_window.get()->createStandardButton(window_config))
                break;
            if (!p_container->addWindow(p_button_window))
                break;
        }
        {
            MyStandardButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                       290, 40,
                                                       130, 40, (HMENU)IDC_DC_STANDARDBUTTON3, true, true, L"Standard 3");
            std::shared_ptr<MyWindow> p_button_window(new MyWindow());
            if (!p_button_window.get()->createStandardButton(window_config))
                break;
            if (!p_container->addWindow(p_button_window))
                break;
        }
        error_message = "";

        error_message = "Failed to create radio button sample windows.";
        // Header text: 'RADIO BUTTONS'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10, 90, 300, 20, (HMENU)IDC_DC_HEADING2, &subclass_config, true, L"RADIO BUTTONS");

            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        // Radio button windows.
        {
            MyRadioButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                    10, 120,
                                                    130, 40, (HMENU)IDC_DC_RADIOBUTTON1, true, true, L"Radio 1");
            std::shared_ptr<MyWindow> p_radio_button_window(new MyWindow());
            if (!p_radio_button_window.get()->createRadioButton(window_config))
                break;
            if (!p_container->addWindow(p_radio_button_window))
                break;
            if (!g_SampleRadioGroup.addRadioButton(p_radio_button_window.get()->hWnd()))
                break;
        }
        {
            MyRadioButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                    150, 120,
                                                    130, 40, (HMENU)IDC_DC_RADIOBUTTON2, true, true, L"Radio 2");
            std::shared_ptr<MyWindow> p_radio_button_window(new MyWindow());
            if (!p_radio_button_window.get()->createRadioButton(window_config))
                break;
            if (!p_container->addWindow(p_radio_button_window))
                break;
            if (!g_SampleRadioGroup.addRadioButton(p_radio_button_window.get()->hWnd()))
                break;
        }
        {
            MyRadioButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                    290, 120,
                                                    130, 40, (HMENU)IDC_DC_RADIOBUTTON3, true, true, L"Radio 3");
            std::shared_ptr<MyWindow> p_radio_button_window(new MyWindow());
            if (!p_radio_button_window.get()->createRadioButton(window_config))
                break;
            if (!p_container->addWindow(p_radio_button_window))
                break;
            if (!g_SampleRadioGroup.addRadioButton(p_radio_button_window.get()->hWnd()))
                break;
        }
        error_message = "";

        error_message = "Failed to create image button sample windows.";
        // Header text: 'IMAGE BUTTONS'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10, 170, 300, 20, (HMENU)IDC_DC_HEADING3, &subclass_config, true, L"IMAGE BUTTONS");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        // Image button windows.
        {
            MyImageButtonSubclassConfig subclass_config(&g_pApp->pUIManager->images.wicBitmapAEDefault,
                                                        &g_pApp->pUIManager->images.wicBitmapAEHover,
                                                        &g_pApp->pUIManager->images.wicBitmapAEActive,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        0, 0, 90, 90);
            MyImageButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                    10, 200,
                                                    90, 90, (HMENU)IDC_DC_IMAGEBUTTON1, &subclass_config, true, true, L"");
            std::shared_ptr<MyWindow> p_image_button_window(new MyWindow());
            if (!p_image_button_window.get()->createImageButton(window_config))
                break;
            if (!p_container->addWindow(p_image_button_window))
                break;
        }
        {
            MyImageButtonSubclassConfig subclass_config(&g_pApp->pUIManager->images.wicBitmapPSDefault,
                                                        &g_pApp->pUIManager->images.wicBitmapPSHover,
                                                        &g_pApp->pUIManager->images.wicBitmapPSActive,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        0, 0, 90, 90);
            MyImageButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                    10 + 100, 200,
                                                    90, 90, (HMENU)IDC_DC_IMAGEBUTTON2, &subclass_config, true, true, L"");
            std::shared_ptr<MyWindow> p_image_button_window(new MyWindow());
            if (!p_image_button_window.get()->createImageButton(window_config))
                break;
            if (!p_container->addWindow(p_image_button_window))
                break;
        }
        {
            MyImageButtonSubclassConfig subclass_config(&g_pApp->pUIManager->images.wicBitmapAIDefault,
                                                        &g_pApp->pUIManager->images.wicBitmapAIHover,
                                                        &g_pApp->pUIManager->images.wicBitmapAIActive,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        0, 0, 90, 90);
            MyImageButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                    10 + 100 + 100, 200,
                                                    90, 90, (HMENU)IDC_DC_IMAGEBUTTON3, &subclass_config, true, true, L"");
            std::shared_ptr<MyWindow> p_image_button_window(new MyWindow());
            if (!p_image_button_window.get()->createImageButton(window_config))
                break;
            if (!p_container->addWindow(p_image_button_window))
                break;
        }
        {
            MyImageButtonSubclassConfig subclass_config(&g_pApp->pUIManager->images.wicBitmapMEDefault,
                                                        &g_pApp->pUIManager->images.wicBitmapMEHover,
                                                        &g_pApp->pUIManager->images.wicBitmapMEActive,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        &g_pApp->pUIManager->colors.background,
                                                        0, 0, 90, 90);
            MyImageButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                    10 + 100 + 100 + 100, 200,
                                                    90, 90, (HMENU)IDC_DC_IMAGEBUTTON4, &subclass_config, true, true, L"");
            std::shared_ptr<MyWindow> p_image_button_window(new MyWindow());
            if (!p_image_button_window.get()->createImageButton(window_config))
                break;
            if (!p_container->addWindow(p_image_button_window))
                break;
            if (!MyUtility::RemoveWindowClassStyle(p_image_button_window.get()->hWnd(), CS_DBLCLKS))
                break;
        }
        error_message = "";

        error_message = "Failed to create editbox sample windows.";
        // Header text: 'EDITBOXES'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10, 300, 300, 20, (HMENU)IDC_DC_HEADING4, &subclass_config, true, L"EDITBOXES");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        // Editbox windows.
        {
            MyEditboxSubclassConfig subclass_config(reinterpret_cast<HMENU>(IDC_DC_EDITBOXNORMALBORDER));
            MyEditboxWindowConfig window_config(p_container->container()->hWnd(),
                                                10, 330,
                                                390, 30, (HMENU)IDC_DC_EDITBOXNORMAL, MyEditboxType::Singleline, &subclass_config, true, true, L"");
            std::shared_ptr<MyWindow> p_editbox_text_window(new MyWindow());
            if (!p_editbox_text_window.get()->createEditbox(window_config))
                break;
            if (!p_container->addWindow(p_editbox_text_window))
                break;
        }
        {
            MyEditboxSubclassConfig subclass_config(reinterpret_cast<HMENU>(IDC_DC_EDITBOXPASSWORDBORDER));
            MyEditboxWindowConfig window_config(p_container->container()->hWnd(),
                                                10, 365,
                                                390, 30, (HMENU)IDC_DC_EDITBOXPASSWORD, MyEditboxType::SinglelinePassword, &subclass_config, true, true, L"");
            std::shared_ptr<MyWindow> p_editbox_password_window(new MyWindow());
            if (!p_editbox_password_window.get()->createEditbox(window_config))
                break;
            if (!p_container->addWindow(p_editbox_password_window))
                break;
        }
        {
            MyEditboxSubclassConfig subclass_config(reinterpret_cast<HMENU>(IDC_DC_EDITBOXMULTILINEBORDER));
            MyEditboxWindowConfig window_config(p_container->container()->hWnd(),
                                                10, 400,
                                                390, 300, (HMENU)IDC_DC_EDITBOXMULTILINE, MyEditboxType::Multiline, &subclass_config, true, true, L"");
            std::shared_ptr<MyWindow> p_editbox_multiline_window(new MyWindow());
            if (!p_editbox_multiline_window.get()->createEditbox(window_config))
                break;
            if (!p_container->addWindow(p_editbox_multiline_window))
                break;
        }
        // Note text: '(Normal)'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_LIGHT;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10 + 390 + 10, 330 + 6, 200, 17, (HMENU)IDC_DC_EDITBOXNORMALNOTE, &subclass_config, true, L"(Normal)");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        // Note text: '(Password)'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_LIGHT;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10 + 390 + 10, 365 + 6, 200, 17, (HMENU)IDC_DC_EDITBOXPASSWORDNOTE, &subclass_config, true, L"(Password)");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        // Note text: '(Multiline)'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_LIGHT;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10 + 390 + 10, 400 + 6, 200, 17, (HMENU)IDC_DC_EDITBOXMULTILINENOTE, &subclass_config, true, L"(Multiline)");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        error_message = "";

        error_message = "Failed to create combobox sample windows.";
        // Header text: 'COMBOBOXES'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10, 710, 300, 20, (HMENU)IDC_DC_HEADING5, &subclass_config, true, L"COMBOBOXES");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }
        // Combobox windows.
        {
            MyDDLComboboxWindowConfig window_config(p_container->container()->hWnd(),
                                                    10, 740,
                                                    130, 40, (HMENU)IDC_DC_COMBOBOXEMPTY, true, true);
            std::shared_ptr<MyWindow> p_ddlcombobox_window(new MyWindow());
            if (!p_ddlcombobox_window.get()->createDDLCombobox(window_config))
                break;
            if (!p_container->addWindow(p_ddlcombobox_window))
                break;
        }
        {
            MyDDLComboboxWindowConfig window_config(p_container->container()->hWnd(),
                                                    150, 740,
                                                    130, 40, (HMENU)IDC_DC_COMBOBOXNORMAL, true, true);
            std::shared_ptr<MyWindow> p_ddlcombobox_window(new MyWindow());
            if (!p_ddlcombobox_window.get()->createDDLCombobox(window_config))
                break;
            if (!p_container->addWindow(p_ddlcombobox_window))
                break;
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Option 1");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Option 2");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Option 3");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Option 4");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Option 5");
            ComboBox_SetCurSel(p_ddlcombobox_window.get()->hWnd(), 2);
        }
        error_message = "";

        // Header text: 'Testing'
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     10, 790, 300, 20, (HMENU)IDC_DC_HEADING6, &subclass_config, true, L"TESTING");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }

        // Open log file button
        {
            MyStandardButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                       10, 820,
                                                       140, 40, (HMENU)IDC_DC_OPENLOGFILE, true, true, L"Open log file");
            std::shared_ptr<MyWindow> p_button_window(new MyWindow());
            if (!p_button_window.get()->createStandardButton(window_config))
                break;
            if (!p_container->addWindow(p_button_window))
                break;
        }
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     160, 820 + 10, 300, 19, (HMENU)IDC_DC_OPENLOGFILENOTE, &subclass_config, true, L"OPEN LOG FILE");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }

        // Select file button
        {
            MyStandardButtonWindowConfig window_config(p_container->container()->hWnd(),
                                                       10, 870,
                                                       140, 40, (HMENU)IDC_DC_SELECTFILE, true, true, L"Select file(s)");
            std::shared_ptr<MyWindow> p_button_window(new MyWindow());
            if (!p_button_window.get()->createStandardButton(window_config))
                break;
            if (!p_container->addWindow(p_button_window))
                break;
        }
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     160, 870 + 10, 300, 19, (HMENU)IDC_DC_SELECTFILENOTE, &subclass_config, true, L"SELECT FILE");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }

        // Select theme combobox
        {
            error_message = "Failed to create the select theme combobox.";
            MyDDLComboboxWindowConfig window_config(p_container->container()->hWnd(),
                                                    10, 920,
                                                    140, 40, (HMENU)IDC_DC_COMBOBOXSELECTTHEME, true, true);
            std::shared_ptr<MyWindow> p_ddlcombobox_window(new MyWindow());
            if (!p_ddlcombobox_window.get()->createDDLCombobox(window_config))
                break;
            if (!p_container->addWindow(p_ddlcombobox_window))
                break;
            int current_theme_combobox_index = 0;
            switch (g_pApp->windowTheme)
            {
            case MyTheme::Light:
                current_theme_combobox_index = 0;
                break;
            case MyTheme::Dark:
                current_theme_combobox_index = 1;
                break;
            case MyTheme::Monokai:
                current_theme_combobox_index = 2;
                break;
            }
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Light");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Dark");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Monokai");
            ComboBox_SetCurSel(p_ddlcombobox_window.get()->hWnd(), current_theme_combobox_index);
            error_message = "";
        }
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     160, 920 + 10, 300, 19, (HMENU)IDC_DC_COMBOBOXSELECTTHEMENOTE, &subclass_config, true, L"SELECT THEME (F9)");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }

        // Select font combobox
        {
            error_message = "Failed to create the select font combobox.";
            MyDDLComboboxWindowConfig window_config(p_container->container()->hWnd(),
                                                    10, 970,
                                                    150, 40, (HMENU)IDC_DC_COMBOBOXSELECTFONT, true, true);
            std::shared_ptr<MyWindow> p_ddlcombobox_window(new MyWindow());
            if (!p_ddlcombobox_window.get()->createDDLCombobox(window_config))
                break;
            if (!p_container->addWindow(p_ddlcombobox_window))
                break;
            int current_font_combobox_index = 0;
            if (g_pApp->pUIManager->fonts.defaultFamily == L"Bahnschrift")
                current_font_combobox_index = 0;
            else if (g_pApp->pUIManager->fonts.defaultFamily == L"Ubuntu")
                current_font_combobox_index = 1;
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Bahnschrift");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Ubuntu");
            ComboBox_SetCurSel(p_ddlcombobox_window.get()->hWnd(), current_font_combobox_index);
            error_message = "";
        }
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     170, 970 + 10, 300, 19, (HMENU)IDC_DC_COMBOBOXSELECTFONTNOTE, &subclass_config, true, L"SELECT FONT");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }

        // Select scroll mode.
        {
            error_message = "Failed to create the select scroll mode combobox.";
            MyDDLComboboxWindowConfig window_config(p_container->container()->hWnd(),
                                                    10, 1020,
                                                    150, 40, (HMENU)IDC_DC_COMBOBOXSELECTSCROLLMODE, true, true);
            std::shared_ptr<MyWindow> p_ddlcombobox_window(new MyWindow());
            if (!p_ddlcombobox_window.get()->createDDLCombobox(window_config))
                break;
            if (!p_container->addWindow(p_ddlcombobox_window))
                break;
            int index = (p_container->enableSmoothScroll ? 1 : 0);
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Instant");
            ComboBox_AddString(p_ddlcombobox_window.get()->hWnd(), L"Smooth");
            ComboBox_SetCurSel(p_ddlcombobox_window.get()->hWnd(), index);
            error_message = "";
        }
        {
            MyStandardTextSubclassConfig subclass_config;
            subclass_config.centerMode = 2;
            subclass_config.fontWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
            subclass_config.pTextColor = &g_pApp->pUIManager->colors.textInactive;
            MyStandardTextWindowConfig window_config(p_container->container()->hWnd(),
                                                     170, 1020 + 10, 300, 19, (HMENU)IDC_DC_COMBOBOXSELECTSCROLLMODENOTE, &subclass_config, true, L"SELECT SCROLL MODE");
            std::shared_ptr<MyWindow> p_window(new MyWindow());
            if (!p_window->createStandardText(window_config))
                break;
            if (!p_container->addWindow(p_window))
                break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'onCreate()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::onReady()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Create the application window render target and its device-dependent resources.
        if (!this->pUIManager->createDeviceResources())
        {
            error_message = "Failed to create the main window render target and its device-dependent resources.";
            break;
        }

        // Show the application window.
        ShowWindow(this->hWnd, SW_NORMAL);

        // Show the non-client windows.
        for (const auto &p_window : this->vNonClientWindows)
            ShowWindow(p_window->hWnd(), SW_NORMAL);

        if (!IsWindowVisible(this->hWnd))
        {
            error_message = "Failed to show the application window.";
            break;
        }

        // Show the client windows.
        ShowWindow(g_pApp->findContainer(IDC_DC_CONTAINER)->container()->hWnd(), SW_SHOW);

        // Indicate that the application is ready.
        this->isWindowReady = true;
        this->logger.writeLog("Application is ready.", "", MyLogType::Info);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyApp' | FUNC: 'onReady()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::onExit()
{
    this->logger.writeLog("Exiting the application ...", "", MyLogType::Debug);

    // Reverse the extended window frames. (Disabling the window drop-shadow effect)
    MARGINS borders = {0, 0, 0, 0};
    HRESULT hr = DwmExtendFrameIntoClientArea(this->hWnd, &borders);
    if (FAILED(hr))
    {
        this->logger.writeLog("Failed to reverse the extended window frames.", "[CLASS: 'MyApp' | FUNC: 'onExit()']", MyLogType::Error);
        return false;
    }

    // <Stop all the in-progress application activities here if needed.>

    return true;
}
bool MyApp::onDestroy()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Destroy non-client windows.
        {
            if (!this->vNonClientWindows.empty())
                this->vNonClientWindows.clear();
            this->logger.writeLog("Non-client window(s) destroyed.", "", MyLogType::Debug);
        }

        // Destroy client windows.
        {
            if (!this->mContainers.empty())
                this->mContainers.clear();
            this->logger.writeLog("Client window(s) destroyed.", "", MyLogType::Debug);
        }

        if (MyContainer::totalInstances || MyWindow::totalInstances || MySubclass::totalInstances)
        {
            this->logger.writeLog("All windows and their associated objects should be destroyed first.", "[CLASS: 'MyApp' | FUNC: 'onDestroy()']", MyLogType::Warn);
            this->logger.writeLog("'MyContainer' instances: " + std::to_string(MyContainer::totalInstances), "[CLASS: 'MyApp' | FUNC: 'onDestroy()']", MyLogType::Warn);
            this->logger.writeLog("'MyWindow' instances: " + std::to_string(MyWindow::totalInstances), "[CLASS: 'MyApp' | FUNC: 'onDestroy()']", MyLogType::Warn);
            this->logger.writeLog("'MySubclass' instances: " + std::to_string(MySubclass::totalInstances), "[CLASS: 'MyApp' | FUNC: 'onDestroy()']", MyLogType::Warn);
            error_message = "Failed to destroy the windows and their resources.";
            break;
        }
        if (this->logger.getLogLevel() > 1)
        {
            this->logger.writeLog("'MyContainer' instances: " + std::to_string(MyContainer::totalInstances), "", MyLogType::Debug);
            this->logger.writeLog("'MyWindow' instances: " + std::to_string(MyWindow::totalInstances), "", MyLogType::Debug);
            this->logger.writeLog("'MySubclass' instances: " + std::to_string(MySubclass::totalInstances), "", MyLogType::Debug);
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        this->logger.writeLog(error_message, "[NAMESPACE: 'nApp::Window::Uninitialization' | FUNC: 'OnDestroy()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyApp::isWindowPlacementMaximize(HWND hWnd)
{
    WINDOWPLACEMENT placement;
    placement.length = sizeof(WINDOWPLACEMENT);

    if (!GetWindowPlacement(hWnd, &placement))
    {
        this->logger.writeLog("Failed to retrieve the window placement struct.", "[CLASS: 'MyApp' | FUNC: 'isWindowPlacementMaximize()']", MyLogType::Error);
        return false;
    }

    return placement.showCmd == SW_MAXIMIZE;
}
void MyApp::adjustMaximizedClientRect(HWND window, RECT &rect)
{
    if (!this->isWindowPlacementMaximize(window))
        return;

    // To be observed behavior:
    // The MonitorFromWindow() function failed when the window is resotred from minimized state to maximized state.
    // Set the flag to MONITOR_DEFAULTTONEAREST instead of MONITOR_DEFAULTTONULL fixed the issue.
    auto monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
    if (!monitor)
        return;

    MONITORINFO monitor_info{};
    monitor_info.cbSize = sizeof(monitor_info);
    if (!GetMonitorInfoW(monitor, &monitor_info))
    {
        this->logger.writeLog("Failed to retrieve the monitor info.", "[CLASS: 'MyApp' | FUNC: 'adjustMaximizedClientRect()']", MyLogType::Error);
        return;
    }

    rect = monitor_info.rcWork;
}