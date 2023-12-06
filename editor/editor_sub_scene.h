// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_SUB_SCENE_H
#define EDITOR_SUB_SCENE_H

#include "editor/editor_file_dialog.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"

class EditorSubScene : public ConfirmationDialog {
    GDCLASS(EditorSubScene, ConfirmationDialog);

    List<Node*> selection;
    LineEdit* path;
    Tree* tree;
    Node* scene;
    bool is_root;

    EditorFileDialog* file_dialog;

    void _fill_tree(Node* p_node, TreeItem* p_parent);
    void _selected_changed();
    void _item_multi_selected(Object* p_object, int p_cell, bool p_selected);
    void _remove_selection_child(Node* p_node);
    void _reown(Node* p_node, List<Node*>* p_to_reown);

    void ok_pressed();

protected:
    void _notification(int p_what);
    static void _bind_methods();
    void _path_browse();
    void _path_selected(const String& p_path);
    void _path_changed(const String& p_path);

public:
    void move(Node* p_new_parent, Node* p_new_owner);
    void clear();
    EditorSubScene();
};

#endif // EDITOR_SUB_SCENE_H
