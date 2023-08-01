/**
 * @file my_winapi.h
 * @brief Header file containing classes that related to WinAPI.
 */

#pragma once

/**
 * @class MyWindow
 *
 * @brief Represents a window.
 * @brief Basically a warpped HWND with extra informations stored, such as my defined window type and pointer to the associated subclass object.
 *
 * Remarks:
 * - There are two variants of MyWindow instances: unmanaged and managed.
 *   The variant is determined at the time of declaration and cannot be changed thereafter.
 *
 * - A managed window variant must create the window control object using predefined creation functions (Start with "create").
 *
 * - A unmanaged window variant must create the window control object manually by access the window handle member directly.
 *
 * - Both window variants will automatically destroy their window object when the object is being destroyed (In other words: When the MyWindow instance's destructor is called).
 *   The managed window will automatically destroy any associated objects/resources (i.e: subclass object).
 *
 * - A MyWindow instance is considered valid when its window handle points to a valid window object.
 */
class MyWindow
{
public:
    /**
     * @brief Default constructor.
     *
     * @param isManaged Specifies whether the window is unmanaged or managed variant.
     */
    MyWindow(bool isManaged = false);

    /**
     * @brief Destructor.
     *
     * @note Destroy the window. If the window is managed, any associated objects/resources are also automatically destroyed.
     */
    ~MyWindow();

private:
    MyWindowType windowType; // The window type.
    bool isManaged;          // Indicate whether the window is a managed variant or not.
    void *pSubclass;         // Pointer to the associated subclass class object.

public:
    HWND hWnd; // Window handle.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Finds a window with a specific ID in the vector of MyWindow objects.
     *
     * @param windows  Vector of MyWindow objects to search in.
     * @param windowID The ID of the window to find.
     *
     * @return Returns the window handle (HWND) of the found window, or nullptr if not found.
     */
    static HWND findWindowInVector(const std::vector<MyWindow> &windows, INT windowID);

    /**
     * @brief Finds a window with a specific ID in the vector of MyWindow pointers.
     *
     * @param windows  Vector of MyWindow pointers to search in.
     * @param windowID The ID of the window to find.
     *
     * @return Returns the window handle (HWND) of the found window, or nullptr if not found.
     */
    static HWND findWindowInVectorP(const std::vector<MyWindow *> &windows, INT windowID);

    /**
     * @brief Finds a window with a specific ID in the vector of MyWindow pointers.
     *
     * @param windows  Vector of MyWindow pointers to search in.
     * @param windowID The ID of the window to find.
     *
     * @return Returns the window pointer (MyWindow *) of the found window, or nullptr if not found.
     */
    static MyWindow *findWindowInVectorPEx(const std::vector<MyWindow *> &windows, INT windowID);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the window is a managed variant.
     *
     * @return Returns true if the window is a managed variant, false otherwise.
     */
    bool isManagedWindow();

    /**
     * @brief Check if the window is valid.
     *
     * @note A MyWindow instance is considered valid when its window handle points to a valid window object.
     *
     * @return Returns true if the window is valid, false otherwise.
     */
    bool isValid();

    /**
     * @brief Get the window type.
     *
     * @note The window type enums are defined in "my_enums.h" header file.
     *
     * @return Returns the window type.
     */
    MyWindowType getWindowType();

public:
    /// [MANAGED WINDOW FUNCTIONS]

    /**
     * @brief Get the pointer to the window-associated subclass object.
     *
     * @return Returns the pointer to the window-associated subclass object, nullptr otherwise.
     */
    void *getSubclassPointer();

    /**
     * @brief Get the pointer (reference) to the window-associated subclass object.
     *
     * @return Returns the pointer (reference) to the window-associated subclass object, nullptr otherwise.
     */
    void *&getSubclassPointerRef();

    /**
     * @brief Create a standard button.
     *
     * @param hWndParent       Handle to the button parent.
     * @param windowText       The button text.
     * @param visibleByDefault Specifies whether the window is visible by default.
     * @param useTabStop       Specifies whether to use tab stop.
     * @param windowPosX       The button horizontal position.
     * @param windowPosY       The button vertical position.
     * @param windowWidth      The button width.
     * @param windowHeight     The button height.
     * @param windowID         The button ID.
     *
     * @return Returns true if the window was successfully created, false otherwise.
     */
    bool createStandardButton(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID);

