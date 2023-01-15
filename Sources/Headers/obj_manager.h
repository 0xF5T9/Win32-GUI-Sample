#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <Dwmapi.h>

class OBJ_Manager
{
public:
       // Color objects
       Gdiplus::Color CL_DEBUG,
              CL_Primary,
              CL_Secondary,
              CL_BorderActive,
              CL_BorderInactive,
              CL_DefaultText,
              CL_InactiveText;
       COLORREF CLR_DEBUG = NULL, CLR_Primary = NULL, CLR_Secondary = NULL, CLR_BorderActive = NULL, CLR_BorderInactive = NULL, CLR_DefaultText = NULL, CLR_InactiveText = NULL;
       HBRUSH HBR_DEBUG = nullptr, HBR_Primary = nullptr, HBR_Secondary = nullptr, HBR_BorderActive = nullptr, HBR_BorderInactive = nullptr;

       // Media objects
       HICON HICO_Close = nullptr, HICO_Close_Hover = nullptr, HICO_Close_Inactive = nullptr, HICO_Minimize = nullptr, HICO_Minimize_Hover = nullptr, HICO_Minimize_Inactive = nullptr;

       // Font objects
       HFONT HFO_Default = nullptr, HFO_Title = nullptr, HFO_Heading = nullptr;

       // Placeholders
       HBRUSH* HBRP_CURRENTBORDER = nullptr;

public:
       OBJ_Manager() 
       {
              UpdateFontObjects();
       }
       ~OBJ_Manager() 
       {
              DestroyColorObjects();
              DestroyMediaObjects();
              DestroyFontObjects();
       }
public:
       bool DestroyColorObjects();
       bool DestroyMediaObjects();
       bool DestroyFontObjects();
       bool UpdateColorObjects(Gdiplus::Color PrimaryColor, Gdiplus::Color SecondaryColor, Gdiplus::Color BorderActiveColor, Gdiplus::Color BorderInactiveColor,
              Gdiplus::Color DefaultTextColor, Gdiplus::Color InactiveTextColor, Gdiplus::Color DebugColor);
       bool UpdateFontObjects();
};