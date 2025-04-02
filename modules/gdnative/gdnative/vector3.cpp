// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/vector3.h"

#include "core/variant.h"
#include "core/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_vector3) == sizeof(Vector3),
    "Vector3 size mismatch"
);

void GDAPI rebel_vector3_new(
    rebel_vector3* r_dest,
    const rebel_real p_x,
    const rebel_real p_y,
    const rebel_real p_z
) {
    Vector3* dest = (Vector3*)r_dest;
    *dest         = Vector3(p_x, p_y, p_z);
}

rebel_string GDAPI rebel_vector3_as_string(const rebel_vector3* p_self) {
    rebel_string ret;
    const Vector3* self = (const Vector3*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_int GDAPI rebel_vector3_min_axis(const rebel_vector3* p_self) {
    const Vector3* self = (const Vector3*)p_self;
    return self->min_axis();
}

rebel_int GDAPI rebel_vector3_max_axis(const rebel_vector3* p_self) {
    const Vector3* self = (const Vector3*)p_self;
    return self->max_axis();
}

rebel_real GDAPI rebel_vector3_length(const rebel_vector3* p_self) {
    const Vector3* self = (const Vector3*)p_self;
    return self->length();
}

rebel_real GDAPI rebel_vector3_length_squared(const rebel_vector3* p_self) {
    const Vector3* self = (const Vector3*)p_self;
    return self->length_squared();
}

rebel_bool GDAPI rebel_vector3_is_normalized(const rebel_vector3* p_self) {
    const Vector3* self = (const Vector3*)p_self;
    return self->is_normalized();
}

rebel_vector3 GDAPI rebel_vector3_normalized(const rebel_vector3* p_self) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    *((Vector3*)&dest)  = self->normalized();
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_inverse(const rebel_vector3* p_self) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    *((Vector3*)&dest)  = self->inverse();
    return dest;
}

rebel_vector3 GDAPI
rebel_vector3_snapped(const rebel_vector3* p_self, const rebel_vector3* p_by) {
    rebel_vector3 dest;
    const Vector3* self      = (const Vector3*)p_self;
    const Vector3* snap_axis = (const Vector3*)p_by;

    *((Vector3*)&dest) = self->snapped(*snap_axis);
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_rotated(
    const rebel_vector3* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* axis = (const Vector3*)p_axis;
    *((Vector3*)&dest)  = self->rotated(*axis, p_phi);
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_linear_interpolate(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b,
    const rebel_real p_t
) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* b    = (const Vector3*)p_b;
    *((Vector3*)&dest)  = self->linear_interpolate(*b, p_t);
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_cubic_interpolate(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b,
    const rebel_vector3* p_pre_a,
    const rebel_vector3* p_post_b,
    const rebel_real p_t
) {
    rebel_vector3 dest;
    const Vector3* self   = (const Vector3*)p_self;
    const Vector3* b      = (const Vector3*)p_b;
    const Vector3* pre_a  = (const Vector3*)p_pre_a;
    const Vector3* post_b = (const Vector3*)p_post_b;
    *((Vector3*)&dest)    = self->cubic_interpolate(*b, *pre_a, *post_b, p_t);
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_move_toward(
    const rebel_vector3* p_self,
    const rebel_vector3* p_to,
    const rebel_real p_delta
) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* to   = (const Vector3*)p_to;
    *((Vector3*)&dest)  = self->move_toward(*to, p_delta);
    return dest;
}

rebel_real GDAPI
rebel_vector3_dot(const rebel_vector3* p_self, const rebel_vector3* p_b) {
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* b    = (const Vector3*)p_b;
    return self->dot(*b);
}

rebel_vector3 GDAPI
rebel_vector3_cross(const rebel_vector3* p_self, const rebel_vector3* p_b) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* b    = (const Vector3*)p_b;
    *((Vector3*)&dest)  = self->cross(*b);
    return dest;
}

rebel_basis GDAPI
rebel_vector3_outer(const rebel_vector3* p_self, const rebel_vector3* p_b) {
    rebel_basis dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* b    = (const Vector3*)p_b;
    *((Basis*)&dest)    = self->outer(*b);
    return dest;
}

rebel_basis GDAPI rebel_vector3_to_diagonal_matrix(const rebel_vector3* p_self
) {
    rebel_basis dest;
    const Vector3* self = (const Vector3*)p_self;
    *((Basis*)&dest)    = self->to_diagonal_matrix();
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_abs(const rebel_vector3* p_self) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    *((Vector3*)&dest)  = self->abs();
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_floor(const rebel_vector3* p_self) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    *((Vector3*)&dest)  = self->floor();
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_ceil(const rebel_vector3* p_self) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    *((Vector3*)&dest)  = self->ceil();
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_direction_to(
    const rebel_vector3* p_self,
    const rebel_vector3* p_to
) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* to   = (const Vector3*)p_to;
    *((Vector3*)&dest)  = self->direction_to(*to);
    return dest;
}

rebel_real GDAPI rebel_vector3_distance_to(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* b    = (const Vector3*)p_b;
    return self->distance_to(*b);
}

rebel_real GDAPI rebel_vector3_distance_squared_to(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* b    = (const Vector3*)p_b;
    return self->distance_squared_to(*b);
}

rebel_real GDAPI
rebel_vector3_angle_to(const rebel_vector3* p_self, const rebel_vector3* p_to) {
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* to   = (const Vector3*)p_to;
    return self->angle_to(*to);
}

rebel_vector3 GDAPI
rebel_vector3_slide(const rebel_vector3* p_self, const rebel_vector3* p_n) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* n    = (const Vector3*)p_n;
    *((Vector3*)&dest)  = self->slide(*n);
    return dest;
}

rebel_vector3 GDAPI
rebel_vector3_bounce(const rebel_vector3* p_self, const rebel_vector3* p_n) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* n    = (const Vector3*)p_n;
    *((Vector3*)&dest)  = self->bounce(*n);
    return dest;
}

