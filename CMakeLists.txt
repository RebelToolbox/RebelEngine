# Basic CMakeLists.txt for IDE support.
# Minimum version is set to the oldest version installed on a supported OS.
# Debian 11(Bullseye) supported until August 2026 uses CMake 3.18.
cmake_minimum_required(VERSION 3.18)
project(RebelEngine)

file(GLOB_RECURSE project_source_files *.c *.cpp)
# Modules include third-party libraries that expect different header search paths.
add_subdirectory(modules)
add_executable(${PROJECT_NAME} ${project_source_files})
target_include_directories(${PROJECT_NAME} PRIVATE .)
