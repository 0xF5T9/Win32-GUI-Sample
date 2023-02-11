/*
*  File: obj_manager.h
*  Object manager class header
*
*  - Manage most objects used to drawing the application (GDI & GDI+ Objects)
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
       /*************************************************************************************************************
       * COLOR OBJECTS:                                                                                             *
       * "Gdiplus::Color" objects will be used to create other variants color object(if needed) of the same color   *
       **************************************************************************************************************/
       Gdiplus::Color CL_DEBUG,       // Application debug color
              CL_Primary,             // Primary application color
              CL_Secondary,           // Secondary application color
              CL_BorderActive,        // Window border active color
              CL_BorderInactive,      // Window border inactive color
              CL_DefaultText,         // Default text color
              CL_InactiveText,        // Inactive text color
              CL_EditBox,             // Editbox color
              CL_EditBoxBorder;       // Editbox border color

       COLORREF CLR_DEBUG = NULL, CLR_Primary = NULL, CLR_Secondary = NULL, CLR_BorderActive = NULL, CLR_BorderInactive = NULL, CLR_DefaultText = NULL, CLR_InactiveText = NULL, CLR_EditBox = NULL, CLR_EditBoxBorder = NULL;
       HBRUSH HBR_DEBUG = nullptr, HBR_Primary = nullptr, HBR_Secondary = nullptr, HBR_BorderActive = nullptr, HBR_BorderInactive = nullptr, HBR_EditBox = nullptr;



       /*****************
       * MEDIA OBJECTS: *
       ******************/
       HICON HICO_Close = nullptr, HICO_Close_Hover = nullptr, HICO_Close_Inactive = nullptr, HICO_Minimize = nullptr, HICO_Minimize_Hover = nullptr, HICO_Minimize_Inactive = nullptr;



       /***************
       * FONT OBJECTS *
       ****************/
       HFONT HFO_Default = nullptr, HFO_Title = nullptr, HFO_Heading = nullptr, HFO_Edit = nullptr, HFO_Note = nullptr;



       /***********************
       * PLACEHOLDER POINTERS *
       ************************/
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