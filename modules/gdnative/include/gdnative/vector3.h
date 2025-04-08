// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_VECTOR3_H
#define GDNATIVE_VECTOR3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_VECTOR3_SIZE 12

#ifndef REBEL_CORE_API_REBEL_VECTOR3_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_VECTOR3_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_VECTOR3_SIZE];
} rebel_vector3;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/basis.h>
#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    REBEL_VECTOR3_AXIS_X,
    REBEL_VECTOR3_AXIS_Y,
    REBEL_VECTOR3_AXIS_Z,
} rebel_vector3_axis;

void GDAPI rebel_vector3_new(
    rebel_vector3* r_dest,
    const rebel_real p_x,
    const rebel_real p_y,
    const rebel_real p_z
);

rebel_string GDAPI rebel_vector3_as_string(const rebel_vector3* p_self);

rebel_int GDAPI rebel_vector3_min_axis(const rebel_vector3* p_self);

rebel_int GDAPI rebel_vector3_max_axis(const rebel_vector3* p_self);

rebel_real GDAPI rebel_vector3_length(const rebel_vector3* p_self);

rebel_real GDAPI rebel_vector3_length_squared(const rebel_vector3* p_self);

rebel_bool GDAPI rebel_vector3_is_normalized(const rebel_vector3* p_self);

rebel_vector3 GDAPI rebel_vector3_normalized(const rebel_vector3* p_self);

rebel_vector3 GDAPI rebel_vector3_inverse(const rebel_vector3* p_self);

rebel_vector3 GDAPI
rebel_vector3_snapped(const rebel_vector3* p_self, const rebel_vector3* p_by);

rebel_vector3 GDAPI rebel_vector3_rotated(
    const rebel_vector3* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
);

rebel_vector3 GDAPI rebel_vector3_linear_interpolate(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b,
    const rebel_real p_t
);

rebel_vector3 GDAPI rebel_vector3_cubic_interpolate(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b,
    const rebel_vector3* p_pre_a,
    const rebel_vector3* p_post_b,
    const rebel_real p_t
);

rebel_vector3 GDAPI rebel_vector3_move_toward(
    const rebel_vector3* p_self,
    const rebel_vector3* p_to,
    const rebel_real p_delta
);

rebel_real GDAPI
rebel_vector3_dot(const rebel_vector3* p_self, const rebel_vector3* p_b);

rebel_vector3 GDAPI
rebel_vector3_cross(const rebel_vector3* p_self, const rebel_vector3* p_b);

rebel_basis GDAPI
rebel_vector3_outer(const rebel_vector3* p_self, const rebel_vector3* p_b);

rebel_basis GDAPI rebel_vector3_to_diagonal_matrix(const rebel_vector3* p_self);

rebel_vector3 GDAPI rebel_vector3_abs(const rebel_vector3* p_self);

rebel_vector3 GDAPI rebel_vector3_floor(const rebel_vector3* p_self);

rebel_vector3 GDAPI rebel_vector3_ceil(const rebel_vector3* p_self);

rebel_vector3 GDAPI rebel_vector3_direction_to(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_real GDAPI rebel_vector3_distance_to(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_real GDAPI rebel_vector3_distance_squared_to(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_real GDAPI
rebel_vector3_angle_to(const rebel_vector3* p_self, const rebel_vector3* p_to);

rebel_vector3 GDAPI
rebel_vector3_slide(const rebel_vector3* p_self, const rebel_vector3* p_n);

rebel_vector3 GDAPI
rebel_vector3_bounce(const rebel_vector3* p_self, const rebel_vector3* p_n);

rebel_vector3 GDAPI
rebel_vector3_reflect(const rebel_vector3* p_self, const rebel_vector3* p_n);

rebel_vector3 GDAPI rebel_vector3_operator_add(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_vector3 GDAPI rebel_vector3_operator_subtract(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_vector3 GDAPI rebel_vector3_operator_multiply_vector(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_vector3 GDAPI rebel_vector3_operator_multiply_scalar(
    const rebel_vector3* p_self,
    const rebel_real p_b
);

rebel_vector3 GDAPI rebel_vector3_operator_divide_vector(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_vector3 GDAPI rebel_vector3_operator_divide_scalar(
    const rebel_vector3* p_self,
    const rebel_real p_b
);

rebel_bool GDAPI rebel_vector3_operator_equal(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_bool GDAPI rebel_vector3_operator_less(
    const rebel_vector3* p_self,
    const rebel_vector3* p_b
);

rebel_vector3 GDAPI rebel_vector3_operator_neg(const rebel_vector3* p_self);

void GDAPI rebel_vector3_set_axis(
    rebel_vector3* p_self,
    const rebel_vector3_axis p_axis,
    const rebel_real p_val
);

rebel_real GDAPI rebel_vector3_get_axis(
    const rebel_vector3* p_self,
    const rebel_vector3_axis p_axis
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VECTOR3_H
