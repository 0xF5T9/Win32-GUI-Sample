/**
 * @file ui.h
 * @brief Header file containing user-defined types that related to UI management.
 */

#ifndef UI_H
#define UI_H

/************
 * UI enums *
 ************/

enum class MyTheme
{
    Light,
    Dark,
    Monokai,
};

/**************
 * UI classes *
 **************/

/**
 * @brief Singleton class encapsulating and managing the UI color objects.
 * @note This class is part of the UIManager class.
 */
class UIColors
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    UIColors();

    /**
     * @brief Destructor.
     */
    ~UIColors();

    // [HELPER FUNCTIONS]

    /**
     * @brief Update the main colors.
     * @param primary Primary color.
     * @param secondary Secondary color.
     * @param borderActive Border active color.
     * @param borderInactive Border inactive color.
     * @param textActive Text active color.
     * @param textInactive Text inactive color.
     * @param textHighlight Text highlight color.
     * @param focus Focus color.
     * @param background Background color.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateMainColors(MYRGBA primary, MYRGBA secondary, MYRGBA borderActive, MYRGBA borderInactive, MYRGBA textActive, MYRGBA textInactive, MYRGBA textHighlight, MYRGBA focus, MYRGBA background);

    /**
     * @brief Update the caption colors.
     * @param captionBackground Caption background color.
     * @param captionTextActive Caption text active color.
     * @param captionTextInactive Caption text inactive color.
     * @param closeButtonBackgroundOnHover Caption close button hover state background color.
     * @param closeButtonBackgroundOnActive Caption close button active state background color.
     * @param maximizeButtonBackgroundOnHover Caption maximize button hover state background color.
     * @param maximizeButtonBackgroundOnActive Caption maximize button active state background color.
     * @param minimizeButtonBackgroundOnHover Caption minimize button hover state background color.
     * @param minimizeButtonBackgroundOnActive Caption minimize button active state background color.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateCaptionColors(MYRGBA captionBackground, MYRGBA captionTextActive, MYRGBA captionTextInactive, MYRGBA closeButtonBackgroundOnHover, MYRGBA closeButtonBackgroundOnActive, MYRGBA maximizeButtonBackgroundOnHover, MYRGBA maximizeButtonBackgroundOnActive, MYRGBA minimizeButtonBackgroundOnHover, MYRGBA minimizeButtonBackgroundOnActive);

    /**
     * @brief Update the standard button colors.
     * @param standardButtonDefault Button default state color.
     * @param standardButtonHover Button hover state color.
     * @param standardButtonActive Button active state color.
     * @param standardButtonBorderDefault Button border default state color.
     * @param standardButtonBorderHover Button border hover state color.
     * @param standardButtonBorderActive Button border active state color.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateStandardButtonColors(MYRGBA standardButtonDefault, MYRGBA standardButtonHover, MYRGBA standardButtonActive, MYRGBA standardButtonBorderDefault, MYRGBA standardButtonBorderHover, MYRGBA standardButtonBorderActive);

    /**
     * @brief Update the radio button colors.
     * @param radioButtonPrimaryDefault Radio button default state primary color.
     * @param radioButtonPrimaryHover Radio button hover state primary color.
     * @param radioButtonPrimaryActive Radio button active state primary color.
     * @param radioButtonSecondaryDefault Radio button default state secondary color.
     * @param radioButtonSecondaryHover Radio button hover state secondary color.
     * @param radioButtonSecondaryActive Radio button active state secondary color.
     * @param radioButtonBorderDefault Radio button border default state color.
     * @param radioButtonBorderHover Radio button border hover state color.
     * @param radioButtonBorderActive Radio button border active state color.
     * @param selectedRadioButtonPrimaryDefault Radio button default state primary color. (Selected)
     * @param selectedRadioButtonPrimaryHover Radio button hover state primary color. (Selected)
     * @param selectedRadioButtonPrimaryActive Radio button active state primary color. (Selected)
     * @param selectedRadioButtonSecondaryDefault Radio button default state secondary color. (Selected)
     * @param selectedRadioButtonSecondaryHover Radio button hover state secondary color. (Selected)
     * @param selectedRadioButtonSecondaryActive Radio button active state secondary color. (Selected)
     * @param selectedRadioButtonBorderDefault Radio button border default state color. (Selected)
     * @param selectedRadioButtonBorderHover Radio button border hover state color. (Selected)
     * @param selectedRadioButtonBorderActive Radio button border active state color. (Selected)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateRadioButtonColors(MYRGBA radioButtonPrimaryDefault, MYRGBA radioButtonPrimaryHover, MYRGBA radioButtonPrimaryActive, MYRGBA radioButtonSecondaryDefault, MYRGBA radioButtonSecondaryHover, MYRGBA radioButtonSecondaryActive, MYRGBA radioButtonBorderDefault, MYRGBA radioButtonBorderHover, MYRGBA radioButtonBorderActive, MYRGBA selectedRadioButtonPrimaryDefault, MYRGBA selectedRadioButtonPrimaryHover, MYRGBA selectedRadioButtonPrimaryActive, MYRGBA selectedRadioButtonSecondaryDefault, MYRGBA selectedRadioButtonSecondaryHover, MYRGBA selectedRadioButtonSecondaryActive, MYRGBA selectedRadioButtonBorderDefault, MYRGBA selectedRadioButtonBorderHover, MYRGBA selectedRadioButtonBorderActive);

    /**
     * @brief Update the editbox colors.
     * @param editbox Editbox default state color.
     * @param editboxBorderDefault Editbox border default state color.
     * @param editboxBorderSelected Editbox border selected state color.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateEditboxColors(MYRGBA editbox, MYRGBA editboxBorderDefault, MYRGBA editboxBorderSelected);

    /**
     * @brief Update the drop-down list combobox colors.
     * @param ddlComboboxDefault Combobox default state color.
     * @param ddlComboboxActive Combobox active state color.
     * @param ddlComboboxBorder Combobox border color.
     * @param ddlComboboxItemBackground Combobox item background color.
     * @param ddlComboboxSelectedItemBackground Combobox selected item background color.
     * @param ddlComboboxDropdownlistBorder Combobox drop-down list window border color.
     * @param ddlComboboxItemTextDefault Combobox item text default state color.
     * @param ddlComboboxItemTextSelected Combobox item text selected state color.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateDDLComboboxColors(MYRGBA ddlComboboxDefault, MYRGBA ddlComboboxActive, MYRGBA t_ddlComboboxBorder, MYRGBA ddlComboboxItemBackground, MYRGBA ddlComboboxSelectedItemBackground, MYRGBA ddlComboboxDropdownlistBorder, MYRGBA ddlComboboxItemTextDefault, MYRGBA ddlComboboxItemTextSelected);

    /**
     * @brief Update the scrollbar colors.
     * @param scrollbarBackground Scrollbar background color.
     * @param scrollbarThumbDefault Scrollbar thumb default state color.
     * @param scrollbarThumbHover Scrollbar thumb hover state color.
     * @param scrollbarThumbDragging Scrollbar thumb dragging state color.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateScrollbarColors(MYRGBA scrollbarBackground, MYRGBA scrollbarThumbDefault, MYRGBA scrollbarThumbHover, MYRGBA scrollbarThumbDragging);

public:
    // [APPLICATION UI COLORS]

    // Main colors.

    MyColor primary;
    MyColor secondary;
    MyColor borderActive;
    MyColor borderInactive;
    MyColor textActive;
    MyColor textInactive;
    MyColor textHighlight;
    MyColor focus;
    MyColor background;

    // Caption colors.

    MyColor captionBackground;
    MyColor captionTextActive;
    MyColor captionTextInactive;
    MyColor closeButtonBackgroundOnHover;
    MyColor closeButtonBackgroundOnActive;
    MyColor maximizeButtonBackgroundOnHover;
    MyColor maximizeButtonBackgroundOnActive;
    MyColor minimizeButtonBackgroundOnHover;
    MyColor minimizeButtonBackgroundOnActive;

    // Standard button colors.

    MyColor standardButtonDefault;
    MyColor standardButtonHover;
    MyColor standardButtonActive;
    MyColor standardButtonBorderDefault;
    MyColor standardButtonBorderHover;
    MyColor standardButtonBorderActive;

    // Radio button colors.

    MyColor radioButtonPrimaryDefault;
    MyColor radioButtonPrimaryHover;
    MyColor radioButtonPrimaryActive;
    MyColor radioButtonSecondaryDefault;
    MyColor radioButtonSecondaryHover;
    MyColor radioButtonSecondaryActive;
    MyColor radioButtonBorderDefault;
    MyColor radioButtonBorderHover;
    MyColor radioButtonBorderActive;
    MyColor selectedRadioButtonPrimaryDefault;
    MyColor selectedRadioButtonPrimaryHover;
    MyColor selectedRadioButtonPrimaryActive;
    MyColor selectedRadioButtonSecondaryDefault;
    MyColor selectedRadioButtonSecondaryHover;
    MyColor selectedRadioButtonSecondaryActive;
    MyColor selectedRadioButtonBorderDefault;
    MyColor selectedRadioButtonBorderHover;
    MyColor selectedRadioButtonBorderActive;

    // Editbox colors.

    MyColor editbox;
    MyColor editboxBorderDefault;
    MyColor editboxBorderSelected;

    // Drop-down list combobox colors.

    MyColor ddlComboboxDefault;
    MyColor ddlComboboxActive;
    MyColor ddlComboboxBorder;
    MyColor ddlComboboxItemBackground;
    MyColor ddlComboboxSelectedItemBackground;
    MyColor ddlComboboxDropdownlistBorder;
    MyColor ddlComboboxItemTextDefault;
    MyColor ddlComboboxItemTextSelected;

    // Scrollbar colors.

    MyColor scrollbarBackground;
    MyColor scrollbarThumbDefault;
    MyColor scrollbarThumbHover;
    MyColor scrollbarThumbDragging;

    // Misc colors.

    MyColor debug = MyColor(0, 255, 0);
    HBRUSH nullBrush = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

private:
    inline static bool instanceExists = false; // Indicate whether the instance already exists.
};

/**
 * @brief Singleton class encapsulating and managing the UI font objects.
 * @note This class is part of the UIManager class.
 */
