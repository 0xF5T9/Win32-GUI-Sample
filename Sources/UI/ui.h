/**
 * @file ui.h
 * @brief Header file containing classes that related to UI management.
 */

#ifndef UI_H
#define UI_H
#include "../../Includes/my_enums.h" // My enum definitions.

/**
 * @struct RGBA
 * @brief A simple struct that represents a color with RGBA values.
 */
struct RGBA
{
    BYTE red;
    BYTE green;
    BYTE blue;
    BYTE alpha = 255;

    RGBA(BYTE red, BYTE green, BYTE blue, BYTE alpha = 255) : red(red), green(green), blue(blue), alpha(alpha) {}
};

/**
 * @class MyColor
 *
 * @brief Represents a color. (Wrapped COLORREF, Gdiplus::Color, HBRUSH and D2D1::ColorF)
 */
class MyColor
{
public:
    /**
     * @brief Constructor 1.
     *
     * @param red   The red value of the color.
     * @param green The green value of the color.
     * @param blue  The blue value of the color.
     * @param alpha The alpha value of the color.
     */
    MyColor(BYTE red, BYTE green, BYTE blue, BYTE alpha = 255);

    /**
     * @brief Constructor 2.
     *
     * @param colors The RGBA struct containing the color values.
     */
    MyColor(RGBA colors);

    /**
     * @brief Copy constructor.
     *
     * @param other The MyColor object to be copied.
     */
    MyColor(const MyColor &other);

    /**
     * @brief Assignment operator.
     *
     * @param other The MyColor object to be assigned.
     */
    MyColor &operator=(const MyColor &other);

    /**
     * @brief Destructor for the MyColor object.
     */
    ~MyColor();

private:
    inline static UINT totalInstances = 0; // Tracks the total number of instances.

    // Allocation variable(s).
    BYTE red, green, blue, alpha; // Color values.

    // Variant(s).
    COLORREF clr;                                               // COLORREF variant of the color.
    Gdiplus::Color gdipColor;                                   // Gdiplus::Color variant of the color.
    HBRUSH hBrush;                                              // HBRUSH variant of the color.
    D2D1::ColorF d2d1Color = D2D1::ColorF(D2D1::ColorF::Black); // D2D1::ColorF variant of the color.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the total instances of MyColor.
     *
     * @return Returns the total instances of MyColor.
     */
    inline static UINT getTotalInstances();

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the red value of the color.
     *
     * @return Returns the red value of the color.
     */
    BYTE getRed() const;

    /**
     * @brief Get the green value of the color.
     *
     * @return Returns the green value of the color.
     */
    BYTE getGreen() const;

    /**
     * @brief Get the blue value of the color.
     *
     * @return Returns the blue value of the color.
     */
    BYTE getBlue() const;

    /**
     * @brief Get the alpha value of the color.
     *
     * @return Returns the alpha value of the color.
     */
    BYTE getAlpha() const;

    /**
     * @brief Get the COLORREF variant (reference) of the color.
     *
     * @return Returns the COLORREF variant of the color.
     */
    const COLORREF &getCOLORREF() const;

    /**
     * @brief Get the Gdiplus::Color variant (reference) of the color.
     *
     * @returns Returns the Gdiplus::Color variant of the color.
     */
    Gdiplus::Color &getGDIPColor();

    /**
     * @brief Get the D2D1::ColorF variant (reference) of the color.
     *
     * @returns Returns the D2D1::ColorF variant of the color.
     */
    D2D1::ColorF &getD2D1Color();

    /**
     * @brief Get the HBRUSH variant (reference) of the color.
     *
     * @returns Returns the HBRUSH variant of the color.
     */
    HBRUSH &getHBRUSH();

    /**
     * @brief Update the color values.
     *
     * @param red   The red value of the color.
     * @param green The green value of the color.
     * @param blue  The blue value of the color.
     * @param alpha The alpha value of the color.
     *
     * @return Returns true if the color values were successfully updated, false otherwise.
     */
    bool update(BYTE red, BYTE green, BYTE blue, BYTE alpha = 255);
};

