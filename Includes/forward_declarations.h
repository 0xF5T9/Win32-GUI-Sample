#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H

// Enums:
enum class MyTheme;
enum class MyLogType;
enum class MyWindowType;
enum class MyEditboxType;
enum class MyImageFormat;
// Structs:
struct MyStandardButtonInitializeConfig;
struct MyStandardButtonSharedAttributesConfig;
struct MyImageButtonInitializeConfig;
struct MyImageButtonSharedAttributesConfig;
struct MyImageButtonNonSharedAttributesConfig;
struct MyRadioButtonInitializeConfig;
struct MyRadioButtonSharedAttributesConfig;
struct MyEditInitializeConfig;
struct MyEditNonSharedPropertiesConfig;
struct MyDDLComboboxSharedPropertiesConfig;
// Classes:
class MyRadioGroup;
// Functions:
namespace nSol
{
    bool ShowLastErrorMessageBox();
    std::wstring ConvertStringToWideString(const std::string&);
}
namespace nApp
{
    namespace File
    {
        namespace Log
        {
            bool WriteLog(std::wstring description, std::wstring details, MyLogType logType, bool noLogTypeText = false);
            bool WriteLogEx(std::wstring description, std::wstring details, MyLogType logType, int logLevel = 2, bool noLogTypeText = false);
        }
    }
    namespace Window
    {
        namespace Utility
        {
            bool RemoveWindowStyle(HWND hWnd, LONG style);
        }
    }
}
namespace nApp::API::GDIPDraw
{
    bool MyDraw_DrawRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, INT Radius, INT Width, Gdiplus::Color pRectColor);
    bool MyDraw_FillRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor = nullptr);
    bool MyDraw_FillRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, INT Radius, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor = nullptr);
    bool MyDraw_FillEllipse(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, INT Diameter, INT PosX, INT PosY, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor = nullptr);
    bool MyDraw_DrawImage(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, Gdiplus::Image* Image, FLOAT Opacity, INT PosX, INT PosY, INT Width, INT Height, bool Centering);
}
using namespace Gdiplus;
using namespace nApp::API::GDIPDraw;
using nApp::File::Log::WriteLog;
using nApp::File::Log::WriteLogEx;
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WindowProcedure_Container_MainContent(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

#endif // FORWARD_DECLARATIONS_H