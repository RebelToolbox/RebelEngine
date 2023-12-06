// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_PATH_H
#define EDITOR_PATH_H

#include "editor_data.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/popup_menu.h"

class EditorPath : public Button {
    GDCLASS(EditorPath, Button);

    EditorHistory* history;

    TextureRect* current_object_icon;
    Label* current_object_label;
    TextureRect* sub_objects_icon;
    PopupMenu* sub_objects_menu;

    Vector<ObjectID> objects;
    EditorPath();

    void _show_popup();
    void _id_pressed(int p_idx);
    void _about_to_show();
    void _add_children_to_popup(Object* p_obj, int p_depth = 0);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void update_path();
    void clear_path();
    void enable_path();

    EditorPath(EditorHistory* p_history);
};

#endif // EDITOR_PATH_H
