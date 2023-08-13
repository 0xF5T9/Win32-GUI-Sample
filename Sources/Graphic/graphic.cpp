/**
 * @file graphic.cpp
 * @brief Implementation of the classes defined in graphic.h.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../Global/global.h"                 // The distribution header of the global variables, forward declarations and my class declarations.

/// @class MyGDIPEngine definitions:
// Destructor:
MyGDIPEngine::~MyGDIPEngine()
{
    // Uninitialize the GDI+ engine.
    // The uninitialize() function should be called manually.
    if (this->isInitialized)
    {
        this->uninitialize();
    }
}
// Public member function(s) [INITIALIZATION FUNCTIONS]:
bool MyGDIPEngine::initialize()
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the GDI+ engine is already initialized.
        if (this->isInitialized)
        {
            error_message = L"The GDI+ engine is already initialized.";
            break;
        }

        // Initialize the GDI+ API.
        if (Gdiplus::GdiplusStartup(&this->gdipToken, &this->gdipStartupInput, NULL))
        {
            error_message = L"Failed to initialize GDI+ API.";
            break;
        }

        // Update initialization status.
        this->isInitialized = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyGDIPEngine\" | FUNC: \"initialize()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyGDIPEngine::uninitialize()
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the GDI+ engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The GDI+ engine is not initialized yet.";
            break;
        }

        // Shutdown GDI+ API.
        Gdiplus::GdiplusShutdown(this->gdipToken);

        // Update initialization status.
        this->isInitialized = false;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyGDIPEngine\" | FUNC: \"uninitialize()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyGDIPEngine::isReady() const
{
    return this->isInitialized;
}

/// @class MyD2D1Engine definitions:
// Destructor:
MyD2D1Engine::~MyD2D1Engine()
{
    // Uninitialize the D2D1 engine.
    // The uninitialize() function should be called manually.
    if (this->isInitialized)
    {
        this->uninitialize();
    }
}
// Public member function(s) [INITIALIZATION FUNCTIONS]:
bool MyD2D1Engine::initialize()
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        // Check if the D2D1 engine is already initialized.
        if (this->isInitialized)
        {
            error_message = L"The D2D1 engine is already initialized.";
            break;
        }

        // Create the D2D1 factory.
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pFactory);
        if (FAILED(hr))
        {
            error_message = L"Failed to create the D2D1 factory.";
            break;
        }

        // Create a DirectWrite factory.
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(this->pWriteFactory),
            reinterpret_cast<IUnknown **>(&this->pWriteFactory));
        if (FAILED(hr))
        {
            error_message = L"Failed to create a DirectWrite factory.";
            break;
        }

        // Create a WIC factory.
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            reinterpret_cast<void **>(&this->pWICFactory));
        if (FAILED(hr))
        {
            error_message = L"Failed to create a WIC factory.";
            break;
        }

        // Update initialization status.
        this->isInitialized = true;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"initialize()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::uninitialize()
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
            break;
        }

        // Release the WIC factory.
        if (this->pWICFactory)
        {
            this->pWICFactory->Release();
            this->pWICFactory = nullptr;
        }

        // Release the DirectWrite factory.
        if (this->pWriteFactory)
        {
            this->pWriteFactory->Release();
            this->pWriteFactory = nullptr;
        }

        // Release the D2D1 factory.
        if (this->pFactory)
        {
            this->pFactory->Release();
            this->pFactory = nullptr;
        }

        // Update initialization status.
        this->isInitialized = false;

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"uninitialize()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
// Public member function(s) [GENERAL FUNCTIONS]:
bool MyD2D1Engine::isReady() const
{
    return this->isInitialized;
}
// Public member function(s) [CREATE RESOURCE HELPER FUNCTIONS]:
ID2D1DCRenderTarget *MyD2D1Engine::createDCRenderTarget(D2D1_RENDER_TARGET_TYPE renderTargetType, D2D1_RENDER_TARGET_USAGE renderTargetUsage, D2D1_ALPHA_MODE renderAlphaMode)
{
    HRESULT hr;
    ID2D1DCRenderTarget *p_rendertarget = nullptr;
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
            break;
        }

        // Create render target properties.
        D2D1_RENDER_TARGET_PROPERTIES rendertarget_properties = D2D1::RenderTargetProperties(renderTargetType,
                                                                                             D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, renderAlphaMode),
                                                                                             0, 0, renderTargetUsage, D2D1_FEATURE_LEVEL_DEFAULT);

        // Create the D2D1 device context render target.
        hr = this->pFactory->CreateDCRenderTarget(&rendertarget_properties, &p_rendertarget);
        if (FAILED(hr))
        {
            error_message = L"Failed to create D2D1 device context render target.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"createDCRenderTarget()\"]", MyLogType::Error);
        if (p_rendertarget)
            p_rendertarget->Release();
        return nullptr;
    }

    return p_rendertarget;
}
bool MyD2D1Engine::createTextFormat(IDWriteTextFormat *&pTextFormat, std::wstring fontFamilyName, float fontSize, DWRITE_FONT_WEIGHT fontWeight,
                                    DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
            break;
        }

        // Create a DirectWrite text format object.
        HRESULT hr = this->pWriteFactory->CreateTextFormat(
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
            error_message = L"Failed to create the DirectWrite text format object.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"createTextFormat()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::createWicBitmapSourceFromResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

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
            error_message = L"This function does not support the resource type \"RT_GROUP_ICON\". Use the function \"createWicBitmapSourceFromIconResource()\" instead.";
            break;
        }

        // Locate the resource.
        resource_handle = FindResourceW(NULL, resourceName, resourceType);
        if (!resource_handle)
        {
            error_message = L"Failed to locate the resource.";
            break;
        }

        // Load the resource.
        resource_data_handle = LoadResource(NULL, resource_handle);
        if (!resource_data_handle)
        {
            error_message = L"Failed to load the resource.";
            break;
        }

        // Lock the resource and get pointer to the resource data.
        p_resource_data = LockResource(resource_data_handle);
        if (!p_resource_data)
        {
            error_message = L"Failed to lock the resource.";
            break;
        }

        // Calculate the resource data size.
        resource_size = SizeofResource(NULL, resource_handle);
        if (!resource_size)
        {
            error_message = L"Failed to calculate the resource data size.";
            break;
        }

        // Create a WIC stream to map onto the memory.
        hr = this->pWICFactory->CreateStream(&p_wic_stream);
        if (FAILED(hr))
        {
            error_message = L"Failed to create a WIC stream.";
            break;
        }

        // Initialize the stream with the memory pointer and size.
        hr = p_wic_stream->InitializeFromMemory(
            reinterpret_cast<BYTE *>(p_resource_data), // Provide a pointer to the resource data for the stream.
            resource_size);                            // Provide the size of the resource data for the stream.
        if (FAILED(hr))
        {
            error_message = L"Failed to initialize the WIC stream.";
            break;
        }

        // Create a decoder for the stream.
        hr = this->pWICFactory->CreateDecoderFromStream(
            p_wic_stream,
            NULL,
            WICDecodeMetadataCacheOnLoad,
            &p_wic_decoder);
        if (FAILED(hr))
        {
            error_message = L"Failed to create a decoder from the WIC stream.";
            break;
        }

        // Get the first frame from the decoder, this frame contains the image.
        hr = p_wic_decoder->GetFrame(0, &p_wic_frame_decode);
        if (FAILED(hr))
        {
            error_message = L"Failed to get the first frame from the WIC decoder";
            break;
        }

        // Create a WIC bitmap scaler to scale the image to the desired size.
        hr = this->pWICFactory->CreateBitmapScaler(&p_wic_scaler);
        if (FAILED(hr))
        {
            error_message = L"Failed to create a WIC bitmap scaler.";
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
            error_message = L"Failed to scale the bitmap to the desired size.";
            break;
        }

        // Create a format converter to convert the scaler output to the desired Direct2D format.
        // Otherwise the Direct2D won't be able to use the bitmap.
        hr = this->pWICFactory->CreateFormatConverter(&p_wic_format_converter);
        if (FAILED(hr))
        {
            error_message = L"Failed to create a WIC format converter.";
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
            error_message = L"Failed to format the bitmap to the desired format.";
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
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"createWicBitmapSourceFromResource()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::createWicBitmapSourceFromIconResource(IWICBitmapSource *&pIWCBitmapSource, PCWSTR iconResourceName, UINT destinationWidth, UINT destinationHeight, D2D1_ALPHA_MODE alphaMode)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

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
            error_message = L"Failed to load the icon resource.";
            break;
        }

        // Create a WIC bitmap from the icon handle.
        hr = this->pWICFactory->CreateBitmapFromHICON(icon_handle, &p_wic_bitmap);
        if (FAILED(hr))
        {
            error_message = L"Failed to create a WIC bitmap from the icon handle.";
            break;
        }

        // Create a WIC bitmap scaler to scale the image to the desired size.
        hr = this->pWICFactory->CreateBitmapScaler(&p_wic_scaler);
        if (FAILED(hr))
        {
            error_message = L"Failed to create a WIC bitmap scaler.";
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
            error_message = L"Failed to scale the bitmap to the desired size.";
            break;
        }

        // Create a format converter to convert the scaler output to the desired Direct2D format.
        // Otherwise the Direct2D won't be able to use the bitmap.
        hr = this->pWICFactory->CreateFormatConverter(&p_wic_format_converter);
        if (FAILED(hr))
        {
            error_message = L"Failed to create a WIC format converter.";
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
            error_message = L"Failed to format the bitmap to the desired format.";
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
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"createWicBitmapSourceFromIconResource()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
// Public member function(s) [DRAWING HELPER FUNCTIONS]:
bool MyD2D1Engine::drawRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
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

        // Restore anti-aliasing.
        pRenderTarget->SetAntialiasMode(previous_antialias_mode);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"drawRectangle()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::drawFillRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
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

        // Restore anti-aliasing.
        pRenderTarget->SetAntialiasMode(previous_antialias_mode);

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"drawFillRectangle()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::drawRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
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
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"drawRoundRectangle()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::drawFillRoundRectangle(ID2D1DCRenderTarget *&pRenderTarget, D2D1_RECT_F rect, FLOAT radiusX, FLOAT radiusY, ID2D1SolidColorBrush *&pBrushRectangle, ID2D1SolidColorBrush *pBrushRectangleBorder, FLOAT strokeWidth)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
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
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"drawFillRoundRectangle()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::drawText(ID2D1DCRenderTarget *&pRenderTarget, IDWriteTextFormat *&pTextFormat, D2D1_RECT_F rect, std::wstring text, ID2D1SolidColorBrush *&pBrushText, FLOAT posX, FLOAT posY, bool centerText)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        HRESULT hr;

        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
            break;
        }

        // Create the text layout.
        IDWriteTextLayout *p_text_layout;
        hr = this->pWriteFactory->CreateTextLayout(text.c_str(), static_cast<UINT32>(text.length()), pTextFormat, rect.right - rect.left, rect.bottom - rect.top, &p_text_layout);
        if (FAILED(hr))
        {
            error_message = L"Failed to create the text layout.";
            break;
        }

        // Center the text if specified.
        if (centerText)
        {
            hr = p_text_layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            if (FAILED(hr))
            {
                error_message = L"Failed to set the text alignment.";
                break;
            }
            hr = p_text_layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            if (FAILED(hr))
            {
                error_message = L"Failed to set the paragraph alignment.";
                break;
            }
        }

        // Draw the text.
        pRenderTarget->DrawTextLayout(D2D1::Point2F(posX, posY), p_text_layout, pBrushText);

        // Release the text layout.
        p_text_layout->Release();

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"drawText()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}
bool MyD2D1Engine::drawImage(ID2D1DCRenderTarget *&pRenderTarget, ID2D1Bitmap *&pD2D1Bitmap, D2D1_RECT_F rect, FLOAT opacity, FLOAT posX, FLOAT posY, FLOAT width, FLOAT height, bool centering, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Check if the D2D1 engine is not initialized yet.
        if (!this->isInitialized)
        {
            error_message = L"The D2D1 engine is not initialized yet.";
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
    {
        WriteLog(error_message, L" [CLASS: \"MyD2D1Engine\" | FUNC: \"drawImage()\"]", MyLogType::Error);
    }

    return are_all_operation_success;
}