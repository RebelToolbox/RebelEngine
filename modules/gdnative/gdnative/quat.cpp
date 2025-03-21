// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/quat.h"

#include "core/math/quat.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(rebel_quat) == sizeof(Quat), "Quat size mismatch");

void GDAPI rebel_quat_new(
    rebel_quat* r_dest,
    const rebel_real p_x,
    const rebel_real p_y,
    const rebel_real p_z,
    const rebel_real p_w
) {
    Quat* dest = (Quat*)r_dest;
    *dest      = Quat(p_x, p_y, p_z, p_w);
}

void GDAPI rebel_quat_new_with_axis_angle(
    rebel_quat* r_dest,
    const rebel_vector3* p_axis,
    const rebel_real p_angle
) {
    const Vector3* axis = (const Vector3*)p_axis;
    Quat* dest          = (Quat*)r_dest;
    *dest               = Quat(*axis, p_angle);
}

void GDAPI
rebel_quat_new_with_basis(rebel_quat* r_dest, const rebel_basis* p_basis) {
    const Basis* basis = (const Basis*)p_basis;
    Quat* dest         = (Quat*)r_dest;
    *dest              = Quat(*basis);
}

void GDAPI
rebel_quat_new_with_euler(rebel_quat* r_dest, const rebel_vector3* p_euler) {
    const Vector3* euler = (const Vector3*)p_euler;
    Quat* dest           = (Quat*)r_dest;
    *dest                = Quat(*euler);
}

rebel_real GDAPI rebel_quat_get_x(const rebel_quat* p_self) {
    const Quat* self = (const Quat*)p_self;
    return self->x;
}

void GDAPI rebel_quat_set_x(rebel_quat* p_self, const rebel_real val) {
    Quat* self = (Quat*)p_self;
    self->x    = val;
}

rebel_real GDAPI rebel_quat_get_y(const rebel_quat* p_self) {
    const Quat* self = (const Quat*)p_self;
    return self->y;
}

void GDAPI rebel_quat_set_y(rebel_quat* p_self, const rebel_real val) {
    Quat* self = (Quat*)p_self;
    self->y    = val;
}

rebel_real GDAPI rebel_quat_get_z(const rebel_quat* p_self) {
    const Quat* self = (const Quat*)p_self;
    return self->z;
}

void GDAPI rebel_quat_set_z(rebel_quat* p_self, const rebel_real val) {
    Quat* self = (Quat*)p_self;
    self->z    = val;
}

rebel_real GDAPI rebel_quat_get_w(const rebel_quat* p_self) {
    const Quat* self = (const Quat*)p_self;
    return self->w;
}

void GDAPI rebel_quat_set_w(rebel_quat* p_self, const rebel_real val) {
    Quat* self = (Quat*)p_self;
    self->w    = val;
}

