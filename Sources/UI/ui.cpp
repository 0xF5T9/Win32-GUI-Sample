/**
 * @file ui.cpp
 * @brief Implementation of the classes defined in ui.h.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../Global/global.h"                 // The distribution header of the global variables, forward declarations and my class declarations.

/// @class UIColors definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIColors::updateMainColors(RGBA primary, RGBA secondary, RGBA borderActive, RGBA borderInactive,
                                RGBA textActive, RGBA textInactive, RGBA textHighlight, RGBA focus, RGBA background)
{
    this->primary = MyColor(primary);
    this->secondary = MyColor(secondary);
    this->borderActive = MyColor(borderActive);
    this->borderInactive = MyColor(borderInactive);
    this->textActive = MyColor(textActive);
    this->textInactive = MyColor(textInactive);
    this->textHighlight = MyColor(textHighlight);
    this->focus = MyColor(focus);
    this->background = MyColor(background);
}
void UIColors::updateCaptionColors(RGBA captionBackground, RGBA captionTextActive, RGBA captionTextInactive,
                                   RGBA closeButtonBackgroundOnHover, RGBA closeButtonBackgroundOnDown,
                                   RGBA maximizeButtonBackgroundOnHover, RGBA maximizeButtonBackgroundOnDown,
                                   RGBA minimizeButtonBackgroundOnHover, RGBA minimizeButtonBackgroundOnDown)
{
    this->captionBackground = MyColor(captionBackground);
    this->captionTextActive = MyColor(captionTextActive);
    this->captionTextInactive = MyColor(captionTextInactive);
    this->closeButtonBackgroundOnHover = MyColor(closeButtonBackgroundOnHover);
    this->closeButtonBackgroundOnDown = MyColor(closeButtonBackgroundOnDown);
    this->maximizeButtonBackgroundOnHover = MyColor(maximizeButtonBackgroundOnHover);
    this->maximizeButtonBackgroundOnDown = MyColor(maximizeButtonBackgroundOnDown);
    this->minimizeButtonBackgroundOnHover = MyColor(minimizeButtonBackgroundOnHover);
    this->minimizeButtonBackgroundOnDown = MyColor(minimizeButtonBackgroundOnDown);
}
void UIColors::updateStandardButtonColors(RGBA standardButtonDefault, RGBA standardButtonHover, RGBA standardButtonDown,
                                          RGBA standardButtonBorderDefault, RGBA standardButtonBorderHover, RGBA standardButtonBorderDown)
{
    this->standardButtonDefault = MyColor(standardButtonDefault);
    this->standardButtonHover = MyColor(standardButtonHover);
    this->standardButtonDown = MyColor(standardButtonDown);
    this->standardButtonBorderDefault = MyColor(standardButtonBorderDefault);
    this->standardButtonBorderHover = MyColor(standardButtonBorderHover);
    this->standardButtonBorderDown = MyColor(standardButtonBorderDown);
}
void UIColors::updateRadioButtonColors(RGBA radioButtonPrimaryDefault, RGBA radioButtonPrimaryHover, RGBA radioButtonPrimaryDown,
                                       RGBA radioButtonSecondaryDefault, RGBA radioButtonSecondaryHover, RGBA radioButtonSecondaryDown,
                                       RGBA radioButtonBorderDefault, RGBA radioButtonBorderHover, RGBA radioButtonBorderDown,
                                       RGBA selectedRadioButtonPrimaryDefault, RGBA selectedRadioButtonPrimaryHover, RGBA selectedRadioButtonPrimaryDown,
                                       RGBA selectedRadioButtonSecondaryDefault, RGBA selectedRadioButtonSecondaryHover, RGBA selectedRadioButtonSecondaryDown,
                                       RGBA selectedRadioButtonBorderDefault, RGBA selectedRadioButtonBorderHover, RGBA selectedRadioButtonBorderDown)
{
    this->radioButtonPrimaryDefault = MyColor(radioButtonPrimaryDefault);
    this->radioButtonPrimaryHover = MyColor(radioButtonPrimaryHover);
    this->radioButtonPrimaryDown = MyColor(radioButtonPrimaryDown);
    this->radioButtonSecondaryDefault = MyColor(radioButtonSecondaryDefault);
    this->radioButtonSecondaryHover = MyColor(radioButtonSecondaryHover);
    this->radioButtonSecondaryDown = MyColor(radioButtonSecondaryDown);
    this->radioButtonBorderDefault = MyColor(radioButtonBorderDefault);
    this->radioButtonBorderHover = MyColor(radioButtonBorderHover);
    this->radioButtonBorderDown = MyColor(radioButtonBorderDown);
    this->selectedRadioButtonPrimaryDefault = MyColor(selectedRadioButtonPrimaryDefault);
    this->selectedRadioButtonPrimaryHover = MyColor(selectedRadioButtonPrimaryHover);
    this->selectedRadioButtonPrimaryDown = MyColor(selectedRadioButtonPrimaryDown);
    this->selectedRadioButtonSecondaryDefault = MyColor(selectedRadioButtonSecondaryDefault);
    this->selectedRadioButtonSecondaryHover = MyColor(selectedRadioButtonSecondaryHover);
    this->selectedRadioButtonSecondaryDown = MyColor(selectedRadioButtonSecondaryDown);
    this->selectedRadioButtonBorderDefault = MyColor(selectedRadioButtonBorderDefault);
    this->selectedRadioButtonBorderHover = MyColor(selectedRadioButtonBorderHover);
    this->selectedRadioButtonBorderDown = MyColor(selectedRadioButtonBorderDown);
}
void UIColors::updateEditboxColors(RGBA editbox, RGBA editboxBorderDefault, RGBA editboxBorderSelected)
{
    this->editbox = MyColor(editbox);
    this->editboxBorderDefault = MyColor(editboxBorderDefault);
    this->editboxBorderSelected = MyColor(editboxBorderSelected);
}
void UIColors::updateDDLComboboxColors(RGBA ddlCombobox, RGBA ddlComboboxBorder, RGBA ddlComboboxItemBackground, RGBA ddlComboboxSelectedItemBackground,
                                       RGBA ddlComboboxDropdownlistBorder, RGBA ddlComboboxItemTextDefault, RGBA ddlComboboxItemTextSelected)
{
    this->ddlCombobox = MyColor(ddlCombobox);
    this->ddlComboboxBorder = MyColor(ddlComboboxBorder);
    this->ddlComboboxItemBackground = MyColor(ddlComboboxItemBackground);
    this->ddlComboboxSelectedItemBackground = MyColor(ddlComboboxSelectedItemBackground);
    this->ddlComboboxDropdownlistBorder = MyColor(ddlComboboxDropdownlistBorder);
    this->ddlComboboxItemTextDefault = MyColor(ddlComboboxItemTextDefault);
    this->ddlComboboxItemTextSelected = MyColor(ddlComboboxItemTextSelected);
}

/// @class UIFonts definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIFonts::updateFonts(MyFont caption, MyFont editbox, MyFont ddlCombobox, MyFont heading, MyFont note)
{
    this->caption = caption;
    this->editbox = editbox;
    this->ddlCombobox = ddlCombobox;
    this->heading = heading;
    this->note = note;
}

/// @class UIImages definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIImages::updateNonClientButtonImages(MyD2D1Image &nonClientCloseButtonDefault, MyD2D1Image &nonClientCloseButtonHover, MyD2D1Image &nonClientCloseButtonDown,
                                           MyD2D1Image &nonClientMaximizeButtonDefault, MyD2D1Image &nonClientMaximizeButtonHover, MyD2D1Image &nonClientMaximizeButtonDown,
                                           MyD2D1Image &nonClientMinimizeButtonDefault, MyD2D1Image &nonClientMinimizeButtonHover, MyD2D1Image &nonClientMinimizeButtonDown)
{
    this->pNonClientCloseButtonDefault = std::make_unique<MyD2D1Image>(nonClientCloseButtonDefault);
    this->pNonClientCloseButtonHover = std::make_unique<MyD2D1Image>(nonClientCloseButtonHover);
    this->pNonClientCloseButtonDown = std::make_unique<MyD2D1Image>(nonClientCloseButtonDown);
    this->pNonClientMaximizeButtonDefault = std::make_unique<MyD2D1Image>(nonClientMaximizeButtonDefault);
    this->pNonClientMaximizeButtonHover = std::make_unique<MyD2D1Image>(nonClientMaximizeButtonHover);
    this->pNonClientMaximizeButtonDown = std::make_unique<MyD2D1Image>(nonClientMaximizeButtonDown);
    this->pNonClientMinimizeButtonDefault = std::make_unique<MyD2D1Image>(nonClientMinimizeButtonDefault);
    this->pNonClientMinimizeButtonHover = std::make_unique<MyD2D1Image>(nonClientMinimizeButtonHover);
    this->pNonClientMinimizeButtonDown = std::make_unique<MyD2D1Image>(nonClientMinimizeButtonDown);
}

/// @class UIRectangles definitions:
// Constructor:
UIRectangles::UIRectangles()
{
    // Caption bar area rectangle.
    this->rectCaption =
        {
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Left.
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Top.
            g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH,           // Right.
            WINDOW_BORDER_DEFAULTWIDTH + WINDOW_CAPTIONBAR_DEFAULTHEIGHT // Bottom.
        };

    // Top border area rectangle.
    this->rectSizeBorderTop =
        {
            0,                         // Left
            0,                         // Top
            g_CurrentWindowWidth,      // Right
            WINDOW_BORDER_DEFAULTWIDTH // Bottom
        };

    // Bottom border area rectangle.
    this->rectSizeBorderBottom =
        {
            0,                                                  // Left
            g_CurrentWindowHeight - WINDOW_BORDER_DEFAULTWIDTH, // Top
            g_CurrentWindowWidth,                               // Right
            g_CurrentWindowHeight                               // Bottom
        };

    // Left border area rectangle.
    this->rectSizeBorderLeft =
        {
            0,                          // Left
            1,                          // Top
            WINDOW_BORDER_DEFAULTWIDTH, // Right
            g_CurrentWindowHeight - 1   // Bottom
        };

    // Right border area rectangle.
    this->rectSizeBorderRight =
        {
            g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH, // Left
            1,                                                 // Top
            g_CurrentWindowWidth,                              // Right
            g_CurrentWindowHeight - 1                          // Bottom
        };
}

/// @class UIElements definitions:
// Constructor:
UIElements::UIElements(MyGDIPEngine *pMyGDIPEngine, MyD2D1Engine *pMyD2D1Engine)
{
    // Check if the pointer to the GDI+ engine object is null or the engine is not ready.
    if (!pMyGDIPEngine || !pMyGDIPEngine->isReady())
    {
        WriteLog(L"Pointer to the GDI+ engine object is null or the engine is not ready.", L" [CLASS: \"UIElements\" | FUNC: \"UIElements()\"]", MyLogType::Error);
        throw std::runtime_error("Pointer to the GDI+ engine object is null or the engine is not ready.");
    }

    // Check if the pointer to the Direct2D engine object is null or the engine is not ready.
    if (!pMyD2D1Engine || !pMyD2D1Engine->isReady())
    {
        WriteLog(L"Pointer to the Direct2D engine object is null or the engine is not ready.", L" [CLASS: \"UIElements\" | FUNC: \"UIElements()\"]", MyLogType::Error);
        throw std::runtime_error("Pointer to the Direct2D engine object is null or the engine is not ready.");
    }
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool UIElements::createD2D1DeviceResources(HWND hWnd, bool recreateRenderTarget)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        // Get the device context.
        HDC hdc = GetDC(hWnd);
        if (!hdc)
        {
            error_message = L"Failed to get the device context.";
            break;
        }

        // Get the client area of the window.
        RECT rect_window;
        if (!GetClientRect(hWnd, &rect_window))
        {
            error_message = L"Failed to get the client area of the window.";
            break;
        }

        // (Re)create the render target.
        if (recreateRenderTarget)
        {
            // Release the render target if it exists.
            if (this->pRenderTarget)
            {
                this->pRenderTarget->Release();
                this->pRenderTarget = nullptr;
            }

            this->pRenderTarget = g_pD2D1Engine->createDCRenderTarget();
            if (!this->pRenderTarget)
            {
                error_message = L"Failed to create the render target.";
                break;
            }
        }

        // Bind the device context to the render target.
        hr = this->pRenderTarget->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = L"Failed to bind the device context to the render target.";
            break;
        }

        // (Re)create the device-dependent resources.
        {
            // Release device-dependent resources if they exist.
            if (this->pSolidColorBrushCaptionBackground)
            {
                this->pSolidColorBrushCaptionBackground->Release();
                this->pSolidColorBrushCaptionBackground = nullptr;
            }
            if (this->pSolidColorBrushBackground)
            {
                this->pSolidColorBrushBackground->Release();
                this->pSolidColorBrushBackground = nullptr;
            }
            if (this->pSolidColorBrushBorderActive)
            {
                this->pSolidColorBrushBorderActive->Release();
                this->pSolidColorBrushBorderActive = nullptr;
            }
            if (this->pSolidColorBrushBorderInactive)
            {
                this->pSolidColorBrushBorderInactive->Release();
                this->pSolidColorBrushBorderInactive = nullptr;
            }

            // Create device-dependent resources.
            hr = this->pRenderTarget->CreateSolidColorBrush(this->colors.captionBackground.getD2D1Color(), &this->pSolidColorBrushCaptionBackground);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the solid color brush for the caption bar background.";
                break;
            }
            hr = this->pRenderTarget->CreateSolidColorBrush(this->colors.background.getD2D1Color(), &this->pSolidColorBrushBackground);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the solid color brush for the background.";
                break;
            }
            hr = this->pRenderTarget->CreateSolidColorBrush(this->colors.borderActive.getD2D1Color(), &this->pSolidColorBrushBorderActive);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the solid color brush for the active border.";
                break;
            }
            hr = this->pRenderTarget->CreateSolidColorBrush(this->colors.borderInactive.getD2D1Color(), &this->pSolidColorBrushBorderInactive);
            if (FAILED(hr))
            {
                error_message = L"Failed to create the solid color brush for the inactive border.";
                break;
            }
        }

        // Release the device context.
        if (!ReleaseDC(hWnd, hdc))
        {
            error_message = L"Failed to release the device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"UIElements\" | FUNC: \"createD2D1DeviceResources()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
void UIElements::releaseD2D1DeviceResources()
{
    if (this->pSolidColorBrushCaptionBackground)
    {
        this->pSolidColorBrushCaptionBackground->Release();
        this->pSolidColorBrushCaptionBackground = nullptr;
    }
    if (this->pSolidColorBrushBackground)
    {
        this->pSolidColorBrushBackground->Release();
        this->pSolidColorBrushBackground = nullptr;
    }
    if (this->pSolidColorBrushBorderActive)
    {
        this->pSolidColorBrushBorderActive->Release();
        this->pSolidColorBrushBorderActive = nullptr;
    }
    if (this->pSolidColorBrushBorderInactive)
    {
        this->pSolidColorBrushBorderInactive->Release();
        this->pSolidColorBrushBorderInactive = nullptr;
    }
    if (this->pRenderTarget)
    {
        this->pRenderTarget->Release();
        this->pRenderTarget = nullptr;
    }
}