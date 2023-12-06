// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CAPSULE_SHAPE_2D_H
#define CAPSULE_SHAPE_2D_H

#include "scene/resources/shape_2d.h"

class CapsuleShape2D : public Shape2D {
    GDCLASS(CapsuleShape2D, Shape2D);

    real_t height;
    real_t radius;

    void _update_shape();
    Vector<Vector2> _get_points() const;

protected:
    static void _bind_methods();

public:
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const;

    void set_height(real_t p_height);
    real_t get_height() const;

    void set_radius(real_t p_radius);
    real_t get_radius() const;

    virtual void draw(const RID& p_to_rid, const Color& p_color);
    virtual Rect2 get_rect() const;

    CapsuleShape2D();
};

#endif // CAPSULE_SHAPE_2D_H
