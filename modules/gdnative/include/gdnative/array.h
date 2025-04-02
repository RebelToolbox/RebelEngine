// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_ARRAY_H
#define GDNATIVE_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_ARRAY_SIZE];
} rebel_array;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/pool_arrays.h>
#include <gdnative/variant.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_array_new(rebel_array* r_dest);
void GDAPI rebel_array_new_copy(rebel_array* r_dest, const rebel_array* p_src);
void GDAPI rebel_array_new_pool_color_array(
    rebel_array* r_dest,
    const rebel_pool_color_array* p_pca
);
void GDAPI rebel_array_new_pool_vector3_array(
    rebel_array* r_dest,
    const rebel_pool_vector3_array* p_pv3a
);
void GDAPI rebel_array_new_pool_vector2_array(
    rebel_array* r_dest,
    const rebel_pool_vector2_array* p_pv2a
);
void GDAPI rebel_array_new_pool_string_array(
    rebel_array* r_dest,
    const rebel_pool_string_array* p_psa
);
void GDAPI rebel_array_new_pool_real_array(
    rebel_array* r_dest,
    const rebel_pool_real_array* p_pra
);
void GDAPI rebel_array_new_pool_int_array(
    rebel_array* r_dest,
    const rebel_pool_int_array* p_pia
);
void GDAPI rebel_array_new_pool_byte_array(
    rebel_array* r_dest,
    const rebel_pool_byte_array* p_pba
);

void GDAPI rebel_array_set(
    rebel_array* p_self,
    const rebel_int p_idx,
    const rebel_variant* p_value
);

rebel_variant GDAPI
rebel_array_get(const rebel_array* p_self, const rebel_int p_idx);

rebel_variant GDAPI* rebel_array_operator_index(
    rebel_array* p_self,
    const rebel_int p_idx
);

const rebel_variant GDAPI* rebel_array_operator_index_const(
    const rebel_array* p_self,
    const rebel_int p_idx
);

void GDAPI
rebel_array_append(rebel_array* p_self, const rebel_variant* p_value);

void GDAPI rebel_array_clear(rebel_array* p_self);

rebel_int GDAPI
rebel_array_count(const rebel_array* p_self, const rebel_variant* p_value);

rebel_bool GDAPI rebel_array_empty(const rebel_array* p_self);

void GDAPI rebel_array_erase(rebel_array* p_self, const rebel_variant* p_value);

rebel_variant GDAPI rebel_array_front(const rebel_array* p_self);

rebel_variant GDAPI rebel_array_back(const rebel_array* p_self);

rebel_int GDAPI rebel_array_find(
    const rebel_array* p_self,
    const rebel_variant* p_what,
    const rebel_int p_from
);

rebel_int GDAPI
rebel_array_find_last(const rebel_array* p_self, const rebel_variant* p_what);

rebel_bool GDAPI
rebel_array_has(const rebel_array* p_self, const rebel_variant* p_value);

rebel_int GDAPI rebel_array_hash(const rebel_array* p_self);

void GDAPI rebel_array_insert(
    rebel_array* p_self,
    const rebel_int p_pos,
    const rebel_variant* p_value
);

void GDAPI rebel_array_invert(rebel_array* p_self);

rebel_variant GDAPI rebel_array_pop_back(rebel_array* p_self);

rebel_variant GDAPI rebel_array_pop_front(rebel_array* p_self);

void GDAPI
rebel_array_push_back(rebel_array* p_self, const rebel_variant* p_value);

void GDAPI
rebel_array_push_front(rebel_array* p_self, const rebel_variant* p_value);

void GDAPI rebel_array_remove(rebel_array* p_self, const rebel_int p_idx);

void GDAPI rebel_array_resize(rebel_array* p_self, const rebel_int p_size);

rebel_int GDAPI rebel_array_rfind(
    const rebel_array* p_self,
    const rebel_variant* p_what,
    const rebel_int p_from
);

rebel_int GDAPI rebel_array_size(const rebel_array* p_self);

void GDAPI rebel_array_sort(rebel_array* p_self);

void GDAPI rebel_array_sort_custom(
    rebel_array* p_self,
    rebel_object* p_obj,
    const rebel_string* p_func
);

rebel_int GDAPI rebel_array_bsearch(
    rebel_array* p_self,
    const rebel_variant* p_value,
    const rebel_bool p_before
);

rebel_int GDAPI rebel_array_bsearch_custom(
    rebel_array* p_self,
    const rebel_variant* p_value,
    rebel_object* p_obj,
    const rebel_string* p_func,
    const rebel_bool p_before
);

void GDAPI rebel_array_destroy(rebel_array* p_self);

rebel_array GDAPI
rebel_array_duplicate(const rebel_array* p_self, const rebel_bool p_deep);

rebel_array GDAPI rebel_array_slice(
    const rebel_array* p_self,
    const rebel_int p_begin,
    const rebel_int p_end,
    const rebel_int p_step,
    const rebel_bool p_deep
);

rebel_variant GDAPI rebel_array_max(const rebel_array* p_self);

rebel_variant GDAPI rebel_array_min(const rebel_array* p_self);

void GDAPI rebel_array_shuffle(rebel_array* p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_ARRAY_H
