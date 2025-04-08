// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/transform2d.h"

#include "core/math/transform_2d.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_transform2d) == sizeof(Transform2D),
    "Transform2D size mismatch"
);

void GDAPI rebel_transform2d_new(
    rebel_transform2d* r_dest,
    const rebel_real p_rot,
    const rebel_vector2* p_pos
) {
    const Vector2* pos = (const Vector2*)p_pos;
    Transform2D* dest  = (Transform2D*)r_dest;
    *dest              = Transform2D(p_rot, *pos);
}

void GDAPI rebel_transform2d_new_axis_origin(
    rebel_transform2d* r_dest,
    const rebel_vector2* p_x_axis,
    const rebel_vector2* p_y_axis,
    const rebel_vector2* p_origin
) {
    const Vector2* x_axis = (const Vector2*)p_x_axis;
    const Vector2* y_axis = (const Vector2*)p_y_axis;
    const Vector2* origin = (const Vector2*)p_origin;
    Transform2D* dest     = (Transform2D*)r_dest;
    *dest                 = Transform2D(
        x_axis->x,
        x_axis->y,
        y_axis->x,
        y_axis->y,
        origin->x,
        origin->y
    );
}

rebel_string GDAPI rebel_transform2d_as_string(const rebel_transform2d* p_self
) {
    rebel_string ret;
    const Transform2D* self = (const Transform2D*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_transform2d GDAPI
rebel_transform2d_inverse(const rebel_transform2d* p_self) {
    rebel_transform2d dest;
    const Transform2D* self = (const Transform2D*)p_self;
    *((Transform2D*)&dest)  = self->inverse();
    return dest;
}

rebel_transform2d GDAPI
rebel_transform2d_affine_inverse(const rebel_transform2d* p_self) {
    rebel_transform2d dest;
    const Transform2D* self = (const Transform2D*)p_self;
    *((Transform2D*)&dest)  = self->affine_inverse();
    return dest;
}

rebel_real GDAPI rebel_transform2d_get_rotation(const rebel_transform2d* p_self
) {
    const Transform2D* self = (const Transform2D*)p_self;
    return self->get_rotation();
}

rebel_vector2 GDAPI rebel_transform2d_get_origin(const rebel_transform2d* p_self
) {
    rebel_vector2 dest;
    const Transform2D* self = (const Transform2D*)p_self;
    *((Vector2*)&dest)      = self->get_origin();
    return dest;
}

rebel_vector2 GDAPI rebel_transform2d_get_scale(const rebel_transform2d* p_self
) {
    rebel_vector2 dest;
    const Transform2D* self = (const Transform2D*)p_self;
    *((Vector2*)&dest)      = self->get_scale();
    return dest;
}

rebel_transform2d GDAPI
rebel_transform2d_orthonormalized(const rebel_transform2d* p_self) {
    rebel_transform2d dest;
    const Transform2D* self = (const Transform2D*)p_self;
    *((Transform2D*)&dest)  = self->orthonormalized();
    return dest;
}

rebel_transform2d GDAPI rebel_transform2d_rotated(
    const rebel_transform2d* p_self,
    const rebel_real p_phi
) {
    rebel_transform2d dest;
    const Transform2D* self = (const Transform2D*)p_self;

    *((Transform2D*)&dest) = self->rotated(p_phi);
    return dest;
}

rebel_transform2d GDAPI rebel_transform2d_scaled(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_scale
) {
    rebel_transform2d dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Vector2* scale    = (const Vector2*)p_scale;
    *((Transform2D*)&dest)  = self->scaled(*scale);
    return dest;
}

rebel_transform2d GDAPI rebel_transform2d_translated(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_offset
) {
    rebel_transform2d dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Vector2* offset   = (const Vector2*)p_offset;
    *((Transform2D*)&dest)  = self->translated(*offset);
    return dest;
}

rebel_vector2 GDAPI rebel_transform2d_xform_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
) {
    rebel_vector2 raw_dest;
    Vector2* dest           = (Vector2*)&raw_dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Vector2* v        = (const Vector2*)p_v;
    *dest                   = self->xform(*v);
    return raw_dest;
}

rebel_vector2 GDAPI rebel_transform2d_xform_inv_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
) {
    rebel_vector2 raw_dest;
    Vector2* dest           = (Vector2*)&raw_dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Vector2* v        = (const Vector2*)p_v;
    *dest                   = self->xform_inv(*v);
    return raw_dest;
}

rebel_vector2 GDAPI rebel_transform2d_basis_xform_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
) {
    rebel_vector2 raw_dest;
    Vector2* dest           = (Vector2*)&raw_dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Vector2* v        = (const Vector2*)p_v;
    *dest                   = self->basis_xform(*v);
    return raw_dest;
}

rebel_vector2 GDAPI rebel_transform2d_basis_xform_inv_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
) {
    rebel_vector2 raw_dest;
    Vector2* dest           = (Vector2*)&raw_dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Vector2* v        = (const Vector2*)p_v;
    *dest                   = self->basis_xform_inv(*v);
    return raw_dest;
}

rebel_transform2d GDAPI rebel_transform2d_interpolate_with(
    const rebel_transform2d* p_self,
    const rebel_transform2d* p_m,
    const rebel_real p_c
) {
    rebel_transform2d dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Transform2D* m    = (const Transform2D*)p_m;
    *((Transform2D*)&dest)  = self->interpolate_with(*m, p_c);
    return dest;
}

rebel_bool GDAPI rebel_transform2d_operator_equal(
    const rebel_transform2d* p_self,
    const rebel_transform2d* p_b
) {
    const Transform2D* self = (const Transform2D*)p_self;
    const Transform2D* b    = (const Transform2D*)p_b;
    return *self == *b;
}

rebel_transform2d GDAPI rebel_transform2d_operator_multiply(
    const rebel_transform2d* p_self,
    const rebel_transform2d* p_b
) {
    rebel_transform2d raw_dest;
    Transform2D* dest       = (Transform2D*)&raw_dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Transform2D* b    = (const Transform2D*)p_b;
    *dest                   = *self * *b;
    return raw_dest;
}

void GDAPI rebel_transform2d_new_identity(rebel_transform2d* r_dest) {
    Transform2D* dest = (Transform2D*)r_dest;
    *dest             = Transform2D();
}

rebel_rect2 GDAPI rebel_transform2d_xform_rect2(
    const rebel_transform2d* p_self,
    const rebel_rect2* p_v
) {
    rebel_rect2 raw_dest;
    Rect2* dest             = (Rect2*)&raw_dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Rect2* v          = (const Rect2*)p_v;
    *dest                   = self->xform(*v);
    return raw_dest;
}

rebel_rect2 GDAPI rebel_transform2d_xform_inv_rect2(
    const rebel_transform2d* p_self,
    const rebel_rect2* p_v
) {
    rebel_rect2 raw_dest;
    Rect2* dest             = (Rect2*)&raw_dest;
    const Transform2D* self = (const Transform2D*)p_self;
    const Rect2* v          = (const Rect2*)p_v;
    *dest                   = self->xform_inv(*v);
    return raw_dest;
}

#ifdef __cplusplus
}
#endif
