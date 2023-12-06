// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_POLYGON_H
#define COLLISION_POLYGON_H

#include "scene/3d/spatial.h"
#include "scene/resources/shape.h"

class CollisionObject;

class CollisionPolygon : public Spatial {
    GDCLASS(CollisionPolygon, Spatial);
    real_t margin = 0.04;

protected:
    float depth;
    AABB aabb;
    Vector<Point2> polygon;

    uint32_t owner_id;
    CollisionObject* parent;

    bool disabled;

    void _build_polygon();

    void _update_in_shape_owner(bool p_xform_only = false);

    bool _is_editable_3d_polygon() const;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_depth(float p_depth);
    float get_depth() const;

    void set_polygon(const Vector<Point2>& p_polygon);
    Vector<Point2> get_polygon() const;

    void set_disabled(bool p_disabled);
    bool is_disabled() const;

    virtual AABB get_item_rect() const;

    real_t get_margin() const;
    void set_margin(real_t p_margin);

    String get_configuration_warning() const;

    CollisionPolygon();
};

#endif // COLLISION_POLYGON_H
