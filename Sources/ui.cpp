/**
 * @file ui.cpp
 * @brief Implementation of the user-defined types defined in ui.h.
 */

#include "../Headers/standard_includes.h" // Include standard libraries.
#include "../Headers/my_includes.h"       // Include my user-defined type headers.

/******************************
 * UI classes implementations *
 ******************************/

// [UIColors] class implementations:

UIColors::UIColors()
{
    if (!UIColors::instanceExists)
        UIColors::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The UI color instance already exists.", "[CLASS: 'UIColors' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The UI color instance already exists.");
    }
}
UIColors::~UIColors()
{
    UIColors::instanceExists = false;
}
bool UIColors::updateMainColors(MYRGBA primary, MYRGBA secondary, MYRGBA borderActive, MYRGBA borderInactive, MYRGBA textActive, MYRGBA textInactive, MYRGBA textHighlight, MYRGBA focus, MYRGBA background)
{
    if (!this->primary.update(primary) ||
        !this->secondary.update(secondary) ||
        !this->borderActive.update(borderActive) ||
        !this->borderInactive.update(borderInactive) ||
        !this->textActive.update(textActive) ||
        !this->textInactive.update(textInactive) ||
        !this->textHighlight.update(textHighlight) ||
        !this->focus.update(focus) ||
        !this->background.update(background))
        return false;

    return true;
}
bool UIColors::updateCaptionColors(MYRGBA captionBackground, MYRGBA captionTextActive, MYRGBA captionTextInactive, MYRGBA closeButtonBackgroundOnHover, MYRGBA closeButtonBackgroundOnActive, MYRGBA maximizeButtonBackgroundOnHover, MYRGBA maximizeButtonBackgroundOnActive, MYRGBA minimizeButtonBackgroundOnHover, MYRGBA minimizeButtonBackgroundOnActive)
{
    if (!this->captionBackground.update(captionBackground) ||
        !this->captionTextActive.update(captionTextActive) ||
        !this->captionTextInactive.update(captionTextInactive) ||
        !this->closeButtonBackgroundOnHover.update(closeButtonBackgroundOnHover) ||
        !this->closeButtonBackgroundOnActive.update(closeButtonBackgroundOnActive) ||
        !this->maximizeButtonBackgroundOnHover.update(maximizeButtonBackgroundOnHover) ||
        !this->maximizeButtonBackgroundOnActive.update(maximizeButtonBackgroundOnActive) ||
        !this->minimizeButtonBackgroundOnHover.update(minimizeButtonBackgroundOnHover) ||
        !this->minimizeButtonBackgroundOnActive.update(minimizeButtonBackgroundOnActive))
        return false;

    return true;
}
bool UIColors::updateStandardButtonColors(MYRGBA standardButtonDefault, MYRGBA standardButtonHover, MYRGBA standardButtonActive, MYRGBA standardButtonBorderDefault, MYRGBA standardButtonBorderHover, MYRGBA standardButtonBorderActive)
{
    if (!this->standardButtonDefault.update(standardButtonDefault) ||
        !this->standardButtonHover.update(standardButtonHover) ||
        !this->standardButtonActive.update(standardButtonActive) ||
        !this->standardButtonBorderDefault.update(standardButtonBorderDefault) ||
        !this->standardButtonBorderHover.update(standardButtonBorderHover) ||
        !this->standardButtonBorderActive.update(standardButtonBorderActive))
        return false;

    return true;
}
bool UIColors::updateRadioButtonColors(MYRGBA radioButtonPrimaryDefault, MYRGBA radioButtonPrimaryHover, MYRGBA radioButtonPrimaryActive, MYRGBA radioButtonSecondaryDefault, MYRGBA radioButtonSecondaryHover, MYRGBA radioButtonSecondaryActive, MYRGBA radioButtonBorderDefault, MYRGBA radioButtonBorderHover, MYRGBA radioButtonBorderActive, MYRGBA selectedRadioButtonPrimaryDefault, MYRGBA selectedRadioButtonPrimaryHover, MYRGBA selectedRadioButtonPrimaryActive, MYRGBA selectedRadioButtonSecondaryDefault, MYRGBA selectedRadioButtonSecondaryHover, MYRGBA selectedRadioButtonSecondaryActive, MYRGBA selectedRadioButtonBorderDefault, MYRGBA selectedRadioButtonBorderHover, MYRGBA selectedRadioButtonBorderActive)
{
    if (!this->radioButtonPrimaryDefault.update(radioButtonPrimaryDefault) ||
        !this->radioButtonPrimaryHover.update(radioButtonPrimaryHover) ||
        !this->radioButtonPrimaryActive.update(radioButtonPrimaryActive) ||
        !this->radioButtonSecondaryDefault.update(radioButtonSecondaryDefault) ||
        !this->radioButtonSecondaryHover.update(radioButtonSecondaryHover) ||
        !this->radioButtonSecondaryActive.update(radioButtonSecondaryActive) ||
        !this->radioButtonBorderDefault.update(radioButtonBorderDefault) ||
        !this->radioButtonBorderHover.update(radioButtonBorderHover) ||
        !this->radioButtonBorderActive.update(radioButtonBorderActive) ||
        !this->selectedRadioButtonPrimaryDefault.update(selectedRadioButtonPrimaryDefault) ||
        !this->selectedRadioButtonPrimaryHover.update(selectedRadioButtonPrimaryHover) ||
        !this->selectedRadioButtonPrimaryActive.update(selectedRadioButtonPrimaryActive) ||
        !this->selectedRadioButtonSecondaryDefault.update(selectedRadioButtonSecondaryDefault) ||
        !this->selectedRadioButtonSecondaryHover.update(selectedRadioButtonSecondaryHover) ||
        !this->selectedRadioButtonSecondaryActive.update(selectedRadioButtonSecondaryActive) ||
        !this->selectedRadioButtonBorderDefault.update(selectedRadioButtonBorderDefault) ||
        !this->selectedRadioButtonBorderHover.update(selectedRadioButtonBorderHover) ||
        !this->selectedRadioButtonBorderActive.update(selectedRadioButtonBorderActive))
        return false;

    return true;
}
bool UIColors::updateEditboxColors(MYRGBA editbox, MYRGBA editboxBorderDefault, MYRGBA editboxBorderSelected)
{
    if (!this->editbox.update(editbox) ||
        !this->editboxBorderDefault.update(editboxBorderDefault) ||
        !this->editboxBorderSelected.update(editboxBorderSelected))
        return false;

    return true;
}
bool UIColors::updateDDLComboboxColors(MYRGBA ddlComboboxDefault, MYRGBA ddlComboboxActive, MYRGBA ddlComboboxBorder, MYRGBA ddlComboboxItemBackground, MYRGBA ddlComboboxSelectedItemBackground, MYRGBA ddlComboboxDropdownlistBorder, MYRGBA ddlComboboxItemTextDefault, MYRGBA ddlComboboxItemTextSelected)
{
    if (!this->ddlComboboxDefault.update(ddlComboboxDefault) ||
        !this->ddlComboboxActive.update(ddlComboboxActive) ||
        !this->ddlComboboxBorder.update(ddlComboboxBorder) ||
        !this->ddlComboboxItemBackground.update(ddlComboboxItemBackground) ||
        !this->ddlComboboxSelectedItemBackground.update(ddlComboboxSelectedItemBackground) ||
        !this->ddlComboboxDropdownlistBorder.update(ddlComboboxDropdownlistBorder) ||
        !this->ddlComboboxItemTextDefault.update(ddlComboboxItemTextDefault) ||
        !this->ddlComboboxItemTextSelected.update(ddlComboboxItemTextSelected))
        return false;

    return true;
}
bool UIColors::updateScrollbarColors(MYRGBA scrollbarBackground, MYRGBA scrollbarThumbDefault, MYRGBA scrollbarThumbHover, MYRGBA scrollbarThumbDragging)
{
    if (!this->scrollbarBackground.update(scrollbarBackground) ||
        !this->scrollbarThumbDefault.update(scrollbarThumbDefault) ||
        !this->scrollbarThumbHover.update(scrollbarThumbHover) ||
        !this->scrollbarThumbDragging.update(scrollbarThumbDragging))
        return false;

    return true;
}