rebel_string GDAPI rebel_quat_as_string(const rebel_quat* p_self) {
    rebel_string ret;
    const Quat* self = (const Quat*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_real GDAPI rebel_quat_length(const rebel_quat* p_self) {
    const Quat* self = (const Quat*)p_self;
    return self->length();
}

rebel_real GDAPI rebel_quat_length_squared(const rebel_quat* p_self) {
    const Quat* self = (const Quat*)p_self;
    return self->length_squared();
}

rebel_quat GDAPI rebel_quat_normalized(const rebel_quat* p_self) {
    rebel_quat dest;
    const Quat* self = (const Quat*)p_self;
    *((Quat*)&dest)  = self->normalized();
    return dest;
}

rebel_bool GDAPI rebel_quat_is_normalized(const rebel_quat* p_self) {
    const Quat* self = (const Quat*)p_self;
    return self->is_normalized();
}

rebel_quat GDAPI rebel_quat_inverse(const rebel_quat* p_self) {
    rebel_quat dest;
    const Quat* self = (const Quat*)p_self;
    *((Quat*)&dest)  = self->inverse();
    return dest;
}

rebel_real GDAPI
rebel_quat_dot(const rebel_quat* p_self, const rebel_quat* p_b) {
    const Quat* self = (const Quat*)p_self;
    const Quat* b    = (const Quat*)p_b;
    return self->dot(*b);
}

rebel_vector3 GDAPI
rebel_quat_xform(const rebel_quat* p_self, const rebel_vector3* p_v) {
    rebel_vector3 dest;
    const Quat* self   = (const Quat*)p_self;
    const Vector3* v   = (const Vector3*)p_v;
    *((Vector3*)&dest) = self->xform(*v);
    return dest;
}

rebel_quat GDAPI rebel_quat_slerp(
    const rebel_quat* p_self,
    const rebel_quat* p_b,
    const rebel_real p_t
) {
    rebel_quat dest;
    const Quat* self = (const Quat*)p_self;
    const Quat* b    = (const Quat*)p_b;
    *((Quat*)&dest)  = self->slerp(*b, p_t);
    return dest;
}

rebel_quat GDAPI rebel_quat_slerpni(
    const rebel_quat* p_self,
    const rebel_quat* p_b,
    const rebel_real p_t
) {
    rebel_quat dest;
    const Quat* self = (const Quat*)p_self;
    const Quat* b    = (const Quat*)p_b;
    *((Quat*)&dest)  = self->slerpni(*b, p_t);
    return dest;
}

rebel_quat GDAPI rebel_quat_cubic_slerp(
    const rebel_quat* p_self,
    const rebel_quat* p_b,
    const rebel_quat* p_pre_a,
    const rebel_quat* p_post_b,
    const rebel_real p_t
) {
    rebel_quat dest;
    const Quat* self   = (const Quat*)p_self;
    const Quat* b      = (const Quat*)p_b;
    const Quat* pre_a  = (const Quat*)p_pre_a;
    const Quat* post_b = (const Quat*)p_post_b;
    *((Quat*)&dest)    = self->cubic_slerp(*b, *pre_a, *post_b, p_t);
    return dest;
}

rebel_quat GDAPI
rebel_quat_operator_multiply(const rebel_quat* p_self, const rebel_real p_b) {
    rebel_quat raw_dest;
    Quat* dest       = (Quat*)&raw_dest;
    const Quat* self = (const Quat*)p_self;
    *dest            = *self * p_b;
    return raw_dest;
}

rebel_quat GDAPI
rebel_quat_operator_add(const rebel_quat* p_self, const rebel_quat* p_b) {
    rebel_quat raw_dest;
    Quat* dest       = (Quat*)&raw_dest;
    const Quat* self = (const Quat*)p_self;
    const Quat* b    = (const Quat*)p_b;
    *dest            = *self + *b;
    return raw_dest;
}

rebel_quat GDAPI
rebel_quat_operator_subtract(const rebel_quat* p_self, const rebel_quat* p_b) {
    rebel_quat raw_dest;
    Quat* dest       = (Quat*)&raw_dest;
    const Quat* self = (const Quat*)p_self;
    const Quat* b    = (const Quat*)p_b;
    *dest            = *self - *b;
    return raw_dest;
}

rebel_quat GDAPI
rebel_quat_operator_divide(const rebel_quat* p_self, const rebel_real p_b) {
    rebel_quat raw_dest;
    Quat* dest       = (Quat*)&raw_dest;
    const Quat* self = (const Quat*)p_self;
    *dest            = *self / p_b;
    return raw_dest;
}

rebel_bool GDAPI
rebel_quat_operator_equal(const rebel_quat* p_self, const rebel_quat* p_b) {
    const Quat* self = (const Quat*)p_self;
    const Quat* b    = (const Quat*)p_b;
    return *self == *b;
}

rebel_quat GDAPI rebel_quat_operator_neg(const rebel_quat* p_self) {
    rebel_quat raw_dest;
    Quat* dest       = (Quat*)&raw_dest;
    const Quat* self = (const Quat*)p_self;
    *dest            = -(*self);
    return raw_dest;
}

void GDAPI rebel_quat_set_axis_angle(
    rebel_quat* p_self,
    const rebel_vector3* p_axis,
    const rebel_real p_angle
) {
    Quat* self          = (Quat*)p_self;
    const Vector3* axis = (const Vector3*)p_axis;
    self->set_axis_angle(*axis, p_angle);
}

#ifdef __cplusplus
}
#endif
