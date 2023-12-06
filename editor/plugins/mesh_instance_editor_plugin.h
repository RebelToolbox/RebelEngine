// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MESH_INSTANCE_EDITOR_PLUGIN_H
#define MESH_INSTANCE_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/3d/mesh_instance.h"
#include "scene/gui/spin_box.h"

class MeshInstanceEditor : public Control {
    GDCLASS(MeshInstanceEditor, Control);

    enum Menu {
        MENU_OPTION_CREATE_STATIC_TRIMESH_BODY,
        MENU_OPTION_CREATE_TRIMESH_COLLISION_SHAPE,
        MENU_OPTION_CREATE_SINGLE_CONVEX_COLLISION_SHAPE,
        MENU_OPTION_CREATE_SIMPLIFIED_CONVEX_COLLISION_SHAPE,
        MENU_OPTION_CREATE_MULTIPLE_CONVEX_COLLISION_SHAPES,
        MENU_OPTION_CREATE_NAVMESH,
        MENU_OPTION_CREATE_OUTLINE_MESH,
        MENU_OPTION_CREATE_UV2,
        MENU_OPTION_DEBUG_UV1,
        MENU_OPTION_DEBUG_UV2,
    };

    MeshInstance* node;

    MenuButton* options;

    ConfirmationDialog* outline_dialog;
    SpinBox* outline_size;

    AcceptDialog* err_dialog;

    AcceptDialog* debug_uv_dialog;
    Control* debug_uv;
    Vector<Vector2> uv_lines;

    void _menu_option(int p_option);
    void _create_outline_mesh();

    void _create_uv_lines(int p_layer);
    friend class MeshInstanceEditorPlugin;

    void _debug_uv_draw();

protected:
    void _node_removed(Node* p_node);
    static void _bind_methods();

public:
    void edit(MeshInstance* p_mesh);
    MeshInstanceEditor();
};

class MeshInstanceEditorPlugin : public EditorPlugin {
    GDCLASS(MeshInstanceEditorPlugin, EditorPlugin);

    MeshInstanceEditor* mesh_editor;
    EditorNode* editor;

public:
    virtual String get_name() const {
        return "MeshInstance";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    MeshInstanceEditorPlugin(EditorNode* p_node);
    ~MeshInstanceEditorPlugin();
};

#endif // MESH_EDITOR_PLUGIN_H
