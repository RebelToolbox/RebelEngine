// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "editor/editor_node.h"
#include "editor_scene_importer_fbx.h"

#ifdef TOOLS_ENABLED
static void _editor_init() {
    Ref<EditorSceneImporterFBX> import_fbx;
    import_fbx.instance();
    ResourceImporterScene::get_singleton()->add_importer(import_fbx);
}
#endif

void register_fbx_types() {
#ifdef TOOLS_ENABLED
    ClassDB::APIType prev_api = ClassDB::get_current_api();
    ClassDB::set_current_api(ClassDB::API_EDITOR);

    ClassDB::register_class<EditorSceneImporterFBX>();

    ClassDB::set_current_api(prev_api);

    EditorNode::add_init_callback(_editor_init);
#endif
}

void unregister_fbx_types() {}
