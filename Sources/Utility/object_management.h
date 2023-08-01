/**
 * @file object_management.h
 * @brief Header file containing classes that related to object management.
 */

#pragma once

/**
 * @struct MyColor
 * @brief Represents a color with various variant datatypes.
 */
struct MyColor
{
private:
    inline static UINT TotalInstances = 0;            // Tracks the total number of instances.
    inline static UINT TotalInitializedInstances = 0; // Tracks the total number of initialized instances.
    INT ID;                                           // The instance ID. (May coincide with other instances)
    bool isInitialized = false;                       // Indicate whether the instance is initialized.

private:
    BYTE Red = 0;                  // Red value of the color.
    BYTE Green = 0;                // Green value of the color.
    BYTE Blue = 0;                 // Blue value of the color.
    BYTE Alpha = 0;                // Alpha value of the color.
    COLORREF CLR = NULL;           // COLORREF variant of the color.
    Gdiplus::Color GDIP_CL = NULL; // Gdiplus::Color variant of the color.
    HBRUSH HBR = nullptr;          // HBRUSH variant of the color. (Unmanaged object)

public:
    /**
     * @brief Default constructor. (Uninitialized state)
     */
    MyColor();

    /**
     * @brief Default constructor.
     *
     * @param Red   The red value of the color.
     * @param Green The green value of the color.
     * @param Blue  The blue value of the color.
     * @param Alpha The alpha value of the color.
     * @param ID    The instance ID. (Default: 0)
     */
    MyColor(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha = 255, INT ID = 0);

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
     *
     * @return Reference to the assigned MyColor object.
     */
    MyColor &operator=(const MyColor &other);

    /**
     * @brief Destructor for the MyColor object.
     */
    ~MyColor();

private:
    /**
     * @brief Destroys the unmanaged object(s).
     *
     * @return Returns true if the object(s) was successfully destroyed, false otherwise.
     */
    bool DestroyUnmanagedObjects();

public:
    /**
     * @brief Get the total number of existing MyColor instances.
     *
     * @return Returns the total number of existing MyColor instances.
     */
    static UINT GetTotalInstances();

    /**
     * @brief Get the total number of existing initialized MyColor instances.
     *
     * @return Returns the total number of existing initialized MyColor instances.
     */
    static UINT GetTotalInitializedInstances();

public:
    /**
     * @brief Get the instance ID.
     *
     * @return Returns the instance ID.
     */
    INT GetInstanceID();

    /**
     * @brief Set the new instance ID.
     *
     * @param ID The new instance ID.
     */
    void SetInstanceID(INT ID);

    /**
     * @brief Get the red value of the color.
     *
     * @return Returns the red value of the color.
     */
    BYTE GetRed();

    /**
     * @brief Get the green value of the color.
     *
     * @return Returns the green value of the color.
     */
    BYTE GetGreen();

    /**
     * @brief Get the blue value of the color.
     *
     * @return Returns the blue value of the color.
     */
    BYTE GetBlue();

    /**
     * @brief Get the alpha value of the color.
     *
     * @return Returns the alpha value of the color.
     */
    BYTE GetAlpha();

    /**
     * @brief Get the COLORREF variant of the color.
     *
     * @return Returns the COLORREF variant of the color.
     */
    COLORREF GetCLR();

    /**
     * @brief Get the Gdiplus::Color variant of the color.
     *
     * @return Returns the Gdiplus::Color variant of the color.
     */
    Gdiplus::Color GetGDIPColor();

    /**
     * @brief Get the Gdiplus::Color variant (reference) of the color.
     *
     * @return Returns the Gdiplus::Color variant (reference) of the color.
     */
    Gdiplus::Color &GetGDIPColorRef();

    /**
     * @brief Get the HBRUSH variant of the color.
     *
     * @return Returns the HBRUSH variant of the color.
     */
    HBRUSH GetHBR();

    /**
     * @brief Get the HBRUSH variant (reference) of the color.
     *
     * @return Returns the HBRUSH variant (reference) of the color.
     */
    HBRUSH &GetHBRRef();

    /**
     * @brief Updates the MyColor object with new color values.
     *
     * @param Red   The red value of the color.
     * @param Green The green value of the color.
     * @param Blue  The blue value of the color.
     * @param Alpha The alpha value of the color.
     *
     * @return Returns true if successfully updated the object, false otherwise.
     */
    bool Update(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha = 255);
};

/**
 * @struct MyFont
 * @brief Represents a font with various variant datatypes.
 */
struct MyFont
{
private:
    inline static UINT TotalInstances = 0;            // Tracks the total number of instances.
    inline static UINT TotalInitializedInstances = 0; // Tracks the total number of initialized instances.
    INT ID;                                           // The instance ID. (May coincide with other instances)
    bool isInitialized = false;                       // Indicate whether the instance is initialized.

private:
    std::wstring Name;   // The font name.
    USHORT Size;         // The font size.
    USHORT Weight;       // The font weight.
    DWORD Quality;       // The font quality.
    HFONT HFO = nullptr; // HFONT variant of the font. (Unmanaged object)

public:
    /**
     * @brief Default constructor. (Uninitialized state)
     */
    MyFont();

