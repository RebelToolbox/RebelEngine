// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOT_RID_H
#define GODOT_RID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define GODOT_RID_SIZE sizeof(void*)

#ifndef GODOT_CORE_API_GODOT_RID_TYPE_DEFINED
#define GODOT_CORE_API_GODOT_RID_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[GODOT_RID_SIZE];
} godot_rid;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI godot_rid_new(godot_rid* r_dest);

godot_int GDAPI godot_rid_get_id(const godot_rid* p_self);

void GDAPI
godot_rid_new_with_resource(godot_rid* r_dest, const godot_object* p_from);

godot_bool GDAPI
godot_rid_operator_equal(const godot_rid* p_self, const godot_rid* p_b);

godot_bool GDAPI
godot_rid_operator_less(const godot_rid* p_self, const godot_rid* p_b);

#ifdef __cplusplus
}
#endif

#endif // GODOT_RID_H
