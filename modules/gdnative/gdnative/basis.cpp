// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/basis.h"

#include "core/math/basis.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(rebel_basis) == sizeof(Basis), "Basis size mismatch");

void GDAPI rebel_basis_new_with_rows(
    rebel_basis* r_dest,
    const rebel_vector3* p_x_axis,
    const rebel_vector3* p_y_axis,
    const rebel_vector3* p_z_axis
) {
    const Vector3* x_axis = (const Vector3*)p_x_axis;
    const Vector3* y_axis = (const Vector3*)p_y_axis;
    const Vector3* z_axis = (const Vector3*)p_z_axis;
    Basis* dest           = (Basis*)r_dest;
    *dest                 = Basis(*x_axis, *y_axis, *z_axis);
}

void GDAPI rebel_basis_new_with_axis_and_angle(
    rebel_basis* r_dest,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
) {
    const Vector3* axis = (const Vector3*)p_axis;
    Basis* dest         = (Basis*)r_dest;
    *dest               = Basis(*axis, p_phi);
}

void GDAPI
rebel_basis_new_with_euler(rebel_basis* r_dest, const rebel_vector3* p_euler) {
    const Vector3* euler = (const Vector3*)p_euler;
    Basis* dest          = (Basis*)r_dest;
    *dest                = Basis(*euler);
}

