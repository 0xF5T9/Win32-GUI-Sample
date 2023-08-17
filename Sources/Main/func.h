/**
 * @file func.h
 * @brief Include libraries and define global application functions.
 */

#ifndef FUNC_H
#define FUNC_H

#include "../../Includes/standard_includes.h"     // Standard libraries, project resources and global constant includes.
#include "../../Includes/Externals/Ex_DarkMode.h" // External: Reverse and access to undocumented window "Dark Mode" API.
#include "../Global/global.h"                     // The distribution header of the global variables, forward declarations and my class declarations.

/**
 * Uncomment to include libraries using compiler directive:
 * #pragma comment(lib, "UxTheme.lib")
 * #pragma comment(lib, "Comctl32.lib")
 * #pragma comment(lib, "Gdiplus.lib")
 * #pragma comment(lib, "Dwmapi.lib")
 * #pragma comment(lib, "Winmm.lib")
 * #pragma comment(lib, "Powrprof.lib")
 * #pragma comment(lib, "d2d1.lib")
 * #pragma comment(lib, "dwrite.lib")
 * #pragma comment(lib, "windowscodecs.lib")
 */

/**
 * @brief Utility functions and solutions for various scenarios.
 */
namespace nSol
{
    /**
     * @brief Show the last error message "GetLastError()"" via WinAPI message box.
     *
     * @return Returns true if the error message was successfully retrieved, otherwise false.
     *
     * @note Used for debugging purposes.
     */
    bool ShowLastErrorMessageBox()
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
            return false;

        // Show the error message via message box.
        MessageBoxW(NULL, text_buffer, L"", MB_OK);

        return true;
    }

    /**
     * @brief Display the window basic info via WINAPI message box.
     *
     * @param hWnd Handle to the target window (HWND).
     *
     * @return Returns true if all the window information was successfully retrieved, otherwise false.
     *
     * @note Used for debugging purposes.
     */
    bool DisplayWindowInfo(HWND hWnd)
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            RECT rect_window = {0, 0, 0, 0}; // RECT structure to calculate the window dimensions.
            POINT point = {0, 0};            // POINT structure for storing the relative positions.
            WCHAR *p_text_buffer;            // Text buffer for storing temporary strings.
            HWND parent = GetParent(hWnd);   // Parent of the target window.

            // Retrieving the window name.
            SetLastError(ERROR_SUCCESS);
            size_t text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
            if (!text_length && GetLastError())
            {
                error_message = L"Failed to retrieve the window text length.";
                break;
            }
            p_text_buffer = new WCHAR[text_length + 1];
            if (!GetWindowTextW(hWnd, p_text_buffer, static_cast<INT>(text_length) + 1) && text_length != NULL)
            {
                delete[] p_text_buffer;
                error_message = L"Failed to retrieve the window text.";
                break;
            }
            std::wstring window_text(p_text_buffer);
            delete[] p_text_buffer;

            // Retrieving the window class name.
            p_text_buffer = new WCHAR[MAX_CLASS_NAME_LENGTH];
            if (!GetClassNameW(hWnd, p_text_buffer, MAX_CLASS_NAME_LENGTH))
            {
                delete[] p_text_buffer;
                error_message = L"Failed to retrieve the window class name.";
                break;
            }
            std::wstring window_class_name(p_text_buffer);
            delete[] p_text_buffer;

            // Retrieving the window identifier.
            INT window_id = GetDlgCtrlID(hWnd);

            // Retrieving the window's Parent identifier.
            INT parent_id = GetDlgCtrlID(parent);

            // Retrieving the window relative positions.
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
            if (window_style != NULL && window_style & WS_BORDER)
                point.y -= 1;
            INT window_pos_x = point.x, window_pos_y = point.y;

            // Get the window dimensions (client rect).
            INT client_width = 0, client_height = 0;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = L"Failed to retrieve the window client rect.";
                break;
            }
            client_width = rect_window.right - rect_window.left;
            client_height = rect_window.bottom - rect_window.top;

            // Get the window dimensions (non-client rect).
            INT nonclient_width = 0, nonclient_height = 0;
            if (!GetWindowRect(hWnd, &rect_window))
            {
                error_message = L"Failed to retrieve the window non-client rect.";
                break;
            }
            nonclient_width = rect_window.right - rect_window.left;
            nonclient_height = rect_window.bottom - rect_window.top;

            // Output the information message.
            std::wstring message = L"";
            message.append(L"Window text: " + window_text + L"\n");
            message.append(L"Window class name: " + window_class_name + L"\n");
            message.append(L"Window id: " + std::to_wstring(window_id) + L"\n");
            message.append(L"Window Parent id: " + std::to_wstring(parent_id) + L"\n");
            message.append(L"Relative Positions (X, Y): " + std::to_wstring(window_pos_x) + L", " + std::to_wstring(window_pos_y) + L"\n");
            message.append(L"Size (client): " + std::to_wstring(client_width) + L"x" + std::to_wstring(client_height) + L"\n");
            message.append(L"Size (non-client): " + std::to_wstring(nonclient_width) + L"x" + std::to_wstring(nonclient_height) + L"\n");
            MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONINFORMATION);

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [NAMESPACE: \"nSol\" | FUNC: \"DisplayWindowInfo()\"]", MyLogType::Error);
            return false;
        }

        return true;
    }

    /**
     * @brief Retrieve the user desktop resolution.
     *
     * @param horizontal Reference to the variable that will hold the horizontal value.
     * @param vertical   Reference to the variable that will hold the vertical value.
     *
     * @return Returns true if successfully retrieved the desktop resolution, otherwise false.
     */
    bool GetDesktopResolution(INT &horizontal, INT &vertical)
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            const HWND window_desktop = GetDesktopWindow();
            if (!IsWindow(window_desktop))
            {
                error_message = L"Failed to retrieve the desktop window handle.";
                break;
            }

            RECT rect_desktop;
            if (!GetWindowRect(window_desktop, &rect_desktop))
            {
                error_message = L"Failed to retrieve the desktop window rect.";
                break;
            }

            horizontal = rect_desktop.right;
            vertical = rect_desktop.bottom;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [NAMESPACE: \"nSol\" | FUNC: \"GetDesktopResolution()\"]", MyLogType::Error);
            return false;
        }

        return true;
    }

    /**
     * @brief Retrieves the current date and time as a formatted string.
     *
     * Visit: http://en.cppreference.com/Width/cpp/chrono/c/strftime
     * for more information about date/time format.
     *
     * @return Returns the current date and time string formatted as "%Y-%m-%d.%X".
     */
    std::wstring GetCurrentDateTime()
    {
        // Get the current time.
        time_t now = time(0);

        // Create a structure to hold the time.
        tm time_struct;

        // Create a text buffer to store the formatted time string.
        WCHAR text_buffer[80];

        // Convert the current time to the structure.
        if (localtime_s(&time_struct, &now))
            WriteLog(L"Failed to convert the current time to the structure.", L" [NAMESPACE: \"nSol\" | FUNC: \"GetCurrentDateTime()\"]", MyLogType::Error);

        // Format the time struct into a string.
        wcsftime(text_buffer, 79, L"%Y-%m-%d.%X", &time_struct);

        // Return the formatted time string.
        return text_buffer;
    }

    /**
     * @brief Retrieve the name of the current user's preferred power plan.
     *
     * @param powerOptionPlanName Reference to the string variable that will hold the power plan name.
     *
     * @return Returns true if the current user's preferred power plan name was successfully retrieved.
     */
    bool GetUserPowerSettings(std::wstring &powerOptionPlanName)
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
                // Retrieve the friendly name of the power scheme.
                DWORD text_buffer_size = 256;
                UCHAR *text_buffer = new UCHAR[text_buffer_size];
                if (PowerReadFriendlyName(NULL, active_scheme_guid, NULL, NULL, text_buffer, &text_buffer_size))
                {
                    error_message = L"Failed to retrieve the friendly name of the power scheme.";
                    break;
                }

                // Convert the byte buffer to a wide string.
                powerOptionPlanName.reserve(text_buffer_size / 2);
                for (int i = 0; i < (int)text_buffer_size; i += 2)
                {
                    wchar_t *ptr = reinterpret_cast<wchar_t *>(&text_buffer[i]);
                    powerOptionPlanName.push_back(*ptr);
                }

                // Free the buffer memory.
                delete[] text_buffer;
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
        {
            WriteLog(error_message, L" [NAMESPACE: \"nSol\" | FUNC: \"GetUserPowerSettings()\"]", MyLogType::Error);
            return false;
        }

        return true;
    }

    /**
     * @brief Retrieve the class name of the target window.
     *
     * @param hWnd Handle to the target window (HWND).
     *
     * @return Returns the class name of the target window, or an empty string if failed to retrieve the class name.
     */
    std::wstring GetWindowClassName(const HWND &hWnd)
    {
        // Allocate the text buffer for the window class name.
        WCHAR *p_text_buffer = new WCHAR[MAX_CLASS_NAME_LENGTH];

        // Get the window class name.
        if (!GetClassNameW(hWnd, p_text_buffer, MAX_CLASS_NAME_LENGTH))
        {
            delete[] p_text_buffer; // Release text buffer on failure.
            WriteLog(L"Failed to retrieve the window class name.", L" [NAMESPACE: \"nSol\" | FUNC: \"GetWindowClassName()\"]", MyLogType::Error);
            return L"";
        }
        std::wstring class_name(p_text_buffer); // Convert the text buffer to a wide string.
        delete[] p_text_buffer;                 // Release text buffer.

        return class_name;
    }

    /**
     * @brief Retrieve the text of the target window.
     *
     * @param hWnd Handle to the target window (HWND).
     *
     * @return Returns the text of the target window, or an empty string if failed to retrieve the text.
     */
    std::wstring MGetWindowText(const HWND &hWnd)
    {
        // Retrieve the length of the text.
        SetLastError(ERROR_SUCCESS);
        size_t text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
        if (!text_length && GetLastError())
        {
            WriteLog(L"Failed to retrieve the window text length.", L" [NAMESPACE: \"nSol\" | FUNC: \"MGetWindowText()\"]", MyLogType::Error);
            return L"";
        }

        // Allocate the text buffer for the window text.
        WCHAR *p_text_buffer = new WCHAR[text_length + 1]; // +1 for the null terminator.
        if (!GetWindowTextW(hWnd, p_text_buffer, static_cast<INT>(text_length) + 1) && text_length != NULL)
        {
            delete[] p_text_buffer; // Release text buffer on failure.
            WriteLog(L"Failed to retrieve the window text.", L" [NAMESPACE: \"nSol\" | FUNC: \"MGetWindowText()\"]", MyLogType::Error);
            return L"";
        }
        std::wstring window_text(p_text_buffer); // Convert the text buffer to a wide string.
        delete[] p_text_buffer;                  // Release text buffer.

        return window_text;
    }

    /**
     * @brief Checks if a given word is contained within a string.
     *
     * @param str              The string to search within.
     * @param word             The word to find within the string.
     * @param case_sensitive   Specifies whether the search is case-sensitive or not.
     * @param check_boundaries Specifies whether to check word boundaries.
     *                         If true, the word must be surrounded by word boundaries (space, punctuation, etc.) to be considered a match.
     *                         If false, the function will search for partial matches as well.
     *
     * @return Returns true if the string contains the whole word, false otherwise.
     */
    bool ContainsWord(const std::wstring &str, const std::wstring &word, bool case_sensitive = false, bool check_boundaries = false)
    {
        if (check_boundaries)
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

            if (!case_sensitive)
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

            if (!case_sensitive)
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

    /**
     * @brief Check if a string contains at least one digit.
     *
     * @param str The string to search within.
     *
     * @return Returns true if the string contains at least one digit, false otherwise.
     */
    bool ContainsDigit(const std::wstring &str)
    {
        for (const auto &ch : str)
        {
            if (iswdigit(ch))
                return true;
        }

        return false;
    }

    /**
     * @brief Removes non-digit characters from a given string.
     *
     * @param str The input string from which non-digit characters are to be removed.
     *
     * @return The resulting string with non-digit characters removed.
     */
    std::wstring RemoveNonDigitFromString(const std::wstring &str)
    {
        std::wstring ret = std::regex_replace(str, std::wregex(L"\\D"), L"");

        return ret;
    }

    /**
     * @brief Converts a string(std::string) to a wide string(std::wstring).
     *
     * @param str The string to convert.
     *
     * @return The resulting wide string.
     */
    std::wstring ConvertStringToWideString(const std::string &str)
    {
        INT text_length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

        WCHAR *text_buffer = new WCHAR[text_length];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, text_buffer, text_length);
        std::wstring wide_string(text_buffer);
        delete[] text_buffer;

        return wide_string;
    }
}

/**
 * @brief Essential functions for the application.
 */
namespace nApp
{
    /**
     * @brief Functions related to file handling and operations.
     * @brief This namespace contains functions for working with files,
     * @brief such as writing logs, loading settings file, and updating settings file.
     */
    namespace File
    {
        /**
         * @brief Load application settings from a settings file.
         *
         * @param fileDirectory The directory where the settings file is located.
         * @param fileName      The name of the settings file.
         *
         * @return Returns true if the settings are successfully loaded from the settings file.
         */
        bool LoadSettingsFile(std::filesystem::path fileDirectory, std::wstring fileName)
        {
            bool generate_file_notice = true;            // Set this to false to disable file generation notice.
            USHORT total_generate_file_attempts = 0;     // File generation attempt counter.
            const USHORT MAX_GENERATE_FILE_ATTEMPTS = 3; // Maximum file generation attempts.

            while (true)
            {
                std::wifstream settings_file((fileDirectory.c_str() + static_cast<std::wstring>(L"\\") + fileName).c_str()); // Create file reading stream.

                // Check if the file already exists. If so, start reading the file.
                if (settings_file.is_open())
                {
                    // Set the locale before making any conversion to handle UTF-8 characters.
                    std::locale utf8("en_US.UTF-8");
                    settings_file.imbue(utf8);

                    // Read the file line by line.
                    std::wstring line;
                    while (std::getline(settings_file, line))
                    {
                        // line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end()); // Remove all whitespaces from the line.
                        if (line[0] == '#' || line.empty())
                            continue; // Ignore lines starting with '#' or empty lines.

                        auto delimiter_pos = line.find(L"=");
                        auto name = line.substr(0, delimiter_pos);
                        auto value = line.substr(delimiter_pos + 1);
                        if (value[0] == ' ')
                            value = line.substr(delimiter_pos + 2);

                        // Scan and read data from the file.
                        if (nSol::ContainsWord(name, L"Debug")) // Debug level.
                        {
                            // Perform additional check on the value string to avoid crashes from stoi() function.
                            std::wstring converted_value = nSol::RemoveNonDigitFromString(value);
                            if (!converted_value.empty() && nSol::ContainsDigit(converted_value))
                            {
                                INT int_value = std::stoi(converted_value);
                                g_AppLogLevel = int_value;
                            }
                            else
                                g_AppLogLevel = 1;
                        }
                        else if (nSol::ContainsWord(name, L"Theme")) // Theme.
                        {
                            if (nSol::ContainsWord(value, L"Light"))
                                g_CurrentAppTheme = MyTheme::Light;
                            else if (nSol::ContainsWord(value, L"Dark"))
                                g_CurrentAppTheme = MyTheme::Dark;
                            else if (nSol::ContainsWord(value, L"Monokai"))
                                g_CurrentAppTheme = MyTheme::Monokai;
                            else
                                g_CurrentAppTheme = MyTheme::Monokai;
                        }
                    }
                    settings_file.close(); // Close the file stream.
                    return true;
                }

                // If the file doesn't exist, generate the default settings file.
                else
                {
                    // Display the file generation notice.
                    if (generate_file_notice == true)
                    {
                        WriteLogEx(L"Default settings file not found, generating one ...", L"", MyLogType::Info, 1);
                        generate_file_notice = false;
                    }

                    // Create file writing stream.
                    std::wofstream new_settings_file((fileDirectory.c_str() + static_cast<std::wstring>(L"\\") + fileName).c_str());

                    // Set the locale before making any conversion to handle UTF-8 characters.
                    std::locale utf8("en_US.UTF-8");
                    new_settings_file.imbue(utf8);

                    // Write the default settings file.
                    new_settings_file << L"# Available themes: Light, Dark, Monokai.\n# Available settings: Debug\n\nTheme = Monokai\nDebug = 1";

                    // Close the file stream.
                    new_settings_file.close();

                    // Count the file generation attempts. Return false if it exceeds MAX_GENERATE_FILE_ATTEMPTS.
                    if (total_generate_file_attempts == MAX_GENERATE_FILE_ATTEMPTS)
                    {
                        WriteLog(L"Failed to perform write operations.", L" [NAMESPACE: \"nApp::File\" | FUNC: \"LoadSettingsFile()\"]", MyLogType::Error);
                        return false;
                    }
                    total_generate_file_attempts++;
                }
            }

            return false;
        }

