/*
    File: obj_manager.h
    Object manager class header

    - Manage most objects used to drawing the application (GDI & GDI+ Objects)
*/

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
              CL_InactiveText,
              CL_EditBox,
              CL_EditBoxBorder;
       COLORREF CLR_DEBUG = NULL, CLR_Primary = NULL, CLR_Secondary = NULL, CLR_BorderActive = NULL, CLR_BorderInactive = NULL, CLR_DefaultText = NULL, CLR_InactiveText = NULL, CLR_EditBox = NULL, CLR_EditBoxBorder = NULL;
       HBRUSH HBR_DEBUG = nullptr, HBR_Primary = nullptr, HBR_Secondary = nullptr, HBR_BorderActive = nullptr, HBR_BorderInactive = nullptr, HBR_EditBox = nullptr;

       // Media objects
       HICON HICO_Close = nullptr, HICO_Close_Hover = nullptr, HICO_Close_Inactive = nullptr, HICO_Minimize = nullptr, HICO_Minimize_Hover = nullptr, HICO_Minimize_Inactive = nullptr;

       // Font objects
       HFONT HFO_Default = nullptr, HFO_Title = nullptr, HFO_Heading = nullptr, HFO_Edit = nullptr, HFO_Note = nullptr;

       // Placeholder pointers
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
              Gdiplus::Color DefaultTextColor, Gdiplus::Color InactiveTextColor, Gdiplus::Color EditBoxColor, Gdiplus::Color EditBoxBorderColor, Gdiplus::Color DebugColor);
       bool UpdateFontObjects();
};