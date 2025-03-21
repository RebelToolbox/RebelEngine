// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_STRING_NAME_H
#define GDNATIVE_STRING_NAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <wchar.h>

#define REBEL_STRING_NAME_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_STRING_NAME_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_STRING_NAME_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_STRING_NAME_SIZE];
} rebel_string_name;
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
rebel_string_name_new(rebel_string_name* r_dest, const rebel_string* p_name);
void GDAPI
rebel_string_name_new_data(rebel_string_name* r_dest, const char* p_name);

rebel_string GDAPI rebel_string_name_get_name(const rebel_string_name* p_self);

uint32_t GDAPI rebel_string_name_get_hash(const rebel_string_name* p_self);
const void GDAPI* rebel_string_name_get_data_unique_pointer(
    const rebel_string_name* p_self
);

rebel_bool GDAPI rebel_string_name_operator_equal(
    const rebel_string_name* p_self,
    const rebel_string_name* p_other
);
rebel_bool GDAPI rebel_string_name_operator_less(
    const rebel_string_name* p_self,
    const rebel_string_name* p_other
);

void GDAPI rebel_string_name_destroy(rebel_string_name* p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_STRING_NAME_H
