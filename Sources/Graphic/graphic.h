/**
 * @file graphic.h
 * @brief Header file containing classes that related to graphic engine.
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

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
    Gdiplus::GdiplusStartupInput gdipStartupInput; // The GDI+ startup input.
    ULONG_PTR gdipToken;                           // The GDI+ token.

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
    // [INITIALIZATION FUNCTIONS]

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
     * @param centerText    Specifies whether to center the text. If true, the x and y coordinates should be set to 0.
     *
     * @return Returns true if successfully drawn the text, otherwise false.
     */
    bool drawText(ID2D1DCRenderTarget *&pRenderTarget, IDWriteTextFormat *&pTextFormat, D2D1_RECT_F rect, std::wstring text, ID2D1SolidColorBrush *&pBrushText, FLOAT posX = 0, FLOAT posY = 0, bool centerText = false);

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