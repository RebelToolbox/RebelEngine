// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOT_STRING_NAME_H
#define GODOT_STRING_NAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <wchar.h>

#define GODOT_STRING_NAME_SIZE sizeof(void*)

#ifndef GODOT_CORE_API_GODOT_STRING_NAME_TYPE_DEFINED
#define GODOT_CORE_API_GODOT_STRING_NAME_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[GODOT_STRING_NAME_SIZE];
} godot_string_name;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI
godot_string_name_new(godot_string_name* r_dest, const godot_string* p_name);
void GDAPI
godot_string_name_new_data(godot_string_name* r_dest, const char* p_name);

godot_string GDAPI godot_string_name_get_name(const godot_string_name* p_self);

uint32_t GDAPI godot_string_name_get_hash(const godot_string_name* p_self);
const void GDAPI* godot_string_name_get_data_unique_pointer(
    const godot_string_name* p_self
);

godot_bool GDAPI godot_string_name_operator_equal(
    const godot_string_name* p_self,
    const godot_string_name* p_other
);
godot_bool GDAPI godot_string_name_operator_less(
    const godot_string_name* p_self,
    const godot_string_name* p_other
);

void GDAPI godot_string_name_destroy(godot_string_name* p_self);

#ifdef __cplusplus
}
#endif

#endif // GODOT_STRING_NAME_H
