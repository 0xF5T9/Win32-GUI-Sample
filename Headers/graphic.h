/**
 * @file graphic.h
 * @brief Header file containing user-defined types that related to graphic operations.
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

/******************************
 * Graphic-related structures *
 ******************************/

/**
 * @brief A simple data structure that represents a color in the RGBA color space.
 */
struct MYRGBA
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param red Specifies the red value of the color.
     * @param green Specifies the green value of the color.
     * @param blue Specifies the blue value of the color.
     * @param alpha Specifies the alpha value of the color.
     */
    MYRGBA(BYTE red = 0, BYTE green = 0, BYTE blue = 0, BYTE alpha = 255);

public:
    BYTE red, green, blue, alpha;
};

/**
 * @brief A simple data structure that contains image information.
 * @note Used to locate specific image resource in the application resources.
 */
struct MYIMAGEINFO
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     * @param resourceID Specifies the image resource ID.
     * @param resourceType Specifies the image resource type.
     * @param imageWidth Specifies the desired image width.
     * @param imageHeight Specifies the desired image height.
     */
    MYIMAGEINFO(INT resourceID = 0, std::wstring resourceType = L"", UINT imageWidth = 0U, UINT imageHeight = 0U);

public:
    INT resourceID;
    std::wstring resourceType;
    UINT imageWidth;
    UINT imageHeight;
};

/***************************
 * Graphic-related classes *
 ***************************/

/**
 * @brief Represents a color. (Wrapped COLORREF, HBRUSH, D2D1::ColorF)
 */
class MyColor
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor 1.
     * @param rgba The RGBA structure contains basic color information.
     */
    MyColor(MYRGBA rgba = {0});

    /**
     * @brief Constructor 2.
     * @param red Specifies the red value of the color.
     * @param green Specifies the green value of the color.
     * @param blue Specifies the blue value of the color.
     * @param alpha Specifies the alpha value of the color.
     */
    MyColor(BYTE red, BYTE green, BYTE blue, BYTE alpha = 255);

    /**
     * @brief Copy constructor.
     * @param other The object to be copied.
     */
    MyColor(const MyColor &other);

    /**
     * @brief Assignment operator.
     * @param other The object to be assigned.
     */
    MyColor &operator=(const MyColor &other);

    // [UTILITY FUNCTIONS]

    /**
     * @brief Update the color.
     * @param rgba The RGBA structure contains basic color information. (Reference)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool update(const MYRGBA &rgba);

    /**
     * @brief Get the red value of the color.
     * @return Returns the red value of the color.
     */
    const BYTE &getRed();

    /**
     * @brief Get the green value of the color.
     * @return Returns the green value of the color.
     */
    const BYTE &getGreen();

    /**
     * @brief Get the blue value of the color.
     * @return Returns the blue value of the color.
     */
    const BYTE &getBlue();

    /**
     * @brief Get the alpha value of the color.
     * @return Returns the alpha value of the color.
     */
    const BYTE &getAlpha();

    /**
     * @brief Get the COLORREF variant of the color.
     * @return Returns the COLORREF variant of the color.
     */
    const COLORREF &getCOLORREF();

    /**
     * @brief Get the HBRUSH variant of the color.
     * @return Returns the HBRUSH variant of the color.
     * @note Solid brush.
     */
    const HBRUSH &getSolidBrush();

    /**
     * @brief Get the D2D1::ColorF variant of the color.
     * @return Returns the D2D1::ColorF variant of the color.
     */
    const D2D1::ColorF &getD2D1Color();

private:
    MYRGBA rgba;                                        // RGBA values of the color.
    COLORREF colorref;                                  // COLORREF variant of the color.
    std::unique_ptr<HBRUSH, HBRUSHDeleter> pSolidBrush; // HBRUSH (solid brush) variant of the color.
    D2D1::ColorF d2d1Color = D2D1::ColorF(0, 0, 0, 0);  // D2D1::ColorF variant of the color.
};

/**
 * @brief Wrapped IWICBitmapSource.
 * @note Used to create ID2D1Bitmap objects for Direct 2D drawing operations.
 */
