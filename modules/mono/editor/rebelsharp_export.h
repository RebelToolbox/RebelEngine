// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REBELSHARP_EXPORT_H
#define REBELSHARP_EXPORT_H

#include "../mono_gd/gd_mono_header.h"
#include "core/dictionary.h"
#include "core/error_list.h"
#include "core/ustring.h"

namespace RebelSharpExport {

Error get_assembly_dependencies(
    GDMonoAssembly* p_assembly,
    const Vector<String>& p_search_dirs,
    Dictionary& r_dependencies
);

Error get_exported_assembly_dependencies(
    const Dictionary& p_initial_assemblies,
    const String& p_build_config,
    const String& p_custom_lib_dir,
    Dictionary& r_assembly_dependencies
);

} // namespace RebelSharpExport

#endif // REBELSHARP_EXPORT_H
