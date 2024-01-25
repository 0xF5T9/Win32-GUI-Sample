/**
 * @file window.cpp
 * @brief Implementation of the user-defined types defined in window.h.
 */

#include "../Headers/standard_includes.h" // Include standard libraries.
#include "../Headers/my_includes.h"       // Include my user-defined type headers.

/***************************************************
 * Window configuration structures implementations *
 ***************************************************/

MyStandardButtonWindowConfig::MyStandardButtonWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, bool useTabStop, std::wstring windowText)
    : parent(parent), posX(posX), posY(posY), width(width), height(height), id(id), visibleByDefault(visibleByDefault), useTabStop(useTabStop), windowText(windowText) {}
bool MyStandardButtonWindowConfig::isValid()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(this->parent))
        {
            error_message = "Invalid parent.";
            break;
        }

        if (!this->id)
        {
            error_message = "Invalid id.";
            break;
        }

        if (!this->width || !this->height)
        {
            error_message = "Invalid width/height value.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[STRUCT: 'MyStandardButtonWindowConfig' | FUNC: 'isValid()']", MyLogType::Error);

    return are_all_operation_success;
}

MyImageButtonWindowConfig::MyImageButtonWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, MyImageButtonSubclassConfig *pSubclassConfig, bool visibleByDefault, bool useTabStop, std::wstring windowText)
    : parent(parent), posX(posX), posY(posY), width(width), height(height), id(id), pSubclassConfig(pSubclassConfig), visibleByDefault(visibleByDefault), useTabStop(useTabStop), windowText(windowText) {}
bool MyImageButtonWindowConfig::isValid()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(this->parent))
        {
            error_message = "Invalid parent.";
            break;
        }

        if (!this->id)
        {
            error_message = "Invalid id.";
            break;
        }

        if (!this->width || !this->height)
        {
            error_message = "Invalid width/height value.";
            break;
        }

        if (!pSubclassConfig || !pSubclassConfig->isValid())
        {
            error_message = "Invalid subclass configuration structure.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[STRUCT: 'MyImageButtonWindowConfig' | FUNC: 'isValid()']", MyLogType::Error);

    return are_all_operation_success;
}

MyRadioButtonWindowConfig::MyRadioButtonWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, bool useTabStop, std::wstring windowText)
    : parent(parent), posX(posX), posY(posY), width(width), height(height), id(id), visibleByDefault(visibleByDefault), useTabStop(useTabStop), windowText(windowText) {}
bool MyRadioButtonWindowConfig::isValid()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(this->parent))
        {
            error_message = "Invalid parent.";
            break;
        }

        if (!this->id)
        {
            error_message = "Invalid id.";
            break;
        }

        if (!this->width || !this->height)
        {
            error_message = "Invalid width/height value.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[STRUCT: 'MyRadioButtonWindowConfig' | FUNC: 'isValid()']", MyLogType::Error);

    return are_all_operation_success;
}

MyEditboxWindowConfig::MyEditboxWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, MyEditboxType editboxType, MyEditboxSubclassConfig *pSubclassConfig, bool visibleByDefault, bool useTabStop, std::wstring windowText)
    : parent(parent), posX(posX), posY(posY), width(width), height(height), id(id), editboxType(editboxType), pSubclassConfig(pSubclassConfig), visibleByDefault(visibleByDefault), useTabStop(useTabStop), windowText(windowText) {}
bool MyEditboxWindowConfig::isValid()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(this->parent))
        {
            error_message = "Invalid parent.";
            break;
        }

        if (!this->id)
        {
            error_message = "Invalid id.";
            break;
        }

        if (!this->width || !this->height)
        {
            error_message = "Invalid width/height value.";
            break;
        }

        if (!pSubclassConfig || !pSubclassConfig->isValid())
        {
            error_message = "Invalid subclass configuration structure.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[STRUCT: 'MyEditboxWindowConfig' | FUNC: 'isValid()']", MyLogType::Error);

    return are_all_operation_success;
}

MyDDLComboboxWindowConfig::MyDDLComboboxWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, bool useTabStop)
    : parent(parent), posX(posX), posY(posY), width(width), height(height), id(id), visibleByDefault(visibleByDefault), useTabStop(useTabStop) {}
bool MyDDLComboboxWindowConfig::isValid()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(this->parent))
        {
            error_message = "Invalid parent.";
            break;
        }

        if (!this->id)
        {
            error_message = "Invalid id.";
            break;
        }

        if (!this->width || !this->height)
        {
            error_message = "Invalid width/height value.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[STRUCT: 'MyDDLComboboxWindowConfig' | FUNC: 'isValid()']", MyLogType::Error);

    return are_all_operation_success;
}

