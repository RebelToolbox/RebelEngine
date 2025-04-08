// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/array.h"

#include "core/array.h"
#include "core/color.h"
#include "core/os/memory.h"
#include "core/pool_vector.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(rebel_array) == sizeof(Array), "Array size mismatch");

void GDAPI rebel_array_new(rebel_array* r_dest) {
    Array* dest = (Array*)r_dest;
    memnew_placement(dest, Array);
}

void GDAPI rebel_array_new_copy(rebel_array* r_dest, const rebel_array* p_src) {
    Array* dest      = (Array*)r_dest;
    const Array* src = (const Array*)p_src;
    memnew_placement(dest, Array(*src));
}

void GDAPI rebel_array_new_pool_color_array(
    rebel_array* r_dest,
    const rebel_pool_color_array* p_pca
) {
    Array* dest            = (Array*)r_dest;
    PoolVector<Color>* pca = (PoolVector<Color>*)p_pca;
    memnew_placement(dest, Array);
    dest->resize(pca->size());

    for (int i = 0; i < dest->size(); i++) {
        Variant v           = pca->operator[](i);
        dest->operator[](i) = v;
    }
}

void GDAPI rebel_array_new_pool_vector3_array(
    rebel_array* r_dest,
    const rebel_pool_vector3_array* p_pv3a
) {
    Array* dest              = (Array*)r_dest;
    PoolVector<Vector3>* pca = (PoolVector<Vector3>*)p_pv3a;
    memnew_placement(dest, Array);
    dest->resize(pca->size());

    for (int i = 0; i < dest->size(); i++) {
        Variant v           = pca->operator[](i);
        dest->operator[](i) = v;
    }
}

void GDAPI rebel_array_new_pool_vector2_array(
    rebel_array* r_dest,
    const rebel_pool_vector2_array* p_pv2a
) {
    Array* dest              = (Array*)r_dest;
    PoolVector<Vector2>* pca = (PoolVector<Vector2>*)p_pv2a;
    memnew_placement(dest, Array);
    dest->resize(pca->size());

    for (int i = 0; i < dest->size(); i++) {
        Variant v           = pca->operator[](i);
        dest->operator[](i) = v;
    }
}

void GDAPI rebel_array_new_pool_string_array(
    rebel_array* r_dest,
    const rebel_pool_string_array* p_psa
) {
    Array* dest             = (Array*)r_dest;
    PoolVector<String>* pca = (PoolVector<String>*)p_psa;
    memnew_placement(dest, Array);
    dest->resize(pca->size());

    for (int i = 0; i < dest->size(); i++) {
        Variant v           = pca->operator[](i);
        dest->operator[](i) = v;
    }
}

void GDAPI rebel_array_new_pool_real_array(
    rebel_array* r_dest,
    const rebel_pool_real_array* p_pra
) {
    Array* dest                 = (Array*)r_dest;
    PoolVector<rebel_real>* pca = (PoolVector<rebel_real>*)p_pra;
    memnew_placement(dest, Array);
    dest->resize(pca->size());

    for (int i = 0; i < dest->size(); i++) {
        Variant v           = pca->operator[](i);
        dest->operator[](i) = v;
    }
}

void GDAPI rebel_array_new_pool_int_array(
    rebel_array* r_dest,
    const rebel_pool_int_array* p_pia
) {
    Array* dest                = (Array*)r_dest;
    PoolVector<rebel_int>* pca = (PoolVector<rebel_int>*)p_pia;
    memnew_placement(dest, Array);
    dest->resize(pca->size());

    for (int i = 0; i < dest->size(); i++) {
        Variant v           = pca->operator[](i);
        dest->operator[](i) = v;
    }
}

void GDAPI rebel_array_new_pool_byte_array(
    rebel_array* r_dest,
    const rebel_pool_byte_array* p_pba
) {
    Array* dest              = (Array*)r_dest;
    PoolVector<uint8_t>* pca = (PoolVector<uint8_t>*)p_pba;
    memnew_placement(dest, Array);
    dest->resize(pca->size());

    for (int i = 0; i < dest->size(); i++) {
        Variant v           = pca->operator[](i);
        dest->operator[](i) = v;
    }
}

