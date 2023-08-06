# Win32 GUI Sample
Win32 GUI Example

Project: Visual Studio 2022 (C++17)

Architecture: x64

## Set-up compiler environment:
Requires:
- Visual Studio 2022
- C++17

VS Project Configurations:
1. Set unicode character sets:


`C/C++ → Command Line → Additional Options → Add "/utf-8"`


`Advanced → Set "Character Set" to "Use Unicode Character Set"`


2. Link following libraries: UxTheme.lib, Comctl32.lib, Gdiplus.lib, Dwmapi.lib, Winmm.lib, Powrprof.lib


`Linker → Input → Additional Dependencies → Add ".lib"`