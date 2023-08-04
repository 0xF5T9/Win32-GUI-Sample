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
struct MyStandardButtonSharedPropertiesConfig;
struct MyImageButtonInitializeConfig;
struct MyImageButtonSharedPropertiesConfig;
struct MyImageButtonNonSharedPropertiesConfig;
struct MyRadioButtonInitializeConfig;
struct MyRadioButtonSharedPropertiesConfig;
struct MyEditInitializeConfig;
struct MyEditNonSharedPropertiesConfig;
struct MyDDLComboboxSharedPropertiesConfig;
// Classes:
class MyRadioGroup;
// Functions:
namespace nSol
{
    void ShowLastErrorMessageBox();
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
        namespace Creation
        {
            void CreateMyFont(HFONT &hFont, std::wstring FontName, INT FontSize, INT FontWeight = FW_DONTCARE, DWORD FontQuality = DEFAULT_QUALITY);
        }
        namespace Utility
        {
            void RemoveWindowStyle(HWND hWnd, LONG Style);
        }
    }
}
namespace nApp::API::Draw
{
    void GetRoundRectPath(Gdiplus::GraphicsPath *pPath, Gdiplus::Rect rect, INT diameter);
    void DrawRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect rect, Gdiplus::Color color, INT radius, INT width);
    void FillRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Brush *pBrush, Gdiplus::Rect rect, Gdiplus::Color border, INT radius);
    bool MyDraw_DrawRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, INT Radius, INT Width, Gdiplus::Color pRectColor, bool showErrorMsg = false);
    bool MyDraw_FillRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor = nullptr, bool showErrorMsg = false);
    bool MyDraw_FillRoundRect(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, INT Radius, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor = nullptr, bool showErrorMsg = false);
    bool MyDraw_FillEllipse(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, INT Diameter, INT PosX, INT PosY, Gdiplus::Color *pRectColor, Gdiplus::Color *pRectBorderColor = nullptr, bool showErrorMsg = false);
    bool MyDraw_DrawImage(Gdiplus::Graphics *pGraphics, Gdiplus::Rect Rect, Gdiplus::Image* Image, FLOAT Opacity, INT PosX, INT PosY, INT Width, INT Height, bool Centering, bool showErrorMsg = false);
}
using namespace Gdiplus;
using namespace nApp::API::Draw;
using nApp::File::Log::WriteLog;
using nApp::File::Log::WriteLogEx;
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WindowProcedure_Container_MainContent(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

#endif // FORWARD_DECLARATIONS_H