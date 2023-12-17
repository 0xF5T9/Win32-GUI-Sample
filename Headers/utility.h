/**
 * @file utility.h
 * @brief Header file containing user-defined types that related to utility.
 */

#ifndef UTILITY_H
#define UTILITY_H

/*****************
 * Utility enums *
 *****************/

enum class MyLogType
{
    Info,
    Warn,
    Error,
    Debug,
};

/***************************
 * Global helper functions *
 ***************************/

namespace MyUtility
{
    /**
     * @brief Show the useful informations of a window via message box.
     * @note Used for debugging purposes.
     * @note `<windows.h>`, `<string>`, `<memory>`
     * @param hWnd Handle to the window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool ShowWindowInfo(HWND hWnd);

    /**
     * @brief Show the last WinAPI error message via message box.
     * @note Used for debugging purposes.
     * @note `<windows.h>`, `<string>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool ShowLastWinAPIErrorMessage();

    /**
     * @brief Get the user desktop resolution.
     * @param horizontal Reference to the variable that will hold the horizontal value. [OUT]
     * @param vertical   Reference to the variable that will hold the vertical value. [OUT]
     * @note `<windows.h>`, `<string>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool GetDesktopResolution(INT &horizontal, INT &vertical);

    /**
     * @brief Get the name of the current user's preferred power plan.
     * @param preferredPowerPlanName Reference to the string variable that will hold the power plan name. [OUT]
     * @note `<windows.h>`, `<string>`, `<powrprof.h>(powrprof.lib)`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool GetUserPreferredPowerPlanName(std::wstring &preferredPowerPlanName);

    /**
     * @brief Get the class name of the target window.
     * @param hWnd Handle to the window.
     * @note `<windows.h>`, `<string>`, `<memory>`
     * @return Returns the class name of the target window, empty string otherwise.
     */
    std::string GetWindowClassNameString(const HWND &hWnd);

    /**
     * @brief Get the class name of the target window (as wide string).
     * @param hWnd Handle to the window.
     * @note `<windows.h>`, `<string>`, `<memory>`
     * @return Returns the class name of the target window, empty string otherwise.
     */
    std::wstring GetWindowClassNameWideString(const HWND &hWnd);

    /**
     * @brief Get the window text (window name) of the target window.
     * @param hWnd Handle to the window.
     * @note `<windows.h>`, `<string>`, `<memory>`
     * @return Returns the window text of the target window, empty string otherwise.
     */
    std::string GetWindowTextString(const HWND &hWnd);

    /**
     * @brief Get the window text (window name) of the target window (as wide string).
     * @param hWnd Handle to the window.
     * @note `<windows.h>`, `<string>`, `<memory>`
     * @return Returns the window text of the target window, empty string otherwise.
     */
    std::wstring GetWindowTextWideString(const HWND &hWnd);

    /**
     * @brief Get the relative postions of the specified window.
     * @param hWnd Handle to the window.
     * @param windowPosXRef Reference to the variable that will hold the x-postion. [OUT]
     * @param windowPosYRef Reference to the variable that will hold the y-postion. [OUT]
     * @note `<windows.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool GetWindowRelativePosition(HWND hWnd, INT &windowPosXRef, INT &windowPosYRef);

    /**
     * @brief Get the dimensions of the specified window.
     * @param hWnd Handle to the window.
     * @param windowWidthRef Reference to the variable that will hold the width value. [OUT]
     * @param windowHeightRef Reference to the variable that will hold the height value. [OUT]
     * @note `<windows.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool GetWindowDimensions(HWND hWnd, INT &windowWidthRef, INT &windowHeightRef);

    /**
     * @brief Get the scroll info of a scrollbar window.
     * @param hWnd Handle to the window.
     * @param scrollInfo Reference to the variable that will hold the scroll info value. [OUT]
     * @note `<windows.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool GetScrollInfo(HWND hWnd, SCROLLINFO &scrollInfo);

    /**
     * @brief Remove a specific window style from a window.
     * @param hWnd Handle to the window.
     * @param style The window style to be removed.
     * @note `<windows.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool RemoveWindowStyle(HWND hWnd, LONG style);

    /**
     * @brief Remove a specific window extended style from a window.
     * @param hWnd Handle to the window.
     * @param style The window extended style to be removed.
     * @note `<windows.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool RemoveWindowExStyle(HWND hWnd, LONG style);

    /**
     * @brief Remove a specific class style from a window class.
     * @note All windows using the window class are affected.
     * @param hWnd A handle to the window and, indirectly, the class to which the window belongs.
     * @param style The class style to be removed.
     * @note `<windows.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool RemoveWindowClassStyle(HWND hWnd, LONG style);

    /**
     * @brief Checks if a given word is contained within a string.
     * @param str The string to search within.
     * @param word The word to find within the string.
     * @param caseSensitive Specifies whether the search is case-sensitive or not.
     * @param checkBoundaries Specifies whether to check word boundaries.
     *                         If true, the word must be surrounded by word boundaries (space, punctuation, etc.) to be considered a match.
     *                         If false, the function will search for partial matches as well.
     * @note `<string>`, `<cwctype>`
     * @return Returns true if the string contains the whole word, false otherwise.
     */
    bool ContainsWord(const std::string &str, const std::string &word, bool caseSensitive = false, bool checkBoundaries = false);