void GDAPI rebel_array_set(
    rebel_array* p_self,
    const rebel_int p_idx,
    const rebel_variant* p_value
) {
    Array* self             = (Array*)p_self;
    Variant* val            = (Variant*)p_value;
    self->operator[](p_idx) = *val;
}

rebel_variant GDAPI
rebel_array_get(const rebel_array* p_self, const rebel_int p_idx) {
    rebel_variant raw_dest;
    Variant* dest     = (Variant*)&raw_dest;
    const Array* self = (const Array*)p_self;
    memnew_placement(dest, Variant(self->operator[](p_idx)));
    return raw_dest;
}

rebel_variant GDAPI* rebel_array_operator_index(
    rebel_array* p_self,
    const rebel_int p_idx
) {
    Array* self = (Array*)p_self;
    return (rebel_variant*)&self->operator[](p_idx);
}

const rebel_variant GDAPI* rebel_array_operator_index_const(
    const rebel_array* p_self,
    const rebel_int p_idx
) {
    const Array* self = (const Array*)p_self;
    return (const rebel_variant*)&self->operator[](p_idx);
}

void GDAPI
rebel_array_append(rebel_array* p_self, const rebel_variant* p_value) {
    Array* self  = (Array*)p_self;
    Variant* val = (Variant*)p_value;
    self->append(*val);
}

void GDAPI rebel_array_clear(rebel_array* p_self) {
    Array* self = (Array*)p_self;
    self->clear();
}

rebel_int GDAPI
rebel_array_count(const rebel_array* p_self, const rebel_variant* p_value) {
    const Array* self  = (const Array*)p_self;
    const Variant* val = (const Variant*)p_value;
    return self->count(*val);
}

rebel_bool GDAPI rebel_array_empty(const rebel_array* p_self) {
    const Array* self = (const Array*)p_self;
    return self->empty();
}

void GDAPI
rebel_array_erase(rebel_array* p_self, const rebel_variant* p_value) {
    Array* self        = (Array*)p_self;
    const Variant* val = (const Variant*)p_value;
    self->erase(*val);
}

rebel_variant GDAPI rebel_array_front(const rebel_array* p_self) {
    const Array* self = (const Array*)p_self;
    rebel_variant v;
    Variant* val = (Variant*)&v;
    memnew_placement(val, Variant);
    *val = self->front();
    return v;
}

rebel_variant GDAPI rebel_array_back(const rebel_array* p_self) {
    const Array* self = (const Array*)p_self;
    rebel_variant v;
    Variant* val = (Variant*)&v;
    memnew_placement(val, Variant);
    *val = self->back();
    return v;
}

rebel_int GDAPI rebel_array_find(
    const rebel_array* p_self,
    const rebel_variant* p_what,
    const rebel_int p_from
) {
    const Array* self  = (const Array*)p_self;
    const Variant* val = (const Variant*)p_what;
    return self->find(*val, p_from);
}

rebel_int GDAPI
rebel_array_find_last(const rebel_array* p_self, const rebel_variant* p_what) {
    const Array* self  = (const Array*)p_self;
    const Variant* val = (const Variant*)p_what;
    return self->find_last(*val);
}

rebel_bool GDAPI
rebel_array_has(const rebel_array* p_self, const rebel_variant* p_value) {
    const Array* self  = (const Array*)p_self;
    const Variant* val = (const Variant*)p_value;
    return self->has(*val);
}

rebel_int GDAPI rebel_array_hash(const rebel_array* p_self) {
    const Array* self = (const Array*)p_self;
    return self->hash();
}

void GDAPI rebel_array_insert(
    rebel_array* p_self,
    const rebel_int p_pos,
    const rebel_variant* p_value
) {
    Array* self        = (Array*)p_self;
    const Variant* val = (const Variant*)p_value;
    self->insert(p_pos, *val);
}

void GDAPI rebel_array_invert(rebel_array* p_self) {
    Array* self = (Array*)p_self;
    self->invert();
}

rebel_variant GDAPI rebel_array_pop_back(rebel_array* p_self) {
    Array* self = (Array*)p_self;
    rebel_variant v;
    Variant* val = (Variant*)&v;
    memnew_placement(val, Variant);
    *val = self->pop_back();
    return v;
}

