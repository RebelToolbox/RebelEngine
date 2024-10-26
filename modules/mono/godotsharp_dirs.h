// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOTSHARP_DIRS_H
#define GODOTSHARP_DIRS_H

#include "core/ustring.h"

namespace GodotSharpDirs {

String get_res_data_dir();
String get_res_metadata_dir();
String get_res_assemblies_base_dir();
String get_res_assemblies_dir();
String get_res_config_dir();
String get_res_temp_dir();
String get_res_temp_assemblies_base_dir();
String get_res_temp_assemblies_dir();

String get_mono_user_dir();
String get_mono_logs_dir();

#ifdef TOOLS_ENABLED
String get_mono_solutions_dir();
String get_build_logs_dir();

String get_project_sln_path();
String get_project_csproj_path();

String get_data_editor_tools_dir();
String get_data_editor_prebuilt_api_dir();
#else
String get_data_game_assemblies_dir();
#endif

String get_data_mono_etc_dir();
String get_data_mono_lib_dir();

#ifdef WINDOWS_ENABLED
String get_data_mono_bin_dir();
#endif

} // namespace GodotSharpDirs

#endif // GODOTSHARP_DIRS_H
