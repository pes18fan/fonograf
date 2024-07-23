@echo off
setlocal enabledelayedexpansion

set "vendor_dir=vendor"
set "miniaudio_url=https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h"
set "rogueutil_url=https://raw.githubusercontent.com/sakhmatd/rogueutil/master/rogueutil.h"

:: Check if vendor dir is present
if not exist "%vendor_dir%" (
    mkdir "%vendor_dir%"
)

:: Check for existence of miniaudio and rogueutil
if not exist "%vendor_dir%\miniaudio.h" (
    echo Downloading miniaudio.h...
    curl -o "%vendor_dir%\miniaudio.h" "%miniaudio_url%"
    if %errorlevel% neq 0 (
        echo Failed to get miniaudio.h
        exit /b 1
    )
)

if not exist "%vendor_dir%\rogueutil.h" (
    echo Downloading rogueutil.h...
    curl -o "%vendor_dir%\rogueutil.h" "%rogueutil_url%"
    if %errorlevel% neq 0 (
        echo Failed to get rogueutil.h
        exit /b 1
    )
)

:: Compile
echo Compiling...
clang++ -std=c++17 -ladvapi32 -o fonograf.exe main.cpp fonograf.cpp ui.cpp
 
if %errorlevel% neq 0 (
    echo Failed to compile.
    exit /b 1
) else (
    echo Compiled successfully!
)
