// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_POLYGON_2D_EDITOR_PLUGIN_H
#define COLLISION_POLYGON_2D_EDITOR_PLUGIN_H

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/collision_polygon_2d.h"

class CollisionPolygon2DEditor : public AbstractPolygon2DEditor {
    GDCLASS(CollisionPolygon2DEditor, AbstractPolygon2DEditor);

    CollisionPolygon2D* node;

protected:
    virtual Node2D* _get_node() const;
    virtual void _set_node(Node* p_polygon);

public:
    CollisionPolygon2DEditor(EditorNode* p_editor);
};

class CollisionPolygon2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
    GDCLASS(CollisionPolygon2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
    CollisionPolygon2DEditorPlugin(EditorNode* p_node);
};

#endif // COLLISION_POLYGON_2D_EDITOR_PLUGIN_H
