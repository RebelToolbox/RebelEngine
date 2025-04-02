// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_VECTOR2_H
#define GDNATIVE_VECTOR2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_VECTOR2_SIZE 8

#ifndef REBEL_CORE_API_REBEL_VECTOR2_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_VECTOR2_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_VECTOR2_SIZE];
} rebel_vector2;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_vector2_new(
    rebel_vector2* r_dest,
    const rebel_real p_x,
    const rebel_real p_y
);

rebel_string GDAPI rebel_vector2_as_string(const rebel_vector2* p_self);

rebel_vector2 GDAPI rebel_vector2_normalized(const rebel_vector2* p_self);

rebel_real GDAPI rebel_vector2_length(const rebel_vector2* p_self);

rebel_real GDAPI rebel_vector2_angle(const rebel_vector2* p_self);

rebel_real GDAPI rebel_vector2_length_squared(const rebel_vector2* p_self);

rebel_bool GDAPI rebel_vector2_is_normalized(const rebel_vector2* p_self);

rebel_vector2 GDAPI rebel_vector2_direction_to(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
);

rebel_real GDAPI rebel_vector2_distance_to(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to
);

rebel_real GDAPI rebel_vector2_distance_squared_to(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to
);

rebel_real GDAPI
rebel_vector2_angle_to(const rebel_vector2* p_self, const rebel_vector2* p_to);

rebel_real GDAPI rebel_vector2_angle_to_point(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to
);

rebel_vector2 GDAPI rebel_vector2_linear_interpolate(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b,
    const rebel_real p_t
);

rebel_vector2 GDAPI rebel_vector2_cubic_interpolate(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b,
    const rebel_vector2* p_pre_a,
    const rebel_vector2* p_post_b,
    const rebel_real p_t
);

rebel_vector2 GDAPI rebel_vector2_move_toward(
    const rebel_vector2* p_self,
    const rebel_vector2* p_to,
    const rebel_real p_delta
);

rebel_vector2 GDAPI
rebel_vector2_rotated(const rebel_vector2* p_self, const rebel_real p_phi);

rebel_vector2 GDAPI rebel_vector2_tangent(const rebel_vector2* p_self);

rebel_vector2 GDAPI rebel_vector2_floor(const rebel_vector2* p_self);

rebel_vector2 GDAPI
rebel_vector2_snapped(const rebel_vector2* p_self, const rebel_vector2* p_by);

rebel_real GDAPI rebel_vector2_aspect(const rebel_vector2* p_self);

rebel_real GDAPI
rebel_vector2_dot(const rebel_vector2* p_self, const rebel_vector2* p_with);

rebel_vector2 GDAPI
rebel_vector2_slide(const rebel_vector2* p_self, const rebel_vector2* p_n);

rebel_vector2 GDAPI
rebel_vector2_bounce(const rebel_vector2* p_self, const rebel_vector2* p_n);

rebel_vector2 GDAPI
rebel_vector2_reflect(const rebel_vector2* p_self, const rebel_vector2* p_n);

rebel_vector2 GDAPI rebel_vector2_abs(const rebel_vector2* p_self);

rebel_vector2 GDAPI
rebel_vector2_clamped(const rebel_vector2* p_self, const rebel_real p_length);

rebel_vector2 GDAPI rebel_vector2_operator_add(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
);

rebel_vector2 GDAPI rebel_vector2_operator_subtract(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
);

rebel_vector2 GDAPI rebel_vector2_operator_multiply_vector(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
);

rebel_vector2 GDAPI rebel_vector2_operator_multiply_scalar(
    const rebel_vector2* p_self,
    const rebel_real p_b
);

rebel_vector2 GDAPI rebel_vector2_operator_divide_vector(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
);

rebel_vector2 GDAPI rebel_vector2_operator_divide_scalar(
    const rebel_vector2* p_self,
    const rebel_real p_b
);

rebel_bool GDAPI rebel_vector2_operator_equal(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
);

rebel_bool GDAPI rebel_vector2_operator_less(
    const rebel_vector2* p_self,
    const rebel_vector2* p_b
);

rebel_vector2 GDAPI rebel_vector2_operator_neg(const rebel_vector2* p_self);

void GDAPI rebel_vector2_set_x(rebel_vector2* p_self, const rebel_real p_x);

void GDAPI rebel_vector2_set_y(rebel_vector2* p_self, const rebel_real p_y);

rebel_real GDAPI rebel_vector2_get_x(const rebel_vector2* p_self);

rebel_real GDAPI rebel_vector2_get_y(const rebel_vector2* p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VECTOR2_H
