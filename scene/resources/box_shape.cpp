// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "box_shape.h"

#include "servers/physics_server.h"

Vector<Vector3> BoxShape::get_debug_mesh_lines() {
    Vector<Vector3> lines;
    AABB aabb;
    aabb.position = -get_extents();
    aabb.size     = aabb.position * -2;

    for (int i = 0; i < 12; i++) {
        Vector3 a, b;
        aabb.get_edge(i, a, b);
        lines.push_back(a);
        lines.push_back(b);
    }

    return lines;
}

void BoxShape::_update_shape() {
    PhysicsServer::get_singleton()->shape_set_data(get_shape(), extents);
    Shape::_update_shape();
}

void BoxShape::set_extents(const Vector3& p_extents) {
    extents = p_extents;
    _update_shape();
    notify_change_to_owners();
    _change_notify("extents");
}

Vector3 BoxShape::get_extents() const {
    return extents;
}

void BoxShape::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_extents", "extents"),
        &BoxShape::set_extents
    );
    ClassDB::bind_method(D_METHOD("get_extents"), &BoxShape::get_extents);

    ADD_PROPERTY(
        PropertyInfo(Variant::VECTOR3, "extents"),
        "set_extents",
        "get_extents"
    );
}

BoxShape::BoxShape() :
    Shape(PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_BOX)
    ) {
    set_extents(Vector3(1, 1, 1));
}
