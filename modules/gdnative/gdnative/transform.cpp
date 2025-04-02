// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/transform.h"

#include "core/math/transform.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_transform) == sizeof(Transform),
    "Transform size mismatch"
);

void GDAPI rebel_transform_new_with_axis_origin(
    rebel_transform* r_dest,
    const rebel_vector3* p_x_axis,
    const rebel_vector3* p_y_axis,
    const rebel_vector3* p_z_axis,
    const rebel_vector3* p_origin
) {
    const Vector3* x_axis = (const Vector3*)p_x_axis;
    const Vector3* y_axis = (const Vector3*)p_y_axis;
    const Vector3* z_axis = (const Vector3*)p_z_axis;
    const Vector3* origin = (const Vector3*)p_origin;
    Transform* dest       = (Transform*)r_dest;
    dest->basis.set_axis(0, *x_axis);
    dest->basis.set_axis(1, *y_axis);
    dest->basis.set_axis(2, *z_axis);
    dest->origin = *origin;
}

void GDAPI rebel_transform_new(
    rebel_transform* r_dest,
    const rebel_basis* p_basis,
    const rebel_vector3* p_origin
) {
    const Basis* basis    = (const Basis*)p_basis;
    const Vector3* origin = (const Vector3*)p_origin;
    Transform* dest       = (Transform*)r_dest;
    *dest                 = Transform(*basis, *origin);
}

void GDAPI rebel_transform_new_with_quat(
    rebel_transform* r_dest,
    const rebel_quat* p_quat
) {
    const Quat* quat = (const Quat*)p_quat;
    Transform* dest  = (Transform*)r_dest;
    *dest            = Transform(*quat);
}

rebel_basis GDAPI rebel_transform_get_basis(const rebel_transform* p_self) {
    rebel_basis dest;
    const Transform* self = (const Transform*)p_self;
    *((Basis*)&dest)      = self->basis;
    return dest;
}

void GDAPI
rebel_transform_set_basis(rebel_transform* p_self, const rebel_basis* p_v) {
    Transform* self = (Transform*)p_self;
    const Basis* v  = (const Basis*)p_v;
    self->basis     = *v;
}

rebel_vector3 GDAPI rebel_transform_get_origin(const rebel_transform* p_self) {
    rebel_vector3 dest;
    const Transform* self = (const Transform*)p_self;
    *((Vector3*)&dest)    = self->origin;
    return dest;
}

void GDAPI
rebel_transform_set_origin(rebel_transform* p_self, const rebel_vector3* p_v) {
    Transform* self  = (Transform*)p_self;
    const Vector3* v = (const Vector3*)p_v;
    self->origin     = *v;
}

