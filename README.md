# Win32 GUI Sample
Win32 GUI Example | ...

Compiler: Visual Studio 2022+ (C++17)

Architecture: x64

## Set-up compile environment:
Requires:
- Visual Studio 2022+
- C++17

VS Project Configurations:
1. Set unicode character sets:


`C/C++ → Command Line → Additional Options → Add "/utf-8"`


`Advanced → Set "Character Set" to "Use Unicode Character Set"`


2. Link following libraries: Dwmapi.lib, UxTheme.lib, Comctl32.lib, Gdiplus.lib, winmm.lib


`Linker → Input → Additional Dependencies → Add ".lib"`
