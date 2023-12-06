// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "skeleton_ik_editor_plugin.h"

#include "scene/animation/skeleton_ik.h"

void SkeletonIKEditorPlugin::_play() {
    if (!skeleton_ik) {
        return;
    }

    if (!skeleton_ik->get_parent_skeleton()) {
        return;
    }

    if (play_btn->is_pressed()) {
        skeleton_ik->start();
    } else {
        skeleton_ik->stop();
        skeleton_ik->get_parent_skeleton()->clear_bones_global_pose_override();
    }
}

void SkeletonIKEditorPlugin::edit(Object* p_object) {
    if (p_object != skeleton_ik) {
        if (skeleton_ik) {
            play_btn->set_pressed(false);
            _play();
        }
    }

    SkeletonIK* s = Object::cast_to<SkeletonIK>(p_object);
    if (!s) {
        return;
    }

    skeleton_ik = s;
}

bool SkeletonIKEditorPlugin::handles(Object* p_object) const {
    return p_object->is_class("SkeletonIK");
}

void SkeletonIKEditorPlugin::make_visible(bool p_visible) {
    if (p_visible) {
        play_btn->show();
    } else {
        play_btn->hide();
    }
}

void SkeletonIKEditorPlugin::_bind_methods() {
    ClassDB::bind_method("_play", &SkeletonIKEditorPlugin::_play);
}

SkeletonIKEditorPlugin::SkeletonIKEditorPlugin(EditorNode* p_node) {
    editor   = p_node;
    play_btn = memnew(Button);
    play_btn->set_icon(editor->get_gui_base()->get_icon("Play", "EditorIcons"));
    play_btn->set_text(TTR("Play IK"));
    play_btn->set_toggle_mode(true);
    play_btn->hide();
    play_btn->connect("pressed", this, "_play");
    add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, play_btn);
    skeleton_ik = nullptr;
}

SkeletonIKEditorPlugin::~SkeletonIKEditorPlugin() {}
