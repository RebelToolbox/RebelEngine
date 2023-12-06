// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "convex_polygon_shape_2d.h"

#include "core/math/geometry.h"
#include "servers/physics_2d_server.h"
#include "servers/visual_server.h"

bool ConvexPolygonShape2D::_edit_is_selected_on_click(
    const Point2& p_point,
    double p_tolerance
) const {
    return Geometry::is_point_in_polygon(p_point, points);
}

void ConvexPolygonShape2D::_update_shape() {
    Vector<Vector2> final_points = points;
    if (Geometry::is_polygon_clockwise(final_points
        )) { // needs to be counter clockwise
        final_points.invert();
    }
    Physics2DServer::get_singleton()->shape_set_data(get_rid(), final_points);
    emit_changed();
}

void ConvexPolygonShape2D::set_point_cloud(const Vector<Vector2>& p_points) {
    Vector<Point2> hull = Geometry::convex_hull_2d(p_points);
    ERR_FAIL_COND(hull.size() < 3);
    set_points(hull);
}

void ConvexPolygonShape2D::set_points(const Vector<Vector2>& p_points) {
    points = p_points;

    _update_shape();
}

Vector<Vector2> ConvexPolygonShape2D::get_points() const {
    return points;
}

void ConvexPolygonShape2D::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_point_cloud", "point_cloud"),
        &ConvexPolygonShape2D::set_point_cloud
    );
    ClassDB::bind_method(
        D_METHOD("set_points", "points"),
        &ConvexPolygonShape2D::set_points
    );
    ClassDB::bind_method(
        D_METHOD("get_points"),
        &ConvexPolygonShape2D::get_points
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "points"),
        "set_points",
        "get_points"
    );
}

void ConvexPolygonShape2D::draw(const RID& p_to_rid, const Color& p_color) {
    if (points.size() < 3) {
        return;
    }

    Vector<Color> col;
    col.push_back(p_color);
    VisualServer::get_singleton()
        ->canvas_item_add_polygon(p_to_rid, points, col);
    if (is_collision_outline_enabled()) {
        VisualServer::get_singleton()
            ->canvas_item_add_polyline(p_to_rid, points, col, 1.0, true);
        // Draw the last segment as it's not drawn by
        // `canvas_item_add_polyline()`.
        VisualServer::get_singleton()->canvas_item_add_line(
            p_to_rid,
            points[points.size() - 1],
            points[0],
            p_color,
            1.0,
            true
        );
    }
}

Rect2 ConvexPolygonShape2D::get_rect() const {
    Rect2 rect;
    for (int i = 0; i < points.size(); i++) {
        if (i == 0) {
            rect.position = points[i];
        } else {
            rect.expand_to(points[i]);
        }
    }

    return rect;
}

ConvexPolygonShape2D::ConvexPolygonShape2D() :
    Shape2D(Physics2DServer::get_singleton()->convex_polygon_shape_create()) {}
