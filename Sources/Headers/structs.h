/*
	File: structs.h
	My structs definitions
*/

#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <Windows.h>
#include <Windowsx.h>
#include <Uxtheme.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <Dwmapi.h>

struct ContainerInfo
{
private:

    const int DefaultSpacingPixel = 10;   // Default spacing distance between lines (In-pixel)
    int CurrentLines = 0;                 // Total numbers of the current lines
    int CurrentPos = 0;                   // Latest vertical position of the current line (In-pixel)
    int VerticalContentPixels = 0;        // Total vertical pixels of all contents
    int ExtraVerticalContentPixels = 0;   // Extra spacing pixel modifier variable
    
private:

    /**
    * Get the item(hWnd) width and height 
    */
    bool GetItemSize(HWND hWnd, int& Width, int& Height, bool IsNonClient = 0)
    {
        RECT LRECT;

        if (!IsNonClient) if (!GetClientRect(hWnd, &LRECT)) return false;
        else if (!GetWindowRect(hWnd, &LRECT)) return false;

        Width = LRECT.right - LRECT.left;
        Height = LRECT.bottom - LRECT.top;

        return true;
    }

public:

    /**
    * Get current line's vertical position 
    */
   int GetCurVertPos()
   {
        return this->CurrentPos + this->ExtraVerticalContentPixels;
   }

    /**
    * Get total vertical pixels of all contents
    */
    int GetContentPixels()
    {
        int RET = (((this->CurrentLines - 1) * this->DefaultSpacingPixel) + this->ExtraVerticalContentPixels) + this->VerticalContentPixels;

        return RET;
    }

    /**
    * Register new line
    */
    void EndLine(HWND hWnd, int ExtraSpacingPixel = 0)
    {
        int Width, Height;
        this->GetItemSize(hWnd, Width, Height);

        this->VerticalContentPixels += Height;
        this->ExtraVerticalContentPixels += ExtraSpacingPixel;
        this->CurrentPos += (Height + this->DefaultSpacingPixel);
        this->CurrentLines += 1;
    }
};