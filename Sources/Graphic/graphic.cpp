/**
 * @file graphic.cpp
 * @brief Implementation of the classes defined in graphic.h.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../Global/global.h"                 // The distribution header of the global variables, forward declarations and my class declarations.

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
ID2D1DCRenderTarget *MyD2D1Engine::createDCRenderTarget(HDC &hDC, RECT &rect, bool disableRTLRendering)
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

        // Set the layout of the GDI device context.
        // Readmore: https://learn.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-gdi-interoperation-overview
        if (disableRTLRendering)
        {
            if (SetLayout(hDC, LAYOUT_BITMAPORIENTATIONPRESERVED) == GDI_ERROR)
            {
                error_message = L"Failed to set the layout of the GDI device context.";
                break;
            }
        }

        // Create render target properties.
        D2D1_RENDER_TARGET_PROPERTIES rendertarget_properties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                                                                             D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
                                                                                             0, 0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT);

        // Create the D2D1 device context render target.
        hr = this->pFactory->CreateDCRenderTarget(&rendertarget_properties, &p_rendertarget);
        if (FAILED(hr))
        {
            error_message = L"Failed to create D2D1 device context render target.";
            break;
        }

        // Bind the D2D1 device context render target to the GDI device context.
        hr = p_rendertarget->BindDC(hDC, &rect);
        if (FAILED(hr))
        {
            error_message = L"Failed to bind the D2D1 device context render target to the GDI device context.";
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