    /**
     * @brief Create a image button.
     *
     * @param hWndParent                Handle to the button parent.
     * @param windowText                The button text.
     * @param visibleByDefault          Specifies whether the window is visible by default.
     * @param configNonSharedProperties Non-shared properties configuration structure.
     * @param useTabStop                Specifies whether to use tab stop.
     * @param windowPosX                The button horizontal position.
     * @param windowPosY                The button vertical position.
     * @param windowWidth               The button width.
     * @param windowHeight              The button height.
     * @param windowID                  The button ID.
     *
     * @return Returns true if the window was successfully created, false otherwise.
     */
    bool createImageButton(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, MyImageButtonNonSharedPropertiesConfig configNonSharedProperties, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID);

    /**
     * @brief Create a radio button.
     *
     * @param hWndParent       Handle to the button parent.
     * @param windowText       The button text.
     * @param visibleByDefault Specifies whether the window is visible by default.
     * @param useTabStop       Specifies whether to use tab stop.
     * @param windowPosX       The button horizontal position.
     * @param windowPosY       The button vertical position.
     * @param windowWidth      The button width.
     * @param windowHeight     The button height.
     * @param windowID         The button ID.
     *
     * @return Returns true if the window was successfully created, false otherwise.
     */
    bool createRadioButton(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID);

    /**
     * @brief Create a editbox.
     *
     * @param hWndParent                Handle to the editbox parent.
     * @param windowText                The editbox default text.
     * @param visibleByDefault          Specifies whether the window is visible by default.
     * @param useTabStop                Specifies whether to use tab stop.
     * @param configNonSharedProperties Non-shared properties configuration structure.
     * @param editboxType               The editbox type.
     * @param windowPosX                The editbox horizontal position.
     * @param windowPosY                The editbox vertical position.
     * @param windowWidth               The editbox width.
     * @param windowHeight              The editbox height.
     * @param windowID                  The editbox ID.
     */
    bool createEditbox(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, MyEditNonSharedPropertiesConfig configNonSharedProperties, MyEditboxType editboxType, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID);

    /**
     * @brief Create a drop-down list combobox.
     *
     * @param hWndParent       Handle to the combobox parent.
     * @param visibleByDefault Specifies whether the window is visible by default.
     * @param useTabStop       Specifies whether to use tab stop.
     * @param windowPosX       The combobox horizontal position.
     * @param windowPosY       The combobox vertical position.
     * @param windowWidth      The combobox width.
     * @param windowHeight     The combobox height.
     * @param windowID         The combobox ID.
     */
    bool createDDLCombobox(HWND hWndParent, bool visibleByDefault, bool useTabStop, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID);
};

/**
 * @class MyContainerScrollManager
 *
 * @brief This class provides functionality to manage the scroll informations for a container (MyContainer).
 *
 * @note This class is used by MyContainer class.
 * @note Only support vertical scrolling for now.
 * @note The class is a work in progress, and everything is subject to change as I learn more!
 */
class MyContainerScrollManager
{
private:
    /**
     * Notes to the future me in case of confusion.
     *
     * Note 1:
     * The total vertical spacing pixels in the container = (@verticalScrollTotalHorizontalLines - 1) * (@verticalScrollDefaultSpacingDistance)
     *
     * However, when a horizontal line uses a custom spacing value other than the default, it will result in an incorrect total spacing pixel value,
     * The modifier variable @verticalScrollAdditionalSpacingPixels will hold any arising spacing value and will be added in the final math.
     *
     * So the total vertical spacing pixels in the container = (@verticalScrollTotalHorizontalLines - 1) * (@verticalScrollDefaultSpacingDistance) + @verticalScrollAdditionalSpacingPixels
     *
     * Note 2:
     * The class doesn't yet support dynamic padding changes.
     * Therefore padding values can only be changed using Reset() function, which will also reset the scroll mananger's parameters to default values.
     */

    const INT verticalScrollDefaultSpacingDistance = 10; // Default vertical spacing distance between container horizontal lines. (in pixels)
    INT verticalScrollCurrentLinePosition = 0;           // Current horizontal line's vertical postion. (in pixels)
    INT verticalScrollAdditionalSpacingPixels = 0;       // Additional vertical spacing pixels. (Read Note 1 for more information)
    UINT verticalScrollTotalHorizontalLines = 0;         // Indicate the total number of registered horizontal lines in the container.
    UINT verticalScrollTotalHorizontalLinePixels = 0;    // The total vertical height of all registered horizontal lines in pixels. (Spacing pixels not included)

    UINT paddingVertical = 0;                // Container vertical padding (in pixels).
    UINT paddingHorizontal = 0;              // Container horizontal padding (in pixels).
    bool paddingIncludeRightPadding = false; // Indicate whether the container include the right padding.
    bool paddingIncludeBottomPadding = true; // Indicate whether the container include the bottom padding.
    bool paddingLocked = false;              // Indicate whether the container padding values is locked. (Read Note 2 for more information)

private:
    /// [INTERNAL FUNCTIONS]

