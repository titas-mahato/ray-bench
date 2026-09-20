param (
    [switch]$Run,
    [switch]$Clean,
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"

Write-Host "=========================================" -ForegroundColor Cyan
Write-Host " ray-bench: Hardware Performance Benchmark" -ForegroundColor Cyan
Write-Host "=========================================" -ForegroundColor Cyan

$ProjectDir = $PSScriptRoot

if ($Clean) {
    Write-Host "[ray-bench] Cleaning build artifacts..." -ForegroundColor Yellow
    if (Test-Path "$ProjectDir\build") { Remove-Item "$ProjectDir\build" -Recurse -Force }
    if (Test-Path "$ProjectDir\bin") { Remove-Item "$ProjectDir\bin" -Recurse -Force }
    Write-Host "[ray-bench] Clean completed." -ForegroundColor Green
    if (-not $Run) { exit 0 }
}

# Locate CMake executable
$CMakeCmd = $null
$FoundCmd = Get-Command cmake -ErrorAction SilentlyContinue
if ($FoundCmd) {
    $CMakeCmd = $FoundCmd.Source
}

if (-not $CMakeCmd) {
    # Check common Visual Studio BuildTools / Community paths
    $KnownPaths = @(
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
    )

    foreach ($p in $KnownPaths) {
        if (Test-Path $p) {
            $CMakeCmd = $p
            break
        }
    }
}

if (-not $CMakeCmd) {
    Write-Error "CMake could not be located in PATH or Visual Studio directories. Please install CMake or run from Developer PowerShell."
    exit 1
}

Write-Host "[ray-bench] Using CMake: $CMakeCmd" -ForegroundColor Gray

# Configure CMake
Write-Host "[ray-bench] Configuring CMake build system ($Config)..." -ForegroundColor Yellow
& $CMakeCmd -B "$ProjectDir\build" -S "$ProjectDir" -DCMAKE_BUILD_TYPE=$Config

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit $LASTEXITCODE
}

# Build executable
Write-Host "[ray-bench] Compiling project..." -ForegroundColor Yellow
& $CMakeCmd --build "$ProjectDir\build" --config $Config

if ($LASTEXITCODE -ne 0) {
    Write-Error "Compilation failed."
    exit $LASTEXITCODE
}

Write-Host "[ray-bench] Build successful!" -ForegroundColor Green

# Locate output binary
$ExeCandidates = @(
    "$ProjectDir\build\bin\$Config\ray-bench.exe",
    "$ProjectDir\build\bin\ray-bench.exe",
    "$ProjectDir\build\$Config\ray-bench.exe",
    "$ProjectDir\build\ray-bench.exe"
)

$TargetExe = $null
foreach ($c in $ExeCandidates) {
    if (Test-Path $c) {
        $TargetExe = $c
        break
    }
}

if ($TargetExe) {
    Write-Host "[ray-bench] Binary generated at: $TargetExe" -ForegroundColor Cyan
}

if ($Run) {
    if ($TargetExe) {
        Write-Host "[ray-bench] Launching benchmark..." -ForegroundColor Magenta
        & $TargetExe
    } else {
        Write-Error "Target executable not found to run."
        exit 1
    }
}
