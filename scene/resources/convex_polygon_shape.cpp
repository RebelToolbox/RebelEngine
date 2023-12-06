// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "convex_polygon_shape.h"

#include "core/math/convex_hull.h"
#include "servers/physics_server.h"

Vector<Vector3> ConvexPolygonShape::get_debug_mesh_lines() {
    PoolVector<Vector3> points = get_points();

    if (points.size() > 3) {
        Vector<Vector3> varr = Variant(points);
        Geometry::MeshData md;
        Error err = ConvexHullComputer::convex_hull(varr, md);
        if (err == OK) {
            Vector<Vector3> lines;
            lines.resize(md.edges.size() * 2);
            for (int i = 0; i < md.edges.size(); i++) {
                lines.write[i * 2 + 0] = md.vertices[md.edges[i].a];
                lines.write[i * 2 + 1] = md.vertices[md.edges[i].b];
            }
            return lines;
        }
    }

    return Vector<Vector3>();
}

void ConvexPolygonShape::_update_shape() {
    PhysicsServer::get_singleton()->shape_set_data(get_shape(), points);
    Shape::_update_shape();
}

void ConvexPolygonShape::set_points(const PoolVector<Vector3>& p_points) {
    points = p_points;
    _update_shape();
    notify_change_to_owners();
}

PoolVector<Vector3> ConvexPolygonShape::get_points() const {
    return points;
}

void ConvexPolygonShape::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_points", "points"),
        &ConvexPolygonShape::set_points
    );
    ClassDB::bind_method(
        D_METHOD("get_points"),
        &ConvexPolygonShape::get_points
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "points"),
        "set_points",
        "get_points"
    );
}

ConvexPolygonShape::ConvexPolygonShape() :
    Shape(PhysicsServer::get_singleton()->shape_create(
        PhysicsServer::SHAPE_CONVEX_POLYGON
    )) {}
