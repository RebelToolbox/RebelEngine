// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NAVIGATIONPOLYGONEDITORPLUGIN_H
#define NAVIGATIONPOLYGONEDITORPLUGIN_H

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/navigation_polygon.h"

class NavigationPolygonEditor : public AbstractPolygon2DEditor {
    GDCLASS(NavigationPolygonEditor, AbstractPolygon2DEditor);

    NavigationPolygonInstance* node;

    Ref<NavigationPolygon> _ensure_navpoly() const;

protected:
    virtual Node2D* _get_node() const;
    virtual void _set_node(Node* p_polygon);

    virtual int _get_polygon_count() const;
    virtual Variant _get_polygon(int p_idx) const;
    virtual void _set_polygon(int p_idx, const Variant& p_polygon) const;

    virtual void _action_add_polygon(const Variant& p_polygon);
    virtual void _action_remove_polygon(int p_idx);
    virtual void _action_set_polygon(
        int p_idx,
        const Variant& p_previous,
        const Variant& p_polygon
    );

    virtual bool _has_resource() const;
    virtual void _create_resource();

public:
    NavigationPolygonEditor(EditorNode* p_editor);
};

class NavigationPolygonEditorPlugin : public AbstractPolygon2DEditorPlugin {
    GDCLASS(NavigationPolygonEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
    NavigationPolygonEditorPlugin(EditorNode* p_node);
};

#endif // NAVIGATIONPOLYGONEDITORPLUGIN_H
