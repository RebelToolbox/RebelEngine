// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PHYSICAL_BONE_PLUGIN_H
#define PHYSICAL_BONE_PLUGIN_H

#include "editor/editor_node.h"

class PhysicalBoneEditor : public Object {
    GDCLASS(PhysicalBoneEditor, Object);

    EditorNode* editor;
    HBoxContainer* spatial_editor_hb;
    ToolButton* button_transform_joint;

    PhysicalBone* selected;

protected:
    static void _bind_methods();

private:
    void _on_toggle_button_transform_joint(bool p_is_pressed);
    void _set_move_joint();

public:
    PhysicalBoneEditor(EditorNode* p_editor);
    ~PhysicalBoneEditor();

    void set_selected(PhysicalBone* p_pb);

    void hide();
    void show();
};

class PhysicalBonePlugin : public EditorPlugin {
    GDCLASS(PhysicalBonePlugin, EditorPlugin);

    EditorNode* editor;
    PhysicalBone* selected;
    PhysicalBoneEditor physical_bone_editor;

public:
    virtual String get_name() const {
        return "PhysicalBone";
    }

    virtual bool handles(Object* p_object) const {
        return p_object->is_class("PhysicalBone");
    }

    virtual void make_visible(bool p_visible);
    virtual void edit(Object* p_node);

    PhysicalBonePlugin(EditorNode* p_editor);
};

#endif
