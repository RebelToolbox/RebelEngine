// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MESH_LIBRARY_EDITOR_PLUGIN_H
#define MESH_LIBRARY_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "scene/resources/mesh_library.h"

class MeshLibraryEditor : public Control {
    GDCLASS(MeshLibraryEditor, Control);

    Ref<MeshLibrary> mesh_library;

    EditorNode* editor;
    MenuButton* menu;
    ConfirmationDialog* cd_remove;
    ConfirmationDialog* cd_update;
    EditorFileDialog* file;
    bool apply_xforms;
    int to_erase;

    enum {
        MENU_OPTION_ADD_ITEM,
        MENU_OPTION_REMOVE_ITEM,
        MENU_OPTION_UPDATE_FROM_SCENE,
        MENU_OPTION_IMPORT_FROM_SCENE,
        MENU_OPTION_IMPORT_FROM_SCENE_APPLY_XFORMS
    };

    int option;
    void _import_scene_cbk(const String& p_str);
    void _menu_cbk(int p_option);
    void _menu_remove_confirm();
    void _menu_update_confirm(bool p_apply_xforms);

    static void _import_scene(
        Node* p_scene,
        Ref<MeshLibrary> p_library,
        bool p_merge,
        bool p_apply_xforms
    );

protected:
    static void _bind_methods();

public:
    MenuButton* get_menu_button() const {
        return menu;
    }

    void edit(const Ref<MeshLibrary>& p_mesh_library);
    static Error update_library_file(
        Node* p_base_scene,
        Ref<MeshLibrary> ml,
        bool p_merge        = true,
        bool p_apply_xforms = false
    );

    MeshLibraryEditor(EditorNode* p_editor);
};

class MeshLibraryEditorPlugin : public EditorPlugin {
    GDCLASS(MeshLibraryEditorPlugin, EditorPlugin);

    MeshLibraryEditor* mesh_library_editor;
    EditorNode* editor;

public:
    virtual String get_name() const {
        return "MeshLibrary";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_node);
    virtual bool handles(Object* p_node) const;
    virtual void make_visible(bool p_visible);

    MeshLibraryEditorPlugin(EditorNode* p_node);
};

#endif // MESH_LIBRARY_EDITOR_PLUGIN_H
