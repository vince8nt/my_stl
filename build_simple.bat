@echo off
REM Simplified build script for cvector with C++23 modules
REM Uses traditional includes but still exports as a module
REM Requires GCC 14+ with basic module support

echo Building cvector with simplified modules...

REM Compiler and flags
set CXX=g++
set CXXFLAGS=-std=c++23 -fmodules-ts -O2 -Wall -Wextra

REM Clean previous builds
echo Cleaning previous builds...
if exist *.gcm del *.gcm
if exist *.o del *.o
if exist cvector_simple.exe del cvector_simple.exe

REM Step 1: Compile the cvector module (traditional includes version)
echo Compiling cvector module...
%CXX% %CXXFLAGS% -c cvector_traditional.cpp -o cvector_traditional.o
if errorlevel 1 (
    echo Error compiling cvector module
    exit /b 1
)

REM Step 2: Compile and link the test program
echo Compiling test program...
%CXX% %CXXFLAGS% cvector_traditional.o test_simple.cpp -o cvector_simple.exe
if errorlevel 1 (
    echo Error compiling test program
    exit /b 1
)

echo Build completed successfully!
echo Run with: cvector_simple.exe