MyVerticalScrollbarWindowConfig::MyVerticalScrollbarWindowConfig(MyContainer *pContainer, HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault)
    : pContainer(pContainer), parent(parent), posX(posX), posY(posY), width(width), height(height), id(id), visibleByDefault(visibleByDefault) {}
bool MyVerticalScrollbarWindowConfig::isValid()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!pContainer || !pContainer->container())
        {
            error_message = "Invalid scroll window.";
            break;
        }

        if (!IsWindow(this->parent))
        {
            error_message = "Invalid parent.";
            break;
        }

        if (!this->id)
        {
            error_message = "Invalid id.";
            break;
        }

        if (!this->width || !this->height)
        {
            error_message = "Invalid width/height value.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[STRUCT: 'MyVerticalScrollbarWindowConfig' | FUNC: 'isValid()']", MyLogType::Error);

    return are_all_operation_success;
}

MyContainerWindowConfig::MyContainerWindowConfig(HWND parent, INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault, SUBCLASSPROC windowProcedure)
    : parent(parent), posX(posX), posY(posY), width(width), height(height), id(id), visibleByDefault(visibleByDefault), windowProcedure(windowProcedure) {}
bool MyContainerWindowConfig::isValid()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(this->parent))
        {
            error_message = "Invalid parent.";
            break;
        }

        if (!this->id)
        {
            error_message = "Invalid id.";
            break;
        }

        if (!this->width || !this->height)
        {
            error_message = "Invalid width/height value.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[STRUCT: 'MyContainerWindowConfig' | FUNC: 'isValid()']", MyLogType::Error);

    return are_all_operation_success;
}

/**********************************
 * Window classes implementations *
 **********************************/

// [MyWindow] class implementations:

