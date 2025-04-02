// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_AABB_H
#define GDNATIVE_AABB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_AABB_SIZE 24

#ifndef REBEL_CORE_API_REBEL_AABB_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_AABB_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_AABB_SIZE];
} rebel_aabb;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/plane.h>
#include <gdnative/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_aabb_new(
    rebel_aabb* r_dest,
    const rebel_vector3* p_pos,
    const rebel_vector3* p_size
);

rebel_vector3 GDAPI rebel_aabb_get_position(const rebel_aabb* p_self);
void GDAPI
rebel_aabb_set_position(const rebel_aabb* p_self, const rebel_vector3* p_v);

rebel_vector3 GDAPI rebel_aabb_get_size(const rebel_aabb* p_self);
void GDAPI
rebel_aabb_set_size(const rebel_aabb* p_self, const rebel_vector3* p_v);

rebel_string GDAPI rebel_aabb_as_string(const rebel_aabb* p_self);

rebel_real GDAPI rebel_aabb_get_area(const rebel_aabb* p_self);

rebel_bool GDAPI rebel_aabb_has_no_area(const rebel_aabb* p_self);

rebel_bool GDAPI rebel_aabb_has_no_surface(const rebel_aabb* p_self);

rebel_bool GDAPI
rebel_aabb_intersects(const rebel_aabb* p_self, const rebel_aabb* p_with);

rebel_bool GDAPI
rebel_aabb_encloses(const rebel_aabb* p_self, const rebel_aabb* p_with);

rebel_aabb GDAPI
rebel_aabb_merge(const rebel_aabb* p_self, const rebel_aabb* p_with);

rebel_aabb GDAPI
rebel_aabb_intersection(const rebel_aabb* p_self, const rebel_aabb* p_with);

rebel_bool GDAPI rebel_aabb_intersects_plane(
    const rebel_aabb* p_self,
    const rebel_plane* p_plane
);

rebel_bool GDAPI rebel_aabb_intersects_segment(
    const rebel_aabb* p_self,
    const rebel_vector3* p_from,
    const rebel_vector3* p_to
);

rebel_bool GDAPI
rebel_aabb_has_point(const rebel_aabb* p_self, const rebel_vector3* p_point);

rebel_vector3 GDAPI
rebel_aabb_get_support(const rebel_aabb* p_self, const rebel_vector3* p_dir);

rebel_vector3 GDAPI rebel_aabb_get_longest_axis(const rebel_aabb* p_self);

rebel_int GDAPI rebel_aabb_get_longest_axis_index(const rebel_aabb* p_self);

rebel_real GDAPI rebel_aabb_get_longest_axis_size(const rebel_aabb* p_self);

rebel_vector3 GDAPI rebel_aabb_get_shortest_axis(const rebel_aabb* p_self);

rebel_int GDAPI rebel_aabb_get_shortest_axis_index(const rebel_aabb* p_self);

rebel_real GDAPI rebel_aabb_get_shortest_axis_size(const rebel_aabb* p_self);

rebel_aabb GDAPI
rebel_aabb_expand(const rebel_aabb* p_self, const rebel_vector3* p_to_point);

rebel_aabb GDAPI
rebel_aabb_grow(const rebel_aabb* p_self, const rebel_real p_by);

rebel_vector3 GDAPI
rebel_aabb_get_endpoint(const rebel_aabb* p_self, const rebel_int p_idx);

rebel_bool GDAPI
rebel_aabb_operator_equal(const rebel_aabb* p_self, const rebel_aabb* p_b);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_AABB_H
