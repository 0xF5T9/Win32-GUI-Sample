/**
 * @file graphic.cpp
 * @brief Implementation of the user-defined types defined in graphic.h.
 */

#include "../Headers/standard_includes.h" // Include standard libraries.
#include "../Headers/my_includes.h"       // Include my user-defined type headers.

/**********************************************
 * Graphic-related structures implementations *
 **********************************************/

MYRGBA::MYRGBA(BYTE red, BYTE green, BYTE blue, BYTE alpha)
    : red(red), green(green), blue(blue), alpha(alpha) {}

MYIMAGEINFO::MYIMAGEINFO(INT resourceID, std::wstring resourceType, UINT imageWidth, UINT imageHeight)
    : resourceID(resourceID), resourceType(resourceType), imageWidth(imageWidth), imageHeight(imageHeight) {}

/*******************************************
 * Graphic-related classes implementations *
 *******************************************/

// [MyColor] class implementations:

MyColor::MyColor(MYRGBA rgba)
{
    if (!this->update(rgba))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyColor' | FUNC: 'Constructor 1']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }
}
MyColor::MyColor(BYTE red, BYTE green, BYTE blue, BYTE alpha)
{
    MYRGBA t_rgba(red, green, blue, alpha);
    if (!this->update(t_rgba))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyColor' | FUNC: 'Constructor 2']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }
}
MyColor::MyColor(const MyColor &other)
{
    if (!this->update(other.rgba))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyColor' | FUNC: 'Copy Constructor']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }
}
MyColor &MyColor::operator=(const MyColor &other)
{
    if (this == &other)
        return *this;

    if (!this->update(other.rgba))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyColor' | FUNC: 'Assignment Operator']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }

    return *this;
}
bool MyColor::update(const MYRGBA &rgba)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        this->rgba = rgba;
        this->colorref = RGB(rgba.red, rgba.green, rgba.blue);
        this->pSolidBrush.reset(new HBRUSH(CreateSolidBrush(this->colorref)));
        this->d2d1Color = D2D1::ColorF(rgba.red / 255.0f, rgba.green / 255.0f, rgba.blue / 255.0f, rgba.alpha / 255.0f);

        if (!*this->pSolidBrush)
        {
            error_message = "Failed to create the solid brush object.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyColor' | FUNC: 'update()']", MyLogType::Error);

    return are_all_operation_success;
}
const BYTE &MyColor::getRed()
{
    return this->rgba.red;
}
const BYTE &MyColor::getGreen()
{
    return this->rgba.green;
}
const BYTE &MyColor::getBlue()
{
    return this->rgba.blue;
}
const BYTE &MyColor::getAlpha()
{
    return this->rgba.alpha;
}
const COLORREF &MyColor::getCOLORREF()
{
    return this->colorref;
}
const HBRUSH &MyColor::getSolidBrush()
{
    return *this->pSolidBrush;
}
const D2D1::ColorF &MyColor::getD2D1Color()
{
    return this->d2d1Color;
}

// [MyWICBitmap] class implementations:

MyWICBitmap::MyWICBitmap(MYIMAGEINFO imageInfo)
{
    if (!this->update(imageInfo))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyWICBitmap' | FUNC: 'Constructor 1']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }
}
MyWICBitmap::MyWICBitmap(INT resourceID, std::wstring resourceType, UINT imageWidth, UINT imageHeight)
{
    MYIMAGEINFO t_imageInfo(resourceID, resourceType, imageWidth, imageHeight);
    if (!this->update(t_imageInfo))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyWICBitmap' | FUNC: 'Constructor 2']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }
}
MyWICBitmap::MyWICBitmap(const MyWICBitmap &other)
{
    if (!this->update(other.imageInfo))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyWICBitmap' | FUNC: 'Copy Constructor']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }
}
MyWICBitmap &MyWICBitmap::operator=(const MyWICBitmap &other)
{
    if (this == &other)
        return *this;

    if (!this->update(other.imageInfo))
    {
        g_pApp->logger.writeLog("Failed to initialize the object.", "[CLASS: 'MyWICBitmap' | FUNC: 'Assignment Operator']", MyLogType::Error);
        throw std::runtime_error("Failed to initialize the object.");
    }

    return *this;
}
bool MyWICBitmap::update(const MYIMAGEINFO &imageInfo)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        this->imageInfo = imageInfo;
        this->pBitmap.reset(new IWICBitmapSource *(nullptr));
        if (!g_pApp->pGraphic->d2d1Engine().createWicBitmapSourceFromResource(*this->pBitmap, MAKEINTRESOURCEW(this->imageInfo.resourceID), this->imageInfo.resourceType.c_str(), this->imageInfo.imageWidth, this->imageInfo.imageHeight) ||
            !*this->pBitmap)
        {
            error_message = "Failed to create the wic bitmap object.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWICBitmap' | FUNC: 'update()']", MyLogType::Error);

    return are_all_operation_success;
}
const MYIMAGEINFO &MyWICBitmap::getInfo() const
{
    return this->imageInfo;
}
IWICBitmapSource *MyWICBitmap::getBitmapSource()
{
    return *this->pBitmap;
}

