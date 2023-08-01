/**
 * @file my_winapi.cpp
 * @brief Implementation of the classes defined in my_winapi.h.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../Global/global.h"                 // The distribution header of the global variables, forward declarations and my class declarations.

/// @class MyWindow definitions:
// Constructor:
MyWindow::MyWindow(bool isManaged) : isManaged(isManaged), windowType(MyWindowType::Unmanaged), pSubclass(nullptr), hWnd(nullptr) {}
// Destructor:
MyWindow::~MyWindow()
{
    // Destroy the window object.
    if (IsWindow(this->hWnd))
    {
        if (!DestroyWindow(this->hWnd))
            WriteLog(L"Failed to destroy the window object.", L" [CLASS: \"MyWindow\" | FUNC: \"~MyWindow()\"]", MyLogType::Error);

        this->hWnd = nullptr;
    }

    // Only managed window variant will destroy their associated objects automatically, if there is any.
    if (!this->isManaged)
        return;

    // Check if the window have associated subclass object.
    if (!this->pSubclass)
        return;

    // Cast the subclass pointer to the correct type and delete the associated subclass object.
    switch (this->windowType)
    {
    case MyWindowType::StandardButton:
    {
        MyStandardButton *p_casted_subclass = static_cast<MyStandardButton *>(this->pSubclass);
        delete p_casted_subclass;

        break;
    }
    case MyWindowType::ImageButton:
    {
        MyImageButton *p_casted_subclass = static_cast<MyImageButton *>(this->pSubclass);
        delete p_casted_subclass;

        break;
    }
    case MyWindowType::RadioButton:
    {
        MyRadioButton *p_casted_subclass = static_cast<MyRadioButton *>(this->pSubclass);
        delete p_casted_subclass;

        break;
    }
    case MyWindowType::Edit:
    {
        MyEdit *p_casted_subclass = static_cast<MyEdit *>(this->pSubclass);
        delete p_casted_subclass;

        break;
    }
    case MyWindowType::DDLCombobox:
    {
        MyDDLCombobox *p_casted_subclass = static_cast<MyDDLCombobox *>(this->pSubclass);
        delete p_casted_subclass;

        break;
    }
    default:
        break;
    }
}
// Public static member function(s) [GENERAL FUNCTIONS]:
HWND MyWindow::findWindowInVector(const std::vector<MyWindow> &windows, INT windowID)
{
    // Iterate through the container and find the window with the specified ID.
    for (const auto &window : windows)
    {
        if (GetDlgCtrlID(window.hWnd) == windowID)
            return window.hWnd;
    }

    return nullptr;
}
HWND MyWindow::findWindowInVectorP(const std::vector<MyWindow *> &windows, INT windowID)
{
    // Iterate through the container and find the window with the specified ID.
    for (const auto &p_window : windows)
    {
        if (p_window && GetDlgCtrlID(p_window->hWnd) == windowID)
            return p_window->hWnd;
    }

    return nullptr;
}
MyWindow *MyWindow::findWindowInVectorPEx(const std::vector<MyWindow *> &windows, INT windowID)
{
    // Iterate through the container and find the window with the specified ID.
    for (const auto &p_window : windows)
    {
        if (p_window && GetDlgCtrlID(p_window->hWnd) == windowID)
            return p_window;
    }

    return nullptr;
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyWindow::isManagedWindow()
{
    return this->isManaged;
}
bool MyWindow::isValid()
{
    // Check if the window hanle is point to a valid window object.
    if (!IsWindow(this->hWnd))
        return false;

    return true;
}
MyWindowType MyWindow::getWindowType()
{
    return this->windowType;
}
// Public member function(s) [MANAGED WINDOW FUNCTIONS]:
void *MyWindow::getSubclassPointer()
{
    return this->pSubclass;
}
void *&MyWindow::getSubclassPointerRef()
{
    return this->pSubclass;
}
bool MyWindow::createStandardButton(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the window is a managed variant.
        if (!this->isManaged)
        {
            error_message = L"Only managed window variant can use this function.";
            break;
        }

        // Check if the window handle is already pointing to an existing window object.
        if (IsWindow(hWnd))
        {
            error_message = L"The window handle is already pointing to an existing window object.";
            break;
        }

        // Prepare the creation flags.
        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD;

        // If the window doesn't want to be visible by default, remove the WS_VISIBLE flag.
        if (!visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        // If the window doesn't want tab stop event, remove the WS_TABSTOP flag.
        if (!useTabStop)
            window_flags &= ~WS_TABSTOP;

        // Create the window.
        this->hWnd = CreateWindowExW(NULL, WC_BUTTON, windowText.c_str(), window_flags, windowPosX, windowPosY, windowWidth, windowHeight, hWndParent, windowID, NULL, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(this->hWnd))
        {
            error_message = L"Failed to create the window object.";
            break;
        }

        // Create the subclass object.
        this->pSubclass = new MyStandardButton();

        // Check if the subclass object was successfully created.
        if (!this->pSubclass)
        {
            error_message = L"Failed to create the subclass object.";
            break;
        }

        // Cast the subclass pointer to the correct type.
        MyStandardButton *p_casted_subclass = static_cast<MyStandardButton *>(this->pSubclass);

        // Install the subclass callback.
        if (!p_casted_subclass->setSubclass(this->hWnd))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        // Set the window type.
        this->windowType = MyWindowType::StandardButton;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to create the standard button.\n");
        message.append(L"Error Message: " + error_message + L"\n");
        message.append(L"CLASS: MyWindow\n");
        message.append(L"FUNC: createStandardButton()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return false;
    }

    return true;
}
bool MyWindow::createImageButton(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, MyImageButtonNonSharedPropertiesConfig configNonSharedProperties, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the window is a managed variant.
        if (!this->isManaged)
        {
            error_message = L"Only managed window variant can use this function.";
            break;
        }

        // Check if the window handle is already pointing to an existing window object.
        if (IsWindow(hWnd))
        {
            error_message = L"The window handle is already pointing to an existing window object.";
            break;
        }

        // Prepare the creation flags.
        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD;

        // If the window doesn't want to be visible by default, remove the WS_VISIBLE flag.
        if (!visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        // If the window doesn't want tab stop event, remove the WS_TABSTOP flag.
        if (!useTabStop)
            window_flags &= ~WS_TABSTOP;

        // Create the window.
        this->hWnd = CreateWindowExW(NULL, WC_BUTTON, windowText.c_str(), window_flags, windowPosX, windowPosY, windowWidth, windowHeight, hWndParent, windowID, NULL, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(this->hWnd))
        {
            error_message = L"Failed to create the window object.";
            break;
        }

        // Create the subclass object.
        this->pSubclass = new MyImageButton();

        // Check if the subclass object was successfully created.
        if (!this->pSubclass)
        {
            error_message = L"Failed to create the subclass object.";
            break;
        }

        // Cast the subclass pointer to the correct type.
        MyImageButton *p_casted_subclass = static_cast<MyImageButton *>(this->pSubclass);

        // Install the subclass callback.
        if (!p_casted_subclass->setSubclass(this->hWnd, configNonSharedProperties))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        // Set the window type.
        this->windowType = MyWindowType::ImageButton;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to create the image button.\n");
        message.append(L"Error Message: " + error_message + L"\n");
        message.append(L"CLASS: MyWindow\n");
        message.append(L"FUNC: createImageButton()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return false;
    }

    return true;
}
bool MyWindow::createRadioButton(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the window is a managed variant.
        if (!this->isManaged)
        {
            error_message = L"Only managed window variant can use this function.";
            break;
        }

        // Check if the window handle is already pointing to an existing window object.
        if (IsWindow(hWnd))
        {
            error_message = L"The window handle is already pointing to an existing window object.";
            break;
        }

        // Prepare the creation flags.
        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD;

        // If the window doesn't want to be visible by default, remove the WS_VISIBLE flag.
        if (!visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        // If the window doesn't want tab stop event, remove the WS_TABSTOP flag.
        if (!useTabStop)
            window_flags &= ~WS_TABSTOP;

        // Create the window.
        this->hWnd = CreateWindowExW(NULL, WC_BUTTON, windowText.c_str(), window_flags, windowPosX, windowPosY, windowWidth, windowHeight, hWndParent, windowID, NULL, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(this->hWnd))
        {
            error_message = L"Failed to create the window object.";
            break;
        }

        // Create the subclass object.
        this->pSubclass = new MyRadioButton();

        // Check if the subclass object was successfully created.
        if (!this->pSubclass)
        {
            error_message = L"Failed to create the subclass object.";
            break;
        }

        // Cast the subclass pointer to the correct type.
        MyRadioButton *p_casted_subclass = static_cast<MyRadioButton *>(this->pSubclass);

        // Install the subclass callback.
        if (!p_casted_subclass->setSubclass(this->hWnd))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        // Set the window type.
        this->windowType = MyWindowType::RadioButton;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to create the radio button.\n");
        message.append(L"Error Message: " + error_message + L"\n");
        message.append(L"CLASS: MyWindow\n");
        message.append(L"FUNC: createRadioButton()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return false;
    }

    return true;
}
bool MyWindow::createEditbox(HWND hWndParent, std::wstring windowText, bool visibleByDefault, bool useTabStop, MyEditNonSharedPropertiesConfig configNonSharedProperties, MyEditboxType editboxType, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the window is a managed variant.
        if (!this->isManaged)
        {
            error_message = L"Only managed window variant can use this function.";
            break;
        }

        // Check if the window handle is already pointing to an existing window object.
        if (IsWindow(hWnd))
        {
            error_message = L"The window handle is already pointing to an existing window object.";
            break;
        }

        // Prepare the creation flags.
        DWORD window_flags = NULL;
        switch (editboxType)
        {
        case MyEditboxType::Singleline:
        {
            window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL;
            break;
        }
        case MyEditboxType::SinglelinePassword:
        {
            window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD;
            break;
        }
        case MyEditboxType::Multiline:
        {
            window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
            break;
        }
        }

        // If the window doesn't want to be visible by default, remove the WS_VISIBLE flag.
        if (!visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        // If the window doesn't want tab stop event, remove the WS_TABSTOP flag.
        if (!useTabStop)
            window_flags &= ~WS_TABSTOP;

        // Create the subclass object.
        this->pSubclass = new MyEdit();

        // Check if the subclass object was successfully created.
        if (!this->pSubclass)
        {
            error_message = L"Failed to create the subclass object.";
            break;
        }

        // Cast the subclass pointer to the correct type.
        MyEdit *p_casted_subclass = static_cast<MyEdit *>(this->pSubclass);

        // Create the static window using the static window handle from the subclass object.
        HWND &hWndStatic = p_casted_subclass->getStaticHandleRef();
        hWndStatic = CreateWindowExW(NULL, WC_STATIC, L"", WS_VISIBLE | WS_CHILD | SS_NOPREFIX, windowPosX, windowPosY, windowWidth, windowHeight, hWndParent, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(configNonSharedProperties.hWndStaticID)), NULL, NULL);

        // Check if the static window was successfully created.
        if (!IsWindow(hWndStatic))
        {
            error_message = L"Failed to create the static window object.";
            break;
        }

        // Create the window.
        this->hWnd = CreateWindowExW(NULL, WC_EDIT, windowText.c_str(), window_flags, windowPosX + 2, windowPosY + 2, windowWidth - 4, windowHeight - 4, hWndParent, windowID, NULL, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(this->hWnd))
        {
            error_message = L"Failed to create the window object.";
            break;
        }

        // Install the subclass callback.
        if (!p_casted_subclass->setSubclass(this->hWnd, configNonSharedProperties))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        // Set the window type.
        this->windowType = MyWindowType::Edit;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to create the editbox.\n");
        message.append(L"Error Message: " + error_message + L"\n");
        message.append(L"CLASS: MyWindow\n");
        message.append(L"FUNC: createEditbox()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return false;
    }

    return true;
}
bool MyWindow::createDDLCombobox(HWND hWndParent, bool visibleByDefault, bool useTabStop, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the window is a managed variant.
        if (!this->isManaged)
        {
            error_message = L"Only managed window variant can use this function.";
            break;
        }

        // Check if the window handle is already pointing to an existing window object.
        if (IsWindow(hWnd))
        {
            error_message = L"The window handle is already pointing to an existing window object.";
            break;
        }

        // Prepare the creation flags.
        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED;

        // If the window doesn't want to be visible by default, remove the WS_VISIBLE flag.
        if (!visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        // If the window doesn't want tab stop event, remove the WS_TABSTOP flag.
        if (!useTabStop)
            window_flags &= ~WS_TABSTOP;

        // Create the window.
        this->hWnd = CreateWindowExW(NULL, WC_COMBOBOX, L"", window_flags, windowPosX, windowPosY, windowWidth, windowHeight, hWndParent, windowID, NULL, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(this->hWnd))
        {
            error_message = L"Failed to create the window object.";
            break;
        }

        // Create the subclass object.
        this->pSubclass = new MyDDLCombobox();

        // Check if the subclass object was successfully created.
        if (!this->pSubclass)
        {
            error_message = L"Failed to create the subclass object.";
            break;
        }

        // Cast the subclass pointer to the correct type.
        MyDDLCombobox *p_casted_subclass = static_cast<MyDDLCombobox *>(this->pSubclass);

        // Install the subclass callback.
        if (!p_casted_subclass->setSubclass(this->hWnd, windowHeight))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        // Set the window type.
        this->windowType = MyWindowType::DDLCombobox;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to create the drop-down list combobox.\n");
        message.append(L"Error Message: " + error_message + L"\n");
        message.append(L"CLASS: MyWindow\n");
        message.append(L"FUNC: createDDLCombobox()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return false;
    }

    return true;
}

/// @class MyContainerScrollManager definitions:
// Private member function(s) [INTERNAL FUNCTIONS]:
bool MyContainerScrollManager::getWindowDimensions(HWND hWnd, UINT &windowWidthRef, UINT &windowHeightRef)
{
    RECT rect_window;

    // Get the window client rect.
    if (!GetClientRect(hWnd, &rect_window))
        return false;

    // Store the dimensions in the output parameters.
    windowWidthRef = rect_window.right - rect_window.left;
    windowHeightRef = rect_window.bottom - rect_window.top;

    return true;
}
bool MyContainerScrollManager::getWindowPosition(HWND hWnd, INT &windowPosXRef, INT &windowPosYRef)
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
        if (window_style == NULL)
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
// Public member function(s) [GENERAL FUNCTIONS]:
void MyContainerScrollManager::setContainerPaddings(UINT horizontalPaddingPixels, bool includeRightPadding, UINT verticalPaddingPixels, bool includeBottomPadding)
{
    // Check if the padding values is locked.
    if (this->paddingLocked)
    {
        WriteLog(L"The class doesn't yet support dynamic padding changes.", L" [CLASS: \"MyContainerScrollManager\" | FUNC: \"setContainerPaddings()\"]", MyLogType::Error);
        return;
    }

    // Set the padding values.
    this->paddingHorizontal = horizontalPaddingPixels;
    this->paddingIncludeRightPadding = includeRightPadding;
    this->paddingVertical = verticalPaddingPixels;
    this->paddingIncludeBottomPadding = includeBottomPadding;

    // Lock the padding values.
    this->paddingLocked = true;
}
INT MyContainerScrollManager::getCurrentHorizontalPosition()
{
    return this->paddingHorizontal;
}
INT MyContainerScrollManager::getCurrentVerticalPosition()
{
    return this->paddingVertical + this->verticalScrollCurrentLinePosition + this->verticalScrollAdditionalSpacingPixels;
}
INT MyContainerScrollManager::getTotalHorizontalLinePixelsWithSpacing()
{
    INT RET = (((this->verticalScrollTotalHorizontalLines - 1) * this->verticalScrollDefaultSpacingDistance) + this->verticalScrollAdditionalSpacingPixels) + this->verticalScrollTotalHorizontalLinePixels + (paddingIncludeBottomPadding ? this->paddingVertical * 2 : this->paddingVertical);

    return RET;
}
void MyContainerScrollManager::registerNewHorizontalLine(HWND hWnd)
{
    // Get the window dimensions.
    UINT width, height;
    if (!this->getWindowDimensions(hWnd, width, height))
    {
        WriteLog(L"Failed to retrieve the window dimensions.", L" [CLASS: \"MyContainerScrollManager\" | FUNC: \"registerNewHorizontalLine()\"]", MyLogType::Error);
    }

    // Get the window relative positions.
    INT pos_x = 0, pos_y = 0, extra_spacing_pixels = 0;
    if (!this->getWindowPosition(hWnd, pos_x, pos_y))
    {
        WriteLog(L"Failed to retrieve the window relative positions.", L" [CLASS: \"MyContainerScrollManager\" | FUNC: \"registerNewHorizontalLine()\"]", MyLogType::Error);
    }

    // Calculate the arising spacing value.
    extra_spacing_pixels = pos_y - this->getCurrentVerticalPosition();

    // Update scroll informations.
    this->verticalScrollTotalHorizontalLinePixels += height;
    this->verticalScrollAdditionalSpacingPixels += extra_spacing_pixels;
    this->verticalScrollCurrentLinePosition += (height + this->verticalScrollDefaultSpacingDistance);
    this->verticalScrollTotalHorizontalLines += 1;
}
void MyContainerScrollManager::resetScrollManager(UINT horizontalPaddingPixels, bool includeRightPadding, UINT verticalPaddingPixels, bool includeBottomPadding)
{
    // Reset scroll information values.
    this->verticalScrollCurrentLinePosition = 0;
    this->verticalScrollAdditionalSpacingPixels = 0;
    this->verticalScrollTotalHorizontalLines = 0;
    this->verticalScrollTotalHorizontalLinePixels = 0;

    // Set new padding values.
    this->paddingHorizontal = horizontalPaddingPixels;
    this->paddingIncludeRightPadding = includeRightPadding;
    this->paddingVertical = verticalPaddingPixels;
    this->paddingIncludeBottomPadding = includeBottomPadding;
}

/// @class MyContainer definitions:
// Constructor:
MyContainer::MyContainer(UINT horizontalPaddingPixels, bool includeRightPadding, UINT verticalPaddingPixels, bool includeBottomPadding)
{
    // Set the container's padding values.
    this->scrollManager.setContainerPaddings(horizontalPaddingPixels, includeRightPadding, verticalPaddingPixels, includeBottomPadding);
}
// Destructor:
MyContainer::~MyContainer()
{
    this->destroyContainer();
}
// Private member function(s) [INTERNAL FUNCTIONS]:
void MyContainer::destroyContainer()
{
    // Destroy the container's associated window(s).
    if (!this->vectorAssociatedWindows.empty())
    {
        for (auto &p_window : this->vectorAssociatedWindows)
        {
            delete p_window;
        }

        this->vectorAssociatedWindows.clear();
    }

    // Destroy the container's child window(s).
    if (!this->vectorChildWindows.empty())
    {
        for (auto &p_window : this->vectorChildWindows)
        {
            delete p_window;
        }

        this->vectorChildWindows.clear();
    }

    // Destroy the container's scrollbar windows.
    if (this->pVerticalScrollbarWindow)
    {
        delete this->pVerticalScrollbarWindow;
        this->pVerticalScrollbarWindow = nullptr;
        this->isVerticalScrollbarCreated = false;
    }

    // Destroy the container window.
    if (this->pContainerWindow)
    {
        delete this->pContainerWindow;
        this->pContainerWindow = nullptr;
        this->isContainerWindowCreated = false;
    }
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyContainer::createContainerWindow(HWND hWndParent, bool visibleByDefault, SUBCLASSPROC windowProcedure, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the container window is already created.
        if (this->isContainerWindowCreated)
        {
            error_message = L"The container window is already created.";
            break;
        }

        // Prepare the creation flags.
        DWORD window_flags = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | SS_NOPREFIX;

        // If the window doesn't want to be visible by default, remove the WS_VISIBLE flag.
        if (!visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        // Create the window.
        this->pContainerWindow = new MyWindow(false);
        this->pContainerWindow->hWnd = CreateWindowExW(WS_EX_CONTROLPARENT, WC_STATIC, L"",
                                                       window_flags,
                                                       windowPosX, windowPosY,
                                                       windowWidth, windowHeight, hWndParent, windowID, NULL, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(this->pContainerWindow->hWnd))
        {
            error_message = L"Failed to create the window object.";
            break;
        }

        // Install the container window procedure.
        if (!SetWindowSubclass(this->pContainerWindow->hWnd, windowProcedure, NULL, NULL))
        {
            error_message = L"Failed to install the container window procedure.";
            break;
        }

        this->isContainerWindowCreated = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyContainer\" | FUNC: \"createContainerWindow()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyContainer::createVerticalScrollbarWindow(HWND hWndParent, bool visibleByDefault, INT windowPosX, INT windowPosY, INT windowWidth, INT windowHeight, HMENU windowID)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the container vertical scrollbar window is already created.
        if (this->isVerticalScrollbarCreated)
        {
            error_message = L"The container vertical scrollbar window is already created.";
            break;
        }

        // Check if the container window has been created yet.
        if (!this->isContainerWindowCreated)
        {
            error_message = L"The container window must be created first before its scrollbar windows can be created.";
            break;
        }

        // Prepare the creation flags.
        DWORD window_flags = WS_VISIBLE | WS_CHILD | SBS_VERT;

        // If the window doesn't want to be visible by default, remove the WS_VISIBLE flag.
        if (!visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        // Create the window.
        this->pVerticalScrollbarWindow = new MyWindow(false);
        this->pVerticalScrollbarWindow->hWnd = CreateWindowExW(NULL, WC_SCROLLBAR, NULL,
                                                               window_flags,
                                                               windowPosX, windowPosY,
                                                               windowWidth, windowHeight, hWndParent, windowID, NULL, NULL);

        // Check if the window was successfully created.
        if (!IsWindow(this->pVerticalScrollbarWindow->hWnd))
        {
            error_message = L"Failed to create the window object.";
            break;
        }

        // Get the container window dimensions.
        RECT rect_container;
        if (!GetClientRect(this->pContainerWindow->hWnd, &rect_container))
        {
            error_message = L"Failed to retrieve the container window dimensions.";
            break;
        }

        this->isVerticalScrollbarCreated = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyContainer\" | FUNC: \"createVerticalScrollbarWindow()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
MyWindow *MyContainer::getContainerWindow()
{
    if (!this->isContainerWindowCreated)
        return nullptr;

    return this->pContainerWindow;
}
MyWindow *&MyContainer::getContainerWindowRef()
{
    return this->pContainerWindow;
}
MyWindow *MyContainer::getContainerVerticalScrollbarWindow()
{
    if (!this->isVerticalScrollbarCreated)
        return nullptr;

    return this->pVerticalScrollbarWindow;
}
MyWindow *&MyContainer::getContainerVerticalScrollbarWindowRef()
{
    return this->pVerticalScrollbarWindow;
}
std::vector<MyWindow *> &MyContainer::getChildWindowsVector()
{
    return this->vectorChildWindows;
}
std::vector<MyWindow *> &MyContainer::getAssociatedWindowsVector()
{
    return this->vectorAssociatedWindows;
}
bool MyContainer::addWindow(MyWindow *pWindow, bool isChild)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the container window has been created yet.
        if (!this->isContainerWindowCreated)
        {
            error_message = L"The container window must be created first before any window can be added to the container.";
            break;
        }

        // Check if the window pointer is pointing to initialized MyWindow object.
        if (!pWindow || !IsWindow(pWindow->hWnd))
        {
            error_message = L"The window pointer is not pointing to an initialized MyWindow object.";
            break;
        }

        // Add the window to the container.
        if (isChild)
        {
            if (GetParent(pWindow->hWnd) != this->pContainerWindow->hWnd)
            {
                error_message = L"The window is not a child of the container window.";
                break;
            }

            this->vectorChildWindows.push_back(pWindow);
        }
        else
            this->vectorAssociatedWindows.push_back(pWindow);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyContainer\" | FUNC: \"addWindow()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyContainer::updateContainerDimensions(INT newX, INT newY, INT newWidth, INT newHeight, bool isCurrentThemeWantScrollbarsVisible, bool resetScrollPosition)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the container window has been created yet.
        if (!this->isContainerWindowCreated)
        {
            error_message = L"The container window must be created first.";
            break;
        }

        // If the container have a vertical scrollbar, get the scrollbar dimensions.
        RECT rect_vertical_scrollbar = {0};
        INT vertical_scrollbar_width = 0, vertical_scrollbar_height = 0;
        if (this->isVerticalScrollbarCreated)
        {
            if (!GetClientRect(this->pVerticalScrollbarWindow->hWnd, &rect_vertical_scrollbar))
            {
                error_message = L"Failed to retrieve the container's vertical scrollbar client rect.";
                break;
            }

            vertical_scrollbar_width = rect_vertical_scrollbar.right - rect_vertical_scrollbar.left;
            vertical_scrollbar_height = rect_vertical_scrollbar.bottom - rect_vertical_scrollbar.top;
        }

        // Prepare an HDWP struct to update the dimensions of the container and its scrollbar all at once.
        HDWP hdwp_controls = BeginDeferWindowPos(2);

        // Update the container dimensions.
        DeferWindowPos(hdwp_controls, this->pContainerWindow->hWnd, NULL, newX, newY,
                       (!isCurrentThemeWantScrollbarsVisible ? newWidth : newWidth - vertical_scrollbar_width),
                       newHeight,
                       SWP_NOZORDER);

        // Update the scrollbar dimensions.
        if (this->isVerticalScrollbarCreated)
        {
            // Update new scrollbar's height.
            if (newHeight > vertical_scrollbar_height || newHeight < vertical_scrollbar_height)
                DeferWindowPos(hdwp_controls, this->pVerticalScrollbarWindow->hWnd, NULL,
                               (newX + newWidth) - vertical_scrollbar_width,
                               newY,
                               vertical_scrollbar_width,
                               newHeight,
                               SWP_NOZORDER);

            // Update new scrollbar's x position.
            else
                DeferWindowPos(hdwp_controls, this->pVerticalScrollbarWindow->hWnd, NULL,
                               (newX + newWidth) - vertical_scrollbar_width,
                               newY,
                               NULL,
                               NULL,
                               SWP_NOSIZE | SWP_NOZORDER);
        }

        // Update the dimensions of the container and its scrollbar all at once.
        if (!EndDeferWindowPos(hdwp_controls))
        {
            error_message = L"Failed to update the container and its scrollbar dimensions.";
            break;
        }

        // Update scroll informations for the container.
        if (this->isVerticalScrollbarCreated)
        {
            // Get the container new dimensions.
            RECT rect_container;
            if (!GetClientRect(g_Container_MainContent->pContainerWindow->hWnd, &rect_container))
            {
                error_message = L"Failed to retrieve the container's client rect.";
                break;
            }

            // Get the container scroll information struct.
            SCROLLINFO scroll_info;
            scroll_info.cbSize = sizeof(SCROLLINFO);
            scroll_info.fMask = SIF_ALL;
            if (!SendMessageW(g_Container_MainContent->pVerticalScrollbarWindow->hWnd, SBM_GETSCROLLINFO, NULL, reinterpret_cast<LPARAM>(&scroll_info)))
            {
                error_message = L"Failed to retrieve the container scroll information struct.";
                break;
            }

            // New container height dimension equivalent to new page size.
            INT new_container_height = rect_container.bottom - rect_container.top;
            scroll_info.nPage = new_container_height;

            // Scroll the container back to the zero pos and reset the scroll pos.
            if (resetScrollPosition)
            {
                ScrollWindowEx(g_Container_MainContent->pContainerWindow->hWnd, NULL, scroll_info.nPos, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                scroll_info.nPos = 0;
            }

            // Set scroll info.
            SendMessageW(g_Container_MainContent->pVerticalScrollbarWindow->hWnd, SBM_SETSCROLLINFO, TRUE, reinterpret_cast<LPARAM>(&scroll_info));

            // Update container scrollbar visibility.
            ShowWindow(this->pVerticalScrollbarWindow->hWnd, (new_container_height > scroll_info.nMax ? SW_HIDE : (isCurrentThemeWantScrollbarsVisible ? SW_SHOW : SW_HIDE)));
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyContainer\" | FUNC: \"updateContainerDimensions()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyContainer::updateScrollbarsVisibility(bool isCurrentThemeWantScrollbarsVisible)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the container window has been created yet.
        if (!this->isContainerWindowCreated)
        {
            error_message = L"The container window must be created first.";
            break;
        }

        // Check if the container vertical scrollbar window is exists.
        if (this->pVerticalScrollbarWindow)
        {
            // Get the container scroll information struct.
            SCROLLINFO scroll_info;
            scroll_info.cbSize = sizeof(SCROLLINFO);
            scroll_info.fMask = SIF_ALL;
            if (!SendMessageW(this->pVerticalScrollbarWindow->hWnd, SBM_GETSCROLLINFO, NULL, reinterpret_cast<LPARAM>(&scroll_info)))
            {
                error_message = L"Failed to retrieve the container scroll information struct.";
                break;
            }

            // Get the container window dimensions.
            RECT rect_container;
            if (!GetClientRect(this->pContainerWindow->hWnd, &rect_container))
            {
                error_message = L"Failed to retrieve the container's client rect.";
                break;
            }

            // If the container is big enough to show all of its content, then hide the scrollbar. Otherwise, show the scrollbar.
            if (static_cast<INT>(rect_container.bottom) > scroll_info.nMax)
                ShowWindow(this->pVerticalScrollbarWindow->hWnd, SW_HIDE);
            else
            {
                if (isCurrentThemeWantScrollbarsVisible)
                    ShowWindow(this->pVerticalScrollbarWindow->hWnd, SW_SHOW);
            }
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyContainer\" | FUNC: \"updateScrollbarsVisibility()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
HWND MyContainer::findMyWindowByID(INT windowID)
{
    // Check if the target window is a child window of the container window.
    if (!this->vectorChildWindows.empty())
    {
        for (auto &p_window : this->vectorChildWindows)
        {
            if (GetDlgCtrlID(p_window->hWnd) == windowID)
                return p_window->hWnd;
        }
    }

    // Check if the target window is an associated window of the container window.
    if (!this->vectorAssociatedWindows.empty())
    {
        for (auto &p_window : this->vectorAssociatedWindows)
        {
            if (GetDlgCtrlID(p_window->hWnd) == windowID)
                return p_window->hWnd;
        }
    }

    // Check if the target window is the container's vertical scrollbar window.
    if (this->isVerticalScrollbarCreated)
    {
        if (GetDlgCtrlID(pVerticalScrollbarWindow->hWnd) == windowID)
            return pVerticalScrollbarWindow->hWnd;
    }

    // Check if the target window is the container window.
    if (this->isContainerWindowCreated)
    {
        if (GetDlgCtrlID(this->pContainerWindow->hWnd) == windowID)
            return pContainerWindow->hWnd;
    }

    return nullptr;
}
MyWindow *MyContainer::findMyWindowByIDEx(INT windowID)
{
    // Check if the target window is a child window of the container window.
    if (!this->vectorChildWindows.empty())
    {
        for (auto &p_window : this->vectorChildWindows)
        {
            if (GetDlgCtrlID(p_window->hWnd) == windowID)
                return p_window;
        }
    }

    // Check if the target window is an associated window of the container window.
    if (!this->vectorAssociatedWindows.empty())
    {
        for (auto &p_window : this->vectorAssociatedWindows)
        {
            if (GetDlgCtrlID(p_window->hWnd) == windowID)
                return p_window;
        }
    }

    // Check if the target window is the container's vertical scrollbar window.
    if (this->isVerticalScrollbarCreated)
    {
        if (GetDlgCtrlID(pVerticalScrollbarWindow->hWnd) == windowID)
            return pVerticalScrollbarWindow;
    }

    // Check if the target window is the container window.
    if (this->isContainerWindowCreated)
    {
        if (GetDlgCtrlID(this->pContainerWindow->hWnd) == windowID)
            return pContainerWindow;
    }

    return nullptr;
}
bool MyContainer::invalidateCheck()
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the container window is valid.
        if (this->isContainerWindowCreated)
        {
            if (!this->pContainerWindow || !IsWindow(this->pContainerWindow->hWnd))
            {
                error_message = L"The container window is invalid.";
                break;
            }
        }

        // Check if the container vertical scrollbar is valid.
        if (this->isVerticalScrollbarCreated)
        {
            if (!this->pVerticalScrollbarWindow || !IsWindow(this->pVerticalScrollbarWindow->hWnd))
            {
                error_message = L"The container vertical scrollbar window is invalid.";
                break;
            }
        }

        // Check if all the container child windows are valid.
        bool are_all_child_windows_valid = true;
        if (!this->vectorChildWindows.empty())
        {
            for (auto &p_window : this->vectorChildWindows)
            {
                if (!p_window || p_window->getWindowType() == MyWindowType::Undefined || !IsWindow(p_window->hWnd))
                {
                    are_all_child_windows_valid = false;
                    break;
                }
            }
        }
        if (!are_all_child_windows_valid)
        {
            error_message = L"One or more of the container child windows are invalid.";
            break;
        }

        // Check if all the container associated windows are valid.
        bool are_all_associated_windows_valid = true;
        if (!this->vectorAssociatedWindows.empty())
        {
            for (auto &p_window : this->vectorAssociatedWindows)
            {
                if (!p_window || p_window->getWindowType() == MyWindowType::Undefined || !IsWindow(p_window->hWnd))
                {
                    are_all_associated_windows_valid = false;
                    break;
                }
            }
        }
        if (!are_all_associated_windows_valid)
        {
            error_message = L"One or more of the container associated windows are invalid.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyContainer\" | FUNC: \"invalidateCheck()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
void MyContainer::resetContainer(UINT horizontalPaddingPixels, bool includeRightPadding, UINT verticalPaddingPixels, bool includeBottomPadding)
{
    // Reset the container scroll manager.
    this->scrollManager.resetScrollManager(horizontalPaddingPixels, includeRightPadding, verticalPaddingPixels, includeBottomPadding);

    // Destroy the container.
    this->destroyContainer();
}
// Public member function(s) [SCROLL MANAGER FUNCTIONS]:
INT MyContainer::getInitiateWindowPosition(bool direction)
{
    if (!direction)
        return this->scrollManager.getCurrentHorizontalPosition();
    else
        return this->scrollManager.getCurrentVerticalPosition();
}
INT MyContainer::getMaximumVerticalScrollingPosition()
{
    return this->scrollManager.getTotalHorizontalLinePixelsWithSpacing();
}
bool MyContainer::setVerticalScrollInformations(bool redrawScrollbar, INT scrollPos, INT scrollMin, INT scrollMax, INT scrollPage)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the container window has been created yet.
        if (!this->isContainerWindowCreated)
        {
            error_message = L"The container window must be created first before the scroll information can be updated.";
            break;
        }

        // Check if the container vertical scrollbar window has been created yet.
        if (!this->isVerticalScrollbarCreated)
        {
            error_message = L"The container vertical scrollbar window must be created first before the scroll information can be updated.";
            break;
        }

        // Get the container window dimensions.
        RECT rect_container;
        if (!GetClientRect(this->pContainerWindow->hWnd, &rect_container))
        {
            error_message = L"Failed to retrieve the container window dimensions.";
            break;
        }

        // Set the container vertical scroll information.
        SCROLLINFO scroll_info;
        scroll_info.cbSize = sizeof(SCROLLINFO);
        scroll_info.fMask = SIF_ALL;
        scroll_info.nPos = scrollPos;
        scroll_info.nMin = scrollMin;
        scroll_info.nMax = (scrollMax == -1 ? this->getMaximumVerticalScrollingPosition() : scrollMax);
        scroll_info.nPage = (scrollPage == -1 ? rect_container.bottom - rect_container.top : scrollPage);
        SendMessageW(this->pVerticalScrollbarWindow->hWnd, SBM_SETSCROLLINFO, redrawScrollbar, reinterpret_cast<LPARAM>(&scroll_info));

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyContainer\" | FUNC: \"updateScrollingInformations()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
void MyContainer::registerNewHorizontalLine(HWND hWnd)
{
    this->scrollManager.registerNewHorizontalLine(hWnd);
}