    /**
     * @brief Checks if a given word is contained within a wide string.
     * @param str The string to search within.
     * @param word The word to find within the string.
     * @param caseSensitive Specifies whether the search is case-sensitive or not.
     * @param checkBoundaries Specifies whether to check word boundaries.
     *                         If true, the word must be surrounded by word boundaries (space, punctuation, etc.) to be considered a match.
     *                         If false, the function will search for partial matches as well.
     * @note `<string>`, `<cwctype>`
     * @return Returns true if the string contains the whole word, false otherwise.
     */
    bool ContainsWord(const std::wstring &str, const std::wstring &word, bool caseSensitive = false, bool checkBoundaries = false);

    /**
     * @brief Check if a string contains at least one digit.
     * @param str The string to search within.
     * @note `<string>`
     * @return Returns true if the string contains at least one digit, false otherwise.
     */
    bool ContainsDigit(const std::string &str);

    /**
     * @brief Check if a wide string contains at least one digit.
     * @param str The string to search within.
     * @note `<string>`
     * @return Returns true if the string contains at least one digit, false otherwise.
     */
    bool ContainsDigit(const std::wstring &str);

    /**
     * @brief Converts a string (std::string) to a wide string (std::wstring).
     * @param str The string to convert.
     * @note `<windows.h>`, `<string>`, `<memory>`
     * @return Returns the converted string.
     */
    std::wstring ConvertStringToWideString(const std::string &str);

    /**
     * @brief Converts a wide string (std::wstring) to a string (std::string).
     * @param str The string to convert.
     * @note `<windows.h>`, `<string>`, `<memory>`
     * @return Returns the converted string.
     */
    std::string ConvertWideStringToString(const std::wstring &wstr);

    /**
     * @brief Removes non-digit characters from a given string.
     * @param str The input string from which non-digit characters are to be removed.
     * @note `<string>`, `<regex>`
     * @return Returns the resulting string with non-digit characters removed.
     */
    std::string RemoveNonDigitFromString(const std::string &str);

    /**
     * @brief Removes non-digit characters from a given string.
     * @param str The input string from which non-digit characters are to be removed.
     * @note `<string>`, `<regex>`
     * @return Returns the resulting string with non-digit characters removed.
     */
    std::wstring RemoveNonDigitFromString(const std::wstring &str);

    /**
     * @brief Open a dialog to select item(s) or folder(s).
     * @param paths Specifies the reference to the string vector that will receive the file or folder path(s). [IN]
     * @param selectFolder Specifies whether to select folder(s) rather than file(s). (optional)
     * @param multiSelect Specifies whether to allow the user to select multiple items. (optional)
     * @note If no item(s) were selected, the function still returns true, and the given vector is unmodified.
     * @note `<windows.h>`, `<string>`, `<vector>`, `<shobjidl.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool OpenFileDialog(std::vector<std::wstring> &paths, bool selectFolder = false, bool multiSelect = false);

    /**
     * @brief Open a dialog to save an item.
     * @param path Specifies the reference to the string that will receive the target save path. [IN]
     * @param defaultFileName Specifies the default save file name. (optional)
     * @param pFilterInfo Specifies the pointer to the pair that contains filter information. (optional)
     * @note If no path was selected, the function still returns true, and the given string is unmodified.
     * @note `<windows.h>`, `<string>`, `<vector>`, `<shobjidl.h>`
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool SaveFileDialog(std::wstring &path, std::wstring defaultFileName = L"", std::pair<COMDLG_FILTERSPEC *, int> *pFilterInfo = nullptr);

    /**
     * @brief Callback functions that are to be used with 'EnumChildWindows()' function.
     */
    namespace Callback
    {
        /**
         * @brief Set font callback.
         * @param hWndChild Handle to the child window.
         * @param hFont HFONT passed as a LPARAM.
         * @note `<windows.h>`
         * @return Returns true if all the operations are successfully performed, false otherwise.
         */
        bool CALLBACK SetFontOnChild(HWND hWndChild, LPARAM hFont);
    }
};

