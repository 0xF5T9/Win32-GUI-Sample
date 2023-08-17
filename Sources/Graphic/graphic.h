/**
 * @file graphic.h
 * @brief Header file containing classes that related to graphic engine.
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H
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
     * @return Returns true if the image values were successfully updated, false otherwise.
     */
    bool update(INT resourceID, MyImageFormat format);
};

/**
 * @class MyDWTextFormat
 *
 * @brief Represents a text format. (Wrapped IDWriteTextFormat)
 * @note IDWriteTextFormat can be used by GDI, GDI+, Direct2D, Direct3D draw text functions.
 */
class MyDWTextFormat
{
public:
    /**
     * @brief Default constructor.
     *
     * @param fontFamilyName The name of the font family.
     * @param fontSize       The size of the font.
     * @param fontWeight     The weight of the font.
     * @param fontStyle      The style of the font.
     * @param fontStretch    The stretch of the font.
     */
    MyDWTextFormat(std::wstring fontFamilyName, float fontSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
                   DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

    /**
     * @brief Copy constructor.
     *
     * @param other The MyDWTextFormat object to be copied.
     */
    MyDWTextFormat(const MyDWTextFormat &other);

    /**
     * @brief Assignment operator.
     *
     * @param other The MyDWTextFormat object to be assigned.
     */
    MyDWTextFormat &operator=(const MyDWTextFormat &other);

    /**
     * @brief Destructor for the MyDWTextFormat object.
     */
    ~MyDWTextFormat();

private:
    inline static UINT totalInstances = 0; // Tracks the total number of instances.

    // Allocation variable(s).
    std::wstring fontFamilyName;     // Name of the font family.
    float fontSize;                  // Size of the font.
    DWRITE_FONT_WEIGHT fontWeight;   // Weight of the font.
    DWRITE_FONT_STYLE fontStyle;     // Style of the font.
    DWRITE_FONT_STRETCH fontStretch; // Stretch of the font.

    // Variant(s).
    IDWriteTextFormat *pTextFormat = nullptr; // IDWriteTextFormat variant of the text format.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the total instances of MyDWTextFormat.
     *
     * @return Returns the total instances of MyDWTextFormat.
     */
    inline static UINT getTotalInstances();

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the name of the font family.
     *
     * @return Returns the name of the font family.
     */
    std::wstring getFontFamilyName() const;

    /**
     * @brief Get the size of the font.
     *
     * @return Returns the size of the font.
     */
    float getFontSize() const;

    /**
     * @brief Get the weight of the font.
     *
     * @return Returns the weight of the font.
     */
    DWRITE_FONT_WEIGHT getFontWeight() const;

    /**
     * @brief Get the style of the font.
     *
     * @return Returns the style of the font.
     */
    DWRITE_FONT_STYLE getFontStyle() const;

    /**
     * @brief Get the stretch of the font.
     *
     * @return Returns the stretch of the font.
     */
    DWRITE_FONT_STRETCH getFontStretch() const;

    /**
     * @brief Get the IDWriteTextFormat variant of the text format.
     *
     * @returns Returns the IDWriteTextFormat variant of the text format.
     */
    IDWriteTextFormat *getTextFormat() const;

    /**
     * @brief Update the text format values.
     *
     * @param fontFamilyName The name of the font family.
     * @param fontSize       The size of the font.
     * @param fontWeight     The weight of the font.
     * @param fontStyle      The style of the font.
     * @param fontStretch    The stretch of the font.
     *
     * @return Returns true if the text format values were successfully updated, false otherwise.
     */
    bool update(std::wstring fontFamilyName, float fontSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
                DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);
};

/**
 * @class MyD2D1Image
 *
 * @brief Represents a image. (Wrapped IWICBitmapSource)
 * @note IWICBitmapSource is used to create ID2D1Bitmap.
 */
class MyD2D1Image
{
public:
    /**
     * @brief Default constructor.
     *
     * @param resourceID   The resource ID of the image.
     * @param resourceType The resource type of the image.
     * @param imageWidth   The width of the image.
     * @param imageHeight  The height of the image.
     */
    MyD2D1Image(INT resourceID, std::wstring resourceType, UINT imageWidth, UINT imageHeight);

    /**
     * @brief Copy constructor.
     *
     * @param other The MyD2D1Image object to be copied.
     */
    MyD2D1Image(const MyD2D1Image &other);

    /**
     * @brief Assignment operator.
     *
     * @param other The MyD2D1Image object to be assigned.
     */
    MyD2D1Image &operator=(const MyD2D1Image &other);

    /**
     * @brief Destructor for the MyD2D1Image object.
     */
    ~MyD2D1Image();

private:
    inline static UINT totalInstances = 0; // Tracks the total number of instances.

    // Allocation variable(s).
    INT resourceID;            // Resource ID of the image.
    std::wstring resourceType; // Resource type of the image.
    UINT imageWidth;           // Width of the image.
    UINT imageHeight;          // Height of the image.

