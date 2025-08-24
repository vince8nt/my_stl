# CVVector with C++23 Modules

This directory contains a modernized version of cvector using C++23 modules with CMake + Ninja + LLVM/Clang.

## Files

- `cvector_module.cpp` - The cvector implementation as a C++23 module
- `test_cvector.cpp` - Test program that imports and uses the cvector module
- `build.sh` - Legacy build script for GCC (requires GCC 15+)
- `build_cmake.sh` - Modern build script using CMake + Ninja + LLVM
- `CMakeLists.txt` - CMake configuration for the project
- `README_modules.md` - This file

## Requirements

### CMake + Ninja + LLVM/Clang (Recommended)
- **CMake 3.28+** with C++23 module support
- **Ninja** build system
- **LLVM/Clang** compiler (better C++23 module support than GCC)
- Linux/Unix/macOS environment

### GCC 15+ (Legacy)
- **GCC 15+** with C++23 module support
- Linux/Unix environment

## Quick Start

### 1. Install Dependencies

```bash
# Install CMake 3.28+, Ninja, and LLVM/Clang
sudo apt update
sudo apt install -y cmake ninja-build clang lld
```

### 2. Build with CMake + Ninja + LLVM

```bash
# Build using the modern build system
chmod +x build_cmake.sh
./build_cmake.sh

# Run the module version
./build/bin/cvector_test
```

### 3. Alternative: Build with GCC (if you have GCC 15+)

```bash
# Use the legacy build script
chmod +x build.sh
./build.sh
./cvector_test
```

## Build Systems

### Method 1: CMake + Ninja + LLVM (Recommended)

This is the modern approach with better C++23 module support:

```bash
# Configure and build
mkdir build && cd build
cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ ..
ninja

# Run tests
./bin/cvector_test
```

**Advantages:**
- Better C++23 module support in LLVM/Clang
- Faster builds with Ninja
- Cross-platform compatibility
- Proper dependency management

### Method 2: Manual CMake Build

```bash
# Create build directory
mkdir build && cd build

# Configure with specific options
cmake -G Ninja \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=23 \
    -DCMAKE_CXX_STANDARD_REQUIRED=ON \
    ..

# Build
ninja
```

### Method 3: Legacy GCC Build

```bash
# Compile the module
g++ -std=c++23 -fmodules-ts -O2 -Wall -Wextra -c cvector_module.cpp -o cvector_module.o

# Compile and link the test
g++ -std=c++23 -fmodules-ts -O2 -Wall -Wextra cvector_module.o test_cvector.cpp -o cvector_test

# Run the test
./cvector_test
```

## Key Compiler Flags for Modules

### LLVM/Clang
- `-std=c++23` - Enable C++23 standard
- `-fcxx-modules` - Enable module support

### GCC
- `-std=c++23` - Enable C++23 standard
- `-fmodules-ts` - Enable module support (experimental)

## Module Features

### Module Declaration
```cpp
module;                    // Global module fragment
// Traditional includes in global module fragment
#include <type_traits>
#include <memory>
#include <cstring>  // for memcpy
// ... other includes
export module cvector;     // Export our module
```

### Usage
```cpp
#include <iostream>
#include <algorithm>
import cvector;           // Import our cvector module

using namespace containers;
cvector<int> vec;         // Use the exported class
```

## Tests Included

1. **Basic Operations** - Construction, push/pop, element access
2. **Iterators** - Forward, reverse, range-based for loops
3. **Iterator Stability** - Verifies iterators remain valid across push operations
4. **Standard Algorithms** - Tests compatibility with std::sort, std::find, etc.

## Benefits of Modules

1. **Faster compilation** - No header parsing
2. **Better encapsulation** - Only exported symbols are visible
3. **Reduced dependencies** - Clear import/export boundaries
4. **Modern C++** - Uses latest language features

## Troubleshooting

### Common Issues

1. **"failed to read compiled module: No such file or directory"**
   - Solution: Use LLVM/Clang instead of GCC, or upgrade to GCC 15+

2. **"CMake version 3.28 or higher is required"**
   - Solution: Install newer CMake: `sudo apt install cmake`

3. **"ninja: command not found"**
   - Solution: Install Ninja: `sudo apt install ninja-build`

4. **"clang++: command not found"**
   - Solution: Install LLVM/Clang: `sudo apt install clang lld`

5. **"module 'cvector' not found"**
   - Solution: Ensure the module is marked as PUBLIC in CMakeLists.txt

### Version Requirements

- **CMake**: 3.28+ (for C++23 module support)
- **Ninja**: Any recent version
- **LLVM/Clang**: 15+ (for better C++23 module support)
- **GCC**: 15+ (if using GCC build system)

## Notes

- LLVM/Clang has better C++23 module support than GCC
- The CMake build system properly handles module compilation
- Uses global module fragment for traditional includes (more compatible)
- The build system supports cross-platform development
- Successfully tested with Clang 19.1.7 on Debian
- All tests pass successfully with proper iterator stability and algorithm compatibility