class UIFonts
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @note Initialize smart pointers and default font objects.
     */
    UIFonts();

    /**
     * @brief Destructor.
     */
    ~UIFonts();

    // [HELPER FUNCTIONS]

    /**
     * @brief Update application default family font.
     * @note Font objects will also be recreated with the new family font.
     * @param defaultFamily Family font name.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateFonts(const std::wstring &defaultFamily);

private:
    // [HELPER FUNCTIONS]

    /**
     * @brief Adds the font resource from a memory image (application resource) to the system.
     * @note Use 'removeMemoryFontResource()' function to remove the added font resource when it is no longer needed.
     * @param handle Reference to the handle that will receive the font memory address, this handle will be used to release the font. [OUT]
     * @param resourceID The font resource id. (.ttf)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool addMemoryFontResource(HANDLE &handle, INT resourceID);

    /**
     * @brief Remove the font added from a memory image file.
     * @param handle Handle to the font memory address.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool removeMemoryFontResource(HANDLE handle);

public:
    // [APPLICATION UI FONTS/TEXT FORMATS]

    std::wstring defaultFamily = L"Bahnschrift"; // Alternative: Ubuntu
    std::unique_ptr<HFONT, HFONTDeleter> hfoCaption;
    std::unique_ptr<HFONT, HFONTDeleter> hfoHeading;
    std::unique_ptr<HFONT, HFONTDeleter> hfoNote;
    std::unique_ptr<HFONT, HFONTDeleter> hfoText1;

private:
    HANDLE hFontResourceUbuntu = nullptr;
    inline static bool instanceExists = false; // Indicate whether the instance already exists.
};

/**
 * @brief Singleton class encapsulating and managing the UI image objects.
 * @note This class is part of the UIManager class.
 */