rebel_vector3 GDAPI
rebel_vector3_reflect(const rebel_vector3* p_self, const rebel_vector3* p_n) {
    rebel_vector3 dest;
    const Vector3* self = (const Vector3*)p_self;
    const Vector3* n    = (const Vector3*)p_n;
    *((Vector3*)&dest)  = self->reflect(*n);
    return dest;
}

rebel_vector3 GDAPI rebel_vector3_operator_add(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    rebel_vector3 raw_dest;
    Vector3* dest    = (Vector3*)&raw_dest;
    Vector3* self    = (Vector3*)p_self;
    const Vector3* b = (const Vector3*)p_b;
    *dest            = *self + *b;
    return raw_dest;
}

rebel_vector3 GDAPI rebel_vector3_operator_subtract(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    rebel_vector3 raw_dest;
    Vector3* dest    = (Vector3*)&raw_dest;
    Vector3* self    = (Vector3*)p_self;
    const Vector3* b = (const Vector3*)p_b;
    *dest            = *self - *b;
    return raw_dest;
}

rebel_vector3 GDAPI rebel_vector3_operator_multiply_vector(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    rebel_vector3 raw_dest;
    Vector3* dest    = (Vector3*)&raw_dest;
    Vector3* self    = (Vector3*)p_self;
    const Vector3* b = (const Vector3*)p_b;
    *dest            = *self * *b;
    return raw_dest;
}

rebel_vector3 GDAPI rebel_vector3_operator_multiply_scalar(
    const rebel_vector3* p_self,
    const rebel_real p_b
) {
    rebel_vector3 raw_dest;
    Vector3* dest = (Vector3*)&raw_dest;
    Vector3* self = (Vector3*)p_self;
    *dest         = *self * p_b;
    return raw_dest;
}

rebel_vector3 GDAPI rebel_vector3_operator_divide_vector(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    rebel_vector3 raw_dest;
    Vector3* dest    = (Vector3*)&raw_dest;
    Vector3* self    = (Vector3*)p_self;
    const Vector3* b = (const Vector3*)p_b;
    *dest            = *self / *b;
    return raw_dest;
}

rebel_vector3 GDAPI rebel_vector3_operator_divide_scalar(
    const rebel_vector3* p_self,
    const rebel_real p_b
) {
    rebel_vector3 raw_dest;
    Vector3* dest = (Vector3*)&raw_dest;
    Vector3* self = (Vector3*)p_self;
    *dest         = *self / p_b;
    return raw_dest;
}

rebel_bool GDAPI rebel_vector3_operator_equal(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    Vector3* self    = (Vector3*)p_self;
    const Vector3* b = (const Vector3*)p_b;
    return *self == *b;
}

rebel_bool GDAPI rebel_vector3_operator_less(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
) {
    Vector3* self    = (Vector3*)p_self;
    const Vector3* b = (const Vector3*)p_b;
    return *self < *b;
}

rebel_vector3 GDAPI rebel_vector3_operator_neg(const rebel_vector3* p_self) {
    rebel_vector3 raw_dest;
    Vector3* dest       = (Vector3*)&raw_dest;
    const Vector3* self = (const Vector3*)p_self;
    *dest               = -(*self);
    return raw_dest;
}

void GDAPI rebel_vector3_set_axis(
    rebel_vector3* p_self,
    const rebel_vector3_axis p_axis,
    const rebel_real p_val
) {
    Vector3* self = (Vector3*)p_self;
    self->set_axis(p_axis, p_val);
}

rebel_real GDAPI rebel_vector3_get_axis(
    const rebel_vector3* p_self,
    const rebel_vector3_axis p_axis
) {
    const Vector3* self = (const Vector3*)p_self;
    return self->get_axis(p_axis);
}

#ifdef __cplusplus
}
#endif
