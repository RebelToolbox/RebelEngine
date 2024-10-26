// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "csharp_project.h"

#include "../csharp_script.h"
#include "../mono_gd/gd_mono_class.h"
#include "../mono_gd/gd_mono_marshal.h"
#include "../utils/string_utils.h"
#include "core/io/json.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"
#include "core/project_settings.h"
#include "script_class_parser.h"

namespace CSharpProject {

void add_item(
    const String& p_project_path,
    const String& p_item_type,
    const String& p_include
) {
    if (!GLOBAL_DEF("mono/project/auto_update_project", true)) {
        return;
    }

    GDMonoAssembly* tools_project_editor_assembly =
        GDMono::get_singleton()->get_tools_project_editor_assembly();

    GDMonoClass* klass = tools_project_editor_assembly->get_class(
        "GodotTools.ProjectEditor",
        "ProjectUtils"
    );

    Variant project_path   = p_project_path;
    Variant item_type      = p_item_type;
    Variant include        = p_include;
    const Variant* args[3] = {&project_path, &item_type, &include};
    MonoException* exc     = NULL;
    klass->get_method("AddItemToProjectChecked", 3)->invoke(NULL, args, &exc);

    if (exc) {
        GDMonoUtils::debug_print_unhandled_exception(exc);
        ERR_FAIL();
    }
}

} // namespace CSharpProject