rebel_string GDAPI rebel_transform_as_string(const rebel_transform* p_self) {
    rebel_string ret;
    const Transform* self = (const Transform*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_transform GDAPI rebel_transform_inverse(const rebel_transform* p_self) {
    rebel_transform dest;
    const Transform* self = (const Transform*)p_self;
    *((Transform*)&dest)  = self->inverse();
    return dest;
}

rebel_transform GDAPI
rebel_transform_affine_inverse(const rebel_transform* p_self) {
    rebel_transform dest;
    const Transform* self = (const Transform*)p_self;
    *((Transform*)&dest)  = self->affine_inverse();
    return dest;
}

rebel_transform GDAPI
rebel_transform_orthonormalized(const rebel_transform* p_self) {
    rebel_transform dest;
    const Transform* self = (const Transform*)p_self;
    *((Transform*)&dest)  = self->orthonormalized();
    return dest;
}

rebel_transform GDAPI rebel_transform_rotated(
    const rebel_transform* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
) {
    rebel_transform dest;
    const Transform* self = (const Transform*)p_self;
    const Vector3* axis   = (const Vector3*)p_axis;
    *((Transform*)&dest)  = self->rotated(*axis, p_phi);
    return dest;
}

rebel_transform GDAPI rebel_transform_scaled(
    const rebel_transform* p_self,
    const rebel_vector3* p_scale
) {
    rebel_transform dest;
    const Transform* self = (const Transform*)p_self;
    const Vector3* scale  = (const Vector3*)p_scale;
    *((Transform*)&dest)  = self->scaled(*scale);
    return dest;
}

rebel_transform GDAPI rebel_transform_translated(
    const rebel_transform* p_self,
    const rebel_vector3* p_ofs
) {
    rebel_transform dest;
    const Transform* self = (const Transform*)p_self;
    const Vector3* ofs    = (const Vector3*)p_ofs;
    *((Transform*)&dest)  = self->translated(*ofs);
    return dest;
}

rebel_transform GDAPI rebel_transform_looking_at(
    const rebel_transform* p_self,
    const rebel_vector3* p_target,
    const rebel_vector3* p_up
) {
    rebel_transform dest;
    const Transform* self = (const Transform*)p_self;
    const Vector3* target = (const Vector3*)p_target;
    const Vector3* up     = (const Vector3*)p_up;
    *((Transform*)&dest)  = self->looking_at(*target, *up);
    return dest;
}

rebel_plane GDAPI rebel_transform_xform_plane(
    const rebel_transform* p_self,
    const rebel_plane* p_v
) {
    rebel_plane raw_dest;
    Plane* dest           = (Plane*)&raw_dest;
    const Transform* self = (const Transform*)p_self;
    const Plane* v        = (const Plane*)p_v;
    *dest                 = self->xform(*v);
    return raw_dest;
}

rebel_plane GDAPI rebel_transform_xform_inv_plane(
    const rebel_transform* p_self,
    const rebel_plane* p_v
) {
    rebel_plane raw_dest;
    Plane* dest           = (Plane*)&raw_dest;
    const Transform* self = (const Transform*)p_self;
    const Plane* v        = (const Plane*)p_v;
    *dest                 = self->xform_inv(*v);
    return raw_dest;
}

void GDAPI rebel_transform_new_identity(rebel_transform* r_dest) {
    Transform* dest = (Transform*)r_dest;
    *dest           = Transform();
}

rebel_bool GDAPI rebel_transform_operator_equal(
    const rebel_transform* p_self,
    const rebel_transform* p_b
) {
    const Transform* self = (const Transform*)p_self;
    const Transform* b    = (const Transform*)p_b;
    return *self == *b;
}

rebel_transform GDAPI rebel_transform_operator_multiply(
    const rebel_transform* p_self,
    const rebel_transform* p_b
) {
    rebel_transform raw_dest;
    Transform* dest       = (Transform*)&raw_dest;
    const Transform* self = (const Transform*)p_self;
    const Transform* b    = (const Transform*)p_b;
    *dest                 = *self * *b;
    return raw_dest;
}

rebel_vector3 GDAPI rebel_transform_xform_vector3(
    const rebel_transform* p_self,
    const rebel_vector3* p_v
) {
    rebel_vector3 raw_dest;
    Vector3* dest         = (Vector3*)&raw_dest;
    const Transform* self = (const Transform*)p_self;
    const Vector3* v      = (const Vector3*)p_v;
    *dest                 = self->xform(*v);
    return raw_dest;
}

rebel_vector3 GDAPI rebel_transform_xform_inv_vector3(
    const rebel_transform* p_self,
    const rebel_vector3* p_v
) {
    rebel_vector3 raw_dest;
    Vector3* dest         = (Vector3*)&raw_dest;
    const Transform* self = (const Transform*)p_self;
    const Vector3* v      = (const Vector3*)p_v;
    *dest                 = self->xform_inv(*v);
    return raw_dest;
}

rebel_aabb GDAPI rebel_transform_xform_aabb(
    const rebel_transform* p_self,
    const rebel_aabb* p_v
) {
    rebel_aabb raw_dest;
    AABB* dest            = (AABB*)&raw_dest;
    const Transform* self = (const Transform*)p_self;
    const AABB* v         = (const AABB*)p_v;
    *dest                 = self->xform(*v);
    return raw_dest;
}

rebel_aabb GDAPI rebel_transform_xform_inv_aabb(
    const rebel_transform* p_self,
    const rebel_aabb* p_v
) {
    rebel_aabb raw_dest;
    AABB* dest            = (AABB*)&raw_dest;
    const Transform* self = (const Transform*)p_self;
    const AABB* v         = (const AABB*)p_v;
    *dest                 = self->xform_inv(*v);
    return raw_dest;
}

#ifdef __cplusplus
}
#endif
