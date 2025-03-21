// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_COLOR_H
#define GDNATIVE_COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_COLOR_SIZE 16

#ifndef REBEL_CORE_API_REBEL_COLOR_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_COLOR_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_COLOR_SIZE];
} rebel_color;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/string.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_color_new_rgba(
    rebel_color* r_dest,
    const rebel_real p_r,
    const rebel_real p_g,
    const rebel_real p_b,
    const rebel_real p_a
);
void GDAPI rebel_color_new_rgb(
    rebel_color* r_dest,
    const rebel_real p_r,
    const rebel_real p_g,
    const rebel_real p_b
);

rebel_real rebel_color_get_r(const rebel_color* p_self);
void rebel_color_set_r(rebel_color* p_self, const rebel_real r);

rebel_real rebel_color_get_g(const rebel_color* p_self);
void rebel_color_set_g(rebel_color* p_self, const rebel_real g);

rebel_real rebel_color_get_b(const rebel_color* p_self);
void rebel_color_set_b(rebel_color* p_self, const rebel_real b);

rebel_real rebel_color_get_a(const rebel_color* p_self);
void rebel_color_set_a(rebel_color* p_self, const rebel_real a);

rebel_real rebel_color_get_h(const rebel_color* p_self);
rebel_real rebel_color_get_s(const rebel_color* p_self);
rebel_real rebel_color_get_v(const rebel_color* p_self);

rebel_string GDAPI rebel_color_as_string(const rebel_color* p_self);

rebel_int GDAPI rebel_color_to_rgba32(const rebel_color* p_self);

rebel_int GDAPI rebel_color_to_abgr32(const rebel_color* p_self);

rebel_int GDAPI rebel_color_to_abgr64(const rebel_color* p_self);

rebel_int GDAPI rebel_color_to_argb64(const rebel_color* p_self);

rebel_int GDAPI rebel_color_to_rgba64(const rebel_color* p_self);

rebel_int GDAPI rebel_color_to_argb32(const rebel_color* p_self);

rebel_real GDAPI rebel_color_gray(const rebel_color* p_self);

rebel_color GDAPI rebel_color_inverted(const rebel_color* p_self);

rebel_color GDAPI rebel_color_contrasted(const rebel_color* p_self);

rebel_color GDAPI rebel_color_linear_interpolate(
    const rebel_color* p_self,
    const rebel_color* p_b,
    const rebel_real p_t
);

rebel_color GDAPI
rebel_color_blend(const rebel_color* p_self, const rebel_color* p_over);

rebel_color GDAPI
rebel_color_darkened(const rebel_color* p_self, const rebel_real p_amount);

rebel_color GDAPI rebel_color_from_hsv(
    const rebel_color* p_self,
    const rebel_real p_h,
    const rebel_real p_s,
    const rebel_real p_v,
    const rebel_real p_a
);

rebel_color GDAPI
rebel_color_lightened(const rebel_color* p_self, const rebel_real p_amount);

rebel_string GDAPI
rebel_color_to_html(const rebel_color* p_self, const rebel_bool p_with_alpha);

rebel_bool GDAPI
rebel_color_operator_equal(const rebel_color* p_self, const rebel_color* p_b);

rebel_bool GDAPI
rebel_color_operator_less(const rebel_color* p_self, const rebel_color* p_b);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_COLOR_H