        /**
         * @brief Update the value in the settings file.
         *
         * @param fileDirectory The directory where the settings file is located.
         * @param fileName      The name of the settings file.
         * @param settingName   The name of the setting to be changed (Available setting names: debug, theme).
         * @param settingValue  The new value for the setting.
         *
         * @return Returns true if successfully updated the value in the settings file.
         */
        bool UpdateSettingsFile(std::filesystem::path fileDirectory, std::wstring fileName, std::wstring settingName, std::wstring settingValue)
        {
            const USHORT MAX_WRITE_FILE_ATTEMPTS = 3;          // Maximum file write attempts.
            SHORT total_write_file_attempts = 0;               // File write attempt counter.
            std::wstring final_string = L"";                   // String that holds the final string to be written to the settings file.
            std::wstring current_app_debug_level_string = L""; // String that holds the current application debug level.
            std::wstring current_app_theme_string = L"";       // String that holds the current application theme.

            // Update the debug level in the settings file.
            if (nSol::ContainsWord(settingName, L"debug"))
            {
                while (true)
                {
                    // Create file writing stream.
                    std::wofstream settings_file((fileDirectory.c_str() + static_cast<std::wstring>(L"\\") + fileName).c_str());

                    // Open the file.
                    if (settings_file.is_open())
                    {
                        // Set the locale before making any conversion to handle UTF-8 characters.
                        std::locale utf8("en_US.UTF-8");
                        settings_file.imbue(utf8);

                        // Update the new setting values.
                        switch (g_CurrentAppTheme)
                        {
                        case MyTheme::Light:
                            current_app_theme_string = L"Light";
                            break;
                        case MyTheme::Dark:
                            current_app_theme_string = L"Dark";
                            break;
                        case MyTheme::Monokai:
                            current_app_theme_string = L"Monokai";
                            break;
                        }
                        current_app_debug_level_string += settingValue;

                        // Writing the settings file.
                        final_string += L"# Available themes: Light, Dark, Monokai.\n# Available settings: Debug\n\nTheme = ";
                        final_string += current_app_theme_string;
                        final_string += L"\nDebug = ";
                        final_string += current_app_debug_level_string;
                        settings_file << final_string;

                        // Close the file stream.
                        settings_file.close();
                        break;
                    }
                    // Count the file write attempts. Display an error message and exit the application if it exceeds MAX_WRITE_FILE_ATTEMPTS.
                    else
                    {
                        if (total_write_file_attempts == MAX_WRITE_FILE_ATTEMPTS)
                        {
                            WriteLog(L"Failed to perform write operations.", L" [NAMESPACE: \"nApp::File\" | FUNC: \"UpdateSettingsFile()\"]", MyLogType::Error);
                            return false;
                        }
                        total_write_file_attempts++;
                    }
                }

                return true;
            }
            // Update the theme in the settings file.
            else if (nSol::ContainsWord(settingName, L"theme"))
            {
                while (true)
                {
                    // Create file writing stream.
                    std::wofstream settings_file((fileDirectory.c_str() + static_cast<std::wstring>(L"\\") + fileName).c_str());

                    // Open the file.
                    if (settings_file.is_open())
                    {
                        // Set the locale before making any conversion to handle UTF-8 characters.
                        std::locale utf8("en_US.UTF-8");
                        settings_file.imbue(utf8);

                        // Update the new setting values.
                        current_app_theme_string += settingValue;
                        current_app_debug_level_string += std::to_wstring(g_AppLogLevel);

                        // Writing the settings file.
                        final_string += L"# Available themes: Light, Dark, Monokai.\n# Available settings: Debug\n\nTheme = ";
                        final_string += current_app_theme_string;
                        final_string += L"\nDebug = ";
                        final_string += current_app_debug_level_string;
                        settings_file << final_string;

                        // Close the file stream.
                        settings_file.close();
                        break;
                    }
                    // Count the file write attempts. Display an error message and exit the application if it exceeds MAX_WRITE_FILE_ATTEMPTS.
                    else
                    {
                        if (total_write_file_attempts == MAX_WRITE_FILE_ATTEMPTS)
                        {
                            WriteLog(L"Failed to perform write operations.", L" [NAMESPACE: \"nApp::File\" | FUNC: \"UpdateSettingsFile()\"]", MyLogType::Error);
                            return false;
                        }
                        total_write_file_attempts++;
                    }
                }

                return true;
            }
            else
            {
                WriteLog(L"Setting name not found.", L" [NAMESPACE: \"nApp::File\" | FUNC: \"UpdateSettingsFile()\"]", MyLogType::Error);
                return false;
            }
        }

        namespace Log
        {
            /**
             * @brief Write a log record to the log file.
             * Note:
             * - The global directory path variable (g_AppDirectoryPath) should be loaded first.
             *
             * @param description   The log description.
             * @param details       The log details.
             * @param logType       The log type.
             * @param noLogTypeText Specifies whether to exclude the log type text.
             *
             * @return Returns true if successfully write the log record, false otherwise.
             */
            bool WriteLog(std::wstring description, std::wstring details, MyLogType logType, bool noLogTypeText)
            {
                // Set the log level based on the log type.
                int log_level = 1;
                switch (logType)
                {
                case MyLogType::Info:
                    log_level = 1;
                    break;
                case MyLogType::Warn:
                    log_level = 1;
                    break;
                case MyLogType::Error:
                    log_level = 1;
                    break;
                case MyLogType::Debug:
                    log_level = 2;
                    break;
                }

                bool are_all_operation_success = false;
                std::wstring error_message = L"";
                while (!are_all_operation_success)
                {
                    // Check if the application directory path is valid.
                    if (g_AppDirectoryPath.empty())
                    {
                        error_message = L"The directory path is invalid (empty).";
                        break;
                    }

                    // Check if the file name is valid.
                    if (g_AppLogFileName.empty())
                    {
                        error_message = L"The file name is invalid (empty).";
                        break;
                    }

                    if (g_AppLogLevel >= log_level)
                    {
                        // Create file writing stream.
                        std::wofstream log_file;
                        log_file.open((g_AppDirectoryPath.c_str() + static_cast<std::wstring>(L"\\") + g_AppLogFileName).c_str(), std::ios_base::app);
                        if (!log_file.is_open())
                        {
                            error_message = L"Failed to create the file writing stream.";
                            break;
                        }

                        // Create log type string.
                        std::wstring log_type;
                        switch (logType)
                        {
                        case MyLogType::Info:
                            log_type = L"INFO";
                            break;
                        case MyLogType::Warn:
                            log_type = L"WARN";
                            break;
                        case MyLogType::Error:
                            log_type = L"ERROR";
                            break;
                        case MyLogType::Debug:
                            log_type = L"DEBUG";
                            break;
                        }

                        // Set the locale before making any conversion to handle UTF-8 characters.
                        std::locale utf8("en_US.UTF-8");
                        log_file.imbue(utf8);

                        // Write log to the file.
                        log_file << L"[" << nSol::GetCurrentDateTime() << L"]";
                        if (!noLogTypeText)
                            log_file << L" (" << log_type << L")";
                        log_file << L" " << description << details << std::endl;

                        // Close the file stream.
                        log_file.close();
                    }

                    are_all_operation_success = true;
                }

                if (!are_all_operation_success)
                {
                    std::wstring message = L"";
                    message.append(L"Error occurred!\n");
                    message.append(L"Failed to write the log record.\n");
                    message.append(L"Error Message: " + error_message);
                    MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

                    // Logging function is a essential function and should not fail. If it fails, throw an exception.
                    throw std::runtime_error("Write log function failed.");

                    return false;
                }

                return true;
            }

            /**
             * @brief Write a log record to the log file.
             * Note:
             * - The global directory path variable (g_AppDirectoryPath) should be loaded first.
             *
             * @param description   The log description.
             * @param details       The log details.
             * @param logType       The log type.
             * @param logLevel      The log level.
             * @param noLogTypeText Specifies whether to exclude the log type text.
             *
             * @return Returns true if successfully write the log record, false otherwise.
             */
            bool WriteLogEx(std::wstring description, std::wstring details, MyLogType logType, int logLevel, bool noLogTypeText)
            {
                bool are_all_operation_success = false;
                std::wstring error_message = L"";
                while (!are_all_operation_success)
                {
                    // Check if the application directory path is valid.
                    if (g_AppDirectoryPath.empty())
                    {
                        error_message = L"The directory path is invalid (empty).";
                        break;
                    }

                    // Check if the file name is valid.
                    if (g_AppLogFileName.empty())
                    {
                        error_message = L"The file name is invalid (empty).";
                        break;
                    }

                    if (g_AppLogLevel >= logLevel)
                    {
                        // Create file writing stream.
                        std::wofstream log_file;
                        log_file.open((g_AppDirectoryPath.c_str() + static_cast<std::wstring>(L"\\") + g_AppLogFileName).c_str(), std::ios_base::app);
                        if (!log_file.is_open())
                        {
                            error_message = L"Failed to create the file writing stream.";
                            break;
                        }

                        // Create log type string.
                        std::wstring log_type;
                        switch (logType)
                        {
                        case MyLogType::Info:
                            log_type = L"INFO";
                            break;
                        case MyLogType::Warn:
                            log_type = L"WARN";
                            break;
                        case MyLogType::Error:
                            log_type = L"ERROR";
                            break;
                        case MyLogType::Debug:
                            log_type = L"DEBUG";
                            break;
                        }

                        // Set the locale before making any conversion to handle UTF-8 characters.
                        std::locale utf8("en_US.UTF-8");
                        log_file.imbue(utf8);

                        // Write log to the file.
                        log_file << L"[" << nSol::GetCurrentDateTime() << L"]";
                        if (!noLogTypeText)
                            log_file << L" (" << log_type << L")";
                        log_file << L" " << description << details << std::endl;

                        // Close the file stream.
                        log_file.close();
                    }

                    are_all_operation_success = true;
                }

                if (!are_all_operation_success)
                {
                    std::wstring message = L"";
                    message.append(L"Error occurred!\n");
                    message.append(L"Failed to write the log record.\n");
                    message.append(L"Error Message: " + error_message);
                    MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

                    // Logging function is a essential function and should not fail. If it fails, throw an exception.
                    throw std::runtime_error("Write log function failed.");

                    return false;
                }

                return true;
            }
        }
    }

    /**
     * @brief Functions related to API initialization and management.
     * @brief This namespace contains functions that handle the initialization,
     * @brief uninitialization, and management of various APIs used by the application.
     */
    namespace API
    {
        /**
         * @brief Initialize undocumented dark mode API introduced in Windows 10 1809.
         * @brief Source: https://github.com/ysc3839/win32-darkmode
         *
         * @param hWnd Handle to the target window (HWND).
         *
         * @return Returns true if the dark mode API was successfully initialized, false otherwise.
         */
        bool InitDarkModeAPI(HWND hWnd)
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Bypass the window version compatibility check.
                // The API might or might not work!
                InitDarkMode(true);

                if (!g_darkModeSupported)
                {
                    error_message = L"The dark mode api is not supported on this window version.";
                    break;
                }

                _AllowDarkModeForWindow(hWnd, true);
                RefreshTitleBarThemeColor(hWnd);

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [NAMESPACE: \"nApp::API\" | FUNC: \"InitDarkModeAPI()\"]", MyLogType::Error);
                return false;
            }

