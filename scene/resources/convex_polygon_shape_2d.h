// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONVEX_POLYGON_SHAPE_2D_H
#define CONVEX_POLYGON_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class ConvexPolygonShape2D : public Shape2D {
    GDCLASS(ConvexPolygonShape2D, Shape2D);

    Vector<Vector2> points;
    void _update_shape();

protected:
    static void _bind_methods();

public:
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const;

    void set_point_cloud(const Vector<Vector2>& p_points);
    void set_points(const Vector<Vector2>& p_points);
    Vector<Vector2> get_points() const;

    virtual void draw(const RID& p_to_rid, const Color& p_color);
    virtual Rect2 get_rect() const;

    ConvexPolygonShape2D();
};

#endif // CONVEX_POLYGON_SHAPE_2D_H
