/**
 * @file ui.h
 * @brief Header file containing classes that related to UI management.
 */

#ifndef UI_H
#define UI_H

/**
 * @class UIColors
 *
 * @brief Singleton class encapsulating and managing the UI color objects.
 *
 * @note This class is used by UIElements class.
 */
class UIColors
{
public:
    /// [DEFAULT APPLICATION UI COLORS]

    // Main colors.
    MyColor primary = MyColor(0, 0, 0);
    MyColor secondary = MyColor(0, 0, 0);
    MyColor borderActive = MyColor(0, 0, 0);
    MyColor borderInactive = MyColor(0, 0, 0);
    MyColor textActive = MyColor(0, 0, 0);
    MyColor textInactive = MyColor(0, 0, 0);
    MyColor textHighlight = MyColor(0, 0, 0);
    MyColor focus = MyColor(0, 0, 0);
    MyColor background = MyColor(0, 0, 0);

    // Caption colors.
    MyColor captionBackground = MyColor(0, 0, 0);
    MyColor captionTextActive = MyColor(0, 0, 0);
    MyColor captionTextInactive = MyColor(0, 0, 0);
    MyColor closeButtonBackgroundOnHover = MyColor(0, 0, 0);
    MyColor closeButtonBackgroundOnDown = MyColor(0, 0, 0);
    MyColor maximizeButtonBackgroundOnHover = MyColor(0, 0, 0);
    MyColor maximizeButtonBackgroundOnDown = MyColor(0, 0, 0);
    MyColor minimizeButtonBackgroundOnHover = MyColor(0, 0, 0);
    MyColor minimizeButtonBackgroundOnDown = MyColor(0, 0, 0);

    // Standard button colors.
    MyColor standardButtonDefault = MyColor(0, 0, 0);
    MyColor standardButtonHover = MyColor(0, 0, 0);
    MyColor standardButtonDown = MyColor(0, 0, 0);
    MyColor standardButtonBorderDefault = MyColor(0, 0, 0);
    MyColor standardButtonBorderHover = MyColor(0, 0, 0);
    MyColor standardButtonBorderDown = MyColor(0, 0, 0);

    // Radio button colors.
    MyColor radioButtonPrimaryDefault = MyColor(0, 0, 0);
    MyColor radioButtonPrimaryHover = MyColor(0, 0, 0);
    MyColor radioButtonPrimaryDown = MyColor(0, 0, 0);
    MyColor radioButtonSecondaryDefault = MyColor(0, 0, 0);
    MyColor radioButtonSecondaryHover = MyColor(0, 0, 0);
    MyColor radioButtonSecondaryDown = MyColor(0, 0, 0);
    MyColor radioButtonBorderDefault = MyColor(0, 0, 0);
    MyColor radioButtonBorderHover = MyColor(0, 0, 0);
    MyColor radioButtonBorderDown = MyColor(0, 0, 0);
    MyColor selectedRadioButtonPrimaryDefault = MyColor(0, 0, 0);
    MyColor selectedRadioButtonPrimaryHover = MyColor(0, 0, 0);
    MyColor selectedRadioButtonPrimaryDown = MyColor(0, 0, 0);
    MyColor selectedRadioButtonSecondaryDefault = MyColor(0, 0, 0);
    MyColor selectedRadioButtonSecondaryHover = MyColor(0, 0, 0);
    MyColor selectedRadioButtonSecondaryDown = MyColor(0, 0, 0);
    MyColor selectedRadioButtonBorderDefault = MyColor(0, 0, 0);
    MyColor selectedRadioButtonBorderHover = MyColor(0, 0, 0);
    MyColor selectedRadioButtonBorderDown = MyColor(0, 0, 0);

    // Editbox colors.
    MyColor editbox = MyColor(0, 0, 0);
    MyColor editboxBorderDefault = MyColor(0, 0, 0);
    MyColor editboxBorderSelected = MyColor(0, 0, 0);

