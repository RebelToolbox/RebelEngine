// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOT_RECT2_H
#define GODOT_RECT2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef GODOT_CORE_API_GODOT_RECT2_TYPE_DEFINED
#define GODOT_CORE_API_GODOT_RECT2_TYPE_DEFINED

typedef struct godot_rect2 {
    uint8_t _dont_touch_that[16];
} godot_rect2;
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

void GDAPI godot_rect2_new_with_position_and_size(
    godot_rect2* r_dest,
    const godot_vector2* p_pos,
    const godot_vector2* p_size
);
void GDAPI godot_rect2_new(
    godot_rect2* r_dest,
    const godot_real p_x,
    const godot_real p_y,
    const godot_real p_width,
    const godot_real p_height
);

godot_string GDAPI godot_rect2_as_string(const godot_rect2* p_self);

godot_real GDAPI godot_rect2_get_area(const godot_rect2* p_self);

godot_bool GDAPI
godot_rect2_intersects(const godot_rect2* p_self, const godot_rect2* p_b);

godot_bool GDAPI
godot_rect2_encloses(const godot_rect2* p_self, const godot_rect2* p_b);

godot_bool GDAPI godot_rect2_has_no_area(const godot_rect2* p_self);

godot_rect2 GDAPI
godot_rect2_clip(const godot_rect2* p_self, const godot_rect2* p_b);

godot_rect2 GDAPI
godot_rect2_merge(const godot_rect2* p_self, const godot_rect2* p_b);

godot_bool GDAPI
godot_rect2_has_point(const godot_rect2* p_self, const godot_vector2* p_point);

godot_rect2 GDAPI
godot_rect2_grow(const godot_rect2* p_self, const godot_real p_by);

godot_rect2 GDAPI godot_rect2_grow_individual(
    const godot_rect2* p_self,
    const godot_real p_left,
    const godot_real p_top,
    const godot_real p_right,
    const godot_real p_bottom
);

godot_rect2 GDAPI godot_rect2_grow_margin(
    const godot_rect2* p_self,
    const godot_int p_margin,
    const godot_real p_by
);

godot_rect2 GDAPI godot_rect2_abs(const godot_rect2* p_self);

godot_rect2 GDAPI
godot_rect2_expand(const godot_rect2* p_self, const godot_vector2* p_to);

godot_bool GDAPI
godot_rect2_operator_equal(const godot_rect2* p_self, const godot_rect2* p_b);

godot_vector2 GDAPI godot_rect2_get_position(const godot_rect2* p_self);

godot_vector2 GDAPI godot_rect2_get_size(const godot_rect2* p_self);

void GDAPI
godot_rect2_set_position(godot_rect2* p_self, const godot_vector2* p_pos);

void GDAPI
godot_rect2_set_size(godot_rect2* p_self, const godot_vector2* p_size);

#ifdef __cplusplus
}
#endif

#endif // GODOT_RECT2_H
