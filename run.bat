@echo off
setlocal
cd /d "%~dp0"
title ray-bench Launcher

if exist "build\bin\ray-bench.exe" (
    start "" "build\bin\ray-bench.exe"
    exit /b 0
)

if exist "build\bin\Release\ray-bench.exe" (
    start "" "build\bin\Release\ray-bench.exe"
    exit /b 0
)

echo [ray-bench] Executable not found. Compiling now...
powershell -ExecutionPolicy Bypass -File .\build.ps1 -Run