    /**
     * @brief Default constructor.
     *
     * @param Name    The name of the font.
     * @param Size    The size of the font.
     * @param Weight  The weight of the font.
     * @param Quality The quality of the font.
     * @param ID      The instance ID. (Default: 0)
     */
    MyFont(std::wstring Name, USHORT Size, USHORT Weight = FW_NORMAL, DWORD Quality = DEFAULT_QUALITY, INT ID = 0);

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
     *
     * @return Reference to the assigned MyFont object.
     */
    MyFont &operator=(const MyFont &other);

    /**
     * @brief Destructor for the MyFont object.
     */
    ~MyFont();

private:
    /**
     * @brief Destroys the unmanaged object(s).
     *
     * @return Returns true if the object(s) was successfully destroyed, false otherwise.
     */
    bool DestroyUnmanagedObjects();

public:
    /**
     * @brief Get the total number of existing MyFont instances.
     *
     * @return Returns the total number of existing MyFont instances.
     */
    static UINT GetTotalInstances();

    /**
     * @brief Get the total number of existing initialized MyFont instances.
     *
     * @return Returns the total number of existing initialized MyFont instances.
     */
    static UINT GetTotalInitializedInstances();

public:
    /**
     * @brief Get the instance ID.
     *
     * @return Returns the instance ID.
     */
    INT GetInstanceID();

    /**
     * @brief Set the new instance ID.
     *
     * @param ID The new instance ID.
     */
    void SetInstanceID(INT ID);

    /**
     * @brief Get the font name.
     *
     * @return Returns the font name.
     */
    std::wstring GetName();

    /**
     * @brief Get the font size.
     *
     * @return Returns the font size.
     */
    USHORT GetSize();

    /**
     * @brief Get the font weight.
     *
     * @return Returns the font weight.
     */
    USHORT GetWeight();

    /**
     * @brief Get the font quality.
     *
     * @return Returns the font quality.
     */
    DWORD GetQuality();

    /**
     * @brief Get the HFONT variant of the font.
     *
     * @return Returns the HFONT variant of the font.
     */
    HFONT GetHFO();

    /**
     * @brief Get the HFONT variant (reference) of the font.
     *
     * @return Returns the HFONT variant (reference) of the font.
     */
    HFONT &GetHFORef();

    /**
     * @brief Updates the MyFont object with new font values.
     *
     * @param Name    The font name.
     * @param Size    The font size.
     * @param Weight  The font weight.
     * @param Quality The font quality.
     *
     * @return Returns true if successfully updated the object, false otherwise.
     */
    bool Update(std::wstring Name, USHORT Size, USHORT Weight = FW_NORMAL, DWORD Quality = DEFAULT_QUALITY);
};

/**
 * @struct MyIcon
 * @brief Represents an icon with various variant datatypes.
 */
struct MyIcon
{
private:
    inline static UINT TotalInstances = 0;            // Tracks the total number of instances.
    inline static UINT TotalInitializedInstances = 0; // Tracks the total number of initialized instances.
    INT ID;                                           // The instance ID. (May coincide with other instances)
    bool isInitialized = false;                       // Indicate whether the instance is initialized.

private:
    HINSTANCE hInstance;  // Handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
    INT Width;            // The width of the icon.
    INT Height;           // The height of the icon.
    INT ResourceID;       // The resource ID of the icon.
    UINT HICON_fuLoad;    // Extra fuLoad flag(s) for the LoadImage() function.
    HICON HICO = nullptr; // HICON variant of the icon. (Unmanaged object)

public:
    /**
     * @brief Default constructor. (Uninitialized state)
     */
    MyIcon();

    /**
     * @brief Default constructor.
     *
     * @param hInstance    Handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     * @param Width        The width of the icon.
     * @param Height       The height of the icon.
     * @param ResourceID   The resource ID of the icon.
     * @param HICON_fuLoad Extra fuLoad flag(s) for the LoadImage() function.
     * @param ID           The instance ID. (Default: 0)
     */
    MyIcon(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad = NULL, INT ID = 0);

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
     *
     * @return Reference to the assigned MyIcon object.
     */
    MyIcon &operator=(const MyIcon &other);

    /**
     * @brief Destructor for the MyIcon object.
     */
    ~MyIcon();

private:
    /**
     * @brief Destroys the unmanaged object(s).
     *
     * @return Returns true if the object(s) was successfully destroyed, false otherwise.
     */
    bool DestroyUnmanagedObjects();

public:
    /**
     * @brief Get the total number of existing MyIcon instances.
     *
     * @return Returns the total number of existing MyIcon instances.
     */
    static UINT GetTotalInstances();

    /**
     * @brief Get the total number of existing initialized MyIcon instances.
     *
     * @return Returns the total number of existing initialized MyIcon instances.
     */
    static UINT GetTotalInitializedInstances();

public:
    /**
     * @brief Get the instance ID.
     *
     * @return Returns the instance ID.
     */
    INT GetInstanceID();

