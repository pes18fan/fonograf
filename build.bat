@echo off
setlocal enabledelayedexpansion

cl.exe /Fefonograf main.cpp

if %errorlevel% neq 0 (
    echo Failed to compile.
    exit /b 1
) else (
    echo Compiled successfully!
)
