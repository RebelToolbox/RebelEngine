// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONCAVE_POLYGON_SHAPE_2D_H
#define CONCAVE_POLYGON_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class ConcavePolygonShape2D : public Shape2D {
    GDCLASS(ConcavePolygonShape2D, Shape2D);

protected:
    static void _bind_methods();

public:
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const;

    void set_segments(const PoolVector<Vector2>& p_segments);
    PoolVector<Vector2> get_segments() const;

    virtual void draw(const RID& p_to_rid, const Color& p_color);
    virtual Rect2 get_rect() const;

    ConcavePolygonShape2D();
};

#endif