    // Combobox (Drop-down list) colors.
    MyColor ddlCombobox = MyColor(0, 0, 0);
    MyColor ddlComboboxBorder = MyColor(0, 0, 0);
    MyColor ddlComboboxItemBackground = MyColor(0, 0, 0);
    MyColor ddlComboboxSelectedItemBackground = MyColor(0, 0, 0);
    MyColor ddlComboboxDropdownlistBorder = MyColor(0, 0, 0);
    MyColor ddlComboboxItemTextDefault = MyColor(0, 0, 0);
    MyColor ddlComboboxItemTextSelected = MyColor(0, 0, 0);

    // Misc colors.
    MyColor debug = MyColor(0, 255, 0);
    HBRUSH nullBrush = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Updates the main colors.
     *
     * @param primary        The primary color.
     * @param secondary      The secondary color.
     * @param borderActive   The active border color.
     * @param borderInactive The inactive border color.
     * @param textActive     The active text color.
     * @param textInactive   The inactive text color.
     * @param textHighlight  The text highlight color.
     * @param focus          The focus color.
     * @param background     The background color.
     */
    void updateMainColors(RGBA primary, RGBA secondary, RGBA borderActive, RGBA borderInactive,
                          RGBA textActive, RGBA textInactive, RGBA textHighlight, RGBA focus, RGBA background);

    /**
     * @brief Updates the caption colors.
     *
     * @param captionBackground               The caption background color.
     * @param captionTextActive               The caption text color when the window is active.
     * @param captionTextInactive             The caption text color when the window is inactive.
     * @param closeButtonBackgroundOnHover    The close button background color when the mouse is hovering over it.
     * @param closeButtonBackgroundOnDown     The close button background color when the mouse is pressing it.
     * @param maximizeButtonBackgroundOnHover The maximize button background color when the mouse is hovering over it.
     * @param maximizeButtonBackgroundOnDown  The maximize button background color when the mouse is pressing it.
     * @param minimizeButtonBackgroundOnHover The minimize button background color when the mouse is hovering over it.
     * @param minimizeButtonBackgroundOnDown  The minimize button background color when the mouse is pressing it.
     */
    void updateCaptionColors(RGBA captionBackground, RGBA captionTextActive, RGBA captionTextInactive,
                             RGBA closeButtonBackgroundOnHover, RGBA closeButtonBackgroundOnDown,
                             RGBA maximizeButtonBackgroundOnHover, RGBA maximizeButtonBackgroundOnDown,
                             RGBA minimizeButtonBackgroundOnHover, RGBA minimizeButtonBackgroundOnDown);

    /**
     * @brief Updates the standard button colors.
     *
     * @param standardButtonDefault       The standard button default color.
     * @param standardButtonHover         The standard button hover color.
     * @param standardButtonDown          The standard button down color.
     * @param standardButtonBorderDefault The standard button border default color.
     * @param standardButtonBorderHover   The standard button border hover color.
     * @param standardButtonBorderDown    The standard button border down color.
     */
    void updateStandardButtonColors(RGBA standardButtonDefault, RGBA standardButtonHover, RGBA standardButtonDown,
                                    RGBA standardButtonBorderDefault, RGBA standardButtonBorderHover, RGBA standardButtonBorderDown);