class MyWICBitmap
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor 1.
     * @param imageInfo The MYIMAGEINFO structure contains image information.
     */
    MyWICBitmap(MYIMAGEINFO imageInfo);

    /**
     * @brief Constructor 2.
     * @param resourceID Specifies the image resource ID.
     * @param resourceType Specifies the image resource type.
     * @param imageWidth Specifies the desired image width.
     * @param imageHeight Specifies the desired image height.
     */
    MyWICBitmap(INT resourceID, std::wstring resourceType, UINT imageWidth, UINT imageHeight);

    /**
     * @brief Copy constructor.
     * @param other The object to be copied.
     */
    MyWICBitmap(const MyWICBitmap &other);

    /**
     * @brief Assignment operator.
     * @param other The object to be assigned.
     */
    MyWICBitmap &operator=(const MyWICBitmap &other);

    // [UTILITY FUNCTIONS]

    /**
     * @brief Update the bitmap.
     * @param imageInfo The MYIMAGEINFO structure contains image information. (Reference)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool update(const MYIMAGEINFO &imageInfo);

    /**
     * @brief Get the bitmap information (width, height, resource type, etc).
     * @return Returns the MYIMAGEINFO struct contains the information.
     */
    const MYIMAGEINFO &getInfo() const;

    /**
     * @brief Get the wic bitmap source (IWICBitmapSource *).
     * @return Returns the wic bitmap source.
     */
    IWICBitmapSource *getBitmapSource();

private:
    MYIMAGEINFO imageInfo;                                                // Image informations.
    std::unique_ptr<IWICBitmapSource *, IWICBitmapSourceDeleter> pBitmap; // Wic bitmap source.
};

/*******************
 * Graphic engines *
 *******************/

/**
 * @brief The engine abstract base class. All engine classes derived from this class.
 */
class MyEngine
{
public:
    // [UTILITY FUNCTIONS]

    /**
     * @brief Check if the engine is initialized.
     * @return Returns true if the engine is initialized, false otherwise.
     */
    bool isInitialized() const;

    // [UN/INITIALIZATION FUNCTIONS]

    virtual bool initialize() = 0;   // Initialize engine.
    virtual bool uninitialize() = 0; // Uninitialize engine.

protected:
    bool initialized = false; // Indicate whether the engine is initialized.
};

/**
 * @brief Singleton class encapsulating the un/initialization of the WAM (Windows Animation Manager) API.
 * @note This class is part of the MyGraphicEngine class.
 */
class MyWAMEngine : public MyEngine
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    MyWAMEngine();

    /**
     * @brief Destructor.
     */
    virtual ~MyWAMEngine() noexcept(false);

    // [UTILITY FUNCTIONS]

    /**
     * @brief Get the WAM animation manager pointer. (Reference)
     * @note Do not call the `Release()` or `Shutdown()` methods via this pointer.
     * @note All uninitialization should be handled via the engine interface.
     * @return Returns the WAM animation manager pointer. (Reference)
     */
    IUIAnimationManager *&manager();

    /**
     * @brief Get the WAM animation timer pointer. (Reference)
     * @note Do not call the `Release()` method via this pointer.
     * @note All uninitialization should be handled via the engine interface.
     * @return Returns the WAM animation timer pointer. (Reference)
     */
    IUIAnimationTimer *&timer();

    /**
     * @brief Get the WAM animation transition library pointer. (Reference)
     * @note Do not call the `Release()` method via this pointer.
     * @note All uninitialization should be handled via the engine interface.
     * @return Returns the WAM animation transition library pointer. (Reference)
     */
    IUIAnimationTransitionLibrary *&transitionLibrary();

    /**
     * @brief Update the animation manager.
     * @note This function is invoked within each message loop to ensure that the WAM animation manager stays synchronized with the current time.
     */
    void updateAnimationManager();

    // [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the engine.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool initialize() override;

    /**
     * @brief Uninitialize the engine.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool uninitialize() override;

    // [ENGINE HELPER FUNCTIONS]

    /**
     * @brief Create an animation variable.
     * @param pAnimationVariable Reference to the animation variable pointer. [OUT]
     * @param initialValue Specifies the initial value.
     * @param lowerBound Specifies the lower bound value.
     * @param upperBound Specifies the upper bound value.
     * @param roundingMode Specifies the rounding mode.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode = UI_ANIMATION_ROUNDING_NEAREST);

    /**
     * @brief Get RGB values from animation variables.
     * @param animationVariableArray A vector of animation variables.
     * @param animationValueArray An array to store RGB values. [OUT]
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool getAnimationValueRGB(std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> &animationVariableArray, INT32 animationValueArray[]);

    /**
     * @brief Get RGB values from animation variables.
     * @param animationVariableArray A vector of animation variables.
     * @param animationValueArray An array to store RGB values. [OUT]
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool getAnimationValueRGB(std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> &animationVariableArray, DOUBLE animationValueArray[]);

private:
    inline static bool instanceExists = false;                   // Indicate whether the instance already exists.
    IUIAnimationManager *pAnimationManager = nullptr;            // WAM animation manager.
    IUIAnimationTimer *pAnimationTimer = nullptr;                // WAM animation timer.
    IUIAnimationTransitionLibrary *pTransitionLibrary = nullptr; // WAM standard transition library.
};

/**
 * @brief Singleton class encapsulating the un/initialization of the Direct2D API.
 * @note This class is part of the MyGraphicEngine class.
 */
