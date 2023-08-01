/**
 * @file object_management.cpp
 * @brief Implementation of the classes defined in object_management.h.
 */

#include "../../Includes/standard_includes.h" // Standard libraries, project resources and global constant includes.
#include "../Global/global.h"                 // The distribution header of the global variables, forward declarations and my class declarations.

/// @class MyColor definitions:
// Constructor:
MyColor::MyColor()
{
    // Increment the total number of instances.
    MyColor::TotalInstances++;
}
MyColor::MyColor(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, INT ID)
{
    // Set instance ID.
    this->SetInstanceID(ID);

    // Initialize the instance.
    if (!this->Update(Red, Green, Blue, Alpha))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: Constructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyColor::TotalInstances++;
}
// Copy Constructor:
MyColor::MyColor(const MyColor &other)
{
    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.Red, other.Green, other.Blue, other.Alpha))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: Copy Constructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyColor::TotalInstances++;
}
// Assignment Operator:
MyColor &MyColor::operator=(const MyColor &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Check if the other instance is uninitialized.
    if (!other.isInitialized)
    {
        // Copy the values from the other instance.
        this->Red = other.Red;
        this->Green = other.Green;
        this->Blue = other.Blue;
        this->Alpha = other.Alpha;
        this->CLR = other.CLR;
        this->GDIP_CL = other.GDIP_CL;
        this->ID = other.ID;

        // If this instance is initialized, destroy unmanaged object(s).
        if (this->isInitialized)
        {
            if (!this->DestroyUnmanagedObjects())
            {
                std::wstring error_message = L"";
                error_message.append(L"Error occurred!\n");
                error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
                error_message.append(L"STRUCT: MyColor\n");
                error_message.append(L"FUNC: Assignment Operator");
                MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }

        return *this;
    }

    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyColor\n");
            error_message.append(L"FUNC: Assignment Operator");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.Red, other.Green, other.Blue, other.Alpha))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: Assignment Operator");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    return *this;
}
// Destructor:
MyColor::~MyColor()
{
    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyColor\n");
            error_message.append(L"FUNC: Destructor");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Decrement the total number of instances.
    MyColor::TotalInstances--;
}
// Private member function(s):
bool MyColor::DestroyUnmanagedObjects()
{
    // Destroy unmanaged object(s).
    if (!DeleteObject(this->HBR))
        return false;

    // Set object pointer(s) to null after the deallocation(s).
    this->HBR = nullptr;

    // Decrement the total number of initialized instances.
    MyColor::TotalInitializedInstances--;

    // Update initialization state.
    this->isInitialized = false;

    return true;
}
// Public static member function(s):
UINT MyColor::GetTotalInstances()
{
    return MyColor::TotalInstances;
}
UINT MyColor::GetTotalInitializedInstances()
{
    return MyColor::TotalInitializedInstances;
}
// Public member function(s):
INT MyColor::GetInstanceID()
{
    return this->ID;
}
void MyColor::SetInstanceID(INT ID)
{
    this->ID = ID;
}
BYTE MyColor::GetRed()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: GetRed()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Red;
}
BYTE MyColor::GetGreen()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: GetGreen()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Green;
}
BYTE MyColor::GetBlue()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: GetBlue()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Blue;
}
BYTE MyColor::GetAlpha()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: GetAlpha()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Alpha;
}
COLORREF MyColor::GetCLR()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: GetCLR()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->CLR;
}
Gdiplus::Color MyColor::GetGDIPColor()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: GetGDIPColor()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->GDIP_CL;
}
Gdiplus::Color &MyColor::GetGDIPColorRef()
{
    return this->GDIP_CL;
}
HBRUSH MyColor::GetHBR()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyColor\n");
        error_message.append(L"FUNC: GetHBR()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->HBR;
}
HBRUSH &MyColor::GetHBRRef()
{
    return this->HBR;
}
bool MyColor::Update(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)
{
    // Check if the instance is initialized.
    if (this->isInitialized)
    {
        // Destroys the unmanaged object(s).
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyColor\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

            return false;
        }
    }

    // Update the new color values.
    this->Red = Red;
    this->Green = Green;
    this->Blue = Blue;
    this->Alpha = Alpha;

    // Create new COLORREF variant of the color.
    this->CLR = RGB(Red, Green, Blue);

    // Create Gdiplus::Color variant of the color.
    this->GDIP_CL = Gdiplus::Color(Alpha, Red, Green, Blue);

    // Create new HBRUSH variant of the color.
    {
        // The brush handle should be null before creation.
        if (this->HBR)
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"The brush handle should be null, but it is not.\n\n");
            error_message.append(L"STRUCT: MyColor\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }

        // Create the brush object.
        this->HBR = CreateSolidBrush(this->CLR);

        // Check if the brush object is successfully created.
        if (!this->HBR)
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to create the brush object.\n\n");
            error_message.append(L"STRUCT: MyColor\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Increment the total number of initialized instances.
    MyColor::TotalInitializedInstances++;

    // Update the initialization state.
    this->isInitialized = true;

    return true;
}

/// @class MyFont definitions:
// Constructor:
MyFont::MyFont()
{
    // Increment the total number of instances.
    MyFont::TotalInstances++;
}
MyFont::MyFont(std::wstring Name, USHORT Size, USHORT Weight, DWORD Quality, INT ID)
{
    // Set instance ID.
    this->SetInstanceID(ID);

    // Initialize the instance.
    if (!this->Update(Name, Size, Weight, Quality))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: Constructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyFont::TotalInstances++;
}
// Copy Constructor:
MyFont::MyFont(const MyFont &other)
{
    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.Name, other.Size, other.Weight, other.Quality))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: Copy Constructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyFont::TotalInstances++;
}
// Assignment Operator:
MyFont &MyFont::operator=(const MyFont &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Check if the other instance is uninitialized.
    if (!other.isInitialized)
    {
        // Copy the values from the other instance.
        this->Name = other.Name;
        this->Size = other.Size;
        this->Weight = other.Weight;
        this->Quality = other.Quality;
        this->ID = other.ID;

        // If this instance is initialized, destroy unmanaged object(s).
        if (this->isInitialized)
        {
            if (!this->DestroyUnmanagedObjects())
            {
                std::wstring error_message = L"";
                error_message.append(L"Error occurred!\n");
                error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
                error_message.append(L"STRUCT: MyFont\n");
                error_message.append(L"FUNC: Assignment Operator");
                MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }

        return *this;
    }

    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyFont\n");
            error_message.append(L"FUNC: Assignment Operator");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.Name, other.Size, other.Weight, other.Quality))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: Assignment Operator");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    return *this;
}
// Destructor:
MyFont::~MyFont()
{
    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyFont\n");
            error_message.append(L"FUNC: Destructor");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Decrement the total number of instances.
    MyFont::TotalInstances--;
}
// Private member function(s):
bool MyFont::DestroyUnmanagedObjects()
{
    // Destroy unmanaged object(s).
    if (!DeleteObject(this->HFO))
        return false;

    // Set object pointer(s) to null after the deallocation(s).
    this->HFO = nullptr;

    // Decrement the total number of initialized instances.
    MyFont::TotalInitializedInstances--;

    // Update initialization state.
    this->isInitialized = false;

    return true;
}
// Public static member function(s):
UINT MyFont::GetTotalInstances()
{
    return MyFont::TotalInstances;
}
UINT MyFont::GetTotalInitializedInstances()
{
    return MyFont::TotalInitializedInstances;
}
// Public member function(s):
INT MyFont::GetInstanceID()
{
    return this->ID;
}
void MyFont::SetInstanceID(INT ID)
{
    this->ID = ID;
}
std::wstring MyFont::GetName()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: GetName()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Name;
}
USHORT MyFont::GetSize()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: GetSize()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Size;
}
USHORT MyFont::GetWeight()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: GetWeight()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Weight;
}
DWORD MyFont::GetQuality()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: GetQuality()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Quality;
}
HFONT MyFont::GetHFO()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyFont\n");
        error_message.append(L"FUNC: GetHFO()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->HFO;
}
HFONT &MyFont::GetHFORef()
{
    return this->HFO;
}
bool MyFont::Update(std::wstring Name, USHORT Size, USHORT Weight, DWORD Quality)
{
    // Check if the instance is initialized.
    if (this->isInitialized)
    {
        // Destroys the unmanaged object(s).
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyFont\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

            return false;
        }
    }

    // Update the new font values.
    this->Name = Name;
    this->Size = Size;
    this->Weight = Weight;
    this->Quality = Quality;

    // Create new HFONT variant of the color.
    {
        // The font handle should be null before creation.
        if (this->HFO)
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"The font handle should be null, but it is not.\n\n");
            error_message.append(L"STRUCT: MyFont\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }

        // Create the font object.
        this->HFO = CreateFontW(static_cast<INT>(this->Size), 0, 0, 0x1, static_cast<INT>(this->Weight),
                                FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, this->Quality,
                                DEFAULT_PITCH | FF_DONTCARE, static_cast<LPCWSTR>(this->Name.c_str()));

        // Check if the font object is successfully created.
        if (!this->HFO)
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to create the font object.\n\n");
            error_message.append(L"STRUCT: MyFont\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Increment the total number of initialized instances.
    MyFont::TotalInitializedInstances++;

    // Update the initialization state.
    this->isInitialized = true;

    return true;
}

/// @class MyIcon definitions:
// Constructor:
MyIcon::MyIcon()
{
    // Increment the total number of instances.
    MyIcon::TotalInstances++;
}
MyIcon::MyIcon(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad, INT ID)
{
    // Set instance ID.
    this->SetInstanceID(ID);

    // Initialize the instance.
    if (!this->Update(hInstance, Width, Height, ResourceID, HICON_fuLoad))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: Constructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyIcon::TotalInstances++;
}
// Copy Constructor:
MyIcon::MyIcon(const MyIcon &other)
{
    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.hInstance, other.Width, other.Height, other.ResourceID, other.HICON_fuLoad))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: Copy Constructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyIcon::TotalInstances++;
}
// Assignment Operator:
MyIcon &MyIcon::operator=(const MyIcon &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Check if the other instance is uninitialized.
    if (!other.isInitialized)
    {
        // Copy the values from the other instance.
        this->hInstance = other.hInstance;
        this->Width = other.Width;
        this->Height = other.Height;
        this->ResourceID = other.ResourceID;
        this->HICON_fuLoad = other.HICON_fuLoad;
        this->ID = other.ID;

        // If this instance is initialized, destroy unmanaged object(s).
        if (this->isInitialized)
        {
            if (!this->DestroyUnmanagedObjects())
            {
                std::wstring error_message = L"";
                error_message.append(L"Error occurred!\n");
                error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
                error_message.append(L"STRUCT: MyIcon\n");
                error_message.append(L"FUNC: Assignment Operator");
                MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }

        return *this;
    }

    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyIcon\n");
            error_message.append(L"FUNC: Assignment Operator");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.hInstance, other.Width, other.Height, other.ResourceID, other.HICON_fuLoad))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the instance.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: Assignment Operator");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    return *this;
}
// Destructor:
MyIcon::~MyIcon()
{
    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyIcon\n");
            error_message.append(L"FUNC: Destructor");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Decrement the total number of instances.
    MyIcon::TotalInstances--;
}
// Private member function(s):
bool MyIcon::DestroyUnmanagedObjects()
{
    // Destroy unmanaged object(s).
    if (!DestroyIcon(this->HICO))
        return false;

    // Set object pointer(s) to null after the deallocation(s).
    this->HICO = nullptr;

    // Decrement the total number of initialized instances.
    MyIcon::TotalInitializedInstances--;

    // Update initialization state.
    this->isInitialized = false;

    return true;
}
// Public static member function(s):
UINT MyIcon::GetTotalInstances()
{
    return MyIcon::TotalInstances;
}
UINT MyIcon::GetTotalInitializedInstances()
{
    return MyIcon::TotalInitializedInstances;
}
// Public member function(s):
INT MyIcon::GetInstanceID()
{
    return this->ID;
}
void MyIcon::SetInstanceID(INT ID)
{
    this->ID = ID;
}
HINSTANCE MyIcon::GetHINSTANCE()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: GetHINSTANCE()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->hInstance;
}
INT MyIcon::GetWidth()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: GetWidth()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Width;
}
INT MyIcon::GetHeight()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: GetHeight()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->Height;
}
INT MyIcon::GetResourceID()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: GetResourceID()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->ResourceID;
}
UINT MyIcon::GetLoadImageFlag()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: GetLoadImageFlag()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->HICON_fuLoad;
}
HICON MyIcon::GetHICO()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"The object is not initialized yet.\n\n");
        error_message.append(L"STRUCT: MyIcon\n");
        error_message.append(L"FUNC: GetHICO()");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->HICO;
}
HICON &MyIcon::GetHICORef()
{
    return this->HICO;
}
bool MyIcon::Update(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad)
{
    // Check if the instance is initialized.
    if (this->isInitialized)
    {
        // Destroys the unmanaged object(s).
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            error_message.append(L"STRUCT: MyIcon\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);

            return false;
        }
    }

    // Update the new icon values.
    this->hInstance = hInstance;
    this->Width = Width;
    this->Height = Height;
    this->ResourceID = ResourceID;
    this->HICON_fuLoad = HICON_fuLoad;

    // Create new HFONT variant of the icon.
    {
        // The icon handle should be null before creation.
        if (this->HICO)
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"The icon handle should be null, but it is not.\n\n");
            error_message.append(L"STRUCT: MyIcon\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }

        // Create the icon object.
        this->HICO = reinterpret_cast<HICON>(LoadImageW(this->hInstance, MAKEINTRESOURCEW(this->ResourceID), IMAGE_ICON, this->Width, this->Height, this->HICON_fuLoad));

        // Check if the icon object is successfully created.
        if (!this->HICO)
        {
            std::wstring error_message = L"";
            error_message.append(L"Error occurred!\n");
            error_message.append(L"Failed to create the icon object.\n\n");
            error_message.append(L"STRUCT: MyIcon\n");
            error_message.append(L"FUNC: Update()");
            MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Increment the total number of initialized instances.
    MyIcon::TotalInitializedInstances++;

    // Update the initialization state.
    this->isInitialized = true;

    return true;
}

/// @class MyImage definitions:
// Constructor:
MyImage::MyImage()
{
    // Increment the total number of instances.
    MyImage::TotalInstances++;
}
MyImage::MyImage(INT ResourceID, ImageFormat Format, INT ID)
{
    // Set instance ID.
    this->SetInstanceID(ID);

    // Initialize the instance.
    if (!this->Update(ResourceID, Format))
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to initialize the instance.\n\n");
        message.append(L"STRUCT: MyImage\n");
        message.append(L"FUNC: Constructor");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyImage::TotalInstances++;
}
// Copy Constructor:
MyImage::MyImage(const MyImage &other)
{
    // Check if the other instance is uninitialized.
    if (!other.isInitialized)
    {
        // Copy the values from the other instance.
        this->ResourceID = other.ResourceID;
        this->Format = other.Format;
        this->ID = other.ID;

        // Increment the total number of instances.
        MyImage::TotalInstances++;

        return;
    }

    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.ResourceID, other.Format))
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to initialize the instance.\n\n");
        message.append(L"STRUCT: MyImage\n");
        message.append(L"FUNC: Copy Constructor");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // Increment the total number of instances.
    MyImage::TotalInstances++;
}
// Assignment Operator:
MyImage &MyImage::operator=(const MyImage &other)
{
    // Check if self assignment.
    if (this == &other)
        return *this;

    // Check if the other instance is uninitialized.
    if (!other.isInitialized)
    {
        // Copy the values from the other instance.
        this->ResourceID = other.ResourceID;
        this->Format = other.Format;
        this->ID = other.ID;

        // If this instance is initialized, destroy unmanaged object(s).
        if (this->isInitialized)
        {
            if (!this->DestroyUnmanagedObjects())
            {
                std::wstring message = L"";
                message.append(L"Error occurred!\n");
                message.append(L"Failed to destroy the unmanaged object(s).\n\n");
                message.append(L"STRUCT: MyImage\n");
                message.append(L"FUNC: Assignment Operator");
                MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }

        return *this;
    }

    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring message = L"";
            message.append(L"Error occurred!\n");
            message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            message.append(L"STRUCT: MyImage\n");
            message.append(L"FUNC: Assignment Operator");
            MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Set instance ID.
    this->SetInstanceID(other.ID);

    // Initialize the instance.
    if (!this->Update(other.ResourceID, other.Format))
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"Failed to initialize the instance.\n\n");
        message.append(L"STRUCT: MyImage\n");
        message.append(L"FUNC: Assignment Operator");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    return *this;
}
// Destructor:
MyImage::~MyImage()
{
    // If this instance is initialized, destroy unmanaged object(s).
    if (this->isInitialized)
    {
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring message = L"";
            message.append(L"Error occurred!\n");
            message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            message.append(L"STRUCT: MyImage\n");
            message.append(L"FUNC: Destructor");
            MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
    }

    // Decrement the total number of instances.
    MyImage::TotalInstances--;
}
// Private member function(s):
bool MyImage::DestroyUnmanagedObjects()
{
    // Delete the image object.
    delete this->pImage;
    this->pImage = nullptr;

    // Release the resource stream.
    this->pStream->Release();
    this->pStream = nullptr;

    // Decrement the total number of initialized instances.
    MyImage::TotalInitializedInstances--;

    // Update initialization state.
    this->isInitialized = false;

    return true;
}
// Public static member function(s):
UINT MyImage::GetTotalInstances()
{
    return MyImage::TotalInstances;
}
UINT MyImage::GetTotalInitializedInstances()
{
    return MyImage::TotalInitializedInstances;
}
// Public member function(s):
INT MyImage::GetInstanceID()
{
    return this->ID;
}
void MyImage::SetInstanceID(INT ID)
{
    this->ID = ID;
}
INT MyImage::GetResourceID()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"The object is not initialized yet.\n\n");
        message.append(L"STRUCT: MyImage\n");
        message.append(L"FUNC: GetResourceID()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return NULL;
    }

    return this->ResourceID;
}
ImageFormat MyImage::GetFormat()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"The object is not initialized yet.\n\n");
        message.append(L"STRUCT: MyImage\n");
        message.append(L"FUNC: GetFormat()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return ImageFormat::Unknown;
    }

    return this->Format;
}
Gdiplus::Image *MyImage::GetImage()
{
    // Check if the instance is initialized.
    if (!this->isInitialized)
    {
        std::wstring message = L"";
        message.append(L"Error occurred!\n");
        message.append(L"The object is not initialized yet.\n\n");
        message.append(L"STRUCT: MyImage\n");
        message.append(L"FUNC: GetImage()");
        MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

        return nullptr;
    }

    return this->pImage;
}
Gdiplus::Image *&MyImage::GetImageRef()
{
    return this->pImage;
}
bool MyImage::Update(INT ResourceID, ImageFormat Format)
{
    // Check if the instance is initialized.
    if (this->isInitialized)
    {
        // Destroys the unmanaged object(s).
        if (!this->DestroyUnmanagedObjects())
        {
            std::wstring message = L"";
            message.append(L"Error occurred!\n");
            message.append(L"Failed to destroy the unmanaged object(s).\n\n");
            message.append(L"STRUCT: MyImage\n");
            message.append(L"FUNC: Update()");
            MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

            return false;
        }
    }

    // Update the new image values.
    this->ResourceID = ResourceID;
    this->Format = Format;

    // Create new Gdiplus::Image variant of the icon.
    {
        // The image pointer and resource stream pointer should be null before creation.
        if (this->pImage || this->pStream)
        {
            std::wstring message = L"";
            message.append(L"Error occurred!\n");
            message.append(L"The image pointer and resource stream pointer should be null, but they are not.\n\n");
            message.append(L"STRUCT: MyImage\n");
            message.append(L"FUNC: Update()");
            MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);

            return false;
        }

        // Create the image object.
        bool are_all_operation_success = false;
        std::wstring error_message = L"";
        while (!are_all_operation_success)
        {
            // Get resource type string.
            std::wstring ResourceType = L"";
            bool isPredefinedResourceType = false;
            auto PredefinedResourceType = RT_RCDATA;
            switch (this->Format)
            {
            case ImageFormat::JPG:
                ResourceType = L"JPG";
                break;
            case ImageFormat::PNG:
                ResourceType = L"PNG";
                break;
            case ImageFormat::ICO:
                ResourceType = L"ICO";
                PredefinedResourceType = RT_GROUP_ICON; 
                isPredefinedResourceType = true;
                // Note: For now loading ico file from application resource will result in GDI+ error code 2
                // as the Gdiplus::Image::FromStream() method expect the stream to contains a single icon and not the whole icon group.
                // This can be solve by using BeginResourceExtract() to extract the icon from the icon group.
                // Further work must be done.
                break;
            default:
                break;
            }
            if (ResourceType.empty())
            {
                error_message = L"Invalid resource format.";
                break;
            }

            // Find the resource.
            HRSRC hResource = FindResourceW(NULL, MAKEINTRESOURCEW(this->ResourceID), (isPredefinedResourceType ? PredefinedResourceType : ResourceType.c_str()));
            if (!hResource)
            {
                error_message = L"Resource not found.";
                break;
            }

            // Get the resource size.
            DWORD ResourceSize = SizeofResource(NULL, hResource);
            if (!ResourceSize)
            {
                error_message = L"Failed to retrieve the resource size.";
                break;
            }

            // Load the resource.
            HGLOBAL hGlobal = LoadResource(NULL, hResource);
            if (!hGlobal)
            {
                error_message = L"Failed to load the resource.";
                break;
            }

            // Lock the resource and get pointer to the resource data.
            const void *pResourceData = LockResource(hGlobal);
            if (!pResourceData)
            {
                error_message = L"Failed to lock the resource.";
                break;
            }

            // Allocate the global memory object.
            HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, ResourceSize);
            if (!hBuffer)
            {
                error_message = L"Failed to allocate the global memory object.";
                break;
            }

            // Lock the global memory object.
            void *pBuffer = GlobalLock(hBuffer);
            if (!pBuffer)
            {
                error_message = L"Failed to lock the global memory object.";
                break;
            }
            memcpy(pBuffer, pResourceData, ResourceSize);

            // Create stream from the resource data.
            HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &this->pStream);
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
            auto Status = this->pImage->GetLastStatus();
            if (Status)
            {
                error_message = L"Failed to create the image object from the stream (GDI+ Error Code: " + std::to_wstring(Status) + L").";
                break;
            }

            are_all_operation_success = true;
        }

        // Check if the image object was successfully created.
        if (!are_all_operation_success)
        {
            std::wstring message = L"";
            message.append(L"Error occurred!\n");
            message.append(L"Failed to create the image object.\n");
            message.append(L"Error Message: " + error_message + L"\n\n");
            message.append(L"STRUCT: MyImage\n");
            message.append(L"FUNC: Update()");
            MessageBoxW(NULL, message.c_str(), L"", MB_OK | MB_ICONERROR);
            return false;
        }
    }

    // Increment the total number of initialized instances.
    MyImage::TotalInitializedInstances++;

    // Update the initialization state.
    this->isInitialized = true;

    return true;
}