/*******************
 * Utility classes *
 *******************/

/**
 * @brief Provides an efficient alternative to GetKeyState() for capturing keydown messages only once instead of continuously while holding the key.
 * @note This class helps to avoid continuous processing of keydown messages and is useful for scenarios where capturing the initial key press is sufficient.
 * @note Readmore: https://stackoverflow.com/questions/10790502/c-getkeystate-has-to-run-once
 */
class KeyToggle
{
public:
    // [CONSTRUCTOR/DESTRUCTOR FUNCTIONS]

    /**
     * @brief Constructor.
     * @param virtualKey The Virtual-Key Code associated with the key.
     */
    KeyToggle(int virtualKey);

    /**
     * @brief Bool operator.
     * @return Returns true if the key associated with this KeyToggle instance is currently pressed, false otherwise.
     */
    operator bool();

private:
    int virtualKey; // Virtual-Key Code. [https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes]
    bool isActive;  // Indicate whether the key is currently pressed or not.
};

/**
 * @brief Simple logging utility for handling application logs.
 */
class MyLogger
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Default constructor.
     * @note Use command line path as logging path by default.
     */
    MyLogger();

    /**
     * @brief Constructor 1.
     * @param path Specifies the custom logging path.
     */
    MyLogger(std::wstring path);

    // [LOGGING FUNCTIONS]

    /**
     * @brief Get the current log level.
     * @return Returns the current log level.
     */
    int getLogLevel();

    /**
     * @brief Set a new logging path.
     * @note Set to empty string to use command line path.
     * @param path Specifies the custom logging path.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setPath(std::wstring path = L"");

    /**
     * @brief Set a new log level.
     * @param logLevel Specifies the log level.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setLogLevel(int logLevel = 1);

    /**
     * @brief Set a new log file name.
     * @param logFileName Specifies the log file name.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setLogFileName(std::wstring logFileName = L"log.log");

    /**
     * @brief Write a log record.
     * @param description Specifies the log description.
     * @param extraDescription Specifies the log extra description. (optional)
     * @param logType Specifies the log type. (optional)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool writeLog(std::string description, std::string extraDescription = "", MyLogType logType = MyLogType::Info);

    /**
     * @brief Write a custom log record.
     * @note Log level is ignored.
     * @param description Specifies the log description.
     * @param includeTime Specifies whether to include the time string.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool writeLog(std::string description, bool includeTime);

private:
    // [HELPER FUNCTIONS]

    /**
     * @brief Get the current date and time as a formatted string.
     * @note Visit: http://en.cppreference.com/Width/cpp/chrono/c/strftime
     * @note for more information about date/time format.
     * @return Returns the current date and time string formatted as "%Y-%m-%d.%X", empty string otherwise.
     */
    std::string getCurrentDateTimeString();

private:
    std::filesystem::path path;
    std::wstring logFileName = L"log.log";
    int logLevel = 1;
};

/****************************************
 * Deleter structs (for smart pointers) *
 ****************************************/

// Deleter base struct:

struct Deleter
{
    Deleter();
    inline static MyLogger logger;
};

// WinAPI deleters:

struct HWNDDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(HWND *variable) const;
};
struct HBRUSHDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(HBRUSH *variable) const;
};
struct HFONTDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(HFONT *variable) const;
};
struct HRGNDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(HRGN *variable) const;
};

// WAM deleters:

struct IUIAnimationStoryboardDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(IUIAnimationStoryboard **variable) const;
};
struct IUIAnimationVariableDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(IUIAnimationVariable **variable) const;
};
struct IUIAnimationTransitionDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(IUIAnimationTransition **variable) const;
};

// Direct2D/DirectWrite deleters:

struct IWICBitmapSourceDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(IWICBitmapSource **variable) const;
};
struct IDWriteTextFormatDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(IDWriteTextFormat **variable) const;
};
struct ID2D1DCRenderTargetDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(ID2D1DCRenderTarget **variable) const;
};
struct ID2D1SolidColorBrushDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(ID2D1SolidColorBrush **variable) const;
};
struct ID2D1BitmapDeleter : public Deleter
{
    inline static bool debugMode = false;
    void operator()(ID2D1Bitmap **variable) const;
};

#endif // UTILITY_H