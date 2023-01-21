/*
    File: obj_manager.cpp
    Object manager class definition

    - Manage most objects used to drawing the application (GDI & GDI+ Objects)
*/

#include "./Headers/c_resources.h"
#include <iostream>
#include <string>
#include <map>
#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <Dwmapi.h>
#include "./Headers./obj_manager.h"
#include "./Headers/subclasses.h"
#include "./Headers/global.h"

using namespace Gdiplus;

// FORWARD DECLARATIONS
namespace nSol
{
       void CreateHFONT(HFONT* hFontPtr, std::wstring fName, int fSize, int fWeight = FW_DONTCARE, int fQuality = DEFAULT_QUALITY);
}

// Destroy current color objects
bool OBJ_Manager::DestroyColorObjects()
{
       if (!DeleteObject(this->HBR_DEBUG) ||
              !DeleteObject(this->HBR_Primary) ||
              !DeleteObject(this->HBR_Secondary) ||
              !DeleteObject(this->HBR_BorderActive) ||
              !DeleteObject(this->HBR_BorderInactive) ||
              !DeleteObject(this->HBR_EditBox))
              return false;

       return true;
}

// Destroy current media objects
bool OBJ_Manager::DestroyMediaObjects()
{
       if (!DestroyIcon(this->HICO_Close) ||
              !DestroyIcon(this->HICO_Close_Hover) ||
              !DestroyIcon(this->HICO_Close_Inactive) ||
              !DestroyIcon(this->HICO_Minimize) ||
              !DestroyIcon(this->HICO_Minimize_Hover) ||
              !DestroyIcon(this->HICO_Minimize_Inactive))
              return false;

       return true;
}

// Destroy current font objects
bool OBJ_Manager::DestroyFontObjects()
{
       if (!DeleteObject(this->HFO_Default) ||
              !DeleteObject(this->HFO_Title) ||
              !DeleteObject(this->HFO_Heading) ||
              !DeleteObject(this->HFO_Edit) ||
              !DeleteObject(this->HFO_Note))
              return false;

       return true;
}

// Update new color objects
bool OBJ_Manager::UpdateColorObjects(Gdiplus::Color PrimaryColor, Gdiplus::Color SecondaryColor, Gdiplus::Color BorderActiveColor, Gdiplus::Color BorderInactiveColor,
       Gdiplus::Color DefaultTextColor, Gdiplus::Color InactiveTextColor, Gdiplus::Color EditBoxColor, Gdiplus::Color EditBoxBorderColor, Gdiplus::Color DebugColor)
{
       static bool IsInitialized = 0;

       if (IsInitialized)
              if (!DestroyColorObjects())
                     return false;

       this->CL_DEBUG = DebugColor;
       this->CLR_DEBUG = RGB((int)CL_DEBUG.GetRed(), (int)CL_DEBUG.GetGreen(), (int)CL_DEBUG.GetBlue());
       this->HBR_DEBUG = CreateSolidBrush(CLR_DEBUG);

       this->CL_Primary = PrimaryColor;
       this->CLR_Primary = RGB((int)CL_Primary.GetRed(), (int)CL_Primary.GetGreen(), (int)CL_Primary.GetBlue());
       this->HBR_Primary = CreateSolidBrush(CLR_Primary);

       this->CL_Secondary = SecondaryColor;
       this->CLR_Secondary = RGB((int)CL_Secondary.GetRed(), (int)CL_Secondary.GetGreen(), (int)CL_Secondary.GetBlue());
       this->HBR_Secondary = CreateSolidBrush(CLR_Secondary);

       this->CL_BorderActive = BorderActiveColor;
       this->CLR_BorderActive = RGB((int)CL_BorderActive.GetRed(), (int)CL_BorderActive.GetGreen(), (int)CL_BorderActive.GetBlue());
       this->HBR_BorderActive = CreateSolidBrush(CLR_BorderActive);

       this->CL_BorderInactive = BorderInactiveColor;
       this->CLR_BorderInactive = RGB((int)CL_BorderInactive.GetRed(), (int)CL_BorderInactive.GetGreen(), (int)CL_BorderInactive.GetBlue());
       this->HBR_BorderInactive = CreateSolidBrush(CLR_BorderInactive);

       this->CL_DefaultText = DefaultTextColor;
       this->CLR_DefaultText = RGB((int)CL_DefaultText.GetRed(), (int)CL_DefaultText.GetGreen(), (int)CL_DefaultText.GetBlue());

       this->CL_InactiveText = InactiveTextColor;
       this->CLR_InactiveText = RGB((int)CL_InactiveText.GetRed(), (int)CL_InactiveText.GetGreen(), (int)CL_InactiveText.GetBlue());

       this->CL_EditBox = EditBoxColor;
       this->CLR_EditBox = RGB((int)CL_EditBox.GetRed(), (int)CL_EditBox.GetGreen(), (int)CL_EditBox.GetBlue());
       this->HBR_EditBox = CreateSolidBrush(CLR_EditBox);

       this->CL_EditBoxBorder = EditBoxBorderColor;
       this->CLR_EditBoxBorder = RGB((int)CL_EditBoxBorder.GetRed(), (int)CL_EditBoxBorder.GetGreen(), (int)CL_EditBoxBorder.GetBlue());

       IsInitialized = true;

       return true;
}

// Update new font objects
bool OBJ_Manager::UpdateFontObjects()
{
       static bool IsInitialized = 0;

       if (IsInitialized)
              if (!DestroyFontObjects())
                     return false;

       nSol::CreateHFONT(&this->HFO_Default, L"Segoe UI", 24, FW_LIGHT, CLEARTYPE_QUALITY);
       nSol::CreateHFONT(&this->HFO_Title, L"Segoe UI", 24, FW_LIGHT, CLEARTYPE_QUALITY);
       nSol::CreateHFONT(&this->HFO_Heading, L"Segoe UI", 28, FW_BOLD, CLEARTYPE_QUALITY);
       nSol::CreateHFONT(&this->HFO_Edit, L"Segoe UI", 28, FW_LIGHT, CLEARTYPE_QUALITY);
       nSol::CreateHFONT(&this->HFO_Note, L"Segoe UI", 28, FW_LIGHT, CLEARTYPE_QUALITY);

       IsInitialized = true;

       return true;
}