    /**
     * @brief Get the width and height of the specified window (HWND).
     *
     * @param hWnd            Handle to the target window (HWND).
     * @param windowWidthRef  Reference to the variable that will hold the width.
     * @param windowHeightRef Reference to the variable that will hold the height.
     *
     * @return Returns true if successfully retrieved the window dimensions, otherwise false.
     */
    bool getWindowDimensions(HWND hWnd, UINT &windowWidthRef, UINT &windowHeightRef);

    /**
     * @brief Get the relative postions of the specified window (HWND).
     *
     * @param hWnd          Handle to the target window (HWND).
     * @param windowPosXRef Reference to the variable that will hold the x-postion.
     * @param windowPosYRef Reference to the variable that will hold the y-postion.
     *
     * @return Returns true if successfully retrieved the window positions; otherwise, false.
     */
    bool getWindowPosition(HWND hWnd, INT &windowPosXRef, INT &windowPosYRef);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Sets the container paddings.
     * @note The class doesn't yet support dynamic padding changes.
     * @note Therefore padding values can only be changed using Reset() function,
     * @note which will also reset the scroll mananger's parameters to default values.
     *
     * @param horizontalPaddingPixels The number of pixels for horizontal padding.
     * @param includeRightPadding     Specifies whether to include right padding or not.
     * @param verticalPaddingPixels   The number of pixels for vertical padding.
     * @param includeBottomPadding    Specifies whether to include bottom padding or not.
     */
    void setContainerPaddings(UINT horizontalPaddingPixels = 0, bool includeRightPadding = false,
                              UINT verticalPaddingPixels = 0, bool includeBottomPadding = true);

    /**
     * @brief Get the current horizontal position of the current horizontal line, in pixels.
     * @note The class only support vertical scrolling for now.
     * @note Therefore the function will just return the @paddingHorizontal value.
     *
     * @return The current horizontal position of the current horizontal line, in pixels.
     */
    INT getCurrentHorizontalPosition();

    /**
     * @brief Get the current vertical position of the current horizontal line, in pixels.
     * @note This value is equivalent to the next container item's vertical position.
     *
     * @return The current vertical position of the current horizontal line, in pixels.
     */
    INT getCurrentVerticalPosition();

    /**
     * @brief Get the total vertical height of all registered horizontal lines in pixels. (Spacing pixels included)
     * @note This value is equivalent to the container maximum vertical scrolling position. (nMax value for SCROLLINFO struct)
     *
     * @return The total vertical height of all registered horizontal lines in pixels. (Spacing pixels included)
     */
    INT getTotalHorizontalLinePixelsWithSpacing();

    /**
     * @brief Register a new horizontal line in the container.
     * @note A horizontal line's height is determined by the item with the greatest height in the horizontal line.
     * @note Therefore, failure to provide the correct item (HWND) will result in incorrect scroll information for the container.
     *
     * @param hWnd Handle to the window (HWND) with the greatest height of the current horizontal line.
     */
    void registerNewHorizontalLine(HWND hWnd);

    /**
     * @brief Reset the scroll manager's parameters to default values.
     *
     * @param horizontalPaddingPixels The number of pixels for horizontal padding.
     * @param includeRightPadding     Specifies whether to include right padding or not.
     * @param verticalPaddingPixels   The number of pixels for vertical padding.
     * @param includeBottomPadding    Specifies whether to include bottom padding or not.
     */
    void resetScrollManager(UINT horizontalPaddingPixels = 0, bool includeRightPadding = false,
                            UINT verticalPaddingPixels = 0, bool includeBottomPadding = true);
};

/**
 * @class MyContainer
 *
 * @brief Represents a container window.
 * @brief A container window holds and manages other windows that belong to it.
 *
 * Remarks:
 * - The container window must be created first before adding any windows to it.
 *
 * - When an window is added to the container, it is considered container's content.
 *   The container's content can be either a child window or an associated window.
 *   The container's contents are destroyed when the container is destroyed.
 *
 * - If the container needs scrolling functionality, scrollbars can be created using the provided handles.
 *   Use the class scroll functions to manage the scroll information.
 * 
 * Usage:
 * 1. Create the container:
 *    - Create a MyContainer object.
 *      If the container intends to use scrolling functionality provided by the internal scroll manager, specify the padding values in the constructor.
 *    - Create the container window using the createContainerWindow() function.
 *      If the container wishes to use builtin scrollbars, create the scrollbars using the createVerticalScrollbarWindow() function.
 * 
 * 2. Create and add windows to the container:
 *   - Create windows (MyWindow) that will be added to the container.
 *   - Use the addWindow() function to add windows (MyWindow) to the container.
 *     Scroll manager functions can be used to calculate the scroll information and position the child windows.
 *   - Use the invalidateCheck() function to check if the container's contents are valid.
 *     Container's contents are: The container window, its scrollbar windows, all of its child and associated windows.
 */
