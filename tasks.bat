@echo off
setlocal EnableDelayedExpansion
chcp 65001
cls

set current_directory=%~dp0
set is_correct_directory=true
set workspace_name=unset
set compile_flags=unset

REM Get workspace name.
for /f "delims=\" %%a in ("%cd%") do set workspace_name=%%~nxa

REM Check if the current directory is the workspace root directory.
if not exist "Headers\" (
    set is_correct_directory=false
)
if not exist "Sources\" (
    set is_correct_directory=false
)
if "%is_correct_directory%" == "false" (
    mecho "This script file must be placed in the workspace root directory." red & echo\ & echo\
    exit /b 1
)

REM Terminate the output executable if it is running to prevent potential write issues.
FOR /F %%x IN ('tasklist /NH /FI "IMAGENAME eq !workspace_name!.exe"') DO IF NOT %%x == !workspace_name!.exe (
    REM ...
) else (
    mecho "Terminating the '" & mecho "!workspace_name!.exe" teal & mecho "' process to prevent potential write issues." & echo\
    taskkill /f /IM !workspace_name!.exe
    timeout /T 1 /NOBREAK > nul
)

REM ###############
REM # MinGW Tasks #
REM ###############

REM [MinGW - Clean]
if "%1" == "mingw-clean" (
    if not exist "Build\MinGW" (
        mecho "No MinGW build files exist." grey & echo\ & echo\
        exit /b 0
    )

    echo Cleaning build files ...
    del /F /S /Q "Build\MinGW\*""
    rmdir /S /Q "Build\MinGW"
    mecho "Build files cleaned." TEAL & echo\ & echo\
    exit /b 0
REM [MinGW - Build Resource File]
) else if "%1" == "mingw-build-rc" (
    if exist "Build\MinGW\resources.o" (
        del /F /Q "Build\MinGW\resources.o"
    )

    echo Building resource file ...
    if not exist "Build\" (
        mecho "Creating 'Build' folder ..." grey & echo\
        mkdir Build
    )
    if not exist "Build\MinGW" (
        mecho "Creating 'MinGW' folder ..." grey & echo\
        mkdir Build\MinGW
    )
    windres "Resources/resources.rc" "Build\MinGW\resources.o"
    if not exist "Build/MinGW/resources.o" (
        mecho "Failed to build the resource file." red & echo\ & echo\
        exit /b 1
    )
    mecho "Resource file built successfully." green & echo\ & echo\

    exit /b 0
