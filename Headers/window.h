/**
 * @file window.h
 * @brief Header file containing user-defined types that related to windows.
 */

#ifndef WINDOW_H
#define WINDOW_H

/****************
 * Window enums *
 ****************/

enum class MyWindowType
{
    Undefined,
    Custom,
    StandardButton,
    ImageButton,
    RadioButton,
    Editbox,
    DDLCombobox,
    VerticalScrollbar
};
enum class MyEditboxType
{
    Singleline,
    SinglelinePassword,
    Multiline
};

/***********************************
 * Window configuration structures *
 ***********************************/

/**
 * @brief Configuration structure for creating a standard button window.
 */
struct MyStandardButtonWindowConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param parent Specifies the parent of the window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param visibleByDefault Specifies whether the window is visible by default. (Optional)
     * @param useTabStop Specifies whether to use tab stop. (Optional)
     * @param windowText Specifies the window text. (Optional)
     */
    MyStandardButtonWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, bool useTabStop, std::wstring windowText);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    HWND parent = nullptr;
    INT posX = 0;
    INT posY = 0;
    INT width = 0;
    INT height = 0;
    HMENU id = nullptr;
    bool visibleByDefault = true;
    bool useTabStop = true;
    std::wstring windowText = L"";
};

/**
 * @brief Configuration structure for creating a image button window.
 */
struct MyImageButtonWindowConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param parent Specifies the parent of the window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param pSubclassConfig Specifies the pointer to the subclass configuration structure.
     * @param visibleByDefault Specifies whether the window is visible by default. (Optional)
     * @param useTabStop Specifies whether to use tab stop. (Optional)
     * @param windowText Specifies the window text. (Optional)
     */
    MyImageButtonWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, MyImageButtonSubclassConfig *pSubclassConfig, bool visibleByDefault, bool useTabStop, std::wstring windowText);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    HWND parent = nullptr;
    INT posX = 0;
    INT posY = 0;
    INT width = 0;
    INT height = 0;
    HMENU id = nullptr;
    MyImageButtonSubclassConfig *pSubclassConfig = nullptr;
    bool visibleByDefault = true;
    bool useTabStop = true;
    std::wstring windowText = L"";
};

/**
 * @brief Configuration structure for creating a radio button window.
 */
struct MyRadioButtonWindowConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param parent Specifies the parent of the window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param visibleByDefault Specifies whether the window is visible by default. (Optional)
     * @param useTabStop Specifies whether to use tab stop. (Optional)
     * @param windowText Specifies the window text. (Optional)
     */
    MyRadioButtonWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, bool useTabStop, std::wstring windowText);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    HWND parent = nullptr;
    INT posX = 0;
    INT posY = 0;
    INT width = 0;
    INT height = 0;
    HMENU id = nullptr;
    bool visibleByDefault = true;
    bool useTabStop = true;
    std::wstring windowText = L"";
};

/**
 * @brief Configuration structure for creating a edit window.
 */
struct MyEditboxWindowConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param parent Specifies the parent of the window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param editboxType Specifies the editbox type.
     * @param pSubclassConfig Specifies the pointer to the subclass configuration structure.
     * @param visibleByDefault Specifies whether the window is visible by default. (Optional)
     * @param useTabStop Specifies whether to use tab stop. (Optional)
     * @param windowText Specifies the window text. (Optional)
     */
    MyEditboxWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, MyEditboxType editboxType, MyEditboxSubclassConfig *pSubclassConfig, bool visibleByDefault, bool useTabStop, std::wstring windowText);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    HWND parent = nullptr;
    INT posX = 0;
    INT posY = 0;
    INT width = 0;
    INT height = 0;
    HMENU id = nullptr;
    MyEditboxType editboxType;
    MyEditboxSubclassConfig *pSubclassConfig = nullptr;
    bool visibleByDefault = true;
    bool useTabStop = true;
    std::wstring windowText = L"";
};

/**
 * @brief Configuration structure for creating a drop-down list combobox window.
 */
struct MyDDLComboboxWindowConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param parent Specifies the parent of the window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param visibleByDefault Specifies whether the window is visible by default. (Optional)
     * @param useTabStop Specifies whether to use tab stop. (Optional)
     */
    MyDDLComboboxWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, bool useTabStop);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    HWND parent = nullptr;
    INT posX = 0;
    INT posY = 0;
    INT width = 0;
    INT height = 0;
    HMENU id = nullptr;
    bool visibleByDefault = true;
    bool useTabStop = true;
};

