/**
 * @file subclass.cpp
 * @brief Implementation of the user-defined types defined in subclass.h.
 */

#include "../Headers/standard_includes.h" // Include standard libraries.
#include "../Headers/my_includes.h"       // Include my user-defined type headers.

/*****************************************************
 * Subclass configuration structures implementations *
 *****************************************************/

MyImageButtonSubclassConfig::MyImageButtonSubclassConfig() {}
MyImageButtonSubclassConfig::MyImageButtonSubclassConfig(MyWICBitmap *pImageDefault, MyWICBitmap *pImageHover, MyWICBitmap *pImageActive, MyColor *pColorBackgroundDefault, MyColor *pColorBackgroundHover, MyColor *pColorBackgroundActive, FLOAT imagePosX, FLOAT imagePosY, FLOAT imageWidth, FLOAT imageHeight)
    : pImageDefault(pImageDefault), pImageHover(pImageHover), pImageActive(pImageActive), pColorBackgroundDefault(pColorBackgroundDefault), pColorBackgroundHover(pColorBackgroundHover), pColorBackgroundActive(pColorBackgroundActive), imagePosX(imagePosX), imagePosY(imagePosY), imageWidth(imageWidth), imageHeight(imageHeight) {}
bool MyImageButtonSubclassConfig::isValid()
{
    if (!pImageDefault || !pImageHover || !pImageActive ||
        !pColorBackgroundDefault || !pColorBackgroundHover || !pColorBackgroundActive)
    {
        g_pApp->logger.writeLog("At least one of the required resource pointers is null.", "[STRUCT: 'MyImageButtonSubclassConfig' | FUNC: 'isValid()']", MyLogType::Error);
        return false;
    }

    if (imageWidth < 1 || imageHeight < 1)
    {
        g_pApp->logger.writeLog("Invalid image width/height.", "[STRUCT: 'MyImageButtonSubclassConfig' | FUNC: 'isValid()']", MyLogType::Error);
        return false;
    }

    return true;
}

MyEditboxSubclassConfig::MyEditboxSubclassConfig() {}
MyEditboxSubclassConfig::MyEditboxSubclassConfig(HMENU staticWindowID)
    : staticWindowID(staticWindowID) {}
bool MyEditboxSubclassConfig::isValid()
{
    if (!staticWindowID)
    {
        g_pApp->logger.writeLog("The static window ID is null.", "[STRUCT: 'MyEditboxSubclassConfig' | FUNC: 'isValid()']", MyLogType::Error);
        return false;
    }

    return true;
}

MyStandardTextSubclassConfig::MyStandardTextSubclassConfig() {}
MyStandardTextSubclassConfig::MyStandardTextSubclassConfig(int centerMode, int posX, int posY, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, float fontScale, MyColor *pTextColor)
    : centerMode(centerMode), posX(posX), posY(posY), fontWeight(fontWeight), fontStyle(fontStyle), fontScale(fontScale), pTextColor(pTextColor) {}
bool MyStandardTextSubclassConfig::isValid()
{
    if (this->centerMode < 0 || this->centerMode > 3)
    {
        g_pApp->logger.writeLog("Invalid center mode.", "[STRUCT: 'MyStandardTextSubclassConfig' | FUNC: 'isValid()']", MyLogType::Error);
        return false;
    }

    if (this->fontScale < 0.1f || this->fontScale > 1.0f)
    {
        g_pApp->logger.writeLog("Invalid font scale (Min: 0.1 | Max: 1.0).", "[STRUCT: 'MyStandardTextSubclassConfig' | FUNC: 'isValid()']", MyLogType::Error);
        return false;
    }

    return true;
}

/**********************************
 * Subclass class implementations *
 **********************************/

bool InitializeSubclassClasses(std::unique_ptr<MyGraphicEngine> *pGraphic, std::unique_ptr<UIManager> *pUIManager, HWND &appWindow)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (MySubclass::isInitialized)
        {
            error_message = "The subclass base class is already initialized.";
            break;
        }

        if (!pGraphic || !pUIManager)
        {
            error_message = "One of the parameter is invalid (NULL).";
            break;
        }

        MySubclass::pAppWindow = &appWindow;
        MySubclass::ppGraphic = &*pGraphic;
        MySubclass::ppUIElements = &*pUIManager;

        MySubclass::isInitialized = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[FUNC: 'InitializeSubclassClasses()']", MyLogType::Error);

    return are_all_operation_success;
}
bool UninitializeSubclassClasses()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!MySubclass::isInitialized)
        {
            error_message = "The subclass base class is not initialized yet.";
            break;
        }

        if (MySubclass::totalInstances)
        {
            error_message = "All windows subclassed by the subclass classes must be destroyed first before uninitializing the subclass base class.";
            break;
        }

        MySubclass::pAppWindow = nullptr;
        MySubclass::ppGraphic = nullptr;
        MySubclass::ppUIElements = nullptr;

        MySubclass::isInitialized = false;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[FUNC: 'UninitializeSubclassClasses()']", MyLogType::Error);

    return are_all_operation_success;
}

// [MySubclass] class implementations:

MySubclass::MySubclass()
{
    if (!MySubclass::isInitialized)
    {
        g_pApp->logger.writeLog("The subclass base class must be initialized first.", "[CLASS: 'MySubclass' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The subclass base class must be initialized first.");
    }
    ++MySubclass::totalInstances;
}
MySubclass::~MySubclass()
{
    --MySubclass::totalInstances;
}
MyGraphicEngine *MySubclass::graphics()
{
    return (*MySubclass::ppGraphic).get();
}
UIManager *MySubclass::objects()
{
    return (*MySubclass::ppUIElements).get();
}

// [MyStandardButtonSubclass] class implementations:

