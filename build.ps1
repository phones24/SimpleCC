# SimpleCC VST3 Plugin Build Script for Windows
# Usage: .\build.ps1 [Release|Debug]

param(
    [Parameter(Position=0)]
    [ValidateSet('Release', 'Debug')]
    [string]$BuildType = 'Release'
)

$ErrorActionPreference = "Stop"

$BUILD_DIR = "build"

Write-Host "=== SimpleCC VST3 Plugin Build ===" -ForegroundColor Cyan
Write-Host "Build type: $BuildType"

# Check if CMake is installed
Write-Host "Checking dependencies..."
try {
    $cmakeVersion = cmake --version 2>&1 | Select-Object -First 1
    Write-Host "  Found: $cmakeVersion" -ForegroundColor Green
} catch {
    Write-Host "Error: CMake is required but not installed." -ForegroundColor Red
    Write-Host "Download from: https://cmake.org/download/" -ForegroundColor Yellow
    exit 1
}

# Check if Visual Studio or Build Tools are installed
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
    $vsPath = & $vswhere -latest -property installationPath
    if ($vsPath) {
        Write-Host "  Found Visual Studio: $vsPath" -ForegroundColor Green
    } else {
        Write-Host "Warning: Visual Studio not found via vswhere" -ForegroundColor Yellow
    }
} else {
    Write-Host "Warning: Visual Studio Installer not found" -ForegroundColor Yellow
    Write-Host "  CMake will attempt to find compiler automatically" -ForegroundColor Yellow
}

# Check if Git is installed
try {
    $gitVersion = git --version 2>&1
    Write-Host "  Found: $gitVersion" -ForegroundColor Green
} catch {
    Write-Host "Error: Git is required but not installed." -ForegroundColor Red
    Write-Host "Download from: https://git-scm.com/download/win" -ForegroundColor Yellow
    exit 1
}

# Create build directory
Write-Host ""
Write-Host "Creating build directory..."
if (-not (Test-Path $BUILD_DIR)) {
    New-Item -ItemType Directory -Path $BUILD_DIR | Out-Null
}

Set-Location $BUILD_DIR

# Configure
Write-Host "Configuring..."
try {
    cmake .. -DCMAKE_BUILD_TYPE="$BuildType"
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed"
    }
} catch {
    Write-Host "Error during CMake configuration: $_" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Build
Write-Host "Building..."
try {
    $numCores = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors
    cmake --build . --config "$BuildType" --parallel $numCores
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }
} catch {
    Write-Host "Error during build: $_" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Set-Location ..

Write-Host ""
Write-Host "=== Build Complete ===" -ForegroundColor Green

# Find VST3 plugin
$vst3Path = Get-ChildItem -Path "$BUILD_DIR" -Recurse -Filter "SimpleCC.vst3" -Directory | Select-Object -First 1

if ($vst3Path) {
    Write-Host "VST3 plugin location:" -ForegroundColor Cyan
    Write-Host "  $($vst3Path.FullName)" -ForegroundColor White
    
    # Determine VST3 install location
    $vst3InstallPath = "$env:COMMONPROGRAMFILES\VST3"
    
    Write-Host ""
    Write-Host "To install, copy the .vst3 folder to:" -ForegroundColor Yellow
    Write-Host "  $vst3InstallPath" -ForegroundColor White
    Write-Host ""
    Write-Host "Or run the following command as Administrator:" -ForegroundColor Yellow
    Write-Host "  Copy-Item -Path '$($vst3Path.FullName)' -Destination '$vst3InstallPath' -Recurse -Force" -ForegroundColor Cyan
} else {
    Write-Host "Warning: VST3 plugin not found in build directory" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Build script completed successfully!" -ForegroundColor Green
