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
    [Parameter(Mandatory=$true)][string[]]$Text,
    [Parameter(Mandatory=$true)][ConsoleColor[]]$Color
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

# Print usage texts.
if (($args.Count -eq 0) -or ($args[0] -match "\?") -or ($args[0] -match "help")) {
    Write-Color -text "Available tasks: ", "build clean" -color Yellow, Cyan
    Write-Color -text "Task options: ", "Use ", "/? ", "(Example: ./script.ps1 build /?)" -color Yellow, White, Cyan, White
    Write-Color -text "Example: ", "./script.ps1 <task name> <task option 1> <task option 2> <...>" -color Yellow, DarkGray
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
    # Print task options
    if (($args.Count -eq 1) -or ($args[1] -match "\?") -or ($args[1] -match "help")) {
        Write-Color "Task options:" Yellow
        Write-Color -text "mingw64-debug", ": Build the executable using MinGW64 compiler. (Debug)" -color Cyan, White
        Write-Color -text "mingw64-release", ": Build the executable using MinGW64 compiler. (Release)" -color Cyan, White
        Write-Color -text "msbuild-debug", ": Build the executable using MSBuild build tool. (Debug)" -color Cyan, White
        Write-Color -text "msbuild-release", ": Build the executable using MSBuild build tool. (Release)" -color Cyan, White
        Write-Color -text "Example: ", "./script.ps1 build mingw64-release" -color Yellow, DarkGray
        exit 0
    }

    # 'mingw64-debug'
    if ($args[1] -eq "mingw64-debug") {
        # Create build directories.
        if (!(Test-Path("./Build"))) {
            Write-Color -Text "Creating '", "Build", "' folder ..." -color DarkGray, Cyan, DarkGray
            New-Item -ItemType Directory -Path ./Build | Out-Null

            if (!(Test-Path("./Build/MinGW"))) {
                Write-Color -Text "Creating '", "MinGW", "' folder ..." -color DarkGray, Cyan, DarkGray
                New-Item -ItemType Directory -Path ./Build/MinGW | Out-Null
            }
        }

        # Clean build files.
        if (Test-Path("./Build/MinGW")) {
            Remove-Item ./Build/MinGW/* -recurse
        }

        # Build resource file.
        Write-Color "Building resource file ..." White
        windres "./Resources/resources.rc" "./Build/MinGW/resources.o"
        if (!(Test-Path("./Build/MinGW/resources.o"))) {
            Write-Color "Failed to build the resource file." Red
            exit 1
        }
        Write-Color "Resource file built successfully.`n" Green
        
        # Compile the executable.
        Write-Color "Compile flags:" Yellow
        $compile_flags = 
        "-Wall -m64 -std=c++17 -municode -mwindows -fdiagnostics-color=always -finput-charset=utf-8 -fexec-charset=utf-8 " +
        "-g -Og Sources/*.cpp Build/MinGW/resources.o -o Build/MinGW/$($workspace_name).exe " +
        "-Wl,-Bdynamic -luxtheme -lcomctl32 -ldwmapi -lwinmm -lpowrprof -ld2d1 -ldwrite -lwindowscodecs " +
        "-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32"
        Write-Color "$($compile_flags)`n" Cyan
        
        Write-Color "[Build in progress, please wait...]" DarkYellow
        g++ $($compile_flags)
        
        if (!(Test-Path("./Build")))
        exit 0
    }

    # 'mingw64-release'
    elseif ($args[1] -eq "mingw64-release") {
        Write-Color "Not implemented yet." DarkGray
        exit 0
    }

    # 'msbuild-debug'
    elseif ($args[1] -eq "msbuild-debug") {
        Write-Color "Not implemented yet." DarkGray
        exit 0
    }

    # 'msbuild-release'
    elseif ($args[1] -eq "msbuild-release") {
        Write-Color "Not implemented yet." DarkGray
        exit 0
    }

    # Invalid build option.
    else {
        Write-Color "Invalid build option." Red
        exit 1
    }
}

# [Invalid tasks]
else {
    Write-Color -text "Invalid task name, use '", "/?", "' to check available task names." -color Red, Cyan, Red
    exit 1
}

exit 0
