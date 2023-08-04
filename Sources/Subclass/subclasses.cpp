/**
 * @file subclasses.cpp
 * @brief Implementation of the classes defined in subclasses.h.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../Global/global.h"                 // The distribution header of the global variables, forward declarations and my class declarations.

/// @class KeyToggle definitions:
// Constructor:
KeyToggle::KeyToggle(int VirtualKeyCode) : virtualKey(VirtualKeyCode), isActive(false) {}
// Bool Operator:
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

/// @class MyStandardButton definitions:
// Destructor:
MyStandardButton::~MyStandardButton()
{
    // Note:
    // If the animation manager created these variables no longer exists,
    // No need to perform any cleanup as the COM objects already released along with its animation manager.
    if (!MyStandardButton::pAnimationManager)
        return;

    // Release the COM objects (Animation variables).
    for (int i = 0; i < 3; i++)
    {
        if (this->pAnimationVariableButtonRGB[i])
            this->pAnimationVariableButtonRGB[i]->Release();
    }
    for (int i = 0; i < 3; i++)
    {
        if (this->pAnimationVariableButtonBorderRGB[i])
            this->pAnimationVariableButtonBorderRGB[i]->Release();
    }
}
// Private member function(s) [INTERNAL FUNCTIONS]:
bool MyStandardButton::initializeAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        // Create the animation variable.
        hr = MyStandardButton::pAnimationManager->CreateAnimationVariable(initialValue, &pAnimationVariable);
        if (FAILED(hr))
        {
            error_message = L"Failed to create the animation variable.";
            break;
        }

        // Set lower bound value.
        hr = pAnimationVariable->SetLowerBound(lowerBound);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable lower bound value.";
            break;
        }

        // Set upper bound value.
        hr = pAnimationVariable->SetUpperBound(upperBound);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable upper bound value.";
            break;
        }

        hr = pAnimationVariable->SetRoundingMode(roundingMode);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable rounding mode.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"initializeAnimationVariable()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyStandardButton::installSubclass(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Install the subclass callback.
        if (!SetWindowSubclass(hWnd, &MyStandardButton::scMyStandardButton, NULL, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = L"Failed to install the subclass callback \"SetWindowSubclass()\".";
            break;
        }

        // Initialize the animation variables.
        error_message = L"Failed to initialize the animation variables.";
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonRGB[0],
                                               this->pColorStandardButtonDefault->getGDIPColor().GetRed(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonRGB[1],
                                               this->pColorStandardButtonDefault->getGDIPColor().GetGreen(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonRGB[2],
                                               this->pColorStandardButtonDefault->getGDIPColor().GetBlue(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonBorderRGB[0],
                                               this->pColorStandardButtonBorderDefault->getGDIPColor().GetRed(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonBorderRGB[1],
                                               this->pColorStandardButtonBorderDefault->getGDIPColor().GetGreen(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonBorderRGB[2],
                                               this->pColorStandardButtonBorderDefault->getGDIPColor().GetBlue(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        error_message = L"";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"installSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyStandardButton::startAnimation(HWND hWnd, MyStandardButton::ButtonAnimationState animationState)
{
    HRESULT hr;

    // Prepare the necessary pointers for the animation.
    IUIAnimationStoryboard *p_storyboard = nullptr;                                          // Storyboard pointer.
    IUIAnimationTransition *p_transition_button_rgb[3] = {nullptr, nullptr, nullptr};        // Transition pointers for the button RGB values.
    IUIAnimationTransition *p_transition_button_border_rgb[3] = {nullptr, nullptr, nullptr}; // Transition pointers for the button border RGB values.

    switch (animationState)
    {
    case ButtonAnimationState::Default:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyStandardButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Default)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Default)";
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonBorderDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonBorderDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonBorderDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Default)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[0], p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[1], p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[2], p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyStandardButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Default)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Default)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyStandardButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Default;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case ButtonAnimationState::Hover:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyStandardButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Hover)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Hover)";
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonBorderHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonBorderHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::hoverAnimationDuration, this->pColorStandardButtonBorderHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Hover)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[0], p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[1], p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[2], p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyStandardButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Hover)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Hover)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyStandardButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Hover;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case ButtonAnimationState::Down:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyStandardButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Down)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Down)";
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::downAnimationDuration, this->pColorStandardButtonDown->getGDIPColor().GetRed(), 0.7, 0.3, &p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::downAnimationDuration, this->pColorStandardButtonDown->getGDIPColor().GetGreen(), 0.7, 0.3, &p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::downAnimationDuration, this->pColorStandardButtonDown->getGDIPColor().GetBlue(), 0.7, 0.3, &p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::downAnimationDuration, this->pColorStandardButtonBorderDown->getGDIPColor().GetRed(), 0.7, 0.3, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::downAnimationDuration, this->pColorStandardButtonBorderDown->getGDIPColor().GetGreen(), 0.7, 0.3, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::downAnimationDuration, this->pColorStandardButtonBorderDown->getGDIPColor().GetBlue(), 0.7, 0.3, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Down)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[0], p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[1], p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[2], p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyStandardButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Down)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Down)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyStandardButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Down;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case ButtonAnimationState::Flash:
    {
        // Prepare the extra necessary pointers for the animation.
        IUIAnimationTransition *p_reverse_transition_button_rgb[3] = {nullptr, nullptr, nullptr};
        IUIAnimationTransition *p_reverse_transition_button_border_rgb[3] = {nullptr, nullptr, nullptr};

        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyStandardButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Flash)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Flash)";
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonDown->getGDIPColor().GetRed(), 0.8, 0.2, &p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonDown->getGDIPColor().GetGreen(), 0.8, 0.2, &p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonDown->getGDIPColor().GetBlue(), 0.8, 0.2, &p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonBorderDown->getGDIPColor().GetRed(), 0.8, 0.2, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonBorderDown->getGDIPColor().GetGreen(), 0.8, 0.2, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonBorderDown->getGDIPColor().GetBlue(), 0.8, 0.2, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonDefault->getGDIPColor().GetRed(), 0.8, 0.2, &p_reverse_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonDefault->getGDIPColor().GetGreen(), 0.8, 0.2, &p_reverse_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonDefault->getGDIPColor().GetBlue(), 0.8, 0.2, &p_reverse_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonBorderDefault->getGDIPColor().GetRed(), 0.8, 0.2, &p_reverse_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonBorderDefault->getGDIPColor().GetGreen(), 0.8, 0.2, &p_reverse_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyStandardButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyStandardButton::flashAnimationDuration, this->pColorStandardButtonBorderDefault->getGDIPColor().GetBlue(), 0.8, 0.2, &p_reverse_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Flash)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[0], p_transition_button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[1], p_transition_button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonRGB[2], p_transition_button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            UI_ANIMATION_KEYFRAME keyframe_end;
            hr = p_storyboard->AddKeyframeAfterTransition(p_transition_button_border_rgb[2], &keyframe_end); // Add keyframe at the end of the previous transitions.
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonRGB[0], p_reverse_transition_button_rgb[0], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonRGB[1], p_reverse_transition_button_rgb[1], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonRGB[2], p_reverse_transition_button_rgb[2], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonBorderRGB[0], p_reverse_transition_button_border_rgb[0], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonBorderRGB[1], p_reverse_transition_button_border_rgb[1], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonBorderRGB[2], p_reverse_transition_button_border_rgb[2], keyframe_end);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyStandardButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Flash)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Flash)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_rgb[i])
                {
                    p_reverse_transition_button_rgb[i]->Release();
                    p_reverse_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_border_rgb[i])
                {
                    p_reverse_transition_button_border_rgb[i]->Release();
                    p_reverse_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyStandardButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Default;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_rgb[i])
                {
                    p_transition_button_rgb[i]->Release();
                    p_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_rgb[i])
                {
                    p_reverse_transition_button_rgb[i]->Release();
                    p_reverse_transition_button_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_border_rgb[i])
                {
                    p_reverse_transition_button_border_rgb[i]->Release();
                    p_reverse_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }
    }

    return false;
}
// Private member function(s) [SUBCLASS CALLBACK FUNCTIONS]:
LRESULT CALLBACK MyStandardButton::scMyStandardButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the poiner from dwRefData and use it to access non-static members.
    MyStandardButton *p_this = reinterpret_cast<MyStandardButton *>(dwRefData);

    // Check if the enter key is pressed and the button has focus.
    if (*MyStandardButton::pEnterKeyToggle && GetFocus() == hWnd)
    {
        // Trigger flash animation if the button is not hovered or pressed down.
        if (!p_this->isHoverState && !p_this->isDownState)
        {
            if (!p_this->startAnimation(hWnd, ButtonAnimationState::Flash))
            {
                WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"!p_this->isHoverState && !p_this->isDownState\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
            }
        }

        // Trigger the button's WM_COMMAND message.
        SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), NULL);
    }
    // Check if the tab key is pressed and the button has focus.
    else if (GetFocus() == hWnd && GetKeyState(VK_TAB) & 0x8000)
    {
        // Perform redraw on the button to draw the focus border.
        InvalidateRect(hWnd, NULL, FALSE);
    }

    switch (uMsg)
    {
    // Remove the window subclass callback.
    case WM_NCDESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyStandardButton::scMyStandardButton, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Override paint messages.
    case WM_PAINT:
    {
        HRESULT hr;
        PAINTSTRUCT ps;
        HDC hdc = NULL;
        HDC mem_hdc = NULL;
        HBITMAP bitmap = NULL;
        HBITMAP bitmap_old = NULL;
        bool is_paint_begined = false;
        bool is_bitmap_created = false;
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get the animation values.
            INT32 button_rgb[3] = {0, 0, 0};
            INT32 button_border_rgb[3] = {0, 0, 0};
            error_message = L"Failed to retrieve the animation values.";
            hr = p_this->pAnimationVariableButtonRGB[0]->GetIntegerValue(&button_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonRGB[1]->GetIntegerValue(&button_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonRGB[2]->GetIntegerValue(&button_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonBorderRGB[0]->GetIntegerValue(&button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonBorderRGB[1]->GetIntegerValue(&button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonBorderRGB[2]->GetIntegerValue(&button_border_rgb[2]);
            if (FAILED(hr))
                break;
            Gdiplus::Color button_color(255, button_rgb[0], button_rgb[1], button_rgb[2]);
            Gdiplus::Color button_border_color(255, button_border_rgb[0], button_border_rgb[1], button_border_rgb[2]);
            error_message = L"";

            // Get the button's client rects.
            RECT rect_button;
            Gdiplus::Rect gdip_rect_button;
            if (!GetClientRect(hWnd, &rect_button))
            {
                error_message = L"Failed to get the client rects.";
                break;
            }
            gdip_rect_button = Gdiplus::Rect(rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);

            // Get the button's text length.
            INT button_text_length = GetWindowTextLengthW(hWnd);
            SetLastError(NULL);
            if (!button_text_length && GetLastError() != NULL)
            {
                error_message = L"Failed to get the button text length.";
                break;
            }

            // Get the button's text.
            WCHAR *text_buffer = new WCHAR[button_text_length + 1];
            if (!GetWindowTextW(hWnd, text_buffer, button_text_length + 1) && button_text_length != NULL)
            {
                error_message = L"Failed to get the button text.";
                delete[] text_buffer;
                break;
            }
            std::wstring button_text(text_buffer);
            delete[] text_buffer;

            // Begin the paintings.
            hdc = BeginPaint(hWnd, &ps);
            if (!hdc)
            {
                error_message = L"Failed to execute \"BeginPaint()\".";
                break;
            }
            is_paint_begined = true;

            // Create memory device context and bitmap object for double buffering.
            mem_hdc = CreateCompatibleDC(hdc);
            if (!mem_hdc)
            {
                error_message = L"Failed to create the compatible memory device context.";
                break;
            }
            bitmap = CreateCompatibleBitmap(hdc, rect_button.right - rect_button.left, rect_button.bottom - rect_button.top);
            if (!bitmap)
            {
                error_message = L"Failed to create the compatible bitmap object.";
                break;
            }
            is_bitmap_created = true;
            bitmap_old = reinterpret_cast<HBITMAP>(SelectObject(mem_hdc, bitmap));
            if (!bitmap_old)
            {
                error_message = L"Failed to select the bitmap object.";
                break;
            }

            // Create Gdiplus::Graphics object for GDI+ drawing operations.
            Gdiplus::Graphics gdip_graphics(mem_hdc);

            // Set GDI+ smoothing mode.
            if (gdip_graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality))
            {
                error_message = L"Failed to set GDI+ smoothing mode.";
                break;
            }

            // Set background mode.
            if (!SetBkMode(mem_hdc, TRANSPARENT))
            {
                error_message = L"Failed to set the background mode.";
                break;
            }

            // Select font object.
            if (!SelectObject(mem_hdc, reinterpret_cast<HFONT>(MyStandardButton::pFontDefault->getHFONT())))
            {
                error_message = L"Failed to select the font object.";
                break;
            }

            // Set text color.
            if (SetTextColor(mem_hdc, (p_this->isHoverState || p_this->isDownState ? MyStandardButton::pColorTextHighlight->getCOLORREF() : MyStandardButton::pColorTextDefault->getCOLORREF())) == CLR_INVALID)
            {
                error_message = L"Failed to set the text color.";
                break;
            }

            // Begin painting to the memory device context.
            {
                // Draw the button's background.
                if (!MyDraw_FillRect(&gdip_graphics, gdip_rect_button, &MyStandardButton::pColorBackground->getGDIPColor()))
                {
                    error_message = L"Failed to draw the button's background.";
                    break;
                }

                // Draw the button.
                if (!MyDraw_FillRoundRect(&gdip_graphics, gdip_rect_button, 4, &button_color, &button_border_color))
                {
                    error_message = L"Failed to draw the button.";
                    break;
                }

                // Draw the button's text.
                DrawTextW(mem_hdc, button_text.c_str(), -1, &rect_button, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                // Draw the button's focus border.
                if (GetFocus() == hWnd)
                {
                    if (!MyDraw_DrawRoundRect(&gdip_graphics, gdip_rect_button, 4, 1, MyStandardButton::pColorFocus->getGDIPColor()))
                    {
                        error_message = L"Failed to draw the button focus border.";
                        break;
                    }
                }
            }

            // Draw contents from memory device context to target device context.
            if (!BitBlt(hdc, 0, 0, rect_button.right - rect_button.left, rect_button.bottom - rect_button.top, mem_hdc, 0, 0, SRCCOPY))
            {
                error_message = L"Failed to draw contents from memory device context to target device context.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (is_paint_begined)
        {
            // Delete the bitmap object.
            if (is_bitmap_created)
            {
                if (!SelectObject(mem_hdc, bitmap_old))
                {
                    WriteLog(L"Failed to select the bitmap object.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
                }
                if (!DeleteObject(bitmap))
                {
                    WriteLog(L"Failed to delete the bitmap object.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
                }
                bitmap = NULL;
                bitmap_old = NULL;
            }

            // Delete the memory device context.
            if (mem_hdc)
            {
                if (!DeleteDC(mem_hdc))
                {
                    WriteLog(L"Failed to delete the memory device context.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
                }
                mem_hdc = NULL;
            }

            // End the paintings.
            EndPaint(hWnd, &ps);
            return 0;
        }

        break;
    }

    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Process the invalidate timer to update the animation manager.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyStandardButton::IDT_ANIMATION_INVALIDATE:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                HRESULT hr;
                // Get the current time.
                UI_ANIMATION_SECONDS seconds_now;
                hr = MyStandardButton::pAnimationTimer->GetTime(&seconds_now);
                if (FAILED(hr))
                {
                    error_message = L"Failed to retrieve the current time.";
                    break;
                }

                // Update the animation manager.
                hr = MyStandardButton::pAnimationManager->Update(seconds_now);
                if (FAILED(hr))
                {
                    error_message = L"Failed to update the animation manager.";
                    break;
                }

                // Invalidate the control.
                InvalidateRect(hWnd, NULL, TRUE);

                // Get the animation manager's status.
                UI_ANIMATION_MANAGER_STATUS status;
                hr = MyStandardButton::pAnimationManager->GetStatus(&status);
                if (FAILED(hr))
                {
                    error_message = L"Failed to get the animation manager's status.";
                    break;
                }

                // Kill the timer if the animation manager is idle.
                if (status == UI_ANIMATION_MANAGER_IDLE)
                {
                    if (!KillTimer(hWnd, MyStandardButton::IDT_ANIMATION_INVALIDATE))
                    {
                        error_message = L"Failed to kill the timer.";
                        break;
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer.
                if (!KillTimer(hWnd, MyStandardButton::IDT_ANIMATION_INVALIDATE))
                {
                    WriteLog(L"Failed to kill the timer.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_TIMER/IDT_ANIMATION_INVALIDATE\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
                }

                WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_TIMER/IDT_ANIMATION_INVALIDATE\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);

                return 0;
            }

            return 0;
        }

        default:
        {
            WriteLog(L"Unprocessed timer message.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_TIMER\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process the left-button mouse click (down) to trigger the appropriate animation and action(s).
    case WM_LBUTTONDOWN:
    {
        if (!p_this->isDownState && p_this->isHoverState)
        {
            p_this->isDownState = true;
            if (!p_this->startAnimation(hWnd, ButtonAnimationState::Down))
            {
                WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_LBUTTONDOWN\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
            }

            // Trigger the button's WM_COMMAND message.
            SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), NULL);

            return 0;
        }

        break;
    }

    // Process the left-button mouse click (up) to trigger the appropriate animation.
    case WM_LBUTTONUP:
    {
        if (p_this->isDownState)
        {
            p_this->isDownState = false;
            if (p_this->isHoverState)
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Hover))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
                }
            }
            else
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Default))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
                }
            }
        }

        break;
    }

    // Process the mouse leave message to trigger the appropriate animation.
    case WM_MOUSELEAVE:
    {
        if (p_this->isHoverState)
        {
            p_this->isHoverState = false;
            p_this->isDownState = false;
            if (!p_this->startAnimation(hWnd, ButtonAnimationState::Default))
            {
                WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_MOUSELEAVE\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
            }

            return 0;
        }

        break;
    }

    // Process the mouse mouse message to trigger the appropriate animation.
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
            if (!p_this->startAnimation(hWnd, ButtonAnimationState::Hover))
            {
                WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyStandardButton\" | MESSAGE: \"WM_MOUSEMOVE\" | CALLBACK: \"scMyStandardButton()\"]", MyLogType::Error);
            }

            return 0;
        }

        break;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
// Public static member function(s) [INITIALIZATION FUNCTIONS]:
bool MyStandardButton::initialize(MyStandardButtonInitializeConfig initializeConfig)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the initialization structure is valid.
        if (!initializeConfig.isValid())
        {
            error_message = L"The initialization structure is invalid.";
            break;
        }

        // Set the pointers.
        MyStandardButton::pEnterKeyToggle = initializeConfig.pEnterKeyToggle;
        MyStandardButton::pAnimationManager = std::ref(*initializeConfig.ppAnimationManager);
        MyStandardButton::pAnimationTimer = std::ref(*initializeConfig.ppAnimationTimer);
        MyStandardButton::pTransitionLibrary = std::ref(*initializeConfig.ppTransitionLibrary);

        // Update initialization state.
        MyStandardButton::isInitialized = true;

        // Update class ready state.
        if (MyStandardButton::isSharedPropertiesSet)
            MyStandardButton::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"initialize()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyStandardButton::setSharedProperties(MyStandardButtonSharedPropertiesConfig configSharedProperties)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the shared properties configuration structure is valid.
        if (!configSharedProperties.isValid())
        {
            error_message = L"The shared properties configuration structure is invalid.";
            break;
        }

        // Set the pointers.
        MyStandardButton::pColorStandardButtonDefault = configSharedProperties.pColorStandardButtonDefault;
        MyStandardButton::pColorStandardButtonHover = configSharedProperties.pColorStandardButtonHover;
        MyStandardButton::pColorStandardButtonDown = configSharedProperties.pColorStandardButtonDown;
        MyStandardButton::pColorStandardButtonBorderDefault = configSharedProperties.pColorStandardButtonBorderDefault;
        MyStandardButton::pColorStandardButtonBorderHover = configSharedProperties.pColorStandardButtonBorderHover;
        MyStandardButton::pColorStandardButtonBorderDown = configSharedProperties.pColorStandardButtonBorderDown;
        MyStandardButton::pColorTextDefault = configSharedProperties.pColorTextDefault;
        MyStandardButton::pColorTextHighlight = configSharedProperties.pColorTextHighlight;
        MyStandardButton::pColorBackground = configSharedProperties.pColorBackground;
        MyStandardButton::pColorFocus = configSharedProperties.pColorFocus;
        MyStandardButton::pFontDefault = configSharedProperties.pFontDefault;

        // Update initialization state.
        MyStandardButton::isSharedPropertiesSet = true;

        // Update class ready state.
        if (MyStandardButton::isInitialized)
            MyStandardButton::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"setSharedProperties()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
bool MyStandardButton::isSubclassed(HWND hWnd, void **ppSubclass)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyStandardButton::scMyStandardButton, NULL, &reference_data);
    if (is_subclassed && ppSubclass)
    {
        *ppSubclass = reinterpret_cast<MyStandardButton *>(reference_data);
    }

    return is_subclassed;
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyStandardButton::setSubclass(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the class is initialized.
        if (!MyStandardButton::isReady)
        {
            error_message = L"The class is not initialized.";
            break;
        }

        // Install the subclass callback.
        if (!this->installSubclass(hWnd))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyStandardButton\" | FUNC: \"setSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyStandardButton::refresh(HWND hWnd)
{
    if (!this->startAnimation(hWnd, this->currentAnimationState))
    {
        WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyStandardButton\" | FUNC: \"refresh()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class MyImageButton definitions:
// Destructor:
MyImageButton::~MyImageButton()
{
    // Note:
    // If the animation manager created these variables no longer exists,
    // No need to perform any cleanup as the COM objects already released along with its animation manager.
    if (!MyImageButton::pAnimationManager)
        return;

    // Release the COM objects (Animation variables).
    for (int i = 0; i < 3; i++)
    {
        if (this->pAnimationVariableBackgroundRGB[i])
            this->pAnimationVariableBackgroundRGB[i]->Release();
    }
    if (this->pAnimationVariableDefaultIconOpacity)
        this->pAnimationVariableDefaultIconOpacity->Release();
    if (this->pAnimationVariableHoverIconOpacity)
        this->pAnimationVariableHoverIconOpacity->Release();
    if (this->pAnimationVariableDownIconOpacity)
        this->pAnimationVariableDownIconOpacity->Release();
}
// Private member function(s) [INTERNAL FUNCTIONS]:
bool MyImageButton::initializeAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        // Create the animation variable.
        hr = MyImageButton::pAnimationManager->CreateAnimationVariable(initialValue, &pAnimationVariable);
        if (FAILED(hr))
        {
            error_message = L"Failed to create the animation variable.";
            break;
        }

        // Set lower bound value.
        hr = pAnimationVariable->SetLowerBound(lowerBound);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable lower bound value.";
            break;
        }

        // Set upper bound value.
        hr = pAnimationVariable->SetUpperBound(upperBound);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable upper bound value.";
            break;
        }

        hr = pAnimationVariable->SetRoundingMode(roundingMode);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable rounding mode.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"initializeAnimationVariable()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyImageButton::installSubclass(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Install the subclass callback.
        if (!SetWindowSubclass(hWnd, &MyImageButton::scMyImageButton, NULL, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = L"Failed to install the subclass callback \"SetWindowSubclass()\".";
            break;
        }

        // Initialize the animation variables.
        error_message = L"Failed to initialize the animation variables.";
        if (!this->initializeAnimationVariable(this->pAnimationVariableBackgroundRGB[0],
                                               this->pColorBackgroundDefault->getGDIPColor().GetRed(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableBackgroundRGB[1],
                                               this->pColorBackgroundDefault->getGDIPColor().GetGreen(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableBackgroundRGB[2],
                                               this->pColorBackgroundDefault->getGDIPColor().GetBlue(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableDefaultIconOpacity, 1.0, 0.0, 1.0))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableHoverIconOpacity, 0.0, 0.0, 1.0))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableDownIconOpacity, 0.0, 0.0, 1.0))
            break;
        error_message = L"";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"installSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyImageButton::startAnimation(HWND hWnd, MyImageButton::ButtonAnimationState animationState)
{
    HRESULT hr;

    // Prepare the necessary pointers for the animation.
    IUIAnimationStoryboard *p_storyboard = nullptr;                                       // Storyboard pointer.
    IUIAnimationTransition *p_transition_background_rgb[3] = {nullptr, nullptr, nullptr}; // Transition pointers for the background RGB values.
    IUIAnimationTransition *p_transition_default_icon_opacity = nullptr;                  // Transition pointer for the default icon opacity value.
    IUIAnimationTransition *p_transition_hover_icon_opacity = nullptr;                    // Transition pointer for the hover icon opacity value.
    IUIAnimationTransition *p_transition_down_icon_opacity = nullptr;                     // Transition pointer for the down icon opacity value.

    switch (animationState)
    {
    case ButtonAnimationState::Default:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyImageButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Default)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Default)";
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, this->pColorBackgroundDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, this->pColorBackgroundDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, this->pColorBackgroundDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 1.0, 0.5, 0.5, &p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 0.0, 0.5, 0.5, &p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 0.0, 0.5, 0.5, &p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Default)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[0], p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[1], p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[2], p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableDefaultIconOpacity, p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableHoverIconOpacity, p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableDownIconOpacity, p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyImageButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Default)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Default)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyImageButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Default;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case ButtonAnimationState::Hover:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyImageButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Hover)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Hover)";
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, this->pColorBackgroundHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, this->pColorBackgroundHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, this->pColorBackgroundHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 0.0, 0.5, 0.5, &p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 1.0, 0.5, 0.5, &p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 0.0, 0.5, 0.5, &p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Hover)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[0], p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[1], p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[2], p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableDefaultIconOpacity, p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableHoverIconOpacity, p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableDownIconOpacity, p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyImageButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Hover)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Hover)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyImageButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Hover;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case ButtonAnimationState::Down:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyImageButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Down)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Down)";
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::downAnimationDuration, this->pColorBackgroundDown->getGDIPColor().GetRed(), 0.7, 0.3, &p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::downAnimationDuration, this->pColorBackgroundDown->getGDIPColor().GetGreen(), 0.7, 0.3, &p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::downAnimationDuration, this->pColorBackgroundDown->getGDIPColor().GetBlue(), 0.7, 0.3, &p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 0.0, 0.5, 0.5, &p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 0.0, 0.5, 0.5, &p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::hoverAnimationDuration, 1.0, 0.5, 0.5, &p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Down)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[0], p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[1], p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[2], p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableDefaultIconOpacity, p_transition_default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableHoverIconOpacity, p_transition_hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableDownIconOpacity, p_transition_down_icon_opacity);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyImageButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Down)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Down)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyImageButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Down;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case ButtonAnimationState::Flash:
    {
        // Prepare the extra necessary pointers for the animation.
        IUIAnimationTransition *p_reverse_transition_background_rgb[3] = {nullptr, nullptr, nullptr};

        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyImageButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Flash)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Flash)";
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::flashAnimationDuration, this->pColorBackgroundDown->getGDIPColor().GetRed(), 0.8, 0.2, &p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::flashAnimationDuration, this->pColorBackgroundDown->getGDIPColor().GetGreen(), 0.8, 0.2, &p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::flashAnimationDuration, this->pColorBackgroundDown->getGDIPColor().GetBlue(), 0.8, 0.2, &p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::flashAnimationDuration, this->pColorBackgroundDefault->getGDIPColor().GetRed(), 0.8, 0.2, &p_reverse_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::flashAnimationDuration, this->pColorBackgroundDefault->getGDIPColor().GetGreen(), 0.8, 0.2, &p_reverse_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyImageButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyImageButton::flashAnimationDuration, this->pColorBackgroundDefault->getGDIPColor().GetBlue(), 0.8, 0.2, &p_reverse_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Flash)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[0], p_transition_background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[1], p_transition_background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableBackgroundRGB[2], p_transition_background_rgb[2]);
            if (FAILED(hr))
                break;
            UI_ANIMATION_KEYFRAME keyframe_end;
            hr = p_storyboard->AddKeyframeAfterTransition(p_transition_background_rgb[2], &keyframe_end); // Add keyframe at the end of the previous transitions.
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableBackgroundRGB[0], p_reverse_transition_background_rgb[0], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableBackgroundRGB[1], p_reverse_transition_background_rgb[1], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableBackgroundRGB[2], p_reverse_transition_background_rgb[2], keyframe_end);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyImageButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Flash)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Flash)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_background_rgb[i])
                {
                    p_reverse_transition_background_rgb[i]->Release();
                    p_reverse_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyImageButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Default;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_background_rgb[i])
                {
                    p_transition_background_rgb[i]->Release();
                    p_transition_background_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_background_rgb[i])
                {
                    p_reverse_transition_background_rgb[i]->Release();
                    p_reverse_transition_background_rgb[i] = nullptr;
                }
            }
            if (p_transition_default_icon_opacity)
            {
                p_transition_default_icon_opacity->Release();
                p_transition_default_icon_opacity = nullptr;
            }
            if (p_transition_hover_icon_opacity)
            {
                p_transition_hover_icon_opacity->Release();
                p_transition_hover_icon_opacity = nullptr;
            }
            if (p_transition_down_icon_opacity)
            {
                p_transition_down_icon_opacity->Release();
                p_transition_down_icon_opacity = nullptr;
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }
    }

    return false;
}
// Private member function(s) [SUBCLASS CALLBACK FUNCTIONS]:
LRESULT CALLBACK MyImageButton::scMyImageButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the poiner from dwRefData and use it to access non-static members.
    MyImageButton *p_this = reinterpret_cast<MyImageButton *>(dwRefData);

    // Check if the enter key is pressed and the button has focus.
    if (*MyImageButton::pEnterKeyToggle && GetFocus() == hWnd)
    {
        // Trigger flash animation if the button is not hovered or pressed down.
        if (!p_this->isHoverState && !p_this->isDownState)
        {
            if (!p_this->startAnimation(hWnd, ButtonAnimationState::Flash))
            {
                WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"!p_this->isHoverState && !p_this->isDownState\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
            }
        }

        // Trigger the button's WM_COMMAND message.
        SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), NULL);
    }
    // Check if the tab key is pressed and the button has focus.
    else if (GetFocus() == hWnd && GetKeyState(VK_TAB) & 0x8000)
    {
        // Perform redraw on the button to draw the focus border.
        InvalidateRect(hWnd, NULL, FALSE);
    }

    switch (uMsg)
    {
    // Remove the window subclass callback.
    case WM_NCDESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyImageButton::scMyImageButton, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Override paint messages.
    case WM_PAINT:
    {
        HRESULT hr;
        PAINTSTRUCT ps;
        HDC hdc = NULL;
        HDC mem_hdc = NULL;
        HBITMAP bitmap = NULL;
        HBITMAP bitmap_old = NULL;
        bool is_paint_begined = false;
        bool is_bitmap_created = false;
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get the animation values.
            INT32 background_rgb[3] = {0, 0, 0};
            DOUBLE default_icon_opacity = 0.0;
            DOUBLE hover_icon_opacity = 0.0;
            DOUBLE down_icon_opacity = 0.0;
            error_message = L"Failed to retrieve the animation values.";
            hr = p_this->pAnimationVariableBackgroundRGB[0]->GetIntegerValue(&background_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableBackgroundRGB[1]->GetIntegerValue(&background_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableBackgroundRGB[2]->GetIntegerValue(&background_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableDefaultIconOpacity->GetValue(&default_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableHoverIconOpacity->GetValue(&hover_icon_opacity);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableDownIconOpacity->GetValue(&down_icon_opacity);
            if (FAILED(hr))
                break;
            Gdiplus::Color background_color(255, background_rgb[0], background_rgb[1], background_rgb[2]);
            error_message = L"";

            // Get the button's client rects.
            RECT rect_button;
            Gdiplus::Rect gdip_rect_button;
            if (!GetClientRect(hWnd, &rect_button))
            {
                error_message = L"Failed to get the client rects.";
                break;
            }
            gdip_rect_button = Gdiplus::Rect(rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);

            // Begin the paintings.
            hdc = BeginPaint(hWnd, &ps);
            if (!hdc)
            {
                error_message = L"Failed to execute \"BeginPaint()\".";
                break;
            }
            is_paint_begined = true;

            // Create memory device context and bitmap object for double buffering.
            mem_hdc = CreateCompatibleDC(hdc);
            if (!mem_hdc)
            {
                error_message = L"Failed to create the compatible memory device context.";
                break;
            }
            bitmap = CreateCompatibleBitmap(hdc, rect_button.right - rect_button.left, rect_button.bottom - rect_button.top);
            if (!bitmap)
            {
                error_message = L"Failed to create the compatible bitmap object.";
                break;
            }
            is_bitmap_created = true;
            bitmap_old = reinterpret_cast<HBITMAP>(SelectObject(mem_hdc, bitmap));
            if (!bitmap_old)
            {
                error_message = L"Failed to select the bitmap object.";
                break;
            }

            // Create Gdiplus::Graphics object for GDI+ drawing operations.
            Gdiplus::Graphics gdip_graphics(mem_hdc);

            // Set GDI+ smoothing mode.
            if (gdip_graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality))
            {
                error_message = L"Failed to set GDI+ smoothing mode.";
                break;
            }

            // Begin painting to the memory device context.
            {
                // Draw the button's background.
                if (!MyDraw_FillRect(&gdip_graphics, gdip_rect_button, &background_color))
                {
                    error_message = L"Failed to draw the button's background.";
                    break;
                }

                // Draw the button default image.
                if (!MyDraw_DrawImage(&gdip_graphics, gdip_rect_button, (*(*p_this->pImageDefault)).getGDIPImage(), static_cast<FLOAT>(default_icon_opacity), p_this->imagePosX, p_this->imagePosY, p_this->imageWidth, p_this->imageHeight, p_this->isCentering))
                {
                    error_message = L"Failed to draw the button's default image.";
                    break;
                }

                // Draw the button hover image.
                if (!p_this->skipHoverAnimationState)
                {
                    if (!MyDraw_DrawImage(&gdip_graphics, gdip_rect_button, (*(*p_this->pImageHover)).getGDIPImage(), static_cast<FLOAT>(hover_icon_opacity), p_this->imagePosX, p_this->imagePosY, p_this->imageWidth, p_this->imageHeight, p_this->isCentering))
                    {
                        error_message = L"Failed to draw the button's hover image.";
                        break;
                    }
                }

                // Draw the button down image.
                if (!p_this->skipDownAnimationState)
                {
                    if (!MyDraw_DrawImage(&gdip_graphics, gdip_rect_button, (*(*p_this->pImageDown)).getGDIPImage(), static_cast<FLOAT>(down_icon_opacity), p_this->imagePosX, p_this->imagePosY, p_this->imageWidth, p_this->imageHeight, p_this->isCentering))
                    {
                        error_message = L"Failed to draw the button's down image.";
                        break;
                    }
                }

                // Draw the button's focus border.
                if (GetFocus() == hWnd)
                {
                    if (!FrameRect(mem_hdc, &rect_button, MyImageButton::pColorFocus->getHBRUSH()))
                    {
                        error_message = L"Failed to draw the button's focus border.";
                        break;
                    }
                }
            }

            // Draw contents from memory device context to target device context.
            if (!BitBlt(hdc, 0, 0, rect_button.right - rect_button.left, rect_button.bottom - rect_button.top, mem_hdc, 0, 0, SRCCOPY))
            {
                error_message = L"Failed to draw contents from memory device context to target device context.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (is_paint_begined)
        {
            // Delete the bitmap object.
            if (is_bitmap_created)
            {
                if (!SelectObject(mem_hdc, bitmap_old))
                {
                    WriteLog(L"Failed to select the bitmap object.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                }
                if (!DeleteObject(bitmap))
                {
                    WriteLog(L"Failed to delete the bitmap object.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                }
                bitmap = NULL;
                bitmap_old = NULL;
            }

            // Delete the memory device context.
            if (mem_hdc)
            {
                if (!DeleteDC(mem_hdc))
                {
                    WriteLog(L"Failed to delete the memory device context.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                }
                mem_hdc = NULL;
            }

            // End the paintings.
            EndPaint(hWnd, &ps);
            return 0;
        }

        break;
    }

    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Process the invalidate timer to update the animation manager.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyImageButton::IDT_ANIMATION_INVALIDATE:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                HRESULT hr;
                // Get the current time.
                UI_ANIMATION_SECONDS seconds_now;
                hr = MyImageButton::pAnimationTimer->GetTime(&seconds_now);
                if (FAILED(hr))
                {
                    error_message = L"Failed to retrieve the current time.";
                    break;
                }

                // Update the animation manager.
                hr = MyImageButton::pAnimationManager->Update(seconds_now);
                if (FAILED(hr))
                {
                    error_message = L"Failed to update the animation manager.";
                    break;
                }

                // Invalidate the control.
                InvalidateRect(hWnd, NULL, TRUE);

                // Get the animation manager's status.
                UI_ANIMATION_MANAGER_STATUS status;
                hr = MyImageButton::pAnimationManager->GetStatus(&status);
                if (FAILED(hr))
                {
                    error_message = L"Failed to get the animation manager's status.";
                    break;
                }

                // Kill the timer if the animation manager is idle.
                if (status == UI_ANIMATION_MANAGER_IDLE)
                {
                    if (!KillTimer(hWnd, MyImageButton::IDT_ANIMATION_INVALIDATE))
                    {
                        error_message = L"Failed to kill the timer.";
                        break;
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer.
                if (!KillTimer(hWnd, MyImageButton::IDT_ANIMATION_INVALIDATE))
                {
                    WriteLog(L"Failed to kill the timer.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_TIMER/IDT_ANIMATION_INVALIDATE\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                }

                WriteLog(error_message, L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_TIMER/IDT_ANIMATION_INVALIDATE\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);

                return 0;
            }

            return 0;
        }

        default:
        {
            WriteLog(L"Unprocessed timer message.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_TIMER\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process the left-button mouse click (down) to trigger the appropriate animation and action(s).
    case WM_LBUTTONDOWN:
    {
        if (!p_this->isDownState && p_this->isHoverState)
        {
            p_this->isDownState = true;
            if (!p_this->skipDownAnimationState)
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Down))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_LBUTTONDOWN\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                }
            }

            // Trigger the button's WM_COMMAND message.
            SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), NULL);

            return 0;
        }

        break;
    }

    // Process the left-button mouse click (up) to trigger the appropriate animation.
    case WM_LBUTTONUP:
    {
        if (p_this->isDownState)
        {
            p_this->isDownState = false;
            if (p_this->isHoverState)
            {
                if (!p_this->skipHoverAnimationState)
                {
                    if (!p_this->startAnimation(hWnd, ButtonAnimationState::Hover))
                    {
                        WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                    }
                }
                else
                {
                    if (!p_this->startAnimation(hWnd, ButtonAnimationState::Default))
                    {
                        WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                    }
                }
            }
            else
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Default))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                }
            }
        }

        break;
    }

    // Process the mouse leave message to trigger the appropriate animation.
    case WM_MOUSELEAVE:
    {
        if (p_this->isHoverState)
        {
            p_this->isHoverState = false;
            p_this->isDownState = false;
            if (!p_this->startAnimation(hWnd, ButtonAnimationState::Default))
            {
                WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_MOUSELEAVE\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
            }

            return 0;
        }

        break;
    }

    // Process the mouse mouse message to trigger the appropriate animation.
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
            if (!p_this->skipHoverAnimationState)
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Hover))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | MESSAGE: \"WM_MOUSEMOVE\" | CALLBACK: \"scMyImageButton()\"]", MyLogType::Error);
                }
            }

            return 0;
        }

        break;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
// Public static member function(s) [INITIALIZATION FUNCTIONS]:
bool MyImageButton::initialize(MyImageButtonInitializeConfig initializeConfig)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the initialization structure is valid.
        if (!initializeConfig.isValid())
        {
            error_message = L"The initialization structure is invalid.";
            break;
        }

        // Set the pointers.
        MyImageButton::pEnterKeyToggle = initializeConfig.pEnterKeyToggle;
        MyImageButton::pAnimationManager = std::ref(*initializeConfig.ppAnimationManager);
        MyImageButton::pAnimationTimer = std::ref(*initializeConfig.ppAnimationTimer);
        MyImageButton::pTransitionLibrary = std::ref(*initializeConfig.ppTransitionLibrary);

        // Update initialization state.
        MyImageButton::isInitialized = true;

        // Update class ready state.
        if (MyImageButton::isSharedPropertiesSet)
            MyImageButton::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"initialize()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyImageButton::setSharedProperties(MyImageButtonSharedPropertiesConfig configSharedProperties)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the shared properties configuration structure is valid.
        if (!configSharedProperties.isValid())
        {
            error_message = L"The shared properties configuration structure is invalid.";
            break;
        }

        // Set the pointers.
        MyImageButton::pColorFocus = configSharedProperties.pColorFocus;

        // Update initialization state.
        MyImageButton::isSharedPropertiesSet = true;

        // Update class ready state.
        if (MyImageButton::isInitialized)
            MyImageButton::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"setSharedProperties()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
bool MyImageButton::isSubclassed(HWND hWnd, void **ppSubclass)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyImageButton::scMyImageButton, NULL, &reference_data);
    if (is_subclassed && ppSubclass)
    {
        *ppSubclass = reinterpret_cast<MyImageButton *>(reference_data);
    }

    return is_subclassed;
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyImageButton::setSubclass(HWND hWnd, MyImageButtonNonSharedPropertiesConfig configNonSharedProperties)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the class is initialized.
        if (!MyImageButton::isReady)
        {
            error_message = L"The class is not initialized.";
            break;
        }

        // Check if the drawing object pointers configuration structure is valid.
        if (!configNonSharedProperties.isValid())
        {
            error_message = L"The drawing object pointers configuration structure is invalid.";
            break;
        }

        // Set the pointers.
        this->pImageDefault = configNonSharedProperties.pImageDefault;
        this->pImageHover = configNonSharedProperties.pImageHover;
        this->pImageDown = configNonSharedProperties.pImageDown;
        this->pColorBackgroundDefault = configNonSharedProperties.pColorBackgroundDefault;
        this->pColorBackgroundHover = configNonSharedProperties.pColorBackgroundHover;
        this->pColorBackgroundDown = configNonSharedProperties.pColorBackgroundDown;
        this->imagePosX = configNonSharedProperties.imagePosX;
        this->imagePosY = configNonSharedProperties.imagePosY;
        this->imageWidth = configNonSharedProperties.imageWidth;
        this->imageHeight = configNonSharedProperties.imageHeight;
        this->isCentering = configNonSharedProperties.centering;
        this->skipHoverAnimationState = configNonSharedProperties.skipHoverAnimationState;
        this->skipDownAnimationState = configNonSharedProperties.skipDownAnimationState;

        // Install the subclass callback.
        if (!this->installSubclass(hWnd))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyImageButton\" | FUNC: \"setSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyImageButton::refresh(HWND hWnd)
{
    if (!this->startAnimation(hWnd, this->currentAnimationState))
    {
        WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyImageButton\" | FUNC: \"refresh()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class MyRadioButton definitions:
// Destructor:
MyRadioButton::~MyRadioButton()
{
    // Note:
    // If the animation manager created these variables no longer exists,
    // No need to perform any cleanup as the COM objects already released along with its animation manager.
    if (!MyRadioButton::pAnimationManager)
        return;

    // Release the COM objects (Animation variables).
    for (int i = 0; i < 3; i++)
    {
        if (this->pAnimationVariableButtonPrimaryRGB[i])
            this->pAnimationVariableButtonPrimaryRGB[i]->Release();
    }
    for (int i = 0; i < 3; i++)
    {
        if (this->pAnimationVariableButtonSecondaryRGB[i])
            this->pAnimationVariableButtonSecondaryRGB[i]->Release();
    }
    for (int i = 0; i < 3; i++)
    {
        if (this->pAnimationVariableButtonBorderRGB[i])
            this->pAnimationVariableButtonBorderRGB[i]->Release();
    }
}
// Private member function(s) [INTERNAL FUNCTIONS]:
bool MyRadioButton::initializeAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        // Create the animation variable.
        hr = MyRadioButton::pAnimationManager->CreateAnimationVariable(initialValue, &pAnimationVariable);
        if (FAILED(hr))
        {
            error_message = L"Failed to create the animation variable.";
            break;
        }

        // Set lower bound value.
        hr = pAnimationVariable->SetLowerBound(lowerBound);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable lower bound value.";
            break;
        }

        // Set upper bound value.
        hr = pAnimationVariable->SetUpperBound(upperBound);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable upper bound value.";
            break;
        }

        hr = pAnimationVariable->SetRoundingMode(roundingMode);
        if (FAILED(hr))
        {
            error_message = L"Failed to set the animation variable rounding mode.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"initializeAnimationVariable()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyRadioButton::installSubclass(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Install the subclass callback.
        if (!SetWindowSubclass(hWnd, &MyRadioButton::scMyRadioButton, NULL, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = L"Failed to install the subclass callback \"SetWindowSubclass()\".";
            break;
        }

        // Initialize the animation variables.
        error_message = L"Failed to initialize the animation variables.";
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonPrimaryRGB[0],
                                               this->pColorRadioButtonPrimaryDefault->getGDIPColor().GetRed(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonPrimaryRGB[1],
                                               this->pColorRadioButtonPrimaryDefault->getGDIPColor().GetGreen(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonPrimaryRGB[2],
                                               this->pColorRadioButtonPrimaryDefault->getGDIPColor().GetBlue(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonSecondaryRGB[0],
                                               this->pColorRadioButtonSecondaryDefault->getGDIPColor().GetRed(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonSecondaryRGB[1],
                                               this->pColorRadioButtonSecondaryDefault->getGDIPColor().GetGreen(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonSecondaryRGB[2],
                                               this->pColorRadioButtonSecondaryDefault->getGDIPColor().GetBlue(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonBorderRGB[0],
                                               this->pColorRadioButtonBorderDefault->getGDIPColor().GetRed(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonBorderRGB[1],
                                               this->pColorRadioButtonBorderDefault->getGDIPColor().GetGreen(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        if (!this->initializeAnimationVariable(this->pAnimationVariableButtonBorderRGB[2],
                                               this->pColorRadioButtonBorderDefault->getGDIPColor().GetBlue(),
                                               0.0, 255.0, UI_ANIMATION_ROUNDING_FLOOR))
            break;
        error_message = L"";

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"installSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyRadioButton::startAnimation(HWND hWnd, MyRadioButton::ButtonAnimationState animationState)
{
    HRESULT hr;

    // Prepare the necessary pointers for the animation.
    IUIAnimationStoryboard *p_storyboard = nullptr;                                             // Storyboard pointer.
    IUIAnimationTransition *p_transition_button_primary_rgb[3] = {nullptr, nullptr, nullptr};   // Transition pointers for the button RGB values (Primary).
    IUIAnimationTransition *p_transition_button_secondary_rgb[3] = {nullptr, nullptr, nullptr}; // Transition pointers for the button RGB values (Secondary).
    IUIAnimationTransition *p_transition_button_border_rgb[3] = {nullptr, nullptr, nullptr};    // Transition pointers for the button border RGB values.

    switch (animationState)
    {
    case Default:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyRadioButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Default)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Default)";
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonBorderDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonBorderDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonBorderDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Default)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[0], p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[1], p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[2], p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[0], p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[1], p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[2], p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Default)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Default)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Default;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case Hover:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyRadioButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Hover)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Hover)";
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonBorderHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonBorderHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorRadioButtonBorderHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Hover)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[0], p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[1], p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[2], p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[0], p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[1], p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[2], p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Hover)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Hover)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Hover;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case Down:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyRadioButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Down)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Down)";
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryDown->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryDown->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonPrimaryDown->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryDown->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryDown->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonSecondaryDown->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonBorderDown->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonBorderDown->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorRadioButtonBorderDown->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Down)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[0], p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[1], p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[2], p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[0], p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[1], p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[2], p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Down)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Down)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Down;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case Selected:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyRadioButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: Selected)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: Selected)";
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDefault->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDefault->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDefault->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: Selected)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[0], p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[1], p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[2], p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[0], p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[1], p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[2], p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: Selected)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: Selected)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Selected;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case SelectedHover:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyRadioButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: SelectedHover)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: SelectedHover)";
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderHover->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderHover->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;

            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::hoverAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderHover->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: SelectedHover)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[0], p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[1], p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[2], p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[0], p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[1], p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[2], p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: SelectedHover)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: SelectedHover)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::SelectedHover;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case SelectedDown:
    {
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyRadioButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: SelectedDown)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: SelectedDown)";
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDown->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDown->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDown->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDown->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDown->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDown->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDown->getGDIPColor().GetRed(), 0.5, 0.5, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDown->getGDIPColor().GetGreen(), 0.5, 0.5, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::downAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDown->getGDIPColor().GetBlue(), 0.5, 0.5, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: SelectedDown)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[0], p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[1], p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[2], p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[0], p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[1], p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[2], p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: SelectedDown)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: SelectedDown)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::SelectedDown;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }

    case SelectedFlash:
    {
        // Prepare the extra necessary pointers for the animation.
        IUIAnimationTransition *p_reverse_transition_button_primary_rgb[3] = {nullptr, nullptr, nullptr};
        IUIAnimationTransition *p_reverse_transition_button_secondary_rgb[3] = {nullptr, nullptr, nullptr};
        IUIAnimationTransition *p_reverse_transition_button_border_rgb[3] = {nullptr, nullptr, nullptr};

        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Create the storyboard.
            hr = MyRadioButton::pAnimationManager->CreateStoryboard(&p_storyboard);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the storyboard. (State: SelectedFlash)";
                break;
            }

            // Create the transition(s) for the animation variable(s).
            error_message = L"Failed to create the transition(s) for the animation variable(s). (State: SelectedFlash)";
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDown->getGDIPColor().GetRed(), 0.8, 0.2, &p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDown->getGDIPColor().GetGreen(), 0.8, 0.2, &p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDown->getGDIPColor().GetBlue(), 0.8, 0.2, &p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDown->getGDIPColor().GetRed(), 0.8, 0.2, &p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDown->getGDIPColor().GetGreen(), 0.8, 0.2, &p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDown->getGDIPColor().GetBlue(), 0.8, 0.2, &p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDown->getGDIPColor().GetRed(), 0.8, 0.2, &p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDown->getGDIPColor().GetGreen(), 0.8, 0.2, &p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDown->getGDIPColor().GetBlue(), 0.8, 0.2, &p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDefault->getGDIPColor().GetRed(), 0.8, 0.2, &p_reverse_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDefault->getGDIPColor().GetGreen(), 0.8, 0.2, &p_reverse_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonPrimaryDefault->getGDIPColor().GetBlue(), 0.8, 0.2, &p_reverse_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDefault->getGDIPColor().GetRed(), 0.8, 0.2, &p_reverse_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDefault->getGDIPColor().GetGreen(), 0.8, 0.2, &p_reverse_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonSecondaryDefault->getGDIPColor().GetBlue(), 0.8, 0.2, &p_reverse_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDefault->getGDIPColor().GetRed(), 0.8, 0.2, &p_reverse_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDefault->getGDIPColor().GetGreen(), 0.8, 0.2, &p_reverse_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = MyRadioButton::pTransitionLibrary->CreateAccelerateDecelerateTransition(MyRadioButton::flashAnimationDuration, MyRadioButton::pColorSelectedRadioButtonBorderDefault->getGDIPColor().GetBlue(), 0.8, 0.2, &p_reverse_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Add the transition(s) to the storyboard.
            error_message = L"Failed to add the transition(s) to the storyboard. (State: SelectedFlash)";
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[0], p_transition_button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[1], p_transition_button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonPrimaryRGB[2], p_transition_button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[0], p_transition_button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[1], p_transition_button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonSecondaryRGB[2], p_transition_button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[0], p_transition_button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[1], p_transition_button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransition(this->pAnimationVariableButtonBorderRGB[2], p_transition_button_border_rgb[2]);
            if (FAILED(hr))
                break;
            // Add keyframe at the end of the previous transitions.
            UI_ANIMATION_KEYFRAME keyframe_end;
            hr = p_storyboard->AddKeyframeAfterTransition(p_transition_button_border_rgb[2], &keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonPrimaryRGB[0], p_reverse_transition_button_primary_rgb[0], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonPrimaryRGB[1], p_reverse_transition_button_primary_rgb[1], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonPrimaryRGB[2], p_reverse_transition_button_primary_rgb[2], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonSecondaryRGB[0], p_reverse_transition_button_secondary_rgb[0], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonSecondaryRGB[1], p_reverse_transition_button_secondary_rgb[1], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonSecondaryRGB[2], p_reverse_transition_button_secondary_rgb[2], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonBorderRGB[0], p_reverse_transition_button_border_rgb[0], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonBorderRGB[1], p_reverse_transition_button_border_rgb[1], keyframe_end);
            if (FAILED(hr))
                break;
            hr = p_storyboard->AddTransitionAtKeyframe(this->pAnimationVariableButtonBorderRGB[2], p_reverse_transition_button_border_rgb[2], keyframe_end);
            if (FAILED(hr))
                break;
            error_message = L"";

            // Update time for the animation timer.
            UI_ANIMATION_SECONDS seconds_now;
            hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to retrieve the current time. (State: SelectedFlash)";
                break;
            }

            // Schedule the storyboard.
            hr = p_storyboard->Schedule(seconds_now);
            if (FAILED(hr))
            {
                error_message = L"Failed to schedule the storyboard. (State: SelectedFlash)";
                break;
            }

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_primary_rgb[i])
                {
                    p_reverse_transition_button_primary_rgb[i]->Release();
                    p_reverse_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_secondary_rgb[i])
                {
                    p_reverse_transition_button_secondary_rgb[i]->Release();
                    p_reverse_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_border_rgb[i])
                {
                    p_reverse_transition_button_border_rgb[i]->Release();
                    p_reverse_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            // Set the invalidation timer.
            SetTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE, 16, (TIMERPROC)NULL);

            // Update button state.
            this->currentAnimationState = ButtonAnimationState::Selected;

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"startAnimation()\"]", MyLogType::Error);

            // Perform necesscary clean up operations.
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_primary_rgb[i])
                {
                    p_transition_button_primary_rgb[i]->Release();
                    p_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_secondary_rgb[i])
                {
                    p_transition_button_secondary_rgb[i]->Release();
                    p_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_transition_button_border_rgb[i])
                {
                    p_transition_button_border_rgb[i]->Release();
                    p_transition_button_border_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_primary_rgb[i])
                {
                    p_reverse_transition_button_primary_rgb[i]->Release();
                    p_reverse_transition_button_primary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_secondary_rgb[i])
                {
                    p_reverse_transition_button_secondary_rgb[i]->Release();
                    p_reverse_transition_button_secondary_rgb[i] = nullptr;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (p_reverse_transition_button_border_rgb[i])
                {
                    p_reverse_transition_button_border_rgb[i]->Release();
                    p_reverse_transition_button_border_rgb[i] = nullptr;
                }
            }
            if (p_storyboard)
            {
                p_storyboard->Release();
                p_storyboard = nullptr;
            }

            break;
        }

        return true;
    }
    }

    return false;
}
// Private member function(s) [SUBCLASS CALLBACK FUNCTIONS]:
LRESULT CALLBACK MyRadioButton::scMyRadioButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the poiner from dwRefData and use it to access non-static members.
    MyRadioButton *p_this = reinterpret_cast<MyRadioButton *>(dwRefData);

    // Check if the enter key is pressed and the button has focus.
    if (*MyRadioButton::pEnterKeyToggle && GetFocus() == hWnd)
    {
        // Triggering the appropriate animation depends on the current animation state.
        if (!p_this->isHoverState && !p_this->isDownState)
        {
            if (!p_this->isSelected)
            {
                p_this->updateSelectionState(hWnd, true);
                if (!p_this->pRadioGroup)
                {
                    WriteLog(L"The radio button doesn't belong to any radio group.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"!p_this->isSelected\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }

                if (!p_this->pRadioGroup->updateRadioState(GetDlgCtrlID(hWnd)))
                {
                    WriteLog(L"Failed to update the radio group state.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"!p_this->isSelected\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }

                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Selected))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"!p_this->isSelected\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }

                // Trigger the button's WM_COMMAND message.
                SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), NULL);
            }
            else
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::SelectedFlash))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"!p_this->isHoverState && !p_this->isDownState\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }
        }
    }
    // Check if the tab key is pressed and the button has focus.
    else if (GetFocus() == hWnd && GetKeyState(VK_TAB) & 0x8000)
    {
        // Perform redraw on the button to draw the focus border.
        InvalidateRect(hWnd, NULL, FALSE);
    }

    switch (uMsg)
    {
    // Remove the window subclass callback.
    case WM_NCDESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyRadioButton::scMyRadioButton, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Override paint messages.
    case WM_PAINT:
    {
        HRESULT hr;
        PAINTSTRUCT ps;
        HDC hdc = NULL;
        HDC mem_hdc = NULL;
        HBITMAP bitmap = NULL;
        HBITMAP bitmap_old = NULL;
        bool is_paint_begined = false;
        bool is_bitmap_created = false;
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get the animation values.
            INT32 button_primary_rgb[3] = {0, 0, 0};
            INT32 button_secondary_rgb[3] = {0, 0, 0};
            INT32 button_border_rgb[3] = {0, 0, 0};
            error_message = L"Failed to retrieve the animation values.";
            hr = p_this->pAnimationVariableButtonPrimaryRGB[0]->GetIntegerValue(&button_primary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonPrimaryRGB[1]->GetIntegerValue(&button_primary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonPrimaryRGB[2]->GetIntegerValue(&button_primary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonSecondaryRGB[0]->GetIntegerValue(&button_secondary_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonSecondaryRGB[1]->GetIntegerValue(&button_secondary_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonSecondaryRGB[2]->GetIntegerValue(&button_secondary_rgb[2]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonBorderRGB[0]->GetIntegerValue(&button_border_rgb[0]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonBorderRGB[1]->GetIntegerValue(&button_border_rgb[1]);
            if (FAILED(hr))
                break;
            hr = p_this->pAnimationVariableButtonBorderRGB[2]->GetIntegerValue(&button_border_rgb[2]);
            if (FAILED(hr))
                break;
            Gdiplus::Color button_primary_color(255, button_primary_rgb[0], button_primary_rgb[1], button_primary_rgb[2]);
            Gdiplus::Color button_secondary_color(255, button_secondary_rgb[0], button_secondary_rgb[1], button_secondary_rgb[2]);
            Gdiplus::Color button_border_color(255, button_border_rgb[0], button_border_rgb[1], button_border_rgb[2]);
            error_message = L"";

            // Get the button's client rects.
            RECT rect_button;
            Gdiplus::Rect gdip_rect_button;
            if (!GetClientRect(hWnd, &rect_button))
            {
                error_message = L"Failed to get the client rects.";
                break;
            }
            gdip_rect_button = Gdiplus::Rect(rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);

            // Get the button's text length.
            INT button_text_length = GetWindowTextLengthW(hWnd);
            SetLastError(NULL);
            if (!button_text_length && GetLastError() != NULL)
            {
                error_message = L"Failed to get the button text length.";
                break;
            }

            // Get the button's text.
            WCHAR *text_buffer = new WCHAR[button_text_length + 1];
            if (!GetWindowTextW(hWnd, text_buffer, button_text_length + 1) && button_text_length != NULL)
            {
                error_message = L"Failed to get the button text.";
                delete[] text_buffer;
                break;
            }
            std::wstring button_text(text_buffer);
            delete[] text_buffer;

            // Begin the paintings.
            hdc = BeginPaint(hWnd, &ps);
            if (!hdc)
            {
                error_message = L"Failed to execute \"BeginPaint()\".";
                break;
            }
            is_paint_begined = true;

            // Create memory device context and bitmap object for double buffering.
            mem_hdc = CreateCompatibleDC(hdc);
            if (!mem_hdc)
            {
                error_message = L"Failed to create the compatible memory device context.";
                break;
            }
            bitmap = CreateCompatibleBitmap(hdc, rect_button.right - rect_button.left, rect_button.bottom - rect_button.top);
            if (!bitmap)
            {
                error_message = L"Failed to create the compatible bitmap object.";
                break;
            }
            is_bitmap_created = true;
            bitmap_old = reinterpret_cast<HBITMAP>(SelectObject(mem_hdc, bitmap));
            if (!bitmap_old)
            {
                error_message = L"Failed to select the bitmap object.";
                break;
            }

            // Create Gdiplus::Graphics object for GDI+ drawing operations.
            Gdiplus::Graphics gdip_graphics(mem_hdc);

            // Set GDI+ smoothing mode.
            if (gdip_graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality))
            {
                error_message = L"Failed to set GDI+ smoothing mode.";
                break;
            }

            // Set background mode.
            if (!SetBkMode(mem_hdc, TRANSPARENT))
            {
                error_message = L"Failed to set the background mode.";
                break;
            }

            // Select font object.
            if (!SelectObject(mem_hdc, reinterpret_cast<HFONT>(MyRadioButton::pFontDefault->getHFONT())))
            {
                error_message = L"Failed to select the font object.";
                break;
            }

            // Set text color.
            if (SetTextColor(mem_hdc, (p_this->isHoverState || p_this->isDownState ? MyRadioButton::pColorRadioButtonTextHighlight->getCOLORREF() : MyRadioButton::pColorRadioButtonTextDefault->getCOLORREF())) == CLR_INVALID)
            {
                error_message = L"Failed to set the text color.";
                break;
            }

            // Begin painting to the memory device context.
            {
                // Draw the button's background.
                if (!MyDraw_FillRect(&gdip_graphics, gdip_rect_button, &MyRadioButton::pColorBackground->getGDIPColor()))
                {
                    error_message = L"Failed to draw the button's background.";
                    break;
                }

                // Draw the button secondary part.
                INT circle1_diameter = gdip_rect_button.GetBottom() / 2;
                INT circle1_posx = 11;
                INT circle1_posy = (gdip_rect_button.GetBottom() / 2) - (circle1_diameter / 2);
                if (!MyDraw_FillEllipse(&gdip_graphics, gdip_rect_button, circle1_diameter, circle1_posx, circle1_posy, &button_secondary_color, (GetFocus() == hWnd ? &MyRadioButton::pColorFocus->getGDIPColor() : &button_border_color)))
                {
                    error_message = L"Failed to draw the button secondary part.";
                    break;
                }

                // Draw the button primary part.
                INT circle2_diameter = circle1_diameter - 6;
                INT circle2_posx = circle1_posx + 3;
                INT circle2_posy = circle1_posy + 3;

                if (!MyDraw_FillEllipse(&gdip_graphics, gdip_rect_button, circle2_diameter, circle2_posx, circle2_posy, &button_primary_color))
                {
                    error_message = L"Failed to draw the button primary part.";
                    break;
                }

                // Draw the button's text.
                RECT rect_text_area{rect_button.left + circle1_diameter + 12 + 5, rect_button.top, rect_button.right - 10, rect_button.bottom};
                DrawTextW(mem_hdc, button_text.c_str(), -1, &rect_text_area, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }

            // Draw contents from memory device context to target device context.
            if (!BitBlt(hdc, 0, 0, rect_button.right - rect_button.left, rect_button.bottom - rect_button.top, mem_hdc, 0, 0, SRCCOPY))
            {
                error_message = L"Failed to draw contents from memory device context to target device context.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (is_paint_begined)
        {
            // Delete the bitmap object.
            if (is_bitmap_created)
            {
                if (!SelectObject(mem_hdc, bitmap_old))
                {
                    WriteLog(L"Failed to select the bitmap object.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
                if (!DeleteObject(bitmap))
                {
                    WriteLog(L"Failed to delete the bitmap object.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
                bitmap = NULL;
                bitmap_old = NULL;
            }

            // Delete the memory device context.
            if (mem_hdc)
            {
                if (!DeleteDC(mem_hdc))
                {
                    WriteLog(L"Failed to delete the memory device context.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
                mem_hdc = NULL;
            }

            // End the paintings.
            EndPaint(hWnd, &ps);
            return 0;
        }

        break;
    }

    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Process the invalidate timer to update the animation manager.
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MyRadioButton::IDT_ANIMATION_INVALIDATE:
        {
            bool are_all_operation_success = false;
            std::wstring error_message = L"";
            while (!are_all_operation_success)
            {
                HRESULT hr;
                // Get the current time.
                UI_ANIMATION_SECONDS seconds_now;
                hr = MyRadioButton::pAnimationTimer->GetTime(&seconds_now);
                if (FAILED(hr))
                {
                    error_message = L"Failed to retrieve the current time.";
                    break;
                }

                // Update the animation manager.
                hr = MyRadioButton::pAnimationManager->Update(seconds_now);
                if (FAILED(hr))
                {
                    error_message = L"Failed to update the animation manager.";
                    break;
                }

                // Invalidate the control.
                InvalidateRect(hWnd, NULL, TRUE);

                // Get the animation manager's status.
                UI_ANIMATION_MANAGER_STATUS status;
                hr = MyRadioButton::pAnimationManager->GetStatus(&status);
                if (FAILED(hr))
                {
                    error_message = L"Failed to get the animation manager's status.";
                    break;
                }

                // Kill the timer if the animation manager is idle.
                if (status == UI_ANIMATION_MANAGER_IDLE)
                {
                    if (!KillTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE))
                    {
                        error_message = L"Failed to kill the timer.";
                        break;
                    }
                }

                are_all_operation_success = true;
            }

            if (!are_all_operation_success)
            {
                // Kill the timer.
                if (!KillTimer(hWnd, MyRadioButton::IDT_ANIMATION_INVALIDATE))
                {
                    WriteLog(L"Failed to kill the timer.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_TIMER/IDT_ANIMATION_INVALIDATE\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }

                WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_TIMER/IDT_ANIMATION_INVALIDATE\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);

                return 0;
            }

            return 0;
        }

        default:
        {
            WriteLog(L"Unprocessed timer message.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_TIMER\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
            break;
        }
        }

        break;
    }

    // Process the left-button mouse click (down) to trigger the appropriate animation and action(s).
    case WM_LBUTTONDOWN:
    {
        if (!p_this->isDownState && p_this->isHoverState)
        {
            p_this->isDownState = true;

            if (!p_this->isSelected)
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Down))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_LBUTTONDOWN\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }
            else
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::SelectedDown))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_LBUTTONDOWN\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }

            return 0;
        }

        break;
    }

    // Process the left-button mouse click (up) to trigger the appropriate animation.
    case WM_LBUTTONUP:
    {
        if (p_this->isDownState)
        {
            p_this->isDownState = false;

            if (p_this->isHoverState)
            {
                if (!p_this->isSelected)
                {
                    p_this->updateSelectionState(hWnd, true);
                    if (!p_this->pRadioGroup)
                    {
                        WriteLog(L"The radio button doesn't belong to any radio group.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                    }

                    if (!p_this->pRadioGroup->updateRadioState(GetDlgCtrlID(hWnd)))
                    {
                        WriteLog(L"Failed to update the radio group state.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                    }

                    // Trigger the button's WM_COMMAND message.
                    SendMessageW(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), NULL);
                }

                if (!p_this->startAnimation(hWnd, ButtonAnimationState::SelectedHover))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_LBUTTONUP\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }
        }

        break;
    }

    // Process the mouse leave message to trigger the appropriate animation.
    case WM_MOUSELEAVE:
    {
        if (p_this->isHoverState)
        {
            p_this->isHoverState = false;
            p_this->isDownState = false;

            if (!p_this->isSelected)
            {
                if (!p_this->startAnimation(hWnd, Default))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_MOUSELEAVE\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }
            else
            {
                if (!p_this->startAnimation(hWnd, Selected))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_MOUSELEAVE\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }

            return 0;
        }

        break;
    }

    // Process the mouse mouse message to trigger the appropriate animation.
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
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::Hover))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_MOUSEMOVE\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }
            else
            {
                if (!p_this->startAnimation(hWnd, ButtonAnimationState::SelectedHover))
                {
                    WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | MESSAGE: \"WM_MOUSEMOVE\" | CALLBACK: \"scMyRadioButton()\"]", MyLogType::Error);
                }
            }

            return 0;
        }

        break;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
// Public static member function(s) [INITIALIZATION FUNCTIONS]:
bool MyRadioButton::initialize(MyRadioButtonInitializeConfig initializeConfig)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the initialization structure is valid.
        if (!initializeConfig.isValid())
        {
            error_message = L"The initialization structure is invalid.";
            break;
        }

        // Set the pointers.
        MyRadioButton::pEnterKeyToggle = initializeConfig.pEnterKeyToggle;
        MyRadioButton::pAnimationManager = std::ref(*initializeConfig.ppAnimationManager);
        MyRadioButton::pAnimationTimer = std::ref(*initializeConfig.ppAnimationTimer);
        MyRadioButton::pTransitionLibrary = std::ref(*initializeConfig.ppTransitionLibrary);

        // Update initialization state.
        MyRadioButton::isInitialized = true;

        // Update class ready state.
        if (MyRadioButton::isSharedPropertiesSet)
            MyRadioButton::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"initialize()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyRadioButton::setSharedProperties(MyRadioButtonSharedPropertiesConfig configSharedProperties)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the shared properties configuration structure is valid.
        if (!configSharedProperties.isValid())
        {
            error_message = L"The shared properties configuration structure is invalid.";
            break;
        }

        // Set the pointers.
        MyRadioButton::pColorRadioButtonPrimaryDefault = configSharedProperties.pColorRadioButtonPrimaryDefault;
        MyRadioButton::pColorRadioButtonPrimaryHover = configSharedProperties.pColorRadioButtonPrimaryHover;
        MyRadioButton::pColorRadioButtonPrimaryDown = configSharedProperties.pColorRadioButtonPrimaryDown;
        MyRadioButton::pColorRadioButtonSecondaryDefault = configSharedProperties.pColorRadioButtonSecondaryDefault;
        MyRadioButton::pColorRadioButtonSecondaryHover = configSharedProperties.pColorRadioButtonSecondaryHover;
        MyRadioButton::pColorRadioButtonSecondaryDown = configSharedProperties.pColorRadioButtonSecondaryDown;
        MyRadioButton::pColorRadioButtonBorderDefault = configSharedProperties.pColorRadioButtonBorderDefault;
        MyRadioButton::pColorRadioButtonBorderHover = configSharedProperties.pColorRadioButtonBorderHover;
        MyRadioButton::pColorRadioButtonBorderDown = configSharedProperties.pColorRadioButtonBorderDown;
        MyRadioButton::pColorSelectedRadioButtonPrimaryDefault = configSharedProperties.pColorSelectedRadioButtonPrimaryDefault;
        MyRadioButton::pColorSelectedRadioButtonPrimaryHover = configSharedProperties.pColorSelectedRadioButtonPrimaryHover;
        MyRadioButton::pColorSelectedRadioButtonPrimaryDown = configSharedProperties.pColorSelectedRadioButtonPrimaryDown;
        MyRadioButton::pColorSelectedRadioButtonSecondaryDefault = configSharedProperties.pColorSelectedRadioButtonSecondaryDefault;
        MyRadioButton::pColorSelectedRadioButtonSecondaryHover = configSharedProperties.pColorSelectedRadioButtonSecondaryHover;
        MyRadioButton::pColorSelectedRadioButtonSecondaryDown = configSharedProperties.pColorSelectedRadioButtonSecondaryDown;
        MyRadioButton::pColorSelectedRadioButtonBorderDefault = configSharedProperties.pColorSelectedRadioButtonBorderDefault;
        MyRadioButton::pColorSelectedRadioButtonBorderHover = configSharedProperties.pColorSelectedRadioButtonBorderHover;
        MyRadioButton::pColorSelectedRadioButtonBorderDown = configSharedProperties.pColorSelectedRadioButtonBorderDown;
        MyRadioButton::pColorRadioButtonTextDefault = configSharedProperties.pColorRadioButtonTextDefault;
        MyRadioButton::pColorRadioButtonTextHighlight = configSharedProperties.pColorRadioButtonTextHighlight;
        MyRadioButton::pColorBackground = configSharedProperties.pColorBackground;
        MyRadioButton::pColorFocus = configSharedProperties.pColorFocus;
        MyRadioButton::pFontDefault = configSharedProperties.pFontDefault;

        // Update initialization state.
        MyRadioButton::isSharedPropertiesSet = true;

        // Update class ready state.
        if (MyRadioButton::isInitialized)
            MyRadioButton::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"setSharedProperties()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
bool MyRadioButton::isSubclassed(HWND hWnd, void **ppSubclass)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyRadioButton::scMyRadioButton, NULL, &reference_data);
    if (is_subclassed && ppSubclass)
    {
        *ppSubclass = reinterpret_cast<MyRadioButton *>(reference_data);
    }

    return is_subclassed;
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyRadioButton::setSubclass(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the class is initialized.
        if (!MyRadioButton::isReady)
        {
            error_message = L"The class is not initialized.";
            break;
        }

        // Install the subclass callback.
        if (!this->installSubclass(hWnd))
        {
            error_message = L"Failed to install the subclass callback.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyRadioButton\" | FUNC: \"setSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
void MyRadioButton::updateSelectionState(HWND hWnd, bool state)
{
    // Ignore if the requested state is the same as the current state.
    if (this->isSelected == state)
        return;

    // Update selection state and trigger the appropriate animation.
    if (!state)
    {
        this->isSelected = false;

        if (!this->startAnimation(hWnd, Default))
        {
            WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | FUNC: \"updateSelectionState()\"]", MyLogType::Error);
        }
    }
    else
    {
        this->isSelected = true;
    }
}
bool MyRadioButton::refresh(HWND hWnd)
{
    if (!this->startAnimation(hWnd, this->currentAnimationState))
    {
        WriteLog(L"Failed to start the button animation.", L" [CLASS: \"MyRadioButton\" | FUNC: \"refresh()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class MyRadioGroup definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyRadioGroup::addRadioButton(MyWindow *pWindow)
{
    // Check if the pointer is null.
    if (!pWindow)
    {
        WriteLog(L"The pointer to the radio button control is null.", L" [CLASS: \"MyRadioGroup\" | FUNC: \"addRadioButton()\"]", MyLogType::Error);
        return false;
    }

    // Check if the window is compatible.
    if (pWindow->getWindowType() != MyWindowType::RadioButton || !pWindow->isManagedWindow())
    {
        WriteLog(L"The window type is incompatible.", L" [CLASS: \"MyRadioGroup\" | FUNC: \"addRadioButton()\"]", MyLogType::Error);
        return false;
    }

    // Check if the window object is valid.
    if (!IsWindow(pWindow->hWnd))
    {
        WriteLog(L"The radio button control is not created yet.", L" [CLASS: \"MyRadioGroup\" | FUNC: \"addRadioButton()\"]", MyLogType::Error);
        return false;
    }

    // Get the associated pointer to the subclass object.
    void *p_subclass = pWindow->getSubclassPointer();
    MyRadioButton *p_casted_subclass = static_cast<MyRadioButton *>(p_subclass);

    // Check if the pointer to subclass object is null.
    if (!p_casted_subclass)
    {
        WriteLog(L"The pointer to the associated subclass instance is null.", L" [CLASS: \"MyRadioGroup\" | FUNC: \"addRadioButton()\"]", MyLogType::Error);
        return false;
    }

    // Link the subclass object with the radio group.
    p_casted_subclass->pRadioGroup = this;

    // Add the button to the map.
    this->mapButtons.insert(std::make_pair(GetDlgCtrlID(pWindow->hWnd), std::make_pair(pWindow->hWnd, p_casted_subclass)));

    return true;
}
INT MyRadioGroup::getRadioState()
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
                {
                    button_2.second.second->updateSelectionState(button_2.second.first, false);
                }
            }

            return true;
        }
    }

    WriteLog(L"This button ID doesn't exists in the radio group.", L" [CLASS: \"MyRadioGroup\" | FUNC: \"updateRadioState()\"]", MyLogType::Error);

    return false;
}

/// @class MyEdit definitions:
// Private member function(s) [INTERNAL FUNCTIONS]:
bool MyEdit::installEditSubclass(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Install the subclass callback.
        if (!SetWindowSubclass(hWnd, &MyEdit::scMyEdit, NULL, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = L"Failed to install the subclass callback \"SetWindowSubclass()\".";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyEdit\" | FUNC: \"installEditSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyEdit::installStaticSubclass(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Install the subclass callback.
        if (!SetWindowSubclass(hWnd, &MyEdit::scMyEditStatic, NULL, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = L"Failed to install the subclass callback \"SetWindowSubclass()\".";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyEdit\" | FUNC: \"installStaticSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
// Private member function(s) [SUBCLASS CALLBACK FUNCTIONS]:
LRESULT CALLBACK MyEdit::scMyEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the poiner from dwRefData and use it to access non-static members.
    MyEdit *p_this = reinterpret_cast<MyEdit *>(dwRefData);

    // Check if the enter key is pressed and the editbox has focus.
    if (*MyEdit::pEnterKeyToggle && GetFocus() == hWnd)
    {
        // Trigger the associated WM_COMMAND.
        if (p_this->associatedWMCommandMessageID)
            SendMessageW(GetParent(hWnd), WM_COMMAND, p_this->associatedWMCommandMessageID, NULL);
    }

    switch (uMsg)
    {
    // Destroy editbox-associated static window and remove the window subclass callback.
    case WM_NCDESTROY:
    {
        // Destroy the editbox-associated static window.
        if (!DestroyWindow(p_this->hWndStatic))
        {
            WriteLog(L"Failed to destroy the editbox-associated static window.", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyEdit()\"]", MyLogType::Error);
        }

        // Remove the window subclass callback.
        if (!RemoveWindowSubclass(hWnd, &MyEdit::scMyEdit, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyEdit()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;

    // Process the tab-stop event.
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        // Capture the VK_TAB key down event to handle tab stop messages.
        case VK_TAB:
        {
            // Check if the editbox want to skip the tab stop event.
            if (p_this->skipTabStop)
                break;

            // Set focus to the next tab stop.
            HWND next_focus_window = GetNextDlgTabItem(*MyEdit::pMainWindowHandle, hWnd, static_cast<INT>(GetKeyState(VK_SHIFT) & 0x8000));
            SetFocus(next_focus_window);

            break;
        }

        default:
            break;
        }

        break;
    }

    // Invalidate the editbox border upon focus.
    case WM_SETFOCUS:
    {
        HRGN hrgn_top = NULL, hrgn_bottom = NULL, hrgn_left = NULL, hrgn_right = NULL, hrgn_combo = NULL;

        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get static's client rect.
            RECT rect_static;
            if (!GetClientRect(p_this->hWndStatic, &rect_static))
            {
                error_message = L"Failed to retrieve the static's client rect.";
                break;
            }

            // Create the region object for redrawing.
            hrgn_top = CreateRectRgn(rect_static.left, rect_static.top, rect_static.right, rect_static.top + 2);
            hrgn_bottom = CreateRectRgn(rect_static.left, rect_static.bottom - 2, rect_static.right, rect_static.bottom);
            hrgn_left = CreateRectRgn(rect_static.left, rect_static.top, rect_static.left + 2, rect_static.bottom);
            hrgn_right = CreateRectRgn(rect_static.right - 2, rect_static.top, rect_static.right, rect_static.bottom);
            hrgn_combo = CreateRectRgn(0, 0, 0, 0);
            CombineRgn(hrgn_combo, hrgn_top, hrgn_bottom, RGN_OR);
            CombineRgn(hrgn_combo, hrgn_combo, hrgn_left, RGN_OR);
            CombineRgn(hrgn_combo, hrgn_combo, hrgn_right, RGN_OR);
            if (!hrgn_top || !hrgn_bottom || !hrgn_left || !hrgn_right || !hrgn_combo)
            {
                error_message = L"Failed to create the region object (HRGN).";
                break;
            }

            // Redraw the static window.
            RedrawWindow(p_this->hWndStatic, NULL, hrgn_combo, RDW_INVALIDATE | RDW_UPDATENOW);

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_SETFOCUS\" | CALLBACK: \"scMyEdit()\"]", MyLogType::Error);
        }

        // Perform necessary cleanup.
        if ((hrgn_top && !DeleteObject(hrgn_top)) ||
            (hrgn_bottom && !DeleteObject(hrgn_bottom)) ||
            (hrgn_left && !DeleteObject(hrgn_left)) ||
            (hrgn_right && !DeleteObject(hrgn_right)) ||
            (hrgn_combo && !DeleteObject(hrgn_combo)))
        {
            WriteLog(L"Failed to clean up the region objects (HRGN).", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_SETFOCUS\" | CALLBACK: \"scMyEdit()\"]", MyLogType::Error);
        }

        break;
    }

    // Invalidate the editbox border upon focus-lost.
    case WM_KILLFOCUS:
    {
        HRGN hrgn_top = NULL, hrgn_bottom = NULL, hrgn_left = NULL, hrgn_right = NULL, hrgn_combo = NULL;

        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get static's client rect.
            RECT rect_static;
            if (!GetClientRect(p_this->hWndStatic, &rect_static))
            {
                error_message = L"Failed to retrieve the static's client rect.";
                break;
            }

            // Create the region object for redrawing.
            hrgn_top = CreateRectRgn(rect_static.left, rect_static.top, rect_static.right, rect_static.top + 2);
            hrgn_bottom = CreateRectRgn(rect_static.left, rect_static.bottom - 2, rect_static.right, rect_static.bottom);
            hrgn_left = CreateRectRgn(rect_static.left, rect_static.top, rect_static.left + 2, rect_static.bottom);
            hrgn_right = CreateRectRgn(rect_static.right - 2, rect_static.top, rect_static.right, rect_static.bottom);
            hrgn_combo = CreateRectRgn(0, 0, 0, 0);
            CombineRgn(hrgn_combo, hrgn_top, hrgn_bottom, RGN_OR);
            CombineRgn(hrgn_combo, hrgn_combo, hrgn_left, RGN_OR);
            CombineRgn(hrgn_combo, hrgn_combo, hrgn_right, RGN_OR);
            if (!hrgn_top || !hrgn_bottom || !hrgn_left || !hrgn_right || !hrgn_combo)
            {
                error_message = L"Failed to create the region object (HRGN).";
                break;
            }

            // Redraw the static window.
            RedrawWindow(p_this->hWndStatic, NULL, hrgn_combo, RDW_INVALIDATE | RDW_UPDATENOW);

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_KILLFOCUS\" | CALLBACK: \"scMyEdit()\"]", MyLogType::Error);
        }

        // Perform necessary cleanup.
        if ((hrgn_top && !DeleteObject(hrgn_top)) ||
            (hrgn_bottom && !DeleteObject(hrgn_bottom)) ||
            (hrgn_left && !DeleteObject(hrgn_left)) ||
            (hrgn_right && !DeleteObject(hrgn_right)) ||
            (hrgn_combo && !DeleteObject(hrgn_combo)))
        {
            WriteLog(L"Failed to clean up the region objects (HRGN).", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_KILLFOCUS\" | CALLBACK: \"scMyEdit()\"]", MyLogType::Error);
        }

        break;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK MyEdit::scMyEditStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the poiner from dwRefData and use it to access non-static members.
    MyEdit *p_this = reinterpret_cast<MyEdit *>(dwRefData);

    switch (uMsg)
    {
    // Remove the window subclass callback.
    case WM_NCDESTROY:
    {
        // Remove the window subclass callback.
        if (!RemoveWindowSubclass(hWnd, &MyEdit::scMyEditStatic, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyEditStatic()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Override paint messages.
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = NULL;
        HDC mem_hdc = NULL;
        HBITMAP bitmap = NULL;
        HBITMAP bitmap_old = NULL;
        bool is_paint_begined = false;
        bool is_bitmap_created = false;
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get the editbox-associated static window client rects.
            RECT rect_static1, rect_static2;
            if (!GetClientRect(hWnd, &rect_static1))
            {
                error_message = L"Failed to retrieve the editbox-associated static window's client rects.";
                break;
            }
            rect_static2 = rect_static1;
            rect_static2.left += 1;
            rect_static2.right -= 1;
            rect_static2.top += 1;
            rect_static2.bottom -= 1;
            Gdiplus::Rect gdip_rect_static(rect_static1.top, rect_static1.left, rect_static1.right, rect_static1.bottom);

            // Begin the paintings.
            hdc = BeginPaint(hWnd, &ps);
            if (!hdc)
            {
                error_message = L"Failed to execute \"BeginPaint()\".";
                break;
            }
            is_paint_begined = true;

            // Create memory device context and bitmap object for double buffering.
            mem_hdc = CreateCompatibleDC(hdc);
            if (!mem_hdc)
            {
                error_message = L"Failed to create the compatible memory device context.";
                break;
            }
            bitmap = CreateCompatibleBitmap(hdc, rect_static1.right - rect_static1.left, rect_static1.bottom - rect_static1.top);
            if (!bitmap)
            {
                error_message = L"Failed to create the compatible bitmap object.";
                break;
            }
            is_bitmap_created = true;
            bitmap_old = reinterpret_cast<HBITMAP>(SelectObject(mem_hdc, bitmap));
            if (!bitmap_old)
            {
                error_message = L"Failed to select the bitmap object.";
                break;
            }

            // Begin painting to the memory device context.
            {
                FillRect(mem_hdc, &rect_static1, g_pUIElements->colors.editbox.getHBRUSH());
                FrameRect(mem_hdc, &rect_static1, g_pUIElements->colors.background.getHBRUSH());
                FrameRect(mem_hdc, &rect_static2, g_pUIElements->colors.editbox.getHBRUSH());
                Gdiplus::Graphics gdip_graphics(mem_hdc);
                gdip_graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
                DrawRoundRect(&gdip_graphics, gdip_rect_static, (GetFocus() == p_this->hWndEdit ? g_pUIElements->colors.editboxBorderSelected.getGDIPColor() : g_pUIElements->colors.editboxBorderDefault.getGDIPColor()), 2, 1);
            }

            // Draw contents from memory device context to target device context.
            if (!BitBlt(hdc, 0, 0, rect_static1.right - rect_static1.left, rect_static1.bottom - rect_static1.top, mem_hdc, 0, 0, SRCCOPY))
            {
                error_message = L"Failed to draw contents from memory device context to target device context.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyEditStatic()\"]", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (is_paint_begined)
        {
            // Delete the bitmap object.
            if (is_bitmap_created)
            {
                if (!SelectObject(mem_hdc, bitmap_old))
                {
                    WriteLog(L"Failed to select the bitmap object.", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyEditStatic()\"]", MyLogType::Error);
                }
                if (!DeleteObject(bitmap))
                {
                    WriteLog(L"Failed to delete the bitmap object.", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyEditStatic()\"]", MyLogType::Error);
                }
                bitmap = NULL;
                bitmap_old = NULL;
            }

            // Delete the memory device context.
            if (mem_hdc)
            {
                if (!DeleteDC(mem_hdc))
                {
                    WriteLog(L"Failed to delete the memory device context.", L" [CLASS: \"MyEdit\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyEditStatic()\"]", MyLogType::Error);
                }
                mem_hdc = NULL;
            }

            // End the paintings.
            EndPaint(hWnd, &ps);
            return 0;
        }

        break;
    }

    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
        return 1;
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
// Public static member function(s) [INITIALIZATION FUNCTIONS]:
bool MyEdit::initialize(MyEditInitializeConfig initializeConfig)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the initialization structure is valid.
        if (!initializeConfig.isValid())
        {
            error_message = L"The initialization structure is invalid.";
            break;
        }

        // Set the pointers.
        MyEdit::pEnterKeyToggle = initializeConfig.pEnterKeyToggle;
        MyEdit::pMainWindowHandle = initializeConfig.pMainWindowHandle;

        // Update initialization state.
        MyEdit::isInitialized = true;

        // Update class ready state.
        MyEdit::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyEdit\" | FUNC: \"initialize()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
bool MyEdit::isSubclassed(HWND hWnd, bool isStaticHandle, void **ppSubclass)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, (!isStaticHandle ? &MyEdit::scMyEdit : &MyEdit::scMyEditStatic), NULL, &reference_data);
    if (is_subclassed && ppSubclass)
    {
        *ppSubclass = reinterpret_cast<MyEdit *>(reference_data);
    }

    return is_subclassed;
}
// Public member function(s) [GENERAL FUNCTIONS]:
HWND MyEdit::getStaticHandle()
{
    return this->hWndStatic;
}
HWND &MyEdit::getStaticHandleRef()
{
    return this->hWndStatic;
}
bool MyEdit::setSubclass(HWND hWnd, MyEditNonSharedPropertiesConfig nonSharedPropertiesConfig)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the class is initialized.
        if (!MyEdit::isReady)
        {
            error_message = L"The class is not initialized.";
            break;
        }

        // Check if the non-shared properties configuration structure is valid.
        if (!nonSharedPropertiesConfig.isValid())
        {
            error_message = L"The non-shared properties configuration structure is invalid.";
            break;
        }

        // Set the pointers.
        this->associatedWMCommandMessageID = nonSharedPropertiesConfig.associatedWMCommandMessageID;
        this->skipTabStop = nonSharedPropertiesConfig.skipTabStop;

        // Install the subclass callback for the editbox.
        if (!this->installEditSubclass(hWnd))
        {
            error_message = L"Failed to install the subclass callback for the editbox.";
            break;
        }

        // Install the subclass callback for the editbox-associated static window.
        if (!this->installStaticSubclass(this->hWndStatic))
        {
            error_message = L"Failed to install the subclass callback for the editbox-associated static window.";
            break;
        }

        // Store the editbox handle.
        this->hWndEdit = hWnd;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyEdit\" | FUNC: \"setSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class MyDDLCombobox definitions:
// Private member function(s) [INTERNAL FUNCTIONS]:
bool MyDDLCombobox::installSubclassDDLCombobox(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Install the subclass callback.
        if (!SetWindowSubclass(hWnd, &MyDDLCombobox::scMyDDLCombobox, NULL, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = L"Failed to install the subclass callback \"SetWindowSubclass()\".";
            break;
        }

        this->hWndDDLCombobox = hWnd;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyDDLCombobox\" | FUNC: \"installSubclassDDLCombobox()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
bool MyDDLCombobox::installSubclassDropdownlist(HWND hWnd)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Install the subclass callback.
        if (!SetWindowSubclass(hWnd, &MyDDLCombobox::scMyDDLComboboxDropdownlist, NULL, reinterpret_cast<DWORD_PTR>(this)))
        {
            error_message = L"Failed to install the subclass callback \"SetWindowSubclass()\".";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyDDLCombobox\" | FUNC: \"installSubclassDropdownlist()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
// Private member function(s) [SUBCLASS CALLBACK FUNCTIONS]:
LRESULT CALLBACK MyDDLCombobox::scMyDDLCombobox(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the poiner from dwRefData and use it to access non-static members.
    MyDDLCombobox *p_this = reinterpret_cast<MyDDLCombobox *>(dwRefData);

    switch (uMsg)
    {
    // Remove the window subclass callback.
    case WM_NCDESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyDDLCombobox::scMyDDLCombobox, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyDDLCombobox()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
    {
        return 1;
    }

    // Override paint messages.
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = NULL;
        HDC mem_hdc = NULL;
        HBITMAP bitmap = NULL;
        HBITMAP bitmap_old = NULL;
        bool is_paint_begined = false;
        bool is_bitmap_created = false;
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // The combobox must have CBS_DROPDOWNLIST style.
            DWORD combobox_style = static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE));
            if (!(combobox_style & CBS_DROPDOWNLIST))
            {
                error_message = L"The combobox does not have CBS_DROPDOWNLIST style.";
                break;
            }

            // Get the combobox client rects.
            RECT rect_combobox;
            if (!GetClientRect(hWnd, &rect_combobox))
            {
                error_message = L"Failed to retrieve the combobox's client rects.";
                break;
            }
            Gdiplus::Rect gdip_rect_combobox(rect_combobox.top, rect_combobox.left, rect_combobox.right, rect_combobox.bottom);

            // Create Gdiplus::SolidBrush object for drawing operations.
            Gdiplus::SolidBrush gdip_brush_combobox = MyDDLCombobox::pColorDDLCombobox->getGDIPColor();

            // Begin the paintings.
            hdc = BeginPaint(hWnd, &ps);
            if (!hdc)
            {
                error_message = L"Failed to execute \"BeginPaint()\".";
                break;
            }
            is_paint_begined = true;

            // Create memory device context and bitmap object for double buffering.
            mem_hdc = CreateCompatibleDC(hdc);
            if (!mem_hdc)
            {
                error_message = L"Failed to create the compatible memory device context.";
                break;
            }
            bitmap = CreateCompatibleBitmap(hdc, rect_combobox.right - rect_combobox.left, rect_combobox.bottom - rect_combobox.top);
            if (!bitmap)
            {
                error_message = L"Failed to create the compatible bitmap object.";
                break;
            }
            is_bitmap_created = true;
            bitmap_old = reinterpret_cast<HBITMAP>(SelectObject(mem_hdc, bitmap));
            if (!bitmap_old)
            {
                error_message = L"Failed to select the bitmap object.";
                break;
            }

            // Create Gdiplus::Graphics object for GDI+ drawing operations.
            Gdiplus::Graphics gdip_graphics(mem_hdc);

            // Set GDI+ smoothing mode.
            if (gdip_graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality))
            {
                error_message = L"Failed to set GDI+ smoothing mode.";
                break;
            }

            // Set background mode.
            if (!SetBkMode(mem_hdc, TRANSPARENT))
            {
                error_message = L"Failed to set the background mode.";
                break;
            }

            // Select font object.
            if (!SelectObject(mem_hdc, reinterpret_cast<HFONT>(MyDDLCombobox::pFontDefault->getHFONT())))
            {
                error_message = L"Failed to select the font object.";
                break;
            }

            // Set text color.
            if (SetTextColor(mem_hdc, MyDDLCombobox::pColorText->getCOLORREF()))
            {
                error_message = L"Failed to set the text color.";
                break;
            }

            // Begin painting to the memory device context.
            {
                // Draw the combobox's background.
                FillRect(mem_hdc, &rect_combobox, MyDDLCombobox::pColorBackground->getHBRUSH());

                // Draw the combobox.
                FillRoundRect(&gdip_graphics, &gdip_brush_combobox, gdip_rect_combobox,
                              (GetFocus() == hWnd ? MyDDLCombobox::pColorFocus->getGDIPColor() : MyDDLCombobox::pColorDDLComboboxBorder->getGDIPColor()), 4);

                // Get and paint current selected combobox item text.
                INT current_selected_item_index = static_cast<INT>(SendMessageW(hWnd, CB_GETCURSEL, NULL, NULL));
                if (current_selected_item_index >= 0)
                {
                    INT text_length = static_cast<INT>(SendMessageW(hWnd, CB_GETLBTEXTLEN, current_selected_item_index, 0));
                    WCHAR *text_buffer = new WCHAR[text_length + 1];
                    SendMessageW(hWnd, CB_GETLBTEXT, current_selected_item_index, (LPARAM)text_buffer);
                    DrawTextW(mem_hdc, text_buffer, -1, &rect_combobox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    delete[] text_buffer;
                }
            }

            // Draw contents from memory device context to target device context.
            if (!BitBlt(hdc, 0, 0, rect_combobox.right - rect_combobox.left, rect_combobox.bottom - rect_combobox.top, mem_hdc, 0, 0, SRCCOPY))
            {
                error_message = L"Failed to draw contents from memory device context to target device context.";
                break;
            }

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyDDLCombobox()\"]", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (is_paint_begined)
        {
            // Delete the bitmap object.
            if (is_bitmap_created)
            {
                if (!SelectObject(mem_hdc, bitmap_old))
                {
                    WriteLog(L"Failed to select the bitmap object.", L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyDDLCombobox()\"]", MyLogType::Error);
                }
                if (!DeleteObject(bitmap))
                {
                    WriteLog(L"Failed to delete the bitmap object.", L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyDDLCombobox()\"]", MyLogType::Error);
                }
                bitmap = NULL;
                bitmap_old = NULL;
            }

            // Delete the memory device context.
            if (mem_hdc)
            {
                if (!DeleteDC(mem_hdc))
                {
                    WriteLog(L"Failed to delete the memory device context.", L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_PAINT\" | CALLBACK: \"scMyDDLCombobox()\"]", MyLogType::Error);
                }
                mem_hdc = NULL;
            }

            // End the paintings.
            EndPaint(hWnd, &ps);
            break;
        }

        break;
    }

    // Suppress unnecessary non-client paints.
    case WM_NCPAINT:
    {
        return 0;
    }

    // Suppress WM_SETFOCUS and WM_KILLFOCUS messages.
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
    {
        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    }

    // Trigger the drop-down list open message on mouse click.
    case WM_LBUTTONDOWN:
    {
        // Set focus to the combobox.
        SetFocus(hWnd);

        // Open the drop-down list.
        SendMessageW(hWnd, CB_SHOWDROPDOWN, TRUE, NULL);

        // Perform an extra invalidate operation, otherwise the combobox item won't be rendered.
        RedrawWindow(p_this->hWndDDLComboboxDropdownlist, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        break;
    }

    // Process WM_KEYUP messages to capture the ENTER key.
    case WM_KEYUP:
    {
        if (SendMessageW(hWnd, CB_GETDROPPEDSTATE, NULL, NULL) && wParam == VK_RETURN)
        {
            SendMessageW(hWnd, CB_SHOWDROPDOWN, FALSE, NULL);
            return 0;
        }
        else if (!SendMessageW(hWnd, CB_GETDROPPEDSTATE, NULL, NULL) && wParam == VK_RETURN)
        {
            SendMessageW(hWnd, CB_SHOWDROPDOWN, TRUE, NULL);
            RedrawWindow(p_this->hWndDDLComboboxDropdownlist, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            return 0;
        }

        break;
    }

    // Process WM_KEYDOWN messages to perform extra invalidate operations.
    case WM_KEYDOWN:
    {
        // Ignore the ENTER key.
        if (wParam == VK_RETURN)
            return 0;

        InvalidateRect(p_this->hWndDDLComboboxDropdownlist, NULL, FALSE);
        break;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK MyDDLCombobox::scMyDDLComboboxDropdownlist(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    // Extract the poiner from dwRefData and use it to access non-static members.
    MyDDLCombobox *p_this = reinterpret_cast<MyDDLCombobox *>(dwRefData);

    switch (uMsg)
    {
    // Remove the window subclass callback.
    case WM_NCDESTROY:
    {
        if (!RemoveWindowSubclass(hWnd, &MyDDLCombobox::scMyDDLComboboxDropdownlist, uIdSubclass))
        {
            WriteLog(L"Failed to remove window subclass callback.", L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_NCDESTROY\" | CALLBACK: \"scMyDDLComboboxDropdownlist()\"]", MyLogType::Error);
        }

        return 0;
    }

    // Suppress all system background erase requests.
    case WM_ERASEBKGND:
    {
        return 1;
    }

    // Override non-client paint messages.
    case WM_NCPAINT:
    {
        HDC hdc = NULL;
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get the combobox drop down list's device context.
            hdc = GetWindowDC(hWnd);
            if (!hdc)
            {
                error_message = L"Failed to retrieve the combobox drop down list's device context.";
                break;
            }

            // Get the combobox drop down list's client rect.
            RECT rect_dropdownlist;
            if (!GetClientRect(hWnd, &rect_dropdownlist))
            {
                error_message = L"Failed to retrieve the combobox drop down list's client rect.";
                break;
            }

            // Get the total items of the combobox to get the ID of the last item.
            INT total_combobox_items = static_cast<INT>(SendMessageW(p_this->hWndDDLCombobox, CB_GETCOUNT, NULL, NULL));
            if (total_combobox_items == CB_ERR)
            {
                error_message = L"Failed to retrieve the total combobox items.";
                break;
            }

            // Get current selectet combobox item index.
            INT current_selected_combobox_item = static_cast<INT>(SendMessageW(p_this->hWndDDLCombobox, CB_GETCURSEL, NULL, NULL));
            if (current_selected_combobox_item == CB_ERR)
            {
                error_message = L"Failed to retrieve the current selected combobox item index.";
                break;
            }

            // Fill the 2-pixels gap to match the color of the last item background color.
            RECT rect_dropdownlist_2 = rect_dropdownlist;
            rect_dropdownlist_2.top = --rect_dropdownlist_2.bottom - 1;
            FillRect(hdc, &rect_dropdownlist_2,
                     (current_selected_combobox_item == (total_combobox_items - 1)
                          ? MyDDLCombobox::pColorDDLComboboxSelectedItemBackground->getHBRUSH()
                          : MyDDLCombobox::pColorDDLComboboxItemBackground->getHBRUSH()));

            // Draw the drop-down list's borders.
            FrameRect(hdc, &rect_dropdownlist, MyDDLCombobox::pColorDDLComboboxDropdownlistBorder->getHBRUSH());

            are_all_operation_success = true;
        }

        if (!are_all_operation_success)
        {
            WriteLog(error_message, L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_NCPAINT\" | CALLBACK: \"scMyDDLComboboxDropdownlist()\"]", MyLogType::Error);
        }

        // Perform necesscary clean up operations.
        if (hdc)
        {
            if (!ReleaseDC(hWnd, hdc))
            {
                WriteLog(L"Failed to release the combobox drop down list's device context.", L" [CLASS: \"MyDDLCombobox\" | MESSAGE: \"WM_NCPAINT\" | CALLBACK: \"scMyDDLComboboxDropdownlist()\"]", MyLogType::Error);
            }
        }

        return 0;
    }

    // Process WM_NCCALCSIZE messages to suppress any default painting trigger by this message. (Kill the white background flickering)
    // This affects the default non-client area calculation.
    case WM_NCCALCSIZE:
    {
        return 0;
    }

    // Suppress WM_PRINT & WM_PRINTCLIENT messages and perform an additional invalidate.
    // This removes the drop-down list opening animation.
    case WM_PRINT:
    case WM_PRINTCLIENT:
    {
        // Perform an additional invalidate or otherwise the combobox items won't be rendered.
        return 0;
    }

    // Process WM_LBUTTONDOWN messages to set the timer that will set the focus to the combobox parent.
    // An timer is needed to add a delay to the focus change, otherwise the focus won't be set properly.
    case WM_LBUTTONDOWN:
    {
        SetTimer(hWnd, 1, 16, NULL);
        break;
    }
    case WM_TIMER:
    {
        if (wParam == 1)
        {
            KillTimer(hWnd, 1);
            SetFocus(GetParent(p_this->hWndDDLCombobox));
        }

        break;
    }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
// Public static member function(s) [INITIALIZATION FUNCTIONS]:
bool MyDDLCombobox::setSharedProperties(MyDDLComboboxSharedPropertiesConfig configSharedProperties)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the shared properties configuration structure is valid.
        if (!configSharedProperties.isValid())
        {
            error_message = L"The shared properties configuration structure is invalid.";
            break;
        }

        // Set the pointers.
        MyDDLCombobox::pColorDDLCombobox = configSharedProperties.pColorDDLCombobox;
        ;
        MyDDLCombobox::pColorDDLComboboxBorder = configSharedProperties.pColorDDLComboboxBorder;
        MyDDLCombobox::pColorDDLComboboxItemBackground = configSharedProperties.pColorDDLComboboxItemBackground;
        MyDDLCombobox::pColorDDLComboboxSelectedItemBackground = configSharedProperties.pColorDDLComboboxSelectedItemBackground;
        MyDDLCombobox::pColorDDLComboboxDropdownlistBorder = configSharedProperties.pColorDDLComboboxDropdownlistBorder;
        MyDDLCombobox::pColorText = configSharedProperties.pColorText;
        MyDDLCombobox::pColorBackground = configSharedProperties.pColorBackground;
        MyDDLCombobox::pColorFocus = configSharedProperties.pColorFocus;
        MyDDLCombobox::pFontDefault = configSharedProperties.pFontDefault;

        // Update initialization state.
        MyDDLCombobox::isSharedPropertiesSet = true;

        // Update class ready state.
        if (MyDDLCombobox::isSharedPropertiesSet)
            MyDDLCombobox::isReady = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyDDLCombobox\" | FUNC: \"setSharedProperties()\"]", MyLogType::Error);
        return false;
    }

    return true;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
bool MyDDLCombobox::isSubclassed(HWND hWnd, void **ppSubclass)
{
    DWORD_PTR reference_data;
    bool is_subclassed = GetWindowSubclass(hWnd, &MyDDLCombobox::scMyDDLCombobox, NULL, &reference_data);
    if (is_subclassed && ppSubclass)
    {
        *ppSubclass = reinterpret_cast<MyDDLCombobox *>(reference_data);
    }

    return is_subclassed;
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyDDLCombobox::setSubclass(HWND hWnd, INT comboboxHeight)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the class is initialized.
        if (!MyDDLCombobox::isReady)
        {
            error_message = L"The class is not initialized.";
            break;
        }

        // Install the subclass callback for the DDL combobox.
        if (!this->installSubclassDDLCombobox(hWnd))
        {
            error_message = L"Failed to install the subclass callback for the DDL combobox.";
            break;
        }

        // Get the combobox drop down list HWND.
        COMBOBOXINFO combobox_info{};
        combobox_info.cbSize = sizeof(COMBOBOXINFO);
        if (!GetComboBoxInfo(hWnd, &combobox_info))
        {
            error_message = L"Failed to retrieve the combobox info.";
            break;
        }
        this->hWndDDLComboboxDropdownlist = combobox_info.hwndList;

        // Check if the drop down list window is valid.
        if (!IsWindow(this->hWndDDLComboboxDropdownlist))
        {
            error_message = L"The drop down list window is invalid.";
            break;
        }

        // Install the subclass callback for the combobox drop-down list window.
        if (!this->installSubclassDropdownlist(this->hWndDDLComboboxDropdownlist))
        {
            error_message = L"Failed to install the subclass callback for the combobox drop-down list window.";
            break;
        }

        // Set drop-down list class background brush to none.
        if (!SetClassLongPtrW(this->hWndDDLComboboxDropdownlist, GCLP_HBRBACKGROUND, (LONG_PTR)NULL))
        {
            error_message = L"Failed to set drop-down list class background brush to none.";
            break;
        }

        // Don't want double click message.
        nApp::Window::Utility::RemoveWindowStyle(this->hWndDDLComboboxDropdownlist, CS_DBLCLKS);

        // Don't want drop shadow effect.
        nApp::Window::Utility::RemoveWindowStyle(this->hWndDDLComboboxDropdownlist, CS_DROPSHADOW);

        // Set font for the combobox and its drop-down list window.
        SendMessageW(this->hWndDDLCombobox, WM_SETFONT, reinterpret_cast<WPARAM>(MyDDLCombobox::pFontDefault->getHFONT()), FALSE);

        // Set the combobox height.
        ComboBox_SetItemHeight(hWnd, -1, comboboxHeight - 6);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyDDLCombobox\" | FUNC: \"setSubclass()\"]", MyLogType::Error);
        return false;
    }

    return true;
}