rebel_variant GDAPI rebel_array_pop_front(rebel_array* p_self) {
    Array* self = (Array*)p_self;
    rebel_variant v;
    Variant* val = (Variant*)&v;
    memnew_placement(val, Variant);
    *val = self->pop_front();
    return v;
}

void GDAPI
rebel_array_push_back(rebel_array* p_self, const rebel_variant* p_value) {
    Array* self        = (Array*)p_self;
    const Variant* val = (const Variant*)p_value;
    self->push_back(*val);
}

void GDAPI
rebel_array_push_front(rebel_array* p_self, const rebel_variant* p_value) {
    Array* self        = (Array*)p_self;
    const Variant* val = (const Variant*)p_value;
    self->push_front(*val);
}

void GDAPI rebel_array_remove(rebel_array* p_self, const rebel_int p_idx) {
    Array* self = (Array*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_array_resize(rebel_array* p_self, const rebel_int p_size) {
    Array* self = (Array*)p_self;
    self->resize(p_size);
}

rebel_int GDAPI rebel_array_rfind(
    const rebel_array* p_self,
    const rebel_variant* p_what,
    const rebel_int p_from
) {
    const Array* self  = (const Array*)p_self;
    const Variant* val = (const Variant*)p_what;
    return self->rfind(*val, p_from);
}

rebel_int GDAPI rebel_array_size(const rebel_array* p_self) {
    const Array* self = (const Array*)p_self;
    return self->size();
}

void GDAPI rebel_array_sort(rebel_array* p_self) {
    Array* self = (Array*)p_self;
    self->sort();
}

void GDAPI rebel_array_sort_custom(
    rebel_array* p_self,
    rebel_object* p_obj,
    const rebel_string* p_func
) {
    Array* self        = (Array*)p_self;
    const String* func = (const String*)p_func;
    self->sort_custom((Object*)p_obj, *func);
}

rebel_int GDAPI rebel_array_bsearch(
    rebel_array* p_self,
    const rebel_variant* p_value,
    const rebel_bool p_before
) {
    Array* self = (Array*)p_self;
    return self->bsearch(*(const Variant*)p_value, p_before);
}

rebel_int GDAPI rebel_array_bsearch_custom(
    rebel_array* p_self,
    const rebel_variant* p_value,
    rebel_object* p_obj,
    const rebel_string* p_func,
    const rebel_bool p_before
) {
    Array* self        = (Array*)p_self;
    const String* func = (const String*)p_func;
    return self->bsearch_custom(
        *(const Variant*)p_value,
        (Object*)p_obj,
        *func,
        p_before
    );
}

void GDAPI rebel_array_destroy(rebel_array* p_self) {
    ((Array*)p_self)->~Array();
}

rebel_array GDAPI
rebel_array_duplicate(const rebel_array* p_self, const rebel_bool p_deep) {
    const Array* self = (const Array*)p_self;
    rebel_array res;
    Array* val = (Array*)&res;
    memnew_placement(val, Array);
    *val = self->duplicate(p_deep);
    return res;
}

rebel_array GDAPI rebel_array_slice(
    const rebel_array* p_self,
    const rebel_int p_begin,
    const rebel_int p_end,
    const rebel_int p_step,
    const rebel_bool p_deep
) {
    const Array* self = (const Array*)p_self;
    rebel_array res;
    Array* val = (Array*)&res;
    memnew_placement(val, Array);
    *val = self->slice(p_begin, p_end, p_step, p_deep);
    return res;
}

rebel_variant GDAPI rebel_array_max(const rebel_array* p_self) {
    const Array* self = (const Array*)p_self;
    rebel_variant v;
    Variant* val = (Variant*)&v;
    memnew_placement(val, Variant);
    *val = self->max();
    return v;
}

rebel_variant GDAPI rebel_array_min(const rebel_array* p_self) {
    const Array* self = (const Array*)p_self;
    rebel_variant v;
    Variant* val = (Variant*)&v;
    memnew_placement(val, Variant);
    *val = self->min();
    return v;
}

void GDAPI rebel_array_shuffle(rebel_array* p_self) {
    Array* self = (Array*)p_self;
    self->shuffle();
}

#ifdef __cplusplus
}
#endif
