// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_RECT2_H
#define GDNATIVE_RECT2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef REBEL_CORE_API_REBEL_RECT2_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_RECT2_TYPE_DEFINED

typedef struct rebel_rect2 {
    uint8_t _dont_touch_that[16];
} rebel_rect2;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/vector2.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_rect2_new_with_position_and_size(
    rebel_rect2* r_dest,
    const rebel_vector2* p_pos,
    const rebel_vector2* p_size
);
void GDAPI rebel_rect2_new(
    rebel_rect2* r_dest,
    const rebel_real p_x,
    const rebel_real p_y,
    const rebel_real p_width,
    const rebel_real p_height
);

rebel_string GDAPI rebel_rect2_as_string(const rebel_rect2* p_self);

rebel_real GDAPI rebel_rect2_get_area(const rebel_rect2* p_self);

rebel_bool GDAPI
rebel_rect2_intersects(const rebel_rect2* p_self, const rebel_rect2* p_b);

rebel_bool GDAPI
rebel_rect2_encloses(const rebel_rect2* p_self, const rebel_rect2* p_b);

rebel_bool GDAPI rebel_rect2_has_no_area(const rebel_rect2* p_self);

rebel_rect2 GDAPI
rebel_rect2_clip(const rebel_rect2* p_self, const rebel_rect2* p_b);

rebel_rect2 GDAPI
rebel_rect2_merge(const rebel_rect2* p_self, const rebel_rect2* p_b);

rebel_bool GDAPI
rebel_rect2_has_point(const rebel_rect2* p_self, const rebel_vector2* p_point);

rebel_rect2 GDAPI
rebel_rect2_grow(const rebel_rect2* p_self, const rebel_real p_by);

rebel_rect2 GDAPI rebel_rect2_grow_individual(
    const rebel_rect2* p_self,
    const rebel_real p_left,
    const rebel_real p_top,
    const rebel_real p_right,
    const rebel_real p_bottom
);

rebel_rect2 GDAPI rebel_rect2_grow_margin(
    const rebel_rect2* p_self,
    const rebel_int p_margin,
    const rebel_real p_by
);

rebel_rect2 GDAPI rebel_rect2_abs(const rebel_rect2* p_self);

rebel_rect2 GDAPI
rebel_rect2_expand(const rebel_rect2* p_self, const rebel_vector2* p_to);

rebel_bool GDAPI
rebel_rect2_operator_equal(const rebel_rect2* p_self, const rebel_rect2* p_b);

rebel_vector2 GDAPI rebel_rect2_get_position(const rebel_rect2* p_self);

rebel_vector2 GDAPI rebel_rect2_get_size(const rebel_rect2* p_self);

void GDAPI
rebel_rect2_set_position(rebel_rect2* p_self, const rebel_vector2* p_pos);

void GDAPI
rebel_rect2_set_size(rebel_rect2* p_self, const rebel_vector2* p_size);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_RECT2_H