    /**
     * @brief Set the new instance ID.
     *
     * @param ID The new instance ID.
     */
    void SetInstanceID(INT ID);

    /**
     * @brief Get handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     *
     * @return Returns the handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     */
    HINSTANCE GetHINSTANCE();

    /**
     * @brief Get the width of the icon.
     *
     * @return Returns the width of the icon.
     */
    INT GetWidth();

    /**
     * @brief Get the height of the icon.
     *
     * @return Returns the height of the icon.
     */
    INT GetHeight();

    /**
     * @brief Get the resource ID of the icon.
     *
     * @return Returns the resource ID of the icon.
     */
    INT GetResourceID();

    /**
     * @brief Get the load image flags of the icon.
     *
     * @return Returns the load image flags of the icon.
     */
    UINT GetLoadImageFlag();

    /**
     * @brief Get the HICON variant of the icon.
     *
     * @return Returns the HICON variant of the icon.
     */
    HICON GetHICO();

    /**
     * @brief Get the HICON variant (reference) of the icon.
     *
     * @return Returns the HICON variant (reference) of the icon.
     */
    HICON &GetHICORef();

    /**
     * @brief Updates the MyIcon object with new icon values.
     *
     * @param hInstance    The new handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     * @param Width        The new width of the icon.
     * @param Height       The new height of the icon.
     * @param ResourceID   The new resource ID of the icon.
     * @param HICON_fuLoad Extra fuLoad flag(s) for the LoadImage() function.
     *
     * @return Returns true if successfully updated the object, false otherwise.
     */
    bool Update(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad = NULL);
};

/**
 * @struct MyImage
 * @brief Represents an image with various variant datatypes.
 */
struct MyImage
{
private:
    inline static UINT TotalInstances = 0;            // Tracks the total number of instances.
    inline static UINT TotalInitializedInstances = 0; // Tracks the total number of initialized instances.
    INT ID = 0;                                       // The instance ID. (May coincide with other instances)
    bool isInitialized = false;                       // Indicate whether the instance is initialized.

private:
    INT ResourceID = 0;               // The resource ID of the icon.
    ImageFormat Format;               // The image format.
    IStream *pStream = nullptr;       // Pointer to the resource stream.
    Gdiplus::Image *pImage = nullptr; // Gdiplus::Image variant of the image. (Unmanaged object)

public:
    /**
     * @brief Default constructor. (Uninitialized state)
     */
    MyImage();

    /**
     * @brief Default constructor.
     *
     * @param ResourceID The resource ID of the image.
     * @param Format     The image format.
     * @param ID         The instance ID. (Default: 0)
     */
    MyImage(INT ResourceID, ImageFormat Format, INT ID = 0);

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
     *
     * @return Reference to the assigned MyImage object.
     */
    MyImage &operator=(const MyImage &other);

    /**
     * @brief Destructor for the MyImage object.
     */
    ~MyImage();

private:
    /**
     * @brief Destroys the unmanaged object(s).
     *
     * @return Returns true if the object(s) was successfully destroyed, false otherwise.
     */
    bool DestroyUnmanagedObjects();

public:
    /**
     * @brief Get the total number of existing MyImage instances.
     *
     * @return Returns the total number of existing MyImage instances.
     */
    static UINT GetTotalInstances();

    /**
     * @brief Get the total number of existing initialized MyImage instances.
     *
     * @return Returns the total number of existing initialized MyImage instances.
     */
    static UINT GetTotalInitializedInstances();

public:
    /**
     * @brief Get the instance ID.
     *
     * @return Returns the instance ID.
     */
    INT GetInstanceID();

    /**
     * @brief Set the new instance ID.
     *
     * @param ID The new instance ID.
     */
    void SetInstanceID(INT ID);

    /**
     * @brief Get the resource ID of the image.
     *
     * @return Returns the resource ID of the image.
     */
    INT GetResourceID();

    /**
     * @brief Get the format of the image.
     *
     * @return Returns the format of the image.
     */
    ImageFormat GetFormat();

    /**
     * @brief Get the Gdiplus::Image variant of the image.
     *
     * @return Returns the Gdiplus::Image variant of the image.
     */
    Gdiplus::Image *GetImage();

    /**
     * @brief Get the Gdiplus::Image variant (reference) of the image.
     *
     * @return Returns the Gdiplus::Image variant (reference) of the image.
     */
    Gdiplus::Image *&GetImageRef();

    /**
     * @brief Updates the MyImage object with new image values.
     *
     * @param ResourceID The resource ID of the image.
     * @param Format     The image format.
     *
     * @return Returns true if successfully updated the object, false otherwise.
     */
    bool Update(INT ResourceID, ImageFormat Format);
};

/**
 * @class UIObjectManager_Colors
 * @brief This class provides an interface to access the default color objects and serves as an container for custom color objects.
 * @note This is an internal class of the UIObjectManager class.
 */