/**
 * @class MyFont
 *
 * @brief Represents a font. (Wrapped HFONT)
 */
class MyFont
{
public:
    /**
     * @brief Default constructor.
     *
     * @param name    The name of the font.
     * @param size    The size of the font.
     * @param weight  The weight of the font.
     * @param quality The quality of the font.
     */
    MyFont(std::wstring name, USHORT size, USHORT weight = FW_NORMAL, DWORD quality = DEFAULT_QUALITY);

    /**
     * @brief Copy constructor.
     *
     * @param other The MyFont object to be copied.
     */
    MyFont(const MyFont &other);

    /**
     * @brief Assignment operator.
     *
     * @param other The MyFont object to be assigned.
     */
    MyFont &operator=(const MyFont &other);

    /**
     * @brief Destructor for the MyFont object.
     */
    ~MyFont();

private:
    inline static UINT totalInstances = 0; // Tracks the total number of instances.

    // Allocation variable(s).
    std::wstring name; // Name of the font.
    USHORT size;       // Size of the font.
    USHORT weight;     // Weight of the font.
    DWORD quality;     // Quality of the font.

    // Variant(s).
    HFONT hFont; // HFONT variant of the font.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the total instances of MyFont.
     *
     * @return Returns the total instances of MyFont.
     */
    inline static UINT getTotalInstances();

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @bried Get the name of the font.
     *
     * @return Returns the name of the font.
     */
    std::wstring getName() const;

    /**
     * @brief Get the size of the font.
     *
     * @return Returns the size of the font.
     */
    USHORT getSize() const;

    /**
     * @brief Get the weight of the font.
     *
     * @return Returns the weight of the font.
     */
    USHORT getWeight() const;

    /**
     * @brief Get the quality of the font.
     *
     * @return Returns the quality of the font.
     */
    DWORD getQuality() const;

    /**
     * @brief Get the HFONT variant (reference) of the font.
     *
     * @returns Returns the HFONT variant (reference) of the font.
     */
    HFONT &getHFONT();

    /**
     * @brief Update the font values.
     *
     * @param name    The name of the font.
     * @param size    The size of the font.
     * @param weight  The weight of the font.
     * @param quality The quality of the font.
     *
     * @return Returns true if the font values were successfully updated, false otherwise.
     */
    bool update(std::wstring name, USHORT size, USHORT weight = FW_NORMAL, DWORD quality = DEFAULT_QUALITY);
};

/**
 * @class MyIcon
 *
 * @brief Represents a icon. (Wrapped HICON)
 */
class MyIcon
{
public:
    /**
     * @brief Default constructor.
     *
     * @param hInstance      The handle to the instance of the module whose executable file contains the icon to be loaded.
     * @param width          The width of the icon.
     * @param height         The height of the icon.
     * @param resourceID     The resource ID of the icon.
     * @param loadImageFlags Flags that specify how the icon is to be loaded.
     */
    MyIcon(HINSTANCE hInstance, INT width, INT height, INT resourceID, UINT loadImageFlags = NULL);

    /**
     * @brief Copy constructor.
     *
     * @param other The MyIcon object to be copied.
     */
    MyIcon(const MyIcon &other);

    /**
     * @brief Assignment operator.
     *
     * @param other The MyIcon object to be assigned.
     */
    MyIcon &operator=(const MyIcon &other);

    /**
     * @brief Destructor for the MyIcon object.
     */
    ~MyIcon();

private:
    inline static UINT totalInstances = 0; // Tracks the total number of instances.

    // Allocation variable(s).
    HINSTANCE hInstance; // Handle to the instance of the module whose executable file contains the icon to be loaded.
    INT width;           // Width of the icon.
    INT height;          // Height of the icon.
    INT resourceID;      // Resource ID of the icon.
    UINT loadImageFlags; // Flags that specify how the icon is to be loaded.

