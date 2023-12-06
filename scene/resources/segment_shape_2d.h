// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SEGMENT_SHAPE_2D_H
#define SEGMENT_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class SegmentShape2D : public Shape2D {
    GDCLASS(SegmentShape2D, Shape2D);

    Vector2 a;
    Vector2 b;

    void _update_shape();

protected:
    static void _bind_methods();

public:
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const;

    void set_a(const Vector2& p_a);
    void set_b(const Vector2& p_b);

    Vector2 get_a() const;
    Vector2 get_b() const;

    virtual void draw(const RID& p_to_rid, const Color& p_color);
    virtual Rect2 get_rect() const;

    SegmentShape2D();
};

class RayShape2D : public Shape2D {
    GDCLASS(RayShape2D, Shape2D);

    real_t length;
    bool slips_on_slope;

    void _update_shape();

protected:
    static void _bind_methods();

public:
    void set_length(real_t p_length);
    real_t get_length() const;

    void set_slips_on_slope(bool p_active);
    bool get_slips_on_slope() const;

    virtual void draw(const RID& p_to_rid, const Color& p_color);
    virtual Rect2 get_rect() const;

    RayShape2D();
};

#endif // SEGMENT_SHAPE_2D_H