class UIObjectManager_Colors
{
private:
    std::vector<MyColor> Container; // Container for custom color objects.

public:
    /// [DEFAULT APPLICATION COLORS]
    MyColor Primary = MyColor();         // Primary color.
    MyColor Secondary = MyColor();       // Secondary color.
    MyColor Border = MyColor();          // Border color.
    MyColor Border_Inactive = MyColor(); // Inactive border color.
    MyColor Text = MyColor();            // Text color.
    MyColor Text_Inactive = MyColor();   // Inactive text color.
    MyColor Focus = MyColor();           // Focus color.
    MyColor CloseHover = MyColor();      // Close button hover state color.
    MyColor MinimizeHover = MyColor();   // Minimize button hover state color.
    MyColor NonClient_CloseButton_Background_OnHover = MyColor();    // Non-client close button hover state background color.
    MyColor NonClient_CloseButton_Background_OnDown = MyColor();     // Non-client close button down state background color.
    MyColor NonClient_MinimizeButton_Background_OnHover = MyColor(); // Non-client minimize button hover state background color.
    MyColor NonClient_MinimizeButton_Background_OnDown = MyColor();  // Non-client minimize button down state background color.
    MyColor Background = MyColor();      // Background color.

    MyColor Caption_Background = MyColor();    // Caption background color.
    MyColor Caption_Text = MyColor();          // Caption title text color.
    MyColor Caption_Text_Inactive = MyColor(); // Caption title inactive text color.

    MyColor StandardButton = MyColor();                // Standard button color.
    MyColor StandardButton_OnHover = MyColor();        // Standard button hover state color.
    MyColor StandardButton_OnDown = MyColor();         // Standard button down state color.
    MyColor StandardButton_Border = MyColor();         // Standard button border color.
    MyColor StandardButton_Border_OnHover = MyColor(); // Standard button hover state border color.
    MyColor StandardButton_Border_OnDown = MyColor();  // Standard button down state border color.
    MyColor StandardButton_Text_Default = MyColor();   // Standard button default text color.
    MyColor StandardButton_Text_Highlight = MyColor(); // Standard button highlight text color.

    MyColor RadioPrimary = MyColor();                   // Radio button primary color.
    MyColor RadioPrimary_OnHover = MyColor();           // Radio button hover state primary color.
    MyColor RadioPrimary_OnDown = MyColor();            // Radio button down state primary color.
    MyColor RadioSecondary = MyColor();                 // Radio button secondary color.
    MyColor RadioSecondary_OnHover = MyColor();         // Radio button hover state secondary color.
    MyColor RadioSecondary_OnDown = MyColor();          // Radio button down state secondary color.
    MyColor Radio_Border = MyColor();                   // Radio button border color.
    MyColor Radio_Border_OnHover = MyColor();           // Radio button hover state border color.
    MyColor Radio_Border_OnDown = MyColor();            // Radio button down state border color.
    MyColor SelectedRadioPrimary = MyColor();           // Selected radio button primary color.
    MyColor SelectedRadioPrimary_OnHover = MyColor();   // Selected radio button hover state primary color.
    MyColor SelectedRadioPrimary_OnDown = MyColor();    // Selected radio button down state primary color.
    MyColor SelectedRadioSecondary = MyColor();         // Selected radio button secondary color.
    MyColor SelectedRadioSecondary_OnHover = MyColor(); // Selected radio button hover state secondary color.
    MyColor SelectedRadioSecondary_OnDown = MyColor();  // Selected radio button down state secondary color.
    MyColor SelectedRadio_Border = MyColor();           // Selected radio button border color.
    MyColor SelectedRadio_Border_OnHover = MyColor();   // Selected radio button hover state border color.
    MyColor SelectedRadio_Border_OnDown = MyColor();    // Selected radio button down state border color.
    MyColor Radio_Text_Default = MyColor();             // Radio button default text color.
    MyColor Radio_Text_Highlight = MyColor();           // Radio button highlight text color.

    MyColor Edit = MyColor();                 // Edit color.
    MyColor Edit_Border = MyColor();          // Edit border color.
    MyColor Edit_Border_Selected = MyColor(); // Edit selected state border color.

    MyColor Combobox = MyColor();                                  // Combobox color.
    MyColor Combobox_Border = MyColor();                           // Combobox border color.
    MyColor Combobox_Dropdownlist_Background = MyColor();          // Combobox drop down list background color.
    MyColor Combobox_Dropdownlist_Background_Selected = MyColor(); // Combobox drop down list selected item background color.
    MyColor Combobox_Dropdownlist_Border = MyColor();              // Combobox drop down list border color.
    MyColor Combobox_Dropdownlist_Text_Default = MyColor();        // Combobox drop down list default item text color.
    MyColor Combobox_Dropdownlist_Text_Selected = MyColor();       // Combobox drop down list selected item text color.

