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
            size_t text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd)) + static_cast<size_t>(1);
            p_text_buffer = new WCHAR[text_length];
            if (!GetWindowTextW(hWnd, p_text_buffer, static_cast<INT>(text_length)) && text_length != NULL)
            {
                delete[] p_text_buffer;
                error_message = L"Failed to retrieve the window text.";
                break;
            }
            std::wstring window_name(p_text_buffer);
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
            message.append(L"Window name: " + window_name + L"\n");
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
    std::wstring GetWindowClassName(HWND hWnd)
    {
        // Allocate the text buffer for the window class name.
        WCHAR *text_buffer = new WCHAR[MAX_CLASS_NAME_LENGTH];

        // Get the window class name.
        if (!GetClassNameW(hWnd, text_buffer, MAX_CLASS_NAME_LENGTH))
        {
            WriteLog(L"Failed to retrieve the window class name.", L" [NAMESPACE: \"nSol\" | FUNC: \"GetWindowClassName()\"]", MyLogType::Error);
            delete[] text_buffer;
            return L"";
        }
        std::wstring class_name(text_buffer);
        delete[] text_buffer;

        return class_name;
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
            bool generate_file_notice = true;           // Set this to false to disable file generation notice.
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
                        WriteLog(L"Default settings file not found, generating one ...", L"", MyLogType::Debug);
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
                int log_level;
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
     * @brief deinitialization, and management of various APIs used by the application.
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
         * @brief Initialize GDI Animation API and GDI+ API.
         *
         * @param pToken Pointer to a ULONG_PTR that receives a token.
         * @param pInput Pointer to a GdiplusStartupInput structure.
         *
         * @return Returns true if the GDI Animation API and GDI+ API was successfully initialized, false otherwise.
         */
        bool InitGraphicAPI(ULONG_PTR *pToken, const Gdiplus::GdiplusStartupInput *pInput)
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                // Initialize GDI Animation API.
                HRESULT hr = BufferedPaintInit();
                if (FAILED(hr))
                {
                    error_message = L"Failed to initialize GDI Animation API.";
                    break;
                }

                // Initialize GDI+ API.
                if (Gdiplus::GdiplusStartup(pToken, pInput, NULL))
                {
                    error_message = L"Failed to initialize GDI+ API.";
                    break;
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                WriteLog(error_message, L" [NAMESPACE: \"nApp::API\" | FUNC: \"InitGraphicAPI()\"]", MyLogType::Error);
                return false;
            }

            return true;
        }

        /**
         * @brief Uninitialize GDI Animation API and GDI+ API.
         *
         * @param token Token returned by a previous call to GdiplusStartup.
         *
         * @return Returns true if the GDI Animation API and GDI+ API was successfully uninitialized, false otherwise.
         */
        bool UninitGraphicAPI(ULONG_PTR token)
        {
            // Shutdown GDI+ API.
            Gdiplus::GdiplusShutdown(token);

            // Uninitialize GDI Animation API.
            HRESULT hr = BufferedPaintUnInit();
            if (FAILED(hr))
            {
                WriteLog(L"Failed to uninitialize GDI Animation API.", L" [NAMESPACE: \"nApp::API\" | FUNC: \"UninitGraphicAPI()\"]", MyLogType::Error);
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
     * @brief global drawing objects for the current application theme, as well as
     * @brief updating class names of standard controls to match the current theme (e.g., Dark Scrollbar from DarkMode API).
     */
    namespace Theme
    {
        /**
         * @brief Update the container contents class name to match a specified theme.
         *
         * @param Theme            The theme name.
         * @param ContainerPtr     Handle to the target container pointer (MyContainer*).
         * @param ApplyToNonChilds Specifies whether to apply the changes to container non-childs.
         */
        void UpdateContainerThemeClass(MyTheme Theme, MyContainer *ContainerPtr, bool ApplyToNonChilds = false)
        {
            // Text buffer to retrieve the window class name.
            WCHAR TextBuffer[256];

            // Get the container child & non-childs vectors.
            auto Container_Vector_Childs = ContainerPtr->getChildWindowsVector();
            auto Container_Vector_NonChilds = ContainerPtr->getAssociatedWindowsVector();

            // Get the container vertical scrollbar.
            auto Container_Scrollbar_Vertical = ContainerPtr->pVerticalScrollbarWindow->hWnd;

            switch (Theme)
            {
            // For light themes.
            case MyTheme::Light:
            {
                // Update new class name for the container childs.
                if (!Container_Vector_Childs.empty())
                {
                    for (auto &pWindow : Container_Vector_Childs)
                    {
                        GetClassNameW(pWindow->hWnd, TextBuffer, 256);
                        std::wstring lwstr(TextBuffer);
                        if (lwstr == L"ScrollBar")
                            SetWindowTheme(pWindow->hWnd, L"Explorer", L"ScrollBar");
                        else if (lwstr == L"Button")
                            SetWindowTheme(pWindow->hWnd, L"Explorer", L"Button");
                    }
                }

                // Update new class name for the container non-childs.
                if (ApplyToNonChilds && !Container_Vector_NonChilds.empty())
                {
                    for (auto &pWindow : Container_Vector_NonChilds)
                    {
                        GetClassNameW(pWindow->hWnd, TextBuffer, 256);
                        std::wstring lwstr(TextBuffer);
                        if (lwstr == L"ScrollBar")
                            SetWindowTheme(pWindow->hWnd, L"Explorer", L"ScrollBar");
                        else if (lwstr == L"Button")
                            SetWindowTheme(pWindow->hWnd, L"Explorer", L"Button");
                    }
                }

                // Update new class name for the container scrollbar.
                if (Container_Scrollbar_Vertical)
                {
                    GetClassNameW(Container_Scrollbar_Vertical, TextBuffer, 256);
                    std::wstring lwstr(TextBuffer);
                    if (lwstr == L"ScrollBar")
                        SetWindowTheme(Container_Scrollbar_Vertical, L"Explorer", L"ScrollBar");
                    else if (lwstr == L"Button")
                        SetWindowTheme(Container_Scrollbar_Vertical, L"Explorer", L"Button");
                }

                break;
            }

            // For dark themes.
            case MyTheme::Dark:
            case MyTheme::Monokai:
            {
                // Update new class name for the container childs.
                if (!Container_Vector_Childs.empty())
                {
                    for (auto &pWindow : Container_Vector_Childs)
                    {
                        GetClassNameW(pWindow->hWnd, TextBuffer, 256);
                        std::wstring lwstr(TextBuffer);
                        if (lwstr == L"ScrollBar")
                            SetWindowTheme(pWindow->hWnd, L"DarkMode_Explorer", L"ScrollBar");
                        else if (lwstr == L"Button")
                            SetWindowTheme(pWindow->hWnd, L"DarkMode_Explorer", L"Button");
                    }
                }

                // Update new class name for the container non-childs.
                if (ApplyToNonChilds && !Container_Vector_NonChilds.empty())
                {
                    for (auto &pWindow : Container_Vector_NonChilds)
                    {
                        GetClassNameW(pWindow->hWnd, TextBuffer, 256);
                        std::wstring lwstr(TextBuffer);
                        if (lwstr == L"ScrollBar")
                            SetWindowTheme(pWindow->hWnd, L"DarkMode_Explorer", L"ScrollBar");
                        else if (lwstr == L"Button")
                            SetWindowTheme(pWindow->hWnd, L"DarkMode_Explorer", L"Button");
                    }
                }

                // Update new class name for the container scrollbar.
                if (Container_Scrollbar_Vertical)
                {
                    GetClassNameW(Container_Scrollbar_Vertical, TextBuffer, 256);
                    std::wstring lwstr(TextBuffer);
                    if (lwstr == L"ScrollBar")
                        SetWindowTheme(Container_Scrollbar_Vertical, L"DarkMode_Explorer", L"ScrollBar");
                    else if (lwstr == L"Button")
                        SetWindowTheme(Container_Scrollbar_Vertical, L"DarkMode_Explorer", L"Button");
                }

                break;
            }

            default:
            {
                std::wstring error_message = L"";
                error_message.append(L"Error occurred!\n");
                error_message.append(L"The theme name is not valid.\n\n");
                error_message.append(L"NAMESPACE: nApp::Theme\n");
                error_message.append(L"FUNC: UpdateContainerThemeClass()");
                MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                WriteLog(L"The theme name is not valid.", L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"UpdateContainerThemeClass()\"]", MyLogType::Error);

                break;
            }
            }
        }

        /**
         * @brief Updates all the application containers to match a specified theme.
         *
         * @param Theme The theme name.
         */
        void UpdateContainers(MyTheme Theme)
        {
            /// CONTINUE WORK: Replace code with updateContainer() function from the MyContainer class.
            switch (Theme)
            {
            case MyTheme::Light:
            {
                // Container: MainContent
                if (g_ContainerMainContent->pContainerWindow->hWnd)
                {
                    // Update container positions and dimensions.
                    SetWindowPos(g_ContainerMainContent->pContainerWindow->hWnd, NULL,
                                 WINDOW_BORDER_DEFAULTWIDTH,                                                                                                                            // PosX
                                 g_pUIElements->rectangles.rectCaption.bottom,                                                                                                          // PosY
                                 g_CurrentWindowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2) - WINDOW_SCROLLBAR_DEFAULTWIDTH,                                                               // Width
                                 g_CurrentWindowHeight - (WINDOW_BORDER_DEFAULTWIDTH * 2) - (g_pUIElements->rectangles.rectCaption.bottom - g_pUIElements->rectangles.rectCaption.top), // Height
                                 SWP_NOZORDER);

                    // Check if the container scrollbar exists and if the current container dimensions need the scrollbar to be visible.
                    if (g_ContainerMainContent->pVerticalScrollbarWindow->hWnd && g_IsCurrentThemeWantScrollbarsVisible)
                    {
                        // Get the container client rect.
                        RECT RECT_Container;
                        if (!GetClientRect(g_ContainerMainContent->pContainerWindow->hWnd, &RECT_Container))
                        {
                            std::wstring error_message = L"";
                            error_message.append(L"Error occurred!\n");
                            error_message.append(L"Failed to retrieve the container client rect.\n\n");
                            error_message.append(L"NAMESPACE: nApp::Theme\n");
                            error_message.append(L"FUNC: UpdateContainers()");
                            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                            WriteLog(L"Failed to retrieve the container client rect.", L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"UpdateContainers()\"]", MyLogType::Error);
                        }

                        // Get the container scroll info.
                        SCROLLINFO si;
                        si.cbSize = sizeof(SCROLLINFO);
                        si.fMask = SIF_ALL;
                        if (SendMessageW(g_ContainerMainContent->pVerticalScrollbarWindow->hWnd, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
                        {
                            std::wstring error_message = L"";
                            error_message.append(L"Error occurred!\n");
                            error_message.append(L"Failed to retrieve the container scroll info.\n\n");
                            error_message.append(L"NAMESPACE: nApp::Theme\n");
                            error_message.append(L"FUNC: UpdateContainers()");
                            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                            WriteLog(L"Failed to retrieve the container scroll info.", L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"UpdateContainers()\"]", MyLogType::Error);
                        }

                        // If the container is big enough to show all of its content, then hide the scrollbar. Otherwise, show the scrollbar.
                        if (static_cast<INT>(RECT_Container.bottom) > si.nMax)
                            ShowWindow(g_ContainerMainContent->pVerticalScrollbarWindow->hWnd, SW_HIDE);
                        else
                            ShowWindow(g_ContainerMainContent->pVerticalScrollbarWindow->hWnd, SW_SHOW);
                    }

                    // Update the container contents class name to match the theme.
                    UpdateContainerThemeClass(Theme, g_ContainerMainContent);
                }

                break;
            }

            case MyTheme::Dark:
            {
                // Container: MainContent
                if (g_ContainerMainContent->pContainerWindow->hWnd)
                {
                    // Update container positions and dimensions.
                    SetWindowPos(g_ContainerMainContent->pContainerWindow->hWnd, NULL,
                                 WINDOW_BORDER_DEFAULTWIDTH,                                                                                                                            // PosX
                                 g_pUIElements->rectangles.rectCaption.bottom,                                                                                                          // PosY
                                 g_CurrentWindowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2) - WINDOW_SCROLLBAR_DEFAULTWIDTH,                                                               // Width
                                 g_CurrentWindowHeight - (WINDOW_BORDER_DEFAULTWIDTH * 2) - (g_pUIElements->rectangles.rectCaption.bottom - g_pUIElements->rectangles.rectCaption.top), // Height
                                 SWP_NOZORDER);

                    // Check if the container scrollbar exists and if the current container dimensions require the scrollbar to be visible.
                    if (g_ContainerMainContent->pVerticalScrollbarWindow->hWnd && g_IsCurrentThemeWantScrollbarsVisible)
                    {
                        // Get the container client rect.
                        RECT RECT_Container;
                        if (!GetClientRect(g_ContainerMainContent->pContainerWindow->hWnd, &RECT_Container))
                        {
                            std::wstring error_message = L"";
                            error_message.append(L"Error occurred!\n");
                            error_message.append(L"Failed to retrieve the container client rect.\n\n");
                            error_message.append(L"NAMESPACE: nApp::Theme\n");
                            error_message.append(L"FUNC: UpdateContainers()");
                            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                            WriteLog(L"Failed to retrieve the container client rect.", L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"UpdateContainers()\"]", MyLogType::Error);
                        }

                        // Get the container scroll info.
                        SCROLLINFO si;
                        si.cbSize = sizeof(SCROLLINFO);
                        si.fMask = SIF_ALL;
                        if (SendMessageW(g_ContainerMainContent->pVerticalScrollbarWindow->hWnd, SBM_GETSCROLLINFO, NULL, (LPARAM)&si) == FALSE)
                        {
                            std::wstring error_message = L"";
                            error_message.append(L"Error occurred!\n");
                            error_message.append(L"Failed to retrieve the container scroll info.\n\n");
                            error_message.append(L"NAMESPACE: nApp::Theme\n");
                            error_message.append(L"FUNC: UpdateContainers()");
                            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                            WriteLog(L"Failed to retrieve the container scroll info.", L" [NAMESPACE: \"nApp::Theme\" | FUNC: \"UpdateContainers()\"]", MyLogType::Error);
                        }

                        // If the container is big enough to show all of its content, then hide the scrollbar. Otherwise, show the scrollbar.
                        if (static_cast<INT>(RECT_Container.bottom) > si.nMax)
                            ShowWindow(g_ContainerMainContent->pVerticalScrollbarWindow->hWnd, SW_HIDE);
                        else
                            ShowWindow(g_ContainerMainContent->pVerticalScrollbarWindow->hWnd, SW_SHOW);
                    }

                    // Update the container contents class name to match the theme.
                    UpdateContainerThemeClass(Theme, g_ContainerMainContent);
                }

                break;
            }

            case MyTheme::Monokai:
            {
                // Container: MainContent
                if (g_ContainerMainContent->pContainerWindow->hWnd)
                {
                    // This theme doesn't need the scrollbar to be visible, hide the scrollbar if it exists.
                    if (g_ContainerMainContent->pVerticalScrollbarWindow->hWnd)
                        ShowWindow(g_ContainerMainContent->pVerticalScrollbarWindow->hWnd, SW_HIDE);

                    // Update container positions and dimensions.
                    SetWindowPos(g_ContainerMainContent->pContainerWindow->hWnd, NULL,
                                 WINDOW_BORDER_DEFAULTWIDTH,                                                                                                                            // PosX
                                 g_pUIElements->rectangles.rectCaption.bottom,                                                                                                          // PosY
                                 g_CurrentWindowWidth - (WINDOW_BORDER_DEFAULTWIDTH * 2),                                                                                               // Width
                                 g_CurrentWindowHeight - (WINDOW_BORDER_DEFAULTWIDTH * 2) - (g_pUIElements->rectangles.rectCaption.bottom - g_pUIElements->rectangles.rectCaption.top), // Height
                                 SWP_NOZORDER);

                    // Update the container contents class name to match the theme.
                    UpdateContainerThemeClass(Theme, g_ContainerMainContent);
                }
                break;
            }
            }

            // Refresh the non-client controls.
            if (!g_VectorNonClientWindows.empty())
            {
                for (auto &Window : g_VectorNonClientWindows)
                {
                    if (Window->isManagedWindow())
                    {
                        switch (Window->getWindowType())
                        {
                        case MyWindowType::StandardButton:
                        {
                            MyStandardButton *pSubclass = static_cast<MyStandardButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        case MyWindowType::ImageButton:
                        {
                            MyImageButton *pSubclass = static_cast<MyImageButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        case MyWindowType::RadioButton:
                        {
                            MyRadioButton *pSubclass = static_cast<MyRadioButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        }
                    }
                }
            }

            // Refresh the associated subclass.
            auto Childs = g_ContainerMainContent->getChildWindowsVector();
            if (!Childs.empty())
            {
                for (auto &Window : Childs)
                {
                    if (Window->isManagedWindow())
                    {
                        switch (Window->getWindowType())
                        {
                        case MyWindowType::StandardButton:
                        {
                            MyStandardButton *pSubclass = static_cast<MyStandardButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        case MyWindowType::ImageButton:
                        {
                            MyImageButton *pSubclass = static_cast<MyImageButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        case MyWindowType::RadioButton:
                        {
                            MyRadioButton *pSubclass = static_cast<MyRadioButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        }
                    }
                }
            }
            auto NonChilds = g_ContainerMainContent->getAssociatedWindowsVector();
            if (!NonChilds.empty())
            {
                for (auto &Window : NonChilds)
                {
                    if (Window->isManagedWindow())
                    {
                        switch (Window->getWindowType())
                        {
                        case MyWindowType::StandardButton:
                        {
                            MyStandardButton *pSubclass = static_cast<MyStandardButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        case MyWindowType::ImageButton:
                        {
                            MyImageButton *pSubclass = static_cast<MyImageButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        case MyWindowType::RadioButton:
                        {
                            MyRadioButton *pSubclass = static_cast<MyRadioButton *>(Window->getSubclassPointer());
                            if (pSubclass)
                                pSubclass->refresh(Window->hWnd);
                            break;
                        }
                        }
                    }
                }
            }

            // Update application border brush.
            g_pUIElements->pointers.pCurrentBorderBrush = &g_pUIElements->colors.borderActive.getHBRUSH();
            if (g_IsWindows11BorderAttributeSupported)
            {
                COLORREF border_color = g_pUIElements->colors.borderActive.getCOLORREF();
                HRESULT hr = DwmSetWindowAttribute(g_hWnd, DWMWA_BORDER_COLOR, &border_color, sizeof(border_color));
                if (FAILED(hr))
                {
                    std::wstring error_message = L"Failed to set the window border attribute.";
                    std::wstring message = L"";
                    message.append(L"Error occurred!\n");
                    message.append(L"Failed to update the container.\n");
                    message.append(L"Error Message: " + error_message + L"\n");
                    message.append(L"CLASS: \n");
                    message.append(L"FUNC: UpdateContainers()");
                    MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
                }
            }

            // Redraw the entire application.
            RedrawWindow(g_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

            // Set application to active.
            SetActiveWindow(g_hWnd);
        }

        /**
         * @brief Set or update the application theme.
         *
         * @param Theme       The theme name.
         * @param isFirstCall Specifies whether it is the first time setting the application theme.
         *
         * @return Returns true if the application theme is successfully set. Otherwise, false.
         */
        bool SetAppTheme(MyTheme Theme = MyTheme::Light, bool isFirstCall = false)
        {
            std::wstring ThemeStr = L"";

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

                g_pUIElements->images.updateNonClientButtonImages(*g_pUIElements->images.pCrossBlack,  // Close button default image.
                                                                  *g_pUIElements->images.pCrossWhite,  // Close button hover image.
                                                                  *g_pUIElements->images.pCrossWhite,  // Close button down image.
                                                                  *g_pUIElements->images.pMinusBlack,  // Minimize button default image.
                                                                  *g_pUIElements->images.pMinusBlack,  // Minimize button hover image.
                                                                  *g_pUIElements->images.pMinusBlack); // Minimize button down image.

                // Update standard button class.
                MyStandardButtonSharedPropertiesConfig MyStandardButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.standardButtonDefault,
                        &g_pUIElements->colors.standardButtonHover,
                        &g_pUIElements->colors.standardButtonDown,
                        &g_pUIElements->colors.standardButtonBorderDefault,
                        &g_pUIElements->colors.standardButtonBorderHover,
                        &g_pUIElements->colors.standardButtonBorderDown,
                        &g_pUIElements->colors.textActive,
                        &g_pUIElements->colors.textHighlight,
                        &g_pUIElements->colors.background,
                        &g_pUIElements->colors.focus,
                        &g_pUIElements->fonts.button};
                if (!MyStandardButton::setSharedProperties(MyStandardButtonSharedPropertiesConfig))
                    return false;

                // Update image button class.
                MyImageButtonSharedPropertiesConfig MyImageButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.focus // Image button focus color.
                    };
                if (!MyImageButton::setSharedProperties(MyImageButtonSharedPropertiesConfig))
                    return false;

                // Update radio button class.
                MyRadioButtonSharedPropertiesConfig MyRadioButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.radioButtonPrimaryDefault,
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
                        &g_pUIElements->fonts.button};
                if (!MyRadioButton::setSharedProperties(MyRadioButtonSharedPropertiesConfig))
                    return false;

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
                    return false;

                // Update global variables.
                g_CurrentAppTheme = MyTheme::Light;
                g_IsCurrentThemeWantScrollbarsVisible = true;
                ThemeStr = L"Light";
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

                g_pUIElements->images.updateNonClientButtonImages(*g_pUIElements->images.pCrossGrey,   // Close button default image.
                                                                  *g_pUIElements->images.pCrossWhite,  // Close button hover image.
                                                                  *g_pUIElements->images.pCrossWhite,  // Close button down image.
                                                                  *g_pUIElements->images.pMinusGrey,   // Minimize button default image.
                                                                  *g_pUIElements->images.pMinusWhite,  // Minimize button hover image.
                                                                  *g_pUIElements->images.pMinusWhite); // Minimize button down image.

                // Update standard button class.
                MyStandardButtonSharedPropertiesConfig MyStandardButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.standardButtonDefault,
                        &g_pUIElements->colors.standardButtonHover,
                        &g_pUIElements->colors.standardButtonDown,
                        &g_pUIElements->colors.standardButtonBorderDefault,
                        &g_pUIElements->colors.standardButtonBorderHover,
                        &g_pUIElements->colors.standardButtonBorderDown,
                        &g_pUIElements->colors.textActive,
                        &g_pUIElements->colors.textHighlight,
                        &g_pUIElements->colors.background,
                        &g_pUIElements->colors.focus,
                        &g_pUIElements->fonts.button};
                if (!MyStandardButton::setSharedProperties(MyStandardButtonSharedPropertiesConfig))
                    return false;

                // Update image button class.
                MyImageButtonSharedPropertiesConfig MyImageButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.focus // Image button focus color.
                    };
                if (!MyImageButton::setSharedProperties(MyImageButtonSharedPropertiesConfig))
                    return false;

                // Update radio button class.
                MyRadioButtonSharedPropertiesConfig MyRadioButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.radioButtonPrimaryDefault,
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
                        &g_pUIElements->fonts.button};
                if (!MyRadioButton::setSharedProperties(MyRadioButtonSharedPropertiesConfig))
                    return false;

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
                    return false;

                // Update global variables.
                g_CurrentAppTheme = MyTheme::Dark;
                g_IsCurrentThemeWantScrollbarsVisible = true;
                ThemeStr = L"Dark";
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

                g_pUIElements->images.updateNonClientButtonImages(*g_pUIElements->images.pCrossGrey,   // Close button default image.
                                                                  *g_pUIElements->images.pCrossWhite,  // Close button hover image.
                                                                  *g_pUIElements->images.pCrossWhite,  // Close button down image.
                                                                  *g_pUIElements->images.pMinusGrey,   // Minimize button default image.
                                                                  *g_pUIElements->images.pMinusWhite,  // Minimize button hover image.
                                                                  *g_pUIElements->images.pMinusWhite); // Minimize button down image.

                // Update standard button class.
                MyStandardButtonSharedPropertiesConfig MyStandardButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.standardButtonDefault,
                        &g_pUIElements->colors.standardButtonHover,
                        &g_pUIElements->colors.standardButtonDown,
                        &g_pUIElements->colors.standardButtonBorderDefault,
                        &g_pUIElements->colors.standardButtonBorderHover,
                        &g_pUIElements->colors.standardButtonBorderDown,
                        &g_pUIElements->colors.textActive,
                        &g_pUIElements->colors.textHighlight,
                        &g_pUIElements->colors.background,
                        &g_pUIElements->colors.focus,
                        &g_pUIElements->fonts.button};
                if (!MyStandardButton::setSharedProperties(MyStandardButtonSharedPropertiesConfig))
                    return false;

                // Update image button class.
                MyImageButtonSharedPropertiesConfig MyImageButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.focus // Image button focus color.
                    };
                if (!MyImageButton::setSharedProperties(MyImageButtonSharedPropertiesConfig))
                    return false;

                // Update radio button class.
                MyRadioButtonSharedPropertiesConfig MyRadioButtonSharedPropertiesConfig =
                    {
                        &g_pUIElements->colors.radioButtonPrimaryDefault,
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
                        &g_pUIElements->fonts.button};
                if (!MyRadioButton::setSharedProperties(MyRadioButtonSharedPropertiesConfig))
                    return false;

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
                    return false;

                // Update global variables.
                g_CurrentAppTheme = MyTheme::Monokai;
                g_IsCurrentThemeWantScrollbarsVisible = false;
                ThemeStr = L"Monokai";
                break;
            }
            }

            // Update application containers.
            if (!isFirstCall)
                UpdateContainers(g_CurrentAppTheme);

            if (!isFirstCall)
                WriteLogEx(L"Application theme changed: ", (L"\"" + ThemeStr + L"\"").c_str(), MyLogType::Info, 1);
            else
                WriteLogEx(L"Default application theme: ", (L"\"" + ThemeStr + L"\"").c_str(), MyLogType::Info, 1);
            return true;
        }
    }

    /**
     * @brief Functions related to window management.
     * @brief This namespace contains functions for initialization, deinitialization,
     * @brief creations and utility of the application windows.
     */
    namespace Window
    {
        /**
         * @brief Functions related to window and window component creation.
         */
        namespace Creation
        {
            /**
             * @brief Create a font object for the specified font handle (HFONT).
             *
             * @param hFont       Reference to the font handle.
             * @param FontName    Font name.
             * @param FontSize    Font size.
             * @param FontWeight  Font weight  (default: FW_DONTCARE).
             * @param FontQuality Font quality (default: DEFAULT_QUALITY).
             */
            void CreateMyFont(HFONT &hFont, std::wstring FontName, INT FontSize, INT FontWeight /* = FW_DONTCARE*/, DWORD FontQuality /* = DEFAULT_QUALITY*/)
            {
                hFont = CreateFontW(FontSize, 0, 0, 0x1,
                                    FontWeight, FALSE, FALSE, FALSE, ANSI_CHARSET,
                                    OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, FontQuality,
                                    DEFAULT_PITCH | FF_DONTCARE, (LPCWSTR)FontName.c_str());
            }

            /**
             * @brief Create a horizontal standalone scrollbar.
             *
             * @param hInstance         Handle to an instance.
             * @param hParent           Handle to the scrollbar Parent.
             * @param Identifier        Scrollbar id.
             * @param ScrollbarHeight   Scroll bar height.
             * @param ParentBorderWidth Parent border width (Specify only if the Parent's border is in the client area).
             * @param UseCustomXYW      Flag indicating whether to use custom scrollbar position (PosX, PosY) and width (ScrollbarWidth). If false, ParentBorderWidth will be used.
             * @param cPosX             Custom PosX (Horizontal relative position).
             * @param cPosY             Custom PosY (Vertical relative position).
             * @param ScrollbarWidth    Custom scrollbar width.
             *
             * @return Returns the window handle of the created scrollbar.
             */
            HWND CreateHorizontalScrollbar(HINSTANCE hInstance, HWND hParent, HMENU Identifier, INT ScrollbarHeight, UINT ParentBorderWidth = 0, bool UseCustomXYW = false, INT cPosX = 0, INT cPosY = 0, INT ScrollbarWidth = 0)
            {
                RECT RECT_Parent;
                if (!GetClientRect(hParent, &RECT_Parent))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to retrieve the window Parent client rect.\n\n");
                    error_message.append(L"NAMESPACE: nSol\n");
                    error_message.append(L"FUNC: CreateHorizontalScrollbar()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    return NULL;
                }

                INT PosX, PosY, Width, Height;
                if (!UseCustomXYW)
                {
                    PosX = RECT_Parent.left + ParentBorderWidth;
                    PosY = RECT_Parent.bottom - ParentBorderWidth - ScrollbarHeight;
                    Width = RECT_Parent.right - (ParentBorderWidth * 2);
                    Height = ScrollbarHeight;
                }
                else
                {
                    PosX = cPosX;
                    PosY = cPosY;
                    Width = ScrollbarWidth;
                    Height = ScrollbarHeight;
                }

                return (CreateWindowExW(NULL, L"SCROLLBAR", NULL,
                                        WS_CHILD | /*WS_VISIBLE |*/ SBS_VERT,
                                        PosX, PosY,
                                        Width, Height,
                                        hParent, Identifier, hInstance, NULL));
            }

            /**
             * @brief Create a vertical standalone scrollbar.
             *
             * @param hInstance         Handle to an instance.
             * @param hParent           Handle to the scrollbar Parent.
             * @param Identifier        Scrollbar id.
             * @param ScrollbarWidth    Scrollbar width.
             * @param PosY              Vertical position of the scrollbar.
             * @param ParentBorderWidth Parent border width (Specify only if the Parent's border is in the client area).
             * @param UseCustomXYH      Flag indicating whether to use custom scrollbar position (PosX, PosY) and height (ScrollbarHeight). If false, ParentBorderWidth will be used.
             * @param cPosX             Custom PosX (Horizontal relative position).
             * @param cPosY             Custom PosY (Vertical relative position).
             * @param ScrollbarHeight   Custom scrollbar height.
             *
             * @return Returns the window handle of the created scrollbar.
             */
            HWND CreateVerticalScrollbar(HINSTANCE hInstance, HWND hParent, HMENU Identifier, INT ScrollbarWidth, INT PosY = 0, UINT ParentBorderWidth = 0, bool UseCustomXYH = false, INT cPosX = 0, INT cPosY = 0, INT ScrollbarHeight = 0)
            {
                RECT RECT_Parent;
                if (!GetClientRect(hParent, &RECT_Parent))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to retrieve the window Parent client rect.\n\n");
                    error_message.append(L"NAMESPACE: nSol\n");
                    error_message.append(L"FUNC: CreateVerticalScrollbar()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    return NULL;
                }

                INT PosX, Width, Height;
                if (!UseCustomXYH)
                {
                    PosX = RECT_Parent.right - ScrollbarWidth - ParentBorderWidth;
                    Width = ScrollbarWidth;
                    Height = RECT_Parent.bottom - WINDOW_BORDER_DEFAULTWIDTH - PosY;
                }
                else
                {
                    PosX = cPosX;
                    PosY = cPosY;
                    Width = ScrollbarWidth;
                    Height = ScrollbarHeight;
                }

                return (CreateWindowExW(0, L"SCROLLBAR", NULL,
                                        WS_CHILD | /*WS_VISIBLE |*/ SBS_VERT,
                                        PosX, PosY,
                                        Width, Height,
                                        hParent, Identifier, hInstance, NULL));
            }

            /**
             * @brief Create an edit control with borders.
             * @brief This function creates an edit control by using two window handles: a static hwnd and an actual edit hwnd.
             * @brief The static hwnd is used to handle the WM_CTLCOLORSTATIC message for drawing the border of the edit control.
             *
             * @param PosX            Horizontal relative position of the edit control.
             * @param PosY            Vertical relative position of the edit control.
             * @param Width           Width of the edit control.
             * @param Height          Height of the edit control.
             * @param Parent          Parent window handle for the edit control.
             * @param hWnd_Static     Reference to the window handle that will hold the static hwnd.
             * @param hWnd_Edit       Reference to the window handle that will hold the edit hwnd.
             * @param Static_Styles   Window styles for the static hwnd (DEFAULT: WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT).
             * @param Edit_Styles     Window styles for the edit hwnd (DEFAULT: WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN).
             * @param Static_ID       Static hwnd id.
             * @param Edit_ID         Edit hwnd id.
             * @param Static_ExStyles Extended window styles for the static hwnd (DEFAULT: NULL).
             * @param Edit_ExStyles   Extended window styles for the edit hwnd (DEFAULT: NULL).
             *
             * @return Returns true if the edit control is successfully created.
             */
            bool CreateEditControl(UINT PosX, UINT PosY, UINT Width, UINT Height,
                                   HWND &Parent, HWND &hWnd_Static, HWND &hWnd_Edit,
                                   LONG Static_Styles = WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                   LONG Edit_Styles = WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
                                   HMENU Static_ID = NULL, HMENU Edit_ID = NULL,
                                   LONG Static_ExStyles = NULL, LONG Edit_ExStyles = NULL)
            {
                hWnd_Static = CreateWindowExW(Static_ExStyles, L"STATIC", L"",
                                              Static_Styles,
                                              PosX,
                                              PosY, Width, Height, Parent, Static_ID, NULL, NULL);

                hWnd_Edit = CreateWindowExW(Edit_ExStyles, L"EDIT", L"",
                                            Edit_Styles,
                                            PosX + 2,
                                            PosY + 2,
                                            Width - 4, Height - 4, Parent, Edit_ID, NULL, NULL);

                if (!hWnd_Static || !hWnd_Edit)
                    return false;
                return true;
            }
        }

        /**
         * @brief Functions related to WinAPI utility.
         */
        namespace Utility
        {
            /**
             * @brief Remove a specific window style from a window.
             *
             * @param hWnd  Handle to the target window (HWND).
             * @param Style The window style to be removed.
             */
            void RemoveWindowStyle(HWND hWnd, LONG Style)
            {
                DWORD dwStyle = GetClassLongW(hWnd, GCL_STYLE);
                dwStyle &= ~Style;
                SetClassLongW(hWnd, GCL_STYLE, dwStyle);
            }

            /**
             * @brief Callback function for setting the font on child windows.
             *
             * @param hWnd_Child Handle to the child window.
             * @param hFont      Font handle passed as a LPARAM.
             *
             * @return Returns true if the font handle is valid and the font is set successfully on the child window.
             */
            bool CALLBACK SetFontOnChild(HWND hWnd_Child, LPARAM hFont)
            {
                if (!reinterpret_cast<HFONT>(hFont))
                    return false;

                SendMessageW(hWnd_Child, WM_SETFONT, hFont, TRUE);

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
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to retrieve the window placement struct.\n\n");
                    error_message.append(L"NAMESPACE: nApp::Window::Utility\n");
                    error_message.append(L"FUNC: IsWindowMaximized()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
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

                auto monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
                if (!monitor)
                    return;

                MONITORINFO monitor_info{};
                monitor_info.cbSize = sizeof(monitor_info);
                if (!GetMonitorInfoW(monitor, &monitor_info))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to retrieve the monitor info.\n\n");
                    error_message.append(L"NAMESPACE: nApp::Window::Utility\n");
                    error_message.append(L"FUNC: AdjustMaximizedClientRect()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
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
                // Retrieve the current windows version.
                auto module_ntdll = GetModuleHandleW(L"ntdll.dll");
                auto rtl_get_version_number = reinterpret_cast<void(WINAPI *)(LPDWORD major, LPDWORD minor, LPDWORD build)>(GetProcAddress(module_ntdll, "RtlGetNtVersionNumbers"));
                if (!rtl_get_version_number)
                {
                    WriteLog(L"Failed to retrieve the windows version.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }
                rtl_get_version_number(&g_WindowsMajorVersion, &g_WindowsMinorVersion, &g_WindowsBuildNumber);
                g_WindowsBuildNumber &= ~0xF0000000;
                WriteLog(L"Windows version: ", (L"\"" + std::to_wstring(g_WindowsMajorVersion) + L"." + std::to_wstring(g_WindowsMinorVersion) + L"." + std::to_wstring(g_WindowsBuildNumber) + L" Build " + std::to_wstring(g_WindowsBuildNumber) + L"\"").c_str(), MyLogType::Debug);
                if (g_WindowsBuildNumber >= 22000)
                    g_IsWindows11BorderAttributeSupported = true;

                // Initialize DarkMode API.
                WriteLog(L"Initializing DarkMode API ...", L"", MyLogType::Debug);
                if (!nApp::API::InitDarkModeAPI(hWnd))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to initialize DarkMode API.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitBegin()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to initialize DarkMode API.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }
                else
                    WriteLog(L"DarkMode API initialized.", L"", MyLogType::Debug);

                // Initialize GDI Animation & GDI+ APIs.
                WriteLog(L"Initializing GDI Animation & GDI+ APIs ...", L"", MyLogType::Debug);
                if (!nApp::API::InitGraphicAPI(&g_APIGDIToken, &g_APIGDIStartupInput))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to initialize GDI Animation & GDI+ APIs.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitBegin()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to initialize GDI Animation & GDI+ APIs.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }
                else
                    WriteLog(L"GDI Animation & GDI+ APIs initialized.", L"", MyLogType::Debug);

                // Initialize the Windows Animation Manager API.
                WriteLog(L"Initializing Windows Animation Manager API ...", L"", MyLogType::Debug);
                if (!nApp::API::InitWindowsAnimationManager())
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to initialize Windows Animation Manager API.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitBegin()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to initialize Windows Animation Manager API.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }
                else
                    WriteLog(L"Windows Animation Manager API initialized.", L"", MyLogType::Debug);

                // Initialize animation subclass classes.
                MyStandardButtonInitializeConfig MyStandardButtonInitializeConfig =
                    {
                        &g_KeyToggleENTER,
                        &g_pAnimationManager,
                        &g_pAnimationTimer,
                        &g_pTransitionLibrary};
                MyImageButtonInitializeConfig MyImageButtonInitializeConfig =
                    {
                        &g_KeyToggleENTER,
                        &g_pAnimationManager,
                        &g_pAnimationTimer,
                        &g_pTransitionLibrary};
                MyRadioButtonInitializeConfig MyRadioButtonInitializeConfig =
                    {
                        &g_KeyToggleENTER,
                        &g_pAnimationManager,
                        &g_pAnimationTimer,
                        &g_pTransitionLibrary};
                MyEditInitializeConfig MyEdit_InitializeConfig =
                    {
                        &g_KeyToggleENTER,
                        &g_hWnd};
                if (!MyStandardButton::initialize(MyStandardButtonInitializeConfig) ||
                    !MyImageButton::initialize(MyImageButtonInitializeConfig) ||
                    !MyRadioButton::initialize(MyRadioButtonInitializeConfig) ||
                    !MyEdit::initialize(MyEdit_InitializeConfig))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to initialize animation subclass classes.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitBegin()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to initialize animation subclass classes.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);

                    return false;
                }
                else
                    WriteLog(L"Animation subclass classes initialized.", L"", MyLogType::Debug);

                // Extends the window frames into the client area (enabling the window drop-shadow effect).
                MARGINS borders = {1, 1, 1, 1};
                HRESULT hr = DwmExtendFrameIntoClientArea(hWnd, &borders);
                if (FAILED(hr))
                {
                    _com_error COMError(hr);
                    std::wstring error_message = L"";
                    std::wstring COMErrorMessage = COMError.ErrorMessage();
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to extend the window frames into the client area.\n");
                    error_message.append(L"Error message (COM): " + COMErrorMessage + L"\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitBegin()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to extend the window frames into the client area.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }
                else
                    WriteLog(L"Extended the window frames into client area (MARGINS: 1,1,1,1).", L"", MyLogType::Debug);

                hr = DwmEnableMMCSS(TRUE);
                if (FAILED(hr))
                    return false;

                // Set the minimum resolution for periodic timers to increase the precision of application timers (WM_TIMER).
                if (timeBeginPeriod(15) != TIMERR_NOERROR)
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to set minimum resolution for periodic timers.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitBegin()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to set minimum resolution for periodic timers.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }
                else
                    WriteLog(L"Minimum resolution for periodic timers sets (15).", L"", MyLogType::Debug);

                // Initialize global objects.
                WriteLog(L"Initializing global objects ...", L"", MyLogType::Debug);
                g_pUIElements = new UIElements();
                g_ContainerMainContent = new MyContainer(WINDOW_CONTAINER_DEFAULTPADDING, false, WINDOW_CONTAINER_DEFAULTPADDING, true);
                if (!g_pUIElements || !g_ContainerMainContent)
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to initialize global objects.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitBegin()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to initialize global objects.", L" [NAMESPACE: \"nApp::Window::Initialization\" | FUNC: \"InitBegin()\"]", MyLogType::Error);
                    return false;
                }
                else
                    WriteLog(L"Global objects initialized.", L"", MyLogType::Debug);

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
                if (!Theme::SetAppTheme(g_CurrentAppTheme, true))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to initialize application theme.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Initialization\"\n");
                    error_message.append(L"FUNC: InitTheme()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to initialize application theme", L" [NAMESPACE: \"nApp::Window::Initialization\" | \"FUNC: InitTheme()\"]", MyLogType::Error);
                    return false;
                }

                return true;
            }

            /**
             * @brief Create application window objects.
             *
             * @param hWnd      Handle to the main application window (HWND).
             * @param hInstance Handle to the main application instance (HINSTANCE).
             *
             * @return Returns true if all the window objects are successfully created. Otherwise, returns false.
             */
            bool InitControl(HWND hWnd, HINSTANCE hInstance)
            {
                /// Non-client windows:
                {
                    // Close button
                    MyWindow *pButton_Close = new MyWindow(true);
                    MyImageButtonNonSharedPropertiesConfig Button_Close_NonSharedPropertiesConfig =
                        {
                            &g_pUIElements->images.pNonClientCloseButtonDefault,
                            &g_pUIElements->images.pNonClientCloseButtonHover,
                            &g_pUIElements->images.pNonClientCloseButtonDown,
                            &g_pUIElements->colors.captionBackground,
                            &g_pUIElements->colors.closeButtonBackgroundOnHover,
                            &g_pUIElements->colors.closeButtonBackgroundOnDown,
                            0, 0, 20, 20, true, false, true};
                    if (!pButton_Close->createImageButton(hWnd, L"", false, true, Button_Close_NonSharedPropertiesConfig,
                                                          0, 0, 58, 37, (HMENU)IDC_NONCLIENT_CLOSE_BUTTON))
                        return false;
                    g_VectorNonClientWindows.push_back(pButton_Close);
                    g_pUIElements->miscs.hWndNonClientCloseButton = pButton_Close->hWnd;

                    // Minimize button
                    MyWindow *pButton_Minimize = new MyWindow(true);
                    MyImageButtonNonSharedPropertiesConfig Button_Minimize_NonSharedPropertiesConfig =
                        {
                            &g_pUIElements->images.pNonClientMinimizeButtonDefault,
                            &g_pUIElements->images.pNonClientMinimizeButtonHover,
                            &g_pUIElements->images.pNonClientMinimizeButtonDown,
                            &g_pUIElements->colors.captionBackground,
                            &g_pUIElements->colors.minimizeButtonBackgroundOnHover,
                            &g_pUIElements->colors.minimizeButtonBackgroundOnDown,
                            0, 0, 20, 20, true, false, true};
                    if (!pButton_Minimize->createImageButton(hWnd, L"", false, true, Button_Minimize_NonSharedPropertiesConfig,
                                                             0, 0, 58, 37, (HMENU)IDC_NONCLIENT_MINIMIZE_BUTTON))
                        return false;
                    g_VectorNonClientWindows.push_back(pButton_Minimize);
                    g_pUIElements->miscs.hWndNonClientMinimizeButton = pButton_Minimize->hWnd;

                    // Window title
                    size_t TextLength_AppTitle = static_cast<size_t>(GetWindowTextLengthW(hWnd)) + static_cast<size_t>(1);
                    WCHAR *TextBuffer_AppTitle = new WCHAR[TextLength_AppTitle];
                    GetWindowTextW(hWnd, TextBuffer_AppTitle, static_cast<INT>(TextLength_AppTitle));
                    MyWindow *pText_CaptionTitle = new MyWindow();
                    pText_CaptionTitle->hWnd = CreateWindowExW(NULL, L"STATIC", TextBuffer_AppTitle, WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                               WINDOW_BORDER_DEFAULTWIDTH + 10, WINDOW_BORDER_DEFAULTWIDTH + 7, 383, 23, hWnd, (HMENU)IDC_NONCLIENT_CAPTIONTITLE_STATIC, NULL, NULL);
                    g_VectorNonClientWindows.push_back(pText_CaptionTitle);
                    delete[] TextBuffer_AppTitle;
                }

                /// Client windows:

                // Container: MainContent
                {
                    // Create the container.
                    if (!g_ContainerMainContent->createContainerWindow(hWnd, false, &WindowProcedure_Container_MainContent,
                                                                        WINDOW_BORDER_DEFAULTWIDTH, WINDOW_BORDER_DEFAULTWIDTH + WINDOW_CAPTIONBAR_DEFAULTHEIGHT,
                                                                        0, 0, (HMENU)IDC_MAINCONTENT_CONTAINER))
                        return false;

                    MyWindow *pContainer_MainContent = g_ContainerMainContent->getContainerWindow();

                    // Create container contents.
                    {
                        // Static text (Sample buttons:)
                        MyWindow *pText_SampleButtons = new MyWindow(false);
                        pText_SampleButtons->hWnd = CreateWindowExW(NULL, L"STATIC", L"Sample buttons:",
                                                                    WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                    g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                    477, 26, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SAMPLEBUTTONS_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_SampleButtons, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pText_SampleButtons->hWnd); /// End container line 1.

                        // Standard button
                        MyWindow *pButton_Standard = new MyWindow(true);
                        if (!pButton_Standard->createStandardButton(pContainer_MainContent->hWnd, L"Standard", true, true,
                                                                    g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1), 130, 40, (HMENU)503))
                            return false;

                        g_ContainerMainContent->addWindow(pButton_Standard, true);

                        // Radio Test: First button
                        MyWindow *pButton_Radio_Test_First = new MyWindow(true);
                        if (!pButton_Radio_Test_First->createRadioButton(g_ContainerMainContent->pContainerWindow->hWnd, L"First", true, true,
                                                                         g_ContainerMainContent->getInitiateWindowPosition(0) + 140, g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                         86, 40, (HMENU)IDC_MAINCONTENT_RADIO_TEST_FIRST_BUTTON))
                            return false;
                        if (!SampleRadio.addRadioButton(pButton_Radio_Test_First))
                            return false;
                        g_ContainerMainContent->addWindow(pButton_Radio_Test_First, true);

                        // Radio Test: Second button
                        MyWindow *pButton_Radio_Test_Second = new MyWindow(true);
                        if (!pButton_Radio_Test_Second->createRadioButton(g_ContainerMainContent->pContainerWindow->hWnd, L"Second", true, true,
                                                                          g_ContainerMainContent->getInitiateWindowPosition(0) + 232, g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                          110, 40, (HMENU)IDC_MAINCONTENT_RADIO_TEST_SECOND_BUTTON))
                            return false;
                        if (!SampleRadio.addRadioButton(pButton_Radio_Test_Second))
                            return false;
                        g_ContainerMainContent->addWindow(pButton_Radio_Test_Second, true);

                        // Radio Test: Third button
                        MyWindow *pButton_Radio_Test_Third = new MyWindow(true);
                        if (!pButton_Radio_Test_Third->createRadioButton(g_ContainerMainContent->pContainerWindow->hWnd, L"Third", true, true,
                                                                         g_ContainerMainContent->getInitiateWindowPosition(0) + 346, g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                         92, 40, (HMENU)IDC_MAINCONTENT_RADIO_TEST_THIRD_BUTTON))
                            return false;
                        if (!SampleRadio.addRadioButton(pButton_Radio_Test_Third))
                            return false;
                        g_ContainerMainContent->addWindow(pButton_Radio_Test_Third, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pButton_Standard->hWnd); /// End container line 2.

                        // Static text (Sample editboxs:)
                        MyWindow *pText_SampleEditboxs = new MyWindow(false);
                        pText_SampleEditboxs->hWnd = CreateWindowExW(NULL, L"STATIC", L"Sample editboxs:",
                                                                     WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                     g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                     477, 26, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SAMPLEEDITBOXES_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_SampleEditboxs, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pText_SampleEditboxs->hWnd); /// End container line 3.

                        // Standard editbox (Static text note)
                        MyWindow *pText_StandardEditboxNote = new MyWindow(false);
                        pText_StandardEditboxNote->hWnd = CreateWindowExW(NULL, L"STATIC", L"(Normal)",
                                                                          WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                          g_ContainerMainContent->getInitiateWindowPosition(0) + 360, g_ContainerMainContent->getInitiateWindowPosition(1) + 5,
                                                                          80, 22, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_STANDARDEDITBOXNOTE_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_StandardEditboxNote, true);

                        // Standard editbox
                        MyEditNonSharedPropertiesConfig pEdit_StandardEditbox_NonSharedPropertiesConfig =
                            {
                                IDC_MAINCONTENT_STANDARDEDITBOX_STATIC,
                                IDC_MAINCONTENT_STANDARDEDITBOX_BUTTON,
                                false};
                        MyWindow *pEdit_StandardEditbox = new MyWindow(true);
                        if (!pEdit_StandardEditbox->createEditbox(g_ContainerMainContent->pContainerWindow->hWnd, L"", true, true, pEdit_StandardEditbox_NonSharedPropertiesConfig, MyEditboxType::Singleline,
                                                                  g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                  270, 33, (HMENU)IDC_MAINCONTENT_STANDARDEDITBOX_EDIT))
                            return false;
                        MyEdit *pSubclass_StandardEditbox = static_cast<MyEdit *>(pEdit_StandardEditbox->getSubclassPointer());
                        g_ContainerMainContent->addWindow(pEdit_StandardEditbox, true);

                        // Standard editbox submit button
                        MyWindow *pButton_StandardEditbox = new MyWindow(true);
                        if (!pButton_StandardEditbox->createStandardButton(pContainer_MainContent->hWnd, L"OK", true, true,
                                                                           g_ContainerMainContent->getInitiateWindowPosition(0) + 280, g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                           70, 33, (HMENU)IDC_MAINCONTENT_STANDARDEDITBOX_BUTTON))
                            return false;
                        g_ContainerMainContent->addWindow(pButton_StandardEditbox, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pSubclass_StandardEditbox->getStaticHandleRef()); /// End container line 4.

                        // Password editbox (Static text note)
                        MyWindow *pText_PasswordEditboxNote = new MyWindow(false);
                        pText_PasswordEditboxNote->hWnd = CreateWindowExW(NULL, L"STATIC", L"(Password)",
                                                                          WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                          g_ContainerMainContent->getInitiateWindowPosition(0) + 360, g_ContainerMainContent->getInitiateWindowPosition(1) + 5,
                                                                          95, 22, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_PASSWORDEDITBOXNOTE_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_PasswordEditboxNote, true);

                        // Password editbox
                        MyEditNonSharedPropertiesConfig pEdit_PasswordEditbox_NonSharedPropertiesConfig =
                            {
                                IDC_MAINCONTENT_PASSWORDEDITBOX_STATIC,
                                IDC_MAINCONTENT_PASSWORDEDITBOX_BUTTON,
                                false};
                        MyWindow *pEdit_PasswordEditbox = new MyWindow(true);
                        if (!pEdit_PasswordEditbox->createEditbox(g_ContainerMainContent->pContainerWindow->hWnd, L"", true, true, pEdit_PasswordEditbox_NonSharedPropertiesConfig, MyEditboxType::SinglelinePassword,
                                                                  g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1), 270, 33, (HMENU)IDC_MAINCONTENT_PASSWORDEDITBOX_EDIT))
                            return false;
                        g_ContainerMainContent->addWindow(pEdit_PasswordEditbox, true);
                        MyEdit *pSubclass_PasswordEditbox = static_cast<MyEdit *>(pEdit_PasswordEditbox->getSubclassPointer());

                        // Password editbox submit button
                        MyWindow *pButton_PasswordEditbox = new MyWindow(true);
                        if (!pButton_PasswordEditbox->createStandardButton(pContainer_MainContent->hWnd, L"OK", true, true,
                                                                           g_ContainerMainContent->getInitiateWindowPosition(0) + 280, g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                           70, 33, (HMENU)IDC_MAINCONTENT_PASSWORDEDITBOX_BUTTON))
                            return false;
                        g_ContainerMainContent->addWindow(pButton_PasswordEditbox, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pSubclass_PasswordEditbox->getStaticHandleRef()); /// End container line 5.

                        // Multiline editbox (Static text note)
                        MyWindow *pText_MultilineEditboxNote = new MyWindow(false);
                        pText_MultilineEditboxNote->hWnd = CreateWindowExW(NULL, L"STATIC", L"(Multiline)",
                                                                           WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                           g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                           95, 22, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_MULTILINEEDITBOXNOTE_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_MultilineEditboxNote, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pText_MultilineEditboxNote->hWnd); /// End container line 6.

                        // Multiline editbox
                        MyEditNonSharedPropertiesConfig pEdit_MultilineEditbox_NonSharedPropertiesConfig =
                            {
                                IDC_MAINCONTENT_MULTILINEEDITBOX_STATIC,
                                NULL,
                                false};
                        MyWindow *pEdit_MultilineEditbox = new MyWindow(true);
                        if (!pEdit_MultilineEditbox->createEditbox(g_ContainerMainContent->pContainerWindow->hWnd, L"", true, true, pEdit_MultilineEditbox_NonSharedPropertiesConfig, MyEditboxType::Multiline,
                                                                   g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1), 478, 200, (HMENU)IDC_MAINCONTENT_MULTILINEEDITBOX_EDIT))
                            return false;
                        g_ContainerMainContent->addWindow(pEdit_MultilineEditbox, true);
                        MyEdit *pSubclass_MultilineEditbox = static_cast<MyEdit *>(pEdit_MultilineEditbox->getSubclassPointer());

                        g_ContainerMainContent->registerNewHorizontalLine(pSubclass_MultilineEditbox->getStaticHandleRef()); /// End container line 7.

                        // Multiline editbox clear button
                        MyWindow *pButton_MultilineEditbox = new MyWindow(true);
                        if (!pButton_MultilineEditbox->createStandardButton(pContainer_MainContent->hWnd, L"Clear", true, true,
                                                                            g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1) - 5,
                                                                            478, 33, (HMENU)IDC_MAINCONTENT_MULTILINEEDITBOX_BUTTON))
                            return false;

                        g_ContainerMainContent->addWindow(pButton_MultilineEditbox, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pButton_MultilineEditbox->hWnd); /// End container line 8.

                        // Static text (Sample comboboxs:)
                        MyWindow *pText_SampleComboboxes = new MyWindow(false);
                        pText_SampleComboboxes->hWnd = CreateWindowExW(NULL, L"STATIC", L"Sample comboboxs:",
                                                                       WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                       g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                       477, 26, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SAMPLECOMBOBOXES_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_SampleComboboxes, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pText_SampleComboboxes->hWnd); /// End container line 9.

                        // Select theme combobox (Static text note)
                        MyWindow *pText_SelectThemeComboboxNote = new MyWindow(false);
                        pText_SelectThemeComboboxNote->hWnd = CreateWindowExW(NULL, L"STATIC", L"(Select theme)",
                                                                              WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                                              g_ContainerMainContent->getInitiateWindowPosition(0) + 130, g_ContainerMainContent->getInitiateWindowPosition(1) + 9,
                                                                              127, 22, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_SELECTTHEMENOTE_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_SelectThemeComboboxNote, true);

                        // Select theme combobox
                        MyWindow *pCombobox_SelectThemeCombobox = new MyWindow(true);
                        if (!pCombobox_SelectThemeCombobox->createDDLCombobox(g_ContainerMainContent->pContainerWindow->hWnd, true, true,
                                                                              g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1),
                                                                              120, 40, (HMENU)IDC_MAINCONTENT_SELECTTHEME_COMBOBOX))
                            return false;
                        g_ContainerMainContent->addWindow(pCombobox_SelectThemeCombobox, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pCombobox_SelectThemeCombobox->hWnd); /// End container line 10.

                        // Initialize combobox items (mwCombobox_SelectThemeCombobox).
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
                                SendMessageW(pCombobox_SelectThemeCombobox->hWnd, CB_ADDSTRING, 0, (LPARAM)text_buffer);
                            }
                            // Set default selected item.
                            switch (g_CurrentAppTheme)
                            {
                            case MyTheme::Light:
                                SendMessageW(pCombobox_SelectThemeCombobox->hWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
                                break;

                            case MyTheme::Dark:
                                SendMessageW(pCombobox_SelectThemeCombobox->hWnd, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
                                break;

                            case MyTheme::Monokai:
                                SendMessageW(pCombobox_SelectThemeCombobox->hWnd, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
                                break;
                            }
                        }

                        // Static text (Test zone:)
                        MyWindow *pText_TestZone = new MyWindow(false);
                        pText_TestZone->hWnd = CreateWindowExW(NULL, L"STATIC", L"Test zone:",
                                                               WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_LEFT,
                                                               g_ContainerMainContent->getInitiateWindowPosition(0), g_ContainerMainContent->getInitiateWindowPosition(1),
                                                               477, 26, g_ContainerMainContent->pContainerWindow->hWnd, (HMENU)IDC_MAINCONTENT_TESTZONE_STATIC, NULL, NULL);
                        g_ContainerMainContent->addWindow(pText_TestZone, true);

                        g_ContainerMainContent->registerNewHorizontalLine(pText_TestZone->hWnd); /// End container line 11.

                        // Create container vertical scrollbar.
                        if (!g_ContainerMainContent->createVerticalScrollbarWindow(hWnd, false, 0, 0, WINDOW_SCROLLBAR_DEFAULTWIDTH, 0, (HMENU)IDC_MAINCONTENT_CONTAINER_SCROLLBAR))
                            return false;

                        // Set the scroll information for the container vertical scrollbar.
                        if (!g_ContainerMainContent->setVerticalScrollInformations(false))
                            return false;
                    }
                }

                // Invalidate the containers.
                if (!g_ContainerMainContent->invalidateCheck())
                    return false; // continue here

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
                // Set the default font for all existing window objects.
                // EnumChildWindows(hWnd, (WNDENUMPROC)nApp::Window::Utility::SetFontOnChild, (LPARAM)g_pUIElements->fonts.Default.GetHFORef());

                // Set font for non-client window objects.
                SendMessageW(MyWindow::findWindowInVectorP(g_VectorNonClientWindows, IDC_NONCLIENT_CAPTIONTITLE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.caption.getHFONT(), TRUE);

                // Set font for client window objects.
                {
                    // Static texts (Heading).
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SAMPLEBUTTONS_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.heading.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SAMPLEEDITBOXES_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.heading.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SAMPLECOMBOBOXES_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.heading.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_TESTZONE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.heading.getHFONT(), TRUE);

                    // Static texts (Note).
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_STANDARDEDITBOXNOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_PASSWORDEDITBOXNOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_MULTILINEEDITBOXNOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_SELECTTHEMENOTE_STATIC), WM_SETFONT, (WPARAM)g_pUIElements->fonts.note.getHFONT(), TRUE);

                    // Editboxes.
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_STANDARDEDITBOX_EDIT), WM_SETFONT, (WPARAM)g_pUIElements->fonts.editbox.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_PASSWORDEDITBOX_EDIT), WM_SETFONT, (WPARAM)g_pUIElements->fonts.editbox.getHFONT(), TRUE);
                    SendMessageW(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_MULTILINEEDITBOX_EDIT), WM_SETFONT, (WPARAM)g_pUIElements->fonts.editbox.getHFONT(), TRUE);
                }

                // Modify standard class (BUTTON) style.
                nApp::Window::Utility::RemoveWindowStyle(g_ContainerMainContent->findMyWindowByID(IDC_MAINCONTENT_STANDARDBUTTON_BUTTON), CS_DBLCLKS);

                // Update class name for standard controls in containers based on the application theme.
                Theme::UpdateContainerThemeClass(g_CurrentAppTheme, g_ContainerMainContent);

                return true;
            }

            /**
             * @brief Sets the main window and essential controls to be visible, indicating that the application is ready.
             *
             * @return Returns true if the visibility of the main window and controls is successfully set. Otherwise, returns false.
             */
            bool OnReady()
            {
                ShowWindow(g_hWnd, SW_NORMAL); // Show main window.
                for (const auto &window : g_VectorNonClientWindows)
                {
                    ShowWindow(window->hWnd, SW_NORMAL);
                }                                                                       // Show non-client controls.
                ShowWindow(g_ContainerMainContent->pContainerWindow->hWnd, SW_NORMAL); // Show container (MainContent).

                if (!IsWindowVisible(g_hWnd))
                    return false;

                // Indicate that the application is ready.
                g_IsWindowReady = true;
                WriteLogEx(L"Application is ready.", L"", MyLogType::Info, 1);

                // Update container scrollbar visiblity.
                if (!g_ContainerMainContent->updateScrollbarsVisibility(g_IsCurrentThemeWantScrollbarsVisible))
                    return false;

                // Set the window active check timer to make sure the main window drop-shadow effects are drawn correctly.
                SetTimer(g_hWnd, IDT_ACTIVE_CHECK, 50, NULL);

                return true;
            }
        }

        /**
         * @brief Functions related to application window deinitialization processes.
         * @brief This namespace contains functions that handle the deinitialization of the application.
         */
        namespace Deinitialization
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
                    _com_error COMError(hr);
                    std::wstring error_message = L"";
                    std::wstring COMErrorMessage = COMError.ErrorMessage();
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to reverse the extended window frames.\n");
                    error_message.append(L"Error message (COM): " + COMErrorMessage + L"\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Deinitialization\"\n");
                    error_message.append(L"FUNC: OnExit()");
                    MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to reverse the extended window frames", L" [NAMESPACE: \"nApp::Window::Deinitialization\" | FUNC: \"OnExit()\"]", MyLogType::Error);
                }
                else
                    WriteLog(L"Reversed the extended window frames (MARGINS: 0,0,0,0).", L"", MyLogType::Debug);
            }

            /**
             * @brief Perform necessary deallocations and uninitialize global objects, subclasses, and APIs to properly conclude the application.
             */
            void OnDestroy()
            {
                // Destroy application containers.
                delete g_ContainerMainContent;
                WriteLog(L"Container(s) destroyed.", L"", MyLogType::Debug);

                // Destroy non-client window objects.
                {
                    const auto TotalWindows = g_VectorNonClientWindows.size();
                    UINT SuccessCount = 0;
                    for (auto &window : g_VectorNonClientWindows)
                    {
                        if (window)
                        {
                            delete window;
                            window = nullptr;
                        }
                        SuccessCount++;
                    }
                    WriteLog((std::to_wstring(SuccessCount) + L"/" + std::to_wstring(TotalWindows) + L" non-client window object(s) destroyed.").c_str(), L"", MyLogType::Debug);
                    g_VectorNonClientWindows.clear();
                }

                // Destroy global objects.
                delete g_pUIElements;
                WriteLog(L"Global object(s) destroyed.", L"", MyLogType::Debug);

                // Uninitialize APIs.
                if (!nApp::API::UninitWindowAnimationManager())
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to uninitialize Windows Animation Manager API.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Deinitialization\"\n");
                    error_message.append(L"FUNC: OnDestroy()");
                    if (g_AppLogLevel >= 2)
                        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to uninitialize Windows Animation Manager API.", L" [NAMESPACE: \"nApp::Window::Deinitialization\" | FUNC: \"OnDestroy()\"]", MyLogType::Error);
                }
                else
                    WriteLog(L"Windows Animation Manager API uninitialized.", L"", MyLogType::Debug);
                if (!nApp::API::UninitGraphicAPI(g_APIGDIToken))
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to uninitialize GDI Animation & GDI+ APIs.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Deinitialization\"\n");
                    error_message.append(L"FUNC: OnDestroy()");
                    if (g_AppLogLevel >= 2)
                        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to uninitialize GDI Animation & GDI+ APIs.", L" [NAMESPACE: \"nApp::Window::Deinitialization\" | FUNC: \"OnDestroy()\"]", MyLogType::Error);
                }
                else
                    WriteLog(L"GDI Animation & GDI+ APIs uninitialized.", L"", MyLogType::Debug);

                // Clears previously set minimum timer resolution.
                if (timeEndPeriod(15) != TIMERR_NOERROR)
                {
                    std::wstring error_message = L"";
                    error_message.append(L"Error occurred!\n");
                    error_message.append(L"Failed to clear previously set minimum timer resolution.\n\n");
                    error_message.append(L"NAMESPACE: \"nApp::Window::Deinitialization\"\n");
                    error_message.append(L"FUNC: OnDestroy()");
                    if (g_AppLogLevel >= 2)
                        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                    WriteLog(L"Failed to clear previously set minimum timer resolution.", L" [NAMESPACE: \"nApp::Window::Deinitialization\" | FUNC: \"OnDestroy()\"]", MyLogType::Error);
                }
                else
                    WriteLog(L"Previously set minimum timer resolution cleared.", L"", MyLogType::Debug);
            }
        }
    }
}

#endif // FUNC_H