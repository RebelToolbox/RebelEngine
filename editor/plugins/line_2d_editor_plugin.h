// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LINE_2D_EDITOR_PLUGIN_H
#define LINE_2D_EDITOR_PLUGIN_H

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/line_2d.h"

class Line2DEditor : public AbstractPolygon2DEditor {
    GDCLASS(Line2DEditor, AbstractPolygon2DEditor);

    Line2D* node;

protected:
    virtual Node2D* _get_node() const;
    virtual void _set_node(Node* p_line);

    virtual bool _is_line() const;
    virtual Variant _get_polygon(int p_idx) const;
    virtual void _set_polygon(int p_idx, const Variant& p_polygon) const;
    virtual void _action_set_polygon(
        int p_idx,
        const Variant& p_previous,
        const Variant& p_polygon
    );

public:
    Line2DEditor(EditorNode* p_editor);
};

class Line2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
    GDCLASS(Line2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
    Line2DEditorPlugin(EditorNode* p_node);
};

#endif // LINE_2D_EDITOR_PLUGIN_H
