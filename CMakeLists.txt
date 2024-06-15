cmake_minimum_required(VERSION 3.13)
project(zurox-lang LANGUAGES C CXX)

set(CMAKE_LINKER lld)
set(CMAKE_CXX_LINKER_WRAPPER_FLAG "-fuse-ld=lld")
set(CMAKE_C_LINKER_WRAPPER_FLAG "-fuse-ld=lld")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=lld")

option(NO_OUTPUT_COLOR "Disable colorized output." OFF)
option(ENABLE_TESTS "Enable testing." OFF)

set(CMAKE_CXX_STANDARD 17)cmake_minimum_required(VERSION 3.13)
project(zurox-lang LANGUAGES C CXX)

set(CMAKE_LINKER lld)
set(CMAKE_CXX_LINKER_WRAPPER_FLAG "-fuse-ld=lld")
set(CMAKE_C_LINKER_WRAPPER_FLAG "-fuse-ld=lld")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=lld")

option(NO_OUTPUT_COLOR "Disable colorized output." OFF)
option(ENABLE_TESTS "Enable testing." OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT NO_OUTPUT_COLOR)
    add_compile_options("-fdiagnostics-color=always")
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

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cc")
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/main.cc") # Exclude main.cc

add_executable(zurox-lang ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/src/main.cc) # Add main.cc explicitly for the main executable

# Ensure the LLVM include directories are correctly set
string(REGEX REPLACE "\n" "" LLVM_INCLUDE_DIRS "${LLVM_INCLUDE_DIRS}")
target_include_directories(zurox-lang PRIVATE ${CMAKE_SOURCE_DIR}/include "${LLVM_INCLUDE_DIRS}")

execute_process(
    COMMAND llvm-config --cxxflags --cppflags --cflags 
    OUTPUT_VARIABLE LLVM_COMPILE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\n" " " LLVM_COMPILE "${LLVM_COMPILE}")
target_compile_options(zurox-lang PRIVATE ${LLVM_COMPILE})

execute_process(
    COMMAND llvm-config --ldflags --libs
    OUTPUT_VARIABLE LLVM_LINK
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\n" " " LLVM_LINK "${LLVM_LINK}")
target_link_libraries(zurox-lang PRIVATE ${LLVM_LINK})

if (ENABLE_TESTS)
    find_package(GTest REQUIRED)
    include(CTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/googletest)
    
    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cc")
    
    add_executable(TESTS ${TEST_SOURCES} ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/tests/tmain.cc) # Add tmain.cc explicitly for the test executable
    
    target_include_directories(TESTS PRIVATE ${CMAKE_SOURCE_DIR}/include "${LLVM_INCLUDE_DIRS}" ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
    target_link_libraries(TESTS PRIVATE gtest gmock gtest_main ${LLVM_LINK})
    
    add_test(NAME zurox-tests COMMAND TESTS)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT NO_OUTPUT_COLOR)
    add_compile_options("-fdiagnostics-color=always")
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

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cc")
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/main.cc") # Exclude main.cc

add_executable(zurox-lang ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/src/main.cc) # Add main.cc explicitly for the main executable

# Ensure the LLVM include directories are correctly set
string(REGEX REPLACE "\n" "" LLVM_INCLUDE_DIRS "${LLVM_INCLUDE_DIRS}")
target_include_directories(zurox-lang PRIVATE ${CMAKE_SOURCE_DIR}/include "${LLVM_INCLUDE_DIRS}")

execute_process(
    COMMAND llvm-config --cxxflags --cppflags --cflags 
    OUTPUT_VARIABLE LLVM_COMPILE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\n" " " LLVM_COMPILE "${LLVM_COMPILE}")
target_compile_options(zurox-lang PRIVATE ${LLVM_COMPILE})

execute_process(
    COMMAND llvm-config --ldflags --libs
    OUTPUT_VARIABLE LLVM_LINK
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\n" " " LLVM_LINK "${LLVM_LINK}")
target_link_libraries(zurox-lang PRIVATE ${LLVM_LINK})

if (ENABLE_TESTS)
    find_package(GTest REQUIRED)
    include(CTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/googletest)
    
    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cc")

    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/tests/tmain.cc) # Add tmain.cc explicitly for the test executable
        target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include "${LLVM_INCLUDE_DIRS}" ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
        target_link_libraries(${TEST_NAME} PRIVATE gtest gmock gtest_main ${LLVM_LINK})
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()