/**
 * @brief Configuration structure for creating a vertical scrollbar window.
 */
struct MyVerticalScrollbarWindowConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param pContainer Specifies the scroll container window.
     * @param parent Specifies the parent of the window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param visibleByDefault Specifies whether the window is visible by default. (Optional)
     */
    MyVerticalScrollbarWindowConfig(MyContainer *pContainer, HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    MyContainer *pContainer = nullptr;
    HWND parent = nullptr;
    INT posX = 0;
    INT posY = 0;
    INT width = 0;
    INT height = 0;
    HMENU id = nullptr;
    bool visibleByDefault = true;
};

/**
 * @brief Configuration structure for creating a container window.
 */
struct MyContainerWindowConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param parent Specifies the parent of the window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param visibleByDefault Specifies whether the window is visible by default.
     * @param windowProcedure Specifies the window procedure. (Optional)
     */
    MyContainerWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, SUBCLASSPROC windowProcedure);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    HWND parent = nullptr;
    INT posX = 0;
    INT posY = 0;
    INT width = 0;
    INT height = 0;
    HMENU id = nullptr;
    bool visibleByDefault = true;
    SUBCLASSPROC windowProcedure = nullptr;
};

/******************
 * Window classes *
 ******************/

/**
 * @brief HWND wrapper with extended functionality. (RAII)
 * @note Window creation functions can be used to create predefined controls.
 */
class MyWindow
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    MyWindow();

    /**
     * @brief Destructor.
     */
    ~MyWindow();

public:
    // [UTILITY FUNCTIONS]

    /**
     * @brief Get the handle to the window.
     * @note If the window doesn't exists, return nullptr.
     * @return Returns the handle to the window.
     */
    HWND hWnd();

    /**
     * @brief Get the window type.
     * @return Returns the window type.
     */
    MyWindowType type();

    /**
     * @brief Get the pointer (reference) that point to the associated data.
     * @note If the window is created using predefined window creation functions (not including custom window creation function), this pointer point to the window subclass object.
     * @note For custom windows `MyWindowType::Custom`, this pointer can be used to hold additional data but should be set to nullptr before the window is destroyed.
     * @return Returns the pointer (reference) that point to the associated data.
     */
    void *&data();

    /**
     * @brief Refresh the window appearance.
     * @note This has no effect on custom windows `MyWindowType::Custom`.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    // [WINDOW CREATION FUNCTIONS]

    /**
     * @brief Create a custom window.
     * @param dwExStyle Extended window style.
     * @param lpClassName Window class name.
     * @param lpWindowName Window name.
     * @param dwStyle Window style.
     * @param X X.
     * @param Y Y.
     * @param nWidth  Width.
     * @param nHeight Height.
     * @param hWndParent Parent.
     * @param hMenu Menu/ID.
     * @param hInstance Instance module.
     * @param lpParam Pointer to a value to be passed to the window through the CREATESTRUCT structure.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createCustomWindow(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle,
                            int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

    /**
     * @brief Create a standard button window.
     * @param windowConfig Specifies the window configuration.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createStandardButton(MyStandardButtonWindowConfig &windowConfig);

    /**
     * @brief Create a image button window.
     * @param windowConfig Specifies the window configuration.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createImageButton(MyImageButtonWindowConfig &windowConfig);

    /**
     * @brief Create a radio button window.
     * @param windowConfig Specifies the window configuration.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createRadioButton(MyRadioButtonWindowConfig &windowConfig);

    /**
     * @brief Create a editbox window.
     * @param windowConfig Specifies the window configuration.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createEditbox(MyEditboxWindowConfig &windowConfig);

    /**
     * @brief Create a drop-down list combobox window.
     * @param windowConfig Specifies the window configuration.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDDLCombobox(MyDDLComboboxWindowConfig &windowConfig);

    /**
     * @brief Create a vertical scrollbar window.
     * @param windowConfig Specifies the window configuration.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createVerticalScrollbar(MyVerticalScrollbarWindowConfig &windowConfig);

public:
    inline static UINT64 totalInstances = 0; // Indicate the total number of MyWindow instances. [DEBUG]

private:
    // Pointer to the associated data.
    // If the window is created using predefined window creation functions (not including custom window creation function), this pointer point to the window subclass object.
    // For custom windows `MyWindowType::Custom`, this pointer can be used to hold additional data but should be set to nullptr before the window is destroyed.
    void *pData = nullptr;

    std::unique_ptr<HWND, HWNDDeleter> pWindow; // Window handle.
    MyWindowType windowType;                    // The window type.
    bool isWindowExists = false;                // Indicate whether the window exists.
};

/**
 * @brief Scrollable container window. (RAII)
 * @note Work in progress class, only support vertical scrollbar for now.
 */