class MyContainer
{
public:
    /**
     * @brief Default constructor.
     *
     * @param horizontalPaddingPixels The number of pixels for horizontal padding.
     * @param includeRightPadding     Specifies whether to include right padding or not.
     * @param verticalPaddingPixels   The number of pixels for vertical padding.
     * @param includeBottomPadding    Specifies whether to include bottom padding or not.
     *
     * @note If the container doesn't need scrolling functionality, leave the parameter values at default.
     */
    MyContainer(UINT horizontalPaddingPixels = 0, bool includeRightPadding = false,
                UINT verticalPaddingPixels = 0, bool includeBottomPadding = true);

    /**
     * @brief Destructor.
     *
     * @note Destroy the container window. All of the container's contents are also destroyed. (Child and associated windows, scrollbars, etc.)
     */
    ~MyContainer();

private:
    MyContainerScrollManager scrollManager;          // Provides functionality to manage scroll information if needed.
    std::vector<MyWindow *> vectorChildWindows;      // Vector that holds pointers to the container's child windows.
    std::vector<MyWindow *> vectorAssociatedWindows; // Vector that holds pointers to the container's associated windows.
    bool isContainerWindowCreated = false;           // Indicate whether the container window is created or not.
    bool isVerticalScrollbarCreated = false;         // Indicate whether the container's vertical scrollbar window is created or not.

public:
    MyWindow *pContainerWindow = nullptr;         // Pointer to the container window.
    MyWindow *pVerticalScrollbarWindow = nullptr; // Pointer to the container's vertical scrollbar window.
    // MyWindow *pHorizontalScrollbarWindow = nullptr; // Horizontal scrollbar is not supported yet, the class is a work in progress.

private:
    /// [INTERNAL FUNCTIONS]

    /**
     * @brief Destroy the container window and its contents, scrollbars.
     */
    void destroyContainer();

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Create the container window.
     *
     * @param hWndParent       Handle to the container parent.
     * @param visibleByDefault Specifies whether the window is visible by default.
     * @param windowProcedure  The container procedure.
     * @param windowPosX       The container horizontal position.
     * @param windowPosY       The container vertical position.
     * @param windowWidth      The container width.
     * @param windowHeight     The container height.
     * @param windowID         The container ID.
     */
    bool createContainerWindow(HWND hWndParent, bool visibleByDefault, SUBCLASSPROC windowProcedure, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID);

    /**
     * @brief Create the vertical scrollbar window.
     *
     * @param hWndParent       Handle to the scrollbar parent.
     * @param visibleByDefault Specifies whether the window is visible by default.
     * @param windowPosX       The scrollbar horizontal position.
     * @param windowPosY       The scrollbar vertical position.
     * @param windowWidth      The scrollbar width.
     * @param windowHeight     The scrollbar height.
     * @param windowID         The scrollbar ID.
     */
    bool createVerticalScrollbarWindow(HWND hWndParent, bool visibleByDefault, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID);

    /**
     * @brief Get the pointer to the container window.
     *
     * @return Returns the pointer to the container window, nullptr otherwise.
     */
    MyWindow *getContainerWindow();

    /**
     * @brief Get the pointer (reference) to the container window.
     *
     * @return Returns the pointer (reference) to the container window, nullptr otherwise.
     */
    MyWindow *&getContainerWindowRef();

    /**
     * @brief Get the pointer to the container vertical scrollbar window.
     *
     * @return Returns the pointer to the container vertical scrollbar window, nullptr otherwise.
     */
    MyWindow *getContainerVerticalScrollbarWindow();

    /**
     * @brief Get the pointer (reference) to the container vertical scrollbar window.
     *
     * @return Returns the pointer (reference) to the container vertical scrollbar window, nullptr otherwise.
     */
    MyWindow *&getContainerVerticalScrollbarWindowRef();

    /**
     * @brief Get the vector that contains pointers to the container's child windows.
     *
     * @return Return the vector reference that contains pointers to the container's child windows.
     */
    std::vector<MyWindow *> &getChildWindowsVector();

    /**
     * @brief Get the vector that contains pointers to the container's associated windows.
     *
     * @return Return the vector reference that contains pointers to the container's associated windows.
     */
    std::vector<MyWindow *> &getAssociatedWindowsVector();

