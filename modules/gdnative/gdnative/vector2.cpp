// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/vector2.h"

#include "core/math/vector2.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_vector2) == sizeof(Vector2),
    "Vector2 size mismatch"
);

void GDAPI rebel_vector2_new(
    rebel_vector2* r_dest,
    const rebel_real p_x,
    const rebel_real p_y
) {
    Vector2* dest = (Vector2*)r_dest;
    *dest         = Vector2(p_x, p_y);
}

rebel_string GDAPI rebel_vector2_as_string(const rebel_vector2* p_self) {
    rebel_string ret;
    const Vector2* self = (const Vector2*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_vector2 GDAPI rebel_vector2_normalized(const rebel_vector2* p_self) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    *((Vector2*)&dest)  = self->normalized();
    return dest;
}

rebel_real GDAPI rebel_vector2_length(const rebel_vector2* p_self) {
    const Vector2* self = (const Vector2*)p_self;
    return self->length();
}

rebel_real GDAPI rebel_vector2_angle(const rebel_vector2* p_self) {
    const Vector2* self = (const Vector2*)p_self;
    return self->angle();
}

rebel_real GDAPI rebel_vector2_length_squared(const rebel_vector2* p_self) {
    const Vector2* self = (const Vector2*)p_self;
    return self->length_squared();
}

rebel_bool GDAPI rebel_vector2_is_normalized(const rebel_vector2* p_self) {
    const Vector2* self = (const Vector2*)p_self;
    return self->is_normalized();
}

rebel_vector2 GDAPI rebel_vector2_direction_to(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to
) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* to   = (const Vector2*)p_to;
    *((Vector2*)&dest)  = self->direction_to(*to);
    return dest;
}

rebel_real GDAPI rebel_vector2_distance_to(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to
) {
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* to   = (const Vector2*)p_to;
    return self->distance_to(*to);
}

rebel_real GDAPI rebel_vector2_distance_squared_to(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to
) {
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* to   = (const Vector2*)p_to;
    return self->distance_squared_to(*to);
}

rebel_real GDAPI
rebel_vector2_angle_to(const rebel_vector2* p_self, const rebel_vector2* p_to) {
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* to   = (const Vector2*)p_to;
    return self->angle_to(*to);
}

rebel_real GDAPI rebel_vector2_angle_to_point(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to
) {
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* to   = (const Vector2*)p_to;
    return self->angle_to_point(*to);
}

rebel_vector2 GDAPI rebel_vector2_linear_interpolate(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b,
    const rebel_real p_t
) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* b    = (const Vector2*)p_b;
    *((Vector2*)&dest)  = self->linear_interpolate(*b, p_t);
    return dest;
}

rebel_vector2 GDAPI rebel_vector2_cubic_interpolate(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b,
    const rebel_vector2* p_pre_a,
    const rebel_vector2* p_post_b,
    const rebel_real p_t
) {
    rebel_vector2 dest;
    const Vector2* self   = (const Vector2*)p_self;
    const Vector2* b      = (const Vector2*)p_b;
    const Vector2* pre_a  = (const Vector2*)p_pre_a;
    const Vector2* post_b = (const Vector2*)p_post_b;
    *((Vector2*)&dest)    = self->cubic_interpolate(*b, *pre_a, *post_b, p_t);
    return dest;
}

rebel_vector2 GDAPI rebel_vector2_move_toward(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to,
    const rebel_real p_delta
) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* to   = (const Vector2*)p_to;
    *((Vector2*)&dest)  = self->move_toward(*to, p_delta);
    return dest;
}

rebel_vector2 GDAPI
rebel_vector2_rotated(const rebel_vector2* p_self, const rebel_real p_phi) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;

    *((Vector2*)&dest) = self->rotated(p_phi);
    return dest;
}

rebel_vector2 GDAPI rebel_vector2_tangent(const rebel_vector2* p_self) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    *((Vector2*)&dest)  = self->tangent();
    return dest;
}

rebel_vector2 GDAPI rebel_vector2_floor(const rebel_vector2* p_self) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    *((Vector2*)&dest)  = self->floor();
    return dest;
}

rebel_vector2 GDAPI
rebel_vector2_snapped(const rebel_vector2* p_self, const rebel_vector2* p_by) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* by   = (const Vector2*)p_by;
    *((Vector2*)&dest)  = self->snapped(*by);
    return dest;
}

rebel_real GDAPI rebel_vector2_aspect(const rebel_vector2* p_self) {
    const Vector2* self = (const Vector2*)p_self;
    return self->aspect();
}

rebel_real GDAPI
rebel_vector2_dot(const rebel_vector2* p_self, const rebel_vector2* p_with) {
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* with = (const Vector2*)p_with;
    return self->dot(*with);
}