    // Variant(s).
    IWICBitmapSource *pBitmapSource = nullptr; // IWICBitmapSource variant of the image.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the total instances of MyD2D1Image.
     *
     * @return Returns the total instances of MyD2D1Image.
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
     * @brief Get the resource type of the image.
     *
     * @return Returns the resource type of the image.
     */
    std::wstring getResourceType() const;

    /**
     * @brief Get the width of the image.
     *
     * @return Returns the width of the image.
     */
    UINT getImageWidth() const;

    /**
     * @brief Get the height of the image.
     *
     * @return Returns the height of the image.
     */
    UINT getImageHeight() const;

    /**
     * @brief Get the IWICBitmapSource variant of the image.
     *
     * @returns Returns the IWICBitmapSource variant of the image.
     */
    IWICBitmapSource *getBitmapSource() const;

    /**
     * @brief Update the image values.
     *
     * @param resourceID   The resource ID of the image.
     * @param resourceType The resource type of the image.
     * @param imageWidth   The width of the image.
     * @param imageHeight  The height of the image.
     *
     * @return Returns true if the image values were successfully updated, false otherwise.
     */
    bool update(INT resourceID, std::wstring resourceType, UINT imageWidth, UINT imageHeight);
};

/**
 * @brief My GDI+ engine class (Singleton).
 */
class MyGDIPEngine
{
public:
    /**
     * @brief Destructor.
     */
    ~MyGDIPEngine();

private:
    bool isInitialized = false; // The initialization status of the GDI+ engine.

public:
    Gdiplus::GdiplusStartupInput gdipStartupInput = {0}; // The GDI+ startup input.
    ULONG_PTR gdipToken = NULL;                          // The GDI+ token.

public:
    // [INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the GDI+ engine.
     *
     * @return Returns true if successfully initialized the GDI+ engine, otherwise false.
     */
    bool initialize();

    /**
     * @brief Uninitialize the GDI+ engine.
     *
     * @return Returns true if successfully uninitialized the GDI+ engine, otherwise false.
     */
    bool uninitialize();

public:
    // [GENERAL FUNCTIONS]

    /**
     * @brief Check if the GDI+ engine is ready to use.
     */
    bool isReady() const;
};

/**
 * @brief My Direct2D engine class (Singleton).
 */
class MyD2D1Engine
{
public:
    /**
     * @brief Destructor.
     */
    ~MyD2D1Engine();

private:
    bool isInitialized = false; // The initialization status of the D2D1 engine.

public:
    ID2D1Factory *pFactory = nullptr;           // The Direct2D factory.
    IDWriteFactory *pWriteFactory = nullptr;    // The DirectWrite factory.
    IWICImagingFactory2 *pWICFactory = nullptr; // The WIC factory.

public:
    // [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the D2D1 engine.
     *
     * @return Returns true if successfully initialized the D2D1 engine, otherwise false.
     */
    bool initialize();

    /**
     * @brief Uninitialize the D2D1 engine.
     *
     * @return Returns true if successfully uninitialized the D2D1 engine, otherwise false.
     */
    bool uninitialize();

public:
    // [GENERAL FUNCTIONS]

    /**
     * @brief Check if the D2D1 engine is ready to use.
     */
    bool isReady() const;

public:
    // [CREATE RESOURCE HELPER FUNCTIONS]

    /**
     * @brief Create a Direct2D device context render target.
     *
     * @param renderTargetType  The render target type.       (Default: D2D1_RENDER_TARGET_TYPE_HARDWARE - Using hardware acceleration)
     * @param renderTargetUsage The render target usage.      (Default: D2D1_RENDER_TARGET_USAGE_NONE - No specific render target usage)
     * @param renderAlphaMode   The render target alpha mode. (Default: D2D1_ALPHA_MODE_PREMULTIPLIED - The alpha values are premultiplied.)
     *
     * @return Returns the pointer to the Direct2D device context render target, otherwise nullptr.
     */
    ID2D1DCRenderTarget *createDCRenderTarget(D2D1_RENDER_TARGET_TYPE renderTargetType = D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1_RENDER_TARGET_USAGE renderTargetUsage = D2D1_RENDER_TARGET_USAGE_NONE, D2D1_ALPHA_MODE renderAlphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED);

    bool createTextFormat(IDWriteTextFormat *&pTextFormat, std::wstring fontFamilyName, float fontSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
                          DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

    /**
     * @brief Create a IWICBitmapSource from application resource.
     * @note If the resource type is RT_GROUP_ICON, use createWicBitmapSourceFromIconResource() instead.
     *
     * @param pIWCBitmapSource  The pointer to the IWICBitmapSource. [OUT]
     * @param resourceName      The resource name.
     * @param resourceType      The resource type.
     * @param destinationWidth  The destination width of the bitmap.
     * @param destinationHeight The destination height of the bitmap.
     * @param alphaMode         The alpha mode of the bitmap. (Default: D2D1_ALPHA_MODE_PREMULTIPLIED - The alpha values are premultiplied.)
     *
     * @return Returns true if successfully created the IWICBitmapSource, otherwise false.
     */
    bool createWicBitmapSourceFromResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED);