/***********************************
 * Graphic engines implementations *
 ***********************************/

// [MyEngine] class implementations:

bool MyEngine::isInitialized() const
{
    return this->initialized;
}

// [MyWAMEngine] class implementations:

MyWAMEngine::MyWAMEngine()
{
    if (!MyWAMEngine::instanceExists)
        MyWAMEngine::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The WAM engine instance already exists.", "[CLASS: 'MyWAMEngine' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The WAM engine instance already exists.");
    }
}
MyWAMEngine::~MyWAMEngine() noexcept(false)
{
    if (this->initialized)
    {
        if (!this->uninitialize())
        {
            g_pApp->logger.writeLog("Failed to uninitialize the WAM engine.", "[CLASS: 'MyWAMEngine' | 'Destructor']", MyLogType::Error);
            throw std::runtime_error("Failed to uninitialize the WAM engine.");
        }
    }

    MyWAMEngine::instanceExists = false;
}
IUIAnimationManager *&MyWAMEngine::manager()
{
    return this->pAnimationManager;
}
IUIAnimationTimer *&MyWAMEngine::timer()
{
    return this->pAnimationTimer;
}
IUIAnimationTransitionLibrary *&MyWAMEngine::transitionLibrary()
{
    return this->pTransitionLibrary;
}
void MyWAMEngine::updateAnimationManager()
{
    UI_ANIMATION_MANAGER_STATUS status;

    HRESULT hr = this->manager()->GetStatus(&status);
    if (FAILED(hr))
    {
        g_pApp->logger.writeLog("Failed to get the animation manager status.", "[CLASS: 'MyWAMEngine' | FUNC: 'updateAnimationManager()']", MyLogType::Error);
        throw std::runtime_error("Failed to get the animation manager status.");
    }
    if (!status) // UI_ANIMATION_MANAGER_IDLE == 0
        return;

    UI_ANIMATION_SECONDS seconds_now;
    hr = this->timer()->GetTime(&seconds_now);
    if (FAILED(hr))
    {
        g_pApp->logger.writeLog("Failed to get the current time.", "[CLASS: 'MyWAMEngine' | FUNC: 'updateAnimationManager()']", MyLogType::Error);
        throw std::runtime_error("Failed to get the current time.");
    }
    hr = this->manager()->Update(seconds_now);
    if (FAILED(hr))
    {
        g_pApp->logger.writeLog("Failed to update the animation manager.", "[CLASS: 'MyWAMEngine' | FUNC: 'updateAnimationManager()']", MyLogType::Error);
        throw std::runtime_error("Failed to update the animation manager.");
    }
}
bool MyWAMEngine::initialize()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (this->initialized)
        {
            error_message = "The engine is already initialized.";
            break;
        }

        // Initialize the animation mananger.
        hr = CoCreateInstance(CLSID_UIAnimationManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->pAnimationManager));
        if (FAILED(hr))
        {
            error_message = "Failed to initialize the animation mananger.";
            break;
        }

        // Initialize the animation timer.
        hr = CoCreateInstance(CLSID_UIAnimationTimer, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->pAnimationTimer));
        if (FAILED(hr))
        {
            error_message = "Failed to initialize the animation timer.";
            break;
        }

        // Initialize the standard animation transition library.
        hr = CoCreateInstance(CLSID_UIAnimationTransitionLibrary, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->pTransitionLibrary));
        if (FAILED(hr))
        {
            error_message = "Failed to initialize the standard animation transition library.";
            break;
        }

        this->initialized = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWAMEngine' | FUNC: 'initialize()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWAMEngine::uninitialize()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        // Finish all animation storyboards.
        hr = this->pAnimationManager->FinishAllStoryboards(0);
        if (FAILED(hr))
        {
            error_message = "Failed to finish all animation storyboards.";
            break;
        }

        // Release standard animation library COM object.
        this->pTransitionLibrary->Release();
        this->pTransitionLibrary = nullptr;

        // Release animation timer COM object.
        this->pAnimationTimer->Release();
        this->pAnimationTimer = nullptr;

        // Release animation mananger and it associated COM objects.
        hr = this->pAnimationManager->Shutdown();
        if (FAILED(hr))
        {
            error_message = "Failed to shutdown the animation mananger.";
            break;
        }
        this->pAnimationManager = nullptr;

        this->initialized = false;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWAMEngine' | FUNC: 'uninitialize()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWAMEngine::createAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (pAnimationVariable)
        {
            error_message = "The animation variable pointer is not null. (potential memory leak?)";
            break;
        }

        // Create the animation variable.
        hr = this->pAnimationManager->CreateAnimationVariable(initialValue, &pAnimationVariable);
        if (FAILED(hr))
        {
            error_message = "Failed to create the animation variable.";
            break;
        }

        // Set lower bound value.
        hr = pAnimationVariable->SetLowerBound(lowerBound);
        if (FAILED(hr))
        {
            error_message = "Failed to set the animation variable lower bound value.";
            break;
        }

        // Set upper bound value.
        hr = pAnimationVariable->SetUpperBound(upperBound);
        if (FAILED(hr))
        {
            error_message = "Failed to set the animation variable upper bound value.";
            break;
        }

        hr = pAnimationVariable->SetRoundingMode(roundingMode);
        if (FAILED(hr))
        {
            error_message = "Failed to set the animation variable rounding mode.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyWAMEngine' | FUNC: 'createAnimationVariable()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyWAMEngine::getAnimationValueRGB(std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> &animationVariableArray, INT32 animationValueArray[])
{
    if (!this->initialized)
    {
        g_pApp->logger.writeLog("The engine is not initialized yet.", "[CLASS: 'MyWAMEngine' | FUNC: 'getAnimationValueRGB()']", MyLogType::Error);
        return false;
    }

    HRESULT hr;

    for (int i = 0; i < 3; i++)
    {
        hr = (*animationVariableArray[i])->GetIntegerValue(&animationValueArray[i]);
        if (FAILED(hr))
            return false;
    }

    return true;
}
bool MyWAMEngine::getAnimationValueRGB(std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> &animationVariableArray, DOUBLE animationValueArray[])
{
    if (!this->initialized)
    {
        g_pApp->logger.writeLog("The engine is not initialized yet.", "[CLASS: 'MyWAMEngine' | FUNC: 'getAnimationValueRGB()']", MyLogType::Error);
        return false;
    }

    HRESULT hr;

    for (int i = 0; i < 3; i++)
    {
        hr = (*animationVariableArray[i])->GetValue(&animationValueArray[i]);
        if (FAILED(hr))
            return false;
    }

    return true;
}

// [MyD2D1Engine] class implementations:

MyD2D1Engine::MyD2D1Engine()
{
    if (!MyD2D1Engine::instanceExists)
        MyD2D1Engine::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The Direct2D engine instance already exists.", "[CLASS: 'MyD2D1Engine' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The Direct2D engine instance already exists.");
    }
}
MyD2D1Engine::~MyD2D1Engine() noexcept(false)
{
    if (this->initialized)
    {
        if (!this->uninitialize())
        {
            g_pApp->logger.writeLog("Failed to uninitialize the Direct2D engine.", "[CLASS: 'MyD2D1Engine' | 'Destructor']", MyLogType::Error);
            throw std::runtime_error("Failed to uninitialize the Direct2D engine.");
        }
    }

    MyD2D1Engine::instanceExists = false;
}
ID2D1Factory *&MyD2D1Engine::d2d1Factory()
{
    return this->pD2D1Factory;
}
IDWriteFactory *&MyD2D1Engine::dwriteFactory()
{
    return this->pDWriteFactory;
}
IWICImagingFactory2 *&MyD2D1Engine::wicImageFactory()
{
    return this->pWICImageFactory;
}
bool MyD2D1Engine::initialize()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (this->initialized)
        {
            error_message = "The engine is already initialized.";
            break;
        }

        // Create the D2D1 factory.
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pD2D1Factory);
        if (FAILED(hr))
        {
            error_message = "Failed to create the Direct2D factory.";
            break;
        }

        // Create a DirectWrite factory.
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(this->pDWriteFactory),
            reinterpret_cast<IUnknown **>(&this->pDWriteFactory));
        if (FAILED(hr))
        {
            error_message = "Failed to create the DirectWrite factory.";
            break;
        }

        // Create a WIC factory.
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            reinterpret_cast<void **>(&this->pWICImageFactory));
        if (FAILED(hr))
        {
            error_message = "Failed to create the WIC imaging factory.";
            break;
        }

        this->initialized = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'initialize()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::uninitialize()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        // Release the WIC factory.
        if (this->pWICImageFactory)
        {
            this->pWICImageFactory->Release();
            this->pWICImageFactory = nullptr;
        }

        // Release the DirectWrite factory.
        if (this->pDWriteFactory)
        {
            this->pDWriteFactory->Release();
            this->pDWriteFactory = nullptr;
        }

        // Release the D2D1 factory.
        if (this->pD2D1Factory)
        {
            this->pD2D1Factory->Release();
            this->pD2D1Factory = nullptr;
        }

        this->initialized = false;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'uninitialize()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::createDCRenderTarget(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RENDER_TARGET_TYPE renderTargetType, D2D1_RENDER_TARGET_USAGE renderTargetUsage, D2D1_ALPHA_MODE renderAlphaMode)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (pRenderTarget)
        {
            error_message = "The render target pointer is not null. (potential memory leak?)";
            break;
        }

        // Create the render target properties.
        D2D1_RENDER_TARGET_PROPERTIES rt_properties = D2D1::RenderTargetProperties(renderTargetType,
                                                                                   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, renderAlphaMode),
                                                                                   0, 0, renderTargetUsage, D2D1_FEATURE_LEVEL_DEFAULT);

        // Create the device context render target.
        hr = this->pD2D1Factory->CreateDCRenderTarget(&rt_properties, &pRenderTarget);
        if (FAILED(hr))
        {
            error_message = "Failed to create the device context render target.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'createDCRenderTarget()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::createTextFormat(IDWriteTextFormat *&pTextFormat, std::wstring fontFamilyName, float fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (pTextFormat)
        {
            error_message = "The text format pointer is not null. (potential memory leak?)";
            break;
        }

        // Create the direct write text format.
        hr = this->pDWriteFactory->CreateTextFormat(
            fontFamilyName.c_str(),
            nullptr,
            fontWeight,
            fontStyle,
            fontStretch,
            fontSize,
            L"",
            &pTextFormat);
        if (FAILED(hr))
        {
            error_message = "Failed to create the direct write text format.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'createTextFormat()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::createWicBitmapSourceFromResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (pIWCBitmapSource)
        {
            error_message = "The wic bitmap source pointer is not null. (potential memory leak?)";
            break;
        }

        // Retrieve the pixel format of the render target.
        auto desired_pixel_format = (alphaMode == D2D1_ALPHA_MODE_PREMULTIPLIED ? GUID_WICPixelFormat32bppPBGRA : GUID_WICPixelFormat32bppBGRA);

        // These variables are used to get the target resource.
        HRSRC resource_handle = NULL;        // Handle to the resource.
        HGLOBAL resource_data_handle = NULL; // Handle to the resource data.
        void *p_resource_data = NULL;        // Pointer to the resource data.
        DWORD resource_size = 0;             // Size of the resource data.

        // These variables are used to create the WIC-compatible bitmap.
        IWICStream *p_wic_stream = NULL;                    // Pointer to the WIC stream.
        IWICBitmapDecoder *p_wic_decoder = NULL;            // Pointer to the WIC decoder.
        IWICBitmapFrameDecode *p_wic_frame_decode = NULL;   // Pointer to the WIC frame decoder.
        IWICFormatConverter *p_wic_format_converter = NULL; // Pointer to the WIC format converter.
        IWICBitmapScaler *p_wic_scaler = NULL;              // Pointer to the WIC bitmap scaler.

        // Check if the resource type is incompatible.
        if (resourceType == RT_GROUP_ICON)
        {
            error_message = "This function does not support the resource type 'RT_GROUP_ICON'. Use the function 'createWicBitmapSourceFromIconResource()' instead.";
            break;
        }

        // Locate the resource.
        resource_handle = FindResourceW(NULL, resourceName, resourceType);
        if (!resource_handle)
        {
            error_message = "Failed to locate the resource.";
            break;
        }

        // Load the resource.
        resource_data_handle = LoadResource(NULL, resource_handle);
        if (!resource_data_handle)
        {
            error_message = "Failed to load the resource.";
            break;
        }

        // Lock the resource and get pointer to the resource data.
        p_resource_data = LockResource(resource_data_handle);
        if (!p_resource_data)
        {
            error_message = "Failed to lock the resource.";
            break;
        }

        // Calculate the resource data size.
        resource_size = SizeofResource(NULL, resource_handle);
        if (!resource_size)
        {
            error_message = "Failed to calculate the resource data size.";
            break;
        }

        // Create a WIC stream to map onto the memory.
        hr = this->pWICImageFactory->CreateStream(&p_wic_stream);
        if (FAILED(hr))
        {
            error_message = "Failed to create a WIC stream.";
            break;
        }

        // Initialize the stream with the memory pointer and size.
        hr = p_wic_stream->InitializeFromMemory(
            reinterpret_cast<BYTE *>(p_resource_data), // Provide a pointer to the resource data for the stream.
            resource_size);                            // Provide the size of the resource data for the stream.
        if (FAILED(hr))
        {
            error_message = "Failed to initialize the WIC stream.";
            break;
        }

        // Create a decoder for the stream.
        hr = this->pWICImageFactory->CreateDecoderFromStream(
            p_wic_stream,
            NULL,
            WICDecodeMetadataCacheOnLoad,
            &p_wic_decoder);
        if (FAILED(hr))
        {
            error_message = "Failed to create a decoder from the WIC stream.";
            break;
        }

        // Get the first frame from the decoder, this frame contains the image.
        hr = p_wic_decoder->GetFrame(0, &p_wic_frame_decode);
        if (FAILED(hr))
        {
            error_message = "Failed to get the first frame from the WIC decoder";
            break;
        }

        // Create a WIC bitmap scaler to scale the image to the desired size.
        hr = this->pWICImageFactory->CreateBitmapScaler(&p_wic_scaler);
        if (FAILED(hr))
        {
            error_message = "Failed to create a WIC bitmap scaler.";
            break;
        }

        // Scale the bitmap to the desired size.
        hr = p_wic_scaler->Initialize(
            p_wic_frame_decode, // Provide the frame that contains the image.
            destinationWidth,   // Provide the width of the desired destination image.
            destinationHeight,  // Provide the height of the desired destination image.
            WICBitmapInterpolationModeFant);
        if (FAILED(hr))
        {
            error_message = "Failed to scale the bitmap to the desired size.";
            break;
        }

        // Create a format converter to convert the scaler output to the desired Direct2D format.
        // Otherwise the Direct2D won't be able to use the bitmap.
        hr = this->pWICImageFactory->CreateFormatConverter(&p_wic_format_converter);
        if (FAILED(hr))
        {
            error_message = "Failed to create a WIC format converter.";
            break;
        }

        // Set the desired pixel format.
        hr = p_wic_format_converter->Initialize(
            p_wic_scaler,         // Provide the scaler output. (The scaler output is now contains the resized image.)
            desired_pixel_format, // Provide the desired pixel format.
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut);
        if (FAILED(hr))
        {
            error_message = "Failed to format the bitmap to the desired format.";
            break;
        }

        // Store the WIC bitmap source.
        pIWCBitmapSource = p_wic_format_converter;
        p_wic_format_converter = nullptr;

        // Release the resources.
        if (p_wic_scaler)
            p_wic_scaler->Release();
        if (p_wic_frame_decode)
            p_wic_frame_decode->Release();
        if (p_wic_decoder)
            p_wic_decoder->Release();
        if (p_wic_stream)
            p_wic_stream->Release();

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'createWicBitmapSourceFromResource()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::createWicBitmapSourceFromIconResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR iconResourceName, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (pIWCBitmapSource)
        {
            error_message = "The wic bitmap source pointer is not null. (potential memory leak?)";
            break;
        }

        // Retrieve the pixel format of the render target.
        auto desired_pixel_format = (alphaMode == D2D1_ALPHA_MODE_PREMULTIPLIED ? GUID_WICPixelFormat32bppPBGRA : GUID_WICPixelFormat32bppBGRA);

        // These variables are used to create the WIC-compatible bitmap.
        IWICBitmap *p_wic_bitmap;                           // Pointer to the WIC bitmap.
        IWICFormatConverter *p_wic_format_converter = NULL; // Pointer to the WIC format converter.
        IWICBitmapScaler *p_wic_scaler = NULL;              // Pointer to the WIC bitmap scaler.

        // Load the icon resource to a icon handle. No need to release the icon handle as it is a shared resource. (LR_SHARED)
        HICON icon_handle = reinterpret_cast<HICON>(LoadImageW(GetModuleHandleW(NULL),
                                                               iconResourceName,
                                                               IMAGE_ICON,
                                                               destinationWidth,
                                                               destinationHeight,
                                                               LR_DEFAULTSIZE | LR_SHARED));
        if (!icon_handle)
        {
            error_message = "Failed to load the icon resource.";
            break;
        }

        // Create a WIC bitmap from the icon handle.
        hr = this->pWICImageFactory->CreateBitmapFromHICON(icon_handle, &p_wic_bitmap);
        if (FAILED(hr))
        {
            error_message = "Failed to create a WIC bitmap from the icon handle.";
            break;
        }

        // Create a WIC bitmap scaler to scale the image to the desired size.
        hr = this->pWICImageFactory->CreateBitmapScaler(&p_wic_scaler);
        if (FAILED(hr))
        {
            error_message = "Failed to create a WIC bitmap scaler.";
            break;
        }

        // Scale the bitmap to the desired size.
        hr = p_wic_scaler->Initialize(
            p_wic_bitmap,      // Provide the wic bitmap that contains the image.
            destinationWidth,  // Provide the width of the desired destination image.
            destinationHeight, // Provide the height of the desired destination image.
            WICBitmapInterpolationModeFant);
        if (FAILED(hr))
        {
            error_message = "Failed to scale the bitmap to the desired size.";
            break;
        }

        // Create a format converter to convert the scaler output to the desired Direct2D format.
        // Otherwise the Direct2D won't be able to use the bitmap.
        hr = this->pWICImageFactory->CreateFormatConverter(&p_wic_format_converter);
        if (FAILED(hr))
        {
            error_message = "Failed to create a WIC format converter.";
            break;
        }

        // Set the desired pixel format.
        hr = p_wic_format_converter->Initialize(
            p_wic_scaler,         // Provide the scaler output. (The scaler output is now contains the resized image.)
            desired_pixel_format, // Provide the desired pixel format.
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut);
        if (FAILED(hr))
        {
            error_message = "Failed to format the bitmap to the desired format.";
            break;
        }

        // Store the WIC bitmap source.
        pIWCBitmapSource = p_wic_format_converter;
        p_wic_format_converter = nullptr;

        // Release the resources.
        if (p_wic_scaler)
            p_wic_scaler->Release();
        if (p_wic_bitmap)
            p_wic_bitmap->Release();

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'createWicBitmapSourceFromIconResource()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::drawRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (!pRenderTarget)
        {
            error_message = "The render target pointer is null.";
            break;
        }

        // Disable anti-aliasing.
        auto previous_antialias_mode = pRenderTarget->GetAntialiasMode();
        pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        FLOAT half_stroke_width = strokeWidth / 2.0f;
        rect.left += half_stroke_width;
        rect.top += half_stroke_width;
        rect.right -= half_stroke_width;
        rect.bottom -= half_stroke_width;

        // Draw the rectangle.
        pRenderTarget->DrawRectangle(rect, pBrushRectangle, strokeWidth);

        // Restore the anti-aliasing mode.
        pRenderTarget->SetAntialiasMode(previous_antialias_mode);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'drawRectangle()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::drawFillRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (!pRenderTarget)
        {
            error_message = "The render target pointer is null.";
            break;
        }

        // Disable anti-aliasing.
        auto previous_antialias_mode = pRenderTarget->GetAntialiasMode();
        pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

        // Draw the fill rectangle.
        pRenderTarget->FillRectangle(rect, pBrushRectangle);

        // Draw the border of the rectangle.
        if (pBrushRectangleBorder)
        {
            FLOAT half_stroke_width = strokeWidth / 2.0f;
            rect.left += half_stroke_width;
            rect.top += half_stroke_width;
            rect.right -= half_stroke_width;
            rect.bottom -= half_stroke_width;
            pRenderTarget->DrawRectangle(rect, pBrushRectangleBorder, strokeWidth);
        }

        // Restore the anti-aliasing mode.
        pRenderTarget->SetAntialiasMode(previous_antialias_mode);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'drawFillRectangle()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::drawRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (!pRenderTarget)
        {
            error_message = "The render target pointer is null.";
            break;
        }

        // Create the rounded rectangle.
        FLOAT half_stroke_width = strokeWidth / 2.0f;
        rect.left += half_stroke_width;
        rect.top += half_stroke_width;
        rect.right -= half_stroke_width;
        rect.bottom -= half_stroke_width;
        D2D1_ROUNDED_RECT rounded_rect = D2D1::RoundedRect(rect, radiusX, radiusY);

        // Draw the rounded rectangle.
        pRenderTarget->DrawRoundedRectangle(rounded_rect, pBrushRectangle, strokeWidth);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'drawRoundRectangle()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::drawFillRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (!pRenderTarget)
        {
            error_message = "The render target pointer is null.";
            break;
        }

        // Create the rounded rectangle.
        D2D1_ROUNDED_RECT rounded_rect = D2D1::RoundedRect(rect, radiusX, radiusY);

        // Draw the filled rounded rectangle.
        pRenderTarget->FillRoundedRectangle(rounded_rect, pBrushRectangle);

        // Draw the border of the rectangle.
        if (pBrushRectangleBorder)
        {
            FLOAT half_stroke_width = strokeWidth / 2.0f;
            rect.left += half_stroke_width;
            rect.top += half_stroke_width;
            rect.right -= half_stroke_width;
            rect.bottom -= half_stroke_width;
            rounded_rect = D2D1::RoundedRect(rect, radiusX, radiusY);
            pRenderTarget->DrawRoundedRectangle(rounded_rect, pBrushRectangleBorder, strokeWidth);
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'drawFillRoundRectangle()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::drawText(ID2D1DCRenderTarget *&pRenderTarget, IDWriteTextFormat *&pTextFormat, D2D1_RECT_F rect, std::wstring text, ID2D1SolidColorBrush *&pBrushText, FLOAT posX, FLOAT posY, UINT centerMode)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (!pRenderTarget)
        {
            error_message = "The render target pointer is null.";
            break;
        }

        if (!pTextFormat)
        {
            error_message = "The direct write text format pointer is null.";
            break;
        }

        // Create the text layout.
        IDWriteTextLayout *p_text_layout;
        hr = this->pDWriteFactory->CreateTextLayout(text.c_str(), static_cast<UINT32>(text.length()), pTextFormat, rect.right - rect.left, rect.bottom - rect.top, &p_text_layout);
        if (FAILED(hr))
        {
            error_message = "Failed to create the text layout.";
            break;
        }

        // Center the text if specified.
        bool is_switch_success = false;
        switch (centerMode)
        {
        // No centering.
        case 0:
            is_switch_success = true;
            break;

        // Center the text horizontally.
        case 1:
        {
            hr = p_text_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            if (FAILED(hr))
            {
                error_message = "Failed to set the text alignment.";
                break;
            }
            is_switch_success = true;
            break;
        }

        // Center the text vertically.
        case 2:
        {
            hr = p_text_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            if (FAILED(hr))
            {
                error_message = "Failed to set the paragraph alignment.";
                break;
            }
            is_switch_success = true;
            break;
        }

        // Center the text both horizontally and vertically.
        case 3:
        {
            hr = p_text_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            if (FAILED(hr))
            {
                error_message = "Failed to set the text alignment.";
                break;
            }
            hr = p_text_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            if (FAILED(hr))
            {
                error_message = "Failed to set the paragraph alignment.";
                break;
            }
            is_switch_success = true;
            break;
        }

        default:
            break;
        }
        if (!is_switch_success)
            break;

        // Draw the text.
        pRenderTarget->DrawTextLayout(D2D1::Point2F(posX, posY), p_text_layout, pBrushText);

        // Release the text layout.
        p_text_layout->Release();

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'drawText()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyD2D1Engine::drawImage(ID2D1DCRenderTarget *&pRenderTarget, ID2D1Bitmap *&pD2D1Bitmap, D2D1_RECT_F rect, FLOAT opacity, FLOAT posX, FLOAT posY, FLOAT width, FLOAT height, bool centering, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode)
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        if (!pRenderTarget)
        {
            error_message = "The render target pointer is null.";
            break;
        }

        if (!pD2D1Bitmap)
        {
            error_message = "The bitmap pointer is null.";
            break;
        }

        // Center the image if specified.
        if (centering)
        {
            posX = (rect.right - rect.left) / 2.0f - width / 2.0f;
            posY = (rect.bottom - rect.top) / 2.0f - height / 2.0f;
        }

        // Get the bitmap size.
        D2D1_SIZE_F bitmap_size = pD2D1Bitmap->GetSize();

        // Calculate the destination rectangle.
        D2D1_RECT_F destination_rect = D2D1::RectF(posX, posY, posX + width, posY + height);

        // Calculate the source rectangle.
        D2D1_RECT_F source_rect = D2D1::RectF(0, 0, bitmap_size.width, bitmap_size.height);

        // Draw the bitmap.
        pRenderTarget->DrawBitmap(pD2D1Bitmap, destination_rect, opacity, interpolationMode, source_rect);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyD2D1Engine' | FUNC: 'drawImage()']", MyLogType::Error);

    return are_all_operation_success;
}

// [MyGraphicEngine] class implementations:

MyGraphicEngine::MyGraphicEngine()
{
    if (!MyGraphicEngine::instanceExists)
        MyGraphicEngine::instanceExists = true;
    else
    {
        g_pApp->logger.writeLog("The graphic engine instance already exists.", "[CLASS: 'MyGraphicEngine' | FUNC: 'Constructor']", MyLogType::Error);
        throw std::runtime_error("The graphic engine instance already exists.");
    }
}
MyGraphicEngine::~MyGraphicEngine() noexcept(false)
{
    if (this->initialized)
    {
        if (!this->uninitialize())
        {
            g_pApp->logger.writeLog("Failed to uninitialize the graphic engine.", "[CLASS: 'MyGraphicEngine' | 'Destructor']", MyLogType::Error);
            throw std::runtime_error("Failed to uninitialize the graphic engine.");
        }
    }

    MyGraphicEngine::instanceExists = false;
}
MyWAMEngine &MyGraphicEngine::wamEngine()
{
    return *this->pWAMEngine;
}
MyD2D1Engine &MyGraphicEngine::d2d1Engine()
{
    return *this->pD2D1Engine;
}
bool MyGraphicEngine::initialize()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (this->initialized)
        {
            error_message = "The engine is already initialized.";
            break;
        }

        // Initialize the WAM Engine.
        this->pWAMEngine = std::make_unique<MyWAMEngine>();
        if (!this->pWAMEngine.get()->initialize())
        {
            error_message = "Failed to initialize the WAM engine.";
            break;
        }

        // Initialize the Direct2D Engine.
        this->pD2D1Engine = std::make_unique<MyD2D1Engine>();
        if (!this->d2d1Engine().initialize())
        {
            error_message = "Failed to initialize the Direct2D engine.";
            break;
        }

        this->initialized = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyGraphicEngine' | FUNC: 'initialize()']", MyLogType::Error);

    return are_all_operation_success;
}
bool MyGraphicEngine::uninitialize()
{
    bool are_all_operation_success = false;
    std::string error_message = "";
    while (!are_all_operation_success)
    {
        if (!this->initialized)
        {
            error_message = "The engine is not initialized yet.";
            break;
        }

        // Uninitialize the Direct2D Engine.
        if (!this->d2d1Engine().uninitialize())
        {
            error_message = "Failed to uninitialize the Direct2D engine.";
            break;
        }

        // Uninitialize the WAM Engine.
        if (!this->pWAMEngine.get()->uninitialize())
        {
            error_message = "Failed to uninitialize the WAM engine.";
            break;
        }

        this->initialized = false;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
        g_pApp->logger.writeLog(error_message, "[CLASS: 'MyGraphicEngine' | FUNC: 'uninitialize()']", MyLogType::Error);

    return are_all_operation_success;
}