    // Variant(s).
    HICON hIcon; // HICON variant of the icon.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the total instances of MyIcon.
     *
     * @return Returns the total instances of MyIcon.
     */
    inline static UINT getTotalInstances();

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the handle to the instance of the module whose executable file contains the icon to be loaded.
     *
     * @return Returns the handle to the instance of the module whose executable file contains the icon to be loaded.
     */
    HINSTANCE getHINSTANCE() const;

    /**
     * @brief Get the width of the icon.
     *
     * @return Returns the width of the icon.
     */
    INT getWidth() const;

    /**
     * @brief Get the height of the icon.
     *
     * @return Returns the height of the icon.
     */
    INT getHeight() const;

    /**
     * @brief Get the resource ID of the image.
     *
     * @return Returns the resource ID of the image.
     */
    INT getResourceID() const;

    /**
     * @brief Get the flags that specify how the icon is to be loaded.
     *
     * @return Returns the flags that specify how the icon is to be loaded.
     */
    UINT getLoadImageFlags() const;

    /**
     * @brief Get the HICON variant (reference) of the icon.
     *
     * @return Returns the HICON variant (reference) of the icon.
     */
    HICON &getHICON();

    /**
     * @brief Update the icon values.
     *
     * @param hInstance      The handle to the instance of the module whose executable file contains the icon to be loaded.
     * @param width          The width of the icon.
     * @param height         The height of the icon.
     * @param resourceID     The resource ID of the icon.
     * @param loadImageFlags Flags that specify how the icon is to be loaded.
     *
     * @return Returns true if the icon values were successfully updated, false otherwise.
     */
    bool update(HINSTANCE hInstance, INT width, INT height, INT resourceID, UINT loadImageFlags = NULL);
};

/**
 * @class MyImage
 *
 * @brief Represents a image. (Wrapped Gdiplus::Image)
 */
class MyImage
{
public:
    /**
     * @brief Default constructor.
     *
     * @param resourceID The resource ID of the image.
     * @param format     The format of the image.
     */
    MyImage(INT resourceID, MyImageFormat format);

    /**
     * @brief Copy constructor.
     *
     * @param other The MyImage object to be copied.
     */
    MyImage(const MyImage &other);

    /**
     * @brief Assignment operator.
     *
     * @param other The MyImage object to be assigned.
     */
    MyImage &operator=(const MyImage &other);

    /**
     * @brief Destructor for the MyImage object.
     */
    ~MyImage();

private:
    inline static UINT totalInstances = 0; // Tracks the total number of instances.

    // Allocation variable(s).
    IStream *pStream = nullptr; // Pointer to the resource stream.
    INT resourceID;             // Resource ID of the image.
    MyImageFormat format;       // Format of the image.

    // Variant(s).
    Gdiplus::Image *pImage = nullptr; // Gdiplus::Image variant of the image.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the total instances of MyImage.
     *
     * @return Returns the total instances of MyImage.
     */
    inline static UINT getTotalInstances();

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the resource ID of the image.
     *
     * @return Returns the resource ID of the image.
     */
    INT getResourceID() const;

    /**
     * @brief Get the format of the image.
     *
     * @return Returns the format of the image.
     */
    MyImageFormat getFormat() const;

    /**
     * @brief Get the Gdiplus::Image variant of the image.
     *
     * @returns Returns the Gdiplus::Image variant of the image.
     */
    Gdiplus::Image *getGDIPImage() const;

    /**
     * @brief Update the image values.
     *
     * @param resourceID  The resource ID of the image.
     * @param format      The format of the image.
     *
     * @return Returns true if the color values were successfully updated, false otherwise.
     */
    bool update(INT resourceID, MyImageFormat format);
};

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
 * @brief Singleton class encapsulating and managing the UI font objects.
 *
 * @note This class is used by UIElements class.
 */
class UIFonts
{
public:
    /// [DEFAULT APPLICATION UI FONTS]