class MyD2D1Engine : public MyEngine
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    MyD2D1Engine();

    /**
     * @brief Destructor.
     */
    virtual ~MyD2D1Engine() noexcept(false);

    // [UTILITY FUNCTIONS]

    /**
     * @brief Get the Direct2D factory pointer. (Reference)
     * @note Do not call the `Release()` methods via this pointer.
     * @note All uninitialization should be handled via the engine interface.
     * @return Returns the Direct2D factory pointer. (Reference)
     */
    ID2D1Factory *&d2d1Factory();

    /**
     * @brief Get the DirectWrite factory pointer. (Reference)
     * @note Do not call the `Release()` methods via this pointer.
     * @note All uninitialization should be handled via the engine interface.
     * @return Returns the DirectWrite factory pointer. (Reference)
     */
    IDWriteFactory *&dwriteFactory();

    /**
     * @brief Get the Wic imaging factory pointer. (Reference)
     * @note Do not call the `Release()` methods via this pointer.
     * @note All uninitialization should be handled via the engine interface.
     * @return Returns the Wic imaging factory pointer. (Reference)
     */
    IWICImagingFactory2 *&wicImageFactory();

    // [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the engine.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool initialize() override;

    /**
     * @brief Uninitialize the engine.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool uninitialize() override;

    // [ENGINE HELPER FUNCTIONS - CREATE RESOURCES]

    /**
     * @brief Create a device context render target.
     * @param renderTarget Reference to the render target pointer. [OUT]
     * @param renderTargetType Specifies the render target type.      (Default: D2D1_RENDER_TARGET_TYPE_HARDWARE - Using hardware acceleration)
     * @param renderTargetUsage Specifies the render target usage.    (Default: D2D1_RENDER_TARGET_USAGE_NONE - No specific render target usage)
     * @param renderAlphaMode Specifies the render target alpha mode. (Default: D2D1_ALPHA_MODE_PREMULTIPLIED - The alpha values are premultiplied.)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDCRenderTarget(ID2D1DCRenderTarget *&renderTarget, D2D1_RENDER_TARGET_TYPE renderTargetType = D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1_RENDER_TARGET_USAGE renderTargetUsage = D2D1_RENDER_TARGET_USAGE_NONE, D2D1_ALPHA_MODE renderAlphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED);

    /**
     * @brief Create a direct write text format.
     * @param pTextFormat Reference to the text format pointer. [OUT]
     * @param fontFamilyName Specifies the font family name.
     * @param fontSize Specifies the font size.
     * @param fontWeight Specifies the font weight.
     * @param fontStyle Specifies the font style.
     * @param fontStretch Specifies the font stretch.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createTextFormat(IDWriteTextFormat *&pTextFormat, std::wstring fontFamilyName, float fontSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

    /**
     * @brief Create a wic bitmap source from application resource.
     * @param pIWCBitmapSource Reference to the wic bitmap source pointer. [OUT]
     * @param resourceName Specifies the resource name.
     * @param resourceType Specifies the resource type.
     * @param destinationWidth Specifies the destination width.
     * @param destinationHeight Specifies the destination height.
     * @param alphaMode Specifies the alpha mode. (Default: D2D1_ALPHA_MODE_PREMULTIPLIED - The alpha values are premultiplied.)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createWicBitmapSourceFromResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED);

    /**
     * @brief Create a wic bitmap source from application resource. (Icon resource only)
     * @param pIWCBitmapSource Reference to the wic bitmap source pointer. [OUT]
     * @param iconResourceName Specifies the icon resource name.
     * @param destinationWidth Specifies the destination width.
     * @param destinationHeight Specifies the destination height.
     * @param alphaMode Specifies the alpha mode. (Default: D2D1_ALPHA_MODE_PREMULTIPLIED - The alpha values are premultiplied.)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createWicBitmapSourceFromIconResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR iconResourceName, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED);

    // [ENGINE HELPER FUNCTIONS - DRAWING OPERATIONS]

    /**
     * @brief Draw a rectangle.
     * @param pRenderTarget Reference to the render target pointer.
     * @param rect Specifies the D2D1_RECT_F that represents the rectangle dimensions.
     * @param pBrushRectangle Specifies the rectangle color brush.
     * @param strokeWidth Specifies the stroke width.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool drawRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a filled rectangle.
     * @param pRenderTarget Reference to the render target pointer.
     * @param rect Specifies the D2D1_RECT_F that represents the rectangle dimensions.
     * @param pBrushRectangle Specifies the rectangle color brush.
     * @param pBrushRectangleBorder Specifies the rectangle border color brush.
     * @param strokeWidth Specifies the stroke width.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool drawFillRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder = nullptr, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a rounded rectangle.
     * @param pRenderTarget Reference to the render target pointer.
     * @param rect Specifies the D2D1_RECT_F that represents the rectangle dimensions.
     * @param radiusX Specifies the x-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
     * @param radiusY Specifies the y-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
     * @param pBrushRectangle Specifies the rectangle color brush.
     * @param strokeWidth Specifies the stroke width.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool drawRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a filled rounded rectangle.
     * @param pRenderTarget Reference to the render target pointer.
     * @param rect Specifies the D2D1_RECT_F that represents the rectangle dimensions.
     * @param radiusX Specifies the x-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
     * @param radiusY Specifies the y-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
     * @param pBrushRectangle Specifies the rectangle color brush.
     * @param pBrushRectangleBorder Specifies the rectangle border color brush.
     * @param strokeWidth Specifies the stroke width.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool drawFillRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder = nullptr, FLOAT strokeWidth = 1.0f);

    /**
     * @brief Draw a text.
     * @param pRenderTarget Reference to the render target pointer.
     * @param pTextFormat Reference to the direct write text format pointer.
     * @param rect Specifies the D2D1_RECT_F that represents the drawing area dimensions.
     * @param text Specifies the text to be drawn.
     * @param pBrushText Specifies the text color brush.
     * @param posX Specifies the x-coordinate of the text.
     * @param posY Specifies the y-coordinate of the text.
     * @param centerMode Specifies the centering mode. (0 - No centering, 1 - Center horizontally, 2 - Center vertically, 3 - Center horizontally and vertically.)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool drawText(ID2D1DCRenderTarget *&pRenderTarget, IDWriteTextFormat *&pTextFormat, D2D1_RECT_F rect, std::wstring text, ID2D1SolidColorBrush *&pBrushText, FLOAT posX = 0, FLOAT posY = 0, UINT centerMode = 0);

    /**
     * @brief Draw a image.
     * @param pRenderTarget Reference to the render target pointer.
     * @param pD2D1Bitmap Reference to the bitmap pointer.
     * @param rect Specifies the D2D1_RECT_F that represents the drawing area dimensions.
     * @param opacity Specifies the image opacity.
     * @param posX Specifies the x-coordinate of the image.
     * @param posY Specifies the y-coordinate of the image.
     * @param width Specifies the image width.
     * @param height Specifies the image height.
     * @param centering Specifies whether to center the image. (both horizontally and vertically)
     * @param interpolationMode Specifies the interpolation mode of the image. (Default: D2D1_BITMAP_INTERPOLATION_MODE_LINEAR - Linear interpolation)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool drawImage(ID2D1DCRenderTarget *&pRenderTarget, ID2D1Bitmap *&pD2D1Bitmap, D2D1_RECT_F rect, FLOAT opacity, FLOAT posX, FLOAT posY, FLOAT width, FLOAT height, bool centering = false, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

private:
    inline static bool instanceExists = false;       // Indicate whether the instance already exists.
    ID2D1Factory *pD2D1Factory = nullptr;            // The Direct2D factory.
    IDWriteFactory *pDWriteFactory = nullptr;        // The DirectWrite factory.
    IWICImagingFactory2 *pWICImageFactory = nullptr; // The WIC Image factory.
};

/**
 * @brief Singleton class encapsulating the un/initialization of all graphic-related engines.
 * @note All the graphic-related operations can be accessed via this interface.
 * @note The COM library must be initialized first before `MyGraphicEngine` initialization.
 */
class MyGraphicEngine : public MyEngine
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    MyGraphicEngine();

    /**
     * @brief Destructor.
     */
    virtual ~MyGraphicEngine() noexcept(false);

    // [UTILITY FUNCTIONS]

    /**
     * @brief Get the WAM (Windows Animation Manager) engine. (Reference)
     * @return Returns the WAM (Windows Animation Manager) engine. (Reference)
     */
    MyWAMEngine &wamEngine();

    /**
     * @brief Get the Direct2D engine. (Reference)
     * @return Returns the Direct2D engine. (Reference)
     */
    MyD2D1Engine &d2d1Engine();

    // [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the engine.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool initialize() override;

    /**
     * @brief Uninitialize the engine.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool uninitialize() override;

private:
    inline static bool instanceExists = false; // Indicate whether the instance already exists.
    std::unique_ptr<MyWAMEngine> pWAMEngine;   // The WAM engine.
    std::unique_ptr<MyD2D1Engine> pD2D1Engine; // The Direct2D engine.
};

#endif // GRAPHIC_H