// [UIFonts] class implementations:

UIFonts::UIFonts()
{
    if (!UIFonts::instanceExists)
        UIFonts::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The UI font instance already exists.", "[CLASS: 'UIFonts' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The UI font instance already exists.");
    }

    if (!this->addMemoryFontResource(this->hFontResourceUbuntu, IDR_FONT_UBUNTU))
    {
        g_pApp->logger.writeLog("Failed to add the memory font resource.", "[CLASS: 'UIFonts' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("Failed to add the memory font resource.");
    }

    if (!this->updateFonts(defaultFamily))
    {
        g_pApp->logger.writeLog("Failed to create the font objects.", "[CLASS: 'UIFonts' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("Failed to create the font objects.");
    }
}
UIFonts::~UIFonts()
{
    if (!this->removeMemoryFontResource(this->hFontResourceUbuntu))
        g_pApp->logger.writeLog("Failed to remove the font resource from system memory.", "[CLASS: 'UIFonts' | FUNC: 'Destructor']", MyLogType::Error);

    UIFonts::instanceExists = false;
}
bool UIFonts::updateFonts(const std::wstring &defaultFamily)
{
    this->defaultFamily = defaultFamily;

    this->hfoCaption.reset(new HFONT(NULL));
    this->hfoHeading.reset(new HFONT(NULL));
    this->hfoNote.reset(new HFONT(NULL));
    *this->hfoCaption = CreateFontW(23, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                    this->defaultFamily.c_str());
    *this->hfoHeading = CreateFontW(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                    this->defaultFamily.c_str());
    *this->hfoNote = CreateFontW(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                 CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                 this->defaultFamily.c_str());
    if (!*this->hfoCaption ||
        !*this->hfoHeading ||
        !*this->hfoNote)
        return false;

    return true;
}
bool UIFonts::addMemoryFontResource(HANDLE &handle, INT resourceID)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        handle = INVALID_HANDLE_VALUE;

        HRSRC font_resource = FindResourceW(NULL, MAKEINTRESOURCEW(resourceID), RT_FONT);
        if (!font_resource)
        {
            error_message = "Failed to locate the font resource.";
            break;
        }

        HGLOBAL font_mem = LoadResource(NULL, font_resource);
        if (!font_mem)
        {
            error_message = "Failed to load the font resource.";
            break;
        }

        void *p_font_data = LockResource(font_mem);
        if (!p_font_data)
        {
            error_message = "Failed to retrieve the pointer to the font data";
            break;
        }

        DWORD total_installed_fonts = 0, size = SizeofResource(NULL, font_resource);
        if (!size)
        {
            error_message = "Failed to retrieve the font resource size.";
            break;
        }

        handle = AddFontMemResourceEx(p_font_data, size, nullptr, &total_installed_fonts);
        if (!handle)
        {
            error_message = "Failed to add the font.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'UIFonts' | FUNC: 'addMemoryFontResource()']", MyLogType::Error);

    return are_all_operation_success;
}
bool UIFonts::removeMemoryFontResource(HANDLE handle)
{
    if (!RemoveFontMemResourceEx(handle))
    {
        g_pApp->logger.writeLog("Failed to remove the font.", "[CLASS: 'UIFonts' | FUNC: 'removeMemoryFontResource()']", MyLogType::Error);
        return false;
    }

    return true;
}

// [UIImages] class implementations:

UIImages::UIImages()
{
    if (!UIImages::instanceExists)
        UIImages::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The UI image instance already exists.", "[CLASS: 'UIImages' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The UI image instance already exists.");
    }
}
UIImages::~UIImages()
{
    UIImages::instanceExists = false;
}
bool UIImages::updateNonClientImages(MyTheme theme)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        switch (theme)
        {
        case MyTheme::Light:
        {
            this->pWicBitmapMinimizeImageDefault = &this->wicBitmapMinusBlack;
            this->pWicBitmapMinimizeImageHover = &this->wicBitmapMinusBlack;
            this->pWicBitmapMinimizeImageActive = &this->wicBitmapMinusBlack;
            this->pWicBitmapMaximizeImageDefault = &this->wicBitmapSquareBlack;
            this->pWicBitmapMaximizeImageHover = &this->wicBitmapSquareBlack;
            this->pWicBitmapMaximizeImageActive = &this->wicBitmapSquareBlack;
            this->pWicBitmapCloseImageDefault = &this->wicBitmapCrossBlack;
            this->pWicBitmapCloseImageHover = &this->wicBitmapCrossWhite;
            this->pWicBitmapCloseImageActive = &this->wicBitmapCrossWhite;
            this->pWicBitmapDDLComboboxArrow = &this->wicBitmapTriangleArrowLight;
            break;
        }
        case MyTheme::Dark:
        {
            this->pWicBitmapMinimizeImageDefault = &this->wicBitmapMinusGrey;
            this->pWicBitmapMinimizeImageHover = &this->wicBitmapMinusWhite;
            this->pWicBitmapMinimizeImageActive = &this->wicBitmapMinusWhite;
            this->pWicBitmapMaximizeImageDefault = &this->wicBitmapSquareGrey;
            this->pWicBitmapMaximizeImageHover = &this->wicBitmapSquareWhite;
            this->pWicBitmapMaximizeImageActive = &this->wicBitmapSquareWhite;
            this->pWicBitmapCloseImageDefault = &this->wicBitmapCrossGrey;
            this->pWicBitmapCloseImageHover = &this->wicBitmapCrossWhite;
            this->pWicBitmapCloseImageActive = &this->wicBitmapCrossWhite;
            this->pWicBitmapDDLComboboxArrow = &this->wicBitmapTriangleArrowDark;
            break;
        }
        case MyTheme::Monokai:
        {
            this->pWicBitmapMinimizeImageDefault = &this->wicBitmapMinusGrey;
            this->pWicBitmapMinimizeImageHover = &this->wicBitmapMinusWhite;
            this->pWicBitmapMinimizeImageActive = &this->wicBitmapMinusWhite;
            this->pWicBitmapMaximizeImageDefault = &this->wicBitmapSquareGrey;
            this->pWicBitmapMaximizeImageHover = &this->wicBitmapSquareWhite;
            this->pWicBitmapMaximizeImageActive = &this->wicBitmapSquareWhite;
            this->pWicBitmapCloseImageDefault = &this->wicBitmapCrossGrey;
            this->pWicBitmapCloseImageHover = &this->wicBitmapCrossWhite;
            this->pWicBitmapCloseImageActive = &this->wicBitmapCrossWhite;
            this->pWicBitmapDDLComboboxArrow = &this->wicBitmapTriangleArrowMonokai;
            break;
        }
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'UIImages' | FUNC: 'updateNonClientImages()']", MyLogType::Error);

    return are_all_operation_success;
}

