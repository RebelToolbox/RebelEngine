// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LINE_SHAPE_2D_H
#define LINE_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class LineShape2D : public Shape2D {
    GDCLASS(LineShape2D, Shape2D);

    Vector2 normal;
    real_t d;

    void _update_shape();

protected:
    static void _bind_methods();

public:
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const;

    void set_normal(const Vector2& p_normal);
    void set_d(real_t p_d);

    Vector2 get_normal() const;
    real_t get_d() const;

    virtual void draw(const RID& p_to_rid, const Color& p_color);
    virtual Rect2 get_rect() const;

    LineShape2D();
};

#endif // LINE_SHAPE_2D_H