    /**
     * @brief Updates the radio button colors.
     *
     * @param radioButtonPrimaryDefault           The radio button primary default color.
     * @param radioButtonPrimaryHover             The radio button primary hover color.
     * @param radioButtonPrimaryDown              The radio button primary down color.
     * @param radioButtonSecondaryDefault         The radio button secondary default color.
     * @param radioButtonSecondaryHover           The radio button secondary hover color.
     * @param radioButtonSecondaryDown            The radio button secondary down color.
     * @param radioButtonBorderDefault            The radio button border default color.
     * @param radioButtonBorderHover              The radio button border hover color.
     * @param radioButtonBorderDown               The radio button border down color.
     * @param selectedRadioButtonPrimaryDefault   The selected radio button primary default color.
     * @param selectedRadioButtonPrimaryHover     The selected radio button primary hover color.
     * @param selectedRadioButtonPrimaryDown      The selected radio button primary down color.
     * @param selectedRadioButtonSecondaryDefault The selected radio button secondary default color.
     * @param selectedRadioButtonSecondaryHover   The selected radio button secondary hover color.
     * @param selectedRadioButtonSecondaryDown    The selected radio button secondary down color.
     * @param selectedRadioButtonBorderDefault    The selected radio button border default color.
     * @param selectedRadioButtonBorderHover      The selected radio button border hover color.
     * @param selectedRadioButtonBorderDown       The selected radio button border down color.
     */
    void updateRadioButtonColors(RGBA radioButtonPrimaryDefault, RGBA radioButtonPrimaryHover, RGBA radioButtonPrimaryDown,
                                 RGBA radioButtonSecondaryDefault, RGBA radioButtonSecondaryHover, RGBA radioButtonSecondaryDown,
                                 RGBA radioButtonBorderDefault, RGBA radioButtonBorderHover, RGBA radioButtonBorderDown,
                                 RGBA selectedRadioButtonPrimaryDefault, RGBA selectedRadioButtonPrimaryHover, RGBA selectedRadioButtonPrimaryDown,
                                 RGBA selectedRadioButtonSecondaryDefault, RGBA selectedRadioButtonSecondaryHover, RGBA selectedRadioButtonSecondaryDown,
                                 RGBA selectedRadioButtonBorderDefault, RGBA selectedRadioButtonBorderHover, RGBA selectedRadioButtonBorderDown);

    /**
     * @brief Updates the editbox colors.
     *
     * @param editbox               The editbox color.
     * @param editboxBorderDefault  The editbox border default color.
     * @param editboxBorderSelected The editbox border selected color.
     */
    void updateEditboxColors(RGBA editbox, RGBA editboxBorderDefault, RGBA editboxBorderSelected);

    /**
     * @brief Updates the drop-down list combobox colors.
     *
     * @param ddlCombobox                       The drop-down list combobox color.
     * @param ddlComboboxBorder                 The drop-down list combobox border color.
     * @param ddlComboboxItemBackground         The drop-down list combobox item background color.
     * @param ddlComboboxSelectedItemBackground The drop-down list combobox selected item background color.
     * @param ddlComboboxDropdownlistBorder     The drop-down list combobox dropdownlist border color.
     * @param ddlComboboxItemTextDefault        The drop-down list combobox item text default color.
     * @param ddlComboboxItemTextSelected       The drop-down list combobox item text selected color.
     */
    void updateDDLComboboxColors(RGBA ddlCombobox, RGBA ddlComboboxBorder, RGBA ddlComboboxItemBackground, RGBA ddlComboboxSelectedItemBackground,
                                 RGBA ddlComboboxDropdownlistBorder, RGBA ddlComboboxItemTextDefault, RGBA ddlComboboxItemTextSelected);
};

/**
 * @class UIFonts
 *
 * @brief Singleton class encapsulating and managing the UI font/text format objects.
 *
 * @note This class is used by UIElements class.
 */
class UIFonts
{
public:
    /// [DEFAULT APPLICATION UI FONTS/TEXT FORMATS]

    MyFont caption = MyFont(L"Tahoma", 23, FW_NORMAL, CLEARTYPE_QUALITY);     // Caption font.
    MyFont editbox = MyFont(L"Tahoma", 29, FW_ULTRALIGHT, CLEARTYPE_QUALITY); // Editbox font.
    MyFont ddlCombobox = MyFont(L"Tahoma", 24, FW_NORMAL, CLEARTYPE_QUALITY); // Drop-down list combobox font.
    MyFont heading = MyFont(L"Tahoma", 26, FW_MEDIUM, CLEARTYPE_QUALITY);     // Heading font.
    MyFont note = MyFont(L"Tahoma", 20, FW_NORMAL, CLEARTYPE_QUALITY);        // Note font.

