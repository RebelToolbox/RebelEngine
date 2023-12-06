// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BAKED_LIGHTMAP_EDITOR_PLUGIN_H
#define BAKED_LIGHTMAP_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/3d/baked_lightmap.h"
#include "scene/resources/material.h"

class BakedLightmapEditorPlugin : public EditorPlugin {
    GDCLASS(BakedLightmapEditorPlugin, EditorPlugin);

    BakedLightmap* lightmap;

    ToolButton* bake;
    EditorNode* editor;

    EditorFileDialog* file_dialog;
    static EditorProgress* tmp_progress;
    static EditorProgress* tmp_subprogress;

    static bool bake_func_step(
        float p_progress,
        const String& p_description,
        void*,
        bool p_force_refresh
    );
    static bool bake_func_substep(
        float p_progress,
        const String& p_description,
        void*,
        bool p_force_refresh
    );
    static void bake_func_end(uint32_t p_time_started);

    void _bake_select_file(const String& p_file);
    void _bake();

protected:
    static void _bind_methods();

public:
    virtual String get_name() const {
        return "BakedLightmap";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    BakedLightmapEditorPlugin(EditorNode* p_node);
    ~BakedLightmapEditorPlugin();
};

#endif // BAKED_LIGHTMAP_EDITOR_PLUGIN_H
