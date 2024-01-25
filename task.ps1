# Notes:
# - This script file encoding must be UTF-8 BOM, or unicode characters won't be shown correctly.
# - If the script can't be loaded due to the execution policy, use the below approaches:
#
#   For a normal PowerShell session:
#       1. Use the command: Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process
#       2. Open the script again.
#
#   For the VSCode workspace (settings.json):
#       1. Add the following options to `settings.json`:
#        "terminal.integrated.profiles.windows": {
#            "PowerShell": {
#                "source": "PowerShell",
#                "icon": "terminal-powershell",
#                "args": ["-ExecutionPolicy", "Bypass"]
#            }
#        },
#        "terminal.integrated.defaultProfile.windows": "PowerShell"
#       2. Restart the Visual Studio Code.
#       3. Open the script again.
# ------------------------------------------------------------------------------------------------

# Function that will be used to write colored output.
function Write-Color {
    param(
        [Parameter(Mandatory = $true)][string[]]$Text,
        [Parameter(Mandatory = $true)][ConsoleColor[]]$Color
    )
    # Fallback defaults if one of the values isn't set.
    $LastForegroundColor = [console]::ForegroundColor
    # The real deal coloring.
    for ($i = 0; $i -lt $Text.Count; $i++) {
        $CurrentFGColor = if ($Color[$i]) { $Color[$i] } else { $LastForegroundColor }
        $WriteParams = @{
            NoNewLine       = $true
            ForegroundColor = $CurrentFGColor
        }
        Write-Host $Text[$i] @WriteParams
        # Store last color set, in case next iteration doesn't have a set color.
        $LastForegroundColor = $CurrentFGColor
    }

    Write-Host
}

$disable_mingw64_tasks = $false # The variable determine whether to disable Mingw64 tasks.
$disable_msbuild_tasks = $false # The variable determine whether to disable MSbuild tasks.

# Set required paths here:
$compiler_path = "C:\msys64\ucrt64\bin\g++.exe"              # G++.exe     (Mingw64)
$resource_compiler_path = "C:\msys64\ucrt64\bin\windres.exe" # windres.exe (Mingw64)
$project_file_path = ".\Win32-GUI-Sample.sln"                 # Project file .sln (MSBuild)

# Test the set paths.
if (!(Test-Path("$($compiler_path)"))) {
    Write-Color "The path to the G++ compiler is unset or invalid." DarkGray
    $disable_mingw64_tasks = $true
}
if (!(Test-Path("$($resource_compiler_path)"))) {
    Write-Color "The path to the resource compiler is unset or invalid." DarkGray
    $disable_mingw64_tasks = $true
}
if (!(Test-Path("$($project_file_path)"))) {
    Write-Color "The path to the visual studio project file is unset or invalid." DarkGray
    $disable_msbuild_tasks = $true
}

if ($disable_mingw64_tasks -eq $true) {
    Write-Color "Required path(s) are unset or invalid, Mingw64 tasks are disabled.`n" DarkGray
}
if ($disable_msbuild_tasks -eq $true) {
    Write-Color "Required path(s) are unset or invalid, MSBuild tasks are disabled.`n" DarkGray
}

# Print usage texts.
if (($args.Count -eq 0) -or ($args[0] -match "\?") -or ($args[0] -match "help")) {
    Write-Color -text "Available tasks: ", "build clean" -color Yellow, Cyan
    Write-Color -text "Task options: ", "Use ", "/? ", "(Example: ./task.ps1 build /?)" -color Yellow, White, Cyan, White
    Write-Color -text "Example: ", "./task.ps1 <task name> <task option 1> <task option 2> <...>" -color Yellow, DarkGray
    exit 0
}

# Get the workspace path and name.
$workspace_path = Get-Location
$workspace_name = Split-Path -Path $workspace_path -Leaf

# Terminate the exetcutable if it is running.
$executable_name = "$($workspace_name)"
$process = Get-Process | Where-Object { $_.ProcessName -eq $executable_name }
if ($process) {
    Stop-Process -Id $process.Id -Force
    Write-Color "The executable process was detected. Terminating the process to prevent unexpected conflicts." Cyan
}

# Check if this is the correct directory.
$required_folders = @("Headers", "Sources", "Resources")
foreach ($folder in $required_folders) {
    if (!(Test-Path("./$($folder)"))) {
        Write-Color -text "Workspace path: ", "$($workspace_path)" -color White, Cyan 
        Write-Color -text "Workspace name: ", "$($workspace_name)`n" -color White, Cyan
        Write-Color -text "'", "$($folder)", "' folder not found." -color White, Red, White
        Write-Color "This script file must be placed in the workspace root directory." Yellow
        exit 1
    }
}

# Process the task
$task_name = $args[0]

