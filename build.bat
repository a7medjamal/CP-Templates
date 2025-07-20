@echo off
REM Build script for competitive programming templates
REM Usage: build.bat <source_file> [output_name]

setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Usage: build.bat ^<source_file^> [output_name]
    echo Example: build.bat main.cpp
    echo Example: build.bat main.cpp solution
    exit /b 1
)

set SOURCE=%~1
set OUTPUT=%~2

REM Check if source file exists
if not exist "%SOURCE%" (
    echo Error: Source file "%SOURCE%" not found!
    exit /b 1
)

REM Set output name
if "%OUTPUT%"=="" (
    for %%f in ("%SOURCE%") do set OUTPUT=%%~nf
)

REM Compilation flags for competitive programming
set CFLAGS=-std=c++17 -O2 -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -DLOCAL

echo Compiling %SOURCE%...
g++ %CFLAGS% -o "%OUTPUT%.exe" "%SOURCE%"

if %ERRORLEVEL% equ 0 (
    echo Compilation successful! Output: %OUTPUT%.exe
    echo.
    echo To run with input file: %OUTPUT%.exe ^< input.txt
    echo To run with output file: %OUTPUT%.exe ^< input.txt ^> output.txt
    echo To run interactively: %OUTPUT%.exe
) else (
    echo Compilation failed!
    exit /b 1
)

endlocal