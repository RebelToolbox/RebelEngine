// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/aabb.h"

#include "core/math/aabb.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(rebel_aabb) == sizeof(AABB), "AABB size mismatch");

void GDAPI rebel_aabb_new(
    rebel_aabb* r_dest,
    const rebel_vector3* p_pos,
    const rebel_vector3* p_size
) {
    const Vector3* pos  = (const Vector3*)p_pos;
    const Vector3* size = (const Vector3*)p_size;
    AABB* dest          = (AABB*)r_dest;
    *dest               = AABB(*pos, *size);
}

rebel_vector3 GDAPI rebel_aabb_get_position(const rebel_aabb* p_self) {
    rebel_vector3 raw_ret;
    const AABB* self = (const AABB*)p_self;
    Vector3* ret     = (Vector3*)&raw_ret;
    *ret             = self->position;
    return raw_ret;
}

void GDAPI
rebel_aabb_set_position(const rebel_aabb* p_self, const rebel_vector3* p_v) {
    AABB* self       = (AABB*)p_self;
    const Vector3* v = (const Vector3*)p_v;
    self->position   = *v;
}

rebel_vector3 GDAPI rebel_aabb_get_size(const rebel_aabb* p_self) {
    rebel_vector3 raw_ret;
    const AABB* self = (const AABB*)p_self;
    Vector3* ret     = (Vector3*)&raw_ret;
    *ret             = self->size;
    return raw_ret;
}

void GDAPI
rebel_aabb_set_size(const rebel_aabb* p_self, const rebel_vector3* p_v) {
    AABB* self       = (AABB*)p_self;
    const Vector3* v = (const Vector3*)p_v;
    self->size       = *v;
}

rebel_string GDAPI rebel_aabb_as_string(const rebel_aabb* p_self) {
    rebel_string ret;
    const AABB* self = (const AABB*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_real GDAPI rebel_aabb_get_area(const rebel_aabb* p_self) {
    const AABB* self = (const AABB*)p_self;
    return self->get_area();
}

rebel_bool GDAPI rebel_aabb_has_no_area(const rebel_aabb* p_self) {
    const AABB* self = (const AABB*)p_self;
    return self->has_no_area();
}

rebel_bool GDAPI rebel_aabb_has_no_surface(const rebel_aabb* p_self) {
    const AABB* self = (const AABB*)p_self;
    return self->has_no_surface();
}

rebel_bool GDAPI
rebel_aabb_intersects(const rebel_aabb* p_self, const rebel_aabb* p_with) {
    const AABB* self = (const AABB*)p_self;
    const AABB* with = (const AABB*)p_with;
    return self->intersects(*with);
}

rebel_bool GDAPI
rebel_aabb_encloses(const rebel_aabb* p_self, const rebel_aabb* p_with) {
    const AABB* self = (const AABB*)p_self;
    const AABB* with = (const AABB*)p_with;
    return self->encloses(*with);
}

rebel_aabb GDAPI
rebel_aabb_merge(const rebel_aabb* p_self, const rebel_aabb* p_with) {
    rebel_aabb dest;
    const AABB* self = (const AABB*)p_self;
    const AABB* with = (const AABB*)p_with;
    *((AABB*)&dest)  = self->merge(*with);
    return dest;
}

rebel_aabb GDAPI
rebel_aabb_intersection(const rebel_aabb* p_self, const rebel_aabb* p_with) {
    rebel_aabb dest;
    const AABB* self = (const AABB*)p_self;
    const AABB* with = (const AABB*)p_with;
    *((AABB*)&dest)  = self->intersection(*with);
    return dest;
}

rebel_bool GDAPI rebel_aabb_intersects_plane(
    const rebel_aabb* p_self,
    const rebel_plane* p_plane
) {
    const AABB* self   = (const AABB*)p_self;
    const Plane* plane = (const Plane*)p_plane;
    return self->intersects_plane(*plane);
}

rebel_bool GDAPI rebel_aabb_intersects_segment(
    const rebel_aabb* p_self,
    const rebel_vector3* p_from,
    const rebel_vector3* p_to
) {
    const AABB* self    = (const AABB*)p_self;
    const Vector3* from = (const Vector3*)p_from;
    const Vector3* to   = (const Vector3*)p_to;
    return self->intersects_segment(*from, *to);
}

rebel_bool GDAPI
rebel_aabb_has_point(const rebel_aabb* p_self, const rebel_vector3* p_point) {
    const AABB* self     = (const AABB*)p_self;
    const Vector3* point = (const Vector3*)p_point;
    return self->has_point(*point);
}

rebel_vector3 GDAPI
rebel_aabb_get_support(const rebel_aabb* p_self, const rebel_vector3* p_dir) {
    rebel_vector3 dest;
    const AABB* self   = (const AABB*)p_self;
    const Vector3* dir = (const Vector3*)p_dir;
    *((Vector3*)&dest) = self->get_support(*dir);
    return dest;
}

rebel_vector3 GDAPI rebel_aabb_get_longest_axis(const rebel_aabb* p_self) {
    rebel_vector3 dest;
    const AABB* self   = (const AABB*)p_self;
    *((Vector3*)&dest) = self->get_longest_axis();
    return dest;
}

rebel_int GDAPI rebel_aabb_get_longest_axis_index(const rebel_aabb* p_self) {
    const AABB* self = (const AABB*)p_self;
    return self->get_longest_axis_index();
}

rebel_real GDAPI rebel_aabb_get_longest_axis_size(const rebel_aabb* p_self) {
    const AABB* self = (const AABB*)p_self;
    return self->get_longest_axis_size();
}

rebel_vector3 GDAPI rebel_aabb_get_shortest_axis(const rebel_aabb* p_self) {
    rebel_vector3 dest;
    const AABB* self   = (const AABB*)p_self;
    *((Vector3*)&dest) = self->get_shortest_axis();
    return dest;
}

rebel_int GDAPI rebel_aabb_get_shortest_axis_index(const rebel_aabb* p_self) {
    const AABB* self = (const AABB*)p_self;
    return self->get_shortest_axis_index();
}

rebel_real GDAPI rebel_aabb_get_shortest_axis_size(const rebel_aabb* p_self) {
    const AABB* self = (const AABB*)p_self;
    return self->get_shortest_axis_size();
}

rebel_aabb GDAPI
rebel_aabb_expand(const rebel_aabb* p_self, const rebel_vector3* p_to_point) {
    rebel_aabb dest;
    const AABB* self        = (const AABB*)p_self;
    const Vector3* to_point = (const Vector3*)p_to_point;
    *((AABB*)&dest)         = self->expand(*to_point);
    return dest;
}

rebel_aabb GDAPI
rebel_aabb_grow(const rebel_aabb* p_self, const rebel_real p_by) {
    rebel_aabb dest;
    const AABB* self = (const AABB*)p_self;

    *((AABB*)&dest) = self->grow(p_by);
    return dest;
}

rebel_vector3 GDAPI
rebel_aabb_get_endpoint(const rebel_aabb* p_self, const rebel_int p_idx) {
    rebel_vector3 dest;
    const AABB* self = (const AABB*)p_self;

    *((Vector3*)&dest) = self->get_endpoint(p_idx);
    return dest;
}

rebel_bool GDAPI
rebel_aabb_operator_equal(const rebel_aabb* p_self, const rebel_aabb* p_b) {
    const AABB* self = (const AABB*)p_self;
    const AABB* b    = (const AABB*)p_b;
    return *self == *b;
}

#ifdef __cplusplus
}
#endif
