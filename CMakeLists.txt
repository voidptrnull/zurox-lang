cmake_minimum_required(VERSION 3.13)
project(zurox-lang LANGUAGES C CXX)

################################
# OPTIONS
################################

set(CMAKE_LINKER lld)
#set(CMAKE_CXX_LINKER_WRAPPER_FLAG "-fuse-ld=lld")
#set(CMAKE_C_LINKER_WRAPPER_FLAG "-fuse-ld=lld")

option(NO_OUTPUT_COLOR "Disable colorized output." OFF)
option(ENABLE_TESTS "Enable testing." OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT NO_OUTPUT_COLOR)
    add_compile_options("-fdiagnostics-color=always")
endif()

################################
# ARCHITECTURE
################################

set(SUPPORTED_ARCHITECTURES x86-64 x86-64-v3 aarch64 powerpc64 native)
set(DEFAULT_ARCHITECTURE "native" CACHE STRING "Choose the target CPU architecture from ${SUPPORTED_ARCHITECTURES}.")
set_property(CACHE DEFAULT_ARCHITECTURE PROPERTY STRINGS ${SUPPORTED_ARCHITECTURES})

option(BUILD_ARCH "Specify the target CPU architecture" ${DEFAULT_ARCHITECTURE})

if (NOT BUILD_ARCH)
    set(BUILD_ARCH native)
endif()
add_compile_options(-DBUILD_ARCH="${BUILD_ARCH}") # This is to preserve build information
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


################################
# Optimizations & Stripping
################################

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDbgInfo")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
        message("-- Compiler supports IPO/LTO, enabling IPO/LTO for the build.")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO/LTO not supported, disabling it.")
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "RelWithDbgInfo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe -strip  -Wall -Werror")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pipe -strip  -Wall -Werror")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe  -Wall -Werror")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pipe -Wall -Werror")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -pipe -Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -pipe -Wall")
endif()

################################
# ZLIB DEPENENCIES
################################

find_package(ZLIB REQUIRED)

################################
# LLVM DEPENENCIES
################################

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

string(REGEX REPLACE "\n" "" LLVM_INCLUDE_DIRS "${LLVM_INCLUDE_DIRS}")

execute_process(
    COMMAND llvm-config --cxxflags --cppflags --cflags 
    OUTPUT_VARIABLE LLVM_COMPILE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\n" " " LLVM_COMPILE "${LLVM_COMPILE}")

execute_process(
    COMMAND llvm-config --ldflags --libs
    OUTPUT_VARIABLE LLVM_LINK
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "\n" " " LLVM_LINK "${LLVM_LINK}")

################################
# SOURCE FILES
################################

file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cc")
file(GLOB COMPILER_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/compiler/*.cc")
file(GLOB INTERPRETER_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/interpreter/*.cc")

################################
# ZUROXC EXECUTABLE
################################

add_executable(zuroxc ${SOURCE_FILES} ${COMPILER_SOURCE_FILES})
target_include_directories(zuroxc PRIVATE ${CMAKE_SOURCE_DIR}/include "${LLVM_INCLUDE_DIRS}")
target_compile_options(zuroxc PRIVATE ${LLVM_COMPILE})
target_link_libraries(zuroxc PRIVATE ${LLVM_LINK})

################################
# ZUROXI EXECUTABLE
################################

add_executable(zuroxi ${SOURCE_FILES} ${INTERPRETER_SOURCE_FILES})
target_include_directories(zuroxi PRIVATE ${CMAKE_SOURCE_DIR}/include "${LLVM_INCLUDE_DIRS}")
target_compile_options(zuroxi PRIVATE ${LLVM_COMPILE})
target_link_libraries(zuroxi PRIVATE ${LLVM_LINK})

################################
# TESTS
################################

if (ENABLE_TESTS)
    find_package(GTest REQUIRED)
    include(CTest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/googletest)
    
    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cc")

    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCE_FILES})
        target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include "${LLVM_INCLUDE_DIRS}" "${ICU_INCLUDE_DIRS}" ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
        target_link_libraries(${TEST_NAME} PRIVATE gtest gmock gtest_main ${LLVM_LINK} ${ICU_LIBRARIES})
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()

################################
# INSTALLATION
################################

install(TARGETS zuroxc zuroxi
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)