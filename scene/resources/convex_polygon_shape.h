// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONVEX_POLYGON_SHAPE_H
#define CONVEX_POLYGON_SHAPE_H

#include "scene/resources/shape.h"

class ConvexPolygonShape : public Shape {
    GDCLASS(ConvexPolygonShape, Shape);
    PoolVector<Vector3> points;

protected:
    static void _bind_methods();

    virtual void _update_shape();

public:
    void set_points(const PoolVector<Vector3>& p_points);
    PoolVector<Vector3> get_points() const;

    virtual Vector<Vector3> get_debug_mesh_lines();

    ConvexPolygonShape();
};

#endif // CONVEX_POLYGON_SHAPE_H