    MyColor DEBUG = MyColor(0, 255, 0);                                      // Debug color.
    HBRUSH NullBrush = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH)); // Null brush.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Update the new colors for the main UI elements.
     *
     * @param Primary                                     New primary color.
     * @param Secondary                                   New secondary color.
     * @param Border                                      New border color.
     * @param Border_Inactive                             New inactive border color.
     * @param Text                                        New default text color.
     * @param Text_Inactive                               New inactive text color.
     * @param Focus                                       New focus color.
     * @param CloseHover                                  New close button hover state background color.
     * @param MinimizeHover                               New minimize button hover state background color.
     * @param NonClient_CloseButton_Background_OnHover    New non-client close button hover state background color.
     * @param NonClient_CloseButton_Background_OnDown     New non-client close button down state background color.
     * @param NonClient_MinimizeButton_Background_OnHover New non-client minimize button hover state background color.
     * @param NonClient_MinimizeButton_Background_OnDown  New non-client minimize button down state background color.
     * @param Background                                  New background color.
     */
    void UpdateMainColors(MyColor Primary, MyColor Secondary, MyColor Border, MyColor Border_Inactive,
                          MyColor Text, MyColor Text_Inactive, MyColor Focus, MyColor CloseHover, MyColor MinimizeHover,
                          MyColor NonClient_CloseButton_Background_OnHover, MyColor NonClient_CloseButton_Background_OnDown,
                          MyColor NonClient_MinimizeButton_Background_OnHover, MyColor NonClient_MinimizeButton_Background_OnDown, MyColor Background);

    /**
     * @brief Update the new colors for the caption elements.
     *
     * @param Caption_Background    New caption background color.
     * @param Caption_Text          New default caption text color.
     * @param Caption_Text_Inactive New inactive caption text color.
     */
    void UpdateCaptionColors(MyColor Caption_Background, MyColor Caption_Text, MyColor Caption_Text_Inactive);

    /**
     * @brief Update the new colors for the button elements.
     *
     * @param StandardButton                New standard button color.
     * @param StandardButton_OnHover        New standard button hover state color.
     * @param StandardButton_OnDown         New standard button down state color.
     * @param StandardButton_Border         New Standard button border color.
     * @param StandardButton_Border_OnHover New standard button hover state border color.
     * @param StandardButton_Border_OnDown  New standard button down state border color.
     * @param StandardButton_Text_Default   New standard button default text color.
     * @param StandardButton_Text_Highlight New standard button highlight text color.
     */
    void UpdateStandardButtonColors(MyColor StandardButton, MyColor StandardButton_OnHover, MyColor StandardButton_OnDown, MyColor StandardButton_Border,
                                    MyColor StandardButton_Border_OnHover, MyColor StandardButton_Border_OnDown, MyColor StandardButton_Text_Default, MyColor StandardButton_Text_Highlight);

    /**
     * @brief Update the new colors for the radio button elements.
     *
     * @param RadioPrimary                   New radio button primary color.
     * @param RadioPrimary_OnHover           New radio button hover state primary color.
     * @param RadioPrimary_OnDown            New radio button down state primary color.
     * @param RadioSecondary                 New radio button secondary color.
     * @param RadioSecondary_OnHover         New radio button hover state secondary color.
     * @param RadioSecondary_OnDown          New radio button down state secondary color.
     * @param Radio_Border                   New radio button border color.
     * @param Radio_Border_OnHover           New radio button hover state border color.
     * @param Radio_Border_OnDown            New radio button down state border color.
     * @param SelectedRadioPrimary           New selected radio button primary color.
     * @param SelectedRadioPrimary_OnHover   New selected radio button hover state primary color.
     * @param SelectedRadioPrimary_OnDown    New selected radio button down state primary color.
     * @param SelectedRadioSecondary         New selected radio button secondary color.
     * @param SelectedRadioSecondary_OnHover New selected radio button hover state secondary color.
     * @param SelectedRadioSecondary_OnDown  New selected radio button down state secondary color.
     * @param SelectedRadio_Border           New selected radio button border color.
     * @param SelectedRadio_Border_OnHover   New selected radio button hover state border color.
     * @param SelectedRadio_Border_OnDown    New selected radio button down state border color.
     * @param Radio_Text_Default             New radio button default text color.
     * @param Radio_Text_Highlight           New radio button highlight text color.
     */
    void UpdateRadioButtonColors(MyColor RadioPrimary, MyColor RadioPrimary_OnHover, MyColor RadioPrimary_OnDown, MyColor RadioSecondary, MyColor RadioSecondary_OnHover, MyColor RadioSecondary_OnDown, MyColor Radio_Border, MyColor Radio_Border_OnHover, MyColor Radio_Border_OnDown,
                                 MyColor SelectedRadioPrimary, MyColor SelectedRadioPrimary_OnHover, MyColor SelectedRadioPrimary_OnDown, MyColor SelectedRadioSecondary, MyColor SelectedRadioSecondary_OnHover, MyColor SelectedRadioSecondary_OnDown, MyColor SelectedRadio_Border, MyColor SelectedRadio_Border_OnHover, MyColor SelectedRadio_Border_OnDown,
                                 MyColor Radio_Text_Default, MyColor Radio_Text_Highlight);

    /**
     * Update the new colors for the editbox elements.
     *
     * @param Edit                 New editbox color.
     * @param Edit_Border          New default editbox border color.
     * @param Edit_Border_Selected New selected editbox border color.
     */
    void UpdateEditColors(MyColor Edit, MyColor Edit_Border, MyColor Edit_Border_Selected);

    /**
     * @brief Update the new colors for the combobox elements.
     *
     * @param Combobox                                  New combobox color.
     * @param Combobox_Border                           New combobox border color.
     * @param Combobox_Dropdownlist_Background          New combobox drop down list background color.
     * @param Combobox_Dropdownlist_Background_Selected New combobox drop down list selected item background color.
     * @param Combobox_Dropdownlist_Border              New combobox drop down list border color.
     * @param Combobox_Dropdownlist_Text_Default        New combobox drop down list default item text color.
     * @param Combobox_Dropdownlist_Text_Selected       New combobox drop down list selected item text color.
     */
    void UpdateComboboxColors(MyColor Combobox, MyColor Combobox_Border, MyColor Combobox_Dropdownlist_Background,
                              MyColor Combobox_Dropdownlist_Background_Selected, MyColor Combobox_Dropdownlist_Border,
                              MyColor Combobox_Dropdownlist_Text_Default, MyColor Combobox_Dropdownlist_Text_Selected);

