// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SHAPE_2D_H
#define SHAPE_2D_H

#include "core/resource.h"

class Shape2D : public Resource {
    GDCLASS(Shape2D, Resource);
    OBJ_SAVE_TYPE(Shape2D);

    RID shape;
    real_t custom_bias;

protected:
    static void _bind_methods();
    Shape2D(const RID& p_rid);

public:
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const {
        return get_rect().has_point(p_point);
    }

    void set_custom_solver_bias(real_t p_bias);
    real_t get_custom_solver_bias() const;

    bool collide_with_motion(
        const Transform2D& p_local_xform,
        const Vector2& p_local_motion,
        const Ref<Shape2D>& p_shape,
        const Transform2D& p_shape_xform,
        const Vector2& p_shape_motion
    );
    bool collide(
        const Transform2D& p_local_xform,
        const Ref<Shape2D>& p_shape,
        const Transform2D& p_shape_xform
    );

    Array collide_with_motion_and_get_contacts(
        const Transform2D& p_local_xform,
        const Vector2& p_local_motion,
        const Ref<Shape2D>& p_shape,
        const Transform2D& p_shape_xform,
        const Vector2& p_shape_motion
    );
    Array collide_and_get_contacts(
        const Transform2D& p_local_xform,
        const Ref<Shape2D>& p_shape,
        const Transform2D& p_shape_xform
    );

    virtual void draw(const RID& p_to_rid, const Color& p_color) {}

    virtual Rect2 get_rect() const {
        return Rect2();
    }

    virtual RID get_rid() const;

    bool is_collision_outline_enabled();

    Shape2D();
    ~Shape2D();
};

#endif // SHAPE_2D_H