/// @class UIObjectManager_Colors definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIObjectManager_Colors::UpdateMainColors(MyColor Primary, MyColor Secondary, MyColor Border, MyColor Border_Inactive,
                          MyColor Text, MyColor Text_Inactive, MyColor Focus, MyColor CloseHover, MyColor MinimizeHover,
                          MyColor NonClient_CloseButton_Background_OnHover, MyColor NonClient_CloseButton_Background_OnDown,
                          MyColor NonClient_MinimizeButton_Background_OnHover, MyColor NonClient_MinimizeButton_Background_OnDown, MyColor Background)
{
    this->Primary = Primary;
    this->Secondary = Secondary;
    this->Border = Border;
    this->Border_Inactive = Border_Inactive;
    this->Text = Text;
    this->Text_Inactive = Text_Inactive;
    this->Focus = Focus;
    this->CloseHover = CloseHover;
    this->MinimizeHover = MinimizeHover;
    this->NonClient_CloseButton_Background_OnHover = NonClient_CloseButton_Background_OnHover;
    this->NonClient_CloseButton_Background_OnDown = NonClient_CloseButton_Background_OnDown;
    this->NonClient_MinimizeButton_Background_OnHover = NonClient_MinimizeButton_Background_OnHover;
    this->NonClient_MinimizeButton_Background_OnDown = NonClient_MinimizeButton_Background_OnDown;
    this->Background = Background;
}
void UIObjectManager_Colors::UpdateCaptionColors(MyColor Caption_Background, MyColor Caption_Text, MyColor Caption_Text_Inactive)
{
    this->Caption_Background = Caption_Background;
    this->Caption_Text = Caption_Text;
    this->Caption_Text_Inactive = Caption_Text_Inactive;
}
void UIObjectManager_Colors::UpdateStandardButtonColors(MyColor StandardButton, MyColor StandardButton_OnHover, MyColor StandardButton_OnDown, MyColor StandardButton_Border, MyColor StandardButton_Border_OnHover, MyColor StandardButton_Border_OnDown, MyColor StandardButton_Text_Default, MyColor StandardButton_Text_Highlight)
{
    this->StandardButton = StandardButton;
    this->StandardButton_OnHover = StandardButton_OnHover;
    this->StandardButton_OnDown = StandardButton_OnDown;
    this->StandardButton_Border = StandardButton_Border;
    this->StandardButton_Border_OnHover = StandardButton_Border_OnHover;
    this->StandardButton_Border_OnDown = StandardButton_Border_OnDown;
    this->StandardButton_Text_Default = StandardButton_Text_Default;
    this->StandardButton_Text_Highlight = StandardButton_Text_Highlight;
}
void UIObjectManager_Colors::UpdateRadioButtonColors(MyColor RadioPrimary, MyColor RadioPrimary_OnHover, MyColor RadioPrimary_OnDown, MyColor RadioSecondary, MyColor RadioSecondary_OnHover, MyColor RadioSecondary_OnDown, MyColor Radio_Border, MyColor Radio_Border_OnHover, MyColor Radio_Border_OnDown,
                                                     MyColor SelectedRadioPrimary, MyColor SelectedRadioPrimary_OnHover, MyColor SelectedRadioPrimary_OnDown, MyColor SelectedRadioSecondary, MyColor SelectedRadioSecondary_OnHover, MyColor SelectedRadioSecondary_OnDown, MyColor SelectedRadio_Border, MyColor SelectedRadio_Border_OnHover, MyColor SelectedRadio_Border_OnDown,
                                                     MyColor Radio_Text_Default, MyColor Radio_Text_Highlight)
{
    this->RadioPrimary = RadioPrimary;
    this->RadioPrimary_OnHover = RadioPrimary_OnHover;
    this->RadioPrimary_OnDown = RadioPrimary_OnDown;
    this->RadioSecondary = RadioSecondary;
    this->RadioSecondary_OnHover = RadioSecondary_OnHover;
    this->RadioSecondary_OnDown = RadioSecondary_OnDown;
    this->Radio_Border = Radio_Border;
    this->Radio_Border_OnHover = Radio_Border_OnHover;
    this->Radio_Border_OnDown = Radio_Border_OnDown;
    this->SelectedRadioPrimary = SelectedRadioPrimary;
    this->SelectedRadioPrimary_OnHover = SelectedRadioPrimary_OnHover;
    this->SelectedRadioPrimary_OnDown = SelectedRadioPrimary_OnDown;
    this->SelectedRadioSecondary = SelectedRadioSecondary;
    this->SelectedRadioSecondary_OnHover = SelectedRadioSecondary_OnHover;
    this->SelectedRadioSecondary_OnDown = SelectedRadioSecondary_OnDown;
    this->SelectedRadio_Border = SelectedRadio_Border;
    this->SelectedRadio_Border_OnHover = SelectedRadio_Border_OnHover;
    this->SelectedRadio_Border_OnDown = SelectedRadio_Border_OnDown;
    this->Radio_Text_Default = Radio_Text_Default;
    this->Radio_Text_Highlight = Radio_Text_Highlight;
}
void UIObjectManager_Colors::UpdateEditColors(MyColor Edit, MyColor Edit_Border, MyColor Edit_Border_Selected)
{
    this->Edit = Edit;
    this->Edit_Border = Edit_Border;
    this->Edit_Border_Selected = Edit_Border_Selected;
}
void UIObjectManager_Colors::UpdateComboboxColors(MyColor Combobox, MyColor Combobox_Border, MyColor Combobox_Dropdownlist_Background, MyColor Combobox_Dropdownlist_Background_Selected, MyColor Combobox_Dropdownlist_Border, MyColor Combobox_Dropdownlist_Text_Default, MyColor Combobox_Dropdownlist_Text_Selected)
{
    this->Combobox = Combobox;
    this->Combobox_Border = Combobox_Border;
    this->Combobox_Dropdownlist_Background = Combobox_Dropdownlist_Background;
    this->Combobox_Dropdownlist_Background_Selected = Combobox_Dropdownlist_Background_Selected;
    this->Combobox_Dropdownlist_Border = Combobox_Dropdownlist_Border;
    this->Combobox_Dropdownlist_Text_Default = Combobox_Dropdownlist_Text_Default;
    this->Combobox_Dropdownlist_Text_Selected = Combobox_Dropdownlist_Text_Selected;
}
// Public member function(s) [CONTAINER FUNCTIONS]:
void UIObjectManager_Colors::AddColor(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha, INT ID)
{
    // Check if the ID is unique.
    if (ID != 0)
    {
        for (auto &color : this->Container)
        {
            if (color.GetInstanceID() == ID)
            {
                std::wstring error_message = L"";
                error_message.append(L"Error occurred!\n");
                error_message.append(L"Another instance already uses this ID.\n");
                error_message.append(L"CLASS: UIObjectManager_Colors\n");
                error_message.append(L"FUNC: AddColor()");
                MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Add the object to the container.
    this->Container.push_back(MyColor(Red, Green, Blue, Alpha, ID));
}
bool UIObjectManager_Colors::RemoveColor(INT ID)
{
    // Iterate through the container and remove the instance with the specified ID.
    for (auto it = this->Container.begin(); it != this->Container.end();)
    {
        if ((*it).GetInstanceID() == ID)
        {
            it = this->Container.erase(it);
            return true;
        }
        else
            it++;
    }

    return false;
}
bool UIObjectManager_Colors::RemoveColorEx(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)
{
    bool isAtLeastOneObjectRemoved = false;

    // Iterate through the container and remove the instance(s) with the specified values.
    for (auto it = this->Container.begin(); it != this->Container.end();)
    {
        if ((*it).GetRed() == Red &&
            (*it).GetGreen() == Green &&
            (*it).GetBlue() == Blue &&
            (*it).GetAlpha() == Alpha)
        {
            it = this->Container.erase(it);
            isAtLeastOneObjectRemoved = true;
        }
        else
            it++;
    }

    if (!isAtLeastOneObjectRemoved)
        return false;

    return true;
}
MyColor *UIObjectManager_Colors::GetColor(INT ID)
{
    // Iterate through the container and find the instance with the specified ID.
    for (auto &color : this->Container)
    {
        if (color.GetInstanceID() == ID)
            return &color;
    }

    return nullptr;
}
MyColor *UIObjectManager_Colors::GetColorEx(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)
{
    // Iterate through the container and find the instance with the specified values.
    for (auto &color : this->Container)
    {
        if (color.GetRed() == Red &&
            color.GetGreen() == Green &&
            color.GetBlue() == Blue &&
            color.GetAlpha() == Alpha)
            return &color;
    }

    return nullptr;
}
std::vector<MyColor> &UIObjectManager_Colors::GetContainer()
{
    return this->Container;
}

/// @class UIObjectManager_Fonts definitions:
// Public member function(s) [GENERAL FUNCTIONS]:
void UIObjectManager_Fonts::UpdateMainFonts(MyFont Default, MyFont Caption, MyFont Edit)
{
    this->Default = Default;
    this->Caption = Caption;
    this->Edit = Edit;
}
void UIObjectManager_Fonts::UpdateContainerFonts(MyFont Heading, MyFont Note)
{
    this->Heading = Heading;
    this->Note = Note;
}
// Public member function(s) [CONTAINER FUNCTIONS]:
void UIObjectManager_Fonts::AddFont(std::wstring Name, USHORT Size, USHORT Weight, DWORD Quality, INT ID)
{
    // Check if the ID is unique.
    if (ID != 0)
    {
        for (auto &font : this->Container)
        {
            if (font.GetInstanceID() == ID)
            {
                std::wstring error_message = L"";
                error_message.append(L"Error occurred!\n");
                error_message.append(L"Another instance already uses this ID.\n");
                error_message.append(L"CLASS: UIObjectManager_Fonts\n");
                error_message.append(L"FUNC: AddFont()");
                MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Add the object to the container.
    this->Container.push_back(MyFont(Name, Size, Weight, Quality, ID));
}
bool UIObjectManager_Fonts::RemoveFont(INT ID)
{
    // Iterate through the container and remove the instance with the specified ID.
    for (auto it = this->Container.begin(); it != this->Container.end();)
    {
        if ((*it).GetInstanceID() == ID)
        {
            it = this->Container.erase(it);
            return true;
        }
        else
            it++;
    }

    return false;
}
bool UIObjectManager_Fonts::RemoveFontEx(std::wstring Name, USHORT Size, USHORT Weight, DWORD Quality)
{
    bool isAtLeastOneObjectRemoved = false;

    // Iterate through the container and remove the instance(s) with the specified values.
    for (auto it = this->Container.begin(); it != this->Container.end();)
    {
        if ((*it).GetName() == Name &&
            (*it).GetSize() == Size &&
            (*it).GetWeight() == Weight &&
            (*it).GetQuality() == Quality)
        {
            it = this->Container.erase(it);
            isAtLeastOneObjectRemoved = true;
        }
        else
            it++;
    }

    if (!isAtLeastOneObjectRemoved)
        return false;

    return true;
}
MyFont *UIObjectManager_Fonts::GetFont(INT ID)
{
    for (auto &font : this->Container)
    {
        if (font.GetInstanceID() == ID)
            return &font;
    }

    return nullptr;
}
MyFont *UIObjectManager_Fonts::GetFontEx(std::wstring Name, USHORT Size, USHORT Weight, DWORD Quality)
{
    // Iterate through the container and find the instance with the specified values.
    for (auto &font : this->Container)
    {
        if (font.GetName() == Name &&
            font.GetSize() == Size &&
            font.GetWeight() == Weight &&
            font.GetQuality() == Quality)
            return &font;
    }

    return nullptr;
}
std::vector<MyFont> &UIObjectManager_Fonts::GetContainer()
{
    return this->Container;
}

/// @class UIObjectManager_Icons definitions:
// Constructor:
UIObjectManager_Icons::UIObjectManager_Icons(HINSTANCE hInstance)
{
    // Initialize the default icons.
    if (!this->InitDefaultIcons(hInstance))
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to initialize the default icons.\n");
        error_message.append(L"CLASS: UIObjectManager_Icons\n");
        error_message.append(L"FUNC: Constructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
}
// Destructor:
UIObjectManager_Icons::~UIObjectManager_Icons()
{
    // Deinitialize the default icons.
    if (!this->DeinitDefaultIcons())
    {
        std::wstring error_message = L"";
        error_message.append(L"Error occurred!\n");
        error_message.append(L"Failed to deinitialize the default icons.\n");
        error_message.append(L"CLASS: UIObjectManager_Icons\n");
        error_message.append(L"FUNC: Destructor");
        MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
}
// Private member function(s) [(DE)INITIALIZATION FUNCTIONS]:
bool UIObjectManager_Icons::InitDefaultIcons(HINSTANCE hInstance)
{
    if (this->Cross_Black || this->Cross_Grey || this->Cross_White || this->Minus_Black || this->Minus_Grey || this->Minus_White || this->MAINICON)
        return false;

    this->Cross_Black = new MyIcon(hInstance, 20, 20, IDI_ICON1);
    this->Cross_Grey = new MyIcon(hInstance, 20, 20, IDI_ICON2);
    this->Cross_White = new MyIcon(hInstance, 20, 20, IDI_ICON3);
    this->Minus_Black = new MyIcon(hInstance, 20, 20, IDI_ICON4);
    this->Minus_Grey = new MyIcon(hInstance, 20, 20, IDI_ICON5);
    this->Minus_White = new MyIcon(hInstance, 20, 20, IDI_ICON6);
    this->MAINICON = new MyIcon(hInstance, 20, 20, IDI_ICON7);

    if (!this->Cross_Black || !this->Cross_Grey || !this->Cross_White || !this->Minus_Black || !this->Minus_Grey || !this->Minus_White || !this->MAINICON)
        return false;

    return true;
}
bool UIObjectManager_Icons::DeinitDefaultIcons()
{
    delete this->Cross_Black;
    delete this->Cross_Grey;
    delete this->Cross_White;
    delete this->Minus_Black;
    delete this->Minus_Grey;
    delete this->Minus_White;
    delete this->MAINICON;

    return true;
}
// Public member function(s) [CONTAINER FUNCTIONS]:
void UIObjectManager_Icons::AddIcon(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad, INT ID)
{
    // Check if the ID is unique.
    if (ID != 0)
    {
        for (auto &font : this->Container)
        {
            if (font.GetInstanceID() == ID)
            {
                std::wstring error_message = L"";
                error_message.append(L"Error occurred!\n");
                error_message.append(L"Another instance already uses this ID.\n");
                error_message.append(L"CLASS: UIObjectManager_Fonts\n");
                error_message.append(L"FUNC: AddFont()");
                MessageBoxW(NULL, error_message.c_str(), L"", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Add the object to the container.
    this->Container.push_back(MyIcon(hInstance, Width, Height, ResourceID, HICON_fuLoad, ID));
}
bool UIObjectManager_Icons::RemoveIcon(INT ID)
{
    // Iterate through the container and remove the instance with the specified ID.
    for (auto it = this->Container.begin(); it != this->Container.end();)
    {
        if ((*it).GetInstanceID() == ID)
        {
            it = this->Container.erase(it);
            return true;
        }
        else
            it++;
    }

    return false;
}
bool UIObjectManager_Icons::RemoveIconEx(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad)
{
    bool isAtLeastOneObjectRemoved = false;

    // Iterate through the container and remove the instance(s) with the specified values.
    for (auto it = this->Container.begin(); it != this->Container.end();)
    {
        if ((*it).GetHINSTANCE() == hInstance &&
            (*it).GetWidth() == Width &&
            (*it).GetHeight() == Height &&
            (*it).GetResourceID() == ResourceID &&
            (*it).GetLoadImageFlag() == HICON_fuLoad)
        {
            it = this->Container.erase(it);
            isAtLeastOneObjectRemoved = true;
        }
        else
            it++;
    }

    if (!isAtLeastOneObjectRemoved)
        return false;

    return true;
}
MyIcon *UIObjectManager_Icons::GetIcon(INT ID)
{
    // Iterate through the container and find the instance with the specified ID.
    for (auto &icon : this->Container)
    {
        if (icon.GetInstanceID() == ID)
            return &icon;
    }

    return nullptr;
}
MyIcon *UIObjectManager_Icons::GetIconEx(HINSTANCE hInstance, INT Width, INT Height, INT ResourceID, UINT HICON_fuLoad)
{
    // Iterate through the container and find the instance with the specified values.
    for (auto &icon : this->Container)
    {
        if (icon.GetHINSTANCE() == hInstance &&
            icon.GetWidth() == Width &&
            icon.GetHeight() == Height &&
            icon.GetResourceID() == ResourceID &&
            icon.GetLoadImageFlag() == HICON_fuLoad)
            return &icon;
    }

    return nullptr;
}
std::vector<MyIcon> &UIObjectManager_Icons::GetContainer()
{
    return this->Container;
}

/// @class UIObjectManager_Images definitions:
// Constructor:
UIObjectManager_Images::UIObjectManager_Images()
{
    this->Cross_Grey.Update(IDB_PNG1, ImageFormat::PNG);
    this->Cross_White.Update(IDB_PNG2, ImageFormat::PNG);
    this->Cross_Black.Update(IDB_PNG3, ImageFormat::PNG);
    this->Minus_Grey.Update(IDB_PNG4, ImageFormat::PNG);
    this->Minus_White.Update(IDB_PNG5, ImageFormat::PNG);
    this->Minus_Black.Update(IDB_PNG6, ImageFormat::PNG);
}
// Public member function(s) [GENERAL FUNCTIONS]:
void UIObjectManager_Images::UpdateNonClientButtonImages(MyImage NonClient_CloseButton_Default, MyImage NonClient_CloseButton_Hover, MyImage NonClient_CloseButton_Down,
    MyImage NonClient_MinimizeButton_Default, MyImage NonClient_MinimizeButton_Hover, MyImage NonClient_MinimizeButton_Down)
{
    this->NonClient_CloseButton_Default = NonClient_CloseButton_Default;
    this->NonClient_CloseButton_Hover = NonClient_CloseButton_Hover;
    this->NonClient_CloseButton_Down = NonClient_CloseButton_Down;
    this->NonClient_MinimizeButton_Default = NonClient_MinimizeButton_Default;
    this->NonClient_MinimizeButton_Hover = NonClient_MinimizeButton_Hover;
    this->NonClient_MinimizeButton_Down = NonClient_MinimizeButton_Down;
}

/// @class UIObjectManager_Miscs definitions:
// Constructor:
UIObjectManager_Miscs::UIObjectManager_Miscs()
{
    // Caption bar area rectangle.
    this->RECT_Caption =
        {
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Left
            WINDOW_BORDER_DEFAULTWIDTH,                                  // Top
            g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH,               // Right
            WINDOW_BORDER_DEFAULTWIDTH + WINDOW_CAPTIONBAR_DEFAULTHEIGHT // Bottom
        };

    // Top border area rectangle.
    this->RECT_SizeBorder_Top =
        {
            0,                         // Left
            0,                         // Top
            g_CurrentWindowWidth,          // Right
            WINDOW_BORDER_DEFAULTWIDTH // Bottom
        };

    // Bottom border area rectangle.
    this->RECT_SizeBorder_Bottom =
        {
            0,                                              // Left
            g_CurrentWindowHeight - WINDOW_BORDER_DEFAULTWIDTH, // Top
            g_CurrentWindowWidth,                               // Right
            g_CurrentWindowHeight                               // Bottom
        };

    // Left border area rectangle.
    this->RECT_SizeBorder_Left =
        {
            0,                          // Left
            1,                          // Top
            WINDOW_BORDER_DEFAULTWIDTH, // Right
            g_CurrentWindowHeight - 1       // Bottom
        };

    // Right border area rectangle.
    this->RECT_SizeBorder_Right =
        {
            g_CurrentWindowWidth - WINDOW_BORDER_DEFAULTWIDTH, // Left
            1,                                             // Top
            g_CurrentWindowWidth,                              // Right
            g_CurrentWindowHeight - 1                          // Bottom
        };
}

/// @class UIObjectManager definitions:
// Constructor:
UIObjectManager::UIObjectManager(HINSTANCE hInstance)
{
    Icons = new UIObjectManager_Icons(hInstance);
    this->Pointers.pCurrentBorderBrush = &Colors.Border.GetHBRRef();
}
// Destructor:
UIObjectManager::~UIObjectManager()
{
    delete Icons;
}
// Public static member function(s):
void UIObjectManager::ShowTotalObjects()
{
    std::wstring DebugMsg = L"";
    DebugMsg.append(L"Total MyColor instances: " + std::to_wstring(MyColor::GetTotalInstances()) + L"\n");
    DebugMsg.append(L"Total MyFont instances: " + std::to_wstring(MyFont::GetTotalInstances()) + L"\n");
    DebugMsg.append(L"Total MyIcon instances: " + std::to_wstring(MyIcon::GetTotalInstances()) + L"\n");
    DebugMsg.append(L"Total MyImage instances: " + std::to_wstring(MyImage::GetTotalInstances()) + L"\n");
    DebugMsg.append(L"--\n");
    DebugMsg.append(L"Total initialized MyColor instances: " + std::to_wstring(MyColor::GetTotalInitializedInstances()) + L"\n");
    DebugMsg.append(L"Total initialized MyFont instances: " + std::to_wstring(MyFont::GetTotalInitializedInstances()) + L"\n");
    DebugMsg.append(L"Total initialized MyIcon instances: " + std::to_wstring(MyIcon::GetTotalInitializedInstances()) + L"\n");
    DebugMsg.append(L"Total initialized MyImage instances: " + std::to_wstring(MyImage::GetTotalInitializedInstances()) + L"\n");
    MessageBoxW(NULL, DebugMsg.c_str(), L"", MB_OK | MB_ICONINFORMATION);
}