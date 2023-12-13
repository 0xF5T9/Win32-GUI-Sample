/**
 * @file app.h
 * @brief Header file containing user-defined types that encapsulate the application structure.
 */

#ifndef APP_H
#define APP_H

/**
 * @brief Singleton class that encapsulates the application hierarchy.
 * @note This class forms the overall structure of the application.
 */
class MyApp
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    MyApp();

    /**
     * @brief Destructor.
     */
    ~MyApp();

    // [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Check if the application is initialized.
     * @return Returns true if the applicatuon is initialized, false otherwise.
     */
    bool isInitialized() const;

    /**
     * @brief Initialize the application.
     * @param hInstance Handle to the application instance.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool initialize(HINSTANCE hInstance);

    /**
     * @brief Uninitialize the application.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool uninitialize();

    // [CONFIGURATION FILE FUNCTIONS]

    /**
     * @brief Load the application configuration file.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool loadConfigurationFile();

    /**
     * @brief Update an option in the configuration file.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateConfigurationFile(std::string optionName, std::string optionValue);

    // [PROCEDURE FUNCTIONS]
    // Functions that invoked in the application window procedure.

    /**
     * @brief This function is called within the WM_CREATE message of the application window procedure.
     * @note Application window controls (non-client and client) should be created during this stage.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool onCreate(HWND hWnd);

    /**
     * @brief This function is called after the application window is successfully created and ready to be visible.
     * @note Application window device-dependent resources should be created during this stage.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool onReady();

    /**
     * @brief This function is called within the WM_CLOSE message of the application window procedure.
     * @note Use this stage to stop all the in-progress application activities before destroying the application window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool onExit();

    /**
     * @brief This function is called within the WM_DESTROY message of the application window procedure.
     * @note All the resources and windows related to the application window should be released during this stage.
     * @note e.g.: child windows and their associated resources and objects, container windows, and non-client windows.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool onDestroy();

    // [THEME FUNCTIONS]
    // Functions that managing the application theme.

    /**
     * @brief Update the application parameters and resources to match the specified theme.
     * @param theme Specifies the theme name.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setAppTheme(MyTheme theme);

    /**
     * @brief Update the application container windows resources to match the specified theme.
     * @param theme Specifies the theme name.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateContainers(MyTheme theme);

    // [UTILITY FUNCTIONS]

    /**
     * @brief Add a container to the application window.
     * @param pContainerWindow Pointer to the container window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool addContainer(std::shared_ptr<MyContainer> pContainerWindow);

    /**
     * @brief Remove a container to the application window.
     * @param containerWindowID Specifies the ID of the container window.
     * @return Returns true if the container is found and removed, false otherwise.
     */
    bool removeContainer(INT containerWindowID);

    /**
     * @brief Find a container in the application window.
     * @param containerWindowID Specifies the ID of the container window.
     * @return Returns the found container, nullptr otherwise.
     */
    std::shared_ptr<MyContainer> findContainer(INT containerWindowID);

    // [APPLICATION WINDOW HELPER FUNCTIONS]

    /**
     * @brief Check if the window is maximized.
     * @note Source: https://github.com/melak47/BorderlessWindow/blob/master/BorderlessWindow/src/BorderlessWindow.cpp
     * @param hWnd Handle to the application window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool isWindowPlacementMaximize(HWND hWnd);

    /**
     * @brief Adjust the maximized client rect to fit the monitor resolution (Taskbar excluded).
     * @note This fixed the problem where the WS_POPUP window spilled over monitor edges when maximized.
     * @param window Handle to the application window.
     * @param rect Reference to the RECT struct. [OUT]
     */
    void adjustMaximizedClientRect(HWND window, RECT &rect);

public:
    // Application main variables.

    MyLogger logger;                                            // Instance of the application logger used for writing log records.
    HINSTANCE hInstance = nullptr;                              // Handle to the application instance.
    HWND hWnd = nullptr;                                        // Handle to the application window.
    std::unique_ptr<MyGraphicEngine> pGraphic;                  // Pointer to the graphic engine.
    std::unique_ptr<UIManager> pUIManager;                      // Pointer to the UI manager.
    std::vector<std::shared_ptr<MyWindow>> vNonClientWindows;   // Vector that holds pointers to the non-client windows.
    std::map<INT, std::shared_ptr<MyContainer>> mContainers;    // Map that holds pointers to the container windows.
    const LPCWSTR windowClassName = L"Win32GUISample";          // Application window class name.
    const std::wstring configurationFileName = L"settings.cfg"; // Configuration file name.

    // Application window parameters.

    MyTheme windowTheme = MyTheme::Monokai;           // Indicate the current window theme.
    int windowWidth = 527;                            // Indicate the current window width.
    int windowHeight = 566;                           // Indicate the current window height.
    DWORD windowsMajorVersion = 0;                    // Indicate the current window major version.
    DWORD windowsMinorVersion = 0;                    // Indicate the current window minor version.
    DWORD windowsBuildNumber = 0;                     // Indicate the current window build number.
    bool isAlwaysHideScrollbars = false;              // Indicate whether the current to always hide the scrollbars.
    bool isWindows11BorderAttributeSupported = false; // Indicate whether the current window version supports the Windows 11 border attribute.
    bool isWindowActive = false;                      // Indicate whether the current window is active.
    bool isWindowReady = false;                       // Indicate whether the current window is ready.
    bool isWindowResizing = false;                    // Indicate whether the current window is resizing.
    bool isWindowMinimized = false;                   // Indicate whether the current window is minimized.
    bool isWindowMaximized = false;                   // Indicate whether the current window is maximized.

private:
    inline static bool initialized = false;    // Indicate whether the application is initialized.
    inline static bool instanceExists = false; // Indicate whether the instance already exists.
};

#endif // APP_H