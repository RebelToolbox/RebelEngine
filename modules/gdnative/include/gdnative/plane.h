// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_PLANE_H
#define GDNATIVE_PLANE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_PLANE_SIZE 16

#ifndef REBEL_CORE_API_REBEL_PLANE_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_PLANE_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_PLANE_SIZE];
} rebel_plane;
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

void GDAPI rebel_plane_new_with_reals(
    rebel_plane* r_dest,
    const rebel_real p_a,
    const rebel_real p_b,
    const rebel_real p_c,
    const rebel_real p_d
);
void GDAPI rebel_plane_new_with_vectors(
    rebel_plane* r_dest,
    const rebel_vector3* p_v1,
    const rebel_vector3* p_v2,
    const rebel_vector3* p_v3
);
void GDAPI rebel_plane_new_with_normal(
    rebel_plane* r_dest,
    const rebel_vector3* p_normal,
    const rebel_real p_d
);

rebel_string GDAPI rebel_plane_as_string(const rebel_plane* p_self);

rebel_plane GDAPI rebel_plane_normalized(const rebel_plane* p_self);

rebel_vector3 GDAPI rebel_plane_center(const rebel_plane* p_self);

rebel_vector3 GDAPI rebel_plane_get_any_point(const rebel_plane* p_self);

rebel_bool GDAPI rebel_plane_is_point_over(
    const rebel_plane* p_self,
    const rebel_vector3* p_point
);

rebel_real GDAPI rebel_plane_distance_to(
    const rebel_plane* p_self,
    const rebel_vector3* p_point
);

rebel_bool GDAPI rebel_plane_has_point(
    const rebel_plane* p_self,
    const rebel_vector3* p_point,
    const rebel_real p_epsilon
);

rebel_vector3 GDAPI
rebel_plane_project(const rebel_plane* p_self, const rebel_vector3* p_point);

rebel_bool GDAPI rebel_plane_intersect_3(
    const rebel_plane* p_self,
    rebel_vector3* r_dest,
    const rebel_plane* p_b,
    const rebel_plane* p_c
);

rebel_bool GDAPI rebel_plane_intersects_ray(
    const rebel_plane* p_self,
    rebel_vector3* r_dest,
    const rebel_vector3* p_from,
    const rebel_vector3* p_dir
);

rebel_bool GDAPI rebel_plane_intersects_segment(
    const rebel_plane* p_self,
    rebel_vector3* r_dest,
    const rebel_vector3* p_begin,
    const rebel_vector3* p_end
);

rebel_plane GDAPI rebel_plane_operator_neg(const rebel_plane* p_self);

rebel_bool GDAPI
rebel_plane_operator_equal(const rebel_plane* p_self, const rebel_plane* p_b);

void GDAPI
rebel_plane_set_normal(rebel_plane* p_self, const rebel_vector3* p_normal);

rebel_vector3 GDAPI rebel_plane_get_normal(const rebel_plane* p_self);

rebel_real GDAPI rebel_plane_get_d(const rebel_plane* p_self);

void GDAPI rebel_plane_set_d(rebel_plane* p_self, const rebel_real p_d);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_PLANE_H
