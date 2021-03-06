# Version check
cmake_minimum_required (VERSION 3.8)

if(NOT "${CMAKE_CXX_STANDARD}")
	set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  endif()
endif()

set(proj "nana-dialog-maker")

# Project
project(${proj})

# Add files
file(GLOB sources "source/nana-dialog-maker/*.cpp" "source/nana-dialog-maker/generators/*.cpp")

# Add library
add_library(${proj} STATIC ${sources})

target_include_directories(${proj} PRIVATE "include")
target_include_directories(${proj} PRIVATE "../nana/include")
target_include_directories(${proj} PRIVATE "../fmt/include")

find_library(LNANA nana PATHS "../nana/release")
find_library(LNANA nana PATHS "../fmt/build")

# Compiler Options
set(DEBUG_OPTIONS -fexceptions -g -Wall -pedantic-errors -pedantic)
set(RELEASE_OPTIONS -fexceptions -O3 -Wall -pedantic-errors -pedantic)
target_compile_options(${proj} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
target_compile_options(${proj} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")