    /**
     * @brief Add a new window to the container.
     *
     * @param pWindow Pointer to an initialized MyWindow object.
     * @param isChild Specifies whether the window is a child of the container window.
     *
     * @note The container window must be created first before any windows can be added to it.
     *
     * @return Returns true if successfully added the window to the container, false otherwise.
     */
    bool addWindow(MyWindow *pWindow, bool isChild);

    /**
     * @brief Update the container window dimensions.
     *
     * @param newX                                The new container horizontal position.
     * @param newY                                The new container vertical position.
     * @param newWidth                            The new container width.
     * @param newHeight                           The new container height.
     * @param isCurrentThemeWantScrollbarsVisible Specifies whether the current theme want the scrollbars to be visible or not.
     * @param resetScrollPosition                 Specifies whether to reset the scroll position or not.
     */
    bool updateContainerDimensions(INT newX, INT newY, INT newWidth, INT newHeight, bool isCurrentThemeWantScrollbarsVisible, bool resetScrollPosition);

    /**
     * @brief If the container have scrollbars, use this function to update the scrollbars visibility to match the application current theme and dimensions.
     *
     * @param isCurrentThemeWantScrollbarsVisible Specifies whether the current theme want the scrollbars to be visible or not.
     *
     * @return Returns true if successfully updated the scrollbars visibility, false otherwise.
     */
    bool updateScrollbarsVisibility(bool isCurrentThemeWantScrollbarsVisible);

    /**
     * @brief Finds a window with a specific ID in the container.
     *
     * @param windowID The ID of the window to find.
     *
     * @return Returns the window handle (HWND) of the found window, or nullptr if not found.
     */
    HWND findMyWindowByID(INT windowID);

    /**
     * @brief Finds a window with a specific ID in the container.
     *
     * @param windowID The ID of the window to find.
     *
     * @return Returns the MyWindow pointer to the found window, or nullptr if not found.
     */
    MyWindow *findMyWindowByIDEx(INT windowID);

    /**
     * @brief Perform an invalidate check on the container window and all of its contents.

     * @return Returns true if the container window and all of its contents are valid, false otherwise.
     */
    bool invalidateCheck();

    /**
     * @brief Reset the container to its initial state and allow the container to be reused.
     *
     * @param horizontalPaddingPixels The number of pixels for horizontal padding.
     * @param includeRightPadding     Specifies whether to include right padding or not.
     * @param verticalPaddingPixels   The number of pixels for vertical padding.
     * @param includeBottomPadding    Specifies whether to include bottom padding or not.
     */
    void resetContainer(UINT horizontalPaddingPixels = 0, bool includeRightPadding = false,
                        UINT verticalPaddingPixels = 0, bool includeBottomPadding = true);

public:
    /// [SCROLL MANAGER FUNCTIONS]

    /**
     * @brief Get the initiate position of the current container horizontal line, vertical or horizontal.
     * @note These values are updated every time the container registers a new horizontal line.
     *
     * @param direction 0 - horizontal, 1 - vertical.
     *
     * @return Returns the initiate position of the current container horizontal line, vertical or horizontal.
     */
    INT getInitiateWindowPosition(bool direction);

    /**
     * @brief Get the maximum vertical scrolling position of the container window.
     * @note This value is equivalent to the nMax value for SCROLLINFO struct.
     *
     * @return Return the maximum vertical scrolling position of the container window.
     */
    INT getMaximumVerticalScrollingPosition();

    /**
     * @brief Set the scroll information for the vertical scrollbar window.
     * @note Call this function after the vertical scrollbar window is created to set the scroll information.
     *
     * @param redrawScrollbar Specifies whether to redraw the scrollbar or not.
     * @param scrollPos       Scroll position.
     * @param scrollMin       Minimum scroll position.
     * @param scrollMax       Maximum scroll position.
     *                        If set to -1, the maximum scroll position will be calculated automatically.
     * @param scrollPage      Scroll page.
     *                        If set to -1, the scroll page will be calculated automatically.
     *
     * @return Returns true if successfully set the scroll information, false otherwise.
     */
    bool setVerticalScrollInformations(bool redrawScrollbar, INT scrollPos = 0, INT scrollMin = 0, INT scrollMax = -1, INT scrollPage = -1);

    /**
     * @brief Register a new horizontal line in the container.
     * @note A horizontal line's height is determined by the item with the greatest height in the horizontal line.
     * @note Therefore, failure to provide the correct item (HWND) will result in incorrect scroll information for the container.
     *
     * @param hWnd Handle to the window (HWND) with the greatest height of the current horizontal line.
     */
    void registerNewHorizontalLine(HWND hWnd);
};