MyWindow::MyWindow() : windowType(MyWindowType::Undefined)
{
    ++MyWindow::totalInstances;
}
MyWindow::~MyWindow()
{
    if (!this->isWindowExists)
        return;

    switch (this->windowType)
    {
    case MyWindowType::Custom:
    {
        if (this->pData)
            g_pApp->logger.writeLog("This pointer should be set to nullptr before the window is destroyed.", "[CLASS: 'MyWindow' | FUNC: 'Destructor']", MyLogType::Warn);

        break;
    }
    case MyWindowType::StandardButton:
    {
        delete static_cast<MyStandardButtonSubclass *>(this->pData);
        this->pData = nullptr;
        break;
    }
    case MyWindowType::ImageButton:
    {
        delete static_cast<MyImageButtonSubclass *>(this->pData);
        this->pData = nullptr;
        break;
    }
    case MyWindowType::RadioButton:
    {
        delete static_cast<MyRadioButtonSubclass *>(this->pData);
        this->pData = nullptr;
        break;
    }
    case MyWindowType::Editbox:
    {
        delete static_cast<MyRadioButtonSubclass *>(this->pData);
        this->pData = nullptr;
        break;
    }
    case MyWindowType::DDLCombobox:
    {
        delete static_cast<MyDDLComboboxSubclass *>(this->pData);
        this->pData = nullptr;
        break;
    }
    case MyWindowType::VerticalScrollbar:
    {
        delete static_cast<MyVerticalScrollbarSubclass *>(this->pData);
        this->pData = nullptr;
        break;
    }
    default:
        break;
    }

    --MyWindow::totalInstances;
}
HWND MyWindow::hWnd()
{
    return (this->isWindowExists ? *this->pWindow : nullptr);
}
MyWindowType MyWindow::type()
{
    return this->windowType;
}
void *&MyWindow::data()
{
    return this->pData;
}
bool MyWindow::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->isWindowExists)
        {
            error_message = "The window is not created yet.";
            break;
        }

        bool is_switch_success = false;
        switch (this->type())
        {
        case MyWindowType::Custom:
        {
            is_switch_success = true;
            break;
        }
        case MyWindowType::StandardButton:
        {
            if (!static_cast<MyStandardButtonSubclass *>(this->pData)->refresh())
                break;
            is_switch_success = true;
            break;
        }
        case MyWindowType::ImageButton:
        {
            if (!static_cast<MyImageButtonSubclass *>(this->pData)->refresh())
                break;
            is_switch_success = true;
            break;
        }
        case MyWindowType::RadioButton:
        {
            if (!static_cast<MyRadioButtonSubclass *>(this->pData)->refresh())
                break;
            is_switch_success = true;
            break;
        }
        case MyWindowType::Editbox:
        {
            if (!static_cast<MyEditboxSubclass *>(this->pData)->refresh())
                break;
            is_switch_success = true;
            break;
        }
        case MyWindowType::DDLCombobox:
        {
            if (!static_cast<MyDDLComboboxSubclass *>(this->pData)->refresh())
                break;
            is_switch_success = true;
            break;
        }
        case MyWindowType::VerticalScrollbar:
        {
            if (!static_cast<MyVerticalScrollbarSubclass *>(this->pData)->refresh())
                break;
            is_switch_success = true;
            break;
        }
        default:
            break;
        }
        if (!is_switch_success)
        {
            error_message = "Failed to refresh the window.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWindow::createCustomWindow(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle,
                                  int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isWindowExists)
        {
            error_message = "The window is already exists.";
            break;
        }

        this->pWindow.reset(new HWND(nullptr));
        auto &window = *this->pWindow;

        window = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle,
                                 X, Y,
                                 nWidth, nHeight,
                                 hWndParent, hMenu, hInstance, lpParam);
        if (!window)
        {
            error_message = "Failed to create the window.";
            break;
        }

        this->windowType = MyWindowType::Custom;

        this->isWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'createCustomWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWindow::createStandardButton(MyStandardButtonWindowConfig &windowConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isWindowExists)
        {
            error_message = "The window is already exists.";
            break;
        }

        if (!windowConfig.isValid())
        {
            error_message = "The window configuration contains invalid parameters.";
            break;
        }

        this->pWindow.reset(new HWND(nullptr));
        auto &window = *this->pWindow;

        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS;
        if (!windowConfig.visibleByDefault)
            window_flags &= ~WS_VISIBLE;
        if (!windowConfig.useTabStop)
            window_flags &= ~WS_TABSTOP;

        window = CreateWindowExW(0, WC_BUTTON, windowConfig.windowText.c_str(), window_flags,
                                 windowConfig.posX, windowConfig.posY,
                                 windowConfig.width, windowConfig.height,
                                 windowConfig.parent, windowConfig.id, NULL, NULL);
        if (!window)
        {
            error_message = "Failed to create the window.";
            break;
        }

        this->pData = new MyStandardButtonSubclass();
        if (!static_cast<MyStandardButtonSubclass *>(this->pData)->setWindow(window))
        {
            delete static_cast<MyStandardButtonSubclass *>(this->pData);
            this->pData = nullptr;
            error_message = "Failed to associate the subclass object to the window.";
            break;
        }

        this->windowType = MyWindowType::StandardButton;

        this->isWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'createStandardButton()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWindow::createImageButton(MyImageButtonWindowConfig &windowConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isWindowExists)
        {
            error_message = "The window is already exists.";
            break;
        }

        if (!windowConfig.isValid())
        {
            error_message = "The window configuration contains invalid parameters.";
            break;
        }

        this->pWindow.reset(new HWND(nullptr));
        auto &window = *this->pWindow;

        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS;
        if (!windowConfig.visibleByDefault)
            window_flags &= ~WS_VISIBLE;
        if (!windowConfig.useTabStop)
            window_flags &= ~WS_TABSTOP;

        window = CreateWindowExW(0, WC_BUTTON, windowConfig.windowText.c_str(), window_flags,
                                 windowConfig.posX, windowConfig.posY,
                                 windowConfig.width, windowConfig.height,
                                 windowConfig.parent, windowConfig.id, NULL, NULL);
        if (!window)
        {
            error_message = "Failed to create the window.";
            break;
        }

        this->pData = new MyImageButtonSubclass();
        if (!static_cast<MyImageButtonSubclass *>(this->pData)->setWindow(window, windowConfig.pSubclassConfig))
        {
            delete static_cast<MyImageButtonSubclass *>(this->pData);
            this->pData = nullptr;
            error_message = "Failed to associate the subclass object to the window.";
            break;
        }

        this->windowType = MyWindowType::ImageButton;

        this->isWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'createImageButton()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWindow::createRadioButton(MyRadioButtonWindowConfig &windowConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isWindowExists)
        {
            error_message = "The window is already exists.";
            break;
        }

        if (!windowConfig.isValid())
        {
            error_message = "The window configuration contains invalid parameters.";
            break;
        }

        this->pWindow.reset(new HWND(nullptr));
        auto &window = *this->pWindow;

        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS;
        if (!windowConfig.visibleByDefault)
            window_flags &= ~WS_VISIBLE;
        if (!windowConfig.useTabStop)
            window_flags &= ~WS_TABSTOP;

        window = CreateWindowExW(0, WC_BUTTON, windowConfig.windowText.c_str(), window_flags,
                                 windowConfig.posX, windowConfig.posY,
                                 windowConfig.width, windowConfig.height,
                                 windowConfig.parent, windowConfig.id, NULL, NULL);
        if (!window)
        {
            error_message = "Failed to create the window.";
            break;
        }

        this->pData = new MyRadioButtonSubclass();
        if (!static_cast<MyRadioButtonSubclass *>(this->pData)->setWindow(window))
        {
            delete static_cast<MyRadioButtonSubclass *>(this->pData);
            this->pData = nullptr;
            error_message = "Failed to associate the subclass object to the window.";
            break;
        }

        this->windowType = MyWindowType::RadioButton;

        this->isWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'createRadioButton()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWindow::createEditbox(MyEditboxWindowConfig &windowConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isWindowExists)
        {
            error_message = "The window is already exists.";
            break;
        }

        if (!windowConfig.isValid())
        {
            error_message = "The window configuration contains invalid parameters.";
            break;
        }

        this->pWindow.reset(new HWND(nullptr));
        auto &window = *this->pWindow;

        DWORD window_flags = 0;
        switch (windowConfig.editboxType)
        {
        case MyEditboxType::Singleline:
        {
            window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | WS_CLIPSIBLINGS;
            break;
        }
        case MyEditboxType::SinglelinePassword:
        {
            window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD | WS_CLIPSIBLINGS;
            break;
        }
        case MyEditboxType::Multiline:
        {
            window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_CLIPSIBLINGS;
            break;
        }
        }
        if (!windowConfig.visibleByDefault)
            window_flags &= ~WS_VISIBLE;
        if (!windowConfig.useTabStop)
            window_flags &= ~WS_TABSTOP;

        window = CreateWindowExW(0, MSFTEDIT_CLASS, windowConfig.windowText.c_str(), window_flags,
                                 windowConfig.posX + 2, windowConfig.posY + 2,
                                 windowConfig.width - 4, windowConfig.height - 4,
                                 windowConfig.parent, windowConfig.id, NULL, NULL);
        if (!window)
        {
            error_message = "Failed to create the window.";
            break;
        }

        this->pData = new MyEditboxSubclass();
        HWND &window_static = static_cast<MyEditboxSubclass *>(this->pData)->getStaticHandleRef();
        window_static = CreateWindowExW(0, WC_STATIC, L"", WS_VISIBLE | WS_CHILD | SS_NOPREFIX | WS_CLIPSIBLINGS,
                                        windowConfig.posX, windowConfig.posY,
                                        windowConfig.width, windowConfig.height,
                                        windowConfig.parent, windowConfig.pSubclassConfig->staticWindowID, NULL, NULL);
        if (!window_static)
        {
            error_message = "Failed to create the window. (static)";
            break;
        }

        if (!static_cast<MyEditboxSubclass *>(this->pData)->setWindow(window, windowConfig.pSubclassConfig))
        {
            delete static_cast<MyEditboxSubclass *>(this->pData);
            this->pData = nullptr;
            error_message = "Failed to associate the subclass object to the window.";
            break;
        }

        this->windowType = MyWindowType::Editbox;

        this->isWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'createEditbox()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWindow::createDDLCombobox(MyDDLComboboxWindowConfig &windowConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isWindowExists)
        {
            error_message = "The window is already exists.";
            break;
        }

        if (!windowConfig.isValid())
        {
            error_message = "The window configuration contains invalid parameters.";
            break;
        }

        this->pWindow.reset(new HWND(nullptr));
        auto &window = *this->pWindow;

        DWORD window_flags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED | WS_CLIPSIBLINGS;
        if (!windowConfig.visibleByDefault)
            window_flags &= ~WS_VISIBLE;
        if (!windowConfig.useTabStop)
            window_flags &= ~WS_TABSTOP;

        window = CreateWindowExW(0, WC_COMBOBOX, L"", window_flags,
                                 windowConfig.posX, windowConfig.posY,
                                 windowConfig.width, windowConfig.height,
                                 windowConfig.parent, windowConfig.id, NULL, NULL);
        if (!window)
        {
            error_message = "Failed to create the window.";
            break;
        }

        this->pData = new MyDDLComboboxSubclass();
        if (!static_cast<MyDDLComboboxSubclass *>(this->pData)->setWindow(window, windowConfig.height))
        {
            delete static_cast<MyDDLComboboxSubclass *>(this->pData);
            this->pData = nullptr;
            error_message = "Failed to associate the subclass object to the window.";
            break;
        }

        this->windowType = MyWindowType::DDLCombobox;

        this->isWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'createDDLCombobox()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWindow::createVerticalScrollbar(MyVerticalScrollbarWindowConfig &windowConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isWindowExists)
        {
            error_message = "The window is already exists.";
            break;
        }

        if (!windowConfig.isValid())
        {
            error_message = "The window configuration contains invalid parameters.";
            break;
        }

        this->pWindow.reset(new HWND(nullptr));
        auto &window_scrollbar = *this->pWindow;

        DWORD window_scrollbar_flags = WS_CHILD | SBS_VERT;
        DWORD window_static_flags = WS_VISIBLE | WS_CHILD | SS_NOPREFIX | SS_NOTIFY | WS_CLIPSIBLINGS;
        if (!windowConfig.visibleByDefault)
            window_static_flags &= ~WS_VISIBLE;

        this->pData = new MyVerticalScrollbarSubclass();
        HWND &window_static = static_cast<MyVerticalScrollbarSubclass *>(this->pData)->getStaticHandleRef();

        window_static = CreateWindowExW(0, WC_STATIC, L"", window_static_flags,
                                        windowConfig.posX, windowConfig.posY,
                                        windowConfig.width, windowConfig.height,
                                        windowConfig.parent, NULL, NULL, NULL);
        if (!window_static)
        {
            error_message = "Failed to create the window. (static)";
            break;
        }

        window_scrollbar = CreateWindowExW(0, WC_SCROLLBAR, L"", window_scrollbar_flags,
                                           0, 0,
                                           0, 0,
                                           windowConfig.parent, windowConfig.id, NULL, NULL);
        if (!window_scrollbar)
        {
            error_message = "Failed to create the window. (scrollbar)";
            break;
        }

        if (!static_cast<MyVerticalScrollbarSubclass *>(this->pData)->setWindow(window_scrollbar, windowConfig.pContainer))
        {
            delete static_cast<MyVerticalScrollbarSubclass *>(this->pData);
            this->pData = nullptr;
            error_message = "Failed to associate the subclass object to the window.";
            break;
        }

        this->windowType = MyWindowType::VerticalScrollbar;

        this->isWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWindow' | FUNC: 'createVerticalScrollbar()']", MyLogType::Error);

    return are_all_operation_success;
}