// [UIMiscs] class implementations:

UIMiscs::UIMiscs()
{
    if (!UIMiscs::instanceExists)
        UIMiscs::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The UI misc instance already exists.", "[CLASS: 'UIMiscs' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The UI misc instance already exists.");
    }

    // Caption bar area rectangle.
    this->rectCaption =
        {
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Left.
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Top.
            g_pApp->windowWidth - WINDOW_BORDER_DEFAULTWIDTH,            // Right.
            WINDOW_BORDER_DEFAULTWIDTH + WINDOW_CAPTIONBAR_DEFAULTHEIGHT // Bottom.
        };

    // Top border area rectangle.
    this->rectSizeBorderTop =
        {
            0,                         // Left
            0,                         // Top
            g_pApp->windowWidth,       // Right
            WINDOW_BORDER_DEFAULTWIDTH // Bottom
        };

    // Bottom border area rectangle.
    this->rectSizeBorderBottom =
        {
            0,                                                 // Left
            g_pApp->windowHeight - WINDOW_BORDER_DEFAULTWIDTH, // Top
            g_pApp->windowWidth,                               // Right
            g_pApp->windowHeight                               // Bottom
        };

    // Left border area rectangle.
    this->rectSizeBorderLeft =
        {
            0,                          // Left
            1,                          // Top
            WINDOW_BORDER_DEFAULTWIDTH, // Right
            g_pApp->windowHeight - 1    // Bottom
        };

    // Right border area rectangle.
    this->rectSizeBorderRight =
        {
            g_pApp->windowWidth - WINDOW_BORDER_DEFAULTWIDTH, // Left
            1,                                                // Top
            g_pApp->windowWidth,                              // Right
            g_pApp->windowHeight - 1                          // Bottom
        };
}
UIMiscs::~UIMiscs()
{
    UIMiscs::instanceExists = false;
}

