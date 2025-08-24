cmake_minimum_required(VERSION 3.28)
project(cvector VERSION 1.0.0 LANGUAGES CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prefer Clang for better C++23 module support
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using Clang compiler: ${CMAKE_CXX_COMPILER_VERSION}")
    
    # Enable C++23 modules for Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcxx-modules")
    
else()
    message(WARNING "Clang is recommended for C++23 modules. Current compiler: ${CMAKE_CXX_COMPILER_ID}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts")
endif()

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

# Create cvector module
add_library(cvector_module)
target_sources(cvector_module 
    PUBLIC 
    FILE_SET CXX_MODULES FILES cvector_module.cpp
)
target_compile_features(cvector_module PRIVATE cxx_std_23)

# Set module properties
set_target_properties(cvector_module PROPERTIES
    CXX_MODULE_STD_INTERFACE ON
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(cvector_module PRIVATE
        -fcxx-modules
    )
endif()

# Create test executable
add_executable(cvector_test test_cvector.cpp)
target_link_libraries(cvector_test PRIVATE cvector_module)

# Set output directories
set_target_properties(cvector_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install targets
install(TARGETS cvector_module cvector_test
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Print configuration info
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
