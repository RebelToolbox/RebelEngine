// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"
#ifndef _3D_DISABLED
#include "core/class_db.h"
#include "grid_map.h"
#include "grid_map_editor_plugin.h"
#endif

void register_gridmap_types() {
#ifndef _3D_DISABLED
    ClassDB::register_class<GridMap>();
#ifdef TOOLS_ENABLED
    EditorPlugins::add_by_type<GridMapEditorPlugin>();
#endif
#endif
}

void unregister_gridmap_types() {}
