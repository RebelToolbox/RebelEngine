// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "line_2d_editor_plugin.h"

Node2D* Line2DEditor::_get_node() const {
    return node;
}

void Line2DEditor::_set_node(Node* p_line) {
    node = Object::cast_to<Line2D>(p_line);
}

bool Line2DEditor::_is_line() const {
    return true;
}

Variant Line2DEditor::_get_polygon(int p_idx) const {
    return _get_node()->get("points");
}

void Line2DEditor::_set_polygon(int p_idx, const Variant& p_polygon) const {
    _get_node()->set("points", p_polygon);
}

void Line2DEditor::_action_set_polygon(
    int p_idx,
    const Variant& p_previous,
    const Variant& p_polygon
) {
    Node2D* node = _get_node();
    undo_redo->add_do_method(node, "set_points", p_polygon);
    undo_redo->add_undo_method(node, "set_points", p_previous);
}

Line2DEditor::Line2DEditor(EditorNode* p_editor) :
    AbstractPolygon2DEditor(p_editor) {
    node = nullptr;
}

Line2DEditorPlugin::Line2DEditorPlugin(EditorNode* p_node) :
    AbstractPolygon2DEditorPlugin(
        p_node,
        memnew(Line2DEditor(p_node)),
        "Line2D"
    ) {}
