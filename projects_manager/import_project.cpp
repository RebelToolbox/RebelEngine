// SPDX-FileCopyrightText: 2025 Rebel Engine contributors
//
// SPDX-License-Identifier: MIT

#include "import_project.h"

#include "core/os/dir_access.h"
#include "core/ustring.h"

namespace {
bool create_rebel_project_from_godot_project(
    const String& source_project_file,
    const String& destination_folder
) {
    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (!dir_access->file_exists(source_project_file)) {
        memdelete(dir_access);
        ERR_FAIL_V_MSG(false, "Source project file does not exist.");
    }

    if (!dir_access->dir_exists(destination_folder)) {
        Error error = dir_access->make_dir_recursive(destination_folder);
        if (error != OK) {
            memdelete(dir_access);
            ERR_FAIL_V_MSG(
                false,
                "Could not create destination folder: " + destination_folder
            );
        }
    }
    dir_access->change_dir(destination_folder);

    const String source_folder = source_project_file.get_base_dir();
    Error error = dir_access->copy_dir(source_folder, destination_folder);
    if (error != OK) {
        memdelete(dir_access);
        ERR_FAIL_V_MSG(
            false,
            "Could not copy " + source_folder + " to " + destination_folder
        );
    }

    error = dir_access->rename("project.godot", "project.rebel");
    if (error != OK) {
        memdelete(dir_access);
        ERR_FAIL_V_MSG(
            false,
            "Could not rename project.godot to project.rebel"
        );
    }

    memdelete(dir_access);
    return true;
}
} // namespace

namespace ImportProject {
bool import_godot_project(
    const String& source_project_file,
    const String& destination_folder
) {
    ERR_FAIL_COND_V(
        !create_rebel_project_from_godot_project(
            source_project_file,
            destination_folder
        ),
        false
    );
    return true;
}
} // namespace ImportProject
