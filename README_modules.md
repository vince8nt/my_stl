# CVVector with C++23 Modules

This directory contains a modernized version of cvector using C++23 modules.

## Files

- `cvector_module.cpp` - The cvector implementation as a C++23 module
- `test_cvector.cpp` - Test program that imports and uses the cvector module
- `build.sh` - Build script for compilation
- `README_modules.md` - This file

## Requirements

- **GCC 15+** with C++23 module support
- Linux/Unix environment (for the build script)

## Compilation

### Method 1: Using the build script (recommended)

```bash
chmod +x build.sh
./build.sh
./cvector_test
```

### Method 2: Manual compilation

```bash
# Compile the module
g++ -std=c++23 -fmodules-ts -O2 -Wall -Wextra -c cvector_module.cpp -o cvector_module.o

# Compile and link the test
g++ -std=c++23 -fmodules-ts -O2 -Wall -Wextra cvector_module.o test_cvector.cpp -o cvector_test

# Run the test
./cvector_test
```

## Key GCC Flags for Modules

- `-std=c++23` - Enable C++23 standard
- `-fmodules-ts` - Enable module support (experimental in GCC)

## Module Features

### Module Declaration
```cpp
module;                    // Global module fragment
import std;               // Import standard library
export module cvector;    // Export our module
```

### Usage
```cpp
import std;               // Import standard library
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

## Notes

- Module support in GCC is still experimental as of GCC 15
- Some features may require newer compiler versions
- The build process is different from traditional header-based compilation
