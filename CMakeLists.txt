cmake_minimum_required(VERSION 3.20)

project(
  jkds
  VERSION 0.1.0
  DESCRIPTION "A set of data structures written in modern C++"
  LANGUAGES CXX)

message(STATUS "${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION}")

option(jkds_test "Build tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(JKDS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(JKDS_TESTS_DIR "${CMAKE_SOURCE_DIR}/test")

# Control where libraries and executables are placed during the build.
# With the following settings executables are placed in <the top level of the
# build tree>/bin and libraries/archives in <top level of the build tree>/lib.
# This is particularly useful to run ctests on libraries built on Windows
# machines: tests, which are executables, are placed in the same folders of
# dlls, which are treated as executables as well, so that they can properly
# find the libraries to run. This is a because of missing RPATH on Windows.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Under MSVC, we set CMAKE_DEBUG_POSTFIX to "d" to add a trailing "d" to library
# built in debug mode. In this Windows user can compile, build and install the
# library in both Release and Debug configuration avoiding naming clashes in the
# installation directories.
if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

add_library(jkds INTERFACE)
target_include_directories(jkds INTERFACE ${JKDS_INCLUDE_DIR})

if(jkds_test)
  include(CTest)
	enable_testing()
	add_subdirectory("${JKDS_TESTS_DIR}")
endif()
