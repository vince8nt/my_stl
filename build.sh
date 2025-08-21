#!/bin/bash

# Build script for cvector with C++23 modules
# Requires GCC 15+ with module support

set -e

echo "Building cvector with C++23 modules..."

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++23 -fmodules-ts -O2 -Wall -Wextra"

# Clean previous builds
echo "Cleaning previous builds..."
rm -f *.gcm *.o cvector_test

# Step 1: Build the standard library module interface (if needed)
# Note: This might not be necessary depending on GCC version
echo "Preparing standard library modules..."

# Step 2: Compile the cvector module
echo "Compiling cvector module..."
$CXX $CXXFLAGS -c cvector_module.cpp -o cvector_module.o

# Step 3: Compile and link the test program
echo "Compiling test program..."
$CXX $CXXFLAGS cvector_module.o test_cvector.cpp -o cvector_test

echo "Build completed successfully!"
echo "Run with: ./cvector_test"
