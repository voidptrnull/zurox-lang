cmake_minimum_required(VERSION 3.13)
project(zurox-lang LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(LLVM REQUIRED CONFIG)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SUPPORTED_ARCHITECTURES x86-64 x86-64-v3 aarch64 powerpc64 native)
set(DEFAULT_ARCHITECTURE "native" CACHE STRING "Choose the target CPU architecture from ${SUPPORTED_ARCHITECTURES}.")
set_property(CACHE DEFAULT_ARCHITECTURE PROPERTY STRINGS ${SUPPORTED_ARCHITECTURES})

option(BUILD_ARCH "Specify the target CPU architecture" ${DEFAULT_ARCHITECTURE})

if (NOT BUILD_ARCH)
    set(BUILD_ARCH native)
endif()

if(NOT BUILD_ARCH IN_LIST SUPPORTED_ARCHITECTURES)
    message(FATAL_ERROR "Unsupported architecture: ${BUILD_ARCH}")
endif()

if(BUILD_ARCH STREQUAL "x86-64")
    add_compile_options(-march=x86-64)
elseif(BUILD_ARCH STREQUAL "x86-64-v3")
    add_compile_options(-march=x86-64-v3)
elseif(BUILD_ARCH STREQUAL "aarch64")
    add_compile_options(-march=arm64)
elseif(BUILD_ARCH STREQUAL "powerpc64")
    add_compile_options(-march=powerpc64)
elseif(BUILD_ARCH STREQUAL "native")
    add_compile_options(-march=native)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -pipe -fPIC -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -pipe -Wall") 

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
        message("-- Compiler supports IPO/LTO, enabling IPO/LTO for the build.")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO/LTO not supported, disabling it.")
    endif()
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

list(APPEND CMAKE_PREFIX_PATH "${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include/" "${CMAKE_SOURCE_DIR}/include/parser/")
add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_executable(zurox-lang ${SOURCE_FILES})

#llvm_map_components_to_libnames(llvm_libs support core irreader)
#target_link_libraries(oxium-lang ${llvm_libs})