// [UIManager] class implementations:

UIManager::UIManager(MyGraphicEngine *pGraphic, HWND &hAppWindow)
    : pGraphic(pGraphic), hAppWindow(hAppWindow)
{
    if (!UIManager::instanceExists)
        UIManager::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The UI manager instance already exists.", "[CLASS: 'UIManager' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The UI manager instance already exists.");
    }

    if (!pGraphic->isInitialized())
    {
        g_pApp->logger.writeLog("The graphic engine is not initialized.", "[CLASS: 'UIManager' | FUNC: 'UIManager()']", MyLogType::Error);
        throw std::runtime_error("The graphic engine is not initialized.");
    }
}
UIManager::~UIManager() noexcept(false)
{
    if (!this->pGraphic->isInitialized())
    {
        g_pApp->logger.writeLog("The graphic engine is not initialized.", "[CLASS: 'UIManager' | FUNC: 'UIManager()']", MyLogType::Error);
        throw std::runtime_error("The graphic engine is not initialized.");
    }

    UIManager::instanceExists = false;
}
bool UIManager::updateUITheme(MyTheme theme)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        bool is_switch_success = false;
        switch (theme)
        {
        case MyTheme::Light:
        {
            if (!this->colors.updateMainColors(MYRGBA(255, 255, 255),  // Primary color.
                                               MYRGBA(255, 255, 255),  // Secondary color.
                                               MYRGBA(64, 64, 64),     // Border active color.
                                               MYRGBA(192, 192, 192),  // Border inactive color.
                                               MYRGBA(0, 0, 0),        // Text active color.
                                               MYRGBA(153, 153, 153),  // Text inactive color.
                                               MYRGBA(0, 0, 0),        // Text highlight color.
                                               MYRGBA(0, 162, 237),    // Focus color.
                                               MYRGBA(240, 240, 240))) // Background color.
            {
                error_message = "Failed to update the main colors.";
                break;
            }

            if (!this->colors.updateCaptionColors(MYRGBA(220, 220, 220),  // Caption background color.
                                                  MYRGBA(0, 0, 0),        // Caption text active color.
                                                  MYRGBA(153, 153, 153),  // Caption text inactive color.
                                                  MYRGBA(255, 89, 89),    // Caption close button hover background color.
                                                  MYRGBA(255, 89, 89),    // Caption close button down background color.
                                                  MYRGBA(205, 206, 206),  // Caption maximize button hover background color.
                                                  MYRGBA(205, 206, 206),  // Caption maximize button down background color.
                                                  MYRGBA(205, 206, 206),  // Caption minimize button hover background color.
                                                  MYRGBA(205, 206, 206))) // Caption minimize button down background color.
            {
                error_message = "Failed to update the caption colors.";
                break;
            }

            if (!this->colors.updateStandardButtonColors(MYRGBA(225, 225, 225), // Standard button default color.
                                                         MYRGBA(229, 241, 251), // Standard button hover color.
                                                         MYRGBA(204, 228, 247), // Standard button down color.
                                                         MYRGBA(172, 172, 172), // Standard button border default color.
                                                         MYRGBA(0, 120, 215),   // Standard button border hover color.
                                                         MYRGBA(0, 84, 153)))   // Standard button border down color.
            {
                error_message = "Failed to update the standard button colors.";
                break;
            }

            if (!this->colors.updateRadioButtonColors(MYRGBA(255, 255, 255), // Radio button primary color.
                                                      MYRGBA(255, 255, 255), // Radio button hover state primary color.
                                                      MYRGBA(204, 228, 247), // Radio button down state primary color.
                                                      MYRGBA(255, 255, 255), // Radio button secondary color.
                                                      MYRGBA(255, 255, 255), // Radio button hover state secondary color.
                                                      MYRGBA(204, 228, 247), // Radio button down state secondary color.
                                                      MYRGBA(51, 51, 51),    // Radio button border color.
                                                      MYRGBA(0, 120, 215),   // Radio button hover state border color.
                                                      MYRGBA(0, 84, 153),    // Radio button down state border color.
                                                      MYRGBA(51, 51, 51),    // Selected radio button primary color.
                                                      MYRGBA(0, 120, 215),   // Selected radio button hover state primary color.
                                                      MYRGBA(0, 84, 153),    // Selected radio button down state primary color.
                                                      MYRGBA(255, 255, 255), // Selected radio button secondary color.
                                                      MYRGBA(255, 255, 255), // Selected radio button hover state secondary color.
                                                      MYRGBA(204, 228, 247), // Selected radio button down state secondary color.
                                                      MYRGBA(51, 51, 51),    // Selected radio button border color.
                                                      MYRGBA(0, 120, 215),   // Selected radio button hover state border color.
                                                      MYRGBA(0, 84, 153)))   // Selected radio button down state border color.
            {
                error_message = "Failed to update the radio button colors.";
                break;
            }

            if (!this->colors.updateEditboxColors(MYRGBA(255, 255, 255), // Editbox color.
                                                  MYRGBA(122, 122, 122), // Editbox border default color.
                                                  MYRGBA(0, 120, 215)))  // Editbox border selected color.
            {
                error_message = "Failed to update the editbox colors.";
                break;
            }

            if (!this->colors.updateDDLComboboxColors(MYRGBA(225, 225, 225),  // DDL combobox default color.
                                                      MYRGBA(229, 241, 251),  // DDL combobox active color.
                                                      MYRGBA(172, 172, 172),  // DDL combobox border color.
                                                      MYRGBA(255, 255, 255),  // DDL combobox default item background color.
                                                      MYRGBA(0, 120, 215),    // DDL combobox selected item background color.
                                                      MYRGBA(0, 120, 215),    // DDL combobox drop-down list window border color.
                                                      MYRGBA(0, 0, 0),        // DDL combobox default item text color.
                                                      MYRGBA(255, 255, 255))) // DDL combobox selected item text color.
            {
                error_message = "Failed to update the drop-down list combobox colors.";
                break;
            }

            if (!this->colors.updateScrollbarColors(MYRGBA(230, 230, 230),  // Scrollbar background color.
                                                    MYRGBA(194, 195, 201),  // Scrollbar thumb default color.
                                                    MYRGBA(174, 175, 180),  // Scrollbar thumb hover color.
                                                    MYRGBA(155, 156, 160))) // Scrollbar thumb down color.
            {
                error_message = "Failed to update the scrollbar colors.";
                break;
            }

            if (!this->images.updateNonClientImages(theme))
            {
                error_message = "Failed to update the non-client images.";
                break;
            }

            if (this->miscs.pMinimizeButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pMinimizeButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = this->images.pWicBitmapMinimizeImageDefault;
                    p_subclass->imageConfig.pImageHover = this->images.pWicBitmapMinimizeImageHover;
                    p_subclass->imageConfig.pImageActive = this->images.pWicBitmapMinimizeImageActive;
                }
            }
            if (this->miscs.pMaximizeButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pMaximizeButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = this->images.pWicBitmapMaximizeImageDefault;
                    p_subclass->imageConfig.pImageHover = this->images.pWicBitmapMaximizeImageHover;
                    p_subclass->imageConfig.pImageActive = this->images.pWicBitmapMaximizeImageActive;
                }
            }
            if (this->miscs.pCloseButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pCloseButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = this->images.pWicBitmapCloseImageDefault;
                    p_subclass->imageConfig.pImageHover = this->images.pWicBitmapCloseImageHover;
                    p_subclass->imageConfig.pImageActive = this->images.pWicBitmapCloseImageActive;
                }
            }

            is_switch_success = true;
            break;
        }
        case MyTheme::Dark:
        {
            if (!this->colors.updateMainColors(MYRGBA(0, 0, 0),       // Primary color.
                                               MYRGBA(32, 32, 32),    // Secondary color.
                                               MYRGBA(48, 48, 48),    // Border active color.
                                               MYRGBA(57, 57, 57),    // Border inactive color.
                                               MYRGBA(216, 222, 233), // Text active color.
                                               MYRGBA(162, 162, 162), // Text inactive color.
                                               MYRGBA(216, 222, 233), // Text highlight color.
                                               MYRGBA(0, 162, 237),   // Focus color.
                                               MYRGBA(32, 32, 32)))   // Background color.
            {
                error_message = "Failed to update the main colors.";
                break;
            }

            if (!this->colors.updateCaptionColors(MYRGBA(0, 0, 0),       // Caption background color.
                                                  MYRGBA(216, 222, 233), // Caption text active color.
                                                  MYRGBA(162, 162, 162), // Caption text inactive color.
                                                  MYRGBA(232, 17, 35),   // Caption close button hover background color.
                                                  MYRGBA(232, 17, 35),   // Caption close button down background color.
                                                  MYRGBA(57, 57, 57),    // Caption maximize button hover background color.
                                                  MYRGBA(57, 57, 57),    // Caption maximize button down background color.
                                                  MYRGBA(57, 57, 57),    // Caption minimize button hover background color.
                                                  MYRGBA(57, 57, 57)))   // Caption minimize button down background color.
            {
                error_message = "Failed to update the caption colors.";
                break;
            }

            if (!this->colors.updateStandardButtonColors(MYRGBA(51, 51, 51),     // Standard button default color.
                                                         MYRGBA(69, 69, 69),     // Standard button hover color.
                                                         MYRGBA(102, 102, 102),  // Standard button down color.
                                                         MYRGBA(155, 155, 155),  // Standard button border default color.
                                                         MYRGBA(155, 155, 155),  // Standard button border hover color.
                                                         MYRGBA(155, 155, 155))) // Standard button border down color.
            {
                error_message = "Failed to update the standard button colors.";
                break;
            }

            if (!this->colors.updateRadioButtonColors(MYRGBA(51, 51, 51),     // Radio button primary color.
                                                      MYRGBA(69, 69, 69),     // Radio button hover state primary color.
                                                      MYRGBA(87, 87, 87),     // Radio button down state primary color.
                                                      MYRGBA(51, 51, 51),     // Radio button secondary color.
                                                      MYRGBA(69, 69, 69),     // Radio button hover state secondary color.
                                                      MYRGBA(87, 87, 87),     // Radio button down state secondary color.
                                                      MYRGBA(155, 155, 155),  // Radio button border color.
                                                      MYRGBA(155, 155, 155),  // Radio button hover state border color.
                                                      MYRGBA(155, 155, 155),  // Radio button down state border color.
                                                      MYRGBA(207, 207, 207),  // Selected radio button primary color.
                                                      MYRGBA(225, 225, 225),  // Selected radio button hover state primary color.
                                                      MYRGBA(250, 250, 250),  // Selected radio button down state primary color.
                                                      MYRGBA(51, 51, 51),     // Selected radio button secondary color.
                                                      MYRGBA(51, 51, 51),     // Selected radio button hover state secondary color.
                                                      MYRGBA(51, 51, 51),     // Selected radio button down state secondary color.
                                                      MYRGBA(155, 155, 155),  // Selected radio button border color.
                                                      MYRGBA(155, 155, 155),  // Selected radio button hover state border color.
                                                      MYRGBA(155, 155, 155))) // Selected radio button down state border color.
            {
                error_message = "Failed to update the radio button colors.";
                break;
            }

            if (!this->colors.updateEditboxColors(MYRGBA(48, 48, 48),     // Editbox color.
                                                  MYRGBA(79, 79, 79),     // Editbox border default color.
                                                  MYRGBA(100, 100, 100))) // Editbox border selected color.
            {
                error_message = "Failed to update the editbox colors.";
                break;
            }

            if (!this->colors.updateDDLComboboxColors(MYRGBA(51, 51, 51),     // DDL combobox default color.
                                                      MYRGBA(69, 69, 69),     // DDL combobox active color.
                                                      MYRGBA(155, 155, 155),  // DDL combobox border color.
                                                      MYRGBA(32, 32, 32),     // DDL combobox default item background color.
                                                      MYRGBA(44, 44, 44),     // DDL combobox selected item background color.
                                                      MYRGBA(44, 44, 44),     // DDL combobox drop-down list window border color.
                                                      MYRGBA(162, 162, 162),  // DDL combobox default item text color.
                                                      MYRGBA(255, 255, 255))) // DDL combobox selected item text color.
            {
                error_message = "Failed to update the drop-down list combobox colors.";
                break;
            }

            if (!this->colors.updateScrollbarColors(MYRGBA(46, 46, 46),     // Scrollbar background color.
                                                    MYRGBA(77, 77, 77),     // Scrollbar thumb default color.
                                                    MYRGBA(94, 94, 94),     // Scrollbar thumb hover color.
                                                    MYRGBA(112, 112, 112))) // Scrollbar thumb down color.
            {
                error_message = "Failed to update the scrollbar colors.";
                break;
            }

            if (!this->images.updateNonClientImages(theme))
            {
                error_message = "Failed to update the non-client images.";
                break;
            }

            if (this->miscs.pMinimizeButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pMinimizeButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = &this->images.wicBitmapMinusGrey;
                    p_subclass->imageConfig.pImageHover = &this->images.wicBitmapMinusWhite;
                    p_subclass->imageConfig.pImageActive = &this->images.wicBitmapMinusWhite;
                }
            }
            if (this->miscs.pMaximizeButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pMaximizeButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = &this->images.wicBitmapSquareGrey;
                    p_subclass->imageConfig.pImageHover = &this->images.wicBitmapSquareWhite;
                    p_subclass->imageConfig.pImageActive = &this->images.wicBitmapSquareWhite;
                }
            }
            if (this->miscs.pCloseButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pCloseButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = &this->images.wicBitmapCrossGrey;
                    p_subclass->imageConfig.pImageHover = &this->images.wicBitmapCrossWhite;
                    p_subclass->imageConfig.pImageActive = &this->images.wicBitmapCrossWhite;
                }
            }

            is_switch_success = true;
            break;
        }
        case MyTheme::Monokai:
        {
            if (!this->colors.updateMainColors(MYRGBA(34, 31, 34),    // Primary color.
                                               MYRGBA(45, 42, 46),    // Secondary color.
                                               MYRGBA(25, 24, 26),    // Border active color.
                                               MYRGBA(49, 47, 51),    // Border inactive color.
                                               MYRGBA(231, 230, 229), // Text active color.
                                               MYRGBA(82, 76, 83),    // Text inactive color.
                                               MYRGBA(231, 230, 229), // Text highlight color.
                                               MYRGBA(169, 220, 118), // Focus color.
                                               MYRGBA(45, 42, 46)))   // Background color.
            {
                error_message = "Failed to update the main colors.";
                break;
            }

            if (!this->colors.updateCaptionColors(MYRGBA(34, 31, 34),    // Caption background color.
                                                  MYRGBA(147, 146, 147), // Caption text active color.
                                                  MYRGBA(91, 89, 92),    // Caption text inactive color.
                                                  MYRGBA(232, 17, 35),   // Caption close button hover background color.
                                                  MYRGBA(152, 22, 34),   // Caption close button down background color.
                                                  MYRGBA(63, 61, 63),    // Caption maximize button hover background color.
                                                  MYRGBA(91, 89, 91),    // Caption maximize button down background color.
                                                  MYRGBA(63, 61, 63),    // Caption minimize button hover background color.
                                                  MYRGBA(91, 89, 91)))   // Caption minimize button down background color.
            {
                error_message = "Failed to update the caption colors.";
                break;
            }

            if (!this->colors.updateStandardButtonColors(MYRGBA(64, 62, 65),     // Standard button default color.
                                                         MYRGBA(91, 89, 92),     // Standard button hover color.
                                                         MYRGBA(101, 99, 102),   // Standard button down color.
                                                         MYRGBA(114, 112, 114),  // Standard button border default color.
                                                         MYRGBA(114, 112, 114),  // Standard button border hover color.
                                                         MYRGBA(114, 112, 114))) // Standard button border down color.
            {
                error_message = "Failed to update the standard button colors.";
                break;
            }

            if (!this->colors.updateRadioButtonColors(MYRGBA(64, 62, 65),     // Radio button primary color.
                                                      MYRGBA(91, 89, 92),     // Radio button hover state primary color.
                                                      MYRGBA(101, 99, 102),   // Radio button down state primary color.
                                                      MYRGBA(64, 62, 65),     // Radio button secondary color.
                                                      MYRGBA(91, 89, 92),     // Radio button hover state secondary color.
                                                      MYRGBA(101, 99, 102),   // Radio button down state secondary color.
                                                      MYRGBA(114, 112, 114),  // Radio button border color.
                                                      MYRGBA(114, 112, 114),  // Radio button hover state border color.
                                                      MYRGBA(114, 112, 114),  // Radio button down state border color.
                                                      MYRGBA(231, 230, 229),  // Selected radio button primary color.
                                                      MYRGBA(242, 241, 240),  // Selected radio button hover state primary color.
                                                      MYRGBA(255, 255, 255),  // Selected radio button down state primary color.
                                                      MYRGBA(64, 62, 65),     // Selected radio button secondary color.
                                                      MYRGBA(64, 62, 65),     // Selected radio button hover state secondary color.
                                                      MYRGBA(64, 62, 65),     // Selected radio button down state secondary color.
                                                      MYRGBA(114, 112, 114),  // Selected radio button border color.
                                                      MYRGBA(114, 112, 114),  // Selected radio button hover state border color.
                                                      MYRGBA(114, 112, 114))) // Selected radio button down state border color.
            {
                error_message = "Failed to update the radio button colors.";
                break;
            }

            if (!this->colors.updateEditboxColors(MYRGBA(64, 62, 65),     // Editbox color.
                                                  MYRGBA(64, 62, 65),     // Editbox border default color.
                                                  MYRGBA(114, 112, 114))) // Editbox border selected color.
            {
                error_message = "Failed to update the editbox colors.";
                break;
            }

            if (!this->colors.updateDDLComboboxColors(MYRGBA(64, 62, 65),     // DDL combobox default color.
                                                      MYRGBA(91, 89, 92),     // DDL combobox active color.
                                                      MYRGBA(64, 62, 65),     // DDL combobox border color.
                                                      MYRGBA(64, 62, 65),     // DDL combobox default item background color.
                                                      MYRGBA(74, 72, 75),     // DDL combobox selected item background color.
                                                      MYRGBA(114, 112, 114),  // DDL combobox drop-down list window border color.
                                                      MYRGBA(237, 237, 235),  // DDL combobox default item text color.
                                                      MYRGBA(225, 216, 102))) // DDL combobox selected item text color.
            {
                error_message = "Failed to update the drop-down list combobox colors.";
                break;
            }

            if (!this->colors.updateScrollbarColors(MYRGBA(40, 37, 41),  // Scrollbar background color.
                                                    MYRGBA(59, 56, 60),  // Scrollbar thumb default color.
                                                    MYRGBA(78, 75, 79),  // Scrollbar thumb hover color.
                                                    MYRGBA(98, 95, 98))) // Scrollbar thumb down color.
            {
                error_message = "Failed to update the scrollbar colors.";
                break;
            }

            if (!this->images.updateNonClientImages(theme))
            {
                error_message = "Failed to update the non-client images.";
                break;
            }

            if (this->miscs.pMinimizeButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pMinimizeButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = &this->images.wicBitmapMinusGrey;
                    p_subclass->imageConfig.pImageHover = &this->images.wicBitmapMinusWhite;
                    p_subclass->imageConfig.pImageActive = &this->images.wicBitmapMinusWhite;
                }
            }
            if (this->miscs.pMaximizeButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pMaximizeButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = &this->images.wicBitmapSquareGrey;
                    p_subclass->imageConfig.pImageHover = &this->images.wicBitmapSquareWhite;
                    p_subclass->imageConfig.pImageActive = &this->images.wicBitmapSquareWhite;
                }
            }
            if (this->miscs.pCloseButton)
            {
                auto p_subclass = MyImageButtonSubclass::getSubclassPointer(this->miscs.pCloseButton->hWnd());
                if (p_subclass)
                {
                    p_subclass->imageConfig.pImageDefault = &this->images.wicBitmapCrossGrey;
                    p_subclass->imageConfig.pImageHover = &this->images.wicBitmapCrossWhite;
                    p_subclass->imageConfig.pImageActive = &this->images.wicBitmapCrossWhite;
                }
            }

            is_switch_success = true;
            break;
        }
        }
        if (!is_switch_success)
            break;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'UIManager' | FUNC: 'updateUITheme()']", MyLogType::Error);

    return are_all_operation_success;
}
bool UIManager::createDeviceResources()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!IsWindow(this->hAppWindow))
        {
            error_message = "The global application window handle is invalid.";
            break;
        }

        HDC hdc = GetDC(this->hAppWindow);
        if (!hdc)
        {
            error_message = "Failed to get the window device context.";
            break;
        }

        RECT rect_window;
        if (!GetClientRect(this->hAppWindow, &rect_window))
        {
            error_message = "Failed to retrieve the window client rect.";
            break;
        }

        this->pRenderTarget.reset(new ID2D1DCRenderTarget *(nullptr));
        if (!this->pGraphic->d2d1Engine().createDCRenderTarget(*this->pRenderTarget))
        {
            error_message = "Failed to create the window render target.";
            break;
        }

        HRESULT hr = (*this->pRenderTarget)->BindDC(hdc, &rect_window);
        if (FAILED(hr))
        {
            error_message = "Failed to bind the window device context to the window render target.";
            break;
        }

        this->pSolidColorBrushCaptionBackground.reset(new ID2D1SolidColorBrush *(nullptr));
        this->pSolidColorBrushBackground.reset(new ID2D1SolidColorBrush *(nullptr));
        this->pSolidColorBrushBorderActive.reset(new ID2D1SolidColorBrush *(nullptr));
        this->pSolidColorBrushBorderInactive.reset(new ID2D1SolidColorBrush *(nullptr));

        error_message = "Failed to create the device resources.";
        hr = (*this->pRenderTarget)->CreateSolidColorBrush(this->colors.captionBackground.getD2D1Color(), &*this->pSolidColorBrushCaptionBackground);
        if (FAILED(hr))
            break;
        hr = (*this->pRenderTarget)->CreateSolidColorBrush(this->colors.background.getD2D1Color(), &*this->pSolidColorBrushBackground);
        if (FAILED(hr))
            break;
        hr = (*this->pRenderTarget)->CreateSolidColorBrush(this->colors.borderActive.getD2D1Color(), &*this->pSolidColorBrushBorderActive);
        if (FAILED(hr))
            break;
        hr = (*this->pRenderTarget)->CreateSolidColorBrush(this->colors.borderInactive.getD2D1Color(), &*this->pSolidColorBrushBorderInactive);
        if (FAILED(hr))
            break;
        error_message = "";

        if (!ReleaseDC(this->hAppWindow, hdc))
        {
            error_message = "Failed to release the window device context.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'UIManager' | FUNC: 'createDeviceResources()']", MyLogType::Error);

    return are_all_operation_success;
}