rebel_string GDAPI rebel_basis_as_string(const rebel_basis* p_self) {
    rebel_string ret;
    const Basis* self = (const Basis*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_basis GDAPI rebel_basis_inverse(const rebel_basis* p_self) {
    rebel_basis dest;
    const Basis* self = (const Basis*)p_self;
    *((Basis*)&dest)  = self->inverse();
    return dest;
}

rebel_basis GDAPI rebel_basis_transposed(const rebel_basis* p_self) {
    rebel_basis dest;
    const Basis* self = (const Basis*)p_self;
    *((Basis*)&dest)  = self->transposed();
    return dest;
}

rebel_basis GDAPI rebel_basis_orthonormalized(const rebel_basis* p_self) {
    rebel_basis dest;
    const Basis* self = (const Basis*)p_self;
    *((Basis*)&dest)  = self->orthonormalized();
    return dest;
}

rebel_real GDAPI rebel_basis_determinant(const rebel_basis* p_self) {
    const Basis* self = (const Basis*)p_self;
    return self->determinant();
}

rebel_basis GDAPI rebel_basis_rotated(
    const rebel_basis* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_phi
) {
    rebel_basis dest;
    const Basis* self   = (const Basis*)p_self;
    const Vector3* axis = (const Vector3*)p_axis;
    *((Basis*)&dest)    = self->rotated(*axis, p_phi);
    return dest;
}

rebel_basis GDAPI
rebel_basis_scaled(const rebel_basis* p_self, const rebel_vector3* p_scale) {
    rebel_basis dest;
    const Basis* self    = (const Basis*)p_self;
    const Vector3* scale = (const Vector3*)p_scale;
    *((Basis*)&dest)     = self->scaled(*scale);
    return dest;
}

rebel_vector3 GDAPI rebel_basis_get_scale(const rebel_basis* p_self) {
    rebel_vector3 dest;
    const Basis* self  = (const Basis*)p_self;
    *((Vector3*)&dest) = self->get_scale();
    return dest;
}

rebel_quat GDAPI rebel_basis_get_quat(const rebel_basis* p_self) {
    rebel_quat dest;
    const Basis* self = (const Basis*)p_self;
    *((Quat*)&dest)   = self->get_quat();
    return dest;
}

void GDAPI rebel_basis_set_quat(rebel_basis* p_self, const rebel_quat* p_quat) {
    Basis* self      = (Basis*)p_self;
    const Quat* quat = (const Quat*)p_quat;
    self->set_quat(*quat);
}

void GDAPI rebel_basis_set_axis_angle_scale(
    rebel_basis* p_self,
    const rebel_vector3* p_axis,
    rebel_real p_phi,
    const rebel_vector3* p_scale
) {
    Basis* self          = (Basis*)p_self;
    const Vector3* axis  = (const Vector3*)p_axis;
    const Vector3* scale = (const Vector3*)p_scale;
    self->set_axis_angle_scale(*axis, p_phi, *scale);
}

void GDAPI rebel_basis_set_euler_scale(
    rebel_basis* p_self,
    const rebel_vector3* p_euler,
    const rebel_vector3* p_scale
) {
    Basis* self          = (Basis*)p_self;
    const Vector3* euler = (const Vector3*)p_euler;
    const Vector3* scale = (const Vector3*)p_scale;
    self->set_euler_scale(*euler, *scale);
}

void GDAPI rebel_basis_set_quat_scale(
    rebel_basis* p_self,
    const rebel_quat* p_quat,
    const rebel_vector3* p_scale
) {
    Basis* self          = (Basis*)p_self;
    const Quat* quat     = (const Quat*)p_quat;
    const Vector3* scale = (const Vector3*)p_scale;
    self->set_quat_scale(*quat, *scale);
}

rebel_vector3 GDAPI rebel_basis_get_euler(const rebel_basis* p_self) {
    rebel_vector3 dest;
    const Basis* self  = (const Basis*)p_self;
    *((Vector3*)&dest) = self->get_euler();
    return dest;
}

rebel_real GDAPI
rebel_basis_tdotx(const rebel_basis* p_self, const rebel_vector3* p_with) {
    const Basis* self   = (const Basis*)p_self;
    const Vector3* with = (const Vector3*)p_with;
    return self->tdotx(*with);
}

rebel_real GDAPI
rebel_basis_tdoty(const rebel_basis* p_self, const rebel_vector3* p_with) {
    const Basis* self   = (const Basis*)p_self;
    const Vector3* with = (const Vector3*)p_with;
    return self->tdoty(*with);
}

rebel_real GDAPI
rebel_basis_tdotz(const rebel_basis* p_self, const rebel_vector3* p_with) {
    const Basis* self   = (const Basis*)p_self;
    const Vector3* with = (const Vector3*)p_with;
    return self->tdotz(*with);
}

rebel_vector3 GDAPI
rebel_basis_xform(const rebel_basis* p_self, const rebel_vector3* p_v) {
    rebel_vector3 dest;
    const Basis* self  = (const Basis*)p_self;
    const Vector3* v   = (const Vector3*)p_v;
    *((Vector3*)&dest) = self->xform(*v);
    return dest;
}

rebel_vector3 GDAPI
rebel_basis_xform_inv(const rebel_basis* p_self, const rebel_vector3* p_v) {
    rebel_vector3 dest;
    const Basis* self  = (const Basis*)p_self;
    const Vector3* v   = (const Vector3*)p_v;
    *((Vector3*)&dest) = self->xform_inv(*v);
    return dest;
}

rebel_int GDAPI rebel_basis_get_orthogonal_index(const rebel_basis* p_self) {
    const Basis* self = (const Basis*)p_self;
    return self->get_orthogonal_index();
}

void GDAPI rebel_basis_new(rebel_basis* r_dest) {
    Basis* dest = (Basis*)r_dest;
    *dest       = Basis();
}

void GDAPI rebel_basis_new_with_euler_quat(
    rebel_basis* r_dest,
    const rebel_quat* p_euler
) {
    Basis* dest       = (Basis*)r_dest;
    const Quat* euler = (const Quat*)p_euler;
    *dest             = Basis(*euler);
}

// p_elements is a pointer to an array of 3 (!!) vector3
void GDAPI
rebel_basis_get_elements(const rebel_basis* p_self, rebel_vector3* p_elements) {
    const Basis* self = (const Basis*)p_self;
    Vector3* elements = (Vector3*)p_elements;
    elements[0]       = self->elements[0];
    elements[1]       = self->elements[1];
    elements[2]       = self->elements[2];
}

rebel_vector3 GDAPI
rebel_basis_get_axis(const rebel_basis* p_self, const rebel_int p_axis) {
    rebel_vector3 dest;
    Vector3* d        = (Vector3*)&dest;
    const Basis* self = (const Basis*)p_self;
    *d                = self->get_axis(p_axis);
    return dest;
}

void GDAPI rebel_basis_set_axis(
    rebel_basis* p_self,
    const rebel_int p_axis,
    const rebel_vector3* p_value
) {
    Basis* self          = (Basis*)p_self;
    const Vector3* value = (const Vector3*)p_value;
    self->set_axis(p_axis, *value);
}

rebel_vector3 GDAPI
rebel_basis_get_row(const rebel_basis* p_self, const rebel_int p_row) {
    rebel_vector3 dest;
    Vector3* d        = (Vector3*)&dest;
    const Basis* self = (const Basis*)p_self;
    *d                = self->get_row(p_row);
    return dest;
}

void GDAPI rebel_basis_set_row(
    rebel_basis* p_self,
    const rebel_int p_row,
    const rebel_vector3* p_value
) {
    Basis* self          = (Basis*)p_self;
    const Vector3* value = (const Vector3*)p_value;
    self->set_row(p_row, *value);
}

rebel_bool GDAPI
rebel_basis_operator_equal(const rebel_basis* p_self, const rebel_basis* p_b) {
    const Basis* self = (const Basis*)p_self;
    const Basis* b    = (const Basis*)p_b;
    return *self == *b;
}

rebel_basis GDAPI
rebel_basis_operator_add(const rebel_basis* p_self, const rebel_basis* p_b) {
    rebel_basis raw_dest;
    Basis* dest       = (Basis*)&raw_dest;
    const Basis* self = (const Basis*)p_self;
    const Basis* b    = (const Basis*)p_b;
    *dest             = *self + *b;
    return raw_dest;
}

rebel_basis GDAPI rebel_basis_operator_subtract(
    const rebel_basis* p_self,
    const rebel_basis* p_b
) {
    rebel_basis raw_dest;
    Basis* dest       = (Basis*)&raw_dest;
    const Basis* self = (const Basis*)p_self;
    const Basis* b    = (const Basis*)p_b;
    *dest             = *self - *b;
    return raw_dest;
}

rebel_basis GDAPI rebel_basis_operator_multiply_vector(
    const rebel_basis* p_self,
    const rebel_basis* p_b
) {
    rebel_basis raw_dest;
    Basis* dest       = (Basis*)&raw_dest;
    const Basis* self = (const Basis*)p_self;
    const Basis* b    = (const Basis*)p_b;
    *dest             = *self * *b;
    return raw_dest;
}

rebel_basis GDAPI rebel_basis_operator_multiply_scalar(
    const rebel_basis* p_self,
    const rebel_real p_b
) {
    rebel_basis raw_dest;
    Basis* dest       = (Basis*)&raw_dest;
    const Basis* self = (const Basis*)p_self;
    *dest             = *self * p_b;
    return raw_dest;
}

rebel_basis GDAPI rebel_basis_slerp(
    const rebel_basis* p_self,
    const rebel_basis* p_b,
    const rebel_real p_t
) {
    rebel_basis raw_dest;
    Basis* dest       = (Basis*)&raw_dest;
    const Basis* self = (const Basis*)p_self;
    const Basis* b    = (const Basis*)p_b;
    *dest             = self->slerp(*b, p_t);
    return raw_dest;
}

#ifdef __cplusplus
}
#endif
