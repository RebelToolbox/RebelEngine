// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_SCENE_EXPORTER_GLTF_PLUGIN_H
#define EDITOR_SCENE_EXPORTER_GLTF_PLUGIN_H

#include "editor/editor_plugin.h"
#include "editor_scene_importer_gltf.h"

class SceneExporterGLTFPlugin : public EditorPlugin {
    GDCLASS(SceneExporterGLTFPlugin, EditorPlugin);

    Ref<PackedSceneGLTF> convert_gltf2;
    EditorNode* editor                = nullptr;
    EditorFileDialog* file_export_lib = nullptr;
    void _gltf2_dialog_action(String p_file);
    void convert_scene_to_gltf2(Variant p_null);

protected:
    static void _bind_methods();

public:
    virtual String get_name() const;
    bool has_main_screen() const;
    SceneExporterGLTFPlugin(class EditorNode* p_node);
};

#endif // EDITOR_SCENE_EXPORTER_GLTF_PLUGIN_H
