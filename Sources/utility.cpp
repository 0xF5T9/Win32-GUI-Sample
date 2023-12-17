/**
 * @file utility.cpp
 * @brief Implementation of the user-defined types defined in utility.h.
 */

#include "../Headers/standard_includes.h" // Include standard libraries.
#include "../Headers/my_includes.h"       // Include my user-defined type headers.

/*******************************************
 * Global helper functions implementations *
 *******************************************/

bool MyUtility::ShowWindowInfo(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        const UINT MAX_CLASS_NAME_LENGTH = 255;
        RECT rect_window = {0, 0, 0, 0};
        POINT point = {0, 0};
        HWND parent = GetParent(hWnd);

        // Get the window text.
        SetLastError(ERROR_SUCCESS);
        size_t text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
        if (!text_length && GetLastError())
        {
            error_message = L"Failed to retrieve the window text length.";
            break;
        }
        std::unique_ptr<WCHAR[]> p_text_buffer(new WCHAR[text_length + 1]);
        if (!GetWindowTextW(hWnd, p_text_buffer.get(), static_cast<INT>(text_length) + 1) && text_length)
        {
            error_message = L"Failed to retrieve the window text.";
            break;
        }
        std::wstring window_text(p_text_buffer.get());

        // Get the window class name.
        p_text_buffer.reset(new WCHAR[MAX_CLASS_NAME_LENGTH]);
        if (!GetClassNameW(hWnd, p_text_buffer.get(), MAX_CLASS_NAME_LENGTH))
        {
            error_message = L"Failed to retrieve the window class name.";
            break;
        }
        std::wstring window_class_name(p_text_buffer.get());

        // Get the window ID.
        INT window_id = GetDlgCtrlID(hWnd);

        // Get the window parent ID.
        INT parent_id = GetDlgCtrlID(parent);

        // Get the window relative position.
        SetLastError(69);
        MapWindowPoints(hWnd, parent, &point, 1);
        if (GetLastError() != 69)
        {
            error_message = L"Failed to map window points.";
            break;
        }
        DWORD window_style = GetWindowLongW(hWnd, GWL_STYLE);
        if (!window_style)
        {
            error_message = L"Failed to retrieve the window style.";
            break;
        }
        if (window_style && window_style & WS_BORDER) // Adjust Y position for windows with a border.
            point.y -= 1;                             // Compensate for top border.
        INT window_pos_x = point.x, window_pos_y = point.y;

        // Get the window client area dimensions.
        INT client_width = 0, client_height = 0;
        if (!GetClientRect(hWnd, &rect_window))
        {
            error_message = L"Failed to retrieve the window client rect.";
            break;
        }
        client_width = rect_window.right - rect_window.left;
        client_height = rect_window.bottom - rect_window.top;

        // Get the window non-client area dimensions.
        INT nonclient_width = 0, nonclient_height = 0;
        if (!GetWindowRect(hWnd, &rect_window))
        {
            error_message = L"Failed to retrieve the window non-client rect.";
            break;
        }
        nonclient_width = rect_window.right - rect_window.left;
        nonclient_height = rect_window.bottom - rect_window.top;

        // Output the informations to the message box.
        std::wstring message = L"";
        message.append(L"Window text: " + window_text + L"\n");
        message.append(L"Window class name: " + window_class_name + L"\n");
        message.append(L"Window id: " + std::to_wstring(window_id) + L"\n");
        message.append(L"Window parent id: " + std::to_wstring(parent_id) + L"\n");
        message.append(L"Relative positions (X, Y): " + std::to_wstring(window_pos_x) + L", " + std::to_wstring(window_pos_y) + L"\n");
        message.append(L"Client area dimensions (WxH): " + std::to_wstring(client_width) + L"x" + std::to_wstring(client_height) + L"\n");
        message.append(L"Non-client area dimensions (WxH): " + std::to_wstring(nonclient_width) + L"x" + std::to_wstring(nonclient_height) + L"\n");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONINFORMATION);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

    return are_all_operation_success;
}
bool MyUtility::ShowLastWinAPIErrorMessage()
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Get the error code.
        DWORD error_code = GetLastError();

        // Allocate the text buffer for the error message.
        WCHAR text_buffer[256];

        // Retrieve the error message of the last error code.
        if (!FormatMessageW(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                error_code,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                text_buffer,
                sizeof(text_buffer) / sizeof(wchar_t),
                NULL))
        {
            error_message = L"Failed to retrieve the error message of the last error code.";
            break;
        }

        // Show the error message via message box.
        MessageBoxW(NULL, text_buffer, L"", MB_OK | MB_ICONINFORMATION);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

    return are_all_operation_success;
}
bool MyUtility::GetDesktopResolution(INT &horizontal, INT &vertical)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Get the desktop window handle.
        const HWND window_desktop = GetDesktopWindow();
        if (!IsWindow(window_desktop))
        {
            error_message = L"Failed to retrieve the desktop window handle.";
            break;
        }

        // Get the desktop window dimension.
        RECT rect_desktop;
        if (!GetWindowRect(window_desktop, &rect_desktop))
        {
            error_message = L"Failed to retrieve the desktop window dimensions.";
            break;
        }
        horizontal = rect_desktop.right - rect_desktop.left;
        vertical = rect_desktop.bottom - rect_desktop.top;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

    return are_all_operation_success;
}
bool MyUtility::GetUserPreferredPowerPlanName(std::wstring &preferredPowerPlanName)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Get power active scheme.
        GUID *active_scheme_guid;
        if (PowerGetActiveScheme(NULL, &active_scheme_guid) != ERROR_SUCCESS)
        {
            error_message = L"Failed to retrieve the power active scheme.";
            break;
        }

        {
            // Get the friendly name of the power scheme.
            DWORD text_buffer_size = 256;
            std::unique_ptr<UCHAR[]> p_text_buffer(new UCHAR[text_buffer_size]);
            if (PowerReadFriendlyName(NULL, active_scheme_guid, NULL, NULL, p_text_buffer.get(), &text_buffer_size))
            {
                error_message = L"Failed to retrieve the friendly name of the power scheme.";
                break;
            }

            // Convert the byte buffer to a wide string.
            preferredPowerPlanName.reserve(text_buffer_size / 2);
            for (int i = 0; i < (int)text_buffer_size; i += 2)
            {
                wchar_t *ptr = reinterpret_cast<wchar_t *>(&p_text_buffer[i]);
                preferredPowerPlanName.push_back(*ptr);
            }
        }

        // Free the memory allocated by PowerGetActiveScheme().
        if (LocalFree(active_scheme_guid) != NULL)
        {
            error_message = L"Failed to free the memory allocated by PowerGetActiveScheme() function.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

    return are_all_operation_success;
}
std::string MyUtility::GetWindowClassNameString(const HWND &hWnd)
{
    const UINT MAX_CLASS_NAME_LENGTH = 255;

    // Allocate the text buffer for the window class name.
    std::unique_ptr<CHAR[]> p_text_buffer(new CHAR[MAX_CLASS_NAME_LENGTH]);

    // Get the window class name.
    if (!GetClassNameA(hWnd, p_text_buffer.get(), MAX_CLASS_NAME_LENGTH))
        return "";

    // Convert the text buffer to a string.
    std::string class_name(p_text_buffer.get());

    return class_name;
}
std::wstring MyUtility::GetWindowClassNameWideString(const HWND &hWnd)
{
    const UINT MAX_CLASS_NAME_LENGTH = 255;

    // Allocate the text buffer for the window class name.
    std::unique_ptr<WCHAR[]> p_text_buffer(new WCHAR[MAX_CLASS_NAME_LENGTH]);

    // Get the window class name.
    if (!GetClassNameW(hWnd, p_text_buffer.get(), MAX_CLASS_NAME_LENGTH))
        return L"";

    // Convert the text buffer to a wide string.
    std::wstring class_name(p_text_buffer.get());

    return class_name;
}
std::string MyUtility::GetWindowTextString(const HWND &hWnd)
{
    // Get the length of the window text.
    SetLastError(ERROR_SUCCESS);
    size_t text_length = static_cast<size_t>(GetWindowTextLengthA(hWnd));
    if (!text_length && GetLastError())
        return "";

    // Allocate the text buffer for the window text.
    std::unique_ptr<CHAR[]> p_text_buffer(new CHAR[text_length + 1]); // +1 for the null terminator.
    if (!GetWindowTextA(hWnd, p_text_buffer.get(), static_cast<INT>(text_length) + 1) && text_length)
        return "";

    // Convert the text buffer to a string.
    std::string window_text(p_text_buffer.get());

    return window_text;
}
std::wstring MyUtility::GetWindowTextWideString(const HWND &hWnd)
{
    // Get the length of the window text.
    SetLastError(ERROR_SUCCESS);
    size_t text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
    if (!text_length && GetLastError())
        return L"";

    // Allocate the text buffer for the window text.
    std::unique_ptr<WCHAR[]> p_text_buffer(new WCHAR[text_length + 1]); // +1 for the null terminator.
    if (!GetWindowTextW(hWnd, p_text_buffer.get(), static_cast<INT>(text_length) + 1) && text_length)
        return L"";

    // Convert the text buffer to a wide string.
    std::wstring window_text(p_text_buffer.get());

    return window_text;
}
bool MyUtility::GetWindowRelativePosition(HWND hWnd, INT &windowPosXRef, INT &windowPosYRef)
{
    // Get the handle to the parent window.
    HWND parent = GetParent(hWnd);

    // Initialize the POINT structure for storing the relative positions.
    POINT point = {0, 0};

    // Set a custom error code for checking the success of MapWindowPoints.
    SetLastError(69);

    // Map the window coordinates from hWnd to Parent.
    MapWindowPoints(hWnd, parent, &point, 1);

    // Check if MapWindowPoints failed by comparing the error code.
    if (GetLastError() != 69)
        return false;
    else
    {
        // Get the window style of hWnd.
        DWORD window_style = GetWindowLongW(hWnd, GWL_STYLE);

        // Check if GetWindowLongW failed by comparing the return value.
        if (!window_style)
            return false;
        else
        {
            // Adjust the y-position if the window has the WS_BORDER style.
            if (window_style & WS_BORDER)
            {
                point.y -= 1;
            }
        }

        // Store the relative positions in the output parameters.
        windowPosXRef = point.x;
        windowPosYRef = point.y;
    }

    return true;
}
bool MyUtility::GetWindowDimensions(HWND hWnd, INT &windowWidthRef, INT &windowHeightRef)
{
    RECT rect_window{0};
    if (!GetClientRect(hWnd, &rect_window))
        return false;

    windowWidthRef = rect_window.right - rect_window.left;
    windowHeightRef = rect_window.bottom - rect_window.top;

    return true;
}
bool MyUtility::GetScrollInfo(HWND hWnd, SCROLLINFO &scrollInfo)
{
    if (!IsWindow(hWnd))
        return false;

    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask = SIF_ALL;
    if (!SendMessageW(hWnd, SBM_GETSCROLLINFO, 0, reinterpret_cast<LPARAM>(&scrollInfo)))
        return false;

    return true;
}
bool MyUtility::RemoveWindowStyle(HWND hWnd, LONG style)
{
    DWORD window_style = GetWindowLong(hWnd, GWL_STYLE);
    if (!window_style)
        return false;

    window_style &= ~style;
    if (!SetWindowLong(hWnd, GWL_STYLE, window_style))
        return false;

    return true;
}
bool MyUtility::RemoveWindowExStyle(HWND hWnd, LONG style)
{
    DWORD window_style = GetWindowLong(hWnd, GWL_EXSTYLE);
    if (!window_style)
        return false;

    window_style &= ~style;
    if (!SetWindowLong(hWnd, GWL_EXSTYLE, window_style))
        return false;

    return true;
}
bool MyUtility::RemoveWindowClassStyle(HWND hWnd, LONG style)
{
    DWORD window_style = GetClassLong(hWnd, GCL_STYLE);
    if (!window_style)
        return false;

    window_style &= ~style;
    if (!SetClassLong(hWnd, GCL_STYLE, window_style))
        return false;

    return true;
}
bool MyUtility::ContainsWord(const std::string &str, const std::string &word, bool caseSensitive, bool checkBoundaries)
{
    if (checkBoundaries)
    {
        // Check if the word is found in the string.
        size_t found = str.find(word);
        if (found != std::string::npos)
        {
            // Check if the word is surrounded by word boundaries.
            if ((found == 0 || !isalpha(str[found - 1])) &&
                (found + word.length() == str.length() || !isalpha(str[found + word.length()])))
            {
                return true;
            }
        }

        if (!caseSensitive)
        {
            // Convert the search string to lowercase.
            std::string str_lowercase = "";
            for (auto ch : str)
            {
                auto ch_lower = std::tolower(ch);
                str_lowercase += ch_lower;
            }
            // Convert the search word to lowercase.
            std::string word_lowercase = "";
            for (auto ch : word)
            {
                auto ch_lower = std::tolower(ch);
                word_lowercase += ch_lower;
            }
            // Check if lowercase word is found in the lowercase string.
            found = str_lowercase.find(word_lowercase);
            if (found != std::string::npos)
            {
                // Check if the lowercase word is surrounded by word boundaries.
                if ((found == 0 || !isalpha(str_lowercase[found - 1])) &&
                    (found + word_lowercase.length() == str_lowercase.length() || !isalpha(str_lowercase[found + word_lowercase.length()])))
                {
                    return true;
                }
            }
        }

        return false;
    }
    else
    {
        // Check if the word is found in the string.
        if (str.find(word) != std::string::npos)
            return true;

        if (!caseSensitive)
        {
            // Convert the search string to lowercase.
            std::string str_lowercase = "";
            for (auto ch : str)
            {
                auto ch_lower = std::tolower(ch);
                str_lowercase += ch_lower;
            }
            // Convert the search word to lowercase.
            std::string word_lowercase = "";
            for (auto ch : word)
            {
                auto ch_lower = std::tolower(ch);
                word_lowercase += ch_lower;
            }
            // Check if lowercase word is found in the lowercase string.
            if (str_lowercase.find(word_lowercase) != std::string::npos)
                return true;
        }

        return false;
    }
}
bool MyUtility::ContainsWord(const std::wstring &str, const std::wstring &word, bool caseSensitive, bool checkBoundaries)
{
    if (checkBoundaries)
    {
        // Check if the word is found in the string.
        size_t found = str.find(word);
        if (found != std::wstring::npos)
        {
            // Check if the word is surrounded by word boundaries.
            if ((found == 0 || !iswalpha(str[found - 1])) &&
                (found + word.length() == str.length() || !iswalpha(str[found + word.length()])))
            {
                return true;
            }
        }

        if (!caseSensitive)
        {
            // Convert the search string to lowercase.
            std::wstring str_lowercase = L"";
            for (auto ch : str)
            {
                auto ch_lower = std::towlower(ch);
                str_lowercase += ch_lower;
            }
            // Convert the search word to lowercase.
            std::wstring word_lowercase = L"";
            for (auto ch : word)
            {
                auto ch_lower = std::towlower(ch);
                word_lowercase += ch_lower;
            }
            // Check if lowercase word is found in the lowercase string.
            found = str_lowercase.find(word_lowercase);
            if (found != std::wstring::npos)
            {
                // Check if the lowercase word is surrounded by word boundaries.
                if ((found == 0 || !iswalpha(str_lowercase[found - 1])) &&
                    (found + word_lowercase.length() == str_lowercase.length() || !iswalpha(str_lowercase[found + word_lowercase.length()])))
                {
                    return true;
                }
            }
        }

        return false;
    }
    else
    {
        // Check if the word is found in the string.
        if (str.find(word) != std::wstring::npos)
            return true;

        if (!caseSensitive)
        {
            // Convert the search string to lowercase.
            std::wstring str_lowercase = L"";
            for (auto ch : str)
            {
                auto ch_lower = std::towlower(ch);
                str_lowercase += ch_lower;
            }
            // Convert the search word to lowercase.
            std::wstring word_lowercase = L"";
            for (auto ch : word)
            {
                auto ch_lower = std::towlower(ch);
                word_lowercase += ch_lower;
            }
            // Check if lowercase word is found in the lowercase string.
            if (str_lowercase.find(word_lowercase) != std::wstring::npos)
                return true;
        }

        return false;
    }
}
bool MyUtility::ContainsDigit(const std::string &str)
{
    for (const auto &ch : str)
    {
        if (isdigit(ch))
            return true;
    }

    return false;
}
bool MyUtility::ContainsDigit(const std::wstring &str)
{
    for (const auto &ch : str)
    {
        if (iswdigit(ch))
            return true;
    }

    return false;
}
std::wstring MyUtility::ConvertStringToWideString(const std::string &str)
{
    int text_length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::unique_ptr<wchar_t[]> p_text_buffer(new wchar_t[text_length]);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, p_text_buffer.get(), text_length);
    std::wstring wide_string(p_text_buffer.get());

    return wide_string;
}
std::string MyUtility::ConvertWideStringToString(const std::wstring &wstr)
{
    int text_length = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> p_text_buffer(new char[text_length]);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, p_text_buffer.get(), text_length, NULL, NULL);
    std::string string(p_text_buffer.get());
    return string;
}
std::string MyUtility::RemoveNonDigitFromString(const std::string &str)
{
    std::string ret = std::regex_replace(str, std::regex("\\D"), "");

    return ret;
}
std::wstring MyUtility::RemoveNonDigitFromString(const std::wstring &str)
{
    std::wstring ret = std::regex_replace(str, std::wregex(L"\\D"), L"");

    return ret;
}
bool MyUtility::OpenFileDialog(std::vector<std::wstring> &paths, bool selectFolder, bool multiSelect)
{
    IFileOpenDialog *p_file_open = nullptr;
    bool are_all_operation_success = false;
    while (!are_all_operation_success)
    {
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                                      IID_IFileOpenDialog, reinterpret_cast<void **>(&p_file_open));
        if (FAILED(hr))
            break;

        if (selectFolder || multiSelect)
        {
            FILEOPENDIALOGOPTIONS options = 0;
            hr = p_file_open->GetOptions(&options);
            if (FAILED(hr))
                break;

            if (selectFolder)
                options |= FOS_PICKFOLDERS;
            if (multiSelect)
                options |= FOS_ALLOWMULTISELECT;

            hr = p_file_open->SetOptions(options);
            if (FAILED(hr))
                break;
        }

        hr = p_file_open->Show(NULL);
        if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) // No items were selected.
        {
            are_all_operation_success = true;
            break;
        }
        else if (FAILED(hr))
            break;

        IShellItemArray *p_items;
        hr = p_file_open->GetResults(&p_items);
        if (FAILED(hr))
            break;
        DWORD total_items = 0;
        hr = p_items->GetCount(&total_items);
        if (FAILED(hr))
            break;

        for (int i = 0; i < static_cast<int>(total_items); ++i)
        {
            IShellItem *p_item;
            p_items->GetItemAt(i, &p_item);
            if (SUCCEEDED(hr))
            {
                PWSTR path;
                hr = p_item->GetDisplayName(SIGDN_FILESYSPATH, &path);
                if (SUCCEEDED(hr))
                {
                    paths.push_back(path);
                    CoTaskMemFree(path);
                }
                p_item->Release();
            }
        }

        p_items->Release();
        are_all_operation_success = true;
    }

    if (p_file_open)
        p_file_open->Release();
    return are_all_operation_success;
}
bool MyUtility::SaveFileDialog(std::wstring &path, std::wstring defaultFileName, std::pair<COMDLG_FILTERSPEC *, int> *pFilterInfo)
{
    IFileSaveDialog *p_file_save = nullptr;
    bool are_all_operation_success = false;
    while (!are_all_operation_success)
    {
        HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                                      IID_IFileSaveDialog, reinterpret_cast<void **>(&p_file_save));
        if (FAILED(hr))
            break;

        if (!pFilterInfo)
        {
            COMDLG_FILTERSPEC save_filter[1];
            save_filter[0].pszName = L"All files";
            save_filter[0].pszSpec = L"*.*";
            hr = p_file_save->SetFileTypes(1, save_filter);
            if (FAILED(hr))
                break;
            hr = p_file_save->SetFileTypeIndex(1);
            if (FAILED(hr))
                break;
        }
        else
        {
            hr = p_file_save->SetFileTypes(pFilterInfo->second, pFilterInfo->first);
            if (FAILED(hr))
                break;
            hr = p_file_save->SetFileTypeIndex(1);
            if (FAILED(hr))
                break;
        }

        if (!defaultFileName.empty())
        {
            hr = p_file_save->SetFileName(defaultFileName.c_str());
            if (FAILED(hr))
                break;
        }

        hr = p_file_save->Show(NULL);
        if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) // No item was selected.
        {
            are_all_operation_success = true;
            break;
        }
        else if (FAILED(hr))
            break;

        IShellItem *p_item;
        hr = p_file_save->GetResult(&p_item);
        if (FAILED(hr))
            break;

        PWSTR item_path;
        hr = p_item->GetDisplayName(SIGDN_FILESYSPATH, &item_path);
        if (FAILED(hr))
            break;
        path = item_path;
        CoTaskMemFree(item_path);
        p_item->Release();

        are_all_operation_success = true;
    }

    if (p_file_save)
        p_file_save->Release();
    return are_all_operation_success;
}
bool CALLBACK MyUtility::Callback::SetFontOnChild(HWND hWndChild, LPARAM hFont)
{
    if (!reinterpret_cast<HFONT>(hFont))
        return false;

    SendMessageW(hWndChild, WM_SETFONT, hFont, TRUE);

    return true;
}