class UIImages
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    UIImages();

    /**
     * @brief Destructor.
     */
    ~UIImages();

    // [HELPER FUNCTIONS]

    /**
     * @brief Update the non-client image pointers to match the specified theme.
     * @param theme The theme name.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateNonClientImages(MyTheme theme);

public:
    // [APPLICATION UI IMAGES]

    // WIC images.

    MyWICBitmap wicBitmapCrossGrey = MyWICBitmap(IDR_IMAGE1, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapCrossWhite = MyWICBitmap(IDR_IMAGE2, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapCrossBlack = MyWICBitmap(IDR_IMAGE3, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapSquareGrey = MyWICBitmap(IDR_IMAGE4, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapSquareWhite = MyWICBitmap(IDR_IMAGE5, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapSquareBlack = MyWICBitmap(IDR_IMAGE6, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapMinusGrey = MyWICBitmap(IDR_IMAGE7, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapMinusWhite = MyWICBitmap(IDR_IMAGE8, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapMinusBlack = MyWICBitmap(IDR_IMAGE9, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapTriangleArrowLight = MyWICBitmap(IDR_IMAGE10, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapTriangleArrowDark = MyWICBitmap(IDR_IMAGE11, L"IMAGE", 20, 20);
    MyWICBitmap wicBitmapTriangleArrowMonokai = MyWICBitmap(IDR_IMAGE12, L"IMAGE", 20, 20);

    MyWICBitmap wicBitmapAEDefault = MyWICBitmap(IDR_IMAGE_AE_DEFAULT, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapAEHover = MyWICBitmap(IDR_IMAGE_AE_HOVER, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapAEActive = MyWICBitmap(IDR_IMAGE_AE_ACTIVE, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapPSDefault = MyWICBitmap(IDR_IMAGE_PS_DEFAULT, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapPSHover = MyWICBitmap(IDR_IMAGE_PS_HOVER, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapPSActive = MyWICBitmap(IDR_IMAGE_PS_ACTIVE, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapAIDefault = MyWICBitmap(IDR_IMAGE_AI_DEFAULT, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapAIHover = MyWICBitmap(IDR_IMAGE_AI_HOVER, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapAIActive = MyWICBitmap(IDR_IMAGE_AI_ACTIVE, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapMEDefault = MyWICBitmap(IDR_IMAGE_ME_DEFAULT, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapMEHover = MyWICBitmap(IDR_IMAGE_ME_HOVER, L"IMAGE", 90, 90);
    MyWICBitmap wicBitmapMEActive = MyWICBitmap(IDR_IMAGE_ME_ACTIVE, L"IMAGE", 90, 90);

    // WIC non-client image pointers.

    MyWICBitmap *pWicBitmapMinimizeImageDefault = nullptr;
    MyWICBitmap *pWicBitmapMinimizeImageHover = nullptr;
    MyWICBitmap *pWicBitmapMinimizeImageActive = nullptr;
    MyWICBitmap *pWicBitmapMaximizeImageDefault = nullptr;
    MyWICBitmap *pWicBitmapMaximizeImageHover = nullptr;
    MyWICBitmap *pWicBitmapMaximizeImageActive = nullptr;
    MyWICBitmap *pWicBitmapCloseImageDefault = nullptr;
    MyWICBitmap *pWicBitmapCloseImageHover = nullptr;
    MyWICBitmap *pWicBitmapCloseImageActive = nullptr;
    MyWICBitmap *pWicBitmapDDLComboboxArrow = nullptr;

private:
    inline static bool instanceExists = false; // Indicate whether the instance already exists.
};

/**
 * @brief Singleton class encapsulating and managing the UI miscellaneous variables.
 * @note This class is part of the UIManager class.
 */
