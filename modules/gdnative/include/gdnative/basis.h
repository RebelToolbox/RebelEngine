// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_BASIS_H
#define GDNATIVE_BASIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_BASIS_SIZE 36

#ifndef REBEL_CORE_API_REBEL_BASIS_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_BASIS_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_BASIS_SIZE];
} rebel_basis;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/quat.h>
#include <gdnative/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_basis_new_with_rows(
    rebel_basis* r_dest,
    const rebel_vector3* p_x_axis,
    const rebel_vector3* p_y_axis,
    const rebel_vector3* p_z_axis
);
void GDAPI rebel_basis_new_with_axis_and_angle(
    rebel_basis* r_dest,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
);
void GDAPI
rebel_basis_new_with_euler(rebel_basis* r_dest, const rebel_vector3* p_euler);
void GDAPI
rebel_basis_new_with_euler_quat(rebel_basis* r_dest, const rebel_quat* p_euler);

rebel_string GDAPI rebel_basis_as_string(const rebel_basis* p_self);

rebel_basis GDAPI rebel_basis_inverse(const rebel_basis* p_self);

rebel_basis GDAPI rebel_basis_transposed(const rebel_basis* p_self);

rebel_basis GDAPI rebel_basis_orthonormalized(const rebel_basis* p_self);

rebel_real GDAPI rebel_basis_determinant(const rebel_basis* p_self);

rebel_basis GDAPI rebel_basis_rotated(
    const rebel_basis* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
);

rebel_basis GDAPI
rebel_basis_scaled(const rebel_basis* p_self, const rebel_vector3* p_scale);

rebel_vector3 GDAPI rebel_basis_get_scale(const rebel_basis* p_self);

rebel_vector3 GDAPI rebel_basis_get_euler(const rebel_basis* p_self);

rebel_quat GDAPI rebel_basis_get_quat(const rebel_basis* p_self);

void GDAPI rebel_basis_set_quat(rebel_basis* p_self, const rebel_quat* p_quat);

void GDAPI rebel_basis_set_axis_angle_scale(
    rebel_basis* p_self,
    const rebel_vector3* p_axis,
    rebel_real p_phi,
    const rebel_vector3* p_scale
);

void GDAPI rebel_basis_set_euler_scale(
    rebel_basis* p_self,
    const rebel_vector3* p_euler,
    const rebel_vector3* p_scale
);

void GDAPI rebel_basis_set_quat_scale(
    rebel_basis* p_self,
    const rebel_quat* p_quat,
    const rebel_vector3* p_scale
);

rebel_real GDAPI
rebel_basis_tdotx(const rebel_basis* p_self, const rebel_vector3* p_with);

rebel_real GDAPI
rebel_basis_tdoty(const rebel_basis* p_self, const rebel_vector3* p_with);

rebel_real GDAPI
rebel_basis_tdotz(const rebel_basis* p_self, const rebel_vector3* p_with);

rebel_vector3 GDAPI
rebel_basis_xform(const rebel_basis* p_self, const rebel_vector3* p_v);

rebel_vector3 GDAPI
rebel_basis_xform_inv(const rebel_basis* p_self, const rebel_vector3* p_v);

rebel_int GDAPI rebel_basis_get_orthogonal_index(const rebel_basis* p_self);

void GDAPI rebel_basis_new(rebel_basis* r_dest);

// p_elements is a pointer to an array of 3 (!!) vector3
void GDAPI
rebel_basis_get_elements(const rebel_basis* p_self, rebel_vector3* p_elements);

rebel_vector3 GDAPI
rebel_basis_get_axis(const rebel_basis* p_self, const rebel_int p_axis);

void GDAPI rebel_basis_set_axis(
    rebel_basis* p_self,
    const rebel_int p_axis,
    const rebel_vector3* p_value
);

rebel_vector3 GDAPI
rebel_basis_get_row(const rebel_basis* p_self, const rebel_int p_row);

void GDAPI rebel_basis_set_row(
    rebel_basis* p_self,
    const rebel_int p_row,
    const rebel_vector3* p_value
);

rebel_bool GDAPI
rebel_basis_operator_equal(const rebel_basis* p_self, const rebel_basis* p_b);

rebel_basis GDAPI
rebel_basis_operator_add(const rebel_basis* p_self, const rebel_basis* p_b);

rebel_basis GDAPI rebel_basis_operator_subtract(
    const rebel_basis* p_self,
    const rebel_basis* p_b
);

rebel_basis GDAPI rebel_basis_operator_multiply_vector(
    const rebel_basis* p_self,
    const rebel_basis* p_b
);

rebel_basis GDAPI rebel_basis_operator_multiply_scalar(
    const rebel_basis* p_self,
    const rebel_real p_b
);

rebel_basis GDAPI rebel_basis_slerp(
    const rebel_basis* p_self,
    const rebel_basis* p_b,
    const rebel_real p_t
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_BASIS_H