/***********************************
 * Utility classes implementations *
 ***********************************/

// [KeyToggle] class implementations:

KeyToggle::KeyToggle(int virtualKey) : virtualKey(virtualKey), isActive(false) {}
KeyToggle::operator bool()
{
    if (GetAsyncKeyState(this->virtualKey))
    {
        if (!this->isActive)
        {
            this->isActive = true;
            return true;
        }
    }
    else
        this->isActive = false;

    return false;
}

// [MyLogger] class implementations:

MyLogger::MyLogger()
{
    std::wstring command_line = GetCommandLineW();
    if (command_line.size() <= 2)
        throw std::runtime_error("Unexpected command line string.");

    command_line.erase(0, 1);
    command_line.pop_back();
    this->path = command_line;
}
MyLogger::MyLogger(std::wstring path)
    : path(path)
{
    if (path.size() <= 2)
        throw std::runtime_error("Invalid path string.");
}
int MyLogger::getLogLevel()
{
    return this->logLevel;
}
bool MyLogger::setPath(std::wstring path)
{
    if (path.empty())
    {
        std::wstring command_line = GetCommandLineW();
        if (command_line.size() <= 2)
            throw std::runtime_error("Unexpected command line string.");

        command_line.erase(0, 1);
        command_line.pop_back();
        this->path = command_line;
    }
    else
    {
        if (path.size() <= 2)
            return false;

        this->path = path;
    }

    return true;
}
bool MyLogger::setLogLevel(int logLevel)
{
    this->logLevel = logLevel;
    return true;
}
bool MyLogger::setLogFileName(std::wstring logFileName)
{
    if (logFileName.empty())
        return false;

    this->logFileName = logFileName;

    return true;
}
bool MyLogger::writeLog(std::string description, std::string extraDescription, MyLogType logType)
{
    {
        int log_level = 0;
        switch (logType)
        {
        case MyLogType::Info:
        case MyLogType::Warn:
        case MyLogType::Error:
            log_level = 1;
            break;
        case MyLogType::Debug:
            log_level = 2;
        }

        if (this->logLevel < log_level)
            return true;
    }

    std::ofstream file;
    file.open((this->path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + this->logFileName.c_str()).c_str(), std::ios_base::app);
    if (!file.is_open())
        return false;

    std::string log_type = "";
    switch (logType)
    {
    case MyLogType::Info:
        log_type.append("(INFO)");
        break;
    case MyLogType::Warn:
        log_type.append("(WARN)");
        break;
    case MyLogType::Error:
        log_type.append("(ERROR)");
        break;
    case MyLogType::Debug:
        log_type.append("(DEBUG)");
        break;
    }

    file << "[" << this->getCurrentDateTimeString() << "] " << log_type << " " << description << (!extraDescription.empty() ? (" " + extraDescription) : "") << "\n";

    file.close();
    return true;
}
bool MyLogger::writeLog(std::string description, bool includeTime)
{
    std::ofstream file;
    file.open((this->path.parent_path().c_str() + static_cast<std::wstring>(L"\\") + this->logFileName.c_str()).c_str(), std::ios_base::app);
    if (!file.is_open())
        return false;

    if (includeTime)
        file << "[" << this->getCurrentDateTimeString() << "] ";

    file << description << "\n";

    file.close();
    return true;
}
std::string MyLogger::getCurrentDateTimeString()
{
    time_t now = time(0);
    tm time_struct;
    CHAR text_buffer[80];
    if (localtime_s(&time_struct, &now))
        return "";

    strftime(text_buffer, 79, "%Y-%m-%d.%X", &time_struct);

    return text_buffer;
}