public:
    /// [CONTAINER FUNCTIONS]

    /**
     * @brief Add new color object to the container.
     *
     * @param Red      The red value of the color.
     * @param Green    The green value of the color.
     * @param Blue     The blue value of the color.
     * @param Alpha    The alpha value of the color.
     * @param ID       The instance ID must be unique within the container. If the instance doesn't require an ID, use zero.
     */
    void AddColor(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha = 255, INT ID = 0);

    /**
     * @brief Remove an color object from the container.
     * @note If the target object doesn't have a unique handle ID, use RemoveColorEx() instead.
     *
     * @param ID The instance ID.
     *
     * @return Returns true if the object is successfully removed from the container, false otherwise.
     */
    bool RemoveColor(INT ID);

    /**
     * @brief Remove color objects with specified parameters from the container.
     * @note All the color objects match the specified parameters will be removed.
     * @brief The container's reference can be retrieved using GetContainer() if needed to perform manual operations.
     *
     * @param Red   The red value of the color.
     * @param Green The green value of the color.
     * @param Blue  The blue value of the color.
     * @param Alpha The alpha value of the color.
     *
     * @return Returns true if at least one object is successfully removed from the container, false otherwise.
     */
    bool RemoveColorEx(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha = 255);

    /**
     * @brief Get the color object's pointer from the container using instance ID.
     *
     * @param ID The instance ID.
     *
     * @return Returns the object's pointer if found. Otherwise, return nullptr.
     */
    MyColor *GetColor(INT ID);

    /**
     * @brief Get the color object's pointer from the container using specified parameters.
     *
     * @param Red   The red value of the color.
     * @param Green The green value of the color.
     * @param Blue  The blue value of the color.
     * @param Alpha The alpha value of the color.
     *
     * @return Returns the object's pointer if found. Otherwise, return nullptr.
     */
    MyColor *GetColorEx(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha);

    /**
     * @brief Get the container.
     *
     * @return Return the container's reference.
     */
    std::vector<MyColor> &GetContainer();
};

/**
 * @class UIObjectManager_Fonts
 * @brief This class provides an interface to access the default font objects and serves as an container for custom font objects.
 * @note This is an internal class of the UIObjectManager class.
 */
class UIObjectManager_Fonts
{
private:
    std::vector<MyFont> Container; // Container for custom font objects.

public:
    /// [DEFAULT APPLICATION FONTS]

    MyFont Default = MyFont(L"Tahoma", 24, FW_NORMAL, CLEARTYPE_QUALITY);  // Default font.
    MyFont Caption = MyFont(L"Tahoma", 23, FW_NORMAL, CLEARTYPE_QUALITY);  // Caption font.
    MyFont Edit = MyFont(L"Tahoma", 29, FW_ULTRALIGHT, CLEARTYPE_QUALITY); // Edit font.
    MyFont Combobox = MyFont(L"Tahoma", 24, FW_NORMAL, CLEARTYPE_QUALITY); // Combobox font.

    MyFont Heading = MyFont(L"Tahoma", 26, FW_MEDIUM, CLEARTYPE_QUALITY); // Heading font.
    MyFont Note = MyFont(L"Tahoma", 22, FW_NORMAL, CLEARTYPE_QUALITY);    // Note font.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Update the new main fonts.
     *
     * @param Default New default font.
     * @param Caption New caption font.
     * @param Edit    New editbox font.
     */
    void UpdateMainFonts(MyFont Default, MyFont Caption, MyFont Edit);

    /**
     * @brief Update the new container (@class MyContainer) fonts.
     *
     * @param Heading New heading font.
     * @param Note    New note font.
     */
    void UpdateContainerFonts(MyFont Heading, MyFont Note);

public:
    /// [CONTAINER FUNCTIONS]