class MyContainer
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    MyContainer();

    /**
     * @brief Destructor.
     */
    ~MyContainer();

    // [UTILITY FUNCTIONS]

    /**
     * @brief Get the pointer to the container window.
     * @note If the container window doesn't exists, return nullptr.
     * @return Returns the pointer to the container window.
     */
    MyWindow *container();

    /**
     * @brief Get the pointer to the container vertical scrollbar window.
     * @note If the vertical scrollbar window doesn't exists, return nullptr.
     * @return Returns the pointer to the container vertical scrollbar window.
     */
    MyWindow *verticalScrollbar();

    /**
     * @brief Refresh the container.
     * @param redrawContainerWindow Specifies whether to redraw the container window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh(bool redrawContainerWindow = false);

    /**
     * @brief Update the container window dimensions.
     * @note Automatically update the container scrollbar windows dimensions if exists.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the container window width.
     * @param height Specifies the container window height.
     * @param hideScrollbar Specifies whether to hide the scrollbar windows.
     * @param resetScrollPosition Specifies whether to reset the scroll position back to zero.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateContainerDimensions(INT posX, INT posY, INT width, INT height, bool hideScrollbar, bool resetScrollPosition);

    // [WINDOW FUNCTIONS]

    /**
     * @brief Add a window to the container.
     * @param pWindow Pointer to the window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool addWindow(std::shared_ptr<MyWindow> pWindow);

    /**
     * @brief Remove a window from the container.
     * @param windowID Specifies the ID of the window.
     * @return Returns true if the window is found and removed, false otherwise.
     */
    bool removeWindow(INT windowID);

    /**
     * @brief Find a window in the container.
     * @param windowID Specifies the ID of the window.
     * @return Returns the found window, nullptr otherwise.
     */
    std::shared_ptr<MyWindow> findWindow(INT windowID);

    // [SCROLLING FUNCTIONS]

    /**
     * @brief Scroll the container.
     * @param scrollAmount Specifies the scroll amount in pixels.
     * @param scrollAlternative Specifies whether to scroll the container using DeferWindowPos() method.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool scrollContainer(INT scrollAmount, bool scrollAlternative = false);

    /**
     * @brief Set the bottom padding value.
     * @param scrollBottomPadding Specifies the bottom padding value.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setScrollBottomPadding(UINT scrollBottomPadding);

    // [CREATION FUNCTIONS]

    /**
     * @brief Create the container window.
     * @param containerConfig Specifies the container configuration.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createContainerWindow(MyContainerWindowConfig &containerConfig);

    /**
     * @brief Create the vertical scrollbar window.
     * @param posX Specifies the relative x-posision.
     * @param posY Specifies the relative y-posision.
     * @param width Specifies the window width.
     * @param height Specifies the window height.
     * @param id Specifies the window id.
     * @param visibleByDefault Specifies whether the window is visible by default.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createVerticalScrollbarWindow(INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault);

private:
    // [SCROLLING FUNCTIONS]

    /**
     * @brief Get the maximum vertical scrolling position.
     * @note Padding included.
     * @return Returns the maximum vertical scrolling position.
     */
    INT getMaximumVerticalScrollingPosition();

    /**
     * @brief Update the scroll information for the container scrollbars.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateContainerScrollInformation();

public:
    inline static UINT64 totalInstances = 0;               // Indicate the total number of MyContainer instances. [DEBUG]
    inline static bool autoUpdateScrollInformation = true; // Specifies whether to automatically update the container scroll information after added/removed a window. [TESTING IN PROGRESS]
    bool enableSmoothScroll = true;                        // Specifies whether to use smooth scrolling for the container.

private:
    std::unique_ptr<MyWindow> pContainerWindow;         // Container window.
    std::unique_ptr<MyWindow> pVerticalScrollbarWindow; // Container vertical scrollbar window.
    bool isContainerWindowExists = false;               // Indicate whether the container window is exists.
    bool isVerticalScrollbarWindowExists = false;       // Indicate whether the container vertical scrollbar window is exists.
    std::map<INT, std::shared_ptr<MyWindow>> mWindows;  // Map thats hold the container child windows.
    UINT scrollBottomPadding = 0;                       // Container scroll bottom padding.
};

#endif // WINDOW_H