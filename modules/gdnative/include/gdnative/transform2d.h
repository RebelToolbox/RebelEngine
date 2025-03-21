// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_TRANSFORM2D_H
#define GDNATIVE_TRANSFORM2D_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_TRANSFORM2D_SIZE 24

#ifndef REBEL_CORE_API_REBEL_TRANSFORM2D_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_TRANSFORM2D_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_TRANSFORM2D_SIZE];
} rebel_transform2d;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/variant.h>
#include <gdnative/vector2.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_transform2d_new(
    rebel_transform2d* r_dest,
    const rebel_real p_rot,
    const rebel_vector2* p_pos
);
void GDAPI rebel_transform2d_new_axis_origin(
    rebel_transform2d* r_dest,
    const rebel_vector2* p_x_axis,
    const rebel_vector2* p_y_axis,
    const rebel_vector2* p_origin
);

rebel_string GDAPI rebel_transform2d_as_string(const rebel_transform2d* p_self);

rebel_transform2d GDAPI
rebel_transform2d_inverse(const rebel_transform2d* p_self);

rebel_transform2d GDAPI
rebel_transform2d_affine_inverse(const rebel_transform2d* p_self);

rebel_real GDAPI rebel_transform2d_get_rotation(const rebel_transform2d* p_self
);

rebel_vector2 GDAPI rebel_transform2d_get_origin(const rebel_transform2d* p_self
);

rebel_vector2 GDAPI rebel_transform2d_get_scale(const rebel_transform2d* p_self
);

rebel_transform2d GDAPI
rebel_transform2d_orthonormalized(const rebel_transform2d* p_self);

rebel_transform2d GDAPI rebel_transform2d_rotated(
    const rebel_transform2d* p_self,
    const rebel_real p_phi
);

rebel_transform2d GDAPI rebel_transform2d_scaled(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_scale
);

rebel_transform2d GDAPI rebel_transform2d_translated(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_offset
);

rebel_vector2 GDAPI rebel_transform2d_xform_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
);

rebel_vector2 GDAPI rebel_transform2d_xform_inv_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
);

rebel_vector2 GDAPI rebel_transform2d_basis_xform_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
);

rebel_vector2 GDAPI rebel_transform2d_basis_xform_inv_vector2(
    const rebel_transform2d* p_self,
    const rebel_vector2* p_v
);

rebel_transform2d GDAPI rebel_transform2d_interpolate_with(
    const rebel_transform2d* p_self,
    const rebel_transform2d* p_m,
    const rebel_real p_c
);

rebel_bool GDAPI rebel_transform2d_operator_equal(
    const rebel_transform2d* p_self,
    const rebel_transform2d* p_b
);

rebel_transform2d GDAPI rebel_transform2d_operator_multiply(
    const rebel_transform2d* p_self,
    const rebel_transform2d* p_b
);

void GDAPI rebel_transform2d_new_identity(rebel_transform2d* r_dest);

rebel_rect2 GDAPI rebel_transform2d_xform_rect2(
    const rebel_transform2d* p_self,
    const rebel_rect2* p_v
);

rebel_rect2 GDAPI rebel_transform2d_xform_inv_rect2(
    const rebel_transform2d* p_self,
    const rebel_rect2* p_v
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_TRANSFORM2D_H