    /**
     * @brief Add new font object to the container.
     *
     * @param Name    The font name.
     * @param Size    The font size.
     * @param Weight  The font weight.
     * @param Quality The font quality.
     * @param ID      The instance ID must be unique within the container. If the instance doesn't require an ID, use zero.
     */
    void AddFont(std::wstring Name, USHORT Size, USHORT Weight = FW_NORMAL, DWORD Quality = DEFAULT_QUALITY, INT ID = 0);

    /**
     * @brief Remove an font object from the container.
     * @note If the target object doesn't have a unique handle ID, use RemoveFontEx() instead.
     *
     * @param ID The instance ID.
     *
     * @return Returns true if the object is successfully removed from the container, false otherwise.
     */
    bool RemoveFont(INT ID);

    /**
     * @brief Remove font objects with specified parameters from the container.
     * @note All the font objects match the specified parameters will be removed.
     * @brief The container's reference can be retrieved using GetContainer() if needed to perform manual operations.
     *
     * @param Name    The font name.
     * @param Size    The font size.
     * @param Weight  The font weight.
     * @param Quality The font quality.
     *
     * @return Returns true if at least one object is successfully removed from the container, false otherwise.
     */
    bool RemoveFontEx(std::wstring Name, USHORT Size, USHORT Weight = FW_NORMAL, DWORD Quality = DEFAULT_QUALITY);

    /**
     * @brief Get the font object's pointer from the container using instance ID.
     *
     * @param ID The instance ID.
     *
     * @return Returns the object's pointer if found. Otherwise, return nullptr.
     */
    MyFont *GetFont(INT ID);

    /**
     * @brief Get the font object's pointer from the container using specified parameters.
     *
     * @param Name    The font name.
     * @param Size    The font size.
     * @param Weight  The font weight.
     * @param Quality The font quality.
     *
     * @return Returns the object's pointer if found. Otherwise, return nullptr.
     */
    MyFont *GetFontEx(std::wstring Name, USHORT Size, USHORT Weight = FW_NORMAL, DWORD Quality = DEFAULT_QUALITY);

    /**
     * @brief Get the container.
     *
     * @return Return the container's reference.
     */
    std::vector<MyFont> &GetContainer();
};

/**
 * @class UIObjectManager_Icons
 * @brief This class provides an interface to access the default icon objects and serves as an container for custom icon objects.
 * @note This is an internal class of the UIObjectManager class.
 */
class UIObjectManager_Icons
{
private:
    std::vector<MyIcon> Container; // Container for custom icon objects.

public:
    /// [DEFAULT APPLICATION ICONS]
    /// Note: The icon object(s) will be initialized in the constructor.

    MyIcon *Cross_Black = nullptr; // Black cross icon.
    MyIcon *Cross_Grey = nullptr;  // Grey cross icon.
    MyIcon *Cross_White = nullptr; // White cross icon.
    MyIcon *Minus_Black = nullptr; // Black minus icon.
    MyIcon *Minus_Grey = nullptr;  // Grey minus icon.
    MyIcon *Minus_White = nullptr; // White minus icon.
    MyIcon *MAINICON = nullptr;    // Application main icon.

public:
    /**
     * @brief Default constructor.
     *
     * @param hInstance Handle to the module of either a DLL or executable (.exe) that contains the default icons to be loaded.
     */
    UIObjectManager_Icons(HINSTANCE hInstance);

    /**
     * @brief Default destructor.
     */
    ~UIObjectManager_Icons();

private:
    /// [(DE)INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the default icon objects.
     *
     * @param hInstance Handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     *
     * @return Returns true if successfully initialized the default icons, false otherwise.
     */
    bool InitDefaultIcons(HINSTANCE hInstance);

    /**
     * @brief Deinitialize the default icon objects.
     *
     * @return Returns true if successfully deinitialized the default icons, false otherwise.
     */
    bool DeinitDefaultIcons();

public:
    /// [CONTAINER FUNCTIONS]

    /**
     * @brief Add new icon object to the container.
     *
     * @param hInstance    Handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     * @param Width        The width of the icon.
     * @param Height       The height of the icon.
     * @param ResourceID   The resource ID of the icon.
     * @param HICON_fuLoad Extra fuLoad flag(s) for the LoadImage() function.
     * @param ID           The instance ID must be unique within the container. If the instance doesn't require an ID, use zero.
     */
    void AddIcon(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad = NULL, INT ID = 0);

    /**
     * @brief Remove an icon object from the container.
     * @note If the target object doesn't have a unique handle ID, use RemoveIconEx() instead.
     *
     * @param ID The instance ID.
     *
     * @return Returns true if the object is successfully removed from the container, false otherwise.
     */
    bool RemoveIcon(INT ID);

