// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/rect2.h"

#include "core/math/transform_2d.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(rebel_rect2) == sizeof(Rect2), "Rect2 size mismatch");

void GDAPI rebel_rect2_new_with_position_and_size(
    rebel_rect2* r_dest,
    const rebel_vector2* p_pos,
    const rebel_vector2* p_size
) {
    const Vector2* position = (const Vector2*)p_pos;
    const Vector2* size     = (const Vector2*)p_size;
    Rect2* dest             = (Rect2*)r_dest;
    *dest                   = Rect2(*position, *size);
}

void GDAPI rebel_rect2_new(
    rebel_rect2* r_dest,
    const rebel_real p_x,
    const rebel_real p_y,
    const rebel_real p_width,
    const rebel_real p_height
) {
    Rect2* dest = (Rect2*)r_dest;
    *dest       = Rect2(p_x, p_y, p_width, p_height);
}

rebel_string GDAPI rebel_rect2_as_string(const rebel_rect2* p_self) {
    rebel_string ret;
    const Rect2* self = (const Rect2*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_real GDAPI rebel_rect2_get_area(const rebel_rect2* p_self) {
    const Rect2* self = (const Rect2*)p_self;
    return self->get_area();
}

rebel_bool GDAPI
rebel_rect2_intersects(const rebel_rect2* p_self, const rebel_rect2* p_b) {
    const Rect2* self = (const Rect2*)p_self;
    const Rect2* b    = (const Rect2*)p_b;
    return self->intersects(*b);
}

rebel_bool GDAPI
rebel_rect2_encloses(const rebel_rect2* p_self, const rebel_rect2* p_b) {
    const Rect2* self = (const Rect2*)p_self;
    const Rect2* b    = (const Rect2*)p_b;
    return self->encloses(*b);
}

rebel_bool GDAPI rebel_rect2_has_no_area(const rebel_rect2* p_self) {
    const Rect2* self = (const Rect2*)p_self;
    return self->has_no_area();
}

rebel_rect2 GDAPI
rebel_rect2_clip(const rebel_rect2* p_self, const rebel_rect2* p_b) {
    rebel_rect2 dest;
    const Rect2* self = (const Rect2*)p_self;
    const Rect2* b    = (const Rect2*)p_b;
    *((Rect2*)&dest)  = self->clip(*b);
    return dest;
}

rebel_rect2 GDAPI
rebel_rect2_merge(const rebel_rect2* p_self, const rebel_rect2* p_b) {
    rebel_rect2 dest;
    const Rect2* self = (const Rect2*)p_self;
    const Rect2* b    = (const Rect2*)p_b;
    *((Rect2*)&dest)  = self->merge(*b);
    return dest;
}

rebel_bool GDAPI
rebel_rect2_has_point(const rebel_rect2* p_self, const rebel_vector2* p_point) {
    const Rect2* self    = (const Rect2*)p_self;
    const Vector2* point = (const Vector2*)p_point;
    return self->has_point(*point);
}

rebel_rect2 GDAPI
rebel_rect2_grow(const rebel_rect2* p_self, const rebel_real p_by) {
    rebel_rect2 dest;
    const Rect2* self = (const Rect2*)p_self;

    *((Rect2*)&dest) = self->grow(p_by);
    return dest;
}

rebel_rect2 GDAPI rebel_rect2_grow_individual(
    const rebel_rect2* p_self,
    const rebel_real p_left,
    const rebel_real p_top,
    const rebel_real p_right,
    const rebel_real p_bottom
) {
    rebel_rect2 dest;
    const Rect2* self = (const Rect2*)p_self;
    *((Rect2*)&dest)  = self->grow_individual(p_left, p_top, p_right, p_bottom);
    return dest;
}

rebel_rect2 GDAPI rebel_rect2_grow_margin(
    const rebel_rect2* p_self,
    const rebel_int p_margin,
    const rebel_real p_by
) {
    rebel_rect2 dest;
    const Rect2* self = (const Rect2*)p_self;
    *((Rect2*)&dest)  = self->grow_margin((Margin)p_margin, p_by);
    return dest;
}

rebel_rect2 GDAPI rebel_rect2_abs(const rebel_rect2* p_self) {
    rebel_rect2 dest;
    const Rect2* self = (const Rect2*)p_self;
    *((Rect2*)&dest)  = self->abs();
    return dest;
}

rebel_rect2 GDAPI
rebel_rect2_expand(const rebel_rect2* p_self, const rebel_vector2* p_to) {
    rebel_rect2 dest;
    const Rect2* self = (const Rect2*)p_self;
    const Vector2* to = (const Vector2*)p_to;
    *((Rect2*)&dest)  = self->expand(*to);
    return dest;
}

rebel_bool GDAPI
rebel_rect2_operator_equal(const rebel_rect2* p_self, const rebel_rect2* p_b) {
    const Rect2* self = (const Rect2*)p_self;
    const Rect2* b    = (const Rect2*)p_b;
    return *self == *b;
}

rebel_vector2 GDAPI rebel_rect2_get_position(const rebel_rect2* p_self) {
    rebel_vector2 dest;
    Vector2* d        = (Vector2*)&dest;
    const Rect2* self = (const Rect2*)p_self;
    *d                = self->get_position();
    return dest;
}

rebel_vector2 GDAPI rebel_rect2_get_size(const rebel_rect2* p_self) {
    rebel_vector2 dest;
    Vector2* d        = (Vector2*)&dest;
    const Rect2* self = (const Rect2*)p_self;
    *d                = self->get_size();
    return dest;
}

void GDAPI
rebel_rect2_set_position(rebel_rect2* p_self, const rebel_vector2* p_pos) {
    Rect2* self             = (Rect2*)p_self;
    const Vector2* position = (const Vector2*)p_pos;
    self->set_position(*position);
}

void GDAPI
rebel_rect2_set_size(rebel_rect2* p_self, const rebel_vector2* p_size) {
    Rect2* self         = (Rect2*)p_self;
    const Vector2* size = (const Vector2*)p_size;
    self->set_size(*size);
}

#ifdef __cplusplus
}
#endif
