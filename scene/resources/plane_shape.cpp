// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "plane_shape.h"

#include "servers/physics_server.h"

Vector<Vector3> PlaneShape::get_debug_mesh_lines() {
    Plane p = get_plane();
    Vector<Vector3> points;

    Vector3 n1 = p.get_any_perpendicular_normal();
    Vector3 n2 = p.normal.cross(n1).normalized();

    Vector3 pface[4] = {
        p.normal * p.d + n1 * 10.0 + n2 * 10.0,
        p.normal * p.d + n1 * 10.0 + n2 * -10.0,
        p.normal * p.d + n1 * -10.0 + n2 * -10.0,
        p.normal * p.d + n1 * -10.0 + n2 * 10.0,
    };

    points.push_back(pface[0]);
    points.push_back(pface[1]);
    points.push_back(pface[1]);
    points.push_back(pface[2]);
    points.push_back(pface[2]);
    points.push_back(pface[3]);
    points.push_back(pface[3]);
    points.push_back(pface[0]);
    points.push_back(p.normal * p.d);
    points.push_back(p.normal * p.d + p.normal * 3);

    return points;
}

void PlaneShape::_update_shape() {
    PhysicsServer::get_singleton()->shape_set_data(get_shape(), plane);
    Shape::_update_shape();
}

void PlaneShape::set_plane(Plane p_plane) {
    plane = p_plane;
    _update_shape();
    notify_change_to_owners();
    _change_notify("plane");
}

Plane PlaneShape::get_plane() const {
    return plane;
}

void PlaneShape::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_plane", "plane"),
        &PlaneShape::set_plane
    );
    ClassDB::bind_method(D_METHOD("get_plane"), &PlaneShape::get_plane);

    ADD_PROPERTY(
        PropertyInfo(Variant::PLANE, "plane"),
        "set_plane",
        "get_plane"
    );
}

PlaneShape::PlaneShape() :
    Shape(
        PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_PLANE)
    ) {
    set_plane(Plane(0, 1, 0, 0));
}