bool MyStandardButtonSubclass::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass object is not associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is not associated with a window.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        // Start animation to the current animation state to update animation variables values.
        if (!this->startAnimation(this->currentAnimationState))
        {
            error_message = "Failed to start the animation.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
void MyStandardButtonSubclass::releaseSharedDeviceResources()
{
    // <This subclass class doesn't have any shared device resources>
}
void MyStandardButtonSubclass::setAnimationDuration(ButtonAnimationState animateState, FLOAT milliseconds)
{
    switch (animateState)
    {
    case ButtonAnimationState::Default:
        this->defaultAnimationDuration = milliseconds;
        break;
    case ButtonAnimationState::Hover:
        this->hoverAnimationDuration = milliseconds;
        break;
    case ButtonAnimationState::Active:
        this->activeAnimationDuration = milliseconds;
        break;
    case ButtonAnimationState::Focus:
    case ButtonAnimationState::FocusLost:
        this->focusAnimationDuration = milliseconds;
        break;
    }
}
bool MyStandardButtonSubclass::setWindow(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass base class is initialized.
        if (!this->isInitialized)
        {
            error_message = "The subclass base class is not initialized.";
            break;
        }

        // Check if the subclass object is already associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is already associated with a window.";
            break;
        }

        // Store the window handle(s).
        this->buttonWindow = hWnd;

        // Subclass the window.
        // Store this subclass object pointer as reference data, enabling the subclass procedure to access the non-static object members.
        if (!SetWindowSubclass(this->buttonWindow, &MyStandardButtonSubclass::subclassProcedure, 0, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = "Failed to install the window subclass callback.";
            break;
        }

        // Create the animation variables.
        {
            error_message = "Failed to create the animation variables.";
            this->pAnimationVariableFocusBorderOpacity.reset(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableFocusBorderOpacity, 0.0, 0.0, 1.0))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonRGB[0],
                                                                       this->objects()->colors.standardButtonDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonRGB[1],
                                                                       this->objects()->colors.standardButtonDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonRGB[2],
                                                                       this->objects()->colors.standardButtonDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonBorderRGB[0],
                                                                       this->objects()->colors.standardButtonBorderDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonBorderRGB[1],
                                                                       this->objects()->colors.standardButtonBorderDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonBorderRGB[2],
                                                                       this->objects()->colors.standardButtonBorderDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;

            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonTextRGB[0],
                                                                       this->objects()->colors.textActive.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonTextRGB[1],
                                                                       this->objects()->colors.textActive.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonTextRGB[2],
                                                                       this->objects()->colors.textActive.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            error_message = "";
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'setWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
MyStandardButtonSubclass *MyStandardButtonSubclass::getSubclassPointer(HWND hWnd)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyStandardButtonSubclass::subclassProcedure, 0, &reference_data);
    return (is_subclassed ? reinterpret_cast<MyStandardButtonSubclass *>(reference_data) : nullptr);
}
bool MyStandardButtonSubclass::createSharedDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // HRESULT hr;

        error_message = "Failed to create the shared device resources.";
        // <This subclass class doesn't have any shared device resources>
        error_message = "";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'createSharedDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyStandardButtonSubclass::createDeviceResources(bool recreateSharedResources)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        HDC hdc = GetDC(this->buttonWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->buttonWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pD2D1DCRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->graphics()->d2d1Engine().createDCRenderTarget(*this->pD2D1DCRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        hr = (*this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        if (recreateSharedResources)
            MyStandardButtonSubclass::releaseSharedDeviceResources();

        if (!this->createSharedDeviceResources())
        {
            error_message = "Failed to create the shared device resources.";
            break;
        }

        error_message = "Failed to create the device resources.";
        this->pTextFormat.reset(new IDWriteTextFormat *(nullptr));
        FLOAT font_size = static_cast<FLOAT>((rect_window.bottom - rect_window.top)) * 0.5f;
        if (!this->config.ignoreTextScalingLimits)
        {
            if (font_size < this->config.textLowerBoundSizeLimit)
                font_size = this->config.textLowerBoundSizeLimit;
            else if (font_size > this->config.textUpperBoundSizeLimit)
                font_size = this->config.textUpperBoundSizeLimit;
        }
        if (!this->graphics()->d2d1Engine().createTextFormat(*this->pTextFormat, this->objects()->fonts.defaultFamily, font_size))
            break;
        error_message = "";

        if (!ReleaseDC(this->buttonWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyStandardButtonSubclass::startAnimation(ButtonAnimationState animationState)
{
    HRESULT hr;
    std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter> p_storyboard(new IUIAnimationStoryboard *(nullptr));
    std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_focus_border_opacity(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_button_rgb;
    p_transition_button_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_button_border_rgb;
    p_transition_button_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_button_text_rgb;
    p_transition_button_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));

    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Create the storyboard.
        hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        // Create and add the animation transitions to the storyboard.
        bool is_switch_success = false;
        switch (animationState)
        {
        case ButtonAnimationState::Default:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.standardButtonDefault.getRed(), 0.5, 0.5, &*p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.standardButtonDefault.getGreen(), 0.5, 0.5, &*p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.standardButtonDefault.getBlue(), 0.5, 0.5, &*p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.standardButtonBorderDefault.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.standardButtonBorderDefault.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.standardButtonBorderDefault.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[0], *p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[1], *p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[2], *p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ButtonAnimationState::Default;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::Hover:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.standardButtonHover.getRed(), 0.5, 0.5, &*p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.standardButtonHover.getGreen(), 0.5, 0.5, &*p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.standardButtonHover.getBlue(), 0.5, 0.5, &*p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.standardButtonBorderHover.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.standardButtonBorderHover.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.standardButtonBorderHover.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[0], *p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[1], *p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[2], *p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ButtonAnimationState::Hover;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::Active:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.standardButtonActive.getRed(), 0.5, 0.5, &*p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.standardButtonActive.getGreen(), 0.5, 0.5, &*p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.standardButtonActive.getBlue(), 0.5, 0.5, &*p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.standardButtonBorderActive.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.standardButtonBorderActive.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.standardButtonBorderActive.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getBlue(), 0.7, 0, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[0], *p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[1], *p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonRGB[2], *p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ButtonAnimationState::Active;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::Focus:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 1.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::FocusLost:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        }
        if (!is_switch_success)
        {
            error_message = "Failed to create and add the animation transitions to the storyboard.";
            break;
        }

        // Get the current time.
        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        // Set the storyboard tag.
        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->buttonWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        // Schedule the storyboard.
        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        // Set the animation invalidate timer.
        SetTimer(this->buttonWindow, MyStandardButtonSubclass::IDT_ANIMATION_INVALIDATE, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'startAnimation()']", MyLogType::Error);

    return are_all_operation_success;
}
LRESULT CALLBACK MyStandardButtonSubclass::subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyStandardButtonSubclass *p_this = reinterpret_cast<MyStandardButtonSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override paint messages.
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;

            // Get the latest animation values.
            DOUBLE focus_border_opacity = 0.0;
            INT32 button_rgb[3] = {0, 0, 0};
            INT32 button_border_rgb[3] = {0, 0, 0};
            INT32 button_text_rgb[3] = {0, 0, 0};
            hr = (*p_this->pAnimationVariableFocusBorderOpacity)->GetValue(&focus_border_opacity);
            if (FAILED(hr) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableButtonRGB, button_rgb) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableButtonBorderRGB, button_border_rgb) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableButtonTextRGB, button_text_rgb))
            {
                error_message = "[WM_PAINT] Failed to retrieve the animation values.";
                break;
            }

            // Get the window client rect.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                break;
            }

            // Get the window text length.
            SetLastError(ERROR_SUCCESS);
            size_t window_text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
            if (!window_text_length && GetLastError())
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text length.";
                break;
            }

            // Get the window text.
            std::unique_ptr<WCHAR[]> window_text(new WCHAR[window_text_length + 1]);
            if (!GetWindowTextW(hWnd, window_text.get(), static_cast<INT>(window_text_length) + 1) && window_text_length)
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text.";
                break;
            }

            // Bind the render target to the window device context.
            hr = (*p_this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
            if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                break;
            }

            // Prepare drawing resources.
            D2D1_RECT_F d2d1_rect_window = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window.right), static_cast<FLOAT>(rect_window.bottom));
            D2D1::ColorF d2d1_color_focus_border = p_this->objects()->colors.focus.getD2D1Color();
            d2d1_color_focus_border.a = static_cast<FLOAT>(focus_border_opacity);
            D2D1::ColorF d2d1_color_button(button_rgb[0] / 255.0f, button_rgb[1] / 255.0f, button_rgb[2] / 255.0f, 1.0);
            D2D1::ColorF d2d1_color_button_border(button_border_rgb[0] / 255.0f, button_border_rgb[1] / 255.0f, button_border_rgb[2] / 255.0f, 1.0);
            D2D1::ColorF d2d1_color_button_text(button_text_rgb[0] / 255.0f, button_text_rgb[1] / 255.0f, button_text_rgb[2] / 255.0f, 1.0);
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_focus_border(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_button(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_button_border(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_button_text(new ID2D1SolidColorBrush *(nullptr));
            {
                error_message = "[WM_PAINT] Failed to create the resources.";
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_focus_border, &*p_d2d1_solidcolorbrush_focus_border);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_button, &*p_d2d1_solidcolorbrush_button);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_button_border, &*p_d2d1_solidcolorbrush_button_border);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_button_text, &*p_d2d1_solidcolorbrush_button_text);
                if (FAILED(hr))
                    break;
                error_message = "";
            }

            // Begin drawing.
            (*p_this->pD2D1DCRenderTarget)->BeginDraw();
            (*p_this->pD2D1DCRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            (*p_this->pD2D1DCRenderTarget)->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            {
                // Draw the background.
                (*p_this->pD2D1DCRenderTarget)->Clear((p_this->config.pBackground ? p_this->config.pBackground->getD2D1Color() : p_this->objects()->colors.background.getD2D1Color()));

                // Draw the button.
                p_this->graphics()->d2d1Engine().drawFillRoundRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window, 5, 5, *p_d2d1_solidcolorbrush_button, *p_d2d1_solidcolorbrush_button_border, 1.0);

                // Draw the button text.
                p_this->graphics()->d2d1Engine().drawText((*p_this->pD2D1DCRenderTarget), *p_this->pTextFormat, d2d1_rect_window, window_text.get(), *p_d2d1_solidcolorbrush_button_text, 0, 0, 3);

                // Draw the focus border.
                if (focus_border_opacity)
                    p_this->graphics()->d2d1Engine().drawRoundRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window, 5, 5, *p_d2d1_solidcolorbrush_focus_border);
            }

            // End drawing.
            hr = (*p_this->pD2D1DCRenderTarget)->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!p_this->createDeviceResources(true))
                {
                    error_message = "[WM_PAINT] Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "[WM_PAINT] Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue; // Repeat the paint operation.
            }
            else if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        return 0;
    }

    // Process the animation invalidate timer.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyStandardButtonSubclass::IDT_ANIMATION_INVALIDATE:
        {
            HRESULT hr;
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

                IUIAnimationStoryboard *p_storyboard;
                hr = p_this->graphics()->wamEngine().manager()->GetStoryboardFromTag(NULL, GetDlgCtrlID(hWnd), &p_storyboard);
                if (SUCCEEDED(hr))
                {
                    if (!p_storyboard)
                    {
                        if (!KillTimer(hWnd, MyStandardButtonSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }

                        are_all_operation_success = true;
                        break;
                    }

                    UI_ANIMATION_STORYBOARD_STATUS storyboard_status;
                    hr = p_storyboard->GetStatus(&storyboard_status);
                    if (FAILED(hr))
                    {
                        error_message = "[WM_TIMER] Failed to get the storyboard status.";
                        break;
                    }

                    if (storyboard_status == UI_ANIMATION_STORYBOARD_READY)
                    {
                        if (!KillTimer(hWnd, MyStandardButtonSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer if an error occurs.
                if (!KillTimer(hWnd, MyStandardButtonSubclass::IDT_ANIMATION_INVALIDATE))
                    g_pApp->logger.writeLog("[WM_TIMER] Failed to kill the timer.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            g_pApp->logger.writeLog("[WM_TIMER] Unprocessed timer message.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process WM_GETDLGCODE messages to handle nagivation key input manually.
    case WM_GETDLGCODE:
    {
        bool is_focused = (GetFocus() == hWnd ? true : false);
        if (!is_focused)
            break;

        switch (wParam)
        {
        case VK_TAB:
        case VK_SPACE:
        case VK_RETURN:
            return DLGC_WANTALLKEYS; // Allow the window to capture the key message via WM_KEYDOWN/WM_KEYUP.
        }

        return 0; // Ignore nagivation key by default.
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_TAB:
        {
            HWND next_focus_window = GetNextDlgTabItem(*MyStandardButtonSubclass::pAppWindow, hWnd, static_cast<INT>(GetKeyState(VK_SHIFT) & 0x8000));
            SetFocus(next_focus_window);

            if (p_this->currentAnimationState != ButtonAnimationState::Default)
                if (!p_this->startAnimation(ButtonAnimationState::Default))
                    g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }
        case VK_SPACE:
        case VK_RETURN:
        {
            if (!p_this->isHoverState && p_this->currentAnimationState != ButtonAnimationState::Active)
                if (!p_this->config.skipActiveAnimationState)
                    if (!p_this->startAnimation(ButtonAnimationState::Active))
                        g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }
        }

        break;
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_TAB:
            return 0;
        case VK_SPACE:
        case VK_RETURN:
        {
            if (!p_this->isHoverState && p_this->currentAnimationState == ButtonAnimationState::Active)
            {
                if (!p_this->startAnimation(ButtonAnimationState::Default))
                    g_pApp->logger.writeLog("[WM_KEYUP] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), 0);
            }

            return 0;
        }
        }

        break;
    }

    // Process the left button click-down messages to trigger appropriate actions.
    case WM_LBUTTONDOWN:
    {
        if (!p_this->isActiveState && p_this->isHoverState)
        {
            p_this->isActiveState = true;

            if (!p_this->config.skipActiveAnimationState)
                if (!p_this->startAnimation(ButtonAnimationState::Active))
                    g_pApp->logger.writeLog("[WM_LBUTTONDOWN] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }

        break;
    }

    // Process the left button click-up messages to trigger appropriate actions.
    case WM_LBUTTONUP:
    {
        if (p_this->isActiveState)
        {
            p_this->isActiveState = false;

            if (p_this->isHoverState)
            {
                if (!p_this->config.skipHoverAnimationState)
                    if (!p_this->startAnimation(ButtonAnimationState::Hover))
                        g_pApp->logger.writeLog("[WM_LBUTTONUP] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), 0);
            }
            else if (!p_this->startAnimation(ButtonAnimationState::Default))
                g_pApp->logger.writeLog("[WM_LBUTTONUP] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
        }

        break;
    }

    // Process the mouse leave messages to trigger appropriate actions.
    case WM_MOUSELEAVE:
    {
        if (p_this->isHoverState)
        {
            p_this->isHoverState = false;
            p_this->isActiveState = false;

            if (!p_this->startAnimation(ButtonAnimationState::Default))
                g_pApp->logger.writeLog("[WM_MOUSELEAVE] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }

        break;
    }

    // Process the mouse move messages to trigger appropriate actions.
    case WM_MOUSEMOVE:
    {
        if (!p_this->isHoverState)
        {
            TRACKMOUSEEVENT track_mouse_event;
            track_mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
            track_mouse_event.dwFlags = TME_LEAVE;
            track_mouse_event.hwndTrack = hWnd;
            TrackMouseEvent(&track_mouse_event);
            p_this->isHoverState = true;

            if (!p_this->config.skipHoverAnimationState)
                if (!p_this->startAnimation(ButtonAnimationState::Hover))
                    g_pApp->logger.writeLog("[WM_MOUSEMOVE] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }

        break;
    }

    // Update the appearance of the edit window when it gains focus.
    case WM_SETFOCUS:
    {
        if (!p_this->startAnimation(ButtonAnimationState::Focus))
            g_pApp->logger.writeLog("[WM_SETFOCUS] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
        break;
    }

    // Update the appearance of the edit window when it loses focus.
    case WM_KILLFOCUS:
    {
        if (!p_this->startAnimation(ButtonAnimationState::FocusLost))
            g_pApp->logger.writeLog("[WM_KILLFOCUS] Failed to start the animation.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyStandardButtonSubclass::subclassProcedure, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyStandardButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// [MyImageButtonSubclass] class implementations:

bool MyImageButtonSubclass::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass object is not associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is not associated with a window.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        // Start animation to the current animation state to update animation variables values.
        if (!this->startAnimation(this->currentAnimationState))
        {
            error_message = "Failed to start the animation.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
void MyImageButtonSubclass::releaseSharedDeviceResources()
{
    // <This subclass class doesn't have any shared device resources>
}
void MyImageButtonSubclass::setAnimationDuration(ButtonAnimationState animateState, FLOAT milliseconds)
{
    switch (animateState)
    {
    case ButtonAnimationState::Default:
        this->defaultAnimationDuration = milliseconds;
        break;
    case ButtonAnimationState::Hover:
        this->hoverAnimationDuration = milliseconds;
        break;
    case ButtonAnimationState::Active:
        this->activeAnimationDuration = milliseconds;
        break;
    case ButtonAnimationState::Focus:
    case ButtonAnimationState::FocusLost:
        this->focusAnimationDuration = milliseconds;
        break;
    }
}
bool MyImageButtonSubclass::setWindow(HWND hWnd, MyImageButtonSubclassConfig *pConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass base class is initialized.
        if (!this->isInitialized)
        {
            error_message = "The subclass base class is not initialized.";
            break;
        }

        // Check if the subclass object is already associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is already associated with a window.";
            break;
        }

        // Check if the configuration structure is valid.
        if (!pConfig->isValid())
        {
            error_message = "The configuration structure contains invalid parameters.";
            break;
        }
        this->imageConfig = *pConfig;

        // Store the window handle(s).
        this->buttonWindow = hWnd;

        // Subclass the window.
        // Store this subclass object pointer as reference data, enabling the subclass procedure to access the non-static object members.
        if (!SetWindowSubclass(this->buttonWindow, &MyImageButtonSubclass::subclassProcedure, 0, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = "Failed to install the window subclass callback.";
            break;
        }

        // Create the animation variables.
        {
            error_message = "Failed to create the animation variables.";
            this->pAnimationVariableFocusBorderOpacity.reset(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableDefaultImageOpacity.reset(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableHoverImageOpacity.reset(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableActiveImageOpacity.reset(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableBackgroundRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableBackgroundRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableBackgroundRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableFocusBorderOpacity, 0.0, 0.0, 1.0))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableDefaultImageOpacity, 1.0, 0.0, 1.0))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableHoverImageOpacity, 0.0, 0.0, 1.0))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableActiveImageOpacity, 0.0, 0.0, 1.0))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableBackgroundRGB[0],
                                                                       this->imageConfig.pColorBackgroundDefault->getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableBackgroundRGB[1],
                                                                       this->imageConfig.pColorBackgroundDefault->getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableBackgroundRGB[2],
                                                                       this->imageConfig.pColorBackgroundDefault->getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            error_message = "";
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'setWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
MyImageButtonSubclass *MyImageButtonSubclass::getSubclassPointer(HWND hWnd)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyImageButtonSubclass::subclassProcedure, 0, &reference_data);
    return (is_subclassed ? reinterpret_cast<MyImageButtonSubclass *>(reference_data) : nullptr);
}
bool MyImageButtonSubclass::createSharedDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // HRESULT hr;

        error_message = "Failed to create the shared device resources.";
        // <This subclass class doesn't have any shared device resources>
        error_message = "";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'createSharedDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyImageButtonSubclass::createDeviceResources(bool recreateSharedResources)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        HDC hdc = GetDC(this->buttonWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->buttonWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pD2D1DCRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->graphics()->d2d1Engine().createDCRenderTarget(*this->pD2D1DCRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        hr = (*this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        if (recreateSharedResources)
            MyImageButtonSubclass::releaseSharedDeviceResources();

        if (!this->createSharedDeviceResources())
        {
            error_message = "Failed to create the shared device resources.";
            break;
        }

        error_message = "Failed to create the device resources.";
        this->pD2D1BitmapDefaultImage.reset(new ID2D1Bitmap *(nullptr));
        this->pD2D1BitmapHoverImage.reset(new ID2D1Bitmap *(nullptr));
        this->pD2D1BitmapActiveImage.reset(new ID2D1Bitmap *(nullptr));
        hr = (*this->pD2D1DCRenderTarget)->CreateBitmapFromWicBitmap(this->imageConfig.pImageDefault->getBitmapSource(), &*this->pD2D1BitmapDefaultImage);
        if (FAILED(hr))
            break;
        hr = (*this->pD2D1DCRenderTarget)->CreateBitmapFromWicBitmap(this->imageConfig.pImageHover->getBitmapSource(), &*this->pD2D1BitmapHoverImage);
        if (FAILED(hr))
            break;
        hr = (*this->pD2D1DCRenderTarget)->CreateBitmapFromWicBitmap(this->imageConfig.pImageActive->getBitmapSource(), &*this->pD2D1BitmapActiveImage);
        if (FAILED(hr))
            break;
        error_message = "";

        if (!ReleaseDC(this->buttonWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyImageButtonSubclass::startAnimation(ButtonAnimationState animationState)
{
    HRESULT hr;
    std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter> p_storyboard(new IUIAnimationStoryboard *(nullptr));
    std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_focus_border_opacity(new IUIAnimationTransition *(nullptr));
    std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_default_icon_opacity(new IUIAnimationTransition *(nullptr));
    std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_hover_icon_opacity(new IUIAnimationTransition *(nullptr));
    std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_down_icon_opacity(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_background_rgb;
    p_transition_background_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_background_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_background_rgb.emplace_back(new IUIAnimationTransition *(nullptr));

    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Create the storyboard.
        hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        // Create and add the animation transitions to the storyboard.
        bool is_switch_success = false;
        switch (animationState)
        {
        case ButtonAnimationState::Default:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->imageConfig.pColorBackgroundDefault->getRed(), 0.5, 0.5, &*p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->imageConfig.pColorBackgroundDefault->getGreen(), 0.5, 0.5, &*p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->imageConfig.pColorBackgroundDefault->getBlue(), 0.5, 0.5, &*p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, 1.0, 0.5, 0.5, &*p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[0], *p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[1], *p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[2], *p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableDefaultImageOpacity, *p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableHoverImageOpacity, *p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableActiveImageOpacity, *p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ButtonAnimationState::Default;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::Hover:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->imageConfig.pColorBackgroundHover->getRed(), 0.5, 0.5, &*p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->imageConfig.pColorBackgroundHover->getGreen(), 0.5, 0.5, &*p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->imageConfig.pColorBackgroundHover->getBlue(), 0.5, 0.5, &*p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, 1.0, 0.5, 0.5, &*p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[0], *p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[1], *p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[2], *p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableDefaultImageOpacity, *p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableHoverImageOpacity, *p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableActiveImageOpacity, *p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ButtonAnimationState::Hover;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::Active:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->imageConfig.pColorBackgroundActive->getRed(), 0.5, 0.5, &*p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->imageConfig.pColorBackgroundActive->getGreen(), 0.5, 0.5, &*p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->imageConfig.pColorBackgroundActive->getBlue(), 0.5, 0.5, &*p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, 1.0, 0.5, 0.5, &*p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[0], *p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[1], *p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableBackgroundRGB[2], *p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableDefaultImageOpacity, *p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableHoverImageOpacity, *p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableActiveImageOpacity, *p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ButtonAnimationState::Active;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::Focus:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 1.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        case ButtonAnimationState::FocusLost:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        }
        if (!is_switch_success)
        {
            error_message = "Failed to create and add the animation transitions to the storyboard.";
            break;
        }

        // Get the current time.
        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        // Set the storyboard tag.
        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->buttonWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        // Schedule the storyboard.
        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        // Set the animation invalidate timer.
        SetTimer(this->buttonWindow, MyImageButtonSubclass::IDT_ANIMATION_INVALIDATE, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'startAnimation()']", MyLogType::Error);

    return are_all_operation_success;
}
LRESULT CALLBACK MyImageButtonSubclass::subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyImageButtonSubclass *p_this = reinterpret_cast<MyImageButtonSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override paint messages.
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;

            // Get the latest animation values.
            DOUBLE focus_border_opacity = 0.0;
            DOUBLE default_image_opacity = 0.0;
            DOUBLE hover_image_opacity = 0.0;
            DOUBLE active_image_opacity = 0.0;
            INT32 background_rgb[3] = {0, 0, 0};
            error_message = "[WM_PAINT] Failed to retrieve the animation values.";
            hr = (*p_this->pAnimationVariableFocusBorderOpacity)->GetValue(&focus_border_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_this->pAnimationVariableDefaultImageOpacity)->GetValue(&default_image_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_this->pAnimationVariableHoverImageOpacity)->GetValue(&hover_image_opacity);
            if (FAILED(hr))
                break;
            hr = (*p_this->pAnimationVariableActiveImageOpacity)->GetValue(&active_image_opacity);
            if (FAILED(hr))
                break;
            if (!p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableBackgroundRGB, background_rgb))
                break;
            error_message = "";

            // Get the window client rect.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                break;
            }

            // Bind the render target to the window device context.
            hr = (*p_this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
            if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                break;
            }

            // Prepare drawing resources.
            D2D1_RECT_F d2d1_rect_window = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window.right), static_cast<FLOAT>(rect_window.bottom));
            D2D1::ColorF d2d1_color_focus_border = p_this->objects()->colors.focus.getD2D1Color();
            d2d1_color_focus_border.a = static_cast<FLOAT>(focus_border_opacity);
            D2D1::ColorF d2d1_color_background(background_rgb[0] / 255.0f, background_rgb[1] / 255.0f, background_rgb[2] / 255.0f, 1.0);
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_focus_border(new ID2D1SolidColorBrush *(nullptr));
            ID2D1Bitmap *&p_d2d1_bitmap_default_image = *p_this->pD2D1BitmapDefaultImage;
            ID2D1Bitmap *&p_d2d1_bitmap_hover_image = *p_this->pD2D1BitmapHoverImage;
            ID2D1Bitmap *&p_d2d1_bitmap_down_image = *p_this->pD2D1BitmapActiveImage;
            {
                error_message = "[WM_PAINT] Failed to create the resources.";
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_focus_border, &*p_d2d1_solidcolorbrush_focus_border);
                if (FAILED(hr))
                    break;
                error_message = "";
            }

            // Begin drawing.
            (*p_this->pD2D1DCRenderTarget)->BeginDraw();
            (*p_this->pD2D1DCRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

            {
                // Draw the background.
                (*p_this->pD2D1DCRenderTarget)->Clear(d2d1_color_background);

                // Draw the default image.
                if (p_this->imageConfig.alwaysDrawDefaultImage)
                    p_this->graphics()->d2d1Engine().drawImage((*p_this->pD2D1DCRenderTarget), p_d2d1_bitmap_default_image, d2d1_rect_window, 1.0f, p_this->imageConfig.imagePosX, p_this->imageConfig.imagePosY, p_this->imageConfig.imageWidth, p_this->imageConfig.imageHeight, p_this->imageConfig.centering);
                else
                {
                    if (default_image_opacity)
                        p_this->graphics()->d2d1Engine().drawImage((*p_this->pD2D1DCRenderTarget), p_d2d1_bitmap_default_image, d2d1_rect_window, static_cast<FLOAT>(default_image_opacity), p_this->imageConfig.imagePosX, p_this->imageConfig.imagePosY, p_this->imageConfig.imageWidth, p_this->imageConfig.imageHeight, p_this->imageConfig.centering);
                }

                // Draw the hover image.
                if (!p_this->config.skipHoverAnimationState && hover_image_opacity)
                    p_this->graphics()->d2d1Engine().drawImage((*p_this->pD2D1DCRenderTarget), p_d2d1_bitmap_hover_image, d2d1_rect_window, static_cast<FLOAT>(hover_image_opacity), p_this->imageConfig.imagePosX, p_this->imageConfig.imagePosY, p_this->imageConfig.imageWidth, p_this->imageConfig.imageHeight, p_this->imageConfig.centering);

                // Draw the active image.
                if (!p_this->config.skipActiveAnimationState && active_image_opacity)
                    p_this->graphics()->d2d1Engine().drawImage((*p_this->pD2D1DCRenderTarget), p_d2d1_bitmap_down_image, d2d1_rect_window, static_cast<FLOAT>(active_image_opacity), p_this->imageConfig.imagePosX, p_this->imageConfig.imagePosY, p_this->imageConfig.imageWidth, p_this->imageConfig.imageHeight, p_this->imageConfig.centering);

                // Draw the focus border.
                if (focus_border_opacity)
                    p_this->graphics()->d2d1Engine().drawRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window, *p_d2d1_solidcolorbrush_focus_border);
            }

            // End drawing.
            hr = (*p_this->pD2D1DCRenderTarget)->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!p_this->createDeviceResources(true))
                {
                    error_message = "[WM_PAINT] Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "[WM_PAINT] Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue; // Repeat the paint operation.
            }
            else if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        return 0;
    }

    // Process the animation invalidate timer.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyImageButtonSubclass::IDT_ANIMATION_INVALIDATE:
        {
            HRESULT hr;
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

                IUIAnimationStoryboard *p_storyboard;
                hr = p_this->graphics()->wamEngine().manager()->GetStoryboardFromTag(NULL, GetDlgCtrlID(hWnd), &p_storyboard);
                if (SUCCEEDED(hr))
                {
                    if (!p_storyboard)
                    {
                        if (!KillTimer(hWnd, MyImageButtonSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }

                        are_all_operation_success = true;
                        break;
                    }

                    UI_ANIMATION_STORYBOARD_STATUS storyboard_status;
                    hr = p_storyboard->GetStatus(&storyboard_status);
                    if (FAILED(hr))
                    {
                        error_message = "[WM_TIMER] Failed to get the storyboard status.";
                        break;
                    }

                    if (storyboard_status == UI_ANIMATION_STORYBOARD_READY)
                    {
                        if (!KillTimer(hWnd, MyImageButtonSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer if an error occurs.
                if (!KillTimer(hWnd, MyImageButtonSubclass::IDT_ANIMATION_INVALIDATE))
                    g_pApp->logger.writeLog("[WM_TIMER] Failed to kill the timer.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            g_pApp->logger.writeLog("[WM_TIMER] Unprocessed timer message.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process WM_GETDLGCODE messages to handle nagivation key input manually.
    case WM_GETDLGCODE:
    {
        bool is_focused = (GetFocus() == hWnd ? true : false);
        if (!is_focused)
            break;

        switch (wParam)
        {
        case VK_TAB:
        case VK_SPACE:
        case VK_RETURN:
            return DLGC_WANTALLKEYS; // Allow the window to capture the key message via WM_KEYDOWN/WM_KEYUP.
        }

        return 0; // Ignore nagivation key by default.
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_TAB:
        {
            HWND next_focus_window = GetNextDlgTabItem(*MyImageButtonSubclass::pAppWindow, hWnd, static_cast<INT>(GetKeyState(VK_SHIFT) & 0x8000));
            SetFocus(next_focus_window);

            if (p_this->currentAnimationState != ButtonAnimationState::Default)
                if (!p_this->startAnimation(ButtonAnimationState::Default))
                    g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }
        case VK_SPACE:
        case VK_RETURN:
        {
            if (!p_this->isHoverState && p_this->currentAnimationState != ButtonAnimationState::Active)
                if (!p_this->startAnimation(ButtonAnimationState::Active))
                    g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }
        }

        break;
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_TAB:
            return 0;
        case VK_SPACE:
        case VK_RETURN:
        {
            if (!p_this->isHoverState && p_this->currentAnimationState == ButtonAnimationState::Active)
            {
                if (!p_this->startAnimation(ButtonAnimationState::Default))
                    g_pApp->logger.writeLog("[WM_KEYUP] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), 0);
            }

            return 0;
        }
        }

        break;
    }

    // Process the left button click-down messages to trigger appropriate actions.
    case WM_LBUTTONDOWN:
    {
        if (!p_this->isActiveState && p_this->isHoverState)
        {
            p_this->isActiveState = true;

            if (!p_this->config.skipActiveAnimationState)
                if (!p_this->startAnimation(ButtonAnimationState::Active))
                    g_pApp->logger.writeLog("[WM_LBUTTONDOWN] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }

        break;
    }

    // Process the left button click-up messages to trigger appropriate actions.
    case WM_LBUTTONUP:
    {
        if (p_this->isActiveState)
        {
            p_this->isActiveState = false;

            if (p_this->isHoverState)
            {
                if (!p_this->config.skipHoverAnimationState)
                    if (!p_this->startAnimation(ButtonAnimationState::Hover))
                        g_pApp->logger.writeLog("[WM_LBUTTONUP] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), 0);
            }
            else if (!p_this->startAnimation(ButtonAnimationState::Default))
                g_pApp->logger.writeLog("[WM_LBUTTONUP] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
        }

        break;
    }

    // Process the mouse leave messages to trigger appropriate actions.
    case WM_MOUSELEAVE:
    {
        if (p_this->isHoverState)
        {
            p_this->isHoverState = false;
            p_this->isActiveState = false;

            if (!p_this->startAnimation(ButtonAnimationState::Default))
                g_pApp->logger.writeLog("[WM_MOUSELEAVE] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }

        break;
    }

    // Process the mouse move messages to trigger appropriate actions.
    case WM_MOUSEMOVE:
    {
        if (!p_this->isHoverState)
        {
            TRACKMOUSEEVENT track_mouse_event;
            track_mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
            track_mouse_event.dwFlags = TME_LEAVE;
            track_mouse_event.hwndTrack = hWnd;
            TrackMouseEvent(&track_mouse_event);
            p_this->isHoverState = true;

            if (!p_this->config.skipHoverAnimationState)
                if (!p_this->startAnimation(ButtonAnimationState::Hover))
                    g_pApp->logger.writeLog("[WM_MOUSEMOVE] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

            return 0;
        }

        break;
    }

    // Update the appearance of the edit window when it gains focus.
    case WM_SETFOCUS:
    {
        if (!p_this->startAnimation(ButtonAnimationState::Focus))
            g_pApp->logger.writeLog("[WM_SETFOCUS] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        break;
    }

    // Update the appearance of the edit window when it loses focus.
    case WM_KILLFOCUS:
    {
        if (!p_this->startAnimation(ButtonAnimationState::FocusLost))
            g_pApp->logger.writeLog("[WM_KILLFOCUS] Failed to start the animation.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyImageButtonSubclass::subclassProcedure, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        return 0;
    }
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// [MyRadioButtonSubclass] class implementations:

void MyRadioButtonSubclass::updateSelectionState(bool state)
{
    // Ignore if the requested state is the same as the current state.
    if (this->isSelected == state)
        return;

    // Update selection state and trigger the appropriate animation.
    if (!state)
    {
        this->isSelected = false;

        if (!this->startAnimation(RadioButtonAnimationState::Default))
        {
            g_pApp->logger.writeLog("Failed to start the button animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'updateSelectionState()']", MyLogType::Error);
        }
    }
    else
    {
        this->isSelected = true;
    }
}
bool MyRadioButtonSubclass::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass object is not associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is not associated with a window.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        // Start animation to the current animation state to update animation variables values.
        if (!this->startAnimation(this->currentAnimationState))
        {
            error_message = "Failed to start the animation.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
void MyRadioButtonSubclass::releaseSharedDeviceResources()
{
    // <This subclass class doesn't have any shared device resources>
}
void MyRadioButtonSubclass::setAnimationDuration(RadioButtonAnimationState animateState, FLOAT milliseconds)
{
    switch (animateState)
    {
    case RadioButtonAnimationState::Selected:
    case RadioButtonAnimationState::Default:
        this->defaultAnimationDuration = milliseconds;
        break;
    case RadioButtonAnimationState::SelectedHover:
    case RadioButtonAnimationState::Hover:
        this->hoverAnimationDuration = milliseconds;
        break;
    case RadioButtonAnimationState::SelectedDown:
    case RadioButtonAnimationState::Down:
        this->activeAnimationDuration = milliseconds;
        break;
    case RadioButtonAnimationState::Focus:
    case RadioButtonAnimationState::FocusLost:
        this->focusAnimationDuration = milliseconds;
        break;
    }
}
bool MyRadioButtonSubclass::setWindow(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass base class is initialized.
        if (!this->isInitialized)
        {
            error_message = "The subclass base class is not initialized.";
            break;
        }

        // Check if the subclass object is already associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is already associated with a window.";
            break;
        }

        // Store the window handle(s).
        this->buttonWindow = hWnd;

        // Subclass the window.
        // Store this subclass object pointer as reference data, enabling the subclass procedure to access the non-static object members.
        if (!SetWindowSubclass(this->buttonWindow, &MyRadioButtonSubclass::subclassProcedure, 0, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = "Failed to install the window subclass callback.";
            break;
        }

        // Create the animation variables.
        {
            error_message = "Failed to create the animation variables.";
            this->pAnimationVariableFocusBorderOpacity.reset(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonPrimaryRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonPrimaryRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonPrimaryRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonSecondaryRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonSecondaryRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonSecondaryRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableButtonTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableFocusBorderOpacity, 0.0, 0.0, 1.0))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonPrimaryRGB[0],
                                                                       this->objects()->colors.radioButtonPrimaryDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonPrimaryRGB[1],
                                                                       this->objects()->colors.radioButtonPrimaryDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonPrimaryRGB[2],
                                                                       this->objects()->colors.radioButtonPrimaryDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonSecondaryRGB[0],
                                                                       this->objects()->colors.radioButtonSecondaryDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonSecondaryRGB[1],
                                                                       this->objects()->colors.radioButtonSecondaryDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonSecondaryRGB[2],
                                                                       this->objects()->colors.radioButtonSecondaryDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonBorderRGB[0],
                                                                       this->objects()->colors.radioButtonBorderDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonBorderRGB[1],
                                                                       this->objects()->colors.radioButtonBorderDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonBorderRGB[2],
                                                                       this->objects()->colors.radioButtonBorderDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonTextRGB[0],
                                                                       this->objects()->colors.textActive.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonTextRGB[1],
                                                                       this->objects()->colors.textActive.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableButtonTextRGB[2],
                                                                       this->objects()->colors.textActive.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            error_message = "";
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'setWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
MyRadioButtonSubclass *MyRadioButtonSubclass::getSubclassPointer(HWND hWnd)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyRadioButtonSubclass::subclassProcedure, 0, &reference_data);
    return (is_subclassed ? reinterpret_cast<MyRadioButtonSubclass *>(reference_data) : nullptr);
}
bool MyRadioButtonSubclass::createSharedDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // HRESULT hr;

        error_message = "Failed to create the shared device resources.";
        // <This subclass class doesn't have any shared device resources>
        error_message = "";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'createSharedDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyRadioButtonSubclass::createDeviceResources(bool recreateSharedResources)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        HDC hdc = GetDC(this->buttonWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->buttonWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pD2D1DCRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->graphics()->d2d1Engine().createDCRenderTarget(*this->pD2D1DCRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        hr = (*this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        if (recreateSharedResources)
            MyRadioButtonSubclass::releaseSharedDeviceResources();

        if (!this->createSharedDeviceResources())
        {
            error_message = "Failed to create the shared device resources.";
            break;
        }

        error_message = "Failed to create the device resources.";
        this->pTextFormat.reset(new IDWriteTextFormat *(nullptr));
        FLOAT font_size = static_cast<FLOAT>((rect_window.bottom - rect_window.top)) * 0.5f;
        if (!this->config.ignoreTextScalingLimits)
        {
            if (font_size < this->config.textLowerBoundSizeLimit)
                font_size = this->config.textLowerBoundSizeLimit;
            else if (font_size > this->config.textUpperBoundSizeLimit)
                font_size = this->config.textUpperBoundSizeLimit;
        }
        if (!this->graphics()->d2d1Engine().createTextFormat(*this->pTextFormat, this->objects()->fonts.defaultFamily, font_size))
            break;
        error_message = "";

        if (!ReleaseDC(this->buttonWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyRadioButtonSubclass::startAnimation(RadioButtonAnimationState animationState)
{
    HRESULT hr;
    std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter> p_storyboard(new IUIAnimationStoryboard *(nullptr));
    std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_focus_border_opacity(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_button_primary_rgb;
    p_transition_button_primary_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_primary_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_primary_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_button_secondary_rgb;
    p_transition_button_secondary_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_secondary_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_secondary_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_button_border_rgb;
    p_transition_button_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_button_text_rgb;
    p_transition_button_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_button_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));

    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Create the storyboard.
        hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        // Create and add the animation transitions to the storyboard.
        bool is_switch_success = false;
        switch (animationState)
        {
        case RadioButtonAnimationState::Default:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonPrimaryDefault.getRed(), 0.5, 0.5, &*p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonPrimaryDefault.getGreen(), 0.5, 0.5, &*p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonPrimaryDefault.getBlue(), 0.5, 0.5, &*p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonSecondaryDefault.getRed(), 0.5, 0.5, &*p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonSecondaryDefault.getGreen(), 0.5, 0.5, &*p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonSecondaryDefault.getBlue(), 0.5, 0.5, &*p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonBorderDefault.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonBorderDefault.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.radioButtonBorderDefault.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[0], *p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[1], *p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[2], *p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[0], *p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[1], *p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[2], *p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = RadioButtonAnimationState::Default;

            is_switch_success = true;
            break;
        }
        case RadioButtonAnimationState::Hover:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonPrimaryHover.getRed(), 0.5, 0.5, &*p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonPrimaryHover.getGreen(), 0.5, 0.5, &*p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonPrimaryHover.getBlue(), 0.5, 0.5, &*p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonSecondaryHover.getRed(), 0.5, 0.5, &*p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonSecondaryHover.getGreen(), 0.5, 0.5, &*p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonSecondaryHover.getBlue(), 0.5, 0.5, &*p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonBorderHover.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonBorderHover.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.radioButtonBorderHover.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[0], *p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[1], *p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[2], *p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[0], *p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[1], *p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[2], *p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = RadioButtonAnimationState::Hover;

            is_switch_success = true;
            break;
        }
        case RadioButtonAnimationState::Down:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonPrimaryActive.getRed(), 0.5, 0.5, &*p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonPrimaryActive.getGreen(), 0.5, 0.5, &*p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonPrimaryActive.getBlue(), 0.5, 0.5, &*p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonSecondaryActive.getRed(), 0.5, 0.5, &*p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonSecondaryActive.getGreen(), 0.5, 0.5, &*p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonSecondaryActive.getBlue(), 0.5, 0.5, &*p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonBorderActive.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonBorderActive.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.radioButtonBorderActive.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[0], *p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[1], *p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[2], *p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[0], *p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[1], *p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[2], *p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = RadioButtonAnimationState::Down;

            is_switch_success = true;
            break;
        }
        case RadioButtonAnimationState::Selected:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryDefault.getRed(), 0.5, 0.5, &*p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryDefault.getGreen(), 0.5, 0.5, &*p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryDefault.getBlue(), 0.5, 0.5, &*p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryDefault.getRed(), 0.5, 0.5, &*p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryDefault.getGreen(), 0.5, 0.5, &*p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryDefault.getBlue(), 0.5, 0.5, &*p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonBorderDefault.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonBorderDefault.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.selectedRadioButtonBorderDefault.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[0], *p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[1], *p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[2], *p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[0], *p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[1], *p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[2], *p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = RadioButtonAnimationState::Selected;

            is_switch_success = true;
            break;
        }
        case RadioButtonAnimationState::SelectedHover:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryHover.getRed(), 0.5, 0.5, &*p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryHover.getGreen(), 0.5, 0.5, &*p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryHover.getBlue(), 0.5, 0.5, &*p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryHover.getRed(), 0.5, 0.5, &*p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryHover.getGreen(), 0.5, 0.5, &*p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryHover.getBlue(), 0.5, 0.5, &*p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonBorderHover.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonBorderHover.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.selectedRadioButtonBorderHover.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.textHighlight.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[0], *p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[1], *p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[2], *p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[0], *p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[1], *p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[2], *p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = RadioButtonAnimationState::SelectedHover;

            is_switch_success = true;
            break;
        }
        case RadioButtonAnimationState::SelectedDown:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryActive.getRed(), 0.5, 0.5, &*p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryActive.getGreen(), 0.5, 0.5, &*p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonPrimaryActive.getBlue(), 0.5, 0.5, &*p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryActive.getRed(), 0.5, 0.5, &*p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryActive.getGreen(), 0.5, 0.5, &*p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonSecondaryActive.getBlue(), 0.5, 0.5, &*p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonBorderActive.getRed(), 0.5, 0.5, &*p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonBorderActive.getGreen(), 0.5, 0.5, &*p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.selectedRadioButtonBorderActive.getBlue(), 0.5, 0.5, &*p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getRed(), 0.5, 0.5, &*p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getGreen(), 0.5, 0.5, &*p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getBlue(), 0.5, 0.5, &*p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[0], *p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[1], *p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonPrimaryRGB[2], *p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[0], *p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[1], *p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonSecondaryRGB[2], *p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[0], *p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[1], *p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonBorderRGB[2], *p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[0], *p_transition_button_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[1], *p_transition_button_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableButtonTextRGB[2], *p_transition_button_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = RadioButtonAnimationState::SelectedDown;

            is_switch_success = true;
            break;
        }
        case RadioButtonAnimationState::Focus:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 1.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        case RadioButtonAnimationState::FocusLost:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        }
        if (!is_switch_success)
        {
            error_message = "Failed to create and add the animation transitions to the storyboard.";
            break;
        }

        // Get the current time.
        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        // Set the storyboard tag.
        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->buttonWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        // Schedule the storyboard.
        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        // Set the animation invalidate timer.
        SetTimer(this->buttonWindow, MyRadioButtonSubclass::IDT_ANIMATION_INVALIDATE, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'startAnimation()']", MyLogType::Error);

    return are_all_operation_success;
}
LRESULT CALLBACK MyRadioButtonSubclass::subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyRadioButtonSubclass *p_this = reinterpret_cast<MyRadioButtonSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override paint messages.
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;

            // Get the latest animation values.
            DOUBLE focus_border_opacity = 0.0;
            INT32 button_primary_rgb[3] = {0, 0, 0};
            INT32 button_secondary_rgb[3] = {0, 0, 0};
            INT32 button_border_rgb[3] = {0, 0, 0};
            INT32 button_text_rgb[3] = {0, 0, 0};
            hr = (*p_this->pAnimationVariableFocusBorderOpacity)->GetValue(&focus_border_opacity);
            if (FAILED(hr) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableButtonPrimaryRGB, button_primary_rgb) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableButtonSecondaryRGB, button_secondary_rgb) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableButtonBorderRGB, button_border_rgb) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableButtonTextRGB, button_text_rgb))
            {
                error_message = "[WM_PAINT] Failed to retrieve the animation values.";
                break;
            }

            // Get the window client rect.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                break;
            }

            // Get the window text length.
            SetLastError(ERROR_SUCCESS);
            size_t window_text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
            if (!window_text_length && GetLastError())
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text length.";
                break;
            }

            // Get the window text.
            std::unique_ptr<WCHAR[]> window_text(new WCHAR[window_text_length + 1]);
            if (!GetWindowTextW(hWnd, window_text.get(), static_cast<INT>(window_text_length) + 1) && window_text_length)
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text.";
                break;
            }

            // Bind the render target to the window device context.
            hr = (*p_this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
            if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                break;
            }

            // Prepare drawing resources.
            D2D1_RECT_F d2d1_rect_window = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window.right), static_cast<FLOAT>(rect_window.bottom));
            D2D_RECT_F d2d1_rect_text = d2d1_rect_window;
            D2D1::ColorF d2d1_color_focus_border = p_this->objects()->colors.focus.getD2D1Color();
            d2d1_color_focus_border.a = static_cast<FLOAT>(focus_border_opacity);
            D2D1::ColorF d2d1_color_button_text(button_text_rgb[0] / 255.0f, button_text_rgb[1] / 255.0f, button_text_rgb[2] / 255.0f, 1.0);
            D2D1::ColorF d2d1_color_ellipse = D2D1::ColorF(button_secondary_rgb[0] / 255.0f, button_secondary_rgb[1] / 255.0f, button_secondary_rgb[2] / 255.0f, 1.0f);
            D2D1::ColorF d2d1_color_ellipse_inner = D2D1::ColorF(button_primary_rgb[0] / 255.0f, button_primary_rgb[1] / 255.0f, button_primary_rgb[2] / 255.0f, 1.0f);
            D2D1::ColorF d2d1_color_ellipse_border = D2D1::ColorF(button_border_rgb[0] / 255.0f, button_border_rgb[1] / 255.0f, button_border_rgb[2] / 255.0f, 1.0f);
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_focus_border(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_button_text(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_ellipse(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_ellipse_inner(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_ellipse_border(new ID2D1SolidColorBrush *(nullptr));
            {
                error_message = "[WM_PAINT] Failed to create the resources.";
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_focus_border, &*p_d2d1_solidcolorbrush_focus_border);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_button_text, &*p_d2d1_solidcolorbrush_button_text);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_ellipse, &*p_d2d1_solidcolorbrush_ellipse);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_ellipse_inner, &*p_d2d1_solidcolorbrush_ellipse_inner);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_ellipse_border, &*p_d2d1_solidcolorbrush_ellipse_border);
                if (FAILED(hr))
                    break;
                error_message = "";
            }

            // Begin drawing.
            (*p_this->pD2D1DCRenderTarget)->BeginDraw();
            (*p_this->pD2D1DCRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            (*p_this->pD2D1DCRenderTarget)->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            {
                // Draw the background.
                (*p_this->pD2D1DCRenderTarget)->Clear((p_this->config.pBackground ? p_this->config.pBackground->getD2D1Color() : p_this->objects()->colors.background.getD2D1Color()));

                // Draw the main ellipse.
                FLOAT ellipse_size = (d2d1_rect_window.bottom - d2d1_rect_window.top) * 0.55f / 2.0f;
                FLOAT ellipse_pos_y = (d2d1_rect_window.top + d2d1_rect_window.bottom) / 2.0f;
                FLOAT ellipse_pos_x = ellipse_pos_y;
                FLOAT text_start_pos = ellipse_pos_x + ellipse_size + 10.0f;
                (*p_this->pD2D1DCRenderTarget)->FillEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse_pos_x, ellipse_pos_y), ellipse_size, ellipse_size), *p_d2d1_solidcolorbrush_ellipse);

                // Draw the inner ellipse.
                FLOAT ellipse_inner_size = ellipse_size * 0.75f;
                (*p_this->pD2D1DCRenderTarget)->FillEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse_pos_x, ellipse_pos_y), ellipse_inner_size, ellipse_inner_size), *p_d2d1_solidcolorbrush_ellipse_inner);

                // Draw the ellipse border.
                (*p_this->pD2D1DCRenderTarget)->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse_pos_x, ellipse_pos_y), ellipse_size, ellipse_size), *p_d2d1_solidcolorbrush_ellipse_border, 1.0);

                // Draw the button text.
                p_this->graphics()->d2d1Engine().drawText((*p_this->pD2D1DCRenderTarget), *p_this->pTextFormat, d2d1_rect_text, window_text.get(), *p_d2d1_solidcolorbrush_button_text, d2d1_rect_text.left + text_start_pos, 0, 2);

                // Draw the focus border.
                if (focus_border_opacity)
                    (*p_this->pD2D1DCRenderTarget)->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse_pos_x, ellipse_pos_y), ellipse_size, ellipse_size), *p_d2d1_solidcolorbrush_focus_border, 1.0);
            }

            // End drawing.
            hr = (*p_this->pD2D1DCRenderTarget)->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!p_this->createDeviceResources(true))
                {
                    error_message = "[WM_PAINT] Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "[WM_PAINT] Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue; // Repeat the paint operation.
            }
            else if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        return 0;
    }

    // Process the animation invalidate timer.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyRadioButtonSubclass::IDT_ANIMATION_INVALIDATE:
        {
            HRESULT hr;
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

                IUIAnimationStoryboard *p_storyboard;
                hr = p_this->graphics()->wamEngine().manager()->GetStoryboardFromTag(NULL, GetDlgCtrlID(hWnd), &p_storyboard);
                if (SUCCEEDED(hr))
                {
                    if (!p_storyboard)
                    {
                        if (!KillTimer(hWnd, MyRadioButtonSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }

                        are_all_operation_success = true;
                        break;
                    }

                    UI_ANIMATION_STORYBOARD_STATUS storyboard_status;
                    hr = p_storyboard->GetStatus(&storyboard_status);
                    if (FAILED(hr))
                    {
                        error_message = "[WM_TIMER] Failed to get the storyboard status.";
                        break;
                    }

                    if (storyboard_status == UI_ANIMATION_STORYBOARD_READY)
                    {
                        if (!KillTimer(hWnd, MyRadioButtonSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer if an error occurs.
                if (!KillTimer(hWnd, MyRadioButtonSubclass::IDT_ANIMATION_INVALIDATE))
                    g_pApp->logger.writeLog("[WM_TIMER] Failed to kill the timer.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            g_pApp->logger.writeLog("[WM_TIMER] Unprocessed timer message.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process WM_GETDLGCODE messages to handle nagivation key input manually.
    case WM_GETDLGCODE:
    {
        bool is_focused = (GetFocus() == hWnd ? true : false);
        if (!is_focused)
            break;

        switch (wParam)
        {
        case VK_TAB:
        case VK_SPACE:
        case VK_RETURN:
            return DLGC_WANTALLKEYS; // Allow the window to capture the key message via WM_KEYDOWN/WM_KEYUP.
        }

        return 0; // Ignore nagivation key by default.
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_TAB:
        {
            HWND next_focus_window = GetNextDlgTabItem(*MyRadioButtonSubclass::pAppWindow, hWnd, static_cast<INT>(GetKeyState(VK_SHIFT) & 0x8000));
            SetFocus(next_focus_window);

            if (!p_this->isSelected)
            {
                if (p_this->currentAnimationState != RadioButtonAnimationState::Default)
                    if (!p_this->startAnimation(RadioButtonAnimationState::Default))
                        g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }
            else
            {
                if (p_this->currentAnimationState != RadioButtonAnimationState::Selected)
                    if (!p_this->startAnimation(RadioButtonAnimationState::Selected))
                        g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }
        case VK_SPACE:
        case VK_RETURN:
        {
            if (!p_this->isHoverState && p_this->currentAnimationState != RadioButtonAnimationState::Down && p_this->currentAnimationState != RadioButtonAnimationState::SelectedDown)
            {
                if (!p_this->isSelected)
                {
                    if (!p_this->config.skipActiveAnimationState)
                        if (!p_this->startAnimation(RadioButtonAnimationState::Down))
                            g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
                }
                else
                {
                    if (!p_this->config.skipActiveAnimationState)
                        if (!p_this->startAnimation(RadioButtonAnimationState::SelectedDown))
                            g_pApp->logger.writeLog("[WM_KEYDOWN] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
                }
            }

            return 0;
        }
        }

        break;
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_TAB:
            return 0;
        case VK_SPACE:
        case VK_RETURN:
        {
            if ((!p_this->isHoverState && p_this->currentAnimationState == RadioButtonAnimationState::Down) || p_this->currentAnimationState == RadioButtonAnimationState::SelectedDown)
            {
                if (!p_this->startAnimation(RadioButtonAnimationState::Selected))
                    g_pApp->logger.writeLog("[WM_KEYUP] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                if (!p_this->isSelected)
                {
                    p_this->updateSelectionState(true);
                    if (!p_this->pRadioGroup)
                    {
                        g_pApp->logger.writeLog("[WM_KEYUP] The radio button doesn't belong to any radio group.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
                        return 0;
                    }

                    if (!p_this->pRadioGroup->updateRadioState(GetDlgCtrlID(hWnd)))
                        g_pApp->logger.writeLog("[WM_KEYUP] Failed to update the radio group state.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                    SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), 0);
                }
            }

            return 0;
        }
        }

        break;
    }

    // Process the left button click-down messages to trigger appropriate actions.
    case WM_LBUTTONDOWN:
    {
        if (!p_this->isActiveState && p_this->isHoverState)
        {
            p_this->isActiveState = true;

            if (!p_this->isSelected)
            {
                if (!p_this->config.skipActiveAnimationState)
                    if (!p_this->startAnimation(RadioButtonAnimationState::Down))
                        g_pApp->logger.writeLog("[WM_LBUTTONDOWN] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }
            else
            {
                if (!p_this->config.skipActiveAnimationState)
                    if (!p_this->startAnimation(RadioButtonAnimationState::SelectedDown))
                        g_pApp->logger.writeLog("[WM_LBUTTONDOWN] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        break;
    }

    // Process the left button click-up messages to trigger appropriate actions.
    case WM_LBUTTONUP:
    {
        if (p_this->isActiveState)
        {
            p_this->isActiveState = false;

            if (p_this->isHoverState)
            {
                if (!p_this->isSelected)
                {
                    p_this->updateSelectionState(true);
                    if (!p_this->pRadioGroup)
                    {
                        g_pApp->logger.writeLog("[WM_LBUTTONUP] The radio button doesn't belong to any radio group.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
                        return 0;
                    }

                    if (!p_this->pRadioGroup->updateRadioState(GetDlgCtrlID(hWnd)))
                        g_pApp->logger.writeLog("[WM_LBUTTONUP] Failed to update the radio group state.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                    SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), 0);
                }

                if (!p_this->config.skipHoverAnimationState)
                    if (!p_this->startAnimation(RadioButtonAnimationState::SelectedHover))
                        g_pApp->logger.writeLog("[WM_LBUTTONUP] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }
        }

        break;
    }

    // Process the mouse leave messages to trigger appropriate actions.
    case WM_MOUSELEAVE:
    {
        if (p_this->isHoverState)
        {
            p_this->isHoverState = false;
            p_this->isActiveState = false;

            if (!p_this->isSelected)
            {
                if (!p_this->startAnimation(RadioButtonAnimationState::Default))
                    g_pApp->logger.writeLog("[WM_MOUSELEAVE] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }
            else
            {
                if (!p_this->config.skipSelectedAnimationState)
                    if (!p_this->startAnimation(RadioButtonAnimationState::Selected))
                        g_pApp->logger.writeLog("[WM_MOUSELEAVE] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        break;
    }

    // Process the mouse move messages to trigger appropriate actions.
    case WM_MOUSEMOVE:
    {
        if (!p_this->isHoverState)
        {
            TRACKMOUSEEVENT track_mouse_event;
            track_mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
            track_mouse_event.dwFlags = TME_LEAVE;
            track_mouse_event.hwndTrack = hWnd;
            TrackMouseEvent(&track_mouse_event);
            p_this->isHoverState = true;

            if (!p_this->isSelected)
            {
                if (!p_this->config.skipHoverAnimationState)
                    if (!p_this->startAnimation(RadioButtonAnimationState::Hover))
                        g_pApp->logger.writeLog("[WM_MOUSEMOVE] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }
            else
            {
                if (!p_this->config.skipHoverAnimationState)
                    if (!p_this->startAnimation(RadioButtonAnimationState::SelectedHover))
                        g_pApp->logger.writeLog("[WM_MOUSEMOVE] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        break;
    }

    // Update the appearance of the edit window when it gains focus.
    case WM_SETFOCUS:
    {
        if (!p_this->startAnimation(RadioButtonAnimationState::Focus))
            g_pApp->logger.writeLog("[WM_SETFOCUS] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
        break;
    }

    // Update the appearance of the edit window when it loses focus.
    case WM_KILLFOCUS:
    {
        if (!p_this->startAnimation(RadioButtonAnimationState::FocusLost))
            g_pApp->logger.writeLog("[WM_KILLFOCUS] Failed to start the animation.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyRadioButtonSubclass::subclassProcedure, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyRadioButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// [MyRadioGroup] class implementations:

bool MyRadioGroup::addRadioButton(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(hWnd))
        {
            error_message = "The window object is invalid";
            break;
        }

        auto p_subclass = MyRadioButtonSubclass::getSubclassPointer(hWnd);
        if (!p_subclass)
        {
            error_message = "The window is incompatible (Not subclassed by MyRadioButtonSubclass class).";
            break;
        }

        // Link the subclass object with the radio group.
        p_subclass->pRadioGroup = this;

        // Add the button to the map.
        auto insert_result = this->mapButtons.insert(std::make_pair(GetDlgCtrlID(hWnd), std::make_pair(hWnd, p_subclass)));
        if (!insert_result.second)
        {
            error_message = "A window with the same ID already exists in the radio group.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioGroup' | FUNC: 'addRadioButton()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyRadioGroup::removeRadioButton(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        auto window_id = GetDlgCtrlID(hWnd);
        auto it = this->mapButtons.find(window_id);
        if (it != this->mapButtons.end())
        {
            auto p_subclass = MyRadioButtonSubclass::getSubclassPointer(hWnd);
            if (!p_subclass)
            {
                error_message = "The window is incompatible (Not subclassed by MyRadioButtonSubclass class).";
                break;
            }
            p_subclass->updateSelectionState(false); // Deselect the radio button.
            p_subclass->pRadioGroup = nullptr;       // Unlink the radio button from the group.

            if (this->currentSelectedButtonID == window_id) // Set the radio group state to none.
                this->currentSelectedButtonID = 0;

            this->mapButtons.erase(it);
        }
        else
        {
            error_message = "This window doesn't exist in the radio group.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyRadioGroup' | FUNC: 'removeRadioButton()']", MyLogType::Error);

    return are_all_operation_success;
}
int MyRadioGroup::getRadioState()
{
    return this->currentSelectedButtonID;
}
bool MyRadioGroup::updateRadioState(INT buttonID)
{
    for (const auto &button : this->mapButtons)
    {
        if (button.first == buttonID)
        {
            this->currentSelectedButtonID = buttonID;

            for (const auto &button_2 : this->mapButtons)
            {
                if (button_2.first != buttonID)
                    button_2.second.second->updateSelectionState(false);
            }

            return true;
        }
    }

    g_pApp->logger.writeLog("This window ID doesn't exists in the radio group.", "[CLASS: 'MyRadioGroup' | FUNC: 'updateRadioState()']", MyLogType::Error);

    return false;
}
void MyRadioGroup::resetRadioGroup()
{
    this->currentSelectedButtonID = 0;
    this->mapButtons.clear();
}

// [MyEditboxSubclass] class implementations:

MyEditboxSubclass::~MyEditboxSubclass()
{
    if (!DestroyWindow(this->staticWindow))
        g_pApp->logger.writeLog("Failed to destroy the associated static window.", "[CLASS: 'MyEditboxSubclass' | FUNC: 'Destructor']", MyLogType::Error);
}
HWND MyEditboxSubclass::getStaticHandle()
{
    return this->staticWindow;
}
HWND &MyEditboxSubclass::getStaticHandleRef()
{
    return this->staticWindow;
}
bool MyEditboxSubclass::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass object is not associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is not associated with a window.";
            break;
        }

        // Calculate the font size and create the font object.
        RECT rect_window{0};
        if (!GetClientRect(this->editboxWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }
        DWORD editbox_style = GetWindowLongW(this->editboxWindow, GWL_STYLE);
        FLOAT font_size;
        this->editboxFont.reset(new HFONT(nullptr));
        if (editbox_style & ES_MULTILINE)
        {
            if (this->editConfig.fontSize)
                font_size = this->editConfig.fontSize;
            else
                font_size = 29.0f;
        }
        else
        {
            if (this->editConfig.fontSize)
                font_size = this->editConfig.fontSize;
            else
                font_size = static_cast<FLOAT>(rect_window.bottom - rect_window.top);
        }
        *this->editboxFont = CreateFontW(static_cast<int>(font_size), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                         CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                         (!this->editConfig.editboxWindowFontFamilyName.empty() ? this->editConfig.editboxWindowFontFamilyName.c_str() : this->objects()->fonts.defaultFamily.c_str()));
        if (!*this->editboxFont)
        {
            error_message = "Failed to create the font object.";
            break;
        }

        // Update the edit window character formats.
        SendMessageW(this->editboxWindow, WM_SETFONT, (WPARAM) * this->editboxFont, FALSE);
        this->editDefaultCharFormat.cbSize = sizeof(CHARFORMAT2);
        this->editDefaultCharFormat.dwMask = CFM_COLOR | CFM_BACKCOLOR;
        this->editDefaultCharFormat.crTextColor = this->objects()->colors.textActive.getCOLORREF();
        this->editDefaultCharFormat.crBackColor = this->objects()->colors.editbox.getCOLORREF();
        if (!SendMessageW(this->editboxWindow, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) & this->editDefaultCharFormat) ||
            !SendMessageW(this->editboxWindow, EM_SETBKGNDCOLOR, 0, (LPARAM)this->objects()->colors.editbox.getCOLORREF()))
        {
            error_message = "Failed to set the edit window character formats.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        // Start animation to the current animation state to update animation variables values.
        if (!this->startAnimation(this->currentAnimationState))
        {
            error_message = "Failed to start the animation.";
            break;
        }

        // Extra redraw on the editbox window.
        RedrawWindow(this->editboxWindow, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyEditboxSubclass' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
void MyEditboxSubclass::releaseSharedDeviceResources()
{
    MyEditboxSubclass::pSharedD2D1SolidColorBrushEditbox.reset();
    MyEditboxSubclass::pSharedD2D1SolidColorBrushEditboxBackground.reset();
}
void MyEditboxSubclass::setAnimationDuration(EditAnimationState animateState, FLOAT milliseconds)
{
    switch (animateState)
    {
    case EditAnimationState::Default:
        this->defaultAnimationDuration = milliseconds;
        break;
    case EditAnimationState::Selected:
        this->selectedAnimationDuration = milliseconds;
        break;
    }
}
bool MyEditboxSubclass::setWindow(HWND hWnd, MyEditboxSubclassConfig *pConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass base class is initialized.
        if (!this->isInitialized)
        {
            error_message = "The subclass base class is not initialized.";
            break;
        }

        // Check if the subclass object is already associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is already associated with a window.";
            break;
        }

        // Check if the configuration structure is valid.
        if (!pConfig->isValid())
        {
            error_message = "The configuration structure contains invalid parameters.";
            break;
        }
        this->editConfig = *pConfig;
        this->editboxWindow = hWnd;

        // Calculate the font size and create the font object.
        RECT rect_window{0};
        if (!GetClientRect(this->editboxWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }
        DWORD editbox_style = GetWindowLongW(this->editboxWindow, GWL_STYLE);
        FLOAT font_size;
        this->editboxFont.reset(new HFONT(nullptr));
        if (editbox_style & ES_MULTILINE)
        {
            if (this->editConfig.fontSize)
                font_size = this->editConfig.fontSize;
            else
                font_size = 29.0f;
        }
        else
        {
            if (this->editConfig.fontSize)
                font_size = this->editConfig.fontSize;
            else
                font_size = static_cast<FLOAT>(rect_window.bottom - rect_window.top);
        }
        *this->editboxFont = CreateFontW(static_cast<int>(font_size), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                         CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                         (!this->editConfig.editboxWindowFontFamilyName.empty() ? this->editConfig.editboxWindowFontFamilyName.c_str() : this->objects()->fonts.defaultFamily.c_str()));
        if (!*this->editboxFont)
        {
            error_message = "Failed to create the font object.";
            break;
        }

        // Set the edit window character formats.
        SendMessageW(this->editboxWindow, WM_SETFONT, (WPARAM) * this->editboxFont, FALSE);
        this->editDefaultCharFormat.cbSize = sizeof(CHARFORMAT2);
        this->editDefaultCharFormat.dwMask = CFM_COLOR | CFM_BACKCOLOR;
        this->editDefaultCharFormat.crTextColor = this->objects()->colors.textActive.getCOLORREF();
        this->editDefaultCharFormat.crBackColor = this->objects()->colors.editbox.getCOLORREF();
        if (!SendMessageW(this->editboxWindow, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) & this->editDefaultCharFormat) ||
            !SendMessageW(this->editboxWindow, EM_SETBKGNDCOLOR, 0, (LPARAM)this->objects()->colors.editbox.getCOLORREF()))
        {
            error_message = "Failed to set the edit window character formats.";
            break;
        }

        // Subclass the windows.
        // Store this subclass object pointer as reference data, enabling the subclass procedure to access the non-static object members.
        if (!SetWindowSubclass(this->editboxWindow, &MyEditboxSubclass::subclassProcedureEdit, 0, reinterpret_cast<DWORD_PTR>(this)) ||
            !SetWindowSubclass(this->staticWindow, &MyEditboxSubclass::subclassProcedureStatic, 0, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = "Failed to install the window subclass callback.";
            break;
        }

        // Create the animation variables.
        {
            error_message = "Failed to create the animation variables.";
            this->pAnimationVariableEditboxBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableEditboxBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableEditboxBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableEditboxBorderRGB[0],
                                                                       this->objects()->colors.editboxBorderDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableEditboxBorderRGB[1],
                                                                       this->objects()->colors.editboxBorderDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableEditboxBorderRGB[2],
                                                                       this->objects()->colors.editboxBorderDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            error_message = "";
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyEditboxSubclass' | FUNC: 'setWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
MyEditboxSubclass *MyEditboxSubclass::getSubclassPointer(HWND hWnd)
{
    DWORD_PTR reference_data;
    bool is_subclassed;

    is_subclassed = GetWindowSubclass(hWnd, &MyEditboxSubclass::subclassProcedureEdit, 0, &reference_data);
    if (is_subclassed)
        return reinterpret_cast<MyEditboxSubclass *>(reference_data);

    is_subclassed = GetWindowSubclass(hWnd, &MyEditboxSubclass::subclassProcedureStatic, 0, &reference_data);
    if (is_subclassed)
        return reinterpret_cast<MyEditboxSubclass *>(reference_data);

    return nullptr;
}
bool MyEditboxSubclass::createSharedDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        error_message = "Failed to create the shared device resources.";
        if (!MyEditboxSubclass::pSharedD2D1SolidColorBrushEditbox)
        {
            MyEditboxSubclass::pSharedD2D1SolidColorBrushEditbox.reset(new ID2D1SolidColorBrush *(nullptr));
            hr = (*this->pD2D1DCRenderTarget)->CreateSolidColorBrush(this->objects()->colors.editbox.getD2D1Color(), &*MyEditboxSubclass::pSharedD2D1SolidColorBrushEditbox);
            if (FAILED(hr))
                break;
        }
        if (!MyEditboxSubclass::pSharedD2D1SolidColorBrushEditboxBackground)
        {
            MyEditboxSubclass::pSharedD2D1SolidColorBrushEditboxBackground.reset(new ID2D1SolidColorBrush *(nullptr));
            hr = (*this->pD2D1DCRenderTarget)->CreateSolidColorBrush(this->objects()->colors.background.getD2D1Color(), &*MyEditboxSubclass::pSharedD2D1SolidColorBrushEditboxBackground);
            if (FAILED(hr))
                break;
        }
        error_message = "";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyEditboxSubclass' | FUNC: 'createSharedDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyEditboxSubclass::createDeviceResources(bool recreateSharedResources)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        HDC hdc = GetDC(this->staticWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->staticWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pD2D1DCRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->graphics()->d2d1Engine().createDCRenderTarget(*this->pD2D1DCRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        hr = (*this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        if (recreateSharedResources)
            MyEditboxSubclass::releaseSharedDeviceResources();

        if (!this->createSharedDeviceResources())
        {
            error_message = "Failed to create the shared device resources.";
            break;
        }

        // Create the device resources here ...
        // <This subclass class doesn't have any non-shared device resources>

        if (!ReleaseDC(this->staticWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyEditboxSubclass' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyEditboxSubclass::startAnimation(EditAnimationState animationState)
{
    HRESULT hr;
    std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter> p_storyboard(new IUIAnimationStoryboard *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_editbox_border_rgb;
    p_transition_editbox_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_editbox_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_editbox_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));

    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Create the storyboard.
        hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        // Create and add the animation transitions to the storyboard.
        bool is_switch_success = false;
        switch (animationState)
        {
        case EditAnimationState::Default:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.editboxBorderDefault.getRed(), 0.5, 0.5, &*p_transition_editbox_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.editboxBorderDefault.getGreen(), 0.5, 0.5, &*p_transition_editbox_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.editboxBorderDefault.getBlue(), 0.5, 0.5, &*p_transition_editbox_border_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableEditboxBorderRGB[0], *p_transition_editbox_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableEditboxBorderRGB[1], *p_transition_editbox_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableEditboxBorderRGB[2], *p_transition_editbox_border_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = EditAnimationState::Default;

            is_switch_success = true;
            break;
        }
        case EditAnimationState::Selected:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->selectedAnimationDuration, this->objects()->colors.editboxBorderSelected.getRed(), 0.5, 0.5, &*p_transition_editbox_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->selectedAnimationDuration, this->objects()->colors.editboxBorderSelected.getGreen(), 0.5, 0.5, &*p_transition_editbox_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->selectedAnimationDuration, this->objects()->colors.editboxBorderSelected.getBlue(), 0.5, 0.5, &*p_transition_editbox_border_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableEditboxBorderRGB[0], *p_transition_editbox_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableEditboxBorderRGB[1], *p_transition_editbox_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableEditboxBorderRGB[2], *p_transition_editbox_border_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = EditAnimationState::Selected;

            is_switch_success = true;
            break;
        }
        }
        if (!is_switch_success)
        {
            error_message = "Failed to create and add the animation transitions to the storyboard.";
            break;
        }

        // Get the current time.
        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        // Set the storyboard tag.
        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->staticWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        // Schedule the storyboard.
        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        // Set the animation invalidate timer.
        SetTimer(this->staticWindow, MyEditboxSubclass::IDT_ANIMATION_INVALIDATE, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyEditbox' | FUNC: 'startAnimation()']", MyLogType::Error);

    return are_all_operation_success;
}
LRESULT CALLBACK MyEditboxSubclass::subclassProcedureEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyEditboxSubclass *p_this = reinterpret_cast<MyEditboxSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Process WM_GETDLGCODE messages to handle nagivation key input manually.
    case WM_GETDLGCODE:
    {
        bool is_focused = (GetFocus() == hWnd ? true : false);
        if (!is_focused)
            break;

        DWORD window_style = GetWindowLongW(hWnd, GWL_STYLE);
        DWORD result = 0; // Ignore nagivation key by default.

        switch (wParam)
        {
        case VK_TAB:
        {
            if (!(window_style & WS_TABSTOP))
                result = DLGC_WANTARROWS | DLGC_HASSETSEL | DLGC_WANTCHARS | DLGC_WANTTAB;
            break;
        }
        case VK_RETURN:
        {
            result = DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_HASSETSEL | DLGC_WANTCHARS;
            break;
        }
        }

        return result;
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return 0;
        case VK_RETURN:
            break;
        }

        break;
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYUP:
    {
        switch (wParam)
        {
        // When the multiline edit control is in focus, pressing the ESC key can lead to unexpected behaviors and crashes.
        // The cause is currently unknown. To prevent this, we override the ESC key.
        case VK_ESCAPE:
            return 0;
        case VK_RETURN:
        {
            // If the associated WM_COMMAND message ID is set, trigger the corresponding message.
            if (p_this->editConfig.associatedWMCommandMessageID)
                SendMessageW(GetParent(hWnd), WM_COMMAND, p_this->editConfig.associatedWMCommandMessageID, 0);
            break;
        }
        }

        break;
    }

    // Update the appearance of the edit window when it gains focus.
    case WM_SETFOCUS:
    {
        if (!p_this->startAnimation(EditAnimationState::Selected))
            g_pApp->logger.writeLog("[WM_SETFOCUS] Failed to start the animation.", "[CLASS: 'MyEditboxSubclass' | FUNC: 'subclassProcedureEdit()']", MyLogType::Error);

        break;
    }

    // Update the appearance of the edit window when it loses focus.
    case WM_KILLFOCUS:
    {
        if (!p_this->startAnimation(EditAnimationState::Default))
            g_pApp->logger.writeLog("[WM_KILLFOCUS] Failed to start the animation.", "[CLASS: 'MyEditboxSubclass' | FUNC: 'subclassProcedureEdit()']", MyLogType::Error);

        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyEditboxSubclass::subclassProcedureEdit, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyEditboxSubclass' | FUNC: 'subclassProcedureEdit()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK MyEditboxSubclass::subclassProcedureStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyEditboxSubclass *p_this = reinterpret_cast<MyEditboxSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override paint messages.
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;

            // Get the latest animation values.
            INT32 editbox_border_rgb[3] = {0, 0, 0};
            error_message = "[WM_PAINT] Failed to retrieve the animation values.";
            if (!p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableEditboxBorderRGB, editbox_border_rgb))
                break;
            error_message = "";

            // Get the window client rects.
            RECT rect_window_1, rect_window_2;
            if (!GetClientRect(hWnd, &rect_window_1))
            {
                error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                break;
            }
            rect_window_2 = rect_window_1;
            rect_window_2.left += 1;
            rect_window_2.right -= 1;
            rect_window_2.top += 1;
            rect_window_2.bottom -= 1;

            // Bind the render target to the window device context.
            hr = (*p_this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window_1);
            if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                break;
            }

            // Prepare drawing resources.
            D2D1_RECT_F d2d1_rect_window_1 = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window_1.right), static_cast<FLOAT>(rect_window_1.bottom));
            D2D1_RECT_F d2d1_rect_window_2 = D2D1::RectF(static_cast<FLOAT>(rect_window_2.left), static_cast<FLOAT>(rect_window_2.top), static_cast<FLOAT>(rect_window_2.right), static_cast<FLOAT>(rect_window_2.bottom));
            D2D1::ColorF d2d1_color_border = D2D1::ColorF(editbox_border_rgb[0] / 255.0f, editbox_border_rgb[1] / 255.0f, editbox_border_rgb[2] / 255.0f, 1.0);
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_background_custom(new ID2D1SolidColorBrush *(nullptr));
            auto &p_d2d1_solidcolorbrush_background = (p_this->config.pBackground ? *p_d2d1_solidcolorbrush_background_custom : *MyEditboxSubclass::pSharedD2D1SolidColorBrushEditboxBackground);
            auto &p_d2d1_solidcolorbrush_edit = *MyEditboxSubclass::pSharedD2D1SolidColorBrushEditbox;
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_border(new ID2D1SolidColorBrush *(nullptr));
            {
                error_message = "[WM_PAINT] Failed to create the resources.";
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_border, &*p_d2d1_solidcolorbrush_border);
                if (FAILED(hr))
                    break;
                if (!p_d2d1_solidcolorbrush_background)
                {
                    hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(p_this->config.pBackground->getD2D1Color(), &p_d2d1_solidcolorbrush_background);
                    if (FAILED(hr))
                        break;
                }
                error_message = "";
            }

            // Begin drawing.
            (*p_this->pD2D1DCRenderTarget)->BeginDraw();
            (*p_this->pD2D1DCRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

            // Draw the background.
            (*p_this->pD2D1DCRenderTarget)->Clear(p_this->objects()->colors.editbox.getD2D1Color());

            // Draw the border.
            p_this->graphics()->d2d1Engine().drawRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window_1, p_d2d1_solidcolorbrush_background);
            p_this->graphics()->d2d1Engine().drawRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window_2, p_d2d1_solidcolorbrush_edit);
            p_this->graphics()->d2d1Engine().drawRoundRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window_1, 4, 4, *p_d2d1_solidcolorbrush_border);

            // End drawing.
            hr = (*p_this->pD2D1DCRenderTarget)->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!p_this->createDeviceResources(true))
                {
                    error_message = "[WM_PAINT] Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "[WM_PAINT] Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue; // Repeat the paint operation.
            }
            else if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyEditboxSubclass' | FUNC: 'subclassProcedureStatic()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        return 0;
    }

    // Process the animation invalidate timer.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyEditboxSubclass::IDT_ANIMATION_INVALIDATE:
        {
            HRESULT hr;
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

                IUIAnimationStoryboard *p_storyboard;
                hr = p_this->graphics()->wamEngine().manager()->GetStoryboardFromTag(NULL, GetDlgCtrlID(hWnd), &p_storyboard);
                if (SUCCEEDED(hr))
                {
                    if (!p_storyboard)
                    {
                        if (!KillTimer(hWnd, MyEditboxSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }

                        are_all_operation_success = true;
                        break;
                    }

                    UI_ANIMATION_STORYBOARD_STATUS storyboard_status;
                    hr = p_storyboard->GetStatus(&storyboard_status);
                    if (FAILED(hr))
                    {
                        error_message = "[WM_TIMER] Failed to get the storyboard status.";
                        break;
                    }

                    if (storyboard_status == UI_ANIMATION_STORYBOARD_READY)
                    {
                        if (!KillTimer(hWnd, MyEditboxSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer if an error occurs.
                if (!KillTimer(hWnd, MyEditboxSubclass::IDT_ANIMATION_INVALIDATE))
                    g_pApp->logger.writeLog("[WM_TIMER] Failed to kill the timer.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                g_pApp->logger.writeLog(error_message, "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            g_pApp->logger.writeLog("[WM_TIMER] Unprocessed timer message.", "[CLASS: 'MyEditboxSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyEditboxSubclass::subclassProcedureStatic, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyEditboxSubclass' | FUNC: 'subclassProcedureStatic()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// [MyDDLComboboxSubclass] class implementations:

bool MyDDLComboboxSubclass::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass object is not associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is not associated with a window.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        // Perform additional touches to the combobox.
        {
            // Create compatible font for the drop-down list window.
            this->ddlFont.reset(new HFONT(nullptr));
            *this->ddlFont = CreateFontW(static_cast<int>(-((*this->pTextFormat)->GetFontSize())), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                         CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                         this->objects()->fonts.defaultFamily.c_str());

            // If the combobox drop down is opening during the refresh, close the drop down and mark it for reopen.
            bool reopen_combobox = false;
            if (ComboBox_GetDroppedState(this->comboboxWindow))
            {
                ComboBox_ShowDropdown(this->comboboxWindow, false);
                reopen_combobox = true;
            }

            // Set font for the drop-down list window.
            SendMessageW(this->comboboxWindow, WM_SETFONT, reinterpret_cast<WPARAM>(*this->ddlFont), FALSE);

            // Get the text metrics to calculate perfect item height.
            error_message = "Failed to get the text metrics.";
            HDC hdc = GetDC(this->comboboxWindow);
            if (!hdc)
                break;
            TEXTMETRIC text_metrics;
            ZeroMemory(&text_metrics, sizeof(text_metrics));
            if (!GetTextMetricsW(hdc, &text_metrics))
                break;
            if (!ReleaseDC(this->comboboxWindow, hdc))
                break;
            error_message = "";

            // Set the combobox items height.
            ComboBox_SetItemHeight(this->comboboxWindow, 0, static_cast<int>(text_metrics.tmHeight + text_metrics.tmInternalLeading + text_metrics.tmExternalLeading + 10.0f));

            // Reopen the combobox if it was previously opened.
            if (reopen_combobox)
            {
                ComboBox_ShowDropdown(this->comboboxWindow, true);
                RedrawWindow(this->ddlWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
        }

        // Start animation to the current animation state to update animation variables values.
        if (!this->startAnimation(this->currentAnimationState))
        {
            error_message = "Failed to start the animation.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
void MyDDLComboboxSubclass::releaseSharedDeviceResources()
{
    MyDDLComboboxSubclass::pSharedD2D1BitmapArrow.reset();
}
void MyDDLComboboxSubclass::setAnimationDuration(DDLComboboxAnimationState animateState, FLOAT milliseconds)
{
    switch (animateState)
    {
    case DDLComboboxAnimationState::Default:
        this->defaultAnimationDuration = milliseconds;
        break;
    case DDLComboboxAnimationState::Active:
        this->activeAnimationDuration = milliseconds;
        break;
    case DDLComboboxAnimationState::Focus:
    case DDLComboboxAnimationState::FocusLost:
        this->focusAnimationDuration = milliseconds;
        break;
    }
}
bool MyDDLComboboxSubclass::setWindow(HWND hWnd, INT comboboxHeight)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass base class is initialized.
        if (!this->isInitialized)
        {
            error_message = "The subclass base class is not initialized.";
            break;
        }

        // Check if the subclass object is already associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is already associated with a window.";
            break;
        }

        // Get the combobox and its drop-down list window handles.
        COMBOBOXINFO combobox_info{};
        combobox_info.cbSize = sizeof(COMBOBOXINFO);
        if (!GetComboBoxInfo(hWnd, &combobox_info))
        {
            error_message = "Failed to retrieve the combobox info.";
            break;
        }
        this->ddlWindow = combobox_info.hwndList;
        this->comboboxWindow = hWnd;
        if (!this->ddlWindow || !IsWindow(this->ddlWindow))
        {
            error_message = "The drop-down list window handle is invalid.";
            break;
        }

        // The window must be a drop-down list combobox.
        DWORD combobox_style = static_cast<DWORD>(GetWindowLongW(this->comboboxWindow, GWL_STYLE));
        if (!(combobox_style & CBS_DROPDOWNLIST))
        {
            error_message = "The window is not a drop-down list combobox.";
            break;
        }

        // Subclass the windows.
        // Store this subclass object pointer as reference data, enabling the subclass procedure to access the non-static object members.
        if (!SetWindowSubclass(this->comboboxWindow, &MyDDLComboboxSubclass::subclassProcedureCombobox, 0, reinterpret_cast<DWORD_PTR>(this)) ||
            !SetWindowSubclass(this->ddlWindow, &MyDDLComboboxSubclass::subclassProcedureComboboxDDL, 0, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = "Failed to install the window subclass callback.";
            break;
        }

        // Modify the drop-down list window.
        {
            // Set class background brush to none.
            SetClassLongPtrW(this->ddlWindow, GCLP_HBRBACKGROUND, (LONG_PTR)NULL);

            // Remove the drop-down list window border.
            if (!MyUtility::RemoveWindowStyle(this->ddlWindow, WS_BORDER))
            {
                error_message = "Failed to remove the window style 'WS_BORDER'.";
                break;
            }

            // Remove drop-shadow effects.
            MyUtility::RemoveWindowClassStyle(this->ddlWindow, CS_DROPSHADOW);
        }

        // Create the animation variables.
        {
            error_message = "Failed to create the animation variables.";
            this->pAnimationVariableFocusBorderOpacity.reset(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxBorderRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            this->pAnimationVariableComboboxTextRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableFocusBorderOpacity, 0.0, 0.0, 1.0))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxRGB[0],
                                                                       this->objects()->colors.ddlComboboxDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxRGB[1],
                                                                       this->objects()->colors.ddlComboboxDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxRGB[2],
                                                                       this->objects()->colors.ddlComboboxDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxBorderRGB[0],
                                                                       this->objects()->colors.ddlComboboxBorder.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxBorderRGB[1],
                                                                       this->objects()->colors.ddlComboboxBorder.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxBorderRGB[2],
                                                                       this->objects()->colors.ddlComboboxBorder.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxTextRGB[0],
                                                                       this->objects()->colors.textActive.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxTextRGB[1],
                                                                       this->objects()->colors.textActive.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableComboboxTextRGB[2],
                                                                       this->objects()->colors.textActive.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            error_message = "";
        }

        // Create the device resources.
        if (!this->createDeviceResources(false, comboboxHeight))
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        // Perform additional touches to the combobox.
        {
            // Create compatible font for the drop-down list window.
            this->ddlFont.reset(new HFONT(nullptr));
            *this->ddlFont = CreateFontW(static_cast<int>(-((*this->pTextFormat)->GetFontSize())), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                         CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                         this->objects()->fonts.defaultFamily.c_str());

            // Set the combobox height.
            ComboBox_SetItemHeight(this->comboboxWindow, -1, comboboxHeight - 6);

            // Set font for the drop-down list window.
            SendMessageW(this->comboboxWindow, WM_SETFONT, reinterpret_cast<WPARAM>(*this->ddlFont), FALSE);

            // Get the text metrics to calculate perfect item height.
            error_message = "Failed to get the text metrics.";
            HDC hdc = GetDC(this->comboboxWindow);
            if (!hdc)
                break;
            TEXTMETRIC text_metrics;
            ZeroMemory(&text_metrics, sizeof(text_metrics));
            if (!GetTextMetricsW(hdc, &text_metrics))
                break;
            if (!ReleaseDC(this->comboboxWindow, hdc))
                break;
            error_message = "";

            // Set the combobox items height.
            ComboBox_SetItemHeight(this->comboboxWindow, 0, static_cast<int>(text_metrics.tmHeight + text_metrics.tmInternalLeading + text_metrics.tmExternalLeading + 10.0f));
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'setWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
MyDDLComboboxSubclass *MyDDLComboboxSubclass::getSubclassPointer(HWND hWnd)
{
    DWORD_PTR reference_data;
    bool is_subclassed;

    is_subclassed = GetWindowSubclass(hWnd, &MyDDLComboboxSubclass::subclassProcedureCombobox, 0, &reference_data);
    if (is_subclassed)
        return reinterpret_cast<MyDDLComboboxSubclass *>(reference_data);

    is_subclassed = GetWindowSubclass(hWnd, &MyDDLComboboxSubclass::subclassProcedureComboboxDDL, 0, &reference_data);
    if (is_subclassed)
        return reinterpret_cast<MyDDLComboboxSubclass *>(reference_data);

    return nullptr;
}
bool MyDDLComboboxSubclass::createSharedDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        error_message = "Failed to create the shared device resources.";
        if (!MyDDLComboboxSubclass::pSharedD2D1BitmapArrow)
        {
            MyDDLComboboxSubclass::pSharedD2D1BitmapArrow.reset(new ID2D1Bitmap *(nullptr));
            hr = (*this->pD2D1DCRenderTarget)->CreateBitmapFromWicBitmap(this->objects()->images.pWicBitmapDDLComboboxArrow->getBitmapSource(), &*MyDDLComboboxSubclass::pSharedD2D1BitmapArrow);
            if (FAILED(hr))
                break;
        }
        error_message = "";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'createSharedDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyDDLComboboxSubclass::createDeviceResources(bool recreateSharedResources, int comboboxHeight)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        HDC hdc = GetDC(this->comboboxWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->comboboxWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pD2D1DCRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->graphics()->d2d1Engine().createDCRenderTarget(*this->pD2D1DCRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        hr = (*this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        if (recreateSharedResources)
            MyDDLComboboxSubclass::releaseSharedDeviceResources();

        if (!this->createSharedDeviceResources())
        {
            error_message = "Failed to create the shared device resources.";
            break;
        }

        error_message = "Failed to create the device resources.";
        this->pTextFormat.reset(new IDWriteTextFormat *(nullptr));
        if (!comboboxHeight)
            comboboxHeight = rect_window.bottom - rect_window.top;
        FLOAT font_size = static_cast<FLOAT>((comboboxHeight)) * 0.5f;
        if (!this->config.ignoreTextScalingLimits)
        {
            if (font_size < this->config.textLowerBoundSizeLimit)
                font_size = this->config.textLowerBoundSizeLimit;
            else if (font_size > this->config.textUpperBoundSizeLimit)
                font_size = this->config.textUpperBoundSizeLimit;
        }
        if (!this->graphics()->d2d1Engine().createTextFormat(*this->pTextFormat, this->objects()->fonts.defaultFamily, font_size))
            break;
        error_message = "";

        if (!ReleaseDC(this->comboboxWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyDDLComboboxSubclass::startAnimation(DDLComboboxAnimationState animationState)
{
    HRESULT hr;
    std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter> p_storyboard(new IUIAnimationStoryboard *(nullptr));
    std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_focus_border_opacity(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_combobox_rgb;
    p_transition_combobox_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_combobox_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_combobox_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_combobox_border_rgb;
    p_transition_combobox_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_combobox_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_combobox_border_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_combobox_text_rgb;
    p_transition_combobox_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_combobox_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_combobox_text_rgb.emplace_back(new IUIAnimationTransition *(nullptr));

    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Create the storyboard.
        hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        // Create and add the animation transitions to the storyboard.
        bool is_switch_success = false;
        switch (animationState)
        {
        case DDLComboboxAnimationState::Default:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.ddlComboboxDefault.getRed(), 0.5, 0.5, &*p_transition_combobox_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.ddlComboboxDefault.getGreen(), 0.5, 0.5, &*p_transition_combobox_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.ddlComboboxDefault.getBlue(), 0.5, 0.5, &*p_transition_combobox_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.ddlComboboxBorder.getRed(), 0.5, 0.5, &*p_transition_combobox_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.ddlComboboxBorder.getGreen(), 0.5, 0.5, &*p_transition_combobox_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.ddlComboboxBorder.getBlue(), 0.5, 0.5, &*p_transition_combobox_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getRed(), 0.5, 0.5, &*p_transition_combobox_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getGreen(), 0.5, 0.5, &*p_transition_combobox_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.textActive.getBlue(), 0.5, 0.5, &*p_transition_combobox_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxRGB[0], *p_transition_combobox_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxRGB[1], *p_transition_combobox_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxRGB[2], *p_transition_combobox_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxBorderRGB[0], *p_transition_combobox_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxBorderRGB[1], *p_transition_combobox_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxBorderRGB[2], *p_transition_combobox_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxTextRGB[0], *p_transition_combobox_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxTextRGB[1], *p_transition_combobox_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxTextRGB[2], *p_transition_combobox_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = DDLComboboxAnimationState::Default;

            is_switch_success = true;
            break;
        }
        case DDLComboboxAnimationState::Active:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.ddlComboboxActive.getRed(), 0.5, 0.5, &*p_transition_combobox_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.ddlComboboxActive.getGreen(), 0.5, 0.5, &*p_transition_combobox_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.ddlComboboxActive.getBlue(), 0.5, 0.5, &*p_transition_combobox_rgb[2]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getRed(), 0.5, 0.5, &*p_transition_combobox_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getGreen(), 0.5, 0.5, &*p_transition_combobox_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->activeAnimationDuration, this->objects()->colors.textHighlight.getBlue(), 0.5, 0.5, &*p_transition_combobox_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxRGB[0], *p_transition_combobox_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxRGB[1], *p_transition_combobox_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxRGB[2], *p_transition_combobox_rgb[2]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxTextRGB[0], *p_transition_combobox_text_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxTextRGB[1], *p_transition_combobox_text_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableComboboxTextRGB[2], *p_transition_combobox_text_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = DDLComboboxAnimationState::Active;

            is_switch_success = true;
            break;
        }
        case DDLComboboxAnimationState::Focus:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 1.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        case DDLComboboxAnimationState::FocusLost:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->focusAnimationDuration, 0.0, 0.5, 0.5, &*p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableFocusBorderOpacity, *p_transition_focus_border_opacity);
            if (FAILED(hr))
                break;

            is_switch_success = true;
            break;
        }
        }
        if (!is_switch_success)
        {
            error_message = "Failed to create and add the animation transitions to the storyboard.";
            break;
        }

        // Get the current time.
        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        // Set the storyboard tag.
        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->comboboxWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        // Schedule the storyboard.
        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        // Set the animation invalidate timer.
        SetTimer(this->comboboxWindow, MyDDLComboboxSubclass::IDT_ANIMATION_INVALIDATE, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'startAnimation()']", MyLogType::Error);

    return are_all_operation_success;
}
LRESULT CALLBACK MyDDLComboboxSubclass::subclassProcedureCombobox(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyDDLComboboxSubclass *p_this = reinterpret_cast<MyDDLComboboxSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override paint messages.
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;

            // Get the latest animation values.
            DOUBLE focus_border_opacity = 0.0;
            INT32 combobox_rgb[3] = {0, 0, 0};
            INT32 combobox_border_rgb[3] = {0, 0, 0};
            INT32 combobox_text_rgb[3] = {0, 0, 0};
            hr = (*p_this->pAnimationVariableFocusBorderOpacity)->GetValue(&focus_border_opacity);
            if (FAILED(hr) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableComboboxRGB, combobox_rgb) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableComboboxBorderRGB, combobox_border_rgb) ||
                !p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableComboboxTextRGB, combobox_text_rgb))
            {
                error_message = "[WM_PAINT] Failed to retrieve the animation values.";
                break;
            }

            // Get the window client rects.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                break;
            }

            // Get the window text length.
            SetLastError(ERROR_SUCCESS);
            size_t window_text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
            if (!window_text_length && GetLastError())
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text length.";
                break;
            }

            // Get the window text.
            std::unique_ptr<WCHAR[]> window_text(new WCHAR[window_text_length + 1]);
            if (!GetWindowTextW(hWnd, window_text.get(), static_cast<INT>(window_text_length) + 1) && window_text_length)
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text.";
                break;
            }

            // Bind the render target to the window device context.
            hr = (*p_this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
            if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                break;
            }

            // Prepare drawing resources.
            D2D1_RECT_F d2d1_rect_window = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window.right), static_cast<FLOAT>(rect_window.bottom));
            D2D1::ColorF d2d1_color_focus_border = p_this->objects()->colors.focus.getD2D1Color();
            d2d1_color_focus_border.a = static_cast<FLOAT>(focus_border_opacity);
            D2D1::ColorF d2d1_color_combobox = D2D1::ColorF(combobox_rgb[0] / 255.0f, combobox_rgb[1] / 255.0f, combobox_rgb[2] / 255.0f, 1.0f);
            D2D1::ColorF d2d1_color_combobox_border = D2D1::ColorF(combobox_border_rgb[0] / 255.0f, combobox_border_rgb[1] / 255.0f, combobox_border_rgb[2] / 255.0f, 1.0f);
            D2D1::ColorF d2d1_color_combobox_text(combobox_text_rgb[0] / 255.0f, combobox_text_rgb[1] / 255.0f, combobox_text_rgb[2] / 255.0f, 1.0);
            auto &p_d2d1_bitmap_arrow = *MyDDLComboboxSubclass::pSharedD2D1BitmapArrow;
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_focus_border(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_combobox(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_combobox_border(new ID2D1SolidColorBrush *(nullptr));
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_combobox_text(new ID2D1SolidColorBrush *(nullptr));
            {
                error_message = "[WM_PAINT] Failed to create the resources.";
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_focus_border, &*p_d2d1_solidcolorbrush_focus_border);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_combobox, &*p_d2d1_solidcolorbrush_combobox);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_combobox_border, &*p_d2d1_solidcolorbrush_combobox_border);
                if (FAILED(hr))
                    break;
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_combobox_text, &*p_d2d1_solidcolorbrush_combobox_text);
                if (FAILED(hr))
                    break;
                error_message = "";
            }

            // Begin drawing.
            (*p_this->pD2D1DCRenderTarget)->BeginDraw();
            (*p_this->pD2D1DCRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            (*p_this->pD2D1DCRenderTarget)->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            {
                // Draw the background.
                (*p_this->pD2D1DCRenderTarget)->Clear((p_this->config.pBackground ? p_this->config.pBackground->getD2D1Color() : p_this->objects()->colors.background.getD2D1Color()));

                // Draw the combobox.
                p_this->graphics()->d2d1Engine().drawFillRoundRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window, 5, 5, *p_d2d1_solidcolorbrush_combobox, *p_d2d1_solidcolorbrush_combobox_border, 1.0);

                // Draw the combobox text and the drop down arrow.
                FLOAT padding = 10.0f,
                      image_size = (*p_this->pTextFormat)->GetFontSize();
                p_this->graphics()->d2d1Engine().drawText((*p_this->pD2D1DCRenderTarget), *p_this->pTextFormat, d2d1_rect_window, window_text.get(), *p_d2d1_solidcolorbrush_combobox_text, padding, 0, 2);
                p_this->graphics()->d2d1Engine().drawImage((*p_this->pD2D1DCRenderTarget), p_d2d1_bitmap_arrow, d2d1_rect_window,
                                                           1.0, d2d1_rect_window.right - d2d1_rect_window.left - image_size - padding,
                                                           ((d2d1_rect_window.bottom - d2d1_rect_window.top) / 2) - (image_size / 2),
                                                           image_size, image_size, false);

                // Draw the focus border.
                if (focus_border_opacity)
                    p_this->graphics()->d2d1Engine().drawRoundRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_window, 5, 5, *p_d2d1_solidcolorbrush_focus_border);
            }

            // End drawing.
            hr = (*p_this->pD2D1DCRenderTarget)->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!p_this->createDeviceResources(true))
                {
                    error_message = "[WM_PAINT] Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "[WM_PAINT] Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue; // Repeat the paint operation.
            }
            else if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'subclassProcedureCombobox()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        break;
    }

    // Suppress unnecessary non-client paints.
    case WM_NCPAINT:
        return 0;

    // Process the animation invalidate timer.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyDDLComboboxSubclass::IDT_ANIMATION_INVALIDATE:
        {
            HRESULT hr;
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

                IUIAnimationStoryboard *p_storyboard;
                hr = p_this->graphics()->wamEngine().manager()->GetStoryboardFromTag(NULL, GetDlgCtrlID(hWnd), &p_storyboard);
                if (SUCCEEDED(hr))
                {
                    if (!p_storyboard)
                    {
                        if (!KillTimer(hWnd, MyDDLComboboxSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }

                        are_all_operation_success = true;
                        break;
                    }

                    UI_ANIMATION_STORYBOARD_STATUS storyboard_status;
                    hr = p_storyboard->GetStatus(&storyboard_status);
                    if (FAILED(hr))
                    {
                        error_message = "[WM_TIMER] Failed to get the storyboard status.";
                        break;
                    }

                    if (storyboard_status == UI_ANIMATION_STORYBOARD_READY)
                    {
                        if (!KillTimer(hWnd, MyDDLComboboxSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer if an error occurs.
                if (!KillTimer(hWnd, MyDDLComboboxSubclass::IDT_ANIMATION_INVALIDATE))
                    g_pApp->logger.writeLog("[WM_TIMER] Failed to kill the timer.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

                g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            g_pApp->logger.writeLog("[WM_TIMER] Unprocessed timer message.", "[CLASS: 'MyImageButtonSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process the key-up messages to trigger appropriate actions.
    case WM_KEYUP:
    {
        if (wParam == VK_RETURN)
        {
            auto is_dropdownlist_window_open = SendMessageW(hWnd, CB_GETDROPPEDSTATE, 0, 0);

            if (is_dropdownlist_window_open)
            {
                SendMessageW(hWnd, CB_SHOWDROPDOWN, FALSE, 0);
            }
            else
            {
                SendMessageW(hWnd, CB_SHOWDROPDOWN, TRUE, 0);
                RedrawWindow(p_this->ddlWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }

            return 0;
        }

        break;
    }

    // Process the key-down messages to trigger appropriate actions.
    case WM_KEYDOWN:
    {
        // Ignore the ENTER key.
        if (wParam == VK_RETURN)
            return 0;

        // Keep the drop-down list window invalidated.
        InvalidateRect(p_this->ddlWindow, NULL, FALSE);
        break;
    }

    // Open the drop-down list window on mouse click.
    case WM_LBUTTONDOWN:
    {
        // Set focus to the combobox window.
        SetFocus(hWnd);

        // Open the drop-down list.
        SendMessageW(hWnd, CB_SHOWDROPDOWN, TRUE, 0);

        // Perform an extra invalidate operation, otherwise the combobox item won't be rendered.
        RedrawWindow(p_this->ddlWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        break;
    }

    // Update the appearance of the combobox window when it gains/loses focus.
    case WM_SETFOCUS:
    {
        p_this->isActive = true;
        if (!p_this->startAnimation(DDLComboboxAnimationState::Active) ||
            !p_this->startAnimation(DDLComboboxAnimationState::Focus))
            g_pApp->logger.writeLog("[WM_SETFOCUS] Failed to start the animation.", "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'subclassProcedureCombobox()']", MyLogType::Error);

        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    }
    case WM_KILLFOCUS:
    {
        p_this->isActive = false;
        if (!p_this->startAnimation(DDLComboboxAnimationState::Default) ||
            !p_this->startAnimation(DDLComboboxAnimationState::FocusLost))
            g_pApp->logger.writeLog("[WM_KILLFOCUS] Failed to start the animation.", "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'subclassProcedureCombobox()']", MyLogType::Error);

        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyDDLComboboxSubclass::subclassProcedureCombobox, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'subclassProcedureCombobox()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK MyDDLComboboxSubclass::subclassProcedureComboboxDDL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyDDLComboboxSubclass *p_this = reinterpret_cast<MyDDLComboboxSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override non-client paint messages.
    case WM_NCPAINT:
    {
        HDC hdc = NULL;
        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            // Get the combobox drop down list's device context.
            hdc = GetWindowDC(hWnd);
            if (!hdc)
            {
                error_message = "Failed to retrieve the combobox drop down list's device context.";
                break;
            }

            // Get the combobox drop down list's client rect.
            RECT rect_dropdownlist;
            if (!GetClientRect(hWnd, &rect_dropdownlist))
            {
                error_message = "Failed to retrieve the combobox drop down list's client rect.";
                break;
            }

            // Get the total items of the combobox to get the ID of the last item.
            INT total_combobox_items = static_cast<INT>(SendMessageW(p_this->comboboxWindow, CB_GETCOUNT, 0, 0));
            if (total_combobox_items == CB_ERR)
            {
                error_message = "Failed to retrieve the total combobox items.";
                break;
            }

            // If the combobox don't have any items, don't draw anything.
            if (total_combobox_items == 0)
            {
                are_all_operation_success = true;
                break;
            }

            // Get current selectet combobox item index.
            INT current_selected_combobox_item = static_cast<INT>(SendMessageW(p_this->comboboxWindow, CB_GETCURSEL, 0, 0));
            if (current_selected_combobox_item == CB_ERR)
            {
                error_message = "Failed to retrieve the current selected combobox item index.";
                break;
            }

            // Fill the 2-pixels gap to match the color of the last item background color.
            RECT rect_dropdownlist_2 = rect_dropdownlist;
            rect_dropdownlist_2.top = --rect_dropdownlist_2.bottom - 1;
            FillRect(hdc, &rect_dropdownlist_2,
                     (current_selected_combobox_item == (total_combobox_items - 1)
                          ? p_this->objects()->colors.ddlComboboxSelectedItemBackground.getSolidBrush()
                          : p_this->objects()->colors.ddlComboboxItemBackground.getSolidBrush()));

            // Draw the drop-down list's borders.
            FrameRect(hdc, &rect_dropdownlist, p_this->objects()->colors.ddlComboboxDropdownlistBorder.getSolidBrush());

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyDDLComboboxSubclass' | MESSAGE: 'WM_NCPAINT' | CALLBACK: 'subclassProcedureComboboxDDL()']", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (hdc)
        {
            if (!ReleaseDC(hWnd, hdc))
            {
                g_pApp->logger.writeLog("Failed to release the combobox drop down list's device context.", "[CLASS: 'MyDDLComboboxSubclass' | MESSAGE: 'WM_NCPAINT' | CALLBACK: 'subclassProcedureComboboxDDL()']", MyLogType::Error);
            }
        }

        return 0;
    }

    // Process WM_NCCALCSIZE messages to suppress any default painting trigger by this message. (Kill the white background flickering)
    // This affects the default non-client area calculation.
    case WM_NCCALCSIZE:
        return 0;

    // Suppress WM_PRINT & WM_PRINTCLIENT messages.
    // This removes the drop-down list opening animation.
    case WM_PRINT:
    case WM_PRINTCLIENT:
        return 0;

    // Process the left button click-down messages to set the timer that will set the focus to the combobox parent.
    // An timer is needed to add a delay to the focus change, otherwise the focus won't be set properly.
    case WM_LBUTTONDOWN:
    {
        SetTimer(hWnd, 100, 16, NULL);
        break;
    }
    case WM_TIMER:
    {
        if (wParam == 100)
        {
            KillTimer(hWnd, 100);
            SetFocus(GetParent(p_this->comboboxWindow));
        }

        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyDDLComboboxSubclass::subclassProcedureComboboxDDL, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyDDLComboboxSubclass' | FUNC: 'subclassProcedureComboboxDDL()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// [MyVerticalScrollbarSubclass] class implementations:

MyVerticalScrollbarSubclass ::~MyVerticalScrollbarSubclass()
{
    if (!DestroyWindow(this->staticWindow))
        g_pApp->logger.writeLog("Failed to destroy the associated static window.", "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'Destructor']", MyLogType::Error);
}
bool MyVerticalScrollbarSubclass::scrollWindowByAmount(INT scrollAmount)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Get the scroll informations.
        SCROLLINFO scroll_info;
        int current_scroll_pos = 0,
            min_scroll_pos = 0,
            max_scroll_pos = 0,
            max_upward_scroll_amount = 0,
            max_downward_scroll_amount = 0;
        if (!this->getScrollInfo(scroll_info, &current_scroll_pos, &min_scroll_pos, &max_scroll_pos, &max_upward_scroll_amount, &max_downward_scroll_amount))
        {
            error_message = "Failed to get the scroll information.";
            break;
        }

        // Adjust the scroll amount if it exceeds the boundaries.
        if (current_scroll_pos + scrollAmount > max_scroll_pos)
            scrollAmount = max_downward_scroll_amount;
        else if (current_scroll_pos + scrollAmount < min_scroll_pos)
            scrollAmount = max_upward_scroll_amount;

        if (!pContainer->scrollContainer(scrollAmount))
        {
            error_message = "Failed to scroll the container.";
            break;
        }

        // Update the scroll information.
        scroll_info.nPos += scrollAmount;
        SendMessageW(this->scrollbarWindow, SBM_SETSCROLLINFO, TRUE, reinterpret_cast<LPARAM>(&scroll_info));
        RedrawWindow(this->staticWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'scrollWindowByAmount()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyVerticalScrollbarSubclass::scrollWindowByPos(INT scrollPos)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Get the scroll informations.
        SCROLLINFO scroll_info;
        int current_scroll_pos = 0,
            min_scroll_pos = 0,
            max_scroll_pos = 0,
            scroll_amount = 0;
        if (!this->getScrollInfo(scroll_info, &current_scroll_pos, &min_scroll_pos, &max_scroll_pos))
        {
            error_message = "Failed to get the scroll information.";
            break;
        }

        // Adjust the scroll position if it exceeds the boundaries.
        if (scrollPos < min_scroll_pos)
            scrollPos = min_scroll_pos;
        else if (scrollPos > max_scroll_pos)
            scrollPos = max_scroll_pos;

        if (scrollPos == current_scroll_pos) // No scrolling needed.
        {
            are_all_operation_success = true;
            break;
        }
        else if (scrollPos > current_scroll_pos) // Scrolling downward.
        {
            scroll_amount = scrollPos - current_scroll_pos;
            scroll_info.nPos += scroll_amount;
            if (!pContainer->scrollContainer(scroll_amount))
            {
                error_message = "Failed to scroll the container.";
                break;
            }
        }
        else if (scrollPos < current_scroll_pos) // Scrolling upward.
        {
            scroll_amount = current_scroll_pos - scrollPos;
            scroll_info.nPos += -scroll_amount;
            if (!pContainer->scrollContainer(-scroll_amount))
            {
                error_message = "Failed to scroll the container.";
                break;
            }
        }

        // Update the scroll information.
        SendMessageW(this->scrollbarWindow, SBM_SETSCROLLINFO, TRUE, reinterpret_cast<LPARAM>(&scroll_info));
        RedrawWindow(this->staticWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'scrollWindowByPos()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyVerticalScrollbarSubclass::scrollWindowByAmountSmooth(INT scrollAmount)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Get the scroll informations.
        SCROLLINFO scroll_info;
        int current_scroll_pos = 0,
            min_scroll_pos = 0,
            max_scroll_pos = 0,
            max_upward_scroll_amount = 0,
            max_downward_scroll_amount = 0;
        if (!this->getScrollInfo(scroll_info, &current_scroll_pos, &min_scroll_pos, &max_scroll_pos, &max_upward_scroll_amount, &max_downward_scroll_amount))
        {
            error_message = "Failed to get the scroll information.";
            break;
        }

        // Adjust the scroll amount if it exceeds the boundaries.
        if (current_scroll_pos + scrollAmount > max_scroll_pos)
            scrollAmount = max_downward_scroll_amount;
        else if (current_scroll_pos + scrollAmount < min_scroll_pos)
            scrollAmount = max_upward_scroll_amount;

        bool scroll_direction_changed = false;
        if (this->scrollInProgress) // If scrolling is in progress:
        {
            // Check if the scroll direction has changed.
            scroll_direction_changed = ((this->lastScrollPos > current_scroll_pos && scrollAmount < 0) || (this->lastScrollPos < current_scroll_pos && scrollAmount > 0) ? true : false);
            if (scroll_direction_changed)
                this->lastScrollPos = current_scroll_pos; // Scroll direction changed, stop the scrolling.
            else
                this->lastScrollPos += scrollAmount; // The scroll direction has not changed, stack the scroll amount.
        }
        else // No scrolling is in progress.
        {
            this->lastScrollPos = current_scroll_pos + scrollAmount;
            this->scrollInProgress = true;
        }

        // Calculate the scroll distance:
        int scroll_distance = this->lastScrollPos - current_scroll_pos; // Calculate the scroll distance.
        if (scroll_distance < 0)                                        // In the case of scrolling upward, the calculated scroll distance is a negative value, converting to a positive value.
            scroll_distance = -scroll_distance;
        else if (scroll_distance == 0) // Make sure the scroll distance value is non-zero to avoid the dividing by zero exception.
            scroll_distance = 1;

        // Calculate the scrolling speed based on the scroll distance:
        int base_scroll_speed = 800,                                                                                // The base scroll speed.
            max_scroll_speed = 4000,                                                                                // Maximum scroll speed.
            base_distance_interval = (scroll_direction_changed ? 10 : static_cast<int>(this->SMOOTH_SCROLL_SPEED)), // This base interval will determine the scrolling speed. If the scroll direction has changed, accelerate the scroll speed by lowering the interval.
            distance_interval = static_cast<int>(std::sqrt(base_distance_interval * scroll_distance)),
            scroll_speed = base_scroll_speed + (scroll_distance / distance_interval) * 400;
        if (scroll_speed > max_scroll_speed)
            scroll_speed = max_scroll_speed;

        // Adjust the scroll position if it exceeds the boundaries.
        if (this->lastScrollPos > max_scroll_pos)
            this->lastScrollPos = max_scroll_pos;
        else if (this->lastScrollPos < min_scroll_pos)
            this->lastScrollPos = min_scroll_pos;

        std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter>
        p_storyboard(new IUIAnimationStoryboard *(nullptr));
        std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_last_scroll_pos(new IUIAnimationTransition *(nullptr));
        std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_target_scroll_pos(new IUIAnimationTransition *(nullptr));

        HRESULT hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        error_message = "Failed to create the animation transitions.";
        hr = this->graphics()->wamEngine().transitionLibrary()->CreateInstantaneousTransition(current_scroll_pos, &*p_transition_last_scroll_pos);
        if (FAILED(hr))
            break;
        hr = this->graphics()->wamEngine().transitionLibrary()->CreateLinearTransitionFromSpeed(scroll_speed, this->lastScrollPos, &*p_transition_target_scroll_pos);
        if (FAILED(hr))
            break;
        error_message = "";

        error_message = "Failed to add the transitions to the storyboard.";
        hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableLastScrollPos, *p_transition_last_scroll_pos);
        if (FAILED(hr))
            break;
        UI_ANIMATION_KEYFRAME key_frame;
        hr = (*p_storyboard)->AddKeyframeAfterTransition(*p_transition_last_scroll_pos, &key_frame);
        if (FAILED(hr))
            break;
        hr = (*p_storyboard)->AddTransitionAtKeyframe(*this->pAnimationVariableLastScrollPos, *p_transition_target_scroll_pos, key_frame);
        if (FAILED(hr))
            break;
        error_message = "";

        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->scrollbarWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        this->scrollInProgress = true;
        SetTimer(this->scrollbarWindow, IDT_ANIMATION_SCROLLBAR, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'scrollAnimation2()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyVerticalScrollbarSubclass::scrollWindowByPosSmooth(INT scrollPos, FLOAT milliseconds)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Get the scroll informations.
        int current_scroll_pos = 0;
        SCROLLINFO scroll_info;
        if (!this->getScrollInfo(scroll_info, &current_scroll_pos))
        {
            error_message = "Failed to get the scroll information.";
            break;
        }

        if (scrollPos == scroll_info.nPos) // No scrolling needed.
        {
            are_all_operation_success = true;
            break;
        }

        int last_scroll_pos = 0;
        HRESULT hr = (*this->pAnimationVariableLastScrollPos)->GetIntegerValue(&last_scroll_pos);
        if (FAILED(hr))
        {
            error_message = "Failed to get the animation variable value.";
            break;
        }

        std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter> p_storyboard(new IUIAnimationStoryboard *(nullptr));
        std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_last_scroll_pos(new IUIAnimationTransition *(nullptr));
        std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter> p_transition_target_scroll_pos(new IUIAnimationTransition *(nullptr));

        hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        error_message = "Failed to create the animation transitions.";
        hr = this->graphics()->wamEngine().transitionLibrary()->CreateInstantaneousTransition(current_scroll_pos, &*p_transition_last_scroll_pos);
        if (FAILED(hr))
            break;
        hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(milliseconds, scrollPos, 0.5, 0.5, &*p_transition_target_scroll_pos);
        if (FAILED(hr))
            break;
        error_message = "";

        error_message = "Failed to add the transitions to the storyboard.";
        hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableLastScrollPos, *p_transition_last_scroll_pos);
        if (FAILED(hr))
            break;
        UI_ANIMATION_KEYFRAME key_frame;
        hr = (*p_storyboard)->AddKeyframeAfterTransition(*p_transition_last_scroll_pos, &key_frame);
        if (FAILED(hr))
            break;
        hr = (*p_storyboard)->AddTransitionAtKeyframe(*this->pAnimationVariableLastScrollPos, *p_transition_target_scroll_pos, key_frame);
        if (FAILED(hr))
            break;
        error_message = "";

        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->scrollbarWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        SetTimer(this->scrollbarWindow, this->IDT_ANIMATION_SCROLLBAR, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'scrollWindowByPosSmooth()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyVerticalScrollbarSubclass::getScrollInfo(SCROLLINFO &scrollInfo, int *pCurrentPos, int *pMinScrollPos, int *pMaxScrollPos, int *pMaxUpwardScrollAmount, int *pMaxDownwardScrollAmount)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        scrollInfo.cbSize = sizeof(SCROLLINFO);
        scrollInfo.fMask = SIF_ALL;

        if (!SendMessageW(this->scrollbarWindow, SBM_GETSCROLLINFO, 0, reinterpret_cast<LPARAM>(&scrollInfo)))
        {
            error_message = "Failed to get the scroll information.";
            break;
        }

        if (pCurrentPos)
            *pCurrentPos = scrollInfo.nPos;
        if (pMinScrollPos)
            *pMinScrollPos = scrollInfo.nMin;
        if (pMaxScrollPos)
            *pMaxScrollPos = scrollInfo.nMax - static_cast<int>(scrollInfo.nPage);
        if (pMaxUpwardScrollAmount)
            *pMaxUpwardScrollAmount = -(scrollInfo.nPos - scrollInfo.nMin);
        if (pMaxDownwardScrollAmount)
            *pMaxDownwardScrollAmount = (scrollInfo.nMax - static_cast<int>(scrollInfo.nPage)) - scrollInfo.nPos;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'getScrollInfo()']", MyLogType::Error);

    return are_all_operation_success;
}
HWND MyVerticalScrollbarSubclass::getStaticHandle()
{
    return this->staticWindow;
}
HWND &MyVerticalScrollbarSubclass::getStaticHandleRef()
{
    return this->staticWindow;
}
bool MyVerticalScrollbarSubclass::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass object is not associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is not associated with a window.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        // Start animation to the current animation state to update animation variables values.
        if (!this->startAnimation(this->currentAnimationState))
        {
            error_message = "Failed to start the animation.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
void MyVerticalScrollbarSubclass::releaseSharedDeviceResources()
{
    // <This subclass class doesn't have any shared device resources>
}
void MyVerticalScrollbarSubclass::setAnimationDuration(ScrollbarAnimationState animateState, FLOAT milliseconds)
{
    switch (animateState)
    {
    case ScrollbarAnimationState::Default:
        this->defaultAnimationDuration = milliseconds;
        break;
    case ScrollbarAnimationState::Hover:
        this->hoverAnimationDuration = milliseconds;
        break;
    case ScrollbarAnimationState::Dragging:
        this->draggingAnimationDuration = milliseconds;
        break;
    }
}
bool MyVerticalScrollbarSubclass::setWindow(HWND hWnd, MyContainer *pContainer)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass base class is initialized.
        if (!this->isInitialized)
        {
            error_message = "The subclass base class is not initialized.";
            break;
        }

        // Check if the subclass object is already associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is already associated with a window.";
            break;
        }

        // Subclass the window.
        // Store this subclass object pointer as reference data, enabling the subclass procedure to access the non-static object members.
        this->scrollbarWindow = hWnd;
        this->pContainer = pContainer;
        if (!SetWindowSubclass(this->scrollbarWindow, &MyVerticalScrollbarSubclass::subclassProcedureScrollbar, 0, reinterpret_cast<DWORD_PTR>(this)) ||
            !SetWindowSubclass(this->staticWindow, &MyVerticalScrollbarSubclass::subclassProcedureStatic, 0, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = "Failed to install the window subclass callback.";
            break;
        }

        // Create the animation variables.
        {
            pAnimationVariableScrollbarThumbRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            pAnimationVariableScrollbarThumbRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            pAnimationVariableScrollbarThumbRGB.emplace_back(new IUIAnimationVariable *(nullptr));
            pAnimationVariableLastScrollPos.reset(new IUIAnimationVariable *(nullptr));
            error_message = "Failed to create the animation variables.";
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableScrollbarThumbRGB[0],
                                                                       this->objects()->colors.scrollbarThumbDefault.getRed(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableScrollbarThumbRGB[1],
                                                                       this->objects()->colors.scrollbarThumbDefault.getGreen(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableScrollbarThumbRGB[2],
                                                                       this->objects()->colors.scrollbarThumbDefault.getBlue(),
                                                                       0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            if (!this->graphics()->wamEngine().createAnimationVariable(*this->pAnimationVariableLastScrollPos,
                                                                       0,
                                                                       -100000.0, 100000.0, UI_ANIMATION_ROUNDING_FLOOR))
                break;
            error_message = "";
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'setWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
MyVerticalScrollbarSubclass *MyVerticalScrollbarSubclass::getSubclassPointer(HWND hWnd)
{
    DWORD_PTR reference_data;
    bool is_subclassed;

    is_subclassed = GetWindowSubclass(hWnd, &MyVerticalScrollbarSubclass::subclassProcedureScrollbar, 0, &reference_data);
    if (is_subclassed)
        return reinterpret_cast<MyVerticalScrollbarSubclass *>(reference_data);

    is_subclassed = GetWindowSubclass(hWnd, &MyVerticalScrollbarSubclass::subclassProcedureStatic, 0, &reference_data);
    if (is_subclassed)
        return reinterpret_cast<MyVerticalScrollbarSubclass *>(reference_data);

    return nullptr;
}
bool MyVerticalScrollbarSubclass::createSharedDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // HRESULT hr;

        error_message = "Failed to create the shared device resources.";
        // <This subclass class doesn't have any shared device resources>
        error_message = "";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'createSharedDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyVerticalScrollbarSubclass::createDeviceResources(bool recreateSharedResources)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        HDC hdc = GetDC(this->staticWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->staticWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pD2D1DCRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->graphics()->d2d1Engine().createDCRenderTarget(*this->pD2D1DCRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        hr = (*this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        if (recreateSharedResources)
            MyVerticalScrollbarSubclass::releaseSharedDeviceResources();

        if (!this->createSharedDeviceResources())
        {
            error_message = "Failed to create the shared device resources.";
            break;
        }

        // Create the device resources here ...
        // <This subclass class doesn't have any non-shared device resources>

        if (!ReleaseDC(this->staticWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyVerticalScrollbarSubclass::startAnimation(ScrollbarAnimationState animationState)
{
    HRESULT hr;
    std::unique_ptr<IUIAnimationStoryboard *, IUIAnimationStoryboardDeleter> p_storyboard(new IUIAnimationStoryboard *(nullptr));
    std::vector<std::unique_ptr<IUIAnimationTransition *, IUIAnimationTransitionDeleter>> p_transition_scrollbar_thumb_rgb;
    p_transition_scrollbar_thumb_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_scrollbar_thumb_rgb.emplace_back(new IUIAnimationTransition *(nullptr));
    p_transition_scrollbar_thumb_rgb.emplace_back(new IUIAnimationTransition *(nullptr));

    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Create the storyboard.
        hr = this->graphics()->wamEngine().manager()->CreateStoryboard(&*p_storyboard);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation storyboard.";
            break;
        }

        // Create and add the animation transitions to the storyboard.
        bool is_switch_success = false;
        switch (animationState)
        {
        case ScrollbarAnimationState::Default:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.scrollbarThumbDefault.getRed(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.scrollbarThumbDefault.getGreen(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->defaultAnimationDuration, this->objects()->colors.scrollbarThumbDefault.getBlue(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[0], *p_transition_scrollbar_thumb_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[1], *p_transition_scrollbar_thumb_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[2], *p_transition_scrollbar_thumb_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ScrollbarAnimationState::Default;

            is_switch_success = true;
            break;
        }
        case ScrollbarAnimationState::Hover:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.scrollbarThumbHover.getRed(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.scrollbarThumbHover.getGreen(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->hoverAnimationDuration, this->objects()->colors.scrollbarThumbHover.getBlue(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[0], *p_transition_scrollbar_thumb_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[1], *p_transition_scrollbar_thumb_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[2], *p_transition_scrollbar_thumb_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ScrollbarAnimationState::Hover;

            is_switch_success = true;
            break;
        }
        case ScrollbarAnimationState::Dragging:
        {
            // Create the animation transitions.
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->draggingAnimationDuration, this->objects()->colors.scrollbarThumbDragging.getRed(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[0]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->draggingAnimationDuration, this->objects()->colors.scrollbarThumbDragging.getGreen(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[1]);
            if (FAILED(hr))
                break;
            hr = this->graphics()->wamEngine().transitionLibrary()->CreateAccelerateDecelerateTransition(this->draggingAnimationDuration, this->objects()->colors.scrollbarThumbDragging.getBlue(), 0.5, 0.5, &*p_transition_scrollbar_thumb_rgb[2]);
            if (FAILED(hr))
                break;

            // Add the animation transitions to the storyboard.
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[0], *p_transition_scrollbar_thumb_rgb[0]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[1], *p_transition_scrollbar_thumb_rgb[1]);
            if (FAILED(hr))
                break;
            hr = (*p_storyboard)->AddTransition(*this->pAnimationVariableScrollbarThumbRGB[2], *p_transition_scrollbar_thumb_rgb[2]);
            if (FAILED(hr))
                break;

            // Update the window animation state.
            this->currentAnimationState = ScrollbarAnimationState::Dragging;

            is_switch_success = true;
            break;
        }
        }
        if (!is_switch_success)
        {
            error_message = "Failed to create and add the animation transitions to the storyboard.";
            break;
        }

        // Get the current time.
        UI_ANIMATION_SECONDS seconds_now;
        hr = this->graphics()->wamEngine().timer()->GetTime(&seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to retrieve the current time.";
            break;
        }

        // Set the storyboard tag.
        hr = (*p_storyboard)->SetTag(NULL, GetDlgCtrlID(this->staticWindow));
        if (FAILED(hr))
        {
            error_message = "Failed to set the storyboard tag.";
            break;
        }

        // Schedule the storyboard.
        hr = (*p_storyboard)->Schedule(seconds_now);
        if (FAILED(hr))
        {
            error_message = "Failed to schedule the storyboard.";
            break;
        }

        // Set the animation invalidate timer.
        SetTimer(this->staticWindow, MyVerticalScrollbarSubclass::IDT_ANIMATION_INVALIDATE, USER_TIMER_MINIMUM, (TIMERPROC)NULL);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'startAnimation()']", MyLogType::Error);

    return are_all_operation_success;
}
LRESULT CALLBACK MyVerticalScrollbarSubclass::subclassProcedureScrollbar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyVerticalScrollbarSubclass *p_this = reinterpret_cast<MyVerticalScrollbarSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Process the scroll animation timer.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case IDT_ANIMATION_SCROLLBAR:
        {
            HRESULT hr;
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                int current_scroll_pos = 0;
                hr = (*p_this->pAnimationVariableLastScrollPos)->GetIntegerValue(&current_scroll_pos);
                if (FAILED(hr))
                {
                    error_message = "Failed to get the animation variable value.";
                    break;
                }

                if (!p_this->scrollWindowByPos(current_scroll_pos))
                {
                    error_message = "Failed to scroll the window by pos.";
                    break;
                }

                IUIAnimationStoryboard *p_storyboard;
                hr = p_this->graphics()->wamEngine().manager()->GetStoryboardFromTag(NULL, GetDlgCtrlID(hWnd), &p_storyboard);
                if (SUCCEEDED(hr))
                {
                    if (!p_storyboard)
                    {
                        if (!KillTimer(hWnd, IDT_ANIMATION_SCROLLBAR))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                        p_this->scrollInProgress = false;
                        are_all_operation_success = true;
                        break;
                    }

                    UI_ANIMATION_STORYBOARD_STATUS storyboard_status;
                    hr = p_storyboard->GetStatus(&storyboard_status);
                    if (FAILED(hr))
                    {
                        error_message = "[WM_TIMER] Failed to get the storyboard status.";
                        break;
                    }

                    if (storyboard_status == UI_ANIMATION_STORYBOARD_READY)
                    {
                        if (!KillTimer(hWnd, IDT_ANIMATION_SCROLLBAR))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                        p_this->scrollInProgress = false;
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer if an error occurs.
                if (!KillTimer(hWnd, IDT_ANIMATION_SCROLLBAR))
                    g_pApp->logger.writeLog("[WM_TIMER] Failed to kill the timer.", "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureScrollbar()']", MyLogType::Error);
                p_this->scrollInProgress = false;
                g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureScrollbar()']", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            g_pApp->logger.writeLog("[WM_TIMER] Unprocessed timer message.", "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureScrollbar()']", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyVerticalScrollbarSubclass::subclassProcedureScrollbar, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureScrollbar()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK MyVerticalScrollbarSubclass::subclassProcedureStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyVerticalScrollbarSubclass *p_this = reinterpret_cast<MyVerticalScrollbarSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override paint messages.
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;

            // Get the latest animation values.
            INT32 scrollbar_thumb_rgb[3] = {0, 0, 0};
            error_message = "[WM_PAINT] Failed to retrieve the animation values.";
            if (!p_this->graphics()->wamEngine().getAnimationValueRGB(p_this->pAnimationVariableScrollbarThumbRGB, scrollbar_thumb_rgb))
                break;
            error_message = "";

            // Get the window client rects.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                break;
            }

            // Get the scroll informations.
            SCROLLINFO scroll_info;
            scroll_info.cbSize = sizeof(SCROLLINFO);
            scroll_info.fMask = SIF_ALL;
            if (!SendMessageW(p_this->scrollbarWindow, SBM_GETSCROLLINFO, 0, reinterpret_cast<LPARAM>(&scroll_info)))
            {
                error_message = "[WM_PAINT] Failed to retrieve the scroll information struct.";
                break;
            }

            // Calculate the thumb position and size.
            double visible_ratio = static_cast<double>(scroll_info.nPage) / static_cast<double>(scroll_info.nMax);
            int thumb_height = static_cast<int>(rect_window.bottom * visible_ratio);
            int max_thumb_position = rect_window.bottom - thumb_height;
            int thumb_top = scroll_info.nPos;
            if (scroll_info.nMax > static_cast<INT>(scroll_info.nPage))
            {
                thumb_top = static_cast<int>(static_cast<double>(scroll_info.nPos) / (scroll_info.nMax - scroll_info.nPage) * max_thumb_position);
            }
            RECT rect_thumb = rect_window;
            rect_thumb.top += thumb_top;
            rect_thumb.bottom = rect_thumb.top + thumb_height;

            // Bind the render target to the window device context.
            hr = (*p_this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
            if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                break;
            }

            // Prepare drawing resources.
            D2D1_RECT_F d2d1_rect_thumb = D2D1::RectF(static_cast<FLOAT>(rect_thumb.left),
                                                      static_cast<FLOAT>(rect_thumb.top),
                                                      static_cast<FLOAT>(rect_thumb.right),
                                                      static_cast<FLOAT>(rect_thumb.bottom));
            D2D1::ColorF d2d1_color_scrollbar_thumb = D2D1::ColorF(scrollbar_thumb_rgb[0] / 255.0f, scrollbar_thumb_rgb[1] / 255.0f, scrollbar_thumb_rgb[2] / 255.0f, 1.0);
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_scrollbar_thumb(new ID2D1SolidColorBrush *(nullptr));
            {
                error_message = "[WM_PAINT] Failed to create the resources.";
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_scrollbar_thumb, &*p_d2d1_solidcolorbrush_scrollbar_thumb);
                if (FAILED(hr))
                    break;
                error_message = "";
            }

            // Begin drawing.
            (*p_this->pD2D1DCRenderTarget)->BeginDraw();
            (*p_this->pD2D1DCRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

            {
                // Draw the background.
                (*p_this->pD2D1DCRenderTarget)->Clear(p_this->objects()->colors.scrollbarBackground.getD2D1Color());

                // Draw the scrollbar thumb.
                p_this->graphics()->d2d1Engine().drawFillRectangle((*p_this->pD2D1DCRenderTarget), d2d1_rect_thumb, *p_d2d1_solidcolorbrush_scrollbar_thumb);
            }

            // End drawing.
            hr = (*p_this->pD2D1DCRenderTarget)->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!p_this->createDeviceResources(true))
                {
                    error_message = "[WM_PAINT] Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "[WM_PAINT] Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue; // Repeat the paint operation.
            }
            else if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureStatic()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        return 0;
    }

    // Process the animation invalidate timer.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyVerticalScrollbarSubclass::IDT_ANIMATION_INVALIDATE:
        {
            HRESULT hr;
            bool are_all_operation_success = false;
            std::string error_message = "";
            while (!are_all_operation_success)
            {
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);

                IUIAnimationStoryboard *p_storyboard;
                hr = p_this->graphics()->wamEngine().manager()->GetStoryboardFromTag(NULL, GetDlgCtrlID(hWnd), &p_storyboard);
                if (SUCCEEDED(hr))
                {
                    if (!p_storyboard)
                    {
                        if (!KillTimer(hWnd, MyVerticalScrollbarSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }

                        are_all_operation_success = true;
                        break;
                    }

                    UI_ANIMATION_STORYBOARD_STATUS storyboard_status;
                    hr = p_storyboard->GetStatus(&storyboard_status);
                    if (FAILED(hr))
                    {
                        error_message = "[WM_TIMER] Failed to get the storyboard status.";
                        break;
                    }

                    if (storyboard_status == UI_ANIMATION_STORYBOARD_READY)
                    {
                        if (!KillTimer(hWnd, MyVerticalScrollbarSubclass::IDT_ANIMATION_INVALIDATE))
                        {
                            error_message = "[WM_TIMER] Failed to kill the timer.";
                            break;
                        }
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer if an error occurs.
                if (!KillTimer(hWnd, MyVerticalScrollbarSubclass::IDT_ANIMATION_INVALIDATE))
                    g_pApp->logger.writeLog("[WM_TIMER] Failed to kill the timer.", "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureStatic()']", MyLogType::Error);

                g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureStatic()']", MyLogType::Error);
            }

            return 0;
        }

        default:
        {
            g_pApp->logger.writeLog("[WM_TIMER] Unprocessed timer message.", "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureStatic()']", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process the left-button mouse click (down) to handle thumb event.
    case WM_LBUTTONDBLCLK: // Treat double click as single click.
    case WM_LBUTTONDOWN:
    {
        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            // Get cursor position.
            POINT cursor_position;
            if (!GetCursorPos(&cursor_position))
            {
                error_message = "Failed to retrieve the cursor position.";
                break;
            }
            if (!ScreenToClient(hWnd, &cursor_position))
            {
                error_message = "Failed to convert the cursor position from screen to client.";
                break;
            }

            // Get the scroll informations.
            SCROLLINFO scroll_info;
            scroll_info.cbSize = sizeof(SCROLLINFO);
            scroll_info.fMask = SIF_ALL;
            if (!SendMessageW(p_this->scrollbarWindow, SBM_GETSCROLLINFO, 0, reinterpret_cast<LPARAM>(&scroll_info)))
            {
                error_message = "Failed to retrieve the scroll information.";
                break;
            }

            // Get the window's client rect.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "Failed to retrieve the window client rect.";
                break;
            }

            // Calculate the thumb position and size.
            double visible_ratio = static_cast<double>(scroll_info.nPage) / static_cast<double>(scroll_info.nMax);
            int thumb_height = static_cast<int>(rect_window.bottom * visible_ratio);
            int max_thumb_position = rect_window.bottom - thumb_height;
            int thumb_top = scroll_info.nPos;
            if (scroll_info.nMax > static_cast<INT>(scroll_info.nPage))
            {
                thumb_top = static_cast<int>(static_cast<double>(scroll_info.nPos) / (scroll_info.nMax - scroll_info.nPage) * max_thumb_position);
            }
            RECT rect_thumb = rect_window;
            rect_thumb.top += thumb_top;
            rect_thumb.bottom = rect_thumb.top + thumb_height;

            // The thumb is clicked, begin thumb dragging event.
            if (PtInRect(&rect_thumb, cursor_position))
            {
                p_this->isThumbDragging = true;

                // Store the initial values of this thumb dragging event.
                p_this->initialThumbPos = thumb_top;
                p_this->initialClickPos = cursor_position.y;

                // Set the mouse capture to this window, allow this window to receive mouse messages
                // even when the cursor is outside of this window.
                SetCapture(hWnd);

                // Perform a redraw to update the thumb's appearance.
                // RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
                if (p_this->currentAnimationState != ScrollbarAnimationState::Dragging)
                    p_this->startAnimation(ScrollbarAnimationState::Dragging);
            }
            // Non-thumb area is clicked, scroll the thumb to the click position.
            else
            {
                // Calculate the new scroll position based on the click position.
                int new_scroll_position = cursor_position.y - thumb_height / 2; // Center the click position within the non-thumb area.
                new_scroll_position = static_cast<int>(static_cast<double>(new_scroll_position) / (rect_window.bottom - thumb_height) * (scroll_info.nMax - scroll_info.nPage));

                // Ensure the new scroll position is within the valid range.
                if (new_scroll_position < scroll_info.nMin)
                    new_scroll_position = scroll_info.nMin;
                else if (new_scroll_position > scroll_info.nMax - static_cast<int>(scroll_info.nPage))
                    new_scroll_position = scroll_info.nMax - static_cast<int>(scroll_info.nPage);

                // Scroll to the new position.
                if (p_this->pContainer->enableSmoothScroll)
                {
                    if (!p_this->scrollWindowByPosSmooth(new_scroll_position, 0.15f))
                    {
                        error_message = "Failed to scroll the window by pos. (Smooth)";
                        break;
                    }
                }
                else
                {
                    if (!p_this->scrollWindowByPos(new_scroll_position))
                    {
                        error_message = "Failed to scroll the window by pos.";
                        break;
                    }
                }
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | MESSAGE: 'WM_LBUTTONDOWN' | CALLBACK: 'scMyScrollbarStatic()']", MyLogType::Error);
        }

        break;
    }

    // Process the left-button mouse click (up) to handle thumb event.
    case WM_LBUTTONUP:
    {
        // End thumb dragging event.
        if (p_this->isThumbDragging)
        {
            p_this->isThumbDragging = false;

            // Release the mouse capture.
            ReleaseCapture();

            if (p_this->isThumbHoverState)
            {
                if (p_this->currentAnimationState != ScrollbarAnimationState::Hover)
                    p_this->startAnimation(ScrollbarAnimationState::Hover);
            }
            else
            {
                if (p_this->currentAnimationState != ScrollbarAnimationState::Default)
                    p_this->startAnimation(ScrollbarAnimationState::Default);
            }
        }

        break;
    }

    // Process the mouse mouse message to handle thumb event.
    case WM_MOUSEMOVE:
    {
        bool are_all_operations_success = false;
        std::string error_message = "";
        while (!are_all_operations_success)
        {
            // Get cursor position.
            POINT cursor_position;
            if (!GetCursorPos(&cursor_position))
            {
                error_message = "Failed to retrieve the cursor position.";
                break;
            }
            if (!ScreenToClient(hWnd, &cursor_position))
            {
                error_message = "Failed to convert the cursor position from screen to client.";
                break;
            }

            // Get the scroll informations.
            SCROLLINFO scroll_info;
            scroll_info.cbSize = sizeof(SCROLLINFO);
            scroll_info.fMask = SIF_ALL;
            if (!SendMessageW(p_this->scrollbarWindow, SBM_GETSCROLLINFO, 0, reinterpret_cast<LPARAM>(&scroll_info)))
            {
                error_message = "Failed to retrieve the scroll information.";
                break;
            }

            // Get the window's client rect.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "Failed to retrieve the window client rect.";
                break;
            }

            // Calculate the thumb position and size.
            double visible_ratio = static_cast<double>(scroll_info.nPage) / static_cast<double>(scroll_info.nMax);
            int thumb_height = static_cast<int>(rect_window.bottom * visible_ratio);
            int max_thumb_position = rect_window.bottom - thumb_height;

            // If the thumb is being dragged, scroll the thumb to the cursor position.
            if (p_this->isThumbDragging)
            {
                // Calculate the new scroll position based on the cursor position.
                int thumb_top = p_this->initialThumbPos + (cursor_position.y - p_this->initialClickPos);
                int new_scroll_position = static_cast<int>(static_cast<double>(thumb_top) / (rect_window.bottom - thumb_height) * (scroll_info.nMax - scroll_info.nPage));

                // Ensure the new scroll position is within the valid range.
                if (new_scroll_position < scroll_info.nMin)
                    new_scroll_position = scroll_info.nMin;
                else if (new_scroll_position > scroll_info.nMax - static_cast<int>(scroll_info.nPage))
                    new_scroll_position = scroll_info.nMax - static_cast<int>(scroll_info.nPage);

                // Calculate the scroll amount based on the previous and new scroll positions.
                int scroll_amount = scroll_info.nPos - new_scroll_position;

                // Update the scroll position.
                scroll_info.nPos = new_scroll_position;

                // Set the new scroll position.
                SendMessageW(p_this->scrollbarWindow, SBM_SETPOS, new_scroll_position, TRUE);

                // Scroll the target window.
                p_this->pContainer->scrollContainer(-scroll_amount);

                // Redraw the scrollbar.
                RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
            }
            // If the thumb is not being dragged, check if the cursor is hovering over the thumb.
            // (To update the thumb's appearance)
            else
            {
                int thumb_top = scroll_info.nPos;
                if (scroll_info.nMax > static_cast<INT>(scroll_info.nPage))
                {
                    thumb_top = static_cast<int>(static_cast<double>(scroll_info.nPos) / (scroll_info.nMax - scroll_info.nPage) * max_thumb_position);
                }
                RECT rect_thumb = rect_window;
                rect_thumb.top += thumb_top;
                rect_thumb.bottom = rect_thumb.top + thumb_height;

                // Update the thumb's appearance if the cursor is hovering over the thumb.
                if (PtInRect(&rect_thumb, cursor_position) && !p_this->isThumbHoverState)
                {
                    // Track the mouse leave event.
                    TRACKMOUSEEVENT track_mouse_event;
                    track_mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
                    track_mouse_event.dwFlags = TME_LEAVE;
                    track_mouse_event.hwndTrack = hWnd;
                    TrackMouseEvent(&track_mouse_event);
                    p_this->isThumbHoverState = true;

                    if (p_this->currentAnimationState != ScrollbarAnimationState::Hover)
                        p_this->startAnimation(ScrollbarAnimationState::Hover);
                }
                // Update the thumb's appearance if the cursor is not hovering over the thumb.
                else if (!PtInRect(&rect_thumb, cursor_position) && p_this->isThumbHoverState)
                {
                    p_this->isThumbHoverState = false;

                    if (p_this->currentAnimationState != ScrollbarAnimationState::Default)
                        p_this->startAnimation(ScrollbarAnimationState::Default);
                }
            }

            are_all_operations_success = true;
        }

        if (!are_all_operations_success)
        {
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyVerticalScrollbarSubclass' | MESSAGE: 'WM_MOUSEMOVE' | CALLBACK: 'scMyScrollbarStatic()']", MyLogType::Error);
        }

        break;
    }
    case WM_MOUSELEAVE:
    {
        // Update the thumb hover state.
        if (p_this->isThumbHoverState)
        {
            p_this->isThumbHoverState = false;

            if (p_this->currentAnimationState != ScrollbarAnimationState::Default)
                p_this->startAnimation(ScrollbarAnimationState::Default);

            return 0;
        }

        break;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyVerticalScrollbarSubclass::subclassProcedureStatic, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyVerticalScrollbarSubclass' | FUNC: 'subclassProcedureStatic()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// [MyStandardTextSubclass] class implementations:

bool MyStandardTextSubclass::refresh()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass object is not associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is not associated with a window.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        RedrawWindow(this->textWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardTextSubclass' | FUNC: 'refresh()']", MyLogType::Error);

    return are_all_operation_success;
}
void MyStandardTextSubclass::releaseSharedDeviceResources()
{
    // <This subclass class doesn't have any shared device resources>
}
bool MyStandardTextSubclass::setWindow(HWND hWnd, MyStandardTextSubclassConfig *pConfig)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // Check if the subclass base class is initialized.
        if (!this->isInitialized)
        {
            error_message = "The subclass base class is not initialized.";
            break;
        }

        // Check if the subclass object is already associated with a window.
        if (this->isAssociated)
        {
            error_message = "The subclass object is already associated with a window.";
            break;
        }

        // Check if the configuration structure is valid.
        if (!pConfig->isValid())
        {
            error_message = "The configuration structure contains invalid parameters.";
            break;
        }
        this->textConfig = *pConfig;

        // Store the window handle(s).
        this->textWindow = hWnd;

        // Subclass the window.
        // Store this subclass object pointer as reference data, enabling the subclass procedure to access the non-static object members.
        if (!SetWindowSubclass(this->textWindow, &MyStandardTextSubclass::subclassProcedure, 0, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = "Failed to install the window subclass callback.";
            break;
        }

        // Create the device resources.
        if (!this->createDeviceResources())
        {
            error_message = "Failed to create the device resources.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardTextSubclass' | FUNC: 'setWindow()']", MyLogType::Error);

    return are_all_operation_success;
}
MyStandardTextSubclass *MyStandardTextSubclass::getSubclassPointer(HWND hWnd)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyStandardTextSubclass::subclassProcedure, 0, &reference_data);
    return (is_subclassed ? reinterpret_cast<MyStandardTextSubclass *>(reference_data) : nullptr);
}
bool MyStandardTextSubclass::createSharedDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        // HRESULT hr;

        error_message = "Failed to create the shared device resources.";
        // <This subclass class doesn't have any shared device resources>
        error_message = "";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardTextSubclass' | FUNC: 'createSharedDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyStandardTextSubclass::createDeviceResources(bool recreateSharedResources)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        HDC hdc = GetDC(this->textWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->textWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pD2D1DCRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->graphics()->d2d1Engine().createDCRenderTarget(*this->pD2D1DCRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        hr = (*this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        if (recreateSharedResources)
            MyStandardTextSubclass::releaseSharedDeviceResources();

        if (!this->createSharedDeviceResources())
        {
            error_message = "Failed to create the shared device resources.";
            break;
        }

        error_message = "Failed to create the device resources.";
        this->pTextFormat.reset(new IDWriteTextFormat *(nullptr));
        FLOAT font_size = static_cast<FLOAT>((rect_window.bottom - rect_window.top)) * this->textConfig.fontScale;
        if (!this->config.ignoreTextScalingLimits)
        {
            if (font_size < this->config.textLowerBoundSizeLimit)
                font_size = this->config.textLowerBoundSizeLimit;
            else if (font_size > this->config.textUpperBoundSizeLimit)
                font_size = this->config.textUpperBoundSizeLimit;
        }
        if (!this->graphics()->d2d1Engine().createTextFormat(*this->pTextFormat, this->objects()->fonts.defaultFamily, font_size, this->textConfig.fontWeight, this->textConfig.fontStyle))
            break;
        error_message = "";

        if (!ReleaseDC(this->textWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardTextSubclass' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}
LRESULT CALLBACK MyStandardTextSubclass::subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the subclass object pointer from reference data and use it to access non-static members.
    MyStandardTextSubclass *p_this = reinterpret_cast<MyStandardTextSubclass *>(dwRefData);

    // Process the message.
    switch (uMsg)
    {
    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Override paint messages.
    case WM_PAINT:
    {
        USHORT paint_attempts = 1;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bool are_all_operation_success = false;
        std::string error_message = "";
        while (!are_all_operation_success)
        {
            HRESULT hr;

            // Get the window client rect.
            RECT rect_window;
            if (!GetClientRect(hWnd, &rect_window))
            {
                error_message = "[WM_PAINT] Failed to retrieve the window client rect.";
                break;
            }

            // Get the window text length.
            SetLastError(ERROR_SUCCESS);
            size_t window_text_length = static_cast<size_t>(GetWindowTextLengthW(hWnd));
            if (!window_text_length && GetLastError())
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text length.";
                break;
            }

            // Get the window text.
            std::unique_ptr<WCHAR[]> window_text(new WCHAR[window_text_length + 1]);
            if (!GetWindowTextW(hWnd, window_text.get(), static_cast<INT>(window_text_length) + 1) && window_text_length)
            {
                error_message = "[WM_PAINT] Failed to retrieve the window text.";
                break;
            }

            // Bind the render target to the window device context.
            hr = (*p_this->pD2D1DCRenderTarget)->BindDC(hdc, &rect_window);
            if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to bind the render target to the window device context.";
                break;
            }

            // Prepare drawing resources.
            D2D1_RECT_F d2d1_rect_window = D2D1::RectF(0, 0, static_cast<FLOAT>(rect_window.right), static_cast<FLOAT>(rect_window.bottom));
            const D2D1::ColorF &d2d1_color_text = (p_this->textConfig.pTextColor ? p_this->textConfig.pTextColor->getD2D1Color() : p_this->objects()->colors.textActive.getD2D1Color());
            std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> p_d2d1_solidcolorbrush_text(new ID2D1SolidColorBrush *(nullptr));
            {
                error_message = "[WM_PAINT] Failed to create the resources.";
                hr = (*p_this->pD2D1DCRenderTarget)->CreateSolidColorBrush(d2d1_color_text, &*p_d2d1_solidcolorbrush_text);
                if (FAILED(hr))
                    break;
                error_message = "";
            }

            // Begin drawing.
            (*p_this->pD2D1DCRenderTarget)->BeginDraw();
            (*p_this->pD2D1DCRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            (*p_this->pD2D1DCRenderTarget)->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            {
                // Draw the background.
                (*p_this->pD2D1DCRenderTarget)->Clear((p_this->config.pBackground ? p_this->config.pBackground->getD2D1Color() : p_this->objects()->colors.background.getD2D1Color()));

                // Draw the button text.
                p_this->graphics()->d2d1Engine().drawText((*p_this->pD2D1DCRenderTarget), *p_this->pTextFormat, d2d1_rect_window, window_text.get(), *p_d2d1_solidcolorbrush_text, static_cast<float>(p_this->textConfig.posX), static_cast<float>(p_this->textConfig.posY), p_this->textConfig.centerMode);
            }

            // End drawing.
            hr = (*p_this->pD2D1DCRenderTarget)->EndDraw();
            if (hr == static_cast<long int>(D2DERR_RECREATE_TARGET))
            {
                if (!p_this->createDeviceResources(true))
                {
                    error_message = "[WM_PAINT] Failed to create the device resources.";
                    break;
                }
                if (paint_attempts > 10)
                {
                    error_message = "[WM_PAINT] Failed to create the device resources after 10 attempts.";
                    break;
                }
                paint_attempts++;
                continue; // Repeat the paint operation.
            }
            else if (FAILED(hr))
            {
                error_message = "[WM_PAINT] Failed to end drawing.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
            g_pApp->logger.writeLog(error_message, "[CLASS: 'MyStandardTextSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        EndPaint(hWnd, &ps);
        return 0;
    }

    // Remove the window subclass callback and destroy any associated windows when the window is being destroyed.
    case WM_DESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyStandardTextSubclass::subclassProcedure, uIdSubclass))
            g_pApp->logger.writeLog("[WM_DESTROY] Failed to remove the window subclass callback.", "[CLASS: 'MyStandardTextSubclass' | FUNC: 'subclassProcedure()']", MyLogType::Error);

        return 0;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