rebel_vector2 GDAPI
rebel_vector2_slide(const rebel_vector2* p_self, const rebel_vector2* p_n) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* n    = (const Vector2*)p_n;
    *((Vector2*)&dest)  = self->slide(*n);
    return dest;
}

rebel_vector2 GDAPI
rebel_vector2_bounce(const rebel_vector2* p_self, const rebel_vector2* p_n) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* n    = (const Vector2*)p_n;
    *((Vector2*)&dest)  = self->bounce(*n);
    return dest;
}

rebel_vector2 GDAPI
rebel_vector2_reflect(const rebel_vector2* p_self, const rebel_vector2* p_n) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* n    = (const Vector2*)p_n;
    *((Vector2*)&dest)  = self->reflect(*n);
    return dest;
}

rebel_vector2 GDAPI rebel_vector2_abs(const rebel_vector2* p_self) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;
    *((Vector2*)&dest)  = self->abs();
    return dest;
}

rebel_vector2 GDAPI
rebel_vector2_clamped(const rebel_vector2* p_self, const rebel_real p_length) {
    rebel_vector2 dest;
    const Vector2* self = (const Vector2*)p_self;

    *((Vector2*)&dest) = self->clamped(p_length);
    return dest;
}

rebel_vector2 GDAPI rebel_vector2_operator_add(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
) {
    rebel_vector2 raw_dest;
    Vector2* dest       = (Vector2*)&raw_dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* b    = (const Vector2*)p_b;
    *dest               = *self + *b;
    return raw_dest;
}

rebel_vector2 GDAPI rebel_vector2_operator_subtract(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
) {
    rebel_vector2 raw_dest;
    Vector2* dest       = (Vector2*)&raw_dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* b    = (const Vector2*)p_b;
    *dest               = *self - *b;
    return raw_dest;
}

rebel_vector2 GDAPI rebel_vector2_operator_multiply_vector(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
) {
    rebel_vector2 raw_dest;
    Vector2* dest       = (Vector2*)&raw_dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* b    = (const Vector2*)p_b;
    *dest               = *self * *b;
    return raw_dest;
}

rebel_vector2 GDAPI rebel_vector2_operator_multiply_scalar(
    const rebel_vector2* p_self,
    const rebel_real p_b
) {
    rebel_vector2 raw_dest;
    Vector2* dest       = (Vector2*)&raw_dest;
    const Vector2* self = (const Vector2*)p_self;
    *dest               = *self * p_b;
    return raw_dest;
}

rebel_vector2 GDAPI rebel_vector2_operator_divide_vector(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
) {
    rebel_vector2 raw_dest;
    Vector2* dest       = (Vector2*)&raw_dest;
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* b    = (const Vector2*)p_b;
    *dest               = *self / *b;
    return raw_dest;
}

rebel_vector2 GDAPI rebel_vector2_operator_divide_scalar(
    const rebel_vector2* p_self,
    const rebel_real p_b
) {
    rebel_vector2 raw_dest;
    Vector2* dest       = (Vector2*)&raw_dest;
    const Vector2* self = (const Vector2*)p_self;
    *dest               = *self / p_b;
    return raw_dest;
}

rebel_bool GDAPI rebel_vector2_operator_equal(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
) {
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* b    = (const Vector2*)p_b;
    return *self == *b;
}

rebel_bool GDAPI rebel_vector2_operator_less(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
) {
    const Vector2* self = (const Vector2*)p_self;
    const Vector2* b    = (const Vector2*)p_b;
    return *self < *b;
}

rebel_vector2 GDAPI rebel_vector2_operator_neg(const rebel_vector2* p_self) {
    rebel_vector2 raw_dest;
    Vector2* dest       = (Vector2*)&raw_dest;
    const Vector2* self = (const Vector2*)p_self;
    *dest               = -(*self);
    return raw_dest;
}

void GDAPI rebel_vector2_set_x(rebel_vector2* p_self, const rebel_real p_x) {
    Vector2* self = (Vector2*)p_self;
    self->x       = p_x;
}

void GDAPI rebel_vector2_set_y(rebel_vector2* p_self, const rebel_real p_y) {
    Vector2* self = (Vector2*)p_self;
    self->y       = p_y;
}

rebel_real GDAPI rebel_vector2_get_x(const rebel_vector2* p_self) {
    const Vector2* self = (const Vector2*)p_self;
    return self->x;
}

rebel_real GDAPI rebel_vector2_get_y(const rebel_vector2* p_self) {
    const Vector2* self = (const Vector2*)p_self;
    return self->y;
}

#ifdef __cplusplus
}
#endif