    MyFont caption = MyFont(L"Tahoma", 23, FW_NORMAL, CLEARTYPE_QUALITY);     // Caption font.
    MyFont button = MyFont(L"Tahoma", 24, FW_NORMAL, CLEARTYPE_QUALITY);      // Button font.
    MyFont editbox = MyFont(L"Tahoma", 29, FW_ULTRALIGHT, CLEARTYPE_QUALITY); // Editbox font.
    MyFont ddlCombobox = MyFont(L"Tahoma", 24, FW_NORMAL, CLEARTYPE_QUALITY); // Drop-down list combobox font.
    MyFont heading = MyFont(L"Tahoma", 26, FW_MEDIUM, CLEARTYPE_QUALITY);     // Heading font.
    MyFont note = MyFont(L"Tahoma", 22, FW_NORMAL, CLEARTYPE_QUALITY);        // Note font.

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

    const std::unique_ptr<MyIcon> pCrossGrey = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON2);
    const std::unique_ptr<MyIcon> pCrossWhite = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON3);
    const std::unique_ptr<MyIcon> pCrossBlack = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON1);
    const std::unique_ptr<MyIcon> pMinusGrey = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON5);
    const std::unique_ptr<MyIcon> pMinusWhite = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON6);
    const std::unique_ptr<MyIcon> pMinusBlack = std::make_unique<MyIcon>(GetModuleHandle(NULL), 512, 512, IDI_ICON4);
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
    const std::unique_ptr<MyImage> pCrossGrey = std::make_unique<MyImage>(IDB_PNG1, MyImageFormat::PNG);
    const std::unique_ptr<MyImage> pCrossWhite = std::make_unique<MyImage>(IDB_PNG2, MyImageFormat::PNG);
    const std::unique_ptr<MyImage> pCrossBlack = std::make_unique<MyImage>(IDB_PNG3, MyImageFormat::PNG);
    const std::unique_ptr<MyImage> pMinusGrey = std::make_unique<MyImage>(IDB_PNG4, MyImageFormat::PNG);
    const std::unique_ptr<MyImage> pMinusWhite = std::make_unique<MyImage>(IDB_PNG5, MyImageFormat::PNG);
    const std::unique_ptr<MyImage> pMinusBlack = std::make_unique<MyImage>(IDB_PNG6, MyImageFormat::PNG);

    // Non-client images.
    std::unique_ptr<MyImage> pNonClientCloseButtonDefault;
    std::unique_ptr<MyImage> pNonClientCloseButtonHover;
    std::unique_ptr<MyImage> pNonClientCloseButtonDown;
    std::unique_ptr<MyImage> pNonClientMinimizeButtonDefault;
    std::unique_ptr<MyImage> pNonClientMinimizeButtonHover;
    std::unique_ptr<MyImage> pNonClientMinimizeButtonDown;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Updates the non-client button images.
     *
     * @param nonClientCloseButtonDefault     The non-client close button default image.
     * @param nonClientCloseButtonHover       The non-client close button hover image.
     * @param nonClientCloseButtonDown        The non-client close button down image.
     * @param nonClientMinimizeButtonDefault  The non-client minimize button default image.
     * @param nonClientMinimizeButtonHover    The non-client minimize button hover image.
     * @param nonClientMinimizeButtonDown     The non-client minimize button down image.
     */
    void updateNonClientButtonImages(MyImage &nonClientCloseButtonDefault, MyImage &nonClientCloseButtonHover, MyImage &nonClientCloseButtonDown,
                                     MyImage &nonClientMinimizeButtonDefault, MyImage &nonClientMinimizeButtonHover, MyImage &nonClientMinimizeButtonDown);
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

    D2D1::ColorF *pCurrentBorderColor = nullptr; // Pointer that holds the current application border color.
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
    /// [APPLICATION UI ELEMENTS]

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
     * @brief Show the total number of instances (Color, font, icon, image objects).
     */
    static void showTotalInstances();
};

#endif // UI_H