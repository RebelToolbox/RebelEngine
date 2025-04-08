// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_RID_H
#define GDNATIVE_RID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_RID_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_RID_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_RID_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_RID_SIZE];
} rebel_rid;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_rid_new(rebel_rid* r_dest);

rebel_int GDAPI rebel_rid_get_id(const rebel_rid* p_self);

void GDAPI
rebel_rid_new_with_resource(rebel_rid* r_dest, const rebel_object* p_from);

rebel_bool GDAPI
rebel_rid_operator_equal(const rebel_rid* p_self, const rebel_rid* p_b);

rebel_bool GDAPI
rebel_rid_operator_less(const rebel_rid* p_self, const rebel_rid* p_b);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_RID_H