    /**
     * @brief Remove icon objects with specified parameters from the container.
     * @note All the icon objects match the specified parameters will be removed.
     * @brief The container's reference can be retrieved using GetContainer() if needed to perform manual operations.
     *
     * @param hInstance    Handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     * @param Width        The width of the icon.
     * @param Height       The height of the icon.
     * @param ResourceID   The resource ID of the icon.
     * @param HICON_fuLoad Extra fuLoad flag(s) for the LoadImage() function.
     *
     * @return Returns true if at least one object is successfully removed from the container, false otherwise.
     */
    bool RemoveIconEx(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad = NULL);

    /**
     * @brief Get the icon object's pointer from the container using instance ID.
     *
     * @param ID The instance ID.
     *
     * @return Returns the object's pointer if found. Otherwise, return nullptr.
     */
    MyIcon *GetIcon(INT ID);

    /**
     * @brief Get the icon object's pointer from the container using specified parameters.
     *
     * @param hInstance    Handle to the module of either a DLL or executable (.exe) that contains the icon to be loaded.
     * @param Width        The width of the icon.
     * @param Height       The height of the icon.
     * @param ResourceID   The resource ID of the icon.
     * @param HICON_fuLoad Extra fuLoad flag(s) for the LoadImage() function.
     *
     * @return Returns the object's pointer if found. Otherwise, return nullptr.
     */
    MyIcon *GetIconEx(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad = NULL);

    /**
     * @brief Get the container.
     *
     * @return Return the container's reference.
     */
    std::vector<MyIcon> &GetContainer();
};

class UIObjectManager_Images
{
private:
    std::vector<MyImage> Container; // Container for custom MyImage objects.

public:
    /// [DEFAULT APPLICATION IMAGES]
    MyImage Cross_Grey;  // Cross Grey.
    MyImage Cross_White; // Cross White.
    MyImage Cross_Black; // Cross Black.
    MyImage Minus_Grey;  // Minus Grey.
    MyImage Minus_White; // Minus White.
    MyImage Minus_Black; // Minus Black.
    
    MyImage NonClient_CloseButton_Default;
    MyImage NonClient_CloseButton_Hover;
    MyImage NonClient_CloseButton_Down;
    MyImage NonClient_MinimizeButton_Default;
    MyImage NonClient_MinimizeButton_Hover;
    MyImage NonClient_MinimizeButton_Down;

public:
    UIObjectManager_Images();

public:
    void UpdateNonClientButtonImages(MyImage NonClient_CloseButton_Default, MyImage NonClient_CloseButton_Hover, MyImage NonClient_CloseButton_Down,
    MyImage NonClient_MinimizeButton_Default, MyImage NonClient_MinimizeButton_Hover, MyImage NonClient_MinimizeButton_Down);
};

/**
 * @class UIObjectManager_Pointers
 * @brief This class provides an interface to access pointers related to drawing operations.
 * @note This is an internal class of the UIObjectManager class.
 */
class UIObjectManager_Pointers
{
public:
    HBRUSH *pCurrentBorderBrush = nullptr; // Pointer that holds the current application border brush.
};

/**
 * @class UIObjectManager_Miscs
 * @brief This class provides an interface to access miscellaneous objects related to drawing operations.
 * @note This is an internal class of the UIObjectManager class.
 */
class UIObjectManager_Miscs
{
public:
    /// [APPLICATION RECTS]
    /// The rects hold the dimensions of the application areas.
    /// Note:
    /// - The default rects value will be initialized in the constructor.
    /// - If the application's dimensions change, the equivalent rect must be updated.

    RECT RECT_Caption;           // Caption bar area rectangle.
    RECT RECT_SizeBorder_Top;    // Top border area rectangle.
    RECT RECT_SizeBorder_Bottom; // Bottom border area rectangle.
    RECT RECT_SizeBorder_Left;   // Left border area rectangle.
    RECT RECT_SizeBorder_Right;  // Right border area rectangle.
    HWND hWndNonClientCloseButton = nullptr;
    HWND hWndNonClientMinimizeButton = nullptr;

public:
    /**
     * @brief Default constructor.
     */
    UIObjectManager_Miscs();
};

/**
 * @class UIObjectManager
 * @brief This class provides an interface to access the default drawing objects. It also provides access to custom containers for color, font, icon objects.
 */
class UIObjectManager
{
public:
    UIObjectManager_Colors Colors;     // Contains the application's default colors and a custom color container.
    UIObjectManager_Fonts Fonts;       // Contains the application's default fonts and a custom font container.
    UIObjectManager_Icons *Icons;      // Contains the application's default icons and a custom icon container.
    UIObjectManager_Images Images;
    UIObjectManager_Pointers Pointers; // Contains pointers that related to drawing operations.
    UIObjectManager_Miscs Miscs;       // Contains miscellaneous objects that related to drawing operations.

public:
    /**
     * @brief Default constructor.
     *
     * @param hInstance Handle to the module of either a DLL or executable (.exe) that contains the default icons to be loaded.
     */
    UIObjectManager(HINSTANCE hInstance);

    /**
     * @brief Default destructor.
     */
    ~UIObjectManager();

public:
    /**
     * @brief Show the total number of drawing objects that currently exist via message box (color, font, and icon objects).
     * @note This is a debug function.
     */
    static void ShowTotalObjects();
};