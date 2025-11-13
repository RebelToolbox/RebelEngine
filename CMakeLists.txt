# Basic CMakeLists.txt for IDE support.
# Minimum version is set to the oldest version installed on a supported OS.
# Debian 11(Bullseye) supported until August 2026 uses CMake 3.18.
cmake_minimum_required(VERSION 3.18)
project(RebelEngine)

file(GLOB_RECURSE HEADERS **/*.h)
file(GLOB_RECURSE SOURCES
        **/*.c
        **/*.cpp
)
include_directories(.)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
