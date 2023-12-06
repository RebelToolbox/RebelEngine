// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "rectangle_shape_2d.h"

#include "servers/physics_2d_server.h"
#include "servers/visual_server.h"

void RectangleShape2D::_update_shape() {
    Physics2DServer::get_singleton()->shape_set_data(get_rid(), extents);
    emit_changed();
}

void RectangleShape2D::set_extents(const Vector2& p_extents) {
    extents = p_extents;
    _update_shape();
}

Vector2 RectangleShape2D::get_extents() const {
    return extents;
}

void RectangleShape2D::draw(const RID& p_to_rid, const Color& p_color) {
    VisualServer::get_singleton()->canvas_item_add_rect(
        p_to_rid,
        Rect2(-extents, extents * 2.0),
        p_color
    );
    if (is_collision_outline_enabled()) {
        // Draw an outlined rectangle to make individual shapes easier to
        // distinguish.
        Vector<Vector2> stroke_points;
        stroke_points.resize(5);
        stroke_points.write[0] = -extents;
        stroke_points.write[1] = Vector2(extents.x, -extents.y);
        stroke_points.write[2] = extents;
        stroke_points.write[3] = Vector2(-extents.x, extents.y);
        stroke_points.write[4] = -extents;

        Vector<Color> stroke_colors;
        stroke_colors.resize(5);
        for (int i = 0; i < 5; i++) {
            stroke_colors.write[i] = p_color;
        }

        VisualServer::get_singleton()->canvas_item_add_polyline(
            p_to_rid,
            stroke_points,
            stroke_colors,
            1.0,
            true
        );
    }
}

Rect2 RectangleShape2D::get_rect() const {
    return Rect2(-extents, extents * 2.0);
}

void RectangleShape2D::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_extents", "extents"),
        &RectangleShape2D::set_extents
    );
    ClassDB::bind_method(
        D_METHOD("get_extents"),
        &RectangleShape2D::get_extents
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::VECTOR2, "extents"),
        "set_extents",
        "get_extents"
    );
}

RectangleShape2D::RectangleShape2D() :
    Shape2D(Physics2DServer::get_singleton()->rectangle_shape_create()) {
    extents = Vector2(10, 10);
    _update_shape();
}
