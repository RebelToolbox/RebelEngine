// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_TRANSFORM_H
#define GDNATIVE_TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_TRANSFORM_SIZE 48

#ifndef REBEL_CORE_API_REBEL_TRANSFORM_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_TRANSFORM_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_TRANSFORM_SIZE];
} rebel_transform;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/basis.h>
#include <gdnative/gdnative.h>
#include <gdnative/variant.h>
#include <gdnative/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_transform_new_with_axis_origin(
    rebel_transform* r_dest,
    const rebel_vector3* p_x_axis,
    const rebel_vector3* p_y_axis,
    const rebel_vector3* p_z_axis,
    const rebel_vector3* p_origin
);
void GDAPI rebel_transform_new(
    rebel_transform* r_dest,
    const rebel_basis* p_basis,
    const rebel_vector3* p_origin
);
void GDAPI rebel_transform_new_with_quat(
    rebel_transform* r_dest,
    const rebel_quat* p_quat
);

rebel_basis GDAPI rebel_transform_get_basis(const rebel_transform* p_self);
void GDAPI
rebel_transform_set_basis(rebel_transform* p_self, const rebel_basis* p_v);

rebel_vector3 GDAPI rebel_transform_get_origin(const rebel_transform* p_self);
void GDAPI
rebel_transform_set_origin(rebel_transform* p_self, const rebel_vector3* p_v);

rebel_string GDAPI rebel_transform_as_string(const rebel_transform* p_self);

rebel_transform GDAPI rebel_transform_inverse(const rebel_transform* p_self);

rebel_transform GDAPI
rebel_transform_affine_inverse(const rebel_transform* p_self);

rebel_transform GDAPI
rebel_transform_orthonormalized(const rebel_transform* p_self);

rebel_transform GDAPI rebel_transform_rotated(
    const rebel_transform* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
);

rebel_transform GDAPI rebel_transform_scaled(
    const rebel_transform* p_self,
    const rebel_vector3* p_scale
);

rebel_transform GDAPI rebel_transform_translated(
    const rebel_transform* p_self,
    const rebel_vector3* p_ofs
);

rebel_transform GDAPI rebel_transform_looking_at(
    const rebel_transform* p_self,
    const rebel_vector3* p_target,
    const rebel_vector3* p_up
);

rebel_plane GDAPI rebel_transform_xform_plane(
    const rebel_transform* p_self,
    const rebel_plane* p_v
);

rebel_plane GDAPI rebel_transform_xform_inv_plane(
    const rebel_transform* p_self,
    const rebel_plane* p_v
);

void GDAPI rebel_transform_new_identity(rebel_transform* r_dest);

rebel_bool GDAPI rebel_transform_operator_equal(
    const rebel_transform* p_self,
    const rebel_transform* p_b
);

rebel_transform GDAPI rebel_transform_operator_multiply(
    const rebel_transform* p_self,
    const rebel_transform* p_b
);

rebel_vector3 GDAPI rebel_transform_xform_vector3(
    const rebel_transform* p_self,
    const rebel_vector3* p_v
);

rebel_vector3 GDAPI rebel_transform_xform_inv_vector3(
    const rebel_transform* p_self,
    const rebel_vector3* p_v
);

rebel_aabb GDAPI rebel_transform_xform_aabb(
    const rebel_transform* p_self,
    const rebel_aabb* p_v
);

rebel_aabb GDAPI rebel_transform_xform_inv_aabb(
    const rebel_transform* p_self,
    const rebel_aabb* p_v
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_TRANSFORM_H