REM [MinGW - Dynamic Linking Build (Symbols)]
) else if "%1" == "mingw-build-dl" (
    if not exist "Build/MinGW/resources.o" (
        mecho "The resource file must be built first." grey & echo\ & echo\
        exit /b 1
    )

    mecho "Compile flags:" LIGHT_YELLOW & echo\
    set compile_flags=^
-Wall -m64 -std=c++17 -municode -mwindows -fdiagnostics-color=always -finput-charset=utf-8 -fexec-charset=utf-8 ^
-g -Og Sources/*.cpp Build/MinGW/resources.o -o Build/MinGW/!workspace_name!.exe ^
-Wl,-Bdynamic -luxtheme -lcomctl32 -ldwmapi -lwinmm -lpowrprof -ld2d1 -ldwrite -lwindowscodecs ^
-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
    echo !compile_flags!
    echo\ & mecho "[Build in progress, please wait...]" LIGHT_YELLOW & echo\
    g++ !compile_flags!
    if not exist "Build/MinGW/!workspace_name!.exe" (
        echo\ & mecho "✘ Build failed." red & echo\ & echo\
        exit /b 1
    )
    mecho "✔ Build completed." green & echo\ & echo\

    exit /b 0
REM [MinGW - Dynamic Linking Build (No Symbols)]
) else if "%1" == "mingw-build-dls" (
    if not exist "Build/MinGW/resources.o" (
        mecho "The resource file must be built first." grey & echo\ & echo\
        exit /b 1
    )

    mecho "Compile flags:" LIGHT_YELLOW & echo\
    set compile_flags=^
-Wall -m64 -std=c++17 -municode -mwindows -fdiagnostics-color=always -finput-charset=utf-8 -fexec-charset=utf-8 ^
-Ofast -s Sources/*.cpp Build/MinGW/resources.o -o Build/MinGW/!workspace_name!.exe ^
-Wl,-Bdynamic -luxtheme -lcomctl32 -ldwmapi -lwinmm -lpowrprof -ld2d1 -ldwrite -lwindowscodecs ^
-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
    echo !compile_flags!
    echo\ & mecho "[Build in progress, please wait...]" LIGHT_YELLOW & echo\
    g++ !compile_flags!
    if not exist "Build/MinGW/!workspace_name!.exe" (
        echo\ & mecho "✘ Build failed." red & echo\ & echo\
        exit /b 1
    )
    mecho "✔ Build completed." green & echo\ & echo\

    exit /b 0
REM [MinGW - Static Linking Build]
) else if "%1" == "mingw-build-sl" (
    if not exist "Build/MinGW/resources.o" (
        mecho "The resource file must be built first." grey & echo\ & echo\
        exit /b 1
    )

    mecho "Compile flags:" LIGHT_YELLOW & echo\
    set compile_flags=^
-Wall -m64 -std=c++17 -municode -mwindows -fdiagnostics-color=always -finput-charset=utf-8 -fexec-charset=utf-8 ^
-Ofast -s Sources/*.cpp Build/MinGW/resources.o -o Build/MinGW/!workspace_name!.exe ^
-Wl,-Bstatic -luxtheme -lcomctl32 -ldwmapi -lwinmm -lpowrprof -ld2d1 -ldwrite -lwindowscodecs ^
-Wl,-Bdynamic -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
    echo !compile_flags!
    echo\ & mecho "[Build in progress, please wait...]" LIGHT_YELLOW & echo\
    g++ !compile_flags!
    if not exist "Build/MinGW/!workspace_name!.exe" (
        echo\ & mecho "✘ Build failed." red & echo\ & echo\
        exit /b 1
    )
    mecho "✔ Build completed." green & echo\ & echo\

    exit /b 0

REM #################
REM # MSBuild Tasks #
REM #################

REM [MSBuild - Clean]
) else if "%1" == "msbuild-clean" (
    if not exist "Build\MSBuild" (
        mecho "No MSBuild build files exist." grey & echo\ & echo\
        exit /b 0
    )
    echo Cleaning build files ...
    msbuild !workspace_name!.sln -noLogo /t:Clean /p:Configuration=Release-DL /p:Platform=x64
    msbuild !workspace_name!.sln -noLogo /t:Clean /p:Configuration=Release-SL /p:Platform=x64
    del /F /S /Q "Build\MSBuild\*""
    rmdir /S /Q "Build\MSBuild"
    mecho "Build files cleaned." TEAL & echo\ & echo\
    exit /b 0
REM [MSBuild - Dynamic Linking Build]
) else if "%1" == "msbuild-build-dl" (
    mecho "Build flags:" LIGHT_YELLOW & echo\
    set compile_flags=-noLogo /p:Configuration=Release-DL /p:Platform=x64
    echo !compile_flags!
    msbuild !workspace_name!.sln !compile_flags!

    if not exist "Build/MSBuild/Output/!workspace_name!.exe" (
        echo\ & mecho "✘ Build failed." red & echo\ & echo\
        exit /b 1
    )
    echo\ & mecho "✔ Build completed." green & echo\ & echo\

    exit /b 0
REM [MSBuild - Static Linking Build]
) else if "%1" == "msbuild-build-sl" (
    mecho "Build flags:" LIGHT_YELLOW & echo\
    set compile_flags=-noLogo /p:Configuration=Release-SL /p:Platform=x64
    echo !compile_flags!
    msbuild !workspace_name!.sln !compile_flags!

    if not exist "Build/MSBuild/Output/!workspace_name!.exe" (
        echo\ & mecho "✘ Build failed." red & echo\ & echo\
        exit /b 1
    )
    echo\ & mecho "✔ Build completed." green & echo\ & echo\

    exit /b 0

REM #################
REM # Default Tasks #
REM #################

) else (
    if "%1" == "" (
        mecho "Usage: " yellow & mecho "tasks.bat task_name" grey
    ) else (
        mecho "The requested task '%1' doesn't exist." red
    )
    echo\ & echo\

    exit /b 1
)