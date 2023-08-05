/**
 * @file ui.cpp
 * @brief Implementation of the classes defined in ui.h.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../Global/global.h"                 // The distribution header of the global variables, forward declarations and my class declarations.

/// @class MyColor definitions:
// Constructors:
MyColor::MyColor(BYTE red, BYTE green, BYTE blue, BYTE alpha) : red(red), green(green), blue(blue), alpha(alpha)
{
    // Variant(s).
    this->clr = RGB(this->red, this->green, this->blue);
    this->gdipColor = Gdiplus::Color(this->alpha, this->red, this->green, this->blue);
    this->hBrush = CreateSolidBrush(this->clr);
    if (!this->hBrush)
        WriteLog(L"Failed to create the brush object.", L" [CLASS: \"MyColor\" | FUNC: \"MyColor()\"]", MyLogType::Error);

    MyColor::totalInstances++;
}
MyColor::MyColor(RGBA colors) : red(colors.red), green(colors.green), blue(colors.blue), alpha(colors.alpha)
{
    // Variant(s).
    this->clr = RGB(this->red, this->green, this->blue);
    this->gdipColor = Gdiplus::Color(this->alpha, this->red, this->green, this->blue);
    this->hBrush = CreateSolidBrush(this->clr);
    if (!this->hBrush)
        WriteLog(L"Failed to create the brush object.", L" [CLASS: \"MyColor\" | FUNC: \"MyColor()\"]", MyLogType::Error);

    MyColor::totalInstances++;
}
// Copy constructor:
MyColor::MyColor(const MyColor &other) : red(other.red), green(other.green), blue(other.blue), alpha(other.alpha)
{
    // Variant(s).
    this->clr = RGB(this->red, this->green, this->blue);
    this->gdipColor = Gdiplus::Color(this->alpha, this->red, this->green, this->blue);
    this->hBrush = CreateSolidBrush(this->clr);
    if (!this->hBrush)
        WriteLog(L"Failed to create the brush object.", L" [CLASS: \"MyColor\" | FUNC: \"MyColor()\"]", MyLogType::Error);

    MyColor::totalInstances++;
}
// Assignment operator:
MyColor &MyColor::operator=(const MyColor &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Allocation variable(s).
    this->red = other.red;
    this->green = other.green;
    this->blue = other.blue;
    this->alpha = other.alpha;

    // Variant(s).
    this->clr = RGB(this->red, this->green, this->blue);
    this->gdipColor = Gdiplus::Color(this->alpha, this->red, this->green, this->blue);
    if (this->hBrush)
    {
        if (!DeleteObject(this->hBrush))
            WriteLog(L"Failed to delete the brush object.", L" [CLASS: \"MyColor\" | FUNC: \"&operator=()\"]", MyLogType::Error);
        this->hBrush = nullptr;
    }
    this->hBrush = CreateSolidBrush(this->clr);
    if (!this->hBrush)
        WriteLog(L"Failed to create the brush object.", L" [CLASS: \"MyColor\" | FUNC: \"&operator=()\"]", MyLogType::Error);

    return *this;
}
// Destructor:
MyColor::~MyColor()
{
    // Deallocation of unmanaged object(s).
    if (this->hBrush)
    {
        if (!DeleteObject(this->hBrush))
            WriteLog(L"Failed to delete the brush object.", L" [CLASS: \"MyColor\" | FUNC: \"~MyColor()\"]", MyLogType::Error);
        this->hBrush = nullptr;
    }

    MyColor::totalInstances--;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
UINT MyColor::getTotalInstances()
{
    return MyColor::totalInstances;
}
// Public member function(s) [GENERAL FUNCTIONS]:
BYTE MyColor::getRed() const
{
    return this->red;
}
BYTE MyColor::getGreen() const
{
    return this->green;
}
BYTE MyColor::getBlue() const
{
    return this->blue;
}
BYTE MyColor::getAlpha() const
{
    return this->alpha;
}
const COLORREF &MyColor::getCOLORREF() const
{
    return this->clr;
}
Gdiplus::Color &MyColor::getGDIPColor()
{
    return this->gdipColor;
}
HBRUSH &MyColor::getHBRUSH()
{
    return this->hBrush;
}
bool MyColor::update(BYTE red, BYTE green, BYTE blue, BYTE alpha)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Allocation variable(s).
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->alpha = alpha;

        // Variant(s).
        this->clr = RGB(this->red, this->green, this->blue);
        this->gdipColor = Gdiplus::Color(this->alpha, this->red, this->green, this->blue);
        if (this->hBrush)
        {
            if (!DeleteObject(this->hBrush))
            {
                error_message = L"Failed to delete the brush object.";
                break;
            }
            this->hBrush = nullptr;
        }
        this->hBrush = CreateSolidBrush(this->clr);
        if (!this->hBrush)
        {
            error_message = L"Failed to create the brush object.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyColor\" | FUNC: \"update()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class MyFont definitions:
// Constructors:
MyFont::MyFont(std::wstring name, USHORT size, USHORT weight, DWORD quality) : name(name), size(size), weight(weight), quality(quality)
{
    // Variant(s).
    this->hFont = CreateFontW(static_cast<INT>(this->size), 0, 0, 0x1, static_cast<INT>(this->weight),
                              FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, this->quality,
                              DEFAULT_PITCH | FF_DONTCARE, static_cast<LPCWSTR>(this->name.c_str()));
    if (!this->hFont)
        WriteLog(L"Failed to create the font object.", L" [CLASS: \"MyFont\" | FUNC: \"MyFont()\"]", MyLogType::Error);

    MyFont::totalInstances++;
}
// Copy constructor:
MyFont::MyFont(const MyFont &other) : name(other.name), size(other.size), weight(other.weight), quality(other.quality)
{
    // Variant(s).
    this->hFont = CreateFontW(static_cast<INT>(this->size), 0, 0, 0x1, static_cast<INT>(this->weight),
                              FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, this->quality,
                              DEFAULT_PITCH | FF_DONTCARE, static_cast<LPCWSTR>(this->name.c_str()));
    if (!this->hFont)
        WriteLog(L"Failed to create the font object.", L" [CLASS: \"MyFont\" | FUNC: \"MyFont()\"]", MyLogType::Error);

    MyFont::totalInstances++;
}
// Assignment operator:
MyFont &MyFont::operator=(const MyFont &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Allocation variable(s).
    this->name = other.name;
    this->size = other.size;
    this->weight = other.weight;
    this->quality = other.quality;

    // Variant(s).
    if (this->hFont)
    {
        if (!DeleteObject(this->hFont))
            WriteLog(L"Failed to delete the font object.", L" [CLASS: \"MyFont\" | FUNC: \"&operator=()\"]", MyLogType::Error);
        this->hFont = nullptr;
    }
    this->hFont = CreateFontW(static_cast<INT>(this->size), 0, 0, 0x1, static_cast<INT>(this->weight),
                              FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, this->quality,
                              DEFAULT_PITCH | FF_DONTCARE, static_cast<LPCWSTR>(this->name.c_str()));
    if (!this->hFont)
        WriteLog(L"Failed to create the font object.", L" [CLASS: \"MyFont\" | FUNC: \"&operator=()\"]", MyLogType::Error);

    return *this;
}
// Destructor:
MyFont::~MyFont()
{
    // Deallocation of unmanaged object(s).
    if (this->hFont)
    {
        if (!DeleteObject(this->hFont))
            WriteLog(L"Failed to delete the font object.", L" [CLASS: \"MyFont\" | FUNC: \"~MyFont()\"]", MyLogType::Error);
        this->hFont = nullptr;
    }

    MyFont::totalInstances--;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
UINT MyFont::getTotalInstances()
{
    return MyFont::totalInstances;
}
// Public member function(s) [GENERAL FUNCTIONS]:
std::wstring MyFont::getName() const
{
    return this->name;
}
USHORT MyFont::getSize() const
{
    return this->size;
}
USHORT MyFont::getWeight() const
{
    return this->weight;
}
DWORD MyFont::getQuality() const
{
    return this->quality;
}
HFONT &MyFont::getHFONT()
{
    return this->hFont;
}
bool MyFont::update(std::wstring name, USHORT size, USHORT weight, DWORD quality)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Allocation variable(s).
        this->name = name;
        this->size = size;
        this->weight = weight;
        this->quality = quality;

        // Variant(s).
        if (this->hFont)
        {
            if (!DeleteObject(this->hFont))
            {
                error_message = L"Failed to delete the font object.";
                break;
            }
            this->hFont = nullptr;
        }
        this->hFont = CreateFontW(static_cast<INT>(this->size), 0, 0, 0x1, static_cast<INT>(this->weight),
                                  FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, this->quality,
                                  DEFAULT_PITCH | FF_DONTCARE, static_cast<LPCWSTR>(this->name.c_str()));
        if (!this->hFont)
        {
            error_message = L"Failed to create the font object.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyFont\" | FUNC: \"update()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class MyIcon definitions:
// Constructors:
MyIcon::MyIcon(HINSTANCE hInstance, INT width, INT height, INT resourceID, UINT loadImageFlags) : hInstance(hInstance), width(width), height(height), resourceID(resourceID), loadImageFlags(loadImageFlags)
{
    // Variant(s).
    this->hIcon = reinterpret_cast<HICON>(LoadImageW(this->hInstance, MAKEINTRESOURCEW(this->resourceID), IMAGE_ICON, this->width, this->height, this->loadImageFlags));
    if (!this->hIcon)
        WriteLog(L"Failed to create the icon object.", L" [CLASS: \"MyIcon\" | FUNC: \"MyIcon()\"]", MyLogType::Error);

    MyIcon::totalInstances++;
}
// Copy constructor:
MyIcon::MyIcon(const MyIcon &other) : hInstance(other.hInstance), width(other.width), height(other.height), resourceID(other.resourceID), loadImageFlags(other.loadImageFlags)
{
    // Variant(s).
    this->hIcon = reinterpret_cast<HICON>(LoadImageW(this->hInstance, MAKEINTRESOURCEW(this->resourceID), IMAGE_ICON, this->width, this->height, this->loadImageFlags));
    if (!this->hIcon)
        WriteLog(L"Failed to create the icon object.", L" [CLASS: \"MyIcon\" | FUNC: \"MyIcon()\"]", MyLogType::Error);

    MyIcon::totalInstances++;
}
// Assignment operator:
MyIcon &MyIcon::operator=(const MyIcon &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Allocation variable(s).
    this->hInstance = other.hInstance;
    this->width = other.width;
    this->height = other.height;
    this->resourceID = other.resourceID;
    this->loadImageFlags = other.loadImageFlags;

    // Variant(s).
    if (this->hIcon)
    {
        if (!DestroyIcon(this->hIcon))
            WriteLog(L"Failed to delete the icon object.", L" [CLASS: \"MyIcon\" | FUNC: \"&operator=()\"]", MyLogType::Error);
        this->hIcon = nullptr;
    }
    this->hIcon = reinterpret_cast<HICON>(LoadImageW(this->hInstance, MAKEINTRESOURCEW(this->resourceID), IMAGE_ICON, this->width, this->height, this->loadImageFlags));
    if (!this->hIcon)
        WriteLog(L"Failed to create the icon object.", L" [CLASS: \"MyIcon\" | FUNC: \"&operator=()\"]", MyLogType::Error);

    return *this;
}
// Destructor:
MyIcon::~MyIcon()
{
    // Deallocation of unmanaged object(s).
    if (this->hIcon)
    {
        if (!DestroyIcon(this->hIcon))
            WriteLog(L"Failed to delete the icon object.", L" [CLASS: \"MyIcon\" | FUNC: \"~MyIcon()\"]", MyLogType::Error);
        this->hIcon = nullptr;
    }

    MyIcon::totalInstances--;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
UINT MyIcon::getTotalInstances()
{
    return MyIcon::totalInstances;
}
// Public member function(s) [GENERAL FUNCTIONS]:
HINSTANCE MyIcon::getHINSTANCE() const
{
    return this->hInstance;
}
INT MyIcon::getWidth() const
{
    return this->width;
}
INT MyIcon::getHeight() const
{
    return this->height;
}
INT MyIcon::getResourceID() const
{
    return this->resourceID;
}
UINT MyIcon::getLoadImageFlags() const
{
    return this->loadImageFlags;
}
HICON &MyIcon::getHICON()
{
    return this->hIcon;
}
bool MyIcon::update(HINSTANCE hInstance, INT width, INT height, INT resourceID, UINT loadImageFlags)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Allocation variable(s).
        this->hInstance = hInstance;
        this->width = width;
        this->height = height;
        this->resourceID = resourceID;
        this->loadImageFlags = loadImageFlags;

        // Variant(s).
        if (this->hIcon)
        {
            if (!DestroyIcon(this->hIcon))
            {
                error_message = L"Failed to delete the icon object.";
                break;
            }
            this->hIcon = nullptr;
        }
        this->hIcon = reinterpret_cast<HICON>(LoadImageW(this->hInstance, MAKEINTRESOURCEW(this->resourceID), IMAGE_ICON, this->width, this->height, this->loadImageFlags));
        if (!this->hIcon)
        {
            error_message = L"Failed to create the icon object.";
            break;
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyIcon\" | FUNC: \"update()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class MyImage definitions:
// Constructors:
MyImage::MyImage(INT resourceID, MyImageFormat format) : resourceID(resourceID), format(format)
{
    // Variant(s).
    {
        // Create the image object.
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get resource type string.
            std::wstring resource_type = L"";
            bool is_predefined_resource_type = false;
            auto predefined_resource_type = RT_RCDATA;
            switch (this->format)
            {
            case MyImageFormat::JPG:
                resource_type = L"JPG";
                break;
            case MyImageFormat::PNG:
                resource_type = L"PNG";
                break;
            case MyImageFormat::ICO:
                resource_type = L"ICO";
                predefined_resource_type = RT_GROUP_ICON;
                is_predefined_resource_type = true;
                // Note: For now loading ico file from application resource will result in GDI+ error code 2
                // as the Gdiplus::Image::FromStream() method expect the stream to contains a single icon and not the whole icon group.
                // This can be solve by using BeginResourceExtract() to extract the icon from the icon group.
                // Further work must be done.
                break;
            default:
                break;
            }
            if (resource_type.empty())
            {
                error_message = L"Invalid resource format.";
                break;
            }

            // Find the resource.
            HRSRC resource_handle = FindResourceW(NULL, MAKEINTRESOURCEW(this->resourceID), (is_predefined_resource_type ? predefined_resource_type : resource_type.c_str()));
            if (!resource_handle)
            {
                error_message = L"Resource not found.";
                break;
            }

            // Get the resource size.
            DWORD resource_size = SizeofResource(NULL, resource_handle);
            if (!resource_size)
            {
                error_message = L"Failed to retrieve the resource size.";
                break;
            }

            // Load the resource.
            HGLOBAL global_handle = LoadResource(NULL, resource_handle);
            if (!global_handle)
            {
                error_message = L"Failed to load the resource.";
                break;
            }

            // Lock the resource and get pointer to the resource data.
            const void *p_resource_data = LockResource(global_handle);
            if (!p_resource_data)
            {
                error_message = L"Failed to lock the resource.";
                break;
            }

            // Allocate the global memory object.
            HGLOBAL buffer_handle = GlobalAlloc(GMEM_MOVEABLE, resource_size);
            if (!buffer_handle)
            {
                error_message = L"Failed to allocate the global memory object.";
                break;
            }

            // Lock the global memory object.
            void *p_buffer = GlobalLock(buffer_handle);
            if (!p_buffer)
            {
                error_message = L"Failed to lock the global memory object.";
                break;
            }
            memcpy(p_buffer, p_resource_data, resource_size);

            // Create stream from the resource data.
            HRESULT hr = CreateStreamOnHGlobal(buffer_handle, TRUE, &this->pStream);
            if (FAILED(hr))
            {
                error_message = L"Failed to create stream from resource data.";
                break;
            }

            // Create the image object from the stream.
            this->pImage = Gdiplus::Image::FromStream(this->pStream);
            if (!this->pImage)
            {
                error_message = L"Failed to create the image object from the stream (The GDI+ API might not be initialized yet).";
                break;
            }
            auto status = this->pImage->GetLastStatus();
            if (status)
            {
                error_message = L"Failed to create the image object from the stream (GDI+ Error Code: " + std::to_wstring(status) + L").";
                break;
            }

            are_all_operation_success = true;
        }

        // Check if the image object was successfully created.
        if (!are_all_operation_success)
        {
            WriteLog(L"Failed to create the image object.", L" [CLASS: \"MyImage\" | FUNC: \"MyImage()\"]", MyLogType::Error);
        }
    }

    MyImage::totalInstances++;
}
// Copy constructor:
MyImage::MyImage(const MyImage &other) : resourceID(other.resourceID), format(other.format)
{
    // Variant(s).
    {
        // Create the image object.
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get resource type string.
            std::wstring resource_type = L"";
            bool is_predefined_resource_type = false;
            auto predefined_resource_type = RT_RCDATA;
            switch (this->format)
            {
            case MyImageFormat::JPG:
                resource_type = L"JPG";
                break;
            case MyImageFormat::PNG:
                resource_type = L"PNG";
                break;
            case MyImageFormat::ICO:
                resource_type = L"ICO";
                predefined_resource_type = RT_GROUP_ICON;
                is_predefined_resource_type = true;
                // Note: For now loading ico file from application resource will result in GDI+ error code 2
                // as the Gdiplus::Image::FromStream() method expect the stream to contains a single icon and not the whole icon group.
                // This can be solve by using BeginResourceExtract() to extract the icon from the icon group.
                // Further work must be done.
                break;
            default:
                break;
            }
            if (resource_type.empty())
            {
                error_message = L"Invalid resource format.";
                break;
            }

            // Find the resource.
            HRSRC resource_handle = FindResourceW(NULL, MAKEINTRESOURCEW(this->resourceID), (is_predefined_resource_type ? predefined_resource_type : resource_type.c_str()));
            if (!resource_handle)
            {
                error_message = L"Resource not found.";
                break;
            }

            // Get the resource size.
            DWORD resource_size = SizeofResource(NULL, resource_handle);
            if (!resource_size)
            {
                error_message = L"Failed to retrieve the resource size.";
                break;
            }

            // Load the resource.
            HGLOBAL global_handle = LoadResource(NULL, resource_handle);
            if (!global_handle)
            {
                error_message = L"Failed to load the resource.";
                break;
            }

            // Lock the resource and get pointer to the resource data.
            const void *p_resource_data = LockResource(global_handle);
            if (!p_resource_data)
            {
                error_message = L"Failed to lock the resource.";
                break;
            }

            // Allocate the global memory object.
            HGLOBAL buffer_handle = GlobalAlloc(GMEM_MOVEABLE, resource_size);
            if (!buffer_handle)
            {
                error_message = L"Failed to allocate the global memory object.";
                break;
            }

            // Lock the global memory object.
            void *p_buffer = GlobalLock(buffer_handle);
            if (!p_buffer)
            {
                error_message = L"Failed to lock the global memory object.";
                break;
            }
            memcpy(p_buffer, p_resource_data, resource_size);

            // Create stream from the resource data.
            HRESULT hr = CreateStreamOnHGlobal(buffer_handle, TRUE, &this->pStream);
            if (FAILED(hr))
            {
                error_message = L"Failed to create stream from resource data.";
                break;
            }

            // Create the image object from the stream.
            this->pImage = Gdiplus::Image::FromStream(this->pStream);
            if (!this->pImage)
            {
                error_message = L"Failed to create the image object from the stream (The GDI+ API might not be initialized yet).";
                break;
            }
            auto status = this->pImage->GetLastStatus();
            if (status)
            {
                error_message = L"Failed to create the image object from the stream (GDI+ Error Code: " + std::to_wstring(status) + L").";
                break;
            }

            are_all_operation_success = true;
        }

        // Check if the image object was successfully created.
        if (!are_all_operation_success)
        {
            WriteLog(L"Failed to create the image object.", L" [CLASS: \"MyImage\" | FUNC: \"MyImage()\"]", MyLogType::Error);
        }
    }

    MyImage::totalInstances++;
}
// Assignment operator:
MyImage &MyImage::operator=(const MyImage &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Allocation variable(s).
    this->resourceID = other.resourceID;
    this->format = other.format;

    // Variant(s).
    if (this->pImage)
    {
        delete this->pImage;
        this->pImage = nullptr;
        this->pStream->Release();
        this->pStream = nullptr;
    }
    {
        // Create the image object.
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get resource type string.
            std::wstring resource_type = L"";
            bool is_predefined_resource_type = false;
            auto predefined_resource_type = RT_RCDATA;
            switch (this->format)
            {
            case MyImageFormat::JPG:
                resource_type = L"JPG";
                break;
            case MyImageFormat::PNG:
                resource_type = L"PNG";
                break;
            case MyImageFormat::ICO:
                resource_type = L"ICO";
                predefined_resource_type = RT_GROUP_ICON;
                is_predefined_resource_type = true;
                // Note: For now loading ico file from application resource will result in GDI+ error code 2
                // as the Gdiplus::Image::FromStream() method expect the stream to contains a single icon and not the whole icon group.
                // This can be solve by using BeginResourceExtract() to extract the icon from the icon group.
                // Further work must be done.
                break;
            default:
                break;
            }
            if (resource_type.empty())
            {
                error_message = L"Invalid resource format.";
                break;
            }

            // Find the resource.
            HRSRC resource_handle = FindResourceW(NULL, MAKEINTRESOURCEW(this->resourceID), (is_predefined_resource_type ? predefined_resource_type : resource_type.c_str()));
            if (!resource_handle)
            {
                error_message = L"Resource not found.";
                break;
            }

            // Get the resource size.
            DWORD resource_size = SizeofResource(NULL, resource_handle);
            if (!resource_size)
            {
                error_message = L"Failed to retrieve the resource size.";
                break;
            }

            // Load the resource.
            HGLOBAL global_handle = LoadResource(NULL, resource_handle);
            if (!global_handle)
            {
                error_message = L"Failed to load the resource.";
                break;
            }

            // Lock the resource and get pointer to the resource data.
            const void *p_resource_data = LockResource(global_handle);
            if (!p_resource_data)
            {
                error_message = L"Failed to lock the resource.";
                break;
            }

            // Allocate the global memory object.
            HGLOBAL buffer_handle = GlobalAlloc(GMEM_MOVEABLE, resource_size);
            if (!buffer_handle)
            {
                error_message = L"Failed to allocate the global memory object.";
                break;
            }

            // Lock the global memory object.
            void *p_buffer = GlobalLock(buffer_handle);
            if (!p_buffer)
            {
                error_message = L"Failed to lock the global memory object.";
                break;
            }
            memcpy(p_buffer, p_resource_data, resource_size);

            // Create stream from the resource data.
            HRESULT hr = CreateStreamOnHGlobal(buffer_handle, TRUE, &this->pStream);
            if (FAILED(hr))
            {
                error_message = L"Failed to create stream from resource data.";
                break;
            }

            // Create the image object from the stream.
            this->pImage = Gdiplus::Image::FromStream(this->pStream);
            if (!this->pImage)
            {
                error_message = L"Failed to create the image object from the stream (The GDI+ API might not be initialized yet).";
                break;
            }
            auto status = this->pImage->GetLastStatus();
            if (status)
            {
                error_message = L"Failed to create the image object from the stream (GDI+ Error Code: " + std::to_wstring(status) + L").";
                break;
            }

            are_all_operation_success = true;
        }

        // Check if the image object was successfully created.
        if (!are_all_operation_success)
        {
            WriteLog(L"Failed to create the image object.", L" [CLASS: \"MyImage\" | FUNC: \"&operator=()\"]", MyLogType::Error);
        }
    }

    return *this;
}
// Destructor:
MyImage::~MyImage()
{
    // Deallocation of unmanaged object(s).
    if (this->pImage)
    {
        delete this->pImage;
        this->pImage = nullptr;
        this->pStream->Release();
        this->pStream = nullptr;
    }

    MyImage::totalInstances--;
}
// Public static member function(s) [GENERAL FUNCTIONS]:
UINT MyImage::getTotalInstances()
{
    return MyImage::totalInstances;
}
// Public member function(s) [GENERAL FUNCTIONS]:
INT MyImage::getResourceID() const
{
    return this->resourceID;
}
MyImageFormat MyImage::getFormat() const
{
    return this->format;
}
Gdiplus::Image *MyImage::getGDIPImage() const
{
    return this->pImage;
}
bool MyImage::update(INT resourceID, MyImageFormat format)
{
    bool are_all_operation_success = false;
    std::wstring error_message = L"";
    while (!are_all_operation_success)
    {
        // Allocation variable(s).
        this->resourceID = resourceID;
        this->format = format;

        // Variant(s).
        if (this->pImage)
        {
            delete this->pImage;
            this->pImage = nullptr;
            this->pStream->Release();
            this->pStream = nullptr;
        }
        {
            // Get resource type string.
            std::wstring resource_type = L"";
            bool is_predefined_resource_type = false;
            auto predefined_resource_type = RT_RCDATA;
            switch (this->format)
            {
            case MyImageFormat::JPG:
                resource_type = L"JPG";
                break;
            case MyImageFormat::PNG:
                resource_type = L"PNG";
                break;
            case MyImageFormat::ICO:
                resource_type = L"ICO";
                predefined_resource_type = RT_GROUP_ICON;
                is_predefined_resource_type = true;
                // Note: For now loading ico file from application resource will result in GDI+ error code 2
                // as the Gdiplus::Image::FromStream() method expect the stream to contains a single icon and not the whole icon group.
                // This can be solve by using BeginResourceExtract() to extract the icon from the icon group.
                // Further work must be done.
                break;
            default:
                break;
            }
            if (resource_type.empty())
            {
                error_message = L"Invalid resource format.";
                break;
            }

            // Find the resource.
            HRSRC resource_handle = FindResourceW(NULL, MAKEINTRESOURCEW(this->resourceID), (is_predefined_resource_type ? predefined_resource_type : resource_type.c_str()));
            if (!resource_handle)
            {
                error_message = L"Resource not found.";
                break;
            }

            // Get the resource size.
            DWORD resource_size = SizeofResource(NULL, resource_handle);
            if (!resource_size)
            {
                error_message = L"Failed to retrieve the resource size.";
                break;
            }

            // Load the resource.
            HGLOBAL global_handle = LoadResource(NULL, resource_handle);
            if (!global_handle)
            {
                error_message = L"Failed to load the resource.";
                break;
            }

            // Lock the resource and get pointer to the resource data.
            const void *p_resource_data = LockResource(global_handle);
            if (!p_resource_data)
            {
                error_message = L"Failed to lock the resource.";
                break;
            }

            // Allocate the global memory object.
            HGLOBAL buffer_handle = GlobalAlloc(GMEM_MOVEABLE, resource_size);
            if (!buffer_handle)
            {
                error_message = L"Failed to allocate the global memory object.";
                break;
            }

            // Lock the global memory object.
            void *p_buffer = GlobalLock(buffer_handle);
            if (!p_buffer)
            {
                error_message = L"Failed to lock the global memory object.";
                break;
            }
            memcpy(p_buffer, p_resource_data, resource_size);

            // Create stream from the resource data.
            HRESULT hr = CreateStreamOnHGlobal(buffer_handle, TRUE, &this->pStream);
            if (FAILED(hr))
            {
                error_message = L"Failed to create stream from resource data.";
                break;
            }

            // Create the image object from the stream.
            this->pImage = Gdiplus::Image::FromStream(this->pStream);
            if (!this->pImage)
            {
                error_message = L"Failed to create the image object from the stream (The GDI+ API might not be initialized yet).";
                break;
            }
            auto status = this->pImage->GetLastStatus();
            if (status)
            {
                error_message = L"Failed to create the image object from the stream (GDI+ Error Code: " + std::to_wstring(status) + L").";
                break;
            }
        }

        are_all_operation_success = true;
    }

    if (!are_all_operation_success)
    {
        WriteLog(error_message, L" [CLASS: \"MyImage\" | FUNC: \"update()\"]", MyLogType::Error);
        return false;
    }

    return true;
}

/// @class UIColors definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIColors::updateMainColors(RGBA primary, RGBA secondary, RGBA borderActive, RGBA borderInactive,
                                RGBA textActive, RGBA textInactive, RGBA textHighlight, RGBA focus, RGBA background)
{
    this->primary = MyColor(primary);
    this->secondary = MyColor(secondary);
    this->borderActive = MyColor(borderActive);
    this->borderInactive = MyColor(borderInactive);
    this->textActive = MyColor(textActive);
    this->textInactive = MyColor(textInactive);
    this->textHighlight = MyColor(textHighlight);
    this->focus = MyColor(focus);
    this->background = MyColor(background);
}
void UIColors::updateCaptionColors(RGBA captionBackground, RGBA captionTextActive, RGBA captionTextInactive,
                                   RGBA closeButtonBackgroundOnHover, RGBA closeButtonBackgroundOnDown,
                                   RGBA maximizeButtonBackgroundOnHover, RGBA maximizeButtonBackgroundOnDown,
                                   RGBA minimizeButtonBackgroundOnHover, RGBA minimizeButtonBackgroundOnDown)
{
    this->captionBackground = MyColor(captionBackground);
    this->captionTextActive = MyColor(captionTextActive);
    this->captionTextInactive = MyColor(captionTextInactive);
    this->closeButtonBackgroundOnHover = MyColor(closeButtonBackgroundOnHover);
    this->closeButtonBackgroundOnDown = MyColor(closeButtonBackgroundOnDown);
    this->maximizeButtonBackgroundOnHover = MyColor(maximizeButtonBackgroundOnHover);
    this->maximizeButtonBackgroundOnDown = MyColor(maximizeButtonBackgroundOnDown);
    this->minimizeButtonBackgroundOnHover = MyColor(minimizeButtonBackgroundOnHover);
    this->minimizeButtonBackgroundOnDown = MyColor(minimizeButtonBackgroundOnDown);
}
void UIColors::updateStandardButtonColors(RGBA standardButtonDefault, RGBA standardButtonHover, RGBA standardButtonDown,
                                          RGBA standardButtonBorderDefault, RGBA standardButtonBorderHover, RGBA standardButtonBorderDown)
{
    this->standardButtonDefault = MyColor(standardButtonDefault);
    this->standardButtonHover = MyColor(standardButtonHover);
    this->standardButtonDown = MyColor(standardButtonDown);
    this->standardButtonBorderDefault = MyColor(standardButtonBorderDefault);
    this->standardButtonBorderHover = MyColor(standardButtonBorderHover);
    this->standardButtonBorderDown = MyColor(standardButtonBorderDown);
}
void UIColors::updateRadioButtonColors(RGBA radioButtonPrimaryDefault, RGBA radioButtonPrimaryHover, RGBA radioButtonPrimaryDown,
                                       RGBA radioButtonSecondaryDefault, RGBA radioButtonSecondaryHover, RGBA radioButtonSecondaryDown,
                                       RGBA radioButtonBorderDefault, RGBA radioButtonBorderHover, RGBA radioButtonBorderDown,
                                       RGBA selectedRadioButtonPrimaryDefault, RGBA selectedRadioButtonPrimaryHover, RGBA selectedRadioButtonPrimaryDown,
                                       RGBA selectedRadioButtonSecondaryDefault, RGBA selectedRadioButtonSecondaryHover, RGBA selectedRadioButtonSecondaryDown,
                                       RGBA selectedRadioButtonBorderDefault, RGBA selectedRadioButtonBorderHover, RGBA selectedRadioButtonBorderDown)
{
    this->radioButtonPrimaryDefault = MyColor(radioButtonPrimaryDefault);
    this->radioButtonPrimaryHover = MyColor(radioButtonPrimaryHover);
    this->radioButtonPrimaryDown = MyColor(radioButtonPrimaryDown);
    this->radioButtonSecondaryDefault = MyColor(radioButtonSecondaryDefault);
    this->radioButtonSecondaryHover = MyColor(radioButtonSecondaryHover);
    this->radioButtonSecondaryDown = MyColor(radioButtonSecondaryDown);
    this->radioButtonBorderDefault = MyColor(radioButtonBorderDefault);
    this->radioButtonBorderHover = MyColor(radioButtonBorderHover);
    this->radioButtonBorderDown = MyColor(radioButtonBorderDown);
    this->selectedRadioButtonPrimaryDefault = MyColor(selectedRadioButtonPrimaryDefault);
    this->selectedRadioButtonPrimaryHover = MyColor(selectedRadioButtonPrimaryHover);
    this->selectedRadioButtonPrimaryDown = MyColor(selectedRadioButtonPrimaryDown);
    this->selectedRadioButtonSecondaryDefault = MyColor(selectedRadioButtonSecondaryDefault);
    this->selectedRadioButtonSecondaryHover = MyColor(selectedRadioButtonSecondaryHover);
    this->selectedRadioButtonSecondaryDown = MyColor(selectedRadioButtonSecondaryDown);
    this->selectedRadioButtonBorderDefault = MyColor(selectedRadioButtonBorderDefault);
    this->selectedRadioButtonBorderHover = MyColor(selectedRadioButtonBorderHover);
    this->selectedRadioButtonBorderDown = MyColor(selectedRadioButtonBorderDown);
}
void UIColors::updateEditboxColors(RGBA editbox, RGBA editboxBorderDefault, RGBA editboxBorderSelected)
{
    this->editbox = MyColor(editbox);
    this->editboxBorderDefault = MyColor(editboxBorderDefault);
    this->editboxBorderSelected = MyColor(editboxBorderSelected);
}
void UIColors::updateDDLComboboxColors(RGBA ddlCombobox, RGBA ddlComboboxBorder, RGBA ddlComboboxItemBackground, RGBA ddlComboboxSelectedItemBackground,
                                       RGBA ddlComboboxDropdownlistBorder, RGBA ddlComboboxItemTextDefault, RGBA ddlComboboxItemTextSelected)
{
    this->ddlCombobox = MyColor(ddlCombobox);
    this->ddlComboboxBorder = MyColor(ddlComboboxBorder);
    this->ddlComboboxItemBackground = MyColor(ddlComboboxItemBackground);
    this->ddlComboboxSelectedItemBackground = MyColor(ddlComboboxSelectedItemBackground);
    this->ddlComboboxDropdownlistBorder = MyColor(ddlComboboxDropdownlistBorder);
    this->ddlComboboxItemTextDefault = MyColor(ddlComboboxItemTextDefault);
    this->ddlComboboxItemTextSelected = MyColor(ddlComboboxItemTextSelected);
}

/// @class UIFonts definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIFonts::updateFonts(MyFont caption, MyFont editbox, MyFont ddlCombobox, MyFont heading, MyFont note)
{
    this->caption = caption;
    this->editbox = editbox;
    this->ddlCombobox = ddlCombobox;
    this->heading = heading;
    this->note = note;
}

/// @class UIImages definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIImages::updateNonClientButtonImages(MyImage &nonClientCloseButtonDefault, MyImage &nonClientCloseButtonHover, MyImage &nonClientCloseButtonDown,
                                           MyImage &nonClientMinimizeButtonDefault, MyImage &nonClientMinimizeButtonHover, MyImage &nonClientMinimizeButtonDown)
{
    this->pNonClientCloseButtonDefault = std::make_unique<MyImage>(nonClientCloseButtonDefault);
    this->pNonClientCloseButtonHover = std::make_unique<MyImage>(nonClientCloseButtonHover);
    this->pNonClientCloseButtonDown = std::make_unique<MyImage>(nonClientCloseButtonDown);
    this->pNonClientMinimizeButtonDefault = std::make_unique<MyImage>(nonClientMinimizeButtonDefault);
    this->pNonClientMinimizeButtonHover = std::make_unique<MyImage>(nonClientMinimizeButtonHover);
    this->pNonClientMinimizeButtonDown = std::make_unique<MyImage>(nonClientMinimizeButtonDown);
}

/// @class UIRectangles definitions:
// Constructor:
UIRectangles::UIRectangles()
{
    // Caption bar area rectangle.
    this->rectCaption =
        {
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Left.
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Top.
            g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH,           // Right.
            WINDOW_BORDER_DEFAULTWIDTH + WINDOW_CAPTIONBAR_DEFAULTHEIGHT // Bottom.
        };

    // Top border area rectangle.
    this->rectSizeBorderTop =
        {
            0,                         // Left
            0,                         // Top
            g_CurrentWindowWidth,      // Right
            WINDOW_BORDER_DEFAULTWIDTH // Bottom
        };

    // Bottom border area rectangle.
    this->rectSizeBorderBottom =
        {
            0,                                                  // Left
            g_CurrentWindowHeight - WINDOW_BORDER_DEFAULTWIDTH, // Top
            g_CurrentWindowWidth,                               // Right
            g_CurrentWindowHeight                               // Bottom
        };

    // Left border area rectangle.
    this->rectSizeBorderLeft =
        {
            0,                          // Left
            1,                          // Top
            WINDOW_BORDER_DEFAULTWIDTH, // Right
            g_CurrentWindowHeight - 1   // Bottom
        };

    // Right border area rectangle.
    this->rectSizeBorderRight =
        {
            g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH, // Left
            1,                                                 // Top
            g_CurrentWindowWidth,                              // Right
            g_CurrentWindowHeight - 1                          // Bottom
        };
}

/// @class UIElements definitions:
// Public static member function(s) [GENERAL FUNCTIONS]:
void UIElements::showTotalInstances()
{
    UINT total_color_instances = MyColor::getTotalInstances();
    UINT total_font_instances = MyFont::getTotalInstances();
    UINT total_icon_instances = MyIcon::getTotalInstances();
    UINT total_image_instances = MyImage::getTotalInstances();

    std::wstring message = L"";
    message.append(L"Total instances: " + std::to_wstring(total_color_instances + total_font_instances + total_icon_instances + total_image_instances) + L"\n");
    message.append(L"- Colors: " + std::to_wstring(total_color_instances) + L"\n");
    message.append(L"- Fonts: " + std::to_wstring(total_font_instances) + L"\n");
    message.append(L"- Icons: " + std::to_wstring(total_icon_instances) + L"\n");
    message.append(L"- Images: " + std::to_wstring(total_image_instances) + L"\n");
    MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONINFORMATION);
}