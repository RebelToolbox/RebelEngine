// SPDX-FileCopyrightText: 2025 Rebel Engine contributors
//
// SPDX-License-Identifier: MIT

#ifndef IMPORT_PROJECT_H
#define IMPORT_PROJECT_H

class String;

namespace ImportProject {
bool import_godot_project(
    const String& project_file,
    const String& destination_folder
);
} // namespace ImportProject

#endif // IMPORT_PROJECT_H