# [Build tasks]
if ($task_name -match "build") {
    # Print build options
    if (($args.Count -eq 1) -or ($args[1] -match "\?") -or ($args[1] -match "help")) {
        Write-Color "Build options:" Yellow
        Write-Color -text "mingw64-debug", ": Build the executable using Mingw64 compiler. (Debug)" -color Cyan, White
        Write-Color -text "mingw64-release", ": Build the executable using Mingw64 compiler. (Release)" -color Cyan, White
        Write-Color -text "msbuild-debug", ": Build the executable using MSBuild build tool. (Debug)" -color Cyan, White
        Write-Color -text "msbuild-release", ": Build the executable using MSBuild build tool. (Release)" -color Cyan, White
        Write-Color -text "Example: ", "./task.ps1 build mingw64-release" -color Yellow, DarkGray
        exit 0
    }

    # 'mingw64-debug'
    if ($args[1] -eq "mingw64-debug") {
        if ($disable_mingw64_tasks -eq $true) {
            Write-Color "Mingw64 tasks are disabled." DarkGray
            exit 0
        }

        # Create build directories.
        if (!(Test-Path("./Build"))) {
            Write-Color -Text "Creating '", "Build", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build | Out-Null
        }
        if (!(Test-Path("./Build/Mingw64"))) {
            Write-Color -Text "Creating '", "Mingw64", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build/Mingw64 | Out-Null
        }

        # Clean build files.
        if (Test-Path("./Build/Mingw64")) {
            Remove-Item ./Build/Mingw64/* -recurse
        }

        # Build resource file.
        Write-Color -text "Resource compiler path: ", "$($resource_compiler_path)" -color Yellow, Cyan
        Write-Color "Building resource file ..." White
        $rc_build_command = "$($resource_compiler_path) ./Resources/resources.rc ./Build/Mingw64/resources.o"
        Invoke-Expression $rc_build_command
        if (!(Test-Path("./Build/Mingw64/resources.o"))) {
            Write-Color "Failed to build the resource file." Red
            exit 1
        }
        Write-Color "Resource file built successfully.`n" Green
        
        # Compile the executable.
        Write-Color -text "Compiler path: ", "$($compiler_path)" -color Yellow, Cyan
        $compile_flags =
        "-Wall -m64 -std=c++17 -municode -mwindows -fdiagnostics-color=always -finput-charset=utf-8 -fexec-charset=utf-8 " +
        "-g -Og Sources/*.cpp Build/Mingw64/resources.o -o Build/Mingw64/$($workspace_name).exe " +
        "`"-Wl,-Bdynamic`" -luxtheme -lcomctl32 -ldwmapi -lwinmm -lpowrprof -ld2d1 -ldwrite -lwindowscodecs " +
        "-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32"
        Write-Color -text "Compile flags: ", "$($compile_flags)`n" -color Yellow, Cyan
        
        Write-Color "[Build in progress, please wait...]" DarkYellow
        $build_command = "$($compiler_path) $($compile_flags)"
        Invoke-Expression $build_command

        if (!(Test-Path("./Build/Mingw64/$($workspace_name).exe"))) {
            Write-Color "✘ Build failed." Red
            exit 1
        }
        Write-Color -text "✔ Build completed. (", "$($args[1])", ")" -color Green, Cyan, Green
        exit 0
    }

    # 'mingw64-release'
    elseif ($args[1] -eq "mingw64-release") {
        if ($disable_mingw64_tasks -eq $true) {
            Write-Color "Mingw64 tasks are disabled." DarkGray
            exit 0
        }

        # Create build directories.
        if (!(Test-Path("./Build"))) {
            Write-Color -Text "Creating '", "Build", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build | Out-Null
        }
        if (!(Test-Path("./Build/Mingw64"))) {
            Write-Color -Text "Creating '", "Mingw64", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build/Mingw64 | Out-Null
        }

        # Clean build files.
        if (Test-Path("./Build/Mingw64")) {
            Remove-Item ./Build/Mingw64/* -recurse
        }

        # Build resource file.
        Write-Color -text "Resource compiler path: ", "$($resource_compiler_path)" -color Yellow, Cyan
        Write-Color "Building resource file ..." White
        $rc_build_command = "$($resource_compiler_path) ./Resources/resources.rc ./Build/Mingw64/resources.o"
        Invoke-Expression $rc_build_command
        if (!(Test-Path("./Build/Mingw64/resources.o"))) {
            Write-Color "Failed to build the resource file." Red
            exit 1
        }
        Write-Color "Resource file built successfully.`n" Green
        
        # Compile the executable.
        Write-Color -text "Compiler path: ", "$($compiler_path)" -color Yellow, Cyan
        $compile_flags =
        "-Wall -m64 -std=c++17 -municode -mwindows -fdiagnostics-color=always -finput-charset=utf-8 -fexec-charset=utf-8 " +
        "-Ofast -s Sources/*.cpp Build/Mingw64/resources.o -o Build/Mingw64/$($workspace_name).exe " +
        "`"-Wl,-Bdynamic`" -luxtheme -lcomctl32 -ldwmapi -lwinmm -lpowrprof -ld2d1 -ldwrite -lwindowscodecs " +
        "-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32"
        Write-Color -text "Compile flags: ", "$($compile_flags)`n" -color Yellow, Cyan
        
        Write-Color "[Build in progress, please wait...]" DarkYellow
        $build_command = "$($compiler_path) $($compile_flags)"
        Invoke-Expression $build_command

        if (!(Test-Path("./Build/Mingw64/$($workspace_name).exe"))) {
            Write-Color "✘ Build failed." Red
            exit 1
        }
        Write-Color -text "✔ Build completed. (", "$($args[1])", ")" -color Green, Cyan, Green
        exit 0
    }

    # 'msbuild-debug'
    elseif ($args[1] -eq "msbuild-debug") {
        if ($disable_msbuild_tasks -eq $true) {
            Write-Color "MSBuild tasks are disabled." DarkGray
            exit 0
        }

        # Create build directories.
        if (!(Test-Path("./Build"))) {
            Write-Color -Text "Creating '", "Build", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build | Out-Null
        }
        if (!(Test-Path("./Build/MSBuild"))) {
            Write-Color -Text "Creating '", "MSBuild", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build/MSBuild | Out-Null
        }
        if (Test-Path("./Build/MSBuild/Output")) {
            Remove-Item ./Build/MSBuild/Output/*.exe
        }

        $build_flags = "-noLogo /p:Configuration=Debug /p:Platform=x64"
        Write-Color -text "Build flags: ", "$($build_flags)" -color Yellow, Cyan
        
        $build_command = "msbuild $($workspace_name).sln $($build_flags)"
        Invoke-Expression $build_command

        if (!(Test-Path("./Build/MSBuild/Output/$($workspace_name).exe"))) {
            Write-Color "✘ Build failed." Red
            exit 1
        }
        Write-Color -text "✔ Build completed. (", "$($args[1])", ")" -color Green, Cyan, Green
        exit 0
    }

    # 'msbuild-release'
    elseif ($args[1] -eq "msbuild-release") {
        if ($disable_msbuild_tasks -eq $true) {
            Write-Color "MSBuild tasks are disabled." DarkGray
            exit 0
        }

        # Create build directories.
        if (!(Test-Path("./Build"))) {
            Write-Color -Text "Creating '", "Build", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build | Out-Null
        }
        if (!(Test-Path("./Build/MSBuild"))) {
            Write-Color -Text "Creating '", "MSBuild", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build/MSBuild | Out-Null
        }
        if (Test-Path("./Build/MSBuild/Output")) {
            Remove-Item ./Build/MSBuild/Output/*.exe
        }

        $build_flags = "-noLogo /p:Configuration=Release /p:Platform=x64"
        Write-Color -text "Build flags: ", "$($build_flags)" -color Yellow, Cyan
        
        $build_command = "msbuild $($workspace_name).sln $($build_flags)"
        Invoke-Expression $build_command

        if (!(Test-Path("./Build/MSBuild/Output/$($workspace_name).exe"))) {
            Write-Color "✘ Build failed." Red
            exit 1
        }
        Write-Color -text "✔ Build completed. (", "$($args[1])", ")" -color Green, Cyan, Green
        exit 0
    }

    # Invalid build option.
    else {
        Write-Color "Invalid build option." Red
        exit 1
    }
}

# [Other tasks]
elseif ($task_name -match "clean") {
    # Print clean options
    if (($args.Count -eq 1) -or ($args[1] -match "\?") -or ($args[1] -match "help")) {
        Write-Color "Build options:" Yellow
        Write-Color -text "all", ": Clean all build files." -color Cyan, White
        Write-Color -text "mingw64", ": Clean Mingw64 build files." -color Cyan, White
        Write-Color -text "msbuild", ": Clean MSBuild build files." -color Cyan, White
        Write-Color -text "Example: ", "./task.ps1 clean all" -color Yellow, DarkGray
        exit 0
    }

    # 'all'
    if ($args[1] -match "all") {
        if (!(Test-Path("./Build"))) {
            Write-Color "No build files exist." DarkGray
            exit 0
        }
        Write-Color "Cleaned build files." Green
        Remove-Item -path "./Build" -recurse
        exit 0
    }

    # 'mingw64'
    elseif ($args[1] -match "mingw64") {
        if ((!(Test-Path("./Build"))) -or (!(Test-Path("./Build/Mingw64")))) {
            Write-Color "No build files exist." DarkGray
            exit 0
        }
        Write-Color "Cleaned build files." Green
        Remove-Item ./Build/Mingw64/* -recurse
        exit 0
    }

    # 'msbuild'
    elseif ($args[1] -match "msbuild") {
        if ((!(Test-Path("./Build"))) -or (!(Test-Path("./Build/MSBuild")))) {
            Write-Color "No build files exist." DarkGray
            exit 0
        }
        $clean_debug_command = "msbuild $($project_file_path) -noLogo /t:Clean /p:Configuration=Debug /p:Platform=x64"
        $clean_release_command = "msbuild $($project_file_path) -noLogo /t:Clean /p:Configuration=Release /p:Platform=x64"
        Invoke-Expression $clean_debug_command
        Invoke-Expression $clean_release_command
        Write-Color "Cleaned build files." Green
        exit 0
    }

    # Invalid clean option.
    else {
        Write-Color "Invalid clean option." Red
        exit 1
    }
}

# [Invalid tasks]
else {
    Write-Color -text "Invalid task name, use '", "/?", "' to check available task names." -color Red, Cyan, Red
    exit 1
}

exit 0
