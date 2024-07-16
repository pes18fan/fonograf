@echo off
setlocal enabledelayedexpansion

clang++ -o fonograf.exe main.cpp fonograf.cpp

if %errorlevel% neq 0 (
    echo Failed to compile.
    exit /b 1
) else (
    echo Compiled successfully!
)
