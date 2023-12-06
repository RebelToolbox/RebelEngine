// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "collision_polygon_2d_editor_plugin.h"

Node2D* CollisionPolygon2DEditor::_get_node() const {
    return node;
}

void CollisionPolygon2DEditor::_set_node(Node* p_polygon) {
    node = Object::cast_to<CollisionPolygon2D>(p_polygon);
}

CollisionPolygon2DEditor::CollisionPolygon2DEditor(EditorNode* p_editor) :
    AbstractPolygon2DEditor(p_editor) {
    node = nullptr;
}

CollisionPolygon2DEditorPlugin::CollisionPolygon2DEditorPlugin(
    EditorNode* p_node
) :
    AbstractPolygon2DEditorPlugin(
        p_node,
        memnew(CollisionPolygon2DEditor(p_node)),
        "CollisionPolygon2D"
    ) {}
