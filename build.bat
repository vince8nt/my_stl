@echo off
REM Build script for cvector with C++23 modules
REM Requires GCC 15+ with module support (MinGW-w64 or similar)

echo Building cvector with C++23 modules...

REM Compiler and flags
set CXX=g++
set CXXFLAGS=-std=c++23 -fmodules-ts -O2 -Wall -Wextra

REM Clean previous builds
echo Cleaning previous builds...
if exist *.gcm del *.gcm
if exist *.o del *.o
if exist cvector_test.exe del cvector_test.exe

REM Step 1: Build standard library module
echo Building standard library module...
echo. | %CXX% %CXXFLAGS% -std=c++23 -O2 -fmodules -fsearch-include-path -fmodule-only -c bits/std.cc

REM Step 2: Compile the cvector module
echo Compiling cvector module...
%CXX% %CXXFLAGS% -c cvector_module.cpp -o cvector_module.o
if errorlevel 1 (
    echo Error compiling cvector module
    exit /b 1
)

REM Step 3: Compile and link the test program
echo Compiling test program...
%CXX% %CXXFLAGS% cvector_module.o test_cvector.cpp -o cvector_test.exe
if errorlevel 1 (
    echo Error compiling test program
    exit /b 1
)

echo Build completed successfully!
echo Run with: cvector_test.exe
