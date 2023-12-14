
# Win32 GUI Sample

This is a sample project showcasing a Win32 GUI application. The user interfaces are created with custom drawings using Direct2D.

# How to Build
Compatible Compilers: Mingw64  | MSVC using MSBuild

Architecture: x64

Version: C++17

Build with VSCode tasks and PowerShell script.

Open `task.ps1` file and update environment paths:
```shell
...
# Set required paths here:
$compiler_path = "C:\msys64\mingw64\bin\g++.exe"              # G++.exe     (Mingw64)
$resource_compiler_path = "C:\msys64\mingw64\bin\windres.exe" # windres.exe (Mingw64)
$project_file_path = ".\Win32-GUI-Sample.sln"                 # Project file .sln (MSBuild)
...
```

From the workspace terminal, use `./task.ps1 /?` to show predefined tasks.

For build details, check `task.ps1` file.
