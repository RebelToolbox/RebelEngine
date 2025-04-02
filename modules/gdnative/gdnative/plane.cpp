// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/plane.h"

#include "core/math/plane.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(rebel_plane) == sizeof(Plane), "Plane size mismatch");

void GDAPI rebel_plane_new_with_reals(
    rebel_plane* r_dest,
    const rebel_real p_a,
    const rebel_real p_b,
    const rebel_real p_c,
    const rebel_real p_d
) {
    Plane* dest = (Plane*)r_dest;
    *dest       = Plane(p_a, p_b, p_c, p_d);
}

void GDAPI rebel_plane_new_with_vectors(
    rebel_plane* r_dest,
    const rebel_vector3* p_v1,
    const rebel_vector3* p_v2,
    const rebel_vector3* p_v3
) {
    const Vector3* v1 = (const Vector3*)p_v1;
    const Vector3* v2 = (const Vector3*)p_v2;
    const Vector3* v3 = (const Vector3*)p_v3;
    Plane* dest       = (Plane*)r_dest;
    *dest             = Plane(*v1, *v2, *v3);
}

void GDAPI rebel_plane_new_with_normal(
    rebel_plane* r_dest,
    const rebel_vector3* p_normal,
    const rebel_real p_d
) {
    const Vector3* normal = (const Vector3*)p_normal;
    Plane* dest           = (Plane*)r_dest;
    *dest                 = Plane(*normal, p_d);
}

rebel_string GDAPI rebel_plane_as_string(const rebel_plane* p_self) {
    rebel_string ret;
    const Plane* self = (const Plane*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_plane GDAPI rebel_plane_normalized(const rebel_plane* p_self) {
    rebel_plane dest;
    const Plane* self = (const Plane*)p_self;
    *((Plane*)&dest)  = self->normalized();
    return dest;
}

rebel_vector3 GDAPI rebel_plane_center(const rebel_plane* p_self) {
    rebel_vector3 dest;
    const Plane* self  = (const Plane*)p_self;
    *((Vector3*)&dest) = self->center();
    return dest;
}

rebel_vector3 GDAPI rebel_plane_get_any_point(const rebel_plane* p_self) {
    rebel_vector3 dest;
    const Plane* self  = (const Plane*)p_self;
    *((Vector3*)&dest) = self->get_any_point();
    return dest;
}

rebel_bool GDAPI rebel_plane_is_point_over(
    const rebel_plane* p_self,
    const rebel_vector3* p_point
) {
    const Plane* self    = (const Plane*)p_self;
    const Vector3* point = (const Vector3*)p_point;
    return self->is_point_over(*point);
}

rebel_real GDAPI rebel_plane_distance_to(
    const rebel_plane* p_self,
    const rebel_vector3* p_point
) {
    const Plane* self    = (const Plane*)p_self;
    const Vector3* point = (const Vector3*)p_point;
    return self->distance_to(*point);
}

rebel_bool GDAPI rebel_plane_has_point(
    const rebel_plane* p_self,
    const rebel_vector3* p_point,
    const rebel_real p_epsilon
) {
    const Plane* self    = (const Plane*)p_self;
    const Vector3* point = (const Vector3*)p_point;
    return self->has_point(*point, p_epsilon);
}

rebel_vector3 GDAPI
rebel_plane_project(const rebel_plane* p_self, const rebel_vector3* p_point) {
    rebel_vector3 dest;
    const Plane* self    = (const Plane*)p_self;
    const Vector3* point = (const Vector3*)p_point;
    *((Vector3*)&dest)   = self->project(*point);
    return dest;
}

rebel_bool GDAPI rebel_plane_intersect_3(
    const rebel_plane* p_self,
    rebel_vector3* r_dest,
    const rebel_plane* p_b,
    const rebel_plane* p_c
) {
    const Plane* self = (const Plane*)p_self;
    const Plane* b    = (const Plane*)p_b;
    const Plane* c    = (const Plane*)p_c;
    Vector3* dest     = (Vector3*)r_dest;
    return self->intersect_3(*b, *c, dest);
}

rebel_bool GDAPI rebel_plane_intersects_ray(
    const rebel_plane* p_self,
    rebel_vector3* r_dest,
    const rebel_vector3* p_from,
    const rebel_vector3* p_dir
) {
    const Plane* self   = (const Plane*)p_self;
    const Vector3* from = (const Vector3*)p_from;
    const Vector3* dir  = (const Vector3*)p_dir;
    Vector3* dest       = (Vector3*)r_dest;
    return self->intersects_ray(*from, *dir, dest);
}

rebel_bool GDAPI rebel_plane_intersects_segment(
    const rebel_plane* p_self,
    rebel_vector3* r_dest,
    const rebel_vector3* p_begin,
    const rebel_vector3* p_end
) {
    const Plane* self    = (const Plane*)p_self;
    const Vector3* begin = (const Vector3*)p_begin;
    const Vector3* end   = (const Vector3*)p_end;
    Vector3* dest        = (Vector3*)r_dest;
    return self->intersects_segment(*begin, *end, dest);
}

rebel_plane GDAPI rebel_plane_operator_neg(const rebel_plane* p_self) {
    rebel_plane raw_dest;
    Plane* dest       = (Plane*)&raw_dest;
    const Plane* self = (const Plane*)p_self;
    *dest             = -(*self);
    return raw_dest;
}

rebel_bool GDAPI
rebel_plane_operator_equal(const rebel_plane* p_self, const rebel_plane* p_b) {
    const Plane* self = (const Plane*)p_self;
    const Plane* b    = (const Plane*)p_b;
    return *self == *b;
}

void GDAPI
rebel_plane_set_normal(rebel_plane* p_self, const rebel_vector3* p_normal) {
    Plane* self           = (Plane*)p_self;
    const Vector3* normal = (const Vector3*)p_normal;
    self->set_normal(*normal);
}

rebel_vector3 GDAPI rebel_plane_get_normal(const rebel_plane* p_self) {
    const Plane* self    = (const Plane*)p_self;
    const Vector3 normal = self->get_normal();
    rebel_vector3* v3    = (rebel_vector3*)&normal;
    return *v3;
}

rebel_real GDAPI rebel_plane_get_d(const rebel_plane* p_self) {
    const Plane* self = (const Plane*)p_self;
    return self->d;
}

void GDAPI rebel_plane_set_d(rebel_plane* p_self, const rebel_real p_d) {
    Plane* self = (Plane*)p_self;
    self->d     = p_d;
}

#ifdef __cplusplus
}
#endif
