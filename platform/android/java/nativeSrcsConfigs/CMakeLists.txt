# Non functional cmake build file used to provide Android Studio editor support to the project.
cmake_minimum_required(VERSION 3.6)
project(RebelEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ROOT_DIR ../../../..)

# Get sources
file(GLOB_RECURSE SOURCES ${ROOT_DIR}/*.c**)
file(GLOB_RECURSE HEADERS ${ROOT_DIR}/*.h**)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
        SYSTEM PUBLIC
        ${ROOT_DIR}
        ${ROOT_DIR}/modules/gdnative/include)
