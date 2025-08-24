#!/bin/bash

# Build script for cvector using CMake with Ninja and LLVM/Clang
# Requires CMake 3.28+, Ninja, and LLVM/Clang

set -e

echo "Building cvector with CMake + Ninja + LLVM/Clang..."

# Check for required tools
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        echo "Error: $1 is not installed or not in PATH"
        echo "Please install $1 and try again"
        exit 1
    fi
}

echo "Checking required tools..."
check_tool cmake
check_tool ninja
check_tool clang++

# Get versions
CMAKE_VERSION=$(cmake --version | head -n1)
NINJA_VERSION=$(ninja --version)
CLANG_VERSION=$(clang++ --version | head -n1)

echo "CMake: $CMAKE_VERSION"
echo "Ninja: $NINJA_VERSION"
echo "Clang: $CLANG_VERSION"

# Create build directory
BUILD_DIR="build"
echo "Creating build directory: $BUILD_DIR"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Configure with CMake
echo "Configuring with CMake..."
cd "$BUILD_DIR"
cmake -G Ninja \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=23 \
    -DCMAKE_CXX_STANDARD_REQUIRED=ON \
    ..

# Build
echo "Building with Ninja..."
ninja

echo ""
echo "Build completed successfully!"
echo "Executable is in: $BUILD_DIR/bin/"
echo ""
echo "To run the module version:"
echo "  ./$BUILD_DIR/bin/cvector_test"
