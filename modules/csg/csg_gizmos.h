// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CSG_GIZMOS_H
#define CSG_GIZMOS_H

#include "csg_shape.h"
#include "editor/editor_plugin.h"
#include "editor/spatial_editor_gizmos.h"

class CSGShapeSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
    GDCLASS(CSGShapeSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
    bool has_gizmo(Spatial* p_spatial) override;
    String get_name() const override;
    int get_priority() const override;
    bool is_selectable_when_hidden() const override;
    void redraw(EditorSpatialGizmo* p_gizmo) override;

    String get_handle_name(
        const EditorSpatialGizmo* p_gizmo,
        int p_idx
    ) const override;
    Variant get_handle_value(
        EditorSpatialGizmo* p_gizmo,
        int p_idx
    ) const override;
    void set_handle(
        EditorSpatialGizmo* p_gizmo,
        int p_idx,
        Camera* p_camera,
        const Point2& p_point
    ) override;
    void commit_handle(
        EditorSpatialGizmo* p_gizmo,
        int p_idx,
        const Variant& p_restore,
        bool p_cancel
    ) override;

    CSGShapeSpatialGizmoPlugin();
};

class EditorPluginCSG : public EditorPlugin {
    GDCLASS(EditorPluginCSG, EditorPlugin);

public:
    EditorPluginCSG(EditorNode* p_editor);
};

#endif // CSG_GIZMOS_H
