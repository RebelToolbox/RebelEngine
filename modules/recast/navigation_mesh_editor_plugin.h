// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NAVIGATION_MESH_GENERATOR_PLUGIN_H
#define NAVIGATION_MESH_GENERATOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "navigation_mesh_generator.h"

class NavigationMeshEditor : public Control {
    friend class NavigationMeshEditorPlugin;

    GDCLASS(NavigationMeshEditor, Control);

    AcceptDialog* err_dialog;

    HBoxContainer* bake_hbox;
    ToolButton* button_bake;
    ToolButton* button_reset;
    Label* bake_info;

    NavigationMeshInstance* node;

    void _bake_pressed();
    void _clear_pressed();

protected:
    void _node_removed(Node* p_node);
    static void _bind_methods();
    void _notification(int p_option);

public:
    void edit(NavigationMeshInstance* p_nav_mesh_instance);
    NavigationMeshEditor();
    ~NavigationMeshEditor();
};

class NavigationMeshEditorPlugin : public EditorPlugin {
    GDCLASS(NavigationMeshEditorPlugin, EditorPlugin);

    NavigationMeshEditor* navigation_mesh_editor;
    EditorNode* editor;

public:
    virtual String get_name() const {
        return "NavigationMesh";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    NavigationMeshEditorPlugin(EditorNode* p_node);
    ~NavigationMeshEditorPlugin();
};

#endif // NAVIGATION_MESH_GENERATOR_PLUGIN_H
