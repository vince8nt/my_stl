#!/bin/bash

# Build script for cvector with C++23 modules
# Requires GCC 15+ with module support

set -e

echo "Building cvector with C++23 modules..."

# Check GCC version
GCC_VERSION=$(g++ -dumpversion)
GCC_MAJOR=$(echo $GCC_VERSION | cut -d. -f1)

echo "Detected GCC version: $GCC_VERSION"

if [ "$GCC_MAJOR" -lt 15 ]; then
    echo "Warning: GCC 15+ is required for C++23 modules with std import."
    echo "Current version: $GCC_VERSION"
    echo ""
    echo "Options:"
    echo "1. Use traditional compilation (no modules)"
    echo "2. Install GCC 15+"
    echo "3. Try experimental module compilation (may fail)"
    echo ""
    read -p "Choose option (1-3): " choice
    
    case $choice in
        1)
            echo "Building with traditional compilation..."
            g++ -std=c++23 -O2 -Wall -Wextra cvector_traditional.cpp test_cvector.cpp -o cvector_test
            echo "Build completed successfully!"
            echo "Run with: ./cvector_test"
            exit 0
            ;;
        2)
            echo "Please install GCC 15+ and try again."
            echo "On Debian/Ubuntu: sudo apt install g++-15"
            exit 1
            ;;
        3)
            echo "Attempting experimental module compilation..."
            ;;
        *)
            echo "Invalid choice. Exiting."
            exit 1
            ;;
    esac
fi

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++23 -fmodules-ts -O2 -Wall -Wextra"

# Clean previous builds
echo "Cleaning previous builds..."
rm -f *.gcm *.o cvector_test
rm -rf gcm.cache

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
