# Minimal CMakeLists.txt to provide users of Android Studio access to the Rebel source files.
cmake_minimum_required(VERSION 3.6)
project(RebelEngine)

# Add all Rebel source files
set(ROOT_DIR ../../../..)
file(GLOB_RECURSE SOURCES ${ROOT_DIR}/*.c**)
file(GLOB_RECURSE HEADERS ${ROOT_DIR}/*.h**)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
