// SPDX-FileCopyrightText: 2025 Rebel Engine contributors
//
// SPDX-License-Identifier: MIT

#include "import_project.h"

#include "core/io/config_file.h"
#include "core/os/dir_access.h"
#include "core/project_settings.h"
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

void convert_physics_engine(const String& project_file) {
    const String physics_section = "physics";
    ConfigFile config_file;
    config_file.load(project_file);
    if (!config_file.has_section(physics_section)) {
        return;
    }

    const String engine_2d_key = "2d/physics_engine";
    const String engine_3d_key = "3d/physics_engine";
    const String old_value     = "GodotPhysics";
    const String new_value     = "RebelPhysics";

    if (config_file.has_section_key(physics_section, engine_2d_key)
        && config_file.get_value(physics_section, engine_2d_key) == old_value) {
        config_file.set_value(physics_section, engine_2d_key, new_value);
    }
    if (config_file.has_section_key(physics_section, engine_3d_key)
        && config_file.get_value(physics_section, engine_3d_key) == old_value) {
        config_file.set_value(physics_section, engine_3d_key, new_value);
    }

    config_file.save(project_file);
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
    const String project_file = destination_folder.plus_file("project.rebel");
    convert_physics_engine(project_file);
    return true;
}
} // namespace ImportProject
