// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_QUAT_H
#define GDNATIVE_QUAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_QUAT_SIZE 16

#ifndef REBEL_CORE_API_REBEL_QUAT_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_QUAT_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_QUAT_SIZE];
} rebel_quat;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_quat_new(
    rebel_quat* r_dest,
    const rebel_real p_x,
    const rebel_real p_y,
    const rebel_real p_z,
    const rebel_real p_w
);
void GDAPI rebel_quat_new_with_axis_angle(
    rebel_quat* r_dest,
    const rebel_vector3* p_axis,
    const rebel_real p_angle
);
void GDAPI
rebel_quat_new_with_basis(rebel_quat* r_dest, const rebel_basis* p_basis);
void GDAPI
rebel_quat_new_with_euler(rebel_quat* r_dest, const rebel_vector3* p_euler);

rebel_real GDAPI rebel_quat_get_x(const rebel_quat* p_self);
void GDAPI rebel_quat_set_x(rebel_quat* p_self, const rebel_real val);

rebel_real GDAPI rebel_quat_get_y(const rebel_quat* p_self);
void GDAPI rebel_quat_set_y(rebel_quat* p_self, const rebel_real val);

rebel_real GDAPI rebel_quat_get_z(const rebel_quat* p_self);
void GDAPI rebel_quat_set_z(rebel_quat* p_self, const rebel_real val);

rebel_real GDAPI rebel_quat_get_w(const rebel_quat* p_self);
void GDAPI rebel_quat_set_w(rebel_quat* p_self, const rebel_real val);

rebel_string GDAPI rebel_quat_as_string(const rebel_quat* p_self);

rebel_real GDAPI rebel_quat_length(const rebel_quat* p_self);

rebel_real GDAPI rebel_quat_length_squared(const rebel_quat* p_self);

rebel_quat GDAPI rebel_quat_normalized(const rebel_quat* p_self);

rebel_bool GDAPI rebel_quat_is_normalized(const rebel_quat* p_self);

rebel_quat GDAPI rebel_quat_inverse(const rebel_quat* p_self);

rebel_real GDAPI
rebel_quat_dot(const rebel_quat* p_self, const rebel_quat* p_b);

rebel_vector3 GDAPI
rebel_quat_xform(const rebel_quat* p_self, const rebel_vector3* p_v);

rebel_quat GDAPI rebel_quat_slerp(
    const rebel_quat* p_self,
    const rebel_quat* p_b,
    const rebel_real p_t
);

rebel_quat GDAPI rebel_quat_slerpni(
    const rebel_quat* p_self,
    const rebel_quat* p_b,
    const rebel_real p_t
);

rebel_quat GDAPI rebel_quat_cubic_slerp(
    const rebel_quat* p_self,
    const rebel_quat* p_b,
    const rebel_quat* p_pre_a,
    const rebel_quat* p_post_b,
    const rebel_real p_t
);

rebel_quat GDAPI
rebel_quat_operator_multiply(const rebel_quat* p_self, const rebel_real p_b);

rebel_quat GDAPI
rebel_quat_operator_add(const rebel_quat* p_self, const rebel_quat* p_b);

rebel_quat GDAPI
rebel_quat_operator_subtract(const rebel_quat* p_self, const rebel_quat* p_b);

rebel_quat GDAPI
rebel_quat_operator_divide(const rebel_quat* p_self, const rebel_real p_b);

rebel_bool GDAPI
rebel_quat_operator_equal(const rebel_quat* p_self, const rebel_quat* p_b);

rebel_quat GDAPI rebel_quat_operator_neg(const rebel_quat* p_self);

void GDAPI rebel_quat_set_axis_angle(
    rebel_quat* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_angle
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_QUAT_H
