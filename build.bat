@echo off
setlocal enabledelayedexpansion

REM Create vendor dir if it doesn't exist
if not exist vendor (
    mkdir vendor
)

REM Download with curl
curl -fS -o vendor/miniaudio.h https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h

if %errorlevel% neq 0 (
    echo Failed to download deps.
    exit /b 1
) else (
    echo Successfully downloaded deps.
)

cl.exe /Fefonograf main.cpp

if %errorlevel% neq 0 (
    echo Failed to compile.
    exit /b 1
) else (
    echo Compiled successfully!
)
