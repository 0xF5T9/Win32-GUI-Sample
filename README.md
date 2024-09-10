
# Win32 GUI Sample

This is a throw-away Win32 sample project. It uses Direct2D to custom draw controls, read and save configuration files, logging, and random Win32 scraps.

# How to Build
Compatible Compilers: Mingw64  | MSVC using MSBuild

Architecture: x64

Version: C++17

### 1. Add workspace settings (`settings.json`)
* Configure C/C++ workspace settings to match the compiler. (`Ctrl + Shift + P` -> `>C++: Edit Configurations`)
* Open the `settings.json` file and add the following:
```json
{
    ...
    // This allows VSCode tasks to execute PowerShell scripts.
    "terminal.integrated.profiles.windows": {
        "PowerShell": {
            "source": "PowerShell",
            "icon": "terminal-powershell",
            "args": [
                "-ExecutionPolicy",
                "Bypass"
            ]
        }
    }
    ...
}
```

### 2A. Build with Mingw64
* Open the `launch.json` file and update `miDebuggerPath` path. (Path to `gdb.exe``)
* Open the `task.ps1` file and update environment paths:
```shell
...
# Set required paths here:
$compiler_path = "C:\msys64\mingw64\bin\g++.exe"              # G++.exe     (Mingw64)
$resource_compiler_path = "C:\msys64\mingw64\bin\windres.exe" # windres.exe (Mingw64)
$project_file_path = ".\Win32-GUI-Sample.sln"                 # Project file .sln (MSBuild)
...
```

### 2B. Build with MSBuild

* Install Visual Studio & `Desktop development with C++` workload.
* Open the Visual Studio Code via the `Developer PowerShell for VS 2022` terminal. VSCode must be opened this way, or the VSCode tasks will be unable to run MSBuild-related tasks.

### 3. Execute tasks.

From the workspace terminal, use `./task.ps1 /?` to show predefined tasks.

For build details, check `task.ps1` file.
