// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "camera_editor_plugin.h"

#include "spatial_editor_plugin.h"

void CameraEditor::_node_removed(Node* p_node) {
    if (p_node == node) {
        node = nullptr;
        SpatialEditor::get_singleton()->set_custom_camera(nullptr);
        hide();
    }
}

void CameraEditor::_pressed() {
    Node* sn = (node && preview->is_pressed()) ? node : nullptr;
    SpatialEditor::get_singleton()->set_custom_camera(sn);
}

void CameraEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_pressed"), &CameraEditor::_pressed);
}

void CameraEditor::edit(Node* p_camera) {
    node = p_camera;

    if (!node) {
        preview->set_pressed(false);
        SpatialEditor::get_singleton()->set_custom_camera(nullptr);
    } else {
        if (preview->is_pressed()) {
            SpatialEditor::get_singleton()->set_custom_camera(p_camera);
        } else {
            SpatialEditor::get_singleton()->set_custom_camera(nullptr);
        }
    }
}

CameraEditor::CameraEditor() {
    preview = memnew(Button);
    add_child(preview);

    preview->set_text(TTR("Preview"));
    preview->set_toggle_mode(true);
    preview->set_anchor(MARGIN_LEFT, Control::ANCHOR_END);
    preview->set_anchor(MARGIN_RIGHT, Control::ANCHOR_END);
    preview->set_margin(MARGIN_LEFT, -60);
    preview->set_margin(MARGIN_RIGHT, 0);
    preview->set_margin(MARGIN_TOP, 0);
    preview->set_margin(MARGIN_BOTTOM, 10);
    preview->connect("pressed", this, "_pressed");
}

void CameraEditorPlugin::edit(Object* p_object) {
    SpatialEditor::get_singleton()->set_can_preview(
        Object::cast_to<Camera>(p_object)
    );
    // camera_editor->edit(Object::cast_to<Node>(p_object));
}

bool CameraEditorPlugin::handles(Object* p_object) const {
    return p_object->is_class("Camera");
}

void CameraEditorPlugin::make_visible(bool p_visible) {
    if (p_visible) {
        // SpatialEditor::get_singleton()->set_can_preview(Object::cast_to<Camera>(p_object));
    } else {
        SpatialEditor::get_singleton()->set_can_preview(nullptr);
    }
}

CameraEditorPlugin::CameraEditorPlugin(EditorNode* p_node) {
    editor = p_node;
    /*	camera_editor = memnew( CameraEditor );
    editor->get_viewport()->add_child(camera_editor);

    camera_editor->set_anchor(MARGIN_LEFT,Control::ANCHOR_END);
    camera_editor->set_anchor(MARGIN_RIGHT,Control::ANCHOR_END);
    camera_editor->set_margin(MARGIN_LEFT,60);
    camera_editor->set_margin(MARGIN_RIGHT,0);
    camera_editor->set_margin(MARGIN_TOP,0);
    camera_editor->set_margin(MARGIN_BOTTOM,10);


    camera_editor->hide();
*/
}

CameraEditorPlugin::~CameraEditorPlugin() {}