    // DirectWrite text formats.
    MyDWTextFormat buttonTextFormat = MyDWTextFormat(L"Tahoma", 20, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Updates the fonts.
     *
     * @param caption     The caption font.
     * @param editbox     The editbox font.
     * @param ddlCombobox The drop-down list combobox font.
     * @param heading     The heading font.
     * @param note        The note font.
     */
    void updateFonts(MyFont caption, MyFont editbox, MyFont ddlCombobox, MyFont heading, MyFont note);
};

/**
 * @class UIIcons
 *
 * @brief Singleton class encapsulating and managing the UI icon objects.
 *
 * @note This class is used by UIElements class.
 */
class UIIcons
{
public:
    /// [DEFAULT APPLICATION UI ICONS]

    // const std::unique_ptr<MyIcon> pCrossGrey = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON2);
    // const std::unique_ptr<MyIcon> pCrossWhite = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON3);
    // const std::unique_ptr<MyIcon> pCrossBlack = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON1);
    // const std::unique_ptr<MyIcon> pMinusGrey = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON5);
    // const std::unique_ptr<MyIcon> pMinusWhite = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON6);
    // const std::unique_ptr<MyIcon> pMinusBlack = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON4);
};

/**
 * @class UIImages
 *
 * @brief Singleton class encapsulating and managing the UI image objects.
 *
 * @note This class is used by UIElements class.
 */
class UIImages
{
public:
    /// [DEFAULT APPLICATION UI IMAGES]

