// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOT_ARRAY_H
#define GODOT_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define GODOT_ARRAY_SIZE sizeof(void*)

#ifndef GODOT_CORE_API_GODOT_ARRAY_TYPE_DEFINED
#define GODOT_CORE_API_GODOT_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[GODOT_ARRAY_SIZE];
} godot_array;
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

void GDAPI godot_array_new(godot_array* r_dest);
void GDAPI godot_array_new_copy(godot_array* r_dest, const godot_array* p_src);
void GDAPI godot_array_new_pool_color_array(
    godot_array* r_dest,
    const godot_pool_color_array* p_pca
);
void GDAPI godot_array_new_pool_vector3_array(
    godot_array* r_dest,
    const godot_pool_vector3_array* p_pv3a
);
void GDAPI godot_array_new_pool_vector2_array(
    godot_array* r_dest,
    const godot_pool_vector2_array* p_pv2a
);
void GDAPI godot_array_new_pool_string_array(
    godot_array* r_dest,
    const godot_pool_string_array* p_psa
);
void GDAPI godot_array_new_pool_real_array(
    godot_array* r_dest,
    const godot_pool_real_array* p_pra
);
void GDAPI godot_array_new_pool_int_array(
    godot_array* r_dest,
    const godot_pool_int_array* p_pia
);
void GDAPI godot_array_new_pool_byte_array(
    godot_array* r_dest,
    const godot_pool_byte_array* p_pba
);

void GDAPI godot_array_set(
    godot_array* p_self,
    const godot_int p_idx,
    const godot_variant* p_value
);

godot_variant GDAPI
godot_array_get(const godot_array* p_self, const godot_int p_idx);

godot_variant GDAPI* godot_array_operator_index(
    godot_array* p_self,
    const godot_int p_idx
);

const godot_variant GDAPI* godot_array_operator_index_const(
    const godot_array* p_self,
    const godot_int p_idx
);

void GDAPI
godot_array_append(godot_array* p_self, const godot_variant* p_value);

void GDAPI godot_array_clear(godot_array* p_self);

godot_int GDAPI
godot_array_count(const godot_array* p_self, const godot_variant* p_value);

godot_bool GDAPI godot_array_empty(const godot_array* p_self);

void GDAPI godot_array_erase(godot_array* p_self, const godot_variant* p_value);

godot_variant GDAPI godot_array_front(const godot_array* p_self);

godot_variant GDAPI godot_array_back(const godot_array* p_self);

godot_int GDAPI godot_array_find(
    const godot_array* p_self,
    const godot_variant* p_what,
    const godot_int p_from
);

godot_int GDAPI
godot_array_find_last(const godot_array* p_self, const godot_variant* p_what);

godot_bool GDAPI
godot_array_has(const godot_array* p_self, const godot_variant* p_value);

godot_int GDAPI godot_array_hash(const godot_array* p_self);

void GDAPI godot_array_insert(
    godot_array* p_self,
    const godot_int p_pos,
    const godot_variant* p_value
);

void GDAPI godot_array_invert(godot_array* p_self);

godot_variant GDAPI godot_array_pop_back(godot_array* p_self);

godot_variant GDAPI godot_array_pop_front(godot_array* p_self);

void GDAPI
godot_array_push_back(godot_array* p_self, const godot_variant* p_value);

void GDAPI
godot_array_push_front(godot_array* p_self, const godot_variant* p_value);

void GDAPI godot_array_remove(godot_array* p_self, const godot_int p_idx);

void GDAPI godot_array_resize(godot_array* p_self, const godot_int p_size);

godot_int GDAPI godot_array_rfind(
    const godot_array* p_self,
    const godot_variant* p_what,
    const godot_int p_from
);

godot_int GDAPI godot_array_size(const godot_array* p_self);

void GDAPI godot_array_sort(godot_array* p_self);

void GDAPI godot_array_sort_custom(
    godot_array* p_self,
    godot_object* p_obj,
    const godot_string* p_func
);

godot_int GDAPI godot_array_bsearch(
    godot_array* p_self,
    const godot_variant* p_value,
    const godot_bool p_before
);

godot_int GDAPI godot_array_bsearch_custom(
    godot_array* p_self,
    const godot_variant* p_value,
    godot_object* p_obj,
    const godot_string* p_func,
    const godot_bool p_before
);

void GDAPI godot_array_destroy(godot_array* p_self);

godot_array GDAPI
godot_array_duplicate(const godot_array* p_self, const godot_bool p_deep);

godot_array GDAPI godot_array_slice(
    const godot_array* p_self,
    const godot_int p_begin,
    const godot_int p_end,
    const godot_int p_step,
    const godot_bool p_deep
);

godot_variant GDAPI godot_array_max(const godot_array* p_self);

godot_variant GDAPI godot_array_min(const godot_array* p_self);

void GDAPI godot_array_shuffle(godot_array* p_self);

#ifdef __cplusplus
}
#endif

#endif // GODOT_ARRAY_H
