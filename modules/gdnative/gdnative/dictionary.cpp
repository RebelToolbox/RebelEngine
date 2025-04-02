// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/dictionary.h"

#include "core/variant.h"
// core/variant.h before to avoid compile errors with MSVC
#include "core/dictionary.h"
#include "core/io/json.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_dictionary) == sizeof(Dictionary),
    "Dictionary size mismatch"
);

void GDAPI rebel_dictionary_new(rebel_dictionary* r_dest) {
    Dictionary* dest = (Dictionary*)r_dest;
    memnew_placement(dest, Dictionary);
}

void GDAPI rebel_dictionary_new_copy(
    rebel_dictionary* r_dest,
    const rebel_dictionary* p_src
) {
    Dictionary* dest      = (Dictionary*)r_dest;
    const Dictionary* src = (const Dictionary*)p_src;
    memnew_placement(dest, Dictionary(*src));
}

void GDAPI rebel_dictionary_destroy(rebel_dictionary* p_self) {
    Dictionary* self = (Dictionary*)p_self;
    self->~Dictionary();
}

rebel_dictionary GDAPI rebel_dictionary_duplicate(
    const rebel_dictionary* p_self,
    const rebel_bool p_deep
) {
    const Dictionary* self = (const Dictionary*)p_self;
    rebel_dictionary res;
    Dictionary* val = (Dictionary*)&res;
    memnew_placement(val, Dictionary);
    *val = self->duplicate(p_deep);
    return res;
}

rebel_int GDAPI rebel_dictionary_size(const rebel_dictionary* p_self) {
    const Dictionary* self = (const Dictionary*)p_self;
    return self->size();
}

rebel_bool GDAPI rebel_dictionary_empty(const rebel_dictionary* p_self) {
    const Dictionary* self = (const Dictionary*)p_self;
    return self->empty();
}

void GDAPI rebel_dictionary_clear(rebel_dictionary* p_self) {
    Dictionary* self = (Dictionary*)p_self;
    self->clear();
}

rebel_bool GDAPI rebel_dictionary_has(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
) {
    const Dictionary* self = (const Dictionary*)p_self;
    const Variant* key     = (const Variant*)p_key;
    return self->has(*key);
}

rebel_bool GDAPI rebel_dictionary_has_all(
    const rebel_dictionary* p_self,
    const rebel_array* p_keys
) {
    const Dictionary* self = (const Dictionary*)p_self;
    const Array* keys      = (const Array*)p_keys;
    return self->has_all(*keys);
}

void GDAPI
rebel_dictionary_erase(rebel_dictionary* p_self, const rebel_variant* p_key) {
    Dictionary* self   = (Dictionary*)p_self;
    const Variant* key = (const Variant*)p_key;
    self->erase(*key);
}

rebel_int GDAPI rebel_dictionary_hash(const rebel_dictionary* p_self) {
    const Dictionary* self = (const Dictionary*)p_self;
    return self->hash();
}

rebel_array GDAPI rebel_dictionary_keys(const rebel_dictionary* p_self) {
    rebel_array dest;
    const Dictionary* self = (const Dictionary*)p_self;
    memnew_placement(&dest, Array(self->keys()));
    return dest;
}

rebel_array GDAPI rebel_dictionary_values(const rebel_dictionary* p_self) {
    rebel_array dest;
    const Dictionary* self = (const Dictionary*)p_self;
    memnew_placement(&dest, Array(self->values()));
    return dest;
}

rebel_variant GDAPI rebel_dictionary_get(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
) {
    rebel_variant raw_dest;
    Variant* dest          = (Variant*)&raw_dest;
    const Dictionary* self = (const Dictionary*)p_self;
    const Variant* key     = (const Variant*)p_key;
    memnew_placement(dest, Variant(self->operator[](*key)));
    return raw_dest;
}

void GDAPI rebel_dictionary_set(
    rebel_dictionary* p_self,
    const rebel_variant* p_key,
    const rebel_variant* p_value
) {
    Dictionary* self       = (Dictionary*)p_self;
    const Variant* key     = (const Variant*)p_key;
    const Variant* value   = (const Variant*)p_value;
    self->operator[](*key) = *value;
}

rebel_variant GDAPI* rebel_dictionary_operator_index(
    rebel_dictionary* p_self,
    const rebel_variant* p_key
) {
    Dictionary* self   = (Dictionary*)p_self;
    const Variant* key = (const Variant*)p_key;
    return (rebel_variant*)&self->operator[](*key);
}

const rebel_variant GDAPI* rebel_dictionary_operator_index_const(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
) {
    const Dictionary* self = (const Dictionary*)p_self;
    const Variant* key     = (const Variant*)p_key;
    return (const rebel_variant*)&self->operator[](*key);
}

rebel_variant GDAPI* rebel_dictionary_next(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key
) {
    Dictionary* self   = (Dictionary*)p_self;
    const Variant* key = (const Variant*)p_key;
    return (rebel_variant*)self->next(key);
}

rebel_bool GDAPI rebel_dictionary_operator_equal(
    const rebel_dictionary* p_self,
    const rebel_dictionary* p_b
) {
    const Dictionary* self = (const Dictionary*)p_self;
    const Dictionary* b    = (const Dictionary*)p_b;
    return *self == *b;
}

rebel_string GDAPI rebel_dictionary_to_json(const rebel_dictionary* p_self) {
    rebel_string raw_dest;
    String* dest           = (String*)&raw_dest;
    const Dictionary* self = (const Dictionary*)p_self;
    memnew_placement(dest, String(JSON::print(Variant(*self))));
    return raw_dest;
}

// GDNative core 1.1

rebel_bool GDAPI rebel_dictionary_erase_with_return(
    rebel_dictionary* p_self,
    const rebel_variant* p_key
) {
    Dictionary* self   = (Dictionary*)p_self;
    const Variant* key = (const Variant*)p_key;
    return self->erase(*key);
}

rebel_variant GDAPI rebel_dictionary_get_with_default(
    const rebel_dictionary* p_self,
    const rebel_variant* p_key,
    const rebel_variant* p_default
) {
    const Dictionary* self = (const Dictionary*)p_self;
    const Variant* key     = (const Variant*)p_key;
    const Variant* def     = (const Variant*)p_default;

    rebel_variant raw_dest;
    Variant* dest = (Variant*)&raw_dest;
    memnew_placement(dest, Variant(self->get(*key, *def)));

    return raw_dest;
}

#ifdef __cplusplus
}
#endif