class UIMiscs
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    UIMiscs();

    /**
     * @brief Destructor.
     */
    ~UIMiscs();

public:
    // [APPLICATION MISCELLANEOUS UI VARIABLES]

    // Store the essential non-client window pointers for easy access.

    MyWindow *pCloseButton = nullptr;
    MyWindow *pMaximizeButton = nullptr;
    MyWindow *pMinimizeButton = nullptr;

    // RECT structs representing application window dimensions.

    RECT rectCaption;
    RECT rectSizeBorderTop;
    RECT rectSizeBorderBottom;
    RECT rectSizeBorderLeft;
    RECT rectSizeBorderRight;

private:
    inline static bool instanceExists = false; // Indicate whether the instance already exists.
};

/**
 * @brief Singleton class encapsulating and managing all the UI-related resources.
 * @note All graphic/UI-related resources or operations can be accessed via this interface.
 */
class UIManager
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param pGraphic Specifies the pointer to the initialized graphic engine.
     * @param hAppWindow Specifies the global application window handle.
     */
    UIManager(MyGraphicEngine *pGraphic, HWND &hAppWindow);

    /**
     * @brief Destructor.
     */
    ~UIManager() noexcept(false);

    // [THEME FUNCTIONS]

    /**
     * @brief Update the UI resources to match the specified theme.
     * @param theme Specifies the theme name.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateUITheme(MyTheme theme);

    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the device resources for the application window.
     * @note Any existing device resources will be automatically released beforehand.
     * @note Invoke this function either when the application window's render target is lost
     *       or after executing `updateUITheme()` to reflect new changes.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources();

public:
    // Application window render target and device resources.
    std::unique_ptr<ID2D1DCRenderTarget *, ID2D1DCRenderTargetDeleter> pRenderTarget;
    std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> pSolidColorBrushCaptionBackground;
    std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> pSolidColorBrushBackground;
    std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> pSolidColorBrushBorderActive;
    std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> pSolidColorBrushBorderInactive;

    // UI interfaces.
    UIColors colors;
    UIFonts fonts;
    UIImages images;
    UIMiscs miscs;

private:
    MyGraphicEngine *pGraphic = nullptr;       // Pointer to the graphic engine.
    HWND &hAppWindow;                          // Reference to the application window handle.
    inline static bool instanceExists = false; // Indicate whether the instance already exists.
};

#endif // UI_H