            return true;
        }

        /**
         * @brief Initialize Windows Animation Manager API.
         *
         * @return Returns true if the Windows Animation Manager API was successfully initialized, false otherwise.
         */
        bool InitWindowsAnimationManager()
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                HRESULT hr;

                // Initialize the COM library.
                hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
                if (FAILED(hr))
                {
                    error_message = L"Failed to initialize the COM library.";
                    break;
                }

                // Initialize the animation mananger.
                hr = CoCreateInstance(CLSID_UIAnimationManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pAnimationManager));
                if (FAILED(hr))
                {
                    error_message = L"Failed to initialize the animation mananger.";
                    break;
                }

                // Initialize the animation timer.
                hr = CoCreateInstance(CLSID_UIAnimationTimer, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pAnimationTimer));
                if (FAILED(hr))
                {
                    error_message = L"Failed to initialize the animation timer.";
                    break;
                }

                // Initialize the standard animation transition library.
                hr = CoCreateInstance(CLSID_UIAnimationTransitionLibrary, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pTransitionLibrary));
                if (FAILED(hr))
                {
                    error_message = L"Failed to initialize the standard animation transition library.";
                    break;
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [NAMESPACE: \"nApp::API\" | FUNC: \"InitWindowsAnimationManager()\"]", MyLogType::Error);
                return false;
            }

            return true;
        }

        /**
         * @brief Uninitialize window animation manager API.
         *
         * @return Returns true if the window animation manager API was successfully uninitialized, false otherwise.
         */
        bool UninitWindowAnimationManager()
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                HRESULT hr;

                // Finish all animation storyboards.
                hr = g_pAnimationManager->FinishAllStoryboards(0);
                if (FAILED(hr))
                {
                    error_message = L"Failed to finish all animation storyboards.";
                    break;
                }

                // Release standard animation library COM object.
                g_pTransitionLibrary->Release();
                g_pTransitionLibrary = nullptr;

                // Release animation timer COM object.
                g_pAnimationTimer->Release();
                g_pAnimationTimer = nullptr;

                // Release animation mananger and it associated COM objects.
                hr = g_pAnimationManager->Shutdown();
                if (FAILED(hr))
                {
                    error_message = L"Failed to shutdown the animation mananger.";
                    break;
                }
                g_pAnimationManager = nullptr;

                // Uninitialize the COM library.
                CoUninitialize();

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [NAMESPACE: \"nApp::API\" | FUNC: \"UninitWindowAnimationManager()\"]", MyLogType::Error);
                return false;
            }

            return true;
        }

        /**
         * @brief Namespace containing collection of GDI+ drawing functions.
         */
        namespace GDIPDraw
        {
            /**
             * @brief Get the GDI+ path for a rounded rectangle.
             *
             * @param pPath        Pointer to the GraphicsPath object to store the path.
             * @param rect         The rectangle bounds of the rounded rectangle.
             * @param diameter     The diameter of the corners (radius of curvature).
             *
             * @return Returns true if successfully retrieved the Gdiplus::GraphicsPath, false otherwise.
             */
            bool MyDraw_GetRoundRectPath(Gdiplus::GraphicsPath *pPath, Gdiplus::Rect rect, INT diameter)
            {
                Gdiplus::Status result;

                // Diameter can't exceed width or height.
                if (diameter > rect.Width)
                    diameter = rect.Width;
                if (diameter > rect.Height)
                    diameter = rect.Height;

                // Define a corner .
                Gdiplus::Rect corner(rect.X, rect.Y, diameter, diameter);

                // Begin path.
                result = pPath->Reset();
                if (result)
                {
                    return false;
                }

                // Top left.
                result = pPath->AddArc(corner, 180, 90);
                if (result)
                {
                    return false;
                }

                // Tweak needed for radius of 10 (diameter of 20).
                if (diameter == 20)
                {
                    corner.Width += 1;
                    corner.Height += 1;
                    rect.Width -= 1;
                    rect.Height -= 1;
                }

                // Top right.
                corner.X += (rect.Width - diameter - 1);
                result = pPath->AddArc(corner, 270, 90);
                if (result)
                {
                    return false;
                }

                // Bottom right.
                corner.Y += (rect.Height - diameter - 1);
                result = pPath->AddArc(corner, 0, 90);
                if (result)
                {
                    return false;
                }

                // Bottom left.
                corner.X -= (rect.Width - diameter - 1);
                result = pPath->AddArc(corner, 90, 90);
                if (result)
                {
                    return false;
                }

                // End path.
                result = pPath->CloseFigure();
                if (result)
                {
                    return false;
                }

                return true;
            }

            /**
             * @brief Draw a filled rounded rectangle.
             *
             * @param pGraphics    Pointer to the Gdiplus::Graphics object for drawing.
             * @param rect         Gdiplus::Rect object representing the position and dimensions of the rectangle to be drawn.
             * @param radius       The rectangle's corner round radius.
             * @param width        The rectangle width.
             * @param pRectColor   Pointer to the Gdiplus::Color object for drawing the fill rectangle.
             *
             * @return Returns true if successfully performed the draw operations, false otherwise.
             */
            bool MyDraw_DrawRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect rect, INT radius, INT width, Gdiplus::Color pRectColor)
            {
                Gdiplus::Status result;

                // Store current graphics parameters.
                Gdiplus::Unit previous_page_unit = pGraphics->GetPageUnit();
                Gdiplus::SmoothingMode previous_smoothing_mode = pGraphics->GetSmoothingMode();

                // Set new graphics paramemters for drawing operations.
                result = pGraphics->SetPageUnit(Gdiplus::Unit::UnitPixel);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
                if (result)
                {
                    return false;
                }

                // Create draw path and necessary value(s) for the drawing operations.
                INT diameter = 2 * radius;
                Gdiplus::GraphicsPath path;
                if (!MyDraw_GetRoundRectPath(&path, rect, diameter))
                {
                    return false;
                }

                // Draw the round RECT_Parent.
                Gdiplus::Pen pen_color(pRectColor, 1);
                pen_color.SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);
                result = pGraphics->DrawPath(&pen_color, &path);
                if (result)
                {
                    return false;
                }

                // If width > 1.
                for (int i = 1; i < width; i++)
                {
                    // Left stroke.
                    rect.Inflate(-1, 0);

                    // Get the path.
                    if (!MyDraw_GetRoundRectPath(&path, rect, diameter))
                    {
                        return false;
                    }

                    // Draw the round RECT_Parent.
                    result = pGraphics->DrawPath(&pen_color, &path);
                    if (result)
                    {
                        return false;
                    }

                    // Up stroke.
                    rect.Inflate(0, -1);

                    // Get the path.
                    if (!MyDraw_GetRoundRectPath(&path, rect, diameter))
                    {
                        return false;
                    }

                    // Draw the round RECT_Parent.
                    result = pGraphics->DrawPath(&pen_color, &path);
                    if (result)
                    {
                        return false;
                    }
                }

                // Restore previous graphics parameters.
                result = pGraphics->SetSmoothingMode(previous_smoothing_mode);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetPageUnit(previous_page_unit);
                if (result)
                {
                    return false;
                }

                return true;
            }

            /**
             * @brief Draw a filled rectangle.
             *
             * @param pGraphics        Pointer to the Gdiplus::Graphics object for drawing.
             * @param rect             Gdiplus::Rect object representing the position and dimensions of the rectangle to be drawn.
             * @param pRectColor       Pointer to the Gdiplus::Color object for drawing the fill rectangle.
             * @param pRectBorderColor Pointer to the Gdiplus::Color object for drawing the rectangle border.
             *                         If not specified, the rectangle border will not be drawn.
             *
             * @return Returns true if successfully performed the draw operations, false otherwise.
             */
            bool MyDraw_FillRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect rect, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor)
            {
                Gdiplus::Status result;

                // Store current graphics parameters.
                Gdiplus::Unit previous_page_unit = pGraphics->GetPageUnit();
                Gdiplus::SmoothingMode previous_smoothing_mode = pGraphics->GetSmoothingMode();

                // Set new graphics paramemters for drawing operations.
                result = pGraphics->SetPageUnit(Gdiplus::Unit::UnitPixel);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);
                if (result)
                {
                    return false;
                }

                // Draw the fill rectangle.
                Gdiplus::SolidBrush solid_brush_color = *pRectColor;
                result = pGraphics->FillRectangle(&solid_brush_color, rect.GetLeft() - 1, rect.GetTop() - 1, rect.GetRight() + 1, rect.GetBottom() + 1);
                if (result)
                {
                    return false;
                }

                // Draw the rectangle border.
                if (pRectBorderColor)
                {
                    Gdiplus::Pen pen_color(*pRectBorderColor, 1);
                    pen_color.SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);
                    result = pGraphics->DrawRectangle(&pen_color, rect.GetLeft(), rect.GetTop(), rect.GetRight() - 1, rect.GetBottom() - 1);
                    if (result)
                    {
                        return false;
                    }
                }

                // Restore previous graphics parameters.
                result = pGraphics->SetSmoothingMode(previous_smoothing_mode);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetPageUnit(previous_page_unit);
                if (result)
                {
                    return false;
                }

                return true;
            }

            /**
             * @brief Draw a filled ellipse.
             *
             * @param pGraphics        Pointer to the Gdiplus::Graphics object for drawing.
             * @param rect             Gdiplus::Rect object representing the position and dimensions of the ellipse to be drawn.
             * @param diameter         Diameter of the ellipse.
             * @param posX             X position of the ellipse.
             * @param posY             Y position of the ellipse.
             * @param pRectColor       Pointer to the Gdiplus::Color object for drawing the fill ellipse.
             * @param pRectBorderColor Pointer to the Gdiplus::Color object for drawing the ellipse border.
             *                         If not specified, the ellipse border will not be drawn.
             *
             * @return Returns true if successfully performed the draw operations, false otherwise.
             */
            bool MyDraw_FillEllipse(Gdiplus::Graphics *pGraphics, Gdiplus::Rect rect, INT diameter, INT posX, INT posY, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor)
            {
                Gdiplus::Status result;

                // Adjust drawing positions and dimensions for the ellipse.
                rect.X += posX;
                rect.Y += posY;
                rect.Width = diameter - posX;
                rect.Height = diameter - posY;

                // Store current graphics parameters.
                Gdiplus::Unit previous_page_unit = pGraphics->GetPageUnit();
                Gdiplus::SmoothingMode previous_smoothing_mode = pGraphics->GetSmoothingMode();

                // Set new graphics paramemters for drawing operations.
                result = pGraphics->SetPageUnit(Gdiplus::Unit::UnitPixel);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
                if (result)
                {
                    return false;
                }

                // Draw the fill ellipse.
                Gdiplus::SolidBrush solid_brush_color = *pRectColor;
                result = pGraphics->FillEllipse(&solid_brush_color, rect.GetLeft() - 1, rect.GetTop() - 1, rect.GetRight() + 1, rect.GetBottom() + 1);
                if (result)
                {
                    return false;
                }

                // Draw the ellipse border.
                if (pRectBorderColor)
                {
                    Gdiplus::Pen pen_color(*pRectBorderColor, 1);
                    pen_color.SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);
                    result = pGraphics->DrawEllipse(&pen_color, rect.GetLeft() - 1, rect.GetTop() - 1, rect.GetRight() + 1, rect.GetBottom() + 1);
                    if (result)
                    {
                        return false;
                    }
                }

                // Restore previous graphics parameters.
                result = pGraphics->SetSmoothingMode(previous_smoothing_mode);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetPageUnit(previous_page_unit);
                if (result)
                {
                    return false;
                }

                return true;
            }

            /**
             * @brief Draw a filled rounded rectangle.
             *
             * @param pGraphics        Pointer to the Gdiplus::Graphics object for drawing.
             * @param rect             Gdiplus::Rect object representing the position and dimensions of the rectangle to be drawn.
             * @param radius           The rectangle's corner round radius.
             * @param pRectColor       Pointer to the Gdiplus::Color object for drawing the fill rectangle.
             * @param pRectBorderColor Pointer to the Gdiplus::Color object for drawing the rectangle border.
             *                         If not specified, the rectangle border will not be drawn.
             *
             * @return Returns true if successfully performed the draw operations, false otherwise.
             */
            bool MyDraw_FillRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect rect, INT radius, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor)
            {
                Gdiplus::Status result;

                // Store current graphics parameters.
                Gdiplus::Unit previous_page_unit = pGraphics->GetPageUnit();
                Gdiplus::SmoothingMode previous_smoothing_mode = pGraphics->GetSmoothingMode();

                // Set new graphics paramemters for drawing operations.
                result = pGraphics->SetPageUnit(Gdiplus::Unit::UnitPixel);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
                if (result)
                {
                    return false;
                }

                // Create draw path and necessary value(s) for the drawing operations.
                INT diameter = 2 * radius;
                Gdiplus::GraphicsPath path;
                if (!MyDraw_GetRoundRectPath(&path, rect, diameter))
                {
                    return false;
                }

                // Draw the fill rounded rectangle.
                Gdiplus::SolidBrush solid_brush_color = *pRectColor;
                result = pGraphics->FillPath(&solid_brush_color, &path);
                if (result)
                {
                    return false;
                }

                // Draw the rectangle border.
                if (pRectBorderColor)
                {
                    Gdiplus::Pen pen_color(*pRectBorderColor, 1);
                    pen_color.SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);
                    result = pGraphics->DrawPath(&pen_color, &path);
                    if (result)
                    {
                        return false;
                    }
                }

                // Restore previous graphics parameters.
                result = pGraphics->SetSmoothingMode(previous_smoothing_mode);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetPageUnit(previous_page_unit);
                if (result)
                {
                    return false;
                }

                return true;
            }

            /**
             * @brief Draw a image.
             *
             * @param pGraphics   Pointer to the Gdiplus::Graphics object for drawing.
             * @param rect        Gdiplus::Rect object representing the position and dimensions of the image to be drawn.
             * @param image       Pointer to the Gdiplus::Image object for drawing.
             * @param opacity     The opacity of the image to be drawn.
             * @param posX        The X coordinate of the image to be drawn.
             * @param posY        The Y coordinate of the image to be drawn.
             * @param width       The width of the image to be drawn.
             * @param height      The height of the image to be drawn.
             * @param centering   Whether to center the image to be drawn.
             *
             * @return Returns true if successfully performed the draw operations, false otherwise.
             */
            bool MyDraw_DrawImage(Gdiplus::Graphics *pGraphics, Gdiplus::Rect rect, Gdiplus::Image *image, FLOAT opacity, INT posX, INT posY, INT width, INT height, bool centering)
            {
                Gdiplus::Status result;

                // Store current graphics parameters.
                Gdiplus::Unit previous_page_unit = pGraphics->GetPageUnit();
                Gdiplus::SmoothingMode previous_smoothing_mode = pGraphics->GetSmoothingMode();

                // Set new graphics paramemters for drawing operations.
                result = pGraphics->SetPageUnit(Gdiplus::Unit::UnitPixel);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
                if (result)
                {
                    return false;
                }

                // Create color matrix.
                Gdiplus::ColorMatrix matrix =
                    {
                        1, 0, 0, 0, 0,
                        0, 1, 0, 0, 0,
                        0, 0, 1, 0, 0,
                        0, 0, 0, opacity, 0,
                        0, 0, 0, 0, 1};

                // Create image attributes.
                Gdiplus::ImageAttributes attributes;
                result = attributes.SetColorMatrix(&matrix);
                if (result)
                {
                    return false;
                }

                // Check if centering the image.
                if (centering)
                {
                    posX = rect.GetRight() / 2 - (width / 2);
                    posY = rect.GetBottom() / 2 - (height / 2);
                }

                // Draw the image
                result = pGraphics->DrawImage(image,
                                              Gdiplus::Rect(posX, posY, width, height),
                                              0, 0, image->GetWidth(), image->GetHeight(), Gdiplus::UnitPixel, &attributes);
                if (result)
                {
                    return false;
                }

                // Restore previous graphics parameters.
                result = pGraphics->SetSmoothingMode(previous_smoothing_mode);
                if (result)
                {
                    return false;
                }
                result = pGraphics->SetPageUnit(previous_page_unit);
                if (result)
                {
                    return false;
                }

                return true;
            }
        }
    }

    /**
     * @brief Functions related to theme management.
     * @brief This namespace contains functions for initializing and updating
     * @brief ui objects for the current application theme, as well as
     * @brief updating class names of standard controls to match the current theme (e.g., Dark Scrollbar from DarkMode API).
     */
    namespace Theme
    {
        /**
         * @brief Update the container contents's class names to match a specified theme.
         *
         * @param theme            The theme.
         * @param pContainer       Pointer to the container.
         * @param applyToNonChilds Specifies whether to apply the changes to container's non-childs windows.
         */
        bool UpdateContainerThemeClass(MyTheme theme, MyContainer *pContainer, bool applyToNonChilds = false)
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Get the container child & non-childs vectors.
                auto container_childs = pContainer->getChildWindowsVector();
                auto container_nonchilds = pContainer->getAssociatedWindowsVector();

                // Get the container vertical scrollbar.
                auto container_scrollbar_vertical = pContainer->pVerticalScrollbarWindow->hWnd;

                bool are_all_operation_success_2 = false;
                HRESULT hr;
                switch (theme)
                {
                // For light themes.
                case MyTheme::Light:
                {
                    // Update new class name for the container childs.
                    if (!container_childs.empty())
                    {
                        bool is_for_loop_failed = false;
                        for (auto &p_window : container_childs)
                        {
                            std::wstring class_name = nSol::GetWindowClassName(p_window->hWnd);
                            if (class_name == WC_SCROLLBAR)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"Explorer", WC_SCROLLBAR);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                            else if (class_name == WC_BUTTON)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"Explorer", WC_BUTTON);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                        }
                        if (is_for_loop_failed)
                        {
                            error_message = L"Failed to update new class name for the container child windows.";
                            break;
                        }
                    }

                    // Update new class name for the container non-childs.
                    if (applyToNonChilds && !container_nonchilds.empty())
                    {
                        bool is_for_loop_failed = false;
                        for (auto &p_window : container_nonchilds)
                        {
                            std::wstring class_name = nSol::GetWindowClassName(p_window->hWnd);
                            if (class_name == WC_SCROLLBAR)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"Explorer", WC_SCROLLBAR);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                            else if (class_name == WC_BUTTON)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"Explorer", WC_BUTTON);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                        }
                        if (is_for_loop_failed)
                        {
                            error_message = L"Failed to update new class name for the container associated windows.";
                            break;
                        }
                    }

                    // Update new class name for the container scrollbar.
                    if (container_scrollbar_vertical)
                    {
                        std::wstring class_name = nSol::GetWindowClassName(container_scrollbar_vertical);
                        if (class_name == WC_SCROLLBAR)
                        {
                            hr = SetWindowTheme(container_scrollbar_vertical, L"Explorer", WC_SCROLLBAR);
                            if (FAILED(hr))
                            {
                                error_message = L"Failed to execute \"SetWindowTheme()\".";
                                break;
                            }
                        }
                        else if (class_name == WC_BUTTON)
                        {
                            hr = SetWindowTheme(container_scrollbar_vertical, L"Explorer", WC_BUTTON);
                            if (FAILED(hr))
                            {
                                error_message = L"Failed to execute \"SetWindowTheme()\".";
                                break;
                            }
                        }
                    }

                    are_all_operation_success_2 = true;
                    break;
                }

                // For dark themes.
                case MyTheme::Dark:
                case MyTheme::Monokai:
                {
                    // Update new class name for the container childs.
                    if (!container_childs.empty())
                    {
                        bool is_for_loop_failed = false;
                        for (auto &p_window : container_childs)
                        {
                            std::wstring class_name = nSol::GetWindowClassName(p_window->hWnd);
                            if (class_name == WC_SCROLLBAR)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"DarkMode_Explorer", WC_SCROLLBAR);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                            else if (class_name == WC_BUTTON)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"DarkMode_Explorer", WC_BUTTON);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                        }
                        if (is_for_loop_failed)
                        {
                            error_message = L"Failed to update new class name for the container child windows.";
                            break;
                        }
                    }

                    // Update new class name for the container non-childs.
                    if (applyToNonChilds && !container_nonchilds.empty())
                    {
                        bool is_for_loop_failed = false;
                        for (auto &p_window : container_nonchilds)
                        {
                            std::wstring class_name = nSol::GetWindowClassName(p_window->hWnd);
                            if (class_name == WC_SCROLLBAR)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"DarkMode_Explorer", WC_SCROLLBAR);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                            else if (class_name == WC_BUTTON)
                            {
                                hr = SetWindowTheme(p_window->hWnd, L"DarkMode_Explorer", WC_BUTTON);
                                if (FAILED(hr))
                                {
                                    is_for_loop_failed = true;
                                    break;
                                }
                            }
                        }
                        if (is_for_loop_failed)
                        {
                            error_message = L"Failed to update new class name for the container associated windows.";
                            break;
                        }
                    }

                    // Update new class name for the container scrollbar.
                    if (container_scrollbar_vertical)
                    {
                        std::wstring class_name = nSol::GetWindowClassName(container_scrollbar_vertical);
                        if (class_name == WC_SCROLLBAR)
                        {
                            hr = SetWindowTheme(container_scrollbar_vertical, L"DarkMode_Explorer", WC_SCROLLBAR);
                            if (FAILED(hr))
                            {
                                error_message = L"Failed to execute \"SetWindowTheme()\".";
                                break;
                            }
                        }
                        else if (class_name == WC_BUTTON)
                        {
                            hr = SetWindowTheme(container_scrollbar_vertical, L"DarkMode_Explorer", WC_BUTTON);
                            if (FAILED(hr))
                            {
                                error_message = L"Failed to execute \"SetWindowTheme()\".";
                                break;
                            }
                        }
                    }

                    are_all_operation_success_2 = true;
                    break;
                }

                default:
                {
                    error_message = L"The theme name is not valid.";
                    break;
                }
                }
                if (!are_all_operation_success_2)
                    break;

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"UpdateContainerThemeClass()\"]", MyLogType::Error);
                return false;
            }

            return true;
        }

        /**
         * @brief Updates all the application containers to match a specified theme.
         *
         * @param theme The theme.
         *
         * @return Returns true if all the operations are successful, otherwise false.
         */
        bool UpdateContainers(MyTheme theme)
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Release current subclass classes D2D1 shared resources
                // so they can be recreated in next refresh() call.
                MyStandardButton::releaseD2D1SharedResources();
                MyImageButton::releaseD2D1SharedResources();
                MyRadioButton::releaseD2D1SharedResources();

                // Container: MainContent
                if (g_ContainerMainContent)
                {
                    // Update the container's dimensions.
                    if (!g_ContainerMainContent->updateContainerDimensions(WINDOW_BORDER_DEFAULTWIDTH,
                                                                           g_pUIElements->rectangles.rectCaption.bottom,
                                                                           g_CurrentWindowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2),
                                                                           g_CurrentWindowHeight - (WINDOW_BORDER_DEFAULTWIDTH * 2) - (g_pUIElements->rectangles.rectCaption.bottom - g_pUIElements->rectangles.rectCaption.top),
                                                                           g_IsCurrentThemeWantScrollbarsVisible, false))
                    {
                        error_message = L"Failed to update the container's dimensions.";
                        break;
                    }

                    // Update the container's theme class.
                    if (!UpdateContainerThemeClass(theme, g_ContainerMainContent))
                    {
                        error_message = L"Failed to update the container's theme class.";
                        break;
                    }

                    // Refresh the container's child windows.
                    auto child_windows = g_ContainerMainContent->getChildWindowsVector();
                    if (!child_windows.empty())
                    {
                        bool is_for_loop_failed = false;
                        for (auto &p_window : child_windows)
                        {
                            if (p_window->isManagedWindow())
                            {
                                switch (p_window->getWindowType())
                                {
                                case MyWindowType::StandardButton:
                                {
                                    MyStandardButton *p_casted_subclass = static_cast<MyStandardButton *>(p_window->getSubclassPointer());
                                    if (p_casted_subclass)
                                    {
                                        if (!p_casted_subclass->refresh(p_window->hWnd))
                                            is_for_loop_failed = true;
                                    }
                                    break;
                                }
                                case MyWindowType::ImageButton:
                                {
                                    MyImageButton *p_casted_subclass = static_cast<MyImageButton *>(p_window->getSubclassPointer());
                                    if (p_casted_subclass)
                                    {
                                        if (!p_casted_subclass->refresh(p_window->hWnd))
                                            is_for_loop_failed = true;
                                    }
                                    break;
                                }
                                case MyWindowType::RadioButton:
                                {
                                    MyRadioButton *p_casted_subclass = static_cast<MyRadioButton *>(p_window->getSubclassPointer());
                                    if (p_casted_subclass)
                                    {
                                        if (!p_casted_subclass->refresh(p_window->hWnd))
                                            is_for_loop_failed = true;
                                    }
                                    break;
                                }
                                }
                                if (is_for_loop_failed)
                                    break;
                            }
                        }
                        if (is_for_loop_failed)
                        {
                            error_message = L"Failed to refresh the container's child windows.";
                            break;
                        }
                    }

                    // Refresh the container's associated windows.
                    auto associated_windows = g_ContainerMainContent->getAssociatedWindowsVector();
                    if (!associated_windows.empty())
                    {
                        bool is_for_loop_failed = false;
                        for (auto &p_window : associated_windows)
                        {
                            if (p_window->isManagedWindow())
                            {
                                switch (p_window->getWindowType())
                                {
                                case MyWindowType::StandardButton:
                                {
                                    MyStandardButton *p_casted_subclass = static_cast<MyStandardButton *>(p_window->getSubclassPointer());
                                    if (p_casted_subclass)
                                    {
                                        if (!p_casted_subclass->refresh(p_window->hWnd))
                                            is_for_loop_failed = true;
                                    }
                                    break;
                                }
                                case MyWindowType::ImageButton:
                                {
                                    MyImageButton *p_casted_subclass = static_cast<MyImageButton *>(p_window->getSubclassPointer());
                                    if (p_casted_subclass)
                                    {
                                        if (!p_casted_subclass->refresh(p_window->hWnd))
                                            is_for_loop_failed = true;
                                    }
                                    break;
                                }
                                case MyWindowType::RadioButton:
                                {
                                    MyRadioButton *p_casted_subclass = static_cast<MyRadioButton *>(p_window->getSubclassPointer());
                                    if (p_casted_subclass)
                                    {
                                        if (!p_casted_subclass->refresh(p_window->hWnd))
                                            is_for_loop_failed = true;
                                    }
                                    break;
                                }
                                }
                                if (is_for_loop_failed)
                                    break;
                            }
                        }
                        if (is_for_loop_failed)
                        {
                            error_message = L"Failed to refresh the container's associated windows.";
                            break;
                        }
                    }
                }

                // Refresh the non-client controls.
                if (!g_VectorNonClientWindows.empty())
                {
                    bool is_for_loop_failed = false;
                    for (auto &p_window : g_VectorNonClientWindows)
                    {
                        if (p_window->isManagedWindow())
                        {
                            switch (p_window->getWindowType())
                            {
                            case MyWindowType::StandardButton:
                            {
                                MyStandardButton *p_casted_subclass = static_cast<MyStandardButton *>(p_window->getSubclassPointer());
                                if (p_casted_subclass)
                                {
                                    if (!p_casted_subclass->refresh(p_window->hWnd))
                                        is_for_loop_failed = true;
                                }
                                break;
                            }
                            case MyWindowType::ImageButton:
                            {
                                MyImageButton *p_casted_subclass = static_cast<MyImageButton *>(p_window->getSubclassPointer());
                                if (p_casted_subclass)
                                {
                                    if (!p_casted_subclass->refresh(p_window->hWnd))
                                        is_for_loop_failed = true;
                                }
                                break;
                            }
                            case MyWindowType::RadioButton:
                            {
                                MyRadioButton *p_casted_subclass = static_cast<MyRadioButton *>(p_window->getSubclassPointer());
                                if (p_casted_subclass)
                                {
                                    if (!p_casted_subclass->refresh(p_window->hWnd))
                                        is_for_loop_failed = true;
                                }
                                break;
                            }
                            }
                            if (is_for_loop_failed)
                                break;
                        }
                    }
                    if (is_for_loop_failed)
                    {
                        error_message = L"Failed to refresh the non-client windows.";
                        break;
                    }
                }

                // Update application border brush.
                // g_pUIElements->pointers.pCurrentBorderColor = &g_pUIElements->colors.borderActive.getD2D1Color(); // // Reserved code, likely to be removed in the future.
                g_IsWindowActive = true;
                if (g_IsWindows11BorderAttributeSupported)
                {
                    COLORREF border_color = g_pUIElements->colors.borderActive.getCOLORREF();
                    HRESULT hr = DwmSetWindowAttribute(g_hWnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
                    if (FAILED(hr))
                    {
                        error_message = L"Failed to set the window border attribute.";
                        break;
                    }
                }

                // Redraw the entire application.
                RedrawWindow(g_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

                // Set application to active.
                SetActiveWindow(g_hWnd);

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"UpdateContainers()\"]", MyLogType::Error);
                return false;
            }

            return true;
        }

        /**
         * @brief Set the application theme.
         *
         * @param Theme The theme.
         *
         * @return Returns true if the application theme is successfully set. Otherwise, false.
         */
        bool SetAppTheme(MyTheme Theme = MyTheme::Light)
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                std::wstring theme_name = L"";

                bool is_switch_failed = false;
                switch (Theme)
                {
                case MyTheme::Light:
                {
                    g_pUIElements->colors.updateMainColors(RGBA(255, 255, 255),  // Primary color.
                                                           RGBA(255, 255, 255),  // Secondary color.
                                                           RGBA(64, 64, 64),     // Border active color.
                                                           RGBA(192, 192, 192),  // Border inactive color.
                                                           RGBA(0, 0, 0),        // Text active color.
                                                           RGBA(153, 153, 153),  // Text inactive color.
                                                           RGBA(0, 0, 0),        // Text highlight color.
                                                           RGBA(0, 162, 237),    // Focus color.
                                                           RGBA(240, 240, 240)); // Background color.

                    g_pUIElements->colors.updateCaptionColors(RGBA(255, 255, 255),  // Caption background color.
                                                              RGBA(0, 0, 0),        // Caption text active color.
                                                              RGBA(153, 153, 153),  // Caption text inactive color.
                                                              RGBA(255, 89, 89),    // Caption close button hover background color.
                                                              RGBA(255, 89, 89),    // Caption close button down background color.
                                                              RGBA(205, 206, 206),  // Caption maximize button hover background color.
                                                              RGBA(205, 206, 206),  // Caption maximize button down background color.
                                                              RGBA(205, 206, 206),  // Caption minimize button hover background color.
                                                              RGBA(205, 206, 206)); // Caption minimize button down background color.

                    g_pUIElements->colors.updateStandardButtonColors(RGBA(225, 225, 225), // Standard button default color.
                                                                     RGBA(229, 241, 251), // Standard button hover color.
                                                                     RGBA(204, 228, 247), // Standard button down color.
                                                                     RGBA(172, 172, 172), // Standard button border default color.
                                                                     RGBA(0, 120, 215),   // Standard button border hover color.
                                                                     RGBA(0, 84, 153));   // Standard button border down color.

                    g_pUIElements->colors.updateRadioButtonColors(RGBA(255, 255, 255), // Radio button primary color.
                                                                  RGBA(255, 255, 255), // Radio button hover state primary color.
                                                                  RGBA(204, 228, 247), // Radio button down state primary color.
                                                                  RGBA(255, 255, 255), // Radio button secondary color.
                                                                  RGBA(255, 255, 255), // Radio button hover state secondary color.
                                                                  RGBA(204, 228, 247), // Radio button down state secondary color.
                                                                  RGBA(51, 51, 51),    // Radio button border color.
                                                                  RGBA(0, 120, 215),   // Radio button hover state border color.
                                                                  RGBA(0, 84, 153),    // Radio button down state border color.
                                                                  RGBA(51, 51, 51),    // Selected radio button primary color.
                                                                  RGBA(0, 120, 215),   // Selected radio button hover state primary color.
                                                                  RGBA(0, 84, 153),    // Selected radio button down state primary color.
                                                                  RGBA(255, 255, 255), // Selected radio button secondary color.
                                                                  RGBA(255, 255, 255), // Selected radio button hover state secondary color.
                                                                  RGBA(204, 228, 247), // Selected radio button down state secondary color.
                                                                  RGBA(51, 51, 51),    // Selected radio button border color.
                                                                  RGBA(0, 120, 215),   // Selected radio button hover state border color.
                                                                  RGBA(0, 84, 153));   // Selected radio button down state border color.

                    g_pUIElements->colors.updateEditboxColors(RGBA(255, 255, 255), // Editbox color.
                                                              RGBA(122, 122, 122), // Editbox border default color.
                                                              RGBA(0, 120, 215));  // Editbox border selected color.

                    g_pUIElements->colors.updateDDLComboboxColors(RGBA(225, 225, 225),  // DDL combobox color.
                                                                  RGBA(172, 172, 172),  // DDL combobox border color.
                                                                  RGBA(255, 255, 255),  // DDL combobox default item background color.
                                                                  RGBA(0, 120, 215),    // DDL combobox selected item background color.
                                                                  RGBA(0, 120, 215),    // DDL combobox drop-down list window border color.
                                                                  RGBA(0, 0, 0),        // DDL combobox default item text color.
                                                                  RGBA(255, 255, 255)); // DDL combobox selected item text color.

                    g_pUIElements->images.updateNonClientButtonImages(*g_pUIElements->images.pWICCrossBlack,  // Close button default image.
                                                                      *g_pUIElements->images.pWICCrossWhite,  // Close button hover image.
                                                                      *g_pUIElements->images.pWICCrossWhite,  // Close button down image.
                                                                      *g_pUIElements->images.pWICSquareBlack, // Maximize button default image.
                                                                      *g_pUIElements->images.pWICSquareWhite, // Maximize button hover image.
                                                                      *g_pUIElements->images.pWICSquareWhite, // Maximize button down image.
                                                                      *g_pUIElements->images.pWICMinusBlack,  // Minimize button default image.
                                                                      *g_pUIElements->images.pWICMinusBlack,  // Minimize button hover image.
                                                                      *g_pUIElements->images.pWICMinusBlack); // Minimize button down image.

                    // Update standard button class.
                    MyStandardButtonSharedAttributesConfig shared_config_mystandardbutton = MyStandardButtonSharedAttributesConfig(&g_pUIElements->colors.standardButtonDefault,
                                                                                                                                   &g_pUIElements->colors.standardButtonHover,
                                                                                                                                   &g_pUIElements->colors.standardButtonDown,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderDefault,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderHover,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderDown,
                                                                                                                                   &g_pUIElements->colors.textActive,
                                                                                                                                   &g_pUIElements->colors.textHighlight,
                                                                                                                                   &g_pUIElements->colors.background,
                                                                                                                                   &g_pUIElements->colors.focus,
                                                                                                                                   &g_pUIElements->fonts.buttonTextFormat);
                    if (!MyStandardButton::setSharedAttributes(shared_config_mystandardbutton))
                    {
                        error_message = L"Failed to update standard button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update image button class.
                    MyImageButtonSharedAttributesConfig shared_config_myimagebutton = MyImageButtonSharedAttributesConfig(&g_pUIElements->colors.focus);
                    if (!MyImageButton::setSharedAttributes(shared_config_myimagebutton))
                    {
                        error_message = L"Failed to update image button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update radio button class.
                    MyRadioButtonSharedAttributesConfig shared_config_myradiobutton = MyRadioButtonSharedAttributesConfig(&g_pUIElements->colors.radioButtonPrimaryDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonPrimaryHover,
                                                                                                                          &g_pUIElements->colors.radioButtonPrimaryDown,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryHover,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryDown,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderHover,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderDown,
                                                                                                                          &g_pUIElements->colors.textActive,
                                                                                                                          &g_pUIElements->colors.textHighlight,
                                                                                                                          &g_pUIElements->colors.background,
                                                                                                                          &g_pUIElements->colors.focus,
                                                                                                                          &g_pUIElements->fonts.buttonTextFormat);
                    if (!MyRadioButton::setSharedAttributes(shared_config_myradiobutton))
                    {
                        error_message = L"Failed to update radio button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update ddl combobox class.
                    MyDDLComboboxSharedPropertiesConfig MyDDLComboboxSharedPropertiesConfig =
                        {
                            &g_pUIElements->colors.ddlCombobox,
                            &g_pUIElements->colors.ddlComboboxBorder,
                            &g_pUIElements->colors.ddlComboboxItemBackground,
                            &g_pUIElements->colors.ddlComboboxSelectedItemBackground,
                            &g_pUIElements->colors.ddlComboboxDropdownlistBorder,
                            &g_pUIElements->colors.textActive,
                            &g_pUIElements->colors.background,
                            &g_pUIElements->colors.focus,
                            &g_pUIElements->fonts.ddlCombobox};
                    if (!MyDDLCombobox::setSharedProperties(MyDDLComboboxSharedPropertiesConfig))
                    {
                        error_message = L"Failed to update ddl combobox class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update global variables.
                    g_CurrentAppTheme = MyTheme::Light;
                    g_IsCurrentThemeWantScrollbarsVisible = true;
                    theme_name = L"Light";
                    break;
                }

                case MyTheme::Dark:
                {
                    g_pUIElements->colors.updateMainColors(RGBA(0, 0, 0),       // Primary color.
                                                           RGBA(32, 32, 32),    // Secondary color.
                                                           RGBA(48, 48, 48),    // Border active color.
                                                           RGBA(57, 57, 57),    // Border inactive color.
                                                           RGBA(216, 222, 233), // Text active color.
                                                           RGBA(162, 162, 162), // Text inactive color.
                                                           RGBA(216, 222, 233), // Text highlight color.
                                                           RGBA(0, 162, 237),   // Focus color.
                                                           RGBA(32, 32, 32));   // Background color.

                    g_pUIElements->colors.updateCaptionColors(RGBA(0, 0, 0),       // Caption background color.
                                                              RGBA(216, 222, 233), // Caption text active color.
                                                              RGBA(162, 162, 162), // Caption text inactive color.
                                                              RGBA(232, 17, 35),   // Caption close button hover background color.
                                                              RGBA(232, 17, 35),   // Caption close button down background color.
                                                              RGBA(57, 57, 57),    // Caption maximize button hover background color.
                                                              RGBA(57, 57, 57),    // Caption maximize button down background color.
                                                              RGBA(57, 57, 57),    // Caption minimize button hover background color.
                                                              RGBA(57, 57, 57));   // Caption minimize button down background color.

                    g_pUIElements->colors.updateStandardButtonColors(RGBA(51, 51, 51),     // Standard button default color.
                                                                     RGBA(69, 69, 69),     // Standard button hover color.
                                                                     RGBA(102, 102, 102),  // Standard button down color.
                                                                     RGBA(155, 155, 155),  // Standard button border default color.
                                                                     RGBA(155, 155, 155),  // Standard button border hover color.
                                                                     RGBA(155, 155, 155)); // Standard button border down color.

                    g_pUIElements->colors.updateRadioButtonColors(RGBA(51, 51, 51),     // Radio button primary color.
                                                                  RGBA(69, 69, 69),     // Radio button hover state primary color.
                                                                  RGBA(87, 87, 87),     // Radio button down state primary color.
                                                                  RGBA(51, 51, 51),     // Radio button secondary color.
                                                                  RGBA(69, 69, 69),     // Radio button hover state secondary color.
                                                                  RGBA(87, 87, 87),     // Radio button down state secondary color.
                                                                  RGBA(155, 155, 155),  // Radio button border color.
                                                                  RGBA(155, 155, 155),  // Radio button hover state border color.
                                                                  RGBA(155, 155, 155),  // Radio button down state border color.
                                                                  RGBA(207, 207, 207),  // Selected radio button primary color.
                                                                  RGBA(225, 225, 225),  // Selected radio button hover state primary color.
                                                                  RGBA(250, 250, 250),  // Selected radio button down state primary color.
                                                                  RGBA(51, 51, 51),     // Selected radio button secondary color.
                                                                  RGBA(51, 51, 51),     // Selected radio button hover state secondary color.
                                                                  RGBA(51, 51, 51),     // Selected radio button down state secondary color.
                                                                  RGBA(155, 155, 155),  // Selected radio button border color.
                                                                  RGBA(155, 155, 155),  // Selected radio button hover state border color.
                                                                  RGBA(155, 155, 155)); // Selected radio button down state border color.

                    g_pUIElements->colors.updateEditboxColors(RGBA(48, 48, 48),     // Editbox color.
                                                              RGBA(79, 79, 79),     // Editbox border default color.
                                                              RGBA(100, 100, 100)); // Editbox border selected color.

                    g_pUIElements->colors.updateDDLComboboxColors(RGBA(51, 51, 51),     // DDL combobox color.
                                                                  RGBA(155, 155, 155),  // DDL combobox border color.
                                                                  RGBA(32, 32, 32),     // DDL combobox default item background color.
                                                                  RGBA(44, 44, 44),     // DDL combobox selected item background color.
                                                                  RGBA(44, 44, 44),     // DDL combobox drop-down list window border color.
                                                                  RGBA(162, 162, 162),  // DDL combobox default item text color.
                                                                  RGBA(255, 255, 255)); // DDL combobox selected item text color.

                    g_pUIElements->images.updateNonClientButtonImages(*g_pUIElements->images.pWICCrossGrey,   // Close button default image.
                                                                      *g_pUIElements->images.pWICCrossWhite,  // Close button hover image.
                                                                      *g_pUIElements->images.pWICCrossWhite,  // Close button down image.
                                                                      *g_pUIElements->images.pWICSquareGrey,  // Maximize button default image.
                                                                      *g_pUIElements->images.pWICSquareWhite, // Maximize button hover image.
                                                                      *g_pUIElements->images.pWICSquareWhite, // Maximize button down image.
                                                                      *g_pUIElements->images.pWICMinusGrey,   // Minimize button default image.
                                                                      *g_pUIElements->images.pWICMinusWhite,  // Minimize button hover image.
                                                                      *g_pUIElements->images.pWICMinusWhite); // Minimize button down image.

                    // Update standard button class.
                    MyStandardButtonSharedAttributesConfig shared_config_mystandardbutton = MyStandardButtonSharedAttributesConfig(&g_pUIElements->colors.standardButtonDefault,
                                                                                                                                   &g_pUIElements->colors.standardButtonHover,
                                                                                                                                   &g_pUIElements->colors.standardButtonDown,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderDefault,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderHover,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderDown,
                                                                                                                                   &g_pUIElements->colors.textActive,
                                                                                                                                   &g_pUIElements->colors.textHighlight,
                                                                                                                                   &g_pUIElements->colors.background,
                                                                                                                                   &g_pUIElements->colors.focus,
                                                                                                                                   &g_pUIElements->fonts.buttonTextFormat);
                    if (!MyStandardButton::setSharedAttributes(shared_config_mystandardbutton))
                    {
                        error_message = L"Failed to update standard button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update image button class.
                    MyImageButtonSharedAttributesConfig shared_config_myimagebutton = MyImageButtonSharedAttributesConfig(&g_pUIElements->colors.focus);
                    if (!MyImageButton::setSharedAttributes(shared_config_myimagebutton))
                    {
                        error_message = L"Failed to update image button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update radio button class.
                    MyRadioButtonSharedAttributesConfig shared_config_myradiobutton = MyRadioButtonSharedAttributesConfig(&g_pUIElements->colors.radioButtonPrimaryDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonPrimaryHover,
                                                                                                                          &g_pUIElements->colors.radioButtonPrimaryDown,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryHover,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryDown,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderHover,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderDown,
                                                                                                                          &g_pUIElements->colors.textActive,
                                                                                                                          &g_pUIElements->colors.textHighlight,
                                                                                                                          &g_pUIElements->colors.background,
                                                                                                                          &g_pUIElements->colors.focus,
                                                                                                                          &g_pUIElements->fonts.buttonTextFormat);
                    if (!MyRadioButton::setSharedAttributes(shared_config_myradiobutton))
                    {
                        error_message = L"Failed to update radio button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update ddl combobox class.
                    MyDDLComboboxSharedPropertiesConfig MyDDLComboboxSharedPropertiesConfig =
                        {
                            &g_pUIElements->colors.ddlCombobox,
                            &g_pUIElements->colors.ddlComboboxBorder,
                            &g_pUIElements->colors.ddlComboboxItemBackground,
                            &g_pUIElements->colors.ddlComboboxSelectedItemBackground,
                            &g_pUIElements->colors.ddlComboboxDropdownlistBorder,
                            &g_pUIElements->colors.textActive,
                            &g_pUIElements->colors.background,
                            &g_pUIElements->colors.focus,
                            &g_pUIElements->fonts.ddlCombobox};
                    if (!MyDDLCombobox::setSharedProperties(MyDDLComboboxSharedPropertiesConfig))
                    {
                        error_message = L"Failed to update ddl combobox class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update global variables.
                    g_CurrentAppTheme = MyTheme::Dark;
                    g_IsCurrentThemeWantScrollbarsVisible = true;
                    theme_name = L"Dark";
                    break;
                }

                case MyTheme::Monokai:
                {
                    g_pUIElements->colors.updateMainColors(RGBA(34, 31, 34),    // Primary color.
                                                           RGBA(45, 42, 46),    // Secondary color.
                                                           RGBA(25, 24, 26),    // Border active color.
                                                           RGBA(49, 47, 51),    // Border inactive color.
                                                           RGBA(231, 230, 229), // Text active color.
                                                           RGBA(82, 76, 83),    // Text inactive color.
                                                           RGBA(231, 230, 229), // Text highlight color.
                                                           RGBA(169, 220, 118), // Focus color.
                                                           RGBA(45, 42, 46));   // Background color.

                    g_pUIElements->colors.updateCaptionColors(RGBA(34, 31, 34),    // Caption background color.
                                                              RGBA(147, 146, 147), // Caption text active color.
                                                              RGBA(91, 89, 92),    // Caption text inactive color.
                                                              RGBA(232, 17, 35),   // Caption close button hover background color.
                                                              RGBA(232, 17, 35),   // Caption close button down background color.
                                                              RGBA(63, 61, 63),    // Caption maximize button hover background color.
                                                              RGBA(63, 61, 63),    // Caption maximize button down background color.
                                                              RGBA(63, 61, 63),    // Caption minimize button hover background color.
                                                              RGBA(63, 61, 63));   // Caption minimize button down background color.

                    g_pUIElements->colors.updateStandardButtonColors(RGBA(64, 62, 65),     // Standard button default color.
                                                                     RGBA(91, 89, 92),     // Standard button hover color.
                                                                     RGBA(101, 99, 102),   // Standard button down color.
                                                                     RGBA(114, 112, 114),  // Standard button border default color.
                                                                     RGBA(114, 112, 114),  // Standard button border hover color.
                                                                     RGBA(114, 112, 114)); // Standard button border down color.

                    g_pUIElements->colors.updateRadioButtonColors(RGBA(64, 62, 65),     // Radio button primary color.
                                                                  RGBA(91, 89, 92),     // Radio button hover state primary color.
                                                                  RGBA(101, 99, 102),   // Radio button down state primary color.
                                                                  RGBA(64, 62, 65),     // Radio button secondary color.
                                                                  RGBA(91, 89, 92),     // Radio button hover state secondary color.
                                                                  RGBA(101, 99, 102),   // Radio button down state secondary color.
                                                                  RGBA(114, 112, 114),  // Radio button border color.
                                                                  RGBA(114, 112, 114),  // Radio button hover state border color.
                                                                  RGBA(114, 112, 114),  // Radio button down state border color.
                                                                  RGBA(231, 230, 229),  // Selected radio button primary color.
                                                                  RGBA(242, 241, 240),  // Selected radio button hover state primary color.
                                                                  RGBA(255, 255, 255),  // Selected radio button down state primary color.
                                                                  RGBA(64, 62, 65),     // Selected radio button secondary color.
                                                                  RGBA(64, 62, 65),     // Selected radio button hover state secondary color.
                                                                  RGBA(64, 62, 65),     // Selected radio button down state secondary color.
                                                                  RGBA(114, 112, 114),  // Selected radio button border color.
                                                                  RGBA(114, 112, 114),  // Selected radio button hover state border color.
                                                                  RGBA(114, 112, 114)); // Selected radio button down state border color.

                    g_pUIElements->colors.updateEditboxColors(RGBA(64, 62, 65),     // Editbox color.
                                                              RGBA(64, 62, 65),     // Editbox border default color.
                                                              RGBA(114, 112, 114)); // Editbox border selected color.

                    g_pUIElements->colors.updateDDLComboboxColors(RGBA(64, 62, 65),     // DDL combobox color.
                                                                  RGBA(64, 62, 65),     // DDL combobox border color.
                                                                  RGBA(64, 62, 65),     // DDL combobox default item background color.
                                                                  RGBA(74, 72, 75),     // DDL combobox selected item background color.
                                                                  RGBA(114, 112, 114),  // DDL combobox drop-down list window border color.
                                                                  RGBA(237, 237, 235),  // DDL combobox default item text color.
                                                                  RGBA(225, 216, 102)); // DDL combobox selected item text color.

                    g_pUIElements->images.updateNonClientButtonImages(*g_pUIElements->images.pWICCrossGrey,   // Close button default image.
                                                                      *g_pUIElements->images.pWICCrossWhite,  // Close button hover image.
                                                                      *g_pUIElements->images.pWICCrossWhite,  // Close button down image.
                                                                      *g_pUIElements->images.pWICSquareGrey,  // Maximize button default image.
                                                                      *g_pUIElements->images.pWICSquareWhite, // Maximize button hover image.
                                                                      *g_pUIElements->images.pWICSquareWhite, // Maximize button down image.
                                                                      *g_pUIElements->images.pWICMinusGrey,   // Minimize button default image.
                                                                      *g_pUIElements->images.pWICMinusWhite,  // Minimize button hover image.
                                                                      *g_pUIElements->images.pWICMinusWhite); // Minimize button down image.

                    // Update standard button class.
                    MyStandardButtonSharedAttributesConfig shared_config_mystandardbutton = MyStandardButtonSharedAttributesConfig(&g_pUIElements->colors.standardButtonDefault,
                                                                                                                                   &g_pUIElements->colors.standardButtonHover,
                                                                                                                                   &g_pUIElements->colors.standardButtonDown,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderDefault,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderHover,
                                                                                                                                   &g_pUIElements->colors.standardButtonBorderDown,
                                                                                                                                   &g_pUIElements->colors.textActive,
                                                                                                                                   &g_pUIElements->colors.textHighlight,
                                                                                                                                   &g_pUIElements->colors.background,
                                                                                                                                   &g_pUIElements->colors.focus,
                                                                                                                                   &g_pUIElements->fonts.buttonTextFormat);
                    if (!MyStandardButton::setSharedAttributes(shared_config_mystandardbutton))
                    {
                        error_message = L"Failed to update standard button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update image button class.
                    MyImageButtonSharedAttributesConfig shared_config_myimagebutton = MyImageButtonSharedAttributesConfig(&g_pUIElements->colors.focus);
                    if (!MyImageButton::setSharedAttributes(shared_config_myimagebutton))
                    {
                        error_message = L"Failed to update image button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update radio button class.
                    MyRadioButtonSharedAttributesConfig shared_config_myradiobutton = MyRadioButtonSharedAttributesConfig(&g_pUIElements->colors.radioButtonPrimaryDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonPrimaryHover,
                                                                                                                          &g_pUIElements->colors.radioButtonPrimaryDown,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryHover,
                                                                                                                          &g_pUIElements->colors.radioButtonSecondaryDown,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderDefault,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderHover,
                                                                                                                          &g_pUIElements->colors.radioButtonBorderDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonPrimaryDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonSecondaryDown,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderDefault,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderHover,
                                                                                                                          &g_pUIElements->colors.selectedRadioButtonBorderDown,
                                                                                                                          &g_pUIElements->colors.textActive,
                                                                                                                          &g_pUIElements->colors.textHighlight,
                                                                                                                          &g_pUIElements->colors.background,
                                                                                                                          &g_pUIElements->colors.focus,
                                                                                                                          &g_pUIElements->fonts.buttonTextFormat);
                    if (!MyRadioButton::setSharedAttributes(shared_config_myradiobutton))
                    {
                        error_message = L"Failed to update radio button class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update ddl combobox class.
                    MyDDLComboboxSharedPropertiesConfig MyDDLComboboxSharedPropertiesConfig =
                        {
                            &g_pUIElements->colors.ddlCombobox,
                            &g_pUIElements->colors.ddlComboboxBorder,
                            &g_pUIElements->colors.ddlComboboxItemBackground,
                            &g_pUIElements->colors.ddlComboboxSelectedItemBackground,
                            &g_pUIElements->colors.ddlComboboxDropdownlistBorder,
                            &g_pUIElements->colors.textActive,
                            &g_pUIElements->colors.background,
                            &g_pUIElements->colors.focus,
                            &g_pUIElements->fonts.ddlCombobox};
                    if (!MyDDLCombobox::setSharedProperties(MyDDLComboboxSharedPropertiesConfig))
                    {
                        error_message = L"Failed to update ddl combobox class.";
                        is_switch_failed = true;
                        break;
                    }

                    // Update global variables.
                    g_CurrentAppTheme = MyTheme::Monokai;
                    g_IsCurrentThemeWantScrollbarsVisible = false;
                    theme_name = L"Monokai";
                    break;
                }
                }
                if (is_switch_failed)
                    break;

                // If the window is not ready, then we don't need to update containers and update main window D2D1 device-dependent resources.
                if (!g_IsWindowReady)
                {
                    WriteLogEx(L"Default application theme: ", (L"\"" + theme_name + L"\"").c_str(), MyLogType::Info, 1);
                    are_all_operation_success = true;
                    break;
                }

                // Update containers.
                if (!UpdateContainers(g_CurrentAppTheme))
                {
                    error_message = L"Failed to update containers.";
                    break;
                }

                // Update main window D2D1 device-dependent resources.
                if (!g_pUIElements->createD2D1DeviceResources(g_hWnd, false))
                {
                    error_message = L"Failed to update main window D2D1 device-dependent resources.";
                    break;
                }

                WriteLogEx(L"Application theme changed: ", (L"\"" + theme_name + L"\"").c_str(), MyLogType::Info, 1);

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"SetAppTheme()\"]", MyLogType::Error);
                return false;
            }

            return true;
        }
    }

    /**
     * @brief Functions related to window management.
     * @brief This namespace contains functions for initialization, uninitialization,
     * @brief creations and utility of the application windows.
     */
    namespace Window
    {
        /**
         * @brief Functions related to WinAPI utility.
         */
        namespace Utility
        {
            /**
             * @brief Remove a specific window style from a window.
             *
             * @param hWnd  Handle to the target window (HWND).
             * @param style The window style to be removed.
             */
            bool RemoveWindowStyle(HWND hWnd, LONG style)
            {
                DWORD new_style = GetClassLongW(hWnd, GCL_STYLE);
                if (!new_style)
                    return false;

                new_style &= ~style;
                SetClassLongW(hWnd, GCL_STYLE, new_style);

                return true;
            }

            /**
             * @brief Callback function for setting the font on child windows.
             *
             * @param hWndChild Handle to the child window.
             * @param hFont     Font handle passed as a LPARAM.
             *
             * @return Returns true if the font handle is valid and the font is set successfully on the child window.
             */
            bool CALLBACK SetFontOnChild(HWND hWndChild, LPARAM hFont)
            {
                if (!reinterpret_cast<HFONT>(hFont))
                    return false;

                SendMessageW(hWndChild, WM_SETFONT, hFont, TRUE);

                return true;
            }

            /**
             * @brief Check if the window is maximized.
             * @brief Source: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
             *
             * @param hWnd Handle to the target window (HWND).
             *
             * @return Returns true if the window is maximized, Otherwise, return false.
             */
            bool IsWindowMaximized(HWND hWnd)
            {
                WINDOWPLACEMENT placement;
                placement.length = sizeof(WINDOWPLACEMENT);

                if (!GetWindowPlacement(hWnd, &placement))
                {
                    WriteLog(L"Failed to retrieve the window placement struct.", L" [NAMESPACE: \"nApp::Window::Utility\" | FUNC: \"IsWindowMaximized()\"]", MyLogType::Error);
                    return false;
                }

                return placement.showCmd == SW_MAXIMIZE;
            }

            /**
             * @brief Adjust the maximized client rect to fit the monitor resolution (Taskbar excluded).
             * @brief Source: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
             *
             * @param window Handle to the target window (HWND).
             * @param rect   Reference to the target window's RECT struct.
             */
            void AdjustMaximizedClientRect(HWND window, RECT &rect)
            {
                if (!IsWindowMaximized(window))
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
                    WriteLog(L"Failed to retrieve the monitor info.", L" [NAMESPACE: \"nApp::Window::Utility\" | FUNC: \"AdjustMaximizedClientRect()\"]", MyLogType::Error);
                    return;
                }

                rect = monitor_info.rcWork;
            }
        }

        /**
         * @brief Functions related to application window initialization processes.
         * @brief This namespace contains functions that handle the initialization of the application.
         */
        namespace Initialization
        {
            /**
             * @brief Initialize application global objects, APIs, and set up basic WinAPI settings.
             *
             * @param hWnd      Handle to the main application window (HWND).
             * @param hInstance Handle to the main application instance (HINSTANCE).
             *
             * @return Returns true if all the initialization tasks are performed successfully. Otherwise, returns false.
             */
            bool InitBegin(HWND hWnd, HINSTANCE hInstance)
            {
                bool are_all_operation_success = false;
                std::wstring error_message = L"";
                while (!are_all_operation_success)
                {
                    // Retrieve the current windows version.
                    auto module_ntdll = GetModuleHandleW(L"ntdll.dll");
                    auto rtl_get_version_number = reinterpret_cast<void(WINAPI *)(LPDWORD major, LPDWORD minor, LPDWORD build)>(GetProcAddress(module_ntdll, "RtlGetNtVersionNumbers"));
                    if (!rtl_get_version_number)
                    {
                        error_message = L"Failed to retrieve the windows version.";
                        break;
                    }
                    rtl_get_version_number(&g_WindowsMajorVersion, &g_WindowsMinorVersion, &g_WindowsBuildNumber);
                    g_WindowsBuildNumber &= ~0xF0000000;
                    WriteLog(L"Windows version: ", (L"\"" + std::to_wstring(g_WindowsMajorVersion) + L"." + std::to_wstring(g_WindowsMinorVersion) + L"." + std::to_wstring(g_WindowsBuildNumber) + L" Build " + std::to_wstring(g_WindowsBuildNumber) + L"\"").c_str(), MyLogType::Debug);
                    if (g_WindowsBuildNumber >= 22000)
                        g_IsWindows11BorderAttributeSupported = true;

                    // Initialize API(s).
                    WriteLog(L"Initializing API(s) ...", L"", MyLogType::Debug);
                    {
                        // DarkMode API.
                        if (!nApp::API::InitDarkModeAPI(hWnd))
                        {
                            error_message = L"Failed to initialize DarkMode API.";
                            break;
                        }
                        WriteLog(L"DarkMode API loaded.", L"", MyLogType::Debug);

                        // Windows Animation Manager API.
                        if (!nApp::API::InitWindowsAnimationManager())
                        {
                            error_message = L"Failed to initialize Windows Animation Manager API.";
                            break;
                        }
                        WriteLog(L"Windows Animation Manager API loaded.", L"", MyLogType::Debug);

                        // GDI+ API.
                        g_pGDIPEngine = new MyGDIPEngine();
                        if (!g_pGDIPEngine || !g_pGDIPEngine->initialize())
                        {
                            error_message = L"Failed to initialize GDI+ API.";
                            break;
                        }
                        WriteLog(L"GDI+ API loaded.", L"", MyLogType::Debug);

                        // Direct2D API.
                        g_pD2D1Engine = new MyD2D1Engine();
                        if (!g_pD2D1Engine || !g_pD2D1Engine->initialize())
                        {
                            error_message = L"Failed to initialize Direct2D API.";
                            break;
                        }
                        WriteLog(L"Direct2D API loaded.", L"", MyLogType::Debug);
                    }
                    WriteLog(L"All APIs initialized successfully.", L"", MyLogType::Debug);

                    // Initialize UI-related objects.
                    WriteLog(L"Initializing UI-related objects ...", L"", MyLogType::Debug);
                    g_pUIElements = new UIElements(g_pGDIPEngine, g_pD2D1Engine);
                    g_ContainerMainContent = new MyContainer(WINDOW_CONTAINER_DEFAULTPADDING, false, WINDOW_CONTAINER_DEFAULTPADDING, true);
                    if (!g_pUIElements || !g_ContainerMainContent)
                    {
                        error_message = L"Failed to initialize UI-related objects.";
                        break;
                    }
                    WriteLog(L"UI-related objects initialized.", L"", MyLogType::Debug);

                    // Initialize my subclass classes.
                    WriteLog(L"Initializing subclass classes ...", L"", MyLogType::Debug);
                    MyStandardButtonInitializeConfig initialize_config_mystandardbutton = MyStandardButtonInitializeConfig(g_pAnimationManager, g_pAnimationTimer, g_pTransitionLibrary, g_pD2D1Engine);
                    MyImageButtonInitializeConfig initialize_config_myimagebutton = MyImageButtonInitializeConfig(g_pAnimationManager, g_pAnimationTimer, g_pTransitionLibrary, g_pD2D1Engine);
                    MyRadioButtonInitializeConfig initialize_config_myradiobutton = MyRadioButtonInitializeConfig(g_pAnimationManager, g_pAnimationTimer, g_pTransitionLibrary, g_pD2D1Engine);
                    MyEditInitializeConfig initialize_config_myedit =
                        {
                            &g_KeyToggleENTER,
                            &g_hWnd};
                    if (!MyStandardButton::initialize(initialize_config_mystandardbutton) ||
                        !MyImageButton::initialize(initialize_config_myimagebutton) ||
                        !MyRadioButton::initialize(initialize_config_myradiobutton) ||
                        !MyEdit::initialize(initialize_config_myedit))
                    {
                        error_message = L"Failed to initialize subclass classes.";
                        break;
                    }
                    WriteLog(L"Subclass classes initialized.", L"", MyLogType::Debug);

                    // Extends the window frames into the client area (enabling the window drop-shadow effect).
                    MARGINS borders = {1, 1, 1, 1};
                    HRESULT hr = DwmExtendFrameIntoClientArea(hWnd, &borders);
                    if (FAILED(hr))
                    {
                        error_message = L"Failed to extend the window frames into the client area.";
                        break;
                    }
                    WriteLog(L"Extended the window frames into client area (MARGINS: 1,1,1,1).", L"", MyLogType::Debug);

                    // Enable the DWM MCSS.
                    hr = DwmEnableMMCSS(TRUE);
                    if (FAILED(hr))
                    {
                        error_message = L"Failed to enable the DWM MCSS.";
                        break;
                    }

                    // Set the minimum resolution for periodic timers to increase the precision of application timers (WM_TIMER).
                    if (timeBeginPeriod(15) != TIMERR_NOERROR)
                    {
                        error_message = L"Failed to set minimum resolution for periodic timers.";
                        break;
                    }
                    WriteLog(L"Minimum resolution for periodic timers sets (15).", L"", MyLogType::Debug);

                    are_all_operation_success = true;
                }

                if (!are_all_operation_success)
                {
                    WriteLog(error_message, L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }

                return true;
            }

            /**
             * @brief Initialize the application theme and perform related operations.
             *
             * @param hWnd Handle to the main application window (HWND).
             *
             * @return Returns true if the application theme is successfully initialized. Otherwise, returns false.
             */
            bool InitTheme(HWND hWnd)
            {
                if (!Theme::SetAppTheme(g_CurrentAppTheme))
                {
                    WriteLog(L"Failed to initialize application theme.", L" [NAMESPACE: \"nApp::Window::Initialization\" | \"FUNC: InitTheme()\"]", MyLogType::Error);
                    return false;
                }

                return true;
            }

            /**
             * @brief Create application windows.
             *
             * @param hWnd      Handle to the main application window (HWND).
             * @param hInstance Handle to the main application instance (HINSTANCE).
             *
             * @return Returns true if all the windows are successfully created. Otherwise, returns false.
             */
            bool InitControl(HWND hWnd, HINSTANCE hInstance)
            {
                // Create non-client windows of the main window.
                // Such as the title, minimize, maximize, and close buttons.
                {
                    bool are_all_operation_success = false;
                    std::wstring error_message = L"";
                    while (!are_all_operation_success)
                    {
                        // Window title.
                        std::wstring text_title = nSol::MGetWindowText(hWnd);
                        MyWindow *p_textnormal_title = new MyWindow(false);
                        p_textnormal_title->hWnd = CreateWindowExW(NULL, WC_STATIC, text_title.c_str(), WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                   WINDOW_BORDER_DEFAULTWIDTH + 10, WINDOW_BORDER_DEFAULTWIDTH + 7, 383, 23,
                                                                   hWnd, (HMENU)IDC_NONCLIENT_CAPTIONTITLE_STATIC, NULL, NULL);
                        if (!p_textnormal_title->hWnd)
                        {
                            error_message = L"Failed to create the window title.";
                            break;
                        }
                        SendMessageW(p_textnormal_title->hWnd, WM_SETFONT, (WPARAM)g_pUIElements->fonts.caption.getHFONT(), FALSE);
                        g_VectorNonClientWindows.push_back(p_textnormal_title);

                        // Close button.
                        MyWindow *p_imagebutton_close = new MyWindow(true);
                        MyImageButtonNonSharedAttributesConfig button_close_nonsharedpropertiesconfig = MyImageButtonNonSharedAttributesConfig(&g_pUIElements->images.pNonClientCloseButtonDefault,
                                                                                                                                               &g_pUIElements->images.pNonClientCloseButtonHover,
                                                                                                                                               &g_pUIElements->images.pNonClientCloseButtonDown,
                                                                                                                                               &g_pUIElements->colors.captionBackground,
                                                                                                                                               &g_pUIElements->colors.closeButtonBackgroundOnHover,
                                                                                                                                               &g_pUIElements->colors.closeButtonBackgroundOnDown,
                                                                                                                                               0, 0, 20, 20, true, false, true);
                        if (!p_imagebutton_close->createImageButton(hWnd, L"", false, true, button_close_nonsharedpropertiesconfig,
                                                                    0, 0, 58, 37, (HMENU)IDC_NONCLIENT_CLOSE_BUTTON))
                        {
                            error_message = L"Failed to create the close button.";
                            break;
                        }
                        g_VectorNonClientWindows.push_back(p_imagebutton_close);
                        g_pUIElements->miscs.hWndNonClientCloseButton = p_imagebutton_close->hWnd;

                        // Maximize button.
                        MyWindow *p_imagebutton_maximize = new MyWindow(true);
                        MyImageButtonNonSharedAttributesConfig button_maximize_nonsharedpropertiesconfig = MyImageButtonNonSharedAttributesConfig(&g_pUIElements->images.pNonClientMaximizeButtonDefault,
                                                                                                                                                  &g_pUIElements->images.pNonClientMaximizeButtonHover,
                                                                                                                                                  &g_pUIElements->images.pNonClientMaximizeButtonDown,
                                                                                                                                                  &g_pUIElements->colors.captionBackground,
                                                                                                                                                  &g_pUIElements->colors.maximizeButtonBackgroundOnHover,
                                                                                                                                                  &g_pUIElements->colors.maximizeButtonBackgroundOnDown,
                                                                                                                                                  0, 0, 20, 20, true, false, true);
                        if (!p_imagebutton_maximize->createImageButton(hWnd, L"", false, true, button_maximize_nonsharedpropertiesconfig,
                                                                       0, 0, 58, 37, (HMENU)IDC_NONCLIENT_MAXIMIZE_BUTTON))
                        {
                            error_message = L"Failed to create the maximize button.";
                            break;
                        }
                        g_VectorNonClientWindows.push_back(p_imagebutton_maximize);
                        g_pUIElements->miscs.hWndNonClientMaximizeButton = p_imagebutton_maximize->hWnd;

                        // Minimize button.
                        MyWindow *p_imagebutton_minimize = new MyWindow(true);
                        MyImageButtonNonSharedAttributesConfig button_minimize_nonsharedpropertiesconfig = MyImageButtonNonSharedAttributesConfig(&g_pUIElements->images.pNonClientMinimizeButtonDefault,
                                                                                                                                                  &g_pUIElements->images.pNonClientMinimizeButtonHover,
                                                                                                                                                  &g_pUIElements->images.pNonClientMinimizeButtonDown,
                                                                                                                                                  &g_pUIElements->colors.captionBackground,
                                                                                                                                                  &g_pUIElements->colors.minimizeButtonBackgroundOnHover,
                                                                                                                                                  &g_pUIElements->colors.minimizeButtonBackgroundOnDown,
                                                                                                                                                  0, 0, 20, 20, true, false, true);
                        if (!p_imagebutton_minimize->createImageButton(hWnd, L"", false, true, button_minimize_nonsharedpropertiesconfig,
                                                                       0, 0, 58, 37, (HMENU)IDC_NONCLIENT_MINIMIZE_BUTTON))
                        {
                            error_message = L"Failed to create the minimize button.";
                            break;
                        }
                        g_VectorNonClientWindows.push_back(p_imagebutton_minimize);
                        g_pUIElements->miscs.hWndNonClientMinimizeButton = p_imagebutton_minimize->hWnd;

                        are_all_operation_success = true;
                    }

                    if (!are_all_operation_success)
                    {
                        WriteLog(error_message, L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitControl()\"]", MyLogType::Error);
                        return false;
                    }
                }

                // Create client windows.
                // Container: MainContent
                {
                    bool are_all_operation_success = false;
                    std::wstring error_message = L"";
                    while (!are_all_operation_success)
                    {
                        MyContainer *p_container = g_ContainerMainContent;

                        // Create the container window.
                        if (!p_container->createContainerWindow(hWnd, false, &WindowProcedure_Container_MainContent,
                                                                WINDOW_BORDER_DEFAULTWIDTH, WINDOW_BORDER_DEFAULTWIDTH + WINDOW_CAPTIONBAR_DEFAULTHEIGHT,
                                                                0, 0, (HMENU)IDC_MAINCONTENT_CONTAINER))
                        {
                            error_message = L"Failed to create the container window.";
                            break;
                        }

                        // Create container's contents.
                        error_message = L"Failed to create the container's contents; an error occurred during the process.";
                        {
                            // Header text: "Sample buttons:"
                            MyWindow *p_textheader_samplebuttons = new MyWindow(false);
                            p_textheader_samplebuttons->hWnd = CreateWindowExW(NULL, WC_STATIC, L"Sample buttons:",
                                                                               WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                               p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1),
                                                                               477, 26, p_container->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SAMPLEBUTTONS_STATIC, NULL, NULL);
                            if (!p_textheader_samplebuttons->hWnd ||
                                !p_container->addWindow(p_textheader_samplebuttons, true))
                                break;
                            p_container->registerNewHorizontalLine(p_textheader_samplebuttons->hWnd); /// End container line 1.

                            // Standard button: "Standard"
                            MyWindow *p_standardbutton_sample = new MyWindow(true);
                            if (!p_standardbutton_sample->createStandardButton(p_container->pContainerWindow->hWnd, L"Standard", true, true,
                                                                               p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1), 130, 40, (HMENU)503))
                                break;
                            if (!p_container->addWindow(p_standardbutton_sample, true))
                                break;

                            // Radio button: "First"
                            MyWindow *p_radiobutton_sample_first = new MyWindow(true);
                            if (!p_radiobutton_sample_first->createRadioButton(p_container->pContainerWindow->hWnd, L"First", true, true,
                                                                               p_container->getInitiateWindowPosition(0) + 140, p_container->getInitiateWindowPosition(1),
                                                                               86, 40, (HMENU)IDC_MAINCONTENT_RADIO_TEST_FIRST_BUTTON))
                                break;
                            if (!SampleRadio.addRadioButton(p_radiobutton_sample_first) ||
                                !p_container->addWindow(p_radiobutton_sample_first, true))
                                break;

                            // Radio button: "Second"
                            MyWindow *p_radiobutton_sample_second = new MyWindow(true);
                            if (!p_radiobutton_sample_second->createRadioButton(p_container->pContainerWindow->hWnd, L"Second", true, true,
                                                                                p_container->getInitiateWindowPosition(0) + 232, p_container->getInitiateWindowPosition(1),
                                                                                110, 40, (HMENU)IDC_MAINCONTENT_RADIO_TEST_SECOND_BUTTON))
                                break;
                            if (!SampleRadio.addRadioButton(p_radiobutton_sample_second) ||
                                !p_container->addWindow(p_radiobutton_sample_second, true))
                                break;

                            // Radio button: "Third"
                            MyWindow *p_radiobutton_sample_third = new MyWindow(true);
                            if (!p_radiobutton_sample_third->createRadioButton(p_container->pContainerWindow->hWnd, L"Third", true, true,
                                                                               p_container->getInitiateWindowPosition(0) + 346, p_container->getInitiateWindowPosition(1),
                                                                               92, 40, (HMENU)IDC_MAINCONTENT_RADIO_TEST_THIRD_BUTTON))
                                break;
                            if (!SampleRadio.addRadioButton(p_radiobutton_sample_third) ||
                                !p_container->addWindow(p_radiobutton_sample_third, true))
                                break;
                            p_container->registerNewHorizontalLine(p_standardbutton_sample->hWnd); /// End container line 2.

                            // Header text: "Sample editboxs:"
                            MyWindow *p_textheader_sampleeditboxes = new MyWindow(false);
                            p_textheader_sampleeditboxes->hWnd = CreateWindowExW(NULL, WC_STATIC, L"Sample editboxs:",
                                                                                 WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                                 p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1),
                                                                                 477, 26, p_container->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SAMPLEEDITBOXES_STATIC, NULL, NULL);
                            if (!p_textheader_sampleeditboxes->hWnd ||
                                !p_container->addWindow(p_textheader_sampleeditboxes, true))
                                break;
                            p_container->registerNewHorizontalLine(p_textheader_sampleeditboxes->hWnd); /// End container line 3.

                            // Note text: "(Normal)"
                            MyWindow *p_textnote_standardeditboxnote = new MyWindow(false);
                            p_textnote_standardeditboxnote->hWnd = CreateWindowExW(NULL, WC_STATIC, L"(Normal)",
                                                                                   WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                                   p_container->getInitiateWindowPosition(0) + 360, p_container->getInitiateWindowPosition(1) + 5,
                                                                                   80, 22, p_container->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_STANDARDEDITBOXNOTE_STATIC, NULL, NULL);
                            if (!p_textnote_standardeditboxnote->hWnd ||
                                !p_container->addWindow(p_textnote_standardeditboxnote, true))
                                break;

                            // Sample standard editbox.
                            MyEditNonSharedPropertiesConfig nonshared_config_editbox_standard =
                                {
                                    IDC_MAINCONTENT_STANDARDEDITBOX_STATIC,
                                    IDC_MAINCONTENT_STANDARDEDITBOX_BUTTON,
                                    false};
                            MyWindow *p_editbox_standard = new MyWindow(true);
                            if (!p_editbox_standard->createEditbox(p_container->pContainerWindow->hWnd, L"", true, true, nonshared_config_editbox_standard, MyEditboxType::Singleline,
                                                                   p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1),
                                                                   270, 33, (HMENU)IDC_MAINCONTENT_STANDARDEDITBOX_EDIT, g_pUIElements->fonts.editbox.getHFONT()))
                                break;
                            MyEdit *p_subclass_editbox_standard = static_cast<MyEdit *>(p_editbox_standard->getSubclassPointer());
                            if (!p_container->addWindow(p_editbox_standard, true))
                                break;

                            // Standard button: Sample standard editbox's submit button.
                            MyWindow *p_button_standardeditboxsubmit = new MyWindow(true);
                            if (!p_button_standardeditboxsubmit->createStandardButton(p_container->pContainerWindow->hWnd, L"OK", true, true,
                                                                                      p_container->getInitiateWindowPosition(0) + 280, p_container->getInitiateWindowPosition(1),
                                                                                      70, 33, (HMENU)IDC_MAINCONTENT_STANDARDEDITBOX_BUTTON))
                                break;
                            if (!p_container->addWindow(p_button_standardeditboxsubmit, true))
                                break;
                            p_container->registerNewHorizontalLine(p_subclass_editbox_standard->getStaticHandleRef()); /// End container line 4.

                            // Note text: "(Password)"
                            MyWindow *p_textnote_passwordeditboxnote = new MyWindow(false);
                            p_textnote_passwordeditboxnote->hWnd = CreateWindowExW(NULL, WC_STATIC, L"(Password)",
                                                                                   WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                                   p_container->getInitiateWindowPosition(0) + 360, p_container->getInitiateWindowPosition(1) + 5,
                                                                                   95, 22, p_container->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_PASSWORDEDITBOXNOTE_STATIC, NULL, NULL);
                            if (!p_textnote_passwordeditboxnote->hWnd ||
                                !p_container->addWindow(p_textnote_passwordeditboxnote, true))
                                break;

                            // Sample password editbox.
                            MyEditNonSharedPropertiesConfig nonshared_config_editbox_password =
                                {
                                    IDC_MAINCONTENT_PASSWORDEDITBOX_STATIC,
                                    IDC_MAINCONTENT_PASSWORDEDITBOX_BUTTON,
                                    false};
                            MyWindow *p_editbox_password = new MyWindow(true);
                            if (!p_editbox_password->createEditbox(p_container->pContainerWindow->hWnd, L"", true, true, nonshared_config_editbox_password, MyEditboxType::SinglelinePassword,
                                                                   p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1), 270, 33, (HMENU)IDC_MAINCONTENT_PASSWORDEDITBOX_EDIT, g_pUIElements->fonts.editbox.getHFONT()))
                                break;
                            if (!p_container->addWindow(p_editbox_password, true))
                                break;
                            MyEdit *p_subclass_editbox_password = static_cast<MyEdit *>(p_editbox_password->getSubclassPointer());

                            // Standard button: Sample password editbox's submit button.
                            MyWindow *p_button_passwordeditboxsubmit = new MyWindow(true);
                            if (!p_button_passwordeditboxsubmit->createStandardButton(p_container->pContainerWindow->hWnd, L"OK", true, true,
                                                                                      p_container->getInitiateWindowPosition(0) + 280, p_container->getInitiateWindowPosition(1),
                                                                                      70, 33, (HMENU)IDC_MAINCONTENT_PASSWORDEDITBOX_BUTTON))
                                break;
                            if (!p_container->addWindow(p_button_passwordeditboxsubmit, true))
                                break;
                            p_container->registerNewHorizontalLine(p_subclass_editbox_password->getStaticHandleRef()); /// End container line 5.

                            // Note text: "(Multiline)"
                            MyWindow *p_textnote_multilineeditboxnote = new MyWindow(false);
                            p_textnote_multilineeditboxnote->hWnd = CreateWindowExW(NULL, WC_STATIC, L"(Multiline)",
                                                                                    WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                                    p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1),
                                                                                    95, 22, p_container->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_MULTILINEEDITBOXNOTE_STATIC, NULL, NULL);
                            if (!p_textnote_multilineeditboxnote->hWnd ||
                                !p_container->addWindow(p_textnote_multilineeditboxnote, true))
                                break;
                            p_container->registerNewHorizontalLine(p_textnote_multilineeditboxnote->hWnd); /// End container line 6.

                            // Sample multiline editbox.
                            MyEditNonSharedPropertiesConfig nonshared_config_editbox_multiline =
                                {
                                    IDC_MAINCONTENT_MULTILINEEDITBOX_STATIC,
                                    NULL,
                                    false};
                            MyWindow *p_editbox_multiline = new MyWindow(true);
                            if (!p_editbox_multiline->createEditbox(p_container->pContainerWindow->hWnd, L"", true, true, nonshared_config_editbox_multiline, MyEditboxType::Multiline,
                                                                    p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1), 478, 200, (HMENU)IDC_MAINCONTENT_MULTILINEEDITBOX_EDIT, g_pUIElements->fonts.editbox.getHFONT()))
                                break;
                            if (!p_container->addWindow(p_editbox_multiline, true))
                                break;
                            MyEdit *p_subclass_editbox_multiline = static_cast<MyEdit *>(p_editbox_multiline->getSubclassPointer());
                            p_container->registerNewHorizontalLine(p_subclass_editbox_multiline->getStaticHandleRef()); /// End container line 7.

                            // Standard button: Sample multiline editbox's submit button.
                            MyWindow *p_button_multilineeditboxsubmit = new MyWindow(true);
                            if (!p_button_multilineeditboxsubmit->createStandardButton(p_container->pContainerWindow->hWnd, L"Clear", true, true,
                                                                                       p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1) - 5,
                                                                                       478, 33, (HMENU)IDC_MAINCONTENT_MULTILINEEDITBOX_BUTTON))
                                break;
                            if (!p_container->addWindow(p_button_multilineeditboxsubmit, true))
                                break;
                            p_container->registerNewHorizontalLine(p_button_multilineeditboxsubmit->hWnd); /// End container line 8.

                            // Header text: "Sample comboboxs:"
                            MyWindow *p_textheader_samplecomboboxes = new MyWindow(false);
                            p_textheader_samplecomboboxes->hWnd = CreateWindowExW(NULL, WC_STATIC, L"Sample comboboxs:",
                                                                                  WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                                  p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1),
                                                                                  477, 26, p_container->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SAMPLECOMBOBOXES_STATIC, NULL, NULL);
                            if (!p_textheader_samplecomboboxes->hWnd ||
                                !p_container->addWindow(p_textheader_samplecomboboxes, true))
                                break;
                            p_container->registerNewHorizontalLine(p_textheader_samplecomboboxes->hWnd); /// End container line 9.

                            // Note text: "(Select theme)"
                            MyWindow *p_textnote_selectthemecomboboxnote = new MyWindow(false);
                            p_textnote_selectthemecomboboxnote->hWnd = CreateWindowExW(NULL, WC_STATIC, L"(Select theme)",
                                                                                       WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                                       p_container->getInitiateWindowPosition(0) + 130, p_container->getInitiateWindowPosition(1) + 9,
                                                                                       127, 22, p_container->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SELECTTHEMENOTE_STATIC, NULL, NULL);
                            if (!p_textnote_selectthemecomboboxnote->hWnd ||
                                !p_container->addWindow(p_textnote_selectthemecomboboxnote, true))
                                break;

                            // DDL Combobox: Select theme.
                            MyWindow *p_ddlcombobox_selecttheme = new MyWindow(true);
                            if (!p_ddlcombobox_selecttheme->createDDLCombobox(p_container->pContainerWindow->hWnd, true, true,
                                                                              p_container->getInitiateWindowPosition(0), p_container->getInitiateWindowPosition(1),
                                                                              120, 40, (HMENU)IDC_MAINCONTENT_SELECTTHEME_COMBOBOX))
                                break;
                            if (!p_container->addWindow(p_ddlcombobox_selecttheme, true))
                                break;
                            p_container->registerNewHorizontalLine(p_ddlcombobox_selecttheme->hWnd); /// End container line 10.

                            // Initialize combobox items (select theme ddl combobox).
                            {
                                const UINT total_items = 3;
                                const UINT longest_item_textlength = 8; // Including null string.
                                WCHAR item_array[total_items][longest_item_textlength] =
                                    {
                                        TEXT("Light"), TEXT("Dark"), TEXT("Monokai")};
                                WCHAR text_buffer[longest_item_textlength];
                                memset(&text_buffer, 0, sizeof(text_buffer)); // Memset to ensure the buffer is clean.
                                for (int i = 0; i < total_items; i++)         // Load items to the combobox.
                                {
                                    wcscpy_s(text_buffer, sizeof(text_buffer) / sizeof(WCHAR), (WCHAR *)item_array[i]);
                                    SendMessageW(p_ddlcombobox_selecttheme->hWnd, CB_ADDSTRING, 0, (LPARAM)text_buffer);
                                }
                                // Set default selected item.
                                switch (g_CurrentAppTheme)
                                {
                                case MyTheme::Light:
                                    SendMessageW(p_ddlcombobox_selecttheme->hWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
                                    break;

                                case MyTheme::Dark:
                                    SendMessageW(p_ddlcombobox_selecttheme->hWnd, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
                                    break;

                                case MyTheme::Monokai:
                                    SendMessageW(p_ddlcombobox_selecttheme->hWnd, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
                                    break;
                                }
                            }

                            // Create container vertical scrollbar.
                            if (!p_container->createVerticalScrollbarWindow(hWnd, false, 0, 0, WINDOW_SCROLLBAR_DEFAULTWIDTH, 0, (HMENU)IDC_MAINCONTENT_CONTAINER_SCROLLBAR))
                                break;

                            // Set the scroll information for the container vertical scrollbar.
                            if (!p_container->setVerticalScrollInformations(false))
                                break;
                        }
                        error_message = L"";

                        // Invalidate the container.
                        if (!p_container->invalidateCheck())
                        {
                            error_message = L"The MainContent container is invalid.";
                            break;
                        }

                        are_all_operation_success = true;
                    }

                    if (!are_all_operation_success)
                    {
                        WriteLog(error_message, L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitControl()\"]", MyLogType::Error);
                        return false;
                    }
                }

                return true;
            }

            /**
             * @brief Perform additional operations on the application window objects.
             *
             * @param hWnd Handle to the main application window (HWND).
             *
             * @return Returns true if all the additional operation tasks are performed successfully. Otherwise, returns false.
             */
            bool InitEnd(HWND hWnd)
            {
                bool are_all_operation_success = false;
                std::wstring error_message = L"";
                while (!are_all_operation_success)
                {
                    // Set fonts for unmanaged windows.
                    {
                        // Static texts (Heading).
                        SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SAMPLEBUTTONS_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.heading.getHFONT(), TRUE);
                        SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SAMPLEEDITBOXES_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.heading.getHFONT(), TRUE);
                        SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SAMPLECOMBOBOXES_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.heading.getHFONT(), TRUE);

                        // Static texts (Note).
                        SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_STANDARDEDITBOXNOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);
                        SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_PASSWORDEDITBOXNOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);
                        SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_MULTILINEEDITBOXNOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);
                        SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SELECTTHEMENOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);
                    }

                    // Modify standard class (BUTTON) style.
                    if (!nApp::Window::Utility::RemoveWindowStyle(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_STANDARDBUTTON_BUTTON), CS_DBLCLKS))
                    {
                        error_message = L"Failed to modify standard class (BUTTON) style.";
                        break;
                    }

                    // Update class name for standard controls in containers based on the application theme.
                    if (!Theme::UpdateContainerThemeClass(g_CurrentAppTheme, g_ContainerMainContent))
                    {
                        error_message = L"Failed to update class name for standard controls in containers based on the application theme.";
                        break;
                    }

                    are_all_operation_success = true;
                }

                if (!are_all_operation_success)
                {
                    WriteLog(error_message, L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitEnd()\"]", MyLogType::Error);
                    return false;
                }

                return true;
            }

            /**
             * @brief Sets the main window and its essential child windows to be visible, indicating that the application is ready.
             *
             * @return Returns true if the visibility of the main window and its essential child windows are successfully set. Otherwise, returns false.
             */
            bool OnReady()
            {
                bool are_all_operation_success = false;
                std::wstring error_message = L"";
                while (!are_all_operation_success)
                {
                    // Create the main window render target and its device-dependent resources.
                    if (!g_pUIElements->createD2D1DeviceResources(g_hWnd, true))
                    {
                        error_message = L"Failed to create the main window render target and its device-dependent resources.";
                        break;
                    }

                    // Show main window.
                    ShowWindow(g_hWnd, SW_NORMAL);

                    // Show non-client windows.
                    for (const auto &window : g_VectorNonClientWindows)
                    {
                        ShowWindow(window->hWnd, SW_NORMAL);
                    }

                    // Show containers.
                    ShowWindow(g_ContainerMainContent->pContainerWindow->hWnd, SW_NORMAL);
                    if (!IsWindowVisible(g_hWnd))
                    {
                        error_message = L"Failed to show main window.";
                        break;
                    }

                    // Indicate that the application is ready.
                    g_IsWindowReady = true;
                    WriteLogEx(L"Application is ready.", L"", MyLogType::Info, 1);

                    // Update container scrollbar visiblity.
                    if (!g_ContainerMainContent->updateScrollbarsVisibility(g_IsCurrentThemeWantScrollbarsVisible))
                    {
                        error_message = L"Failed to update container scrollbar visiblity.";
                        break;
                    }

                    /*
                     * The WM_NCACTIVATE message of the main window no longer be suppressed.
                     * It was suppressed to prevent un-wanted non-client paints during window startup animation.
                     * The non-client paint artifacts disappeared after replacing the main window's paint operations with Direct2D.
                     * Drawing performance has improved drastically, eliminating any remaining flickering.
                     * Therefore, the IDT_ACTIVE_CHECK timer is no longer needed.
                     * This code is reserved and likely to be removed in the future.
                     */
                    // Set the window active check timer to make sure the main window drop-shadow effects are drawn correctly on startup.
                    // SetTimer(g_hWnd, IDT_ACTIVE_CHECK, 50, NULL);

                    are_all_operation_success = true;
                }

                if (!are_all_operation_success)
                {
                    WriteLog(error_message, L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"OnReady()\"]", MyLogType::Error);
                    return false;
                }

                return true;
            }
        }

        /**
         * @brief Functions related to application window uninitialization processes.
         * @brief This namespace contains functions that handle the uninitialization of the application.
         */
        namespace Uninitialization
        {
            /**
             * @brief Perform additional operations on the application window objects before exiting the application.
             */
            void OnExit()
            {
                WriteLog(L"Exiting the application ...", L"", MyLogType::Debug);

                // Reverse the extended window frames (disabling the window drop-shadow effect).
                MARGINS borders = {0, 0, 0, 0};
                HRESULT hr = DwmExtendFrameIntoClientArea(g_hWnd, &borders);
                if (FAILED(hr))
                {
                    WriteLog(L"Failed to reverse the extended window frames.", L" [NAMESPACE: \"nApp::Window::Uninitialization\" | FUNC: \"OnExit()\"]", MyLogType::Error);
                    return;
                }
                WriteLog(L"Reversed the extended window frames (MARGINS: 0,0,0,0).", L"", MyLogType::Debug);
            }

            /**
             * @brief Perform necessary deallocations and uninitializes of global objects, APIs and related operations to properly conclude the application.
             *
             * @return Returns true if all the operations are successful, otherwise false.
             */
            bool OnDestroy()
            {
                bool are_all_operation_success = false;
                std::wstring error_message = L"";
                while (!are_all_operation_success)
                {
                    // Destroy the main window render target and its device-dependent resources.
                    g_pUIElements->releaseD2D1DeviceResources();
                    WriteLog(L"Main window render target and its device-dependent resources released.", L"", MyLogType::Debug);

                    // Destroy non-client windows.
                    {
                        if (!g_VectorNonClientWindows.empty())
                        {
                            for (auto &p_window : g_VectorNonClientWindows)
                            {
                                if (p_window)
                                {
                                    delete p_window;
                                }
                            }
                            g_VectorNonClientWindows.clear();
                        }
                        WriteLog(L"Non-client window(s) destroyed.", L"", MyLogType::Debug);
                    }

                    // Uninitialize UI-related objects.
                    delete g_ContainerMainContent;
                    delete g_pUIElements;
                    WriteLog(L"UI-related objects uninitialized.", L"", MyLogType::Debug);

                    // Release left-over D2D1 resources of the subclass classes.
                    MyStandardButton::releaseD2D1SharedResources();
                    MyImageButton::releaseD2D1SharedResources();
                    MyRadioButton::releaseD2D1SharedResources();
                    WriteLog(L"Subclass classes resources released.", L"", MyLogType::Debug);

                    // Uninitialize API(s).
                    {
                        // Direct2D API.
                        if (!g_pD2D1Engine->uninitialize())
                        {
                            error_message = L"Failed to uninitialize Direct2D API.";
                            break;
                        }
                        delete g_pD2D1Engine;
                        WriteLog(L"Direct2D API uninitialized.", L"", MyLogType::Debug);

                        // GDI+ API.
                        if (!g_pGDIPEngine->uninitialize())
                        {
                            error_message = L"Failed to uninitialize GDI+ API.";
                            break;
                        }
                        delete g_pGDIPEngine;
                        WriteLog(L"GDI Animation & GDI+ APIs uninitialized.", L"", MyLogType::Debug);

                        // Windows Animation Manager API.
                        if (!nApp::API::UninitWindowAnimationManager())
                        {
                            error_message = L"Failed to uninitialize Windows Animation Manager API.";
                            break;
                        }
                        WriteLog(L"Windows Animation Manager API uninitialized.", L"", MyLogType::Debug);

                        // Dark Mode API doesn't need to be uninitialized.
                    }

                    // Clears previously set minimum timer resolution.
                    if (timeEndPeriod(15) != TIMERR_NOERROR)
                    {
                        error_message = L"Failed to clear previously set minimum timer resolution.";
                        break;
                    }
                    WriteLog(L"Previously set minimum timer resolution cleared.", L"", MyLogType::Debug);

                    are_all_operation_success = true;
                }

                if (!are_all_operation_success)
                {
                    WriteLog(error_message, L" [NAMESPACE: \"nApp::Window::Uninitialization\" | FUNC: \"OnDestroy()\"]", MyLogType::Error);
                    return false;
                }

                return true;
            }
        }
    }
}

#endif // FUNC_H