// [MyContainer] class implementations:

MyContainer::MyContainer()
{
    ++MyContainer::totalInstances;
}
MyContainer::~MyContainer()
{
    this->mWindows.clear();

    this->pVerticalScrollbarWindow.reset();
    this->pContainerWindow.reset();

    --MyContainer::totalInstances;
}
MyWindow *MyContainer::container()
{
    return (this->isContainerWindowExists ? pContainerWindow.get() : nullptr);
}
MyWindow *MyContainer::verticalScrollbar()
{
    return (this->isContainerWindowExists && this->isVerticalScrollbarWindowExists ? this->pVerticalScrollbarWindow.get() : nullptr);
}
bool MyContainer::refresh(bool redrawContainerWindow)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        {
            bool is_for_loop_failed = false;
            for (auto &[key, p_window] : this->mWindows)
            {
                if (!p_window->refresh())
                {
                    is_for_loop_failed = true;
                    break;
                }
            }
            if (is_for_loop_failed)
            {
                error_message = "Failed to refresh the container window.";
                break;
            }
        }

        if (this->isVerticalScrollbarWindowExists)
        {
            if (!this->verticalScrollbar()->refresh())
            {
                error_message = "Failed to refresh the vertical scrollbar window.";
                break;
            }
        }

        if (redrawContainerWindow)
            RedrawWindow(this->container()->hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyContainer::updateContainerDimensions(INT posX, INT posY, INT width, INT height, bool hideScrollbar, bool resetScrollPosition)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->isContainerWindowExists)
        {
            error_message = "The container window must be created first.";
            break;
        }

        // If the container have a vertical scrollbar, get the scrollbar dimensions
        // and pointer to its subclass object, the subclass object contains handle to the scrollbar associated static window.
        // The scrollbar associated static window represents the scrollbar appearance.
        INT vertical_scrollbar_width = 0,
            vertical_scrollbar_height = 0;
        MyVerticalScrollbarSubclass *p_vertical_scrollbar_subclass = nullptr;
        HWND vertical_scrollbar_static = nullptr;
        if (this->isVerticalScrollbarWindowExists)
        {
            // Get the scrollbar subclass object pointer and the associated static window handle.
            p_vertical_scrollbar_subclass = static_cast<MyVerticalScrollbarSubclass *>(this->pVerticalScrollbarWindow->data());
            vertical_scrollbar_static = p_vertical_scrollbar_subclass->getStaticHandleRef();

            // Get the scrollbar dimensions.
            RECT rect_vertical_scrollbar;
            if (!GetClientRect(vertical_scrollbar_static, &rect_vertical_scrollbar))
            {
                error_message = "Failed to retrieve the container's vertical scrollbar client rect.";
                break;
            }
            vertical_scrollbar_width = rect_vertical_scrollbar.right - rect_vertical_scrollbar.left;
            vertical_scrollbar_height = rect_vertical_scrollbar.bottom - rect_vertical_scrollbar.top;
        }

        // Prepare an HDWP struct to update the dimensions of the container and its scrollbar all at once.
        HDWP hdwp_controls = BeginDeferWindowPos((this->isVerticalScrollbarWindowExists ? 2 : 1));

        // Update the container dimensions.
        DeferWindowPos(hdwp_controls, this->container()->hWnd(), NULL, posX, posY,
                       (hideScrollbar ? width : width - vertical_scrollbar_width), // Make space for the scrollbar if needed.
                       height,
                       SWP_NOZORDER);

        // Update the scrollbar dimensions.
        if (this->isVerticalScrollbarWindowExists)
        {
            // Update new scrollbar's height.
            if (height > vertical_scrollbar_height || height < vertical_scrollbar_height)
            {
                DeferWindowPos(hdwp_controls, vertical_scrollbar_static, NULL,
                               (posX + width) - vertical_scrollbar_width,
                               posY,
                               vertical_scrollbar_width,
                               height,
                               SWP_NOZORDER);
            }

            // Update new scrollbar's x position.
            else
            {
                DeferWindowPos(hdwp_controls, vertical_scrollbar_static, NULL,
                               (posX + width) - vertical_scrollbar_width,
                               posY,
                               0,
                               0,
                               SWP_NOSIZE | SWP_NOZORDER);
            }
        }

        // Update the dimensions of the container and its scrollbar all at once.
        if (!EndDeferWindowPos(hdwp_controls))
        {
            error_message = "Failed to update the container and its scrollbar dimensions.";
            break;
        }

        // Update scroll informations for the container.
        if (this->isVerticalScrollbarWindowExists)
        {
            // Get the container new dimensions.
            RECT rect_container;
            if (!GetClientRect(this->container()->hWnd(), &rect_container))
            {
                error_message = "Failed to retrieve the container's client rect.";
                break;
            }

            // Get the container scroll information struct.
            SCROLLINFO scroll_info;
            scroll_info.cbSize = sizeof(SCROLLINFO);
            scroll_info.fMask = SIF_ALL;
            if (!SendMessageW(this->pVerticalScrollbarWindow->hWnd(), SBM_GETSCROLLINFO, 0, reinterpret_cast<LPARAM>(&scroll_info)))
            {
                error_message = "Failed to retrieve the container scroll information struct.";
                break;
            }

            // New container height dimension equivalent to new page size.
            INT new_container_height = rect_container.bottom - rect_container.top;
            scroll_info.nPage = new_container_height;

            // Scroll the container back to the zero pos and reset the scroll pos.
            if (resetScrollPosition)
            {
                ScrollWindowEx(this->container()->hWnd(), 0, scroll_info.nPos, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
                scroll_info.nPos = 0;
            }

            // Set scroll info.
            SendMessageW(this->pVerticalScrollbarWindow->hWnd(), SBM_SETSCROLLINFO, FALSE, reinterpret_cast<LPARAM>(&scroll_info));

            // Update container scrollbar visibility.
            ShowWindow(vertical_scrollbar_static, (new_container_height > scroll_info.nMax ? SW_HIDE : (hideScrollbar ? SW_HIDE : SW_SHOW)));
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'updateContainerDimensions()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyContainer::addWindow(std::shared_ptr<MyWindow> pWindow)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->isContainerWindowExists)
        {
            error_message = "The container window must be created first.";
            break;
        }

        if (!pWindow || !pWindow->hWnd())
        {
            error_message = "The window pointer is invalid. (null or empty window)";
            break;
        }

        if (GetDlgCtrlID(this->container()->hWnd()) != GetDlgCtrlID(pWindow->hWnd()))
        {
            error_message = "The window must be a direct child of the container window.";
        }

        auto insert_result = this->mWindows.insert(std::make_pair(GetDlgCtrlID(pWindow->hWnd()), pWindow));
        if (!insert_result.second)
        {
            error_message = "A window with the same ID already exists in the container. All windows IDs should be unique.";
            break;
        }

        if (MyContainer::autoUpdateScrollInformation)
        {
            if (this->isVerticalScrollbarWindowExists)
            {
                SCROLLINFO scroll_info;
                if (!MyUtility::GetScrollInfo(this->verticalScrollbar()->hWnd(), scroll_info))
                {
                    error_message = "Failed to get the scroll information.";
                    break;
                }

                if (scroll_info.nPos)
                {
                    int pos_x = 0, pos_y = 0;
                    MyUtility::GetWindowRelativePosition(pWindow->hWnd(), pos_x, pos_y);
                    SetWindowPos(pWindow->hWnd(), NULL, pos_x, pos_y - scroll_info.nPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                }

                if (!this->updateContainerScrollInformation())
                {
                    error_message = "Failed to update the container scroll information.";
                    break;
                }
            }
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'addWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyContainer::removeWindow(INT windowID)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        auto it = this->mWindows.find(windowID);
        if (it != this->mWindows.end())
        {
            this->mWindows.erase(it);
            are_all_operation_success = true;
            break;
        }

        error_message = "Target window not found.";
        break;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'removeWindow()']", MyLogType::Error);
    else
    {
        if (MyContainer::autoUpdateScrollInformation)
            if (this->isVerticalScrollbarWindowExists)
                if (!this->updateContainerScrollInformation())
                    g_pApp->logger.writeLog("Failed to update the container scroll information.", "[CLASS: 'MyContainer' | FUNC: 'removeWindow()']", MyLogType::Error);
    }

    return are_all_operation_success;
}
std::shared_ptr<MyWindow> MyContainer::findWindow(INT windowID)
{
    for (auto &[id, p_window] : this->mWindows)
    {
        if (windowID == id)
            return p_window;
    }

    return nullptr;
}
bool MyContainer::scrollContainer(INT scrollAmount, bool scrollAlternative)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        scrollAmount = -scrollAmount;

        if (!this->isContainerWindowExists)
        {
            error_message = "The container window must be created first.";
            break;
        }

        if (!scrollAlternative) // Scroll the window using ScrollWindowEx().
        {
            RECT rect_window;
            if (!GetClientRect(this->container()->hWnd(), &rect_window))
            {
                error_message = "Failed to get the window client rect.";
                break;
            }

            std::unique_ptr<HRGN, HRGNDeleter> pRegion(new HRGN());
            *pRegion = CreateRectRgn(rect_window.left, rect_window.top, rect_window.right, rect_window.bottom);
            if (!*pRegion)
            {
                error_message = "Failed to create the redraw region.";
                break;
            }

            ScrollWindowEx(this->container()->hWnd(), 0, scrollAmount, nullptr, nullptr, *pRegion, nullptr, SW_SCROLLCHILDREN);
            RedrawWindow(this->container()->hWnd(), 0, *pRegion, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
        }
        else // Scroll the window using DeferWindowPos().
        {
            int total_child_windows = 0;
            {
                auto child_window = GetWindow(this->container()->hWnd(), GW_CHILD);
                while (child_window)
                {
                    ++total_child_windows;
                    child_window = GetWindow(child_window, GW_HWNDNEXT);
                }
            }

            HDWP hdwp_childs = BeginDeferWindowPos(total_child_windows);
            auto child_window = GetWindow(this->container()->hWnd(), GW_CHILD);
            bool is_while_loop_failed = false;
            while (child_window)
            {
                INT pos_x = 0, pos_y = 0;
                if (!MyUtility::GetWindowRelativePosition(child_window, pos_x, pos_y))
                {
                    is_while_loop_failed = true;
                    break;
                }
                DeferWindowPos(hdwp_childs, child_window, NULL, pos_x, pos_y + scrollAmount, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
                child_window = GetWindow(child_window, GW_HWNDNEXT);
            }
            if (is_while_loop_failed)
            {
                error_message = "Failed to retrieve the child window relative positions.";
                break;
            }
            EndDeferWindowPos(hdwp_childs);
            child_window = GetWindow(this->container()->hWnd(), GW_CHILD);
            while (child_window)
            {
                RedrawWindow(child_window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
                child_window = GetWindow(child_window, GW_HWNDNEXT);
            }
            RedrawWindow(this->container()->hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'scrollContainer()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyContainer::setScrollBottomPadding(UINT scrollBottomPadding)
{
    this->scrollBottomPadding = scrollBottomPadding;

    if (this->isVerticalScrollbarWindowExists)
    {
        if (!this->updateContainerScrollInformation())
            return false;
    }

    return true;
}
bool MyContainer::createContainerWindow(MyContainerWindowConfig &containerConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->isContainerWindowExists)
        {
            error_message = "The container window is already exists.";
            break;
        }

        if (!containerConfig.isValid())
        {
            error_message = "The container configuration contains invalid parameters.";
            break;
        }

        this->pContainerWindow.reset(new MyWindow());
        auto &p_container_window = *this->pContainerWindow;

        DWORD window_flags = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | SS_NOPREFIX;
        if (!containerConfig.visibleByDefault)
            window_flags &= ~WS_VISIBLE;

        if (!p_container_window.createCustomWindow(WS_EX_CONTROLPARENT, WC_STATIC, L"", window_flags,
                                                   containerConfig.posX, containerConfig.posY,
                                                   containerConfig.width, containerConfig.height,
                                                   containerConfig.parent, containerConfig.id, NULL, NULL))
        {
            error_message = "Failed to create the container window.";
            break;
        }

        if (containerConfig.windowProcedure)
        {
            if (!SetWindowSubclass(p_container_window.hWnd(), containerConfig.windowProcedure, 0, 0))
            {
                error_message = "Failed to install the container window procedure.";
                break;
            }
        }

        this->isContainerWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'createContainerWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyContainer::createVerticalScrollbarWindow(INT posX, INT posY, INT width, INT height, HMENU id, bool visibleByDefault)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->isContainerWindowExists)
        {
            error_message = "The container window must be created first.";
            break;
        }

        if (this->isVerticalScrollbarWindowExists)
        {
            error_message = "The vertical scrollbar window is already exists.";
            break;
        }

        this->pVerticalScrollbarWindow.reset(new MyWindow());
        auto &p_scrollbar_window = *this->pVerticalScrollbarWindow;

        MyVerticalScrollbarWindowConfig window_config(this, GetParent(this->container()->hWnd()),
                                                      posX, posY,
                                                      width, height,
                                                      (HMENU)id, visibleByDefault);
        if (!window_config.isValid())
        {
            error_message = "The container configuration contains invalid parameters.";
            break;
        }

        if (!p_scrollbar_window.createVerticalScrollbar(window_config))
        {
            error_message = "Failed to create the scrollbar window..";
            break;
        }

        if (!this->updateContainerScrollInformation())
        {
            error_message = "Failed update the scroll information for the target window.";
            break;
        }

        this->isVerticalScrollbarWindowExists = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'createVerticalScrollbarWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
INT MyContainer::getMaximumVerticalScrollingPosition()
{
    int maximum_vertical_scrolling_position = 0;
    for (auto &[id, p_window] : this->mWindows)
    {
        int x = 0, y = 0, w = 0, h = 0;
        MyUtility::GetWindowRelativePosition(p_window->hWnd(), x, y);
        MyUtility::GetWindowDimensions(p_window->hWnd(), w, h);
        auto c = y + h;
        if (c > maximum_vertical_scrolling_position)
            maximum_vertical_scrolling_position = c;
    }

    return maximum_vertical_scrolling_position + this->scrollBottomPadding;
}
bool MyContainer::updateContainerScrollInformation()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->isContainerWindowExists)
        {
            error_message = "The container window must be created first.";
            break;
        }

        if (!this->pVerticalScrollbarWindow || !(*this->pVerticalScrollbarWindow).hWnd())
        {
            error_message = "The scrollbar window is invalid.";
            break;
        }
        auto scrollbar_window = (*this->pVerticalScrollbarWindow).hWnd();

        RECT rect_container;
        if (!GetClientRect(this->container()->hWnd(), &rect_container))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        auto container_page = rect_container.bottom - rect_container.top;
        SCROLLINFO scroll_info;
        if (!MyUtility::GetScrollInfo(scrollbar_window, scroll_info))
        {
            error_message = "Failed to get the scroll information.";
            break;
        }
        ScrollWindowEx(this->container()->hWnd(), 0, scroll_info.nPos, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE);
        scroll_info.nMin = 0;
        scroll_info.nMax = this->getMaximumVerticalScrollingPosition();
        scroll_info.nPage = container_page;
        scroll_info.nPos = 0;
        SendMessageW(scrollbar_window, SBM_SETSCROLLINFO, false, reinterpret_cast<LPARAM>(&scroll_info));

        auto p_subclass = MyVerticalScrollbarSubclass::getSubclassPointer(scrollbar_window);
        if (!p_subclass)
        {
            error_message = "Invalid subclass pointer.";
            break;
        }
        RedrawWindow(p_subclass->getStaticHandleRef(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        if (container_page >= scroll_info.nMax)
            ShowWindow(p_subclass->getStaticHandleRef(), SW_HIDE);
        else
            ShowWindow(p_subclass->getStaticHandleRef(), SW_SHOW);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyContainer' | FUNC: 'updateContainerScrollInformation()']", MyLogType::Error);

    return are_all_operation_success;
}