// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/color.h"

#include "core/color.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(rebel_color) == sizeof(Color), "Color size mismatch");

void GDAPI rebel_color_new_rgba(
    rebel_color* r_dest,
    const rebel_real p_r,
    const rebel_real p_g,
    const rebel_real p_b,
    const rebel_real p_a
) {
    Color* dest = (Color*)r_dest;
    *dest       = Color(p_r, p_g, p_b, p_a);
}

void GDAPI rebel_color_new_rgb(
    rebel_color* r_dest,
    const rebel_real p_r,
    const rebel_real p_g,
    const rebel_real p_b
) {
    Color* dest = (Color*)r_dest;
    *dest       = Color(p_r, p_g, p_b);
}

rebel_real rebel_color_get_r(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->r;
}

void rebel_color_set_r(rebel_color* p_self, const rebel_real val) {
    Color* self = (Color*)p_self;
    self->r     = val;
}

rebel_real rebel_color_get_g(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->g;
}

void rebel_color_set_g(rebel_color* p_self, const rebel_real val) {
    Color* self = (Color*)p_self;
    self->g     = val;
}

rebel_real rebel_color_get_b(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->b;
}

void rebel_color_set_b(rebel_color* p_self, const rebel_real val) {
    Color* self = (Color*)p_self;
    self->b     = val;
}

rebel_real rebel_color_get_a(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->a;
}

void rebel_color_set_a(rebel_color* p_self, const rebel_real val) {
    Color* self = (Color*)p_self;
    self->a     = val;
}

rebel_real rebel_color_get_h(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->get_h();
}

rebel_real rebel_color_get_s(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->get_s();
}

rebel_real rebel_color_get_v(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->get_v();
}

rebel_string GDAPI rebel_color_as_string(const rebel_color* p_self) {
    rebel_string ret;
    const Color* self = (const Color*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_int GDAPI rebel_color_to_rgba32(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->to_rgba32();
}

rebel_int GDAPI rebel_color_to_abgr32(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->to_abgr32();
}

rebel_int GDAPI rebel_color_to_abgr64(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->to_abgr64();
}

rebel_int GDAPI rebel_color_to_argb64(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->to_argb64();
}

rebel_int GDAPI rebel_color_to_rgba64(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->to_rgba64();
}

rebel_int GDAPI rebel_color_to_argb32(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->to_argb32();
}

rebel_real GDAPI rebel_color_gray(const rebel_color* p_self) {
    const Color* self = (const Color*)p_self;
    return self->gray();
}

rebel_color GDAPI rebel_color_inverted(const rebel_color* p_self) {
    rebel_color dest;
    const Color* self = (const Color*)p_self;
    *((Color*)&dest)  = self->inverted();
    return dest;
}

rebel_color GDAPI rebel_color_contrasted(const rebel_color* p_self) {
    rebel_color dest;
    const Color* self = (const Color*)p_self;
    *((Color*)&dest)  = self->contrasted();
    return dest;
}

rebel_color GDAPI rebel_color_linear_interpolate(
    const rebel_color* p_self,
    const rebel_color* p_b,
    const rebel_real p_t
) {
    rebel_color dest;
    const Color* self = (const Color*)p_self;
    const Color* b    = (const Color*)p_b;
    *((Color*)&dest)  = self->linear_interpolate(*b, p_t);
    return dest;
}

rebel_color GDAPI
rebel_color_blend(const rebel_color* p_self, const rebel_color* p_over) {
    rebel_color dest;
    const Color* self = (const Color*)p_self;
    const Color* over = (const Color*)p_over;
    *((Color*)&dest)  = self->blend(*over);
    return dest;
}

rebel_color GDAPI
rebel_color_darkened(const rebel_color* p_self, const rebel_real p_amount) {
    rebel_color dest;
    const Color* self = (const Color*)p_self;
    *((Color*)&dest)  = self->darkened(p_amount);
    return dest;
}

rebel_color GDAPI rebel_color_from_hsv(
    const rebel_color* p_self,
    const rebel_real p_h,
    const rebel_real p_s,
    const rebel_real p_v,
    const rebel_real p_a
) {
    rebel_color dest;
    const Color* self = (const Color*)p_self;
    *((Color*)&dest)  = self->from_hsv(p_h, p_s, p_v, p_a);
    return dest;
}

rebel_color GDAPI
rebel_color_lightened(const rebel_color* p_self, const rebel_real p_amount) {
    rebel_color dest;
    const Color* self = (const Color*)p_self;
    *((Color*)&dest)  = self->lightened(p_amount);
    return dest;
}

rebel_string GDAPI
rebel_color_to_html(const rebel_color* p_self, const rebel_bool p_with_alpha) {
    rebel_string dest;
    const Color* self = (const Color*)p_self;

    memnew_placement(&dest, String(self->to_html(p_with_alpha)));
    return dest;
}

rebel_bool GDAPI
rebel_color_operator_equal(const rebel_color* p_self, const rebel_color* p_b) {
    const Color* self = (const Color*)p_self;
    const Color* b    = (const Color*)p_b;
    return *self == *b;
}

rebel_bool GDAPI
rebel_color_operator_less(const rebel_color* p_self, const rebel_color* p_b) {
    const Color* self = (const Color*)p_self;
    const Color* b    = (const Color*)p_b;
    return *self < *b;
}

#ifdef __cplusplus
}
#endif