    /**
     * @brief Create a IWICBitmapSource from application resource. (Icon resource only)
     * @note If the resource type is not RT_GROUP_ICON, use createWicBitmapSourceFromResource() instead.
     *
     * @param pIWCBitmapSource  The pointer to the IWICBitmapSource. [OUT]
     * @param iconResourceName  The icon resource name.
     * @param destinationWidth  The destination width of the bitmap.
     * @param destinationHeight The destination height of the bitmap.
     * @param alphaMode         The alpha mode of the bitmap. (Default: D2D1_ALPHA_MODE_PREMULTIPLIED - The alpha values are premultiplied.)
     *
     * @return Returns true if successfully created the IWICBitmapSource, otherwise false.
     */
    bool createWicBitmapSourceFromIconResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR iconResourceName, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED);

public:
    // [DRAWING HELPER FUNCTIONS]

    /**
     * @brief Draw a rectangle.
     *
     * @param pRenderTarget   The pointer to the Direct2D device context render target.
     * @param rect            The dimensions of the rectangle.
     * @param pBrushRectangle The rectangle color brush.
     * @param strokeWidth     The stroke width of the rectangle.
     *
     * @return Returns true if successfully drawn the rectangle, otherwise false.
     */
    bool drawRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a filled rectangle.
     *
     * @param pRenderTarget         The pointer to the Direct2D device context render target.
     * @param rect                  The dimensions of the rectangle.
     * @param pBrushRectangle       The rectangle color brush.
     * @param pBrushRectangleBorder The rectangle border color brush. (nullptr to disable border drawing)
     * @param strokeWidth           The stroke width of the rectangle.
     *
     * @return Returns true if successfully drawn the filled rectangle, otherwise false.
     */
    bool drawFillRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder = nullptr, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a rounded rectangle.
     * @note The render target anti-aliasing mode should be set to D2D1_ANTIALIAS_MODE_PER_PRIMITIVE to get the best result.
     *
     * @param pRenderTarget   The pointer to the Direct2D device context render target.
     * @param rect            The dimensions of the rectangle.
     * @param radiusX         The x-radius of the rectangle.
     * @param radiusY         The y-radius of the rectangle.
     * @param pBrushRectangle The rectangle color brush.
     * @param strokeWidth     The stroke width of the rectangle.
     *
     * @return Returns true if successfully drawn the rounded rectangle, otherwise false.
     */
    bool drawRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a filled rounded rectangle.
     *
     * @param pRenderTarget         The pointer to the Direct2D device context render target.
     * @param rect                  The dimensions of the rectangle.
     * @param radiusX               The x-radius of the rectangle.
     * @param radiusY               The y-radius of the rectangle.
     * @param pBrushRectangle       The rectangle color brush.
     * @param pBrushRectangleBorder The rectangle border color brush. (nullptr to disable border drawing)
     * @param strokeWidth           The stroke width of the rectangle.
     *
     * @return Returns true if successfully drawn the filled rounded rectangle, otherwise false.
     */
    bool drawFillRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder = nullptr, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a text.
     *
     * @param pRenderTarget The pointer to the Direct2D device context render target.
     * @param pTextFormat   The pointer to the DirectWrite text format. (Independent resource)
     * @param rect          The dimensions of the text.
     * @param text          The text to draw.
     * @param pBrushText    The text color brush.
     * @param posX          The x-coordinate of the text.
     * @param posY          The y-coordinate of the text.
     * @param centerMode    0 - No centering, 1 - Center horizontally, 2 - Center vertically, 3 - Center horizontally and vertically.
     *
     * @return Returns true if successfully drawn the text, otherwise false.
     */
    bool drawText(ID2D1DCRenderTarget *&pRenderTarget, IDWriteTextFormat *&pTextFormat, D2D1_RECT_F rect, std::wstring text, ID2D1SolidColorBrush *&pBrushText, FLOAT posX = 0, FLOAT posY = 0, UINT centerMode = 0);

    /**
     * @brief Draw a image.
     *
     * @param pRenderTarget      The pointer to the Direct2D device context render target.
     * @param pD2D1Bitmap        The pointer to the Direct2D bitmap.
     * @param rect               The dimensions of the image.
     * @param opacity            The opacity of the image.
     * @param posX               The x-coordinate of the image.
     * @param posY               The y-coordinate of the image.
     * @param width              The width of the image.
     * @param height             The height of the image.
     * @param centering          Specifies whether to center the image. If true, the x and y coordinates should be set to 0.
     * @param interpolationMode  The interpolation mode of the image. (Default: D2D1_BITMAP_INTERPOLATION_MODE_LINEAR - Linear interpolation)
     *
     * @return Returns true if successfully drawn the image, otherwise false.
     */
    bool drawImage(ID2D1DCRenderTarget *&pRenderTarget, ID2D1Bitmap *&pD2D1Bitmap, D2D1_RECT_F rect, FLOAT opacity, FLOAT posX, FLOAT posY, FLOAT width, FLOAT height, bool centering = false, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
};

#endif // GRAPHIC_H