/***********************************
 * Deleter structs implementations *
 ***********************************/

// Deleter base struct:

Deleter::Deleter()
{
    this->logger.setLogLevel(2);
}

// WinAPI deleters:

void HWNDDeleter::operator()(HWND *variable) const
{
    if (variable)
    {
        if (*variable)
        {
            if (!DestroyWindow(*variable))
            {
                Deleter::logger.writeLog("Deleter failed.", "[DELETER: 'HWNDDeleter']", MyLogType::Error);
                throw std::runtime_error("'HWNDDeleter' deleter failed.");
            }

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'HWNDDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'HWNDDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void HBRUSHDeleter::operator()(HBRUSH *variable) const
{
    if (variable)
    {
        if (*variable)
        {
            if (!DeleteObject(*variable))
            {
                Deleter::logger.writeLog("Deleter failed.", "[DELETER: 'HBRUSHDeleter']", MyLogType::Error);
                throw std::runtime_error("'HBRUSHDeleter' deleter failed.");
            }

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'HBRUSHDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'HBRUSHDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void HFONTDeleter::operator()(HFONT *variable) const
{
    if (variable)
    {
        if (*variable)
        {
            if (!DeleteObject(*variable))
            {
                Deleter::logger.writeLog("Deleter failed.", "[DELETER: 'HFONTDeleter']", MyLogType::Error);
                throw std::runtime_error("'HFONTDeleter' deleter failed.");
            }

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'HFONTDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'HFONTDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void HRGNDeleter::operator()(HRGN *variable) const
{
    if (variable)
    {
        if (*variable)
        {
            if (!DeleteObject(*variable))
            {
                Deleter::logger.writeLog("Deleter failed.", "[DELETER: 'HRGNDeleter']", MyLogType::Error);
                throw std::runtime_error("'HRGNDeleter' deleter failed.");
            }

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'HRGNDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'HRGNDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}

// WAM deleters:

void IUIAnimationStoryboardDeleter::operator()(IUIAnimationStoryboard **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'IUIAnimationStoryboardDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'IUIAnimationStoryboardDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void IUIAnimationVariableDeleter::operator()(IUIAnimationVariable **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'IUIAnimationVariableDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'IUIAnimationVariableDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void IUIAnimationTransitionDeleter::operator()(IUIAnimationTransition **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'IUIAnimationTransitionDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'IUIAnimationTransitionDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}

// Direct2D/DirectWrite deleters:

void IWICBitmapSourceDeleter::operator()(IWICBitmapSource **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'IWICBitmapSourceDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'IWICBitmapSourceDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void IDWriteTextFormatDeleter::operator()(IDWriteTextFormat **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'IDWriteTextFormatDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'IDWriteTextFormatDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void ID2D1DCRenderTargetDeleter::operator()(ID2D1DCRenderTarget **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'ID2D1DCRenderTargetDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'ID2D1DCRenderTargetDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void ID2D1SolidColorBrushDeleter::operator()(ID2D1SolidColorBrush **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'ID2D1SolidColorBrushDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'ID2D1SolidColorBrushDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}
void ID2D1BitmapDeleter::operator()(ID2D1Bitmap **variable) const
{
    if (variable)
    {
        if (*variable)
        {
            (*variable)->Release();
            *variable = nullptr;

            if (debugMode)
                Deleter::logger.writeLog("Object released.", "[DELETER: 'ID2D1BitmapDeleter']", MyLogType::Info);
        }
        else
        {
            if (debugMode)
                Deleter::logger.writeLog("Object empty.", "[DELETER: 'ID2D1BitmapDeleter']", MyLogType::Info);
        }

        delete variable;
    }
}