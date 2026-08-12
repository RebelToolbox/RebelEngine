# CMakeLists.txt used for IDE support.
# Third-party libraries have different header search paths.

file(GLOB_RECURSE module_source_files *.cpp)

# We need to remove the module source files from the project source files.
set(project_source_files_without_module_source_files ${parent_source_files})
foreach (file ${module_source_files})
    list(REMOVE_ITEM project_source_files_without_module_source_files ${file} PARENT_SCOPE)
endforeach ()
# Apply changes to the project source files to the parent directory.
set(parent_source_files ${project_source_files_without_module_source_files} PARENT_SCOPE)

add_library(minimp3 EXCLUDE_FROM_ALL ${module_source_files})
target_include_directories(minimp3 PRIVATE ${CMAKE_SOURCE_DIR})
# Add third-party library's header search paths.
target_include_directories(minimp3 PRIVATE ${CMAKE_SOURCE_DIR}/third-party/minimp3)