    // Default images.
    const std::unique_ptr<MyD2D1Image> pWICCrossGrey = std::make_unique<MyD2D1Image>(IDR_IMAGE1, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICCrossWhite = std::make_unique<MyD2D1Image>(IDR_IMAGE2, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICCrossBlack = std::make_unique<MyD2D1Image>(IDR_IMAGE3, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICSquareGrey = std::make_unique<MyD2D1Image>(IDR_IMAGE4, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICSquareWhite = std::make_unique<MyD2D1Image>(IDR_IMAGE5, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICSquareBlack = std::make_unique<MyD2D1Image>(IDR_IMAGE6, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICMinusGrey = std::make_unique<MyD2D1Image>(IDR_IMAGE7, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICMinusWhite = std::make_unique<MyD2D1Image>(IDR_IMAGE8, L"IMAGE", 20, 20);
    const std::unique_ptr<MyD2D1Image> pWICMinusBlack = std::make_unique<MyD2D1Image>(IDR_IMAGE9, L"IMAGE", 20, 20);

    // Non-client images.
    std::unique_ptr<MyD2D1Image> pNonClientCloseButtonDefault;
    std::unique_ptr<MyD2D1Image> pNonClientCloseButtonHover;
    std::unique_ptr<MyD2D1Image> pNonClientCloseButtonDown;
    std::unique_ptr<MyD2D1Image> pNonClientMaximizeButtonDefault;
    std::unique_ptr<MyD2D1Image> pNonClientMaximizeButtonHover;
    std::unique_ptr<MyD2D1Image> pNonClientMaximizeButtonDown;
    std::unique_ptr<MyD2D1Image> pNonClientMinimizeButtonDefault;
    std::unique_ptr<MyD2D1Image> pNonClientMinimizeButtonHover;
    std::unique_ptr<MyD2D1Image> pNonClientMinimizeButtonDown;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Updates the non-client button images.
     *
     * @param nonClientCloseButtonDefault     The non-client close button default image.
     * @param nonClientCloseButtonHover       The non-client close button hover image.
     * @param nonClientCloseButtonDown        The non-client close button down image.
     * @param nonClientMaximizeButtonDefault  The non-client maximize button default image.
     * @param nonClientMaximizeButtonHover    The non-client maximize button hover image.
     * @param nonClientMaximizeButtonDown     The non-client maximize button down image.
     * @param nonClientMinimizeButtonDefault  The non-client minimize button default image.
     * @param nonClientMinimizeButtonHover    The non-client minimize button hover image.
     * @param nonClientMinimizeButtonDown     The non-client minimize button down image.
     */
    void updateNonClientButtonImages(MyD2D1Image &nonClientCloseButtonDefault, MyD2D1Image &nonClientCloseButtonHover, MyD2D1Image &nonClientCloseButtonDown,
                                     MyD2D1Image &nonClientMaximizeButtonDefault, MyD2D1Image &nonClientMaximizeButtonHover, MyD2D1Image &nonClientMaximizeButtonDown,
                                     MyD2D1Image &nonClientMinimizeButtonDefault, MyD2D1Image &nonClientMinimizeButtonHover, MyD2D1Image &nonClientMinimizeButtonDown);
};

/**
 * @class UIRectangles
 *
 * @brief Singleton class encapsulating and managing the UI rectangle objects.
 *
 * @note This class is used by UIElements class.
 */
class UIRectangles
{
public:
    UIRectangles();

public:
    /// [APPLICATION UI RECTANGLES]

    RECT rectCaption;          // Caption bar area rectangle.
    RECT rectSizeBorderTop;    // Top border area rectangle.
    RECT rectSizeBorderBottom; // Bottom border area rectangle.
    RECT rectSizeBorderLeft;   // Left border area rectangle.
    RECT rectSizeBorderRight;  // Right border area rectangle.
};

/**
 * @class UIPointers
 *
 * @brief Singleton class encapsulating and managing the UI pointers.
 *
 * @note This class is used by UIElements class.
 */
class UIPointers
{
public:
    /// [APPLICATION UI RELATED POINTERS]

    // Reserved code, likely to be removed in the future.
    // Pointer that holds the current application border color.
    // D2D1::ColorF *pCurrentBorderColor = nullptr;
};

/**
 * @class UIMiscs
 *
 * @brief Singleton class encapsulating and managing the UI miscellaneous variables.
 *
 * @note This class is used by UIElements class.
 */
class UIMiscs
{
public:
    /// [APPLICATION MISCELLANEOUS UI VARIABLES]

    HWND hWndNonClientCloseButton = nullptr;
    HWND hWndNonClientMaximizeButton = nullptr;
    HWND hWndNonClientMinimizeButton = nullptr;
};

/**
 * @class UIElements
 *
 * @brief Singleton class encapsulating and managing all the UI-related variables.
 * @brief This class provides interface to all the UI-related variables.
 */
class UIElements
{
public:
    /**
     * @brief Default constructor, checks if my GDI+ and Direct2D engine is initialized.
     * @note This constructor exists to prevent the creation of UIElements object before the initialization of my GDI+ and Direct2D engine.
     *
     * @param pMyGDIPEngine Pointer to my GDI+ engine.
     * @param pMyD2D1Engine Pointer to my Direct2D engine.
     */
    UIElements(MyGDIPEngine *pMyGDIPEngine, MyD2D1Engine *pMyD2D1Engine);

public:
    /// [APPLICATION UI ELEMENTS]

    // Application Direct2D main render target and its device-dependent resources.
    ID2D1DCRenderTarget *pRenderTarget = nullptr;
    ID2D1SolidColorBrush *pSolidColorBrushCaptionBackground = nullptr;
    ID2D1SolidColorBrush *pSolidColorBrushBackground = nullptr;
    ID2D1SolidColorBrush *pSolidColorBrushBorderActive = nullptr;
    ID2D1SolidColorBrush *pSolidColorBrushBorderInactive = nullptr;

    // Application UI-related interfaces.
    UIColors colors;
    UIFonts fonts;
    UIIcons icons;
    UIImages images;
    UIRectangles rectangles;
    UIPointers pointers;
    UIMiscs miscs;

public:
    // [GENERAL FUNCTIONS]

    /**
     * @brief (Re)create the D2D1 device resources.
     *
     * @param hWnd                 Handle to the main window.
     * @param recreateRenderTarget Specifies whether to recreate the render target.
     *
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createD2D1DeviceResources(HWND hWnd, bool recreateRenderTarget);

    /**
     * @brief Release the D2D1 device resources.
     */
    void releaseD2D1DeviceResources();
};

#endif // UI_H