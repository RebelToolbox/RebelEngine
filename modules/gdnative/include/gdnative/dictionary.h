// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_DICTIONARY_H
#define GDNATIVE_DICTIONARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_DICTIONARY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_DICTIONARY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_DICTIONARY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_DICTIONARY_SIZE];
} rebel_dictionary;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/array.h>
#include <gdnative/gdnative.h>
#include <gdnative/variant.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_dictionary_new(rebel_dictionary* r_dest);
void GDAPI rebel_dictionary_new_copy(
    rebel_dictionary* r_dest,
    const rebel_dictionary* p_src
);
void GDAPI rebel_dictionary_destroy(rebel_dictionary* p_self);

rebel_dictionary GDAPI rebel_dictionary_duplicate(
    const rebel_dictionary* p_self,
    const rebel_bool p_deep
);

rebel_int GDAPI rebel_dictionary_size(const rebel_dictionary* p_self);

rebel_bool GDAPI rebel_dictionary_empty(const rebel_dictionary* p_self);

void GDAPI rebel_dictionary_clear(rebel_dictionary* p_self);

rebel_bool GDAPI rebel_dictionary_has(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
);

rebel_bool GDAPI rebel_dictionary_has_all(
    const rebel_dictionary* p_self,
    const rebel_array* p_keys
);

void GDAPI
rebel_dictionary_erase(rebel_dictionary* p_self, const rebel_variant* p_key);

rebel_int GDAPI rebel_dictionary_hash(const rebel_dictionary* p_self);

rebel_array GDAPI rebel_dictionary_keys(const rebel_dictionary* p_self);

rebel_array GDAPI rebel_dictionary_values(const rebel_dictionary* p_self);

rebel_variant GDAPI rebel_dictionary_get(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
);
void GDAPI rebel_dictionary_set(
    rebel_dictionary* p_self,
    const rebel_variant* p_key,
    const rebel_variant* p_value
);

rebel_variant GDAPI* rebel_dictionary_operator_index(
    rebel_dictionary* p_self,
    const rebel_variant* p_key
);

const rebel_variant GDAPI* rebel_dictionary_operator_index_const(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
);

rebel_variant GDAPI* rebel_dictionary_next(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
);

rebel_bool GDAPI rebel_dictionary_operator_equal(
    const rebel_dictionary* p_self,
    const rebel_dictionary* p_b
);

rebel_string GDAPI rebel_dictionary_to_json(const rebel_dictionary* p_self);

// GDNative core 1.1

rebel_bool GDAPI rebel_dictionary_erase_with_return(
    rebel_dictionary* p_self,
    const rebel_variant* p_key
);

rebel_variant GDAPI rebel_dictionary_get_with_default(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key,
    const rebel_variant* p_default
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_DICTIONARY_H
