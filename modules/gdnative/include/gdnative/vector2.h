// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOT_VECTOR2_H
#define GODOT_VECTOR2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define GODOT_VECTOR2_SIZE 8

#ifndef GODOT_CORE_API_GODOT_VECTOR2_TYPE_DEFINED
#define GODOT_CORE_API_GODOT_VECTOR2_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[GODOT_VECTOR2_SIZE];
} godot_vector2;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI godot_vector2_new(
    godot_vector2* r_dest,
    const godot_real p_x,
    const godot_real p_y
);

godot_string GDAPI godot_vector2_as_string(const godot_vector2* p_self);

godot_vector2 GDAPI godot_vector2_normalized(const godot_vector2* p_self);

godot_real GDAPI godot_vector2_length(const godot_vector2* p_self);

godot_real GDAPI godot_vector2_angle(const godot_vector2* p_self);

godot_real GDAPI godot_vector2_length_squared(const godot_vector2* p_self);

godot_bool GDAPI godot_vector2_is_normalized(const godot_vector2* p_self);

godot_vector2 GDAPI godot_vector2_direction_to(
    const godot_vector2* p_self,
    const godot_vector2* p_b
);

godot_real GDAPI godot_vector2_distance_to(
    const godot_vector2* p_self,
    const godot_vector2* p_to
);

godot_real GDAPI godot_vector2_distance_squared_to(
    const godot_vector2* p_self,
    const godot_vector2* p_to
);

godot_real GDAPI
godot_vector2_angle_to(const godot_vector2* p_self, const godot_vector2* p_to);

godot_real GDAPI godot_vector2_angle_to_point(
    const godot_vector2* p_self,
    const godot_vector2* p_to
);

godot_vector2 GDAPI godot_vector2_linear_interpolate(
    const godot_vector2* p_self,
    const godot_vector2* p_b,
    const godot_real p_t
);

godot_vector2 GDAPI godot_vector2_cubic_interpolate(
    const godot_vector2* p_self,
    const godot_vector2* p_b,
    const godot_vector2* p_pre_a,
    const godot_vector2* p_post_b,
    const godot_real p_t
);

godot_vector2 GDAPI godot_vector2_move_toward(
    const godot_vector2* p_self,
    const godot_vector2* p_to,
    const godot_real p_delta
);

godot_vector2 GDAPI
godot_vector2_rotated(const godot_vector2* p_self, const godot_real p_phi);

godot_vector2 GDAPI godot_vector2_tangent(const godot_vector2* p_self);

godot_vector2 GDAPI godot_vector2_floor(const godot_vector2* p_self);

godot_vector2 GDAPI
godot_vector2_snapped(const godot_vector2* p_self, const godot_vector2* p_by);

godot_real GDAPI godot_vector2_aspect(const godot_vector2* p_self);

godot_real GDAPI
godot_vector2_dot(const godot_vector2* p_self, const godot_vector2* p_with);

godot_vector2 GDAPI
godot_vector2_slide(const godot_vector2* p_self, const godot_vector2* p_n);

godot_vector2 GDAPI
godot_vector2_bounce(const godot_vector2* p_self, const godot_vector2* p_n);

godot_vector2 GDAPI
godot_vector2_reflect(const godot_vector2* p_self, const godot_vector2* p_n);

godot_vector2 GDAPI godot_vector2_abs(const godot_vector2* p_self);

godot_vector2 GDAPI
godot_vector2_clamped(const godot_vector2* p_self, const godot_real p_length);

godot_vector2 GDAPI godot_vector2_operator_add(
    const godot_vector2* p_self,
    const godot_vector2* p_b
);

godot_vector2 GDAPI godot_vector2_operator_subtract(
    const godot_vector2* p_self,
    const godot_vector2* p_b
);

godot_vector2 GDAPI godot_vector2_operator_multiply_vector(
    const godot_vector2* p_self,
    const godot_vector2* p_b
);

godot_vector2 GDAPI godot_vector2_operator_multiply_scalar(
    const godot_vector2* p_self,
    const godot_real p_b
);

godot_vector2 GDAPI godot_vector2_operator_divide_vector(
    const godot_vector2* p_self,
    const godot_vector2* p_b
);

godot_vector2 GDAPI godot_vector2_operator_divide_scalar(
    const godot_vector2* p_self,
    const godot_real p_b
);

godot_bool GDAPI godot_vector2_operator_equal(
    const godot_vector2* p_self,
    const godot_vector2* p_b
);

godot_bool GDAPI godot_vector2_operator_less(
    const godot_vector2* p_self,
    const godot_vector2* p_b
);

godot_vector2 GDAPI godot_vector2_operator_neg(const godot_vector2* p_self);

void GDAPI godot_vector2_set_x(godot_vector2* p_self, const godot_real p_x);

void GDAPI godot_vector2_set_y(godot_vector2* p_self, const godot_real p_y);

godot_real GDAPI godot_vector2_get_x(const godot_vector2* p_self);

godot_real GDAPI godot_vector2_get_y(const godot_vector2* p_self);

#ifdef __cplusplus
}
#endif

#endif // GODOT_VECTOR2_H
