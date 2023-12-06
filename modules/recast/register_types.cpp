// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "navigation_mesh_editor_plugin.h"

#ifdef TOOLS_ENABLED
EditorNavigationMeshGenerator* _nav_mesh_generator = nullptr;
#endif

void register_recast_types() {
#ifdef TOOLS_ENABLED
    ClassDB::APIType prev_api = ClassDB::get_current_api();
    ClassDB::set_current_api(ClassDB::API_EDITOR);

    EditorPlugins::add_by_type<NavigationMeshEditorPlugin>();
    _nav_mesh_generator = memnew(EditorNavigationMeshGenerator);

    ClassDB::register_class<EditorNavigationMeshGenerator>();

    Engine::get_singleton()->add_singleton(Engine::Singleton(
        "NavigationMeshGenerator",
        EditorNavigationMeshGenerator::get_singleton()
    ));

    ClassDB::set_current_api(prev_api);
#endif
}

void unregister_recast_types() {
#ifdef TOOLS_ENABLED
    if (_nav_mesh_generator) {
        memdelete(_nav_mesh_generator);
    }
#endif
}
