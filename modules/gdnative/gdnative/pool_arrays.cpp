// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/pool_arrays.h"

#include "core/array.h"
#include "core/color.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/pool_vector.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_pool_byte_array) == sizeof(PoolVector<uint8_t>),
    "PoolVector<uint8_t> size mismatch"
);
static_assert(
    sizeof(rebel_pool_int_array) == sizeof(PoolVector<rebel_int>),
    "PoolVector<rebel_int> size mismatch"
);
static_assert(
    sizeof(rebel_pool_real_array) == sizeof(PoolVector<rebel_real>),
    "PoolVector<rebel_real> size mismatch"
);
static_assert(
    sizeof(rebel_pool_string_array) == sizeof(PoolVector<String>),
    "PoolVector<String> size mismatch"
);
static_assert(
    sizeof(rebel_pool_vector2_array) == sizeof(PoolVector<Vector2>),
    "PoolVector<Vector2> size mismatch"
);
static_assert(
    sizeof(rebel_pool_vector3_array) == sizeof(PoolVector<Vector3>),
    "PoolVector<Vector3> size mismatch"
);
static_assert(
    sizeof(rebel_pool_color_array) == sizeof(PoolVector<Color>),
    "PoolVector<Color> size mismatch"
);

#define memnew_placement_custom(m_placement, m_class, m_constr)                \
    _post_initialize(new (m_placement, sizeof(m_class), "") m_constr)

// byte

void GDAPI rebel_pool_byte_array_new(rebel_pool_byte_array* r_dest) {
    PoolVector<uint8_t>* dest = (PoolVector<uint8_t>*)r_dest;
    memnew_placement(dest, PoolVector<uint8_t>);
}

void GDAPI rebel_pool_byte_array_new_copy(
    rebel_pool_byte_array* r_dest,
    const rebel_pool_byte_array* p_src
) {
    PoolVector<uint8_t>* dest      = (PoolVector<uint8_t>*)r_dest;
    const PoolVector<uint8_t>* src = (const PoolVector<uint8_t>*)p_src;
    memnew_placement(dest, PoolVector<uint8_t>(*src));
}

void GDAPI rebel_pool_byte_array_new_with_array(
    rebel_pool_byte_array* r_dest,
    const rebel_array* p_a
) {
    PoolVector<uint8_t>* dest = (PoolVector<uint8_t>*)r_dest;
    Array* a                  = (Array*)p_a;
    memnew_placement(dest, PoolVector<uint8_t>);

    dest->resize(a->size());
    for (int i = 0; i < a->size(); i++) {
        dest->set(i, (*a)[i]);
    }
}

void GDAPI rebel_pool_byte_array_append(
    rebel_pool_byte_array* p_self,
    const uint8_t p_data
) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    self->append(p_data);
}

void GDAPI rebel_pool_byte_array_append_array(
    rebel_pool_byte_array* p_self,
    const rebel_pool_byte_array* p_array
) {
    PoolVector<uint8_t>* self  = (PoolVector<uint8_t>*)p_self;
    PoolVector<uint8_t>* array = (PoolVector<uint8_t>*)p_array;
    self->append_array(*array);
}

rebel_error GDAPI rebel_pool_byte_array_insert(
    rebel_pool_byte_array* p_self,
    const rebel_int p_idx,
    const uint8_t p_data
) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    return (rebel_error)self->insert(p_idx, p_data);
}

void GDAPI rebel_pool_byte_array_invert(rebel_pool_byte_array* p_self) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    self->invert();
}

void GDAPI rebel_pool_byte_array_push_back(
    rebel_pool_byte_array* p_self,
    const uint8_t p_data
) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    self->push_back(p_data);
}

void GDAPI rebel_pool_byte_array_remove(
    rebel_pool_byte_array* p_self,
    const rebel_int p_idx
) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_pool_byte_array_resize(
    rebel_pool_byte_array* p_self,
    const rebel_int p_size
) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    self->resize(p_size);
}

rebel_pool_byte_array_read_access GDAPI* rebel_pool_byte_array_read(
    const rebel_pool_byte_array* p_self
) {
    const PoolVector<uint8_t>* self = (const PoolVector<uint8_t>*)p_self;
    return (rebel_pool_byte_array_read_access*)memnew(
        PoolVector<uint8_t>::Read(self->read())
    );
}

rebel_pool_byte_array_write_access GDAPI* rebel_pool_byte_array_write(
    rebel_pool_byte_array* p_self
) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    return (rebel_pool_byte_array_write_access*)memnew(
        PoolVector<uint8_t>::Write(self->write())
    );
}

void GDAPI rebel_pool_byte_array_set(
    rebel_pool_byte_array* p_self,
    const rebel_int p_idx,
    const uint8_t p_data
) {
    PoolVector<uint8_t>* self = (PoolVector<uint8_t>*)p_self;
    self->set(p_idx, p_data);
}

uint8_t GDAPI rebel_pool_byte_array_get(
    const rebel_pool_byte_array* p_self,
    const rebel_int p_idx
) {
    const PoolVector<uint8_t>* self = (const PoolVector<uint8_t>*)p_self;
    return self->get(p_idx);
}

rebel_int GDAPI rebel_pool_byte_array_size(const rebel_pool_byte_array* p_self
) {
    const PoolVector<uint8_t>* self = (const PoolVector<uint8_t>*)p_self;
    return self->size();
}

rebel_bool GDAPI rebel_pool_byte_array_empty(const rebel_pool_byte_array* p_self
) {
    const PoolVector<uint8_t>* self = (const PoolVector<uint8_t>*)p_self;
    return self->empty();
}

void GDAPI rebel_pool_byte_array_destroy(rebel_pool_byte_array* p_self) {
    ((PoolVector<uint8_t>*)p_self)->~PoolVector();
}

// int

void GDAPI rebel_pool_int_array_new(rebel_pool_int_array* r_dest) {
    PoolVector<rebel_int>* dest = (PoolVector<rebel_int>*)r_dest;
    memnew_placement(dest, PoolVector<rebel_int>);
}

void GDAPI rebel_pool_int_array_new_copy(
    rebel_pool_int_array* r_dest,
    const rebel_pool_int_array* p_src
) {
    PoolVector<rebel_int>* dest      = (PoolVector<rebel_int>*)r_dest;
    const PoolVector<rebel_int>* src = (const PoolVector<rebel_int>*)p_src;
    memnew_placement(dest, PoolVector<rebel_int>(*src));
}

void GDAPI rebel_pool_int_array_new_with_array(
    rebel_pool_int_array* r_dest,
    const rebel_array* p_a
) {
    PoolVector<rebel_int>* dest = (PoolVector<rebel_int>*)r_dest;
    Array* a                    = (Array*)p_a;
    memnew_placement(dest, PoolVector<rebel_int>);

    dest->resize(a->size());
    for (int i = 0; i < a->size(); i++) {
        dest->set(i, (*a)[i]);
    }
}

void GDAPI rebel_pool_int_array_append(
    rebel_pool_int_array* p_self,
    const rebel_int p_data
) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    self->append(p_data);
}

void GDAPI rebel_pool_int_array_append_array(
    rebel_pool_int_array* p_self,
    const rebel_pool_int_array* p_array
) {
    PoolVector<rebel_int>* self  = (PoolVector<rebel_int>*)p_self;
    PoolVector<rebel_int>* array = (PoolVector<rebel_int>*)p_array;
    self->append_array(*array);
}

rebel_error GDAPI rebel_pool_int_array_insert(
    rebel_pool_int_array* p_self,
    const rebel_int p_idx,
    const rebel_int p_data
) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    return (rebel_error)self->insert(p_idx, p_data);
}

void GDAPI rebel_pool_int_array_invert(rebel_pool_int_array* p_self) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    self->invert();
}

void GDAPI rebel_pool_int_array_push_back(
    rebel_pool_int_array* p_self,
    const rebel_int p_data
) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    self->push_back(p_data);
}

void GDAPI rebel_pool_int_array_remove(
    rebel_pool_int_array* p_self,
    const rebel_int p_idx
) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_pool_int_array_resize(
    rebel_pool_int_array* p_self,
    const rebel_int p_size
) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    self->resize(p_size);
}

rebel_pool_int_array_read_access GDAPI* rebel_pool_int_array_read(
    const rebel_pool_int_array* p_self
) {
    const PoolVector<rebel_int>* self = (const PoolVector<rebel_int>*)p_self;
    return (rebel_pool_int_array_read_access*)memnew(
        PoolVector<rebel_int>::Read(self->read())
    );
}

rebel_pool_int_array_write_access GDAPI* rebel_pool_int_array_write(
    rebel_pool_int_array* p_self
) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    return (rebel_pool_int_array_write_access*)memnew(
        PoolVector<rebel_int>::Write(self->write())
    );
}

void GDAPI rebel_pool_int_array_set(
    rebel_pool_int_array* p_self,
    const rebel_int p_idx,
    const rebel_int p_data
) {
    PoolVector<rebel_int>* self = (PoolVector<rebel_int>*)p_self;
    self->set(p_idx, p_data);
}

rebel_int GDAPI rebel_pool_int_array_get(
    const rebel_pool_int_array* p_self,
    const rebel_int p_idx
) {
    const PoolVector<rebel_int>* self = (const PoolVector<rebel_int>*)p_self;
    return self->get(p_idx);
}

rebel_int GDAPI rebel_pool_int_array_size(const rebel_pool_int_array* p_self) {
    const PoolVector<rebel_int>* self = (const PoolVector<rebel_int>*)p_self;
    return self->size();
}

rebel_bool GDAPI rebel_pool_int_array_empty(const rebel_pool_int_array* p_self
) {
    const PoolVector<rebel_int>* self = (const PoolVector<rebel_int>*)p_self;
    return self->empty();
}

void GDAPI rebel_pool_int_array_destroy(rebel_pool_int_array* p_self) {
    ((PoolVector<rebel_int>*)p_self)->~PoolVector();
}

// real

void GDAPI rebel_pool_real_array_new(rebel_pool_real_array* r_dest) {
    PoolVector<rebel_real>* dest = (PoolVector<rebel_real>*)r_dest;
    memnew_placement(dest, PoolVector<rebel_real>);
}

void GDAPI rebel_pool_real_array_new_copy(
    rebel_pool_real_array* r_dest,
    const rebel_pool_real_array* p_src
) {
    PoolVector<rebel_real>* dest      = (PoolVector<rebel_real>*)r_dest;
    const PoolVector<rebel_real>* src = (const PoolVector<rebel_real>*)p_src;
    memnew_placement(dest, PoolVector<rebel_real>(*src));
}

void GDAPI rebel_pool_real_array_new_with_array(
    rebel_pool_real_array* r_dest,
    const rebel_array* p_a
) {
    PoolVector<rebel_real>* dest = (PoolVector<rebel_real>*)r_dest;
    Array* a                     = (Array*)p_a;
    memnew_placement(dest, PoolVector<rebel_real>);

    dest->resize(a->size());
    for (int i = 0; i < a->size(); i++) {
        dest->set(i, (*a)[i]);
    }
}

void GDAPI rebel_pool_real_array_append(
    rebel_pool_real_array* p_self,
    const rebel_real p_data
) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    self->append(p_data);
}

void GDAPI rebel_pool_real_array_append_array(
    rebel_pool_real_array* p_self,
    const rebel_pool_real_array* p_array
) {
    PoolVector<rebel_real>* self  = (PoolVector<rebel_real>*)p_self;
    PoolVector<rebel_real>* array = (PoolVector<rebel_real>*)p_array;
    self->append_array(*array);
}

rebel_error GDAPI rebel_pool_real_array_insert(
    rebel_pool_real_array* p_self,
    const rebel_int p_idx,
    const rebel_real p_data
) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    return (rebel_error)self->insert(p_idx, p_data);
}

void GDAPI rebel_pool_real_array_invert(rebel_pool_real_array* p_self) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    self->invert();
}

void GDAPI rebel_pool_real_array_push_back(
    rebel_pool_real_array* p_self,
    const rebel_real p_data
) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    self->push_back(p_data);
}

void GDAPI rebel_pool_real_array_remove(
    rebel_pool_real_array* p_self,
    const rebel_int p_idx
) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_pool_real_array_resize(
    rebel_pool_real_array* p_self,
    const rebel_int p_size
) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    self->resize(p_size);
}

rebel_pool_real_array_read_access GDAPI* rebel_pool_real_array_read(
    const rebel_pool_real_array* p_self
) {
    const PoolVector<rebel_real>* self = (const PoolVector<rebel_real>*)p_self;
    return (rebel_pool_real_array_read_access*)memnew(
        PoolVector<rebel_real>::Read(self->read())
    );
}

rebel_pool_int_array_write_access GDAPI* rebel_pool_real_array_write(
    rebel_pool_real_array* p_self
) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    return (rebel_pool_real_array_write_access*)memnew(
        PoolVector<rebel_real>::Write(self->write())
    );
}

void GDAPI rebel_pool_real_array_set(
    rebel_pool_real_array* p_self,
    const rebel_int p_idx,
    const rebel_real p_data
) {
    PoolVector<rebel_real>* self = (PoolVector<rebel_real>*)p_self;
    self->set(p_idx, p_data);
}

rebel_real GDAPI rebel_pool_real_array_get(
    const rebel_pool_real_array* p_self,
    const rebel_int p_idx
) {
    const PoolVector<rebel_real>* self = (const PoolVector<rebel_real>*)p_self;
    return self->get(p_idx);
}

rebel_int GDAPI rebel_pool_real_array_size(const rebel_pool_real_array* p_self
) {
    const PoolVector<rebel_real>* self = (const PoolVector<rebel_real>*)p_self;
    return self->size();
}

rebel_bool GDAPI rebel_pool_real_array_empty(const rebel_pool_real_array* p_self
) {
    const PoolVector<rebel_real>* self = (const PoolVector<rebel_real>*)p_self;
    return self->empty();
}

void GDAPI rebel_pool_real_array_destroy(rebel_pool_real_array* p_self) {
    ((PoolVector<rebel_real>*)p_self)->~PoolVector();
}

// string

void GDAPI rebel_pool_string_array_new(rebel_pool_string_array* r_dest) {
    PoolVector<String>* dest = (PoolVector<String>*)r_dest;
    memnew_placement(dest, PoolVector<String>);
}

void GDAPI rebel_pool_string_array_new_copy(
    rebel_pool_string_array* r_dest,
    const rebel_pool_string_array* p_src
) {
    PoolVector<String>* dest      = (PoolVector<String>*)r_dest;
    const PoolVector<String>* src = (const PoolVector<String>*)p_src;
    memnew_placement(dest, PoolVector<String>(*src));
}

void GDAPI rebel_pool_string_array_new_with_array(
    rebel_pool_string_array* r_dest,
    const rebel_array* p_a
) {
    PoolVector<String>* dest = (PoolVector<String>*)r_dest;
    Array* a                 = (Array*)p_a;
    memnew_placement(dest, PoolVector<String>);

    dest->resize(a->size());
    for (int i = 0; i < a->size(); i++) {
        dest->set(i, (*a)[i]);
    }
}

void GDAPI rebel_pool_string_array_append(
    rebel_pool_string_array* p_self,
    const rebel_string* p_data
) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    String& s                = *(String*)p_data;
    self->append(s);
}

void GDAPI rebel_pool_string_array_append_array(
    rebel_pool_string_array* p_self,
    const rebel_pool_string_array* p_array
) {
    PoolVector<String>* self  = (PoolVector<String>*)p_self;
    PoolVector<String>* array = (PoolVector<String>*)p_array;
    self->append_array(*array);
}

rebel_error GDAPI rebel_pool_string_array_insert(
    rebel_pool_string_array* p_self,
    const rebel_int p_idx,
    const rebel_string* p_data
) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    String& s                = *(String*)p_data;
    return (rebel_error)self->insert(p_idx, s);
}

void GDAPI rebel_pool_string_array_invert(rebel_pool_string_array* p_self) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    self->invert();
}

void GDAPI rebel_pool_string_array_push_back(
    rebel_pool_string_array* p_self,
    const rebel_string* p_data
) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    String& s                = *(String*)p_data;
    self->push_back(s);
}

void GDAPI rebel_pool_string_array_remove(
    rebel_pool_string_array* p_self,
    const rebel_int p_idx
) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_pool_string_array_resize(
    rebel_pool_string_array* p_self,
    const rebel_int p_size
) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    self->resize(p_size);
}

rebel_pool_string_array_read_access GDAPI* rebel_pool_string_array_read(
    const rebel_pool_string_array* p_self
) {
    const PoolVector<String>* self = (const PoolVector<String>*)p_self;
    return (rebel_pool_string_array_read_access*)memnew(
        PoolVector<String>::Read(self->read())
    );
}

rebel_pool_string_array_write_access GDAPI* rebel_pool_string_array_write(
    rebel_pool_string_array* p_self
) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    return (rebel_pool_string_array_write_access*)memnew(
        PoolVector<String>::Write(self->write())
    );
}

void GDAPI rebel_pool_string_array_set(
    rebel_pool_string_array* p_self,
    const rebel_int p_idx,
    const rebel_string* p_data
) {
    PoolVector<String>* self = (PoolVector<String>*)p_self;
    String& s                = *(String*)p_data;
    self->set(p_idx, s);
}

rebel_string GDAPI rebel_pool_string_array_get(
    const rebel_pool_string_array* p_self,
    const rebel_int p_idx
) {
    const PoolVector<String>* self = (const PoolVector<String>*)p_self;
    rebel_string str;
    String* s = (String*)&str;
    memnew_placement(s, String);
    *s = self->get(p_idx);
    return str;
}

rebel_int GDAPI
rebel_pool_string_array_size(const rebel_pool_string_array* p_self) {
    const PoolVector<String>* self = (const PoolVector<String>*)p_self;
    return self->size();
}

rebel_bool GDAPI
rebel_pool_string_array_empty(const rebel_pool_string_array* p_self) {
    const PoolVector<String>* self = (const PoolVector<String>*)p_self;
    return self->empty();
}

void GDAPI rebel_pool_string_array_destroy(rebel_pool_string_array* p_self) {
    ((PoolVector<String>*)p_self)->~PoolVector();
}

// vector2

void GDAPI rebel_pool_vector2_array_new(rebel_pool_vector2_array* r_dest) {
    PoolVector<Vector2>* dest = (PoolVector<Vector2>*)r_dest;
    memnew_placement(dest, PoolVector<Vector2>);
}

void GDAPI rebel_pool_vector2_array_new_copy(
    rebel_pool_vector2_array* r_dest,
    const rebel_pool_vector2_array* p_src
) {
    PoolVector<Vector2>* dest      = (PoolVector<Vector2>*)r_dest;
    const PoolVector<Vector2>* src = (const PoolVector<Vector2>*)p_src;
    memnew_placement(dest, PoolVector<Vector2>(*src));
}

void GDAPI rebel_pool_vector2_array_new_with_array(
    rebel_pool_vector2_array* r_dest,
    const rebel_array* p_a
) {
    PoolVector<Vector2>* dest = (PoolVector<Vector2>*)r_dest;
    Array* a                  = (Array*)p_a;
    memnew_placement(dest, PoolVector<Vector2>);

    dest->resize(a->size());
    for (int i = 0; i < a->size(); i++) {
        dest->set(i, (*a)[i]);
    }
}

void GDAPI rebel_pool_vector2_array_append(
    rebel_pool_vector2_array* p_self,
    const rebel_vector2* p_data
) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    Vector2& s                = *(Vector2*)p_data;
    self->append(s);
}

void GDAPI rebel_pool_vector2_array_append_array(
    rebel_pool_vector2_array* p_self,
    const rebel_pool_vector2_array* p_array
) {
    PoolVector<Vector2>* self  = (PoolVector<Vector2>*)p_self;
    PoolVector<Vector2>* array = (PoolVector<Vector2>*)p_array;
    self->append_array(*array);
}

rebel_error GDAPI rebel_pool_vector2_array_insert(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_idx,
    const rebel_vector2* p_data
) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    Vector2& s                = *(Vector2*)p_data;
    return (rebel_error)self->insert(p_idx, s);
}

void GDAPI rebel_pool_vector2_array_invert(rebel_pool_vector2_array* p_self) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    self->invert();
}

void GDAPI rebel_pool_vector2_array_push_back(
    rebel_pool_vector2_array* p_self,
    const rebel_vector2* p_data
) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    Vector2& s                = *(Vector2*)p_data;
    self->push_back(s);
}

void GDAPI rebel_pool_vector2_array_remove(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_idx
) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_pool_vector2_array_resize(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_size
) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    self->resize(p_size);
}

rebel_pool_vector2_array_read_access GDAPI* rebel_pool_vector2_array_read(
    const rebel_pool_vector2_array* p_self
) {
    const PoolVector<Vector2>* self = (const PoolVector<Vector2>*)p_self;
    return (rebel_pool_vector2_array_read_access*)memnew(
        PoolVector<Vector2>::Read(self->read())
    );
}

rebel_pool_vector2_array_write_access GDAPI* rebel_pool_vector2_array_write(
    rebel_pool_vector2_array* p_self
) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    return (rebel_pool_vector2_array_write_access*)memnew(
        PoolVector<Vector2>::Write(self->write())
    );
}

void GDAPI rebel_pool_vector2_array_set(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_idx,
    const rebel_vector2* p_data
) {
    PoolVector<Vector2>* self = (PoolVector<Vector2>*)p_self;
    Vector2& s                = *(Vector2*)p_data;
    self->set(p_idx, s);
}

rebel_vector2 GDAPI rebel_pool_vector2_array_get(
    const rebel_pool_vector2_array* p_self,
    const rebel_int p_idx
) {
    const PoolVector<Vector2>* self = (const PoolVector<Vector2>*)p_self;
    rebel_vector2 v;
    Vector2* s = (Vector2*)&v;
    *s         = self->get(p_idx);
    return v;
}

rebel_int GDAPI
rebel_pool_vector2_array_size(const rebel_pool_vector2_array* p_self) {
    const PoolVector<Vector2>* self = (const PoolVector<Vector2>*)p_self;
    return self->size();
}

rebel_bool GDAPI
rebel_pool_vector2_array_empty(const rebel_pool_vector2_array* p_self) {
    const PoolVector<Vector2>* self = (const PoolVector<Vector2>*)p_self;
    return self->empty();
}

void GDAPI rebel_pool_vector2_array_destroy(rebel_pool_vector2_array* p_self) {
    ((PoolVector<Vector2>*)p_self)->~PoolVector();
}

// vector3

void GDAPI rebel_pool_vector3_array_new(rebel_pool_vector3_array* r_dest) {
    PoolVector<Vector3>* dest = (PoolVector<Vector3>*)r_dest;
    memnew_placement(dest, PoolVector<Vector3>);
}

void GDAPI rebel_pool_vector3_array_new_copy(
    rebel_pool_vector3_array* r_dest,
    const rebel_pool_vector3_array* p_src
) {
    PoolVector<Vector3>* dest      = (PoolVector<Vector3>*)r_dest;
    const PoolVector<Vector3>* src = (const PoolVector<Vector3>*)p_src;
    memnew_placement(dest, PoolVector<Vector3>(*src));
}

void GDAPI rebel_pool_vector3_array_new_with_array(
    rebel_pool_vector3_array* r_dest,
    const rebel_array* p_a
) {
    PoolVector<Vector3>* dest = (PoolVector<Vector3>*)r_dest;
    Array* a                  = (Array*)p_a;
    memnew_placement(dest, PoolVector<Vector3>);

    dest->resize(a->size());
    for (int i = 0; i < a->size(); i++) {
        dest->set(i, (*a)[i]);
    }
}

void GDAPI rebel_pool_vector3_array_append(
    rebel_pool_vector3_array* p_self,
    const rebel_vector3* p_data
) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    Vector3& s                = *(Vector3*)p_data;
    self->append(s);
}

void GDAPI rebel_pool_vector3_array_append_array(
    rebel_pool_vector3_array* p_self,
    const rebel_pool_vector3_array* p_array
) {
    PoolVector<Vector3>* self  = (PoolVector<Vector3>*)p_self;
    PoolVector<Vector3>* array = (PoolVector<Vector3>*)p_array;
    self->append_array(*array);
}

rebel_error GDAPI rebel_pool_vector3_array_insert(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_idx,
    const rebel_vector3* p_data
) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    Vector3& s                = *(Vector3*)p_data;
    return (rebel_error)self->insert(p_idx, s);
}

void GDAPI rebel_pool_vector3_array_invert(rebel_pool_vector3_array* p_self) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    self->invert();
}

void GDAPI rebel_pool_vector3_array_push_back(
    rebel_pool_vector3_array* p_self,
    const rebel_vector3* p_data
) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    Vector3& s                = *(Vector3*)p_data;
    self->push_back(s);
}

void GDAPI rebel_pool_vector3_array_remove(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_idx
) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_pool_vector3_array_resize(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_size
) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    self->resize(p_size);
}

rebel_pool_vector3_array_read_access GDAPI* rebel_pool_vector3_array_read(
    const rebel_pool_vector3_array* p_self
) {
    const PoolVector<Vector3>* self = (const PoolVector<Vector3>*)p_self;
    return (rebel_pool_vector3_array_read_access*)memnew(
        PoolVector<Vector3>::Read(self->read())
    );
}

rebel_pool_vector3_array_write_access GDAPI* rebel_pool_vector3_array_write(
    rebel_pool_vector3_array* p_self
) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    return (rebel_pool_vector3_array_write_access*)memnew(
        PoolVector<Vector3>::Write(self->write())
    );
}

void GDAPI rebel_pool_vector3_array_set(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_idx,
    const rebel_vector3* p_data
) {
    PoolVector<Vector3>* self = (PoolVector<Vector3>*)p_self;
    Vector3& s                = *(Vector3*)p_data;
    self->set(p_idx, s);
}

rebel_vector3 GDAPI rebel_pool_vector3_array_get(
    const rebel_pool_vector3_array* p_self,
    const rebel_int p_idx
) {
    const PoolVector<Vector3>* self = (const PoolVector<Vector3>*)p_self;
    rebel_vector3 v;
    Vector3* s = (Vector3*)&v;
    *s         = self->get(p_idx);
    return v;
}

rebel_int GDAPI
rebel_pool_vector3_array_size(const rebel_pool_vector3_array* p_self) {
    const PoolVector<Vector3>* self = (const PoolVector<Vector3>*)p_self;
    return self->size();
}

rebel_bool GDAPI
rebel_pool_vector3_array_empty(const rebel_pool_vector3_array* p_self) {
    const PoolVector<Vector3>* self = (const PoolVector<Vector3>*)p_self;
    return self->empty();
}

void GDAPI rebel_pool_vector3_array_destroy(rebel_pool_vector3_array* p_self) {
    ((PoolVector<Vector3>*)p_self)->~PoolVector();
}

// color

void GDAPI rebel_pool_color_array_new(rebel_pool_color_array* r_dest) {
    PoolVector<Color>* dest = (PoolVector<Color>*)r_dest;
    memnew_placement(dest, PoolVector<Color>);
}

void GDAPI rebel_pool_color_array_new_copy(
    rebel_pool_color_array* r_dest,
    const rebel_pool_color_array* p_src
) {
    PoolVector<Color>* dest      = (PoolVector<Color>*)r_dest;
    const PoolVector<Color>* src = (const PoolVector<Color>*)p_src;
    memnew_placement(dest, PoolVector<Color>(*src));
}

void GDAPI rebel_pool_color_array_new_with_array(
    rebel_pool_color_array* r_dest,
    const rebel_array* p_a
) {
    PoolVector<Color>* dest = (PoolVector<Color>*)r_dest;
    Array* a                = (Array*)p_a;
    memnew_placement(dest, PoolVector<Color>);

    dest->resize(a->size());
    for (int i = 0; i < a->size(); i++) {
        dest->set(i, (*a)[i]);
    }
}

void GDAPI rebel_pool_color_array_append(
    rebel_pool_color_array* p_self,
    const rebel_color* p_data
) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    Color& s                = *(Color*)p_data;
    self->append(s);
}

void GDAPI rebel_pool_color_array_append_array(
    rebel_pool_color_array* p_self,
    const rebel_pool_color_array* p_array
) {
    PoolVector<Color>* self  = (PoolVector<Color>*)p_self;
    PoolVector<Color>* array = (PoolVector<Color>*)p_array;
    self->append_array(*array);
}

rebel_error GDAPI rebel_pool_color_array_insert(
    rebel_pool_color_array* p_self,
    const rebel_int p_idx,
    const rebel_color* p_data
) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    Color& s                = *(Color*)p_data;
    return (rebel_error)self->insert(p_idx, s);
}

void GDAPI rebel_pool_color_array_invert(rebel_pool_color_array* p_self) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    self->invert();
}

void GDAPI rebel_pool_color_array_push_back(
    rebel_pool_color_array* p_self,
    const rebel_color* p_data
) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    Color& s                = *(Color*)p_data;
    self->push_back(s);
}

void GDAPI rebel_pool_color_array_remove(
    rebel_pool_color_array* p_self,
    const rebel_int p_idx
) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    self->remove(p_idx);
}

void GDAPI rebel_pool_color_array_resize(
    rebel_pool_color_array* p_self,
    const rebel_int p_size
) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    self->resize(p_size);
}

rebel_pool_color_array_read_access GDAPI* rebel_pool_color_array_read(
    const rebel_pool_color_array* p_self
) {
    const PoolVector<Color>* self = (const PoolVector<Color>*)p_self;
    return (rebel_pool_color_array_read_access*)memnew(
        PoolVector<Color>::Read(self->read())
    );
}

rebel_pool_color_array_write_access GDAPI* rebel_pool_color_array_write(
    rebel_pool_color_array* p_self
) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    return (rebel_pool_color_array_write_access*)memnew(
        PoolVector<Color>::Write(self->write())
    );
}

void GDAPI rebel_pool_color_array_set(
    rebel_pool_color_array* p_self,
    const rebel_int p_idx,
    const rebel_color* p_data
) {
    PoolVector<Color>* self = (PoolVector<Color>*)p_self;
    Color& s                = *(Color*)p_data;
    self->set(p_idx, s);
}

rebel_color GDAPI rebel_pool_color_array_get(
    const rebel_pool_color_array* p_self,
    const rebel_int p_idx
) {
    const PoolVector<Color>* self = (const PoolVector<Color>*)p_self;
    rebel_color v;
    Color* s = (Color*)&v;
    *s       = self->get(p_idx);
    return v;
}

rebel_int GDAPI rebel_pool_color_array_size(const rebel_pool_color_array* p_self
) {
    const PoolVector<Color>* self = (const PoolVector<Color>*)p_self;
    return self->size();
}

rebel_bool GDAPI
rebel_pool_color_array_empty(const rebel_pool_color_array* p_self) {
    const PoolVector<Color>* self = (const PoolVector<Color>*)p_self;
    return self->empty();
}

void GDAPI rebel_pool_color_array_destroy(rebel_pool_color_array* p_self) {
    ((PoolVector<Color>*)p_self)->~PoolVector();
}

//
// read accessor functions
//

rebel_pool_byte_array_read_access GDAPI* rebel_pool_byte_array_read_access_copy(
    const rebel_pool_byte_array_read_access* p_other
) {
    PoolVector<uint8_t>::Read* other = (PoolVector<uint8_t>::Read*)p_other;
    return (rebel_pool_byte_array_read_access*)memnew(
        PoolVector<uint8_t>::Read(*other)
    );
}

const uint8_t GDAPI* rebel_pool_byte_array_read_access_ptr(
    const rebel_pool_byte_array_read_access* p_read
) {
    const PoolVector<uint8_t>::Read* read =
        (const PoolVector<uint8_t>::Read*)p_read;
    return read->ptr();
}

void GDAPI rebel_pool_byte_array_read_access_operator_assign(
    rebel_pool_byte_array_read_access* p_read,
    rebel_pool_byte_array_read_access* p_other
) {
    PoolVector<uint8_t>::Read* read  = (PoolVector<uint8_t>::Read*)p_read;
    PoolVector<uint8_t>::Read* other = (PoolVector<uint8_t>::Read*)p_other;
    read->operator=(*other);
}

void GDAPI rebel_pool_byte_array_read_access_destroy(
    rebel_pool_byte_array_read_access* p_read
) {
    memdelete((PoolVector<uint8_t>::Read*)p_read);
}

rebel_pool_int_array_read_access GDAPI* rebel_pool_int_array_read_access_copy(
    const rebel_pool_int_array_read_access* p_other
) {
    PoolVector<rebel_int>::Read* other = (PoolVector<rebel_int>::Read*)p_other;
    return (rebel_pool_int_array_read_access*)memnew(
        PoolVector<rebel_int>::Read(*other)
    );
}

const rebel_int GDAPI* rebel_pool_int_array_read_access_ptr(
    const rebel_pool_int_array_read_access* p_read
) {
    const PoolVector<rebel_int>::Read* read =
        (const PoolVector<rebel_int>::Read*)p_read;
    return read->ptr();
}

void GDAPI rebel_pool_int_array_read_access_operator_assign(
    rebel_pool_int_array_read_access* p_read,
    rebel_pool_int_array_read_access* p_other
) {
    PoolVector<rebel_int>::Read* read  = (PoolVector<rebel_int>::Read*)p_read;
    PoolVector<rebel_int>::Read* other = (PoolVector<rebel_int>::Read*)p_other;
    read->operator=(*other);
}

void GDAPI rebel_pool_int_array_read_access_destroy(
    rebel_pool_int_array_read_access* p_read
) {
    memdelete((PoolVector<rebel_int>::Read*)p_read);
}

rebel_pool_real_array_read_access GDAPI* rebel_pool_real_array_read_access_copy(
    const rebel_pool_real_array_read_access* p_other
) {
    PoolVector<rebel_real>::Read* other =
        (PoolVector<rebel_real>::Read*)p_other;
    return (rebel_pool_real_array_read_access*)memnew(
        PoolVector<rebel_real>::Read(*other)
    );
}

const rebel_real GDAPI* rebel_pool_real_array_read_access_ptr(
    const rebel_pool_real_array_read_access* p_read
) {
    const PoolVector<rebel_real>::Read* read =
        (const PoolVector<rebel_real>::Read*)p_read;
    return read->ptr();
}

void GDAPI rebel_pool_real_array_read_access_operator_assign(
    rebel_pool_real_array_read_access* p_read,
    rebel_pool_real_array_read_access* p_other
) {
    PoolVector<rebel_real>::Read* read = (PoolVector<rebel_real>::Read*)p_read;
    PoolVector<rebel_real>::Read* other =
        (PoolVector<rebel_real>::Read*)p_other;
    read->operator=(*other);
}

void GDAPI rebel_pool_real_array_read_access_destroy(
    rebel_pool_real_array_read_access* p_read
) {
    memdelete((PoolVector<rebel_real>::Read*)p_read);
}

rebel_pool_string_array_read_access GDAPI*
rebel_pool_string_array_read_access_copy(
    const rebel_pool_string_array_read_access* p_other
) {
    PoolVector<String>::Read* other = (PoolVector<String>::Read*)p_other;
    return (rebel_pool_string_array_read_access*)memnew(
        PoolVector<String>::Read(*other)
    );
}

const rebel_string GDAPI* rebel_pool_string_array_read_access_ptr(
    const rebel_pool_string_array_read_access* p_read
) {
    const PoolVector<String>::Read* read =
        (const PoolVector<String>::Read*)p_read;
    return (const rebel_string*)read->ptr();
}

void GDAPI rebel_pool_string_array_read_access_operator_assign(
    rebel_pool_string_array_read_access* p_read,
    rebel_pool_string_array_read_access* p_other
) {
    PoolVector<String>::Read* read  = (PoolVector<String>::Read*)p_read;
    PoolVector<String>::Read* other = (PoolVector<String>::Read*)p_other;
    read->operator=(*other);
}

void GDAPI rebel_pool_string_array_read_access_destroy(
    rebel_pool_string_array_read_access* p_read
) {
    memdelete((PoolVector<String>::Read*)p_read);
}

rebel_pool_vector2_array_read_access GDAPI*
rebel_pool_vector2_array_read_access_copy(
    const rebel_pool_vector2_array_read_access* p_other
) {
    PoolVector<Vector2>::Read* other = (PoolVector<Vector2>::Read*)p_other;
    return (rebel_pool_vector2_array_read_access*)memnew(
        PoolVector<Vector2>::Read(*other)
    );
}

const rebel_vector2 GDAPI* rebel_pool_vector2_array_read_access_ptr(
    const rebel_pool_vector2_array_read_access* p_read
) {
    const PoolVector<Vector2>::Read* read =
        (const PoolVector<Vector2>::Read*)p_read;
    return (const rebel_vector2*)read->ptr();
}

void GDAPI rebel_pool_vector2_array_read_access_operator_assign(
    rebel_pool_vector2_array_read_access* p_read,
    rebel_pool_vector2_array_read_access* p_other
) {
    PoolVector<Vector2>::Read* read  = (PoolVector<Vector2>::Read*)p_read;
    PoolVector<Vector2>::Read* other = (PoolVector<Vector2>::Read*)p_other;
    read->operator=(*other);
}

void GDAPI rebel_pool_vector2_array_read_access_destroy(
    rebel_pool_vector2_array_read_access* p_read
) {
    memdelete((PoolVector<Vector2>::Read*)p_read);
}

rebel_pool_vector3_array_read_access GDAPI*
rebel_pool_vector3_array_read_access_copy(
    const rebel_pool_vector3_array_read_access* p_other
) {
    PoolVector<Vector3>::Read* other = (PoolVector<Vector3>::Read*)p_other;
    return (rebel_pool_vector3_array_read_access*)memnew(
        PoolVector<Vector3>::Read(*other)
    );
}

const rebel_vector3 GDAPI* rebel_pool_vector3_array_read_access_ptr(
    const rebel_pool_vector3_array_read_access* p_read
) {
    const PoolVector<Vector3>::Read* read =
        (const PoolVector<Vector3>::Read*)p_read;
    return (const rebel_vector3*)read->ptr();
}

void GDAPI rebel_pool_vector3_array_read_access_operator_assign(
    rebel_pool_vector3_array_read_access* p_read,
    rebel_pool_vector3_array_read_access* p_other
) {
    PoolVector<Vector3>::Read* read  = (PoolVector<Vector3>::Read*)p_read;
    PoolVector<Vector3>::Read* other = (PoolVector<Vector3>::Read*)p_other;
    read->operator=(*other);
}

void GDAPI rebel_pool_vector3_array_read_access_destroy(
    rebel_pool_vector3_array_read_access* p_read
) {
    memdelete((PoolVector<Vector2>::Read*)p_read);
}

rebel_pool_color_array_read_access GDAPI*
rebel_pool_color_array_read_access_copy(
    const rebel_pool_color_array_read_access* p_other
) {
    PoolVector<Color>::Read* other = (PoolVector<Color>::Read*)p_other;
    return (rebel_pool_color_array_read_access*)memnew(
        PoolVector<Color>::Read(*other)
    );
}

const rebel_color GDAPI* rebel_pool_color_array_read_access_ptr(
    const rebel_pool_color_array_read_access* p_read
) {
    const PoolVector<Color>::Read* read =
        (const PoolVector<Color>::Read*)p_read;
    return (const rebel_color*)read->ptr();
}

void GDAPI rebel_pool_color_array_read_access_operator_assign(
    rebel_pool_color_array_read_access* p_read,
    rebel_pool_color_array_read_access* p_other
) {
    PoolVector<Color>::Read* read  = (PoolVector<Color>::Read*)p_read;
    PoolVector<Color>::Read* other = (PoolVector<Color>::Read*)p_other;
    read->operator=(*other);
}

void GDAPI rebel_pool_color_array_read_access_destroy(
    rebel_pool_color_array_read_access* p_read
) {
    memdelete((PoolVector<Color>::Read*)p_read);
}

//
// write accessor functions
//

rebel_pool_byte_array_write_access GDAPI*
rebel_pool_byte_array_write_access_copy(
    const rebel_pool_byte_array_write_access* p_other
) {
    PoolVector<uint8_t>::Write* other = (PoolVector<uint8_t>::Write*)p_other;
    return (rebel_pool_byte_array_write_access*)memnew(
        PoolVector<uint8_t>::Write(*other)
    );
}

uint8_t GDAPI* rebel_pool_byte_array_write_access_ptr(
    const rebel_pool_byte_array_write_access* p_write
) {
    PoolVector<uint8_t>::Write* write = (PoolVector<uint8_t>::Write*)p_write;
    return write->ptr();
}

void GDAPI rebel_pool_byte_array_write_access_operator_assign(
    rebel_pool_byte_array_write_access* p_write,
    rebel_pool_byte_array_write_access* p_other
) {
    PoolVector<uint8_t>::Write* write = (PoolVector<uint8_t>::Write*)p_write;
    PoolVector<uint8_t>::Write* other = (PoolVector<uint8_t>::Write*)p_other;
    write->operator=(*other);
}

void GDAPI rebel_pool_byte_array_write_access_destroy(
    rebel_pool_byte_array_write_access* p_write
) {
    memdelete((PoolVector<uint8_t>::Write*)p_write);
}

rebel_pool_int_array_write_access GDAPI* rebel_pool_int_array_write_access_copy(
    const rebel_pool_int_array_write_access* p_other
) {
    PoolVector<rebel_int>::Write* other =
        (PoolVector<rebel_int>::Write*)p_other;
    return (rebel_pool_int_array_write_access*)memnew(
        PoolVector<rebel_int>::Write(*other)
    );
}

rebel_int GDAPI* rebel_pool_int_array_write_access_ptr(
    const rebel_pool_int_array_write_access* p_write
) {
    PoolVector<rebel_int>::Write* write =
        (PoolVector<rebel_int>::Write*)p_write;
    return write->ptr();
}

void GDAPI rebel_pool_int_array_write_access_operator_assign(
    rebel_pool_int_array_write_access* p_write,
    rebel_pool_int_array_write_access* p_other
) {
    PoolVector<rebel_int>::Write* write =
        (PoolVector<rebel_int>::Write*)p_write;
    PoolVector<rebel_int>::Write* other =
        (PoolVector<rebel_int>::Write*)p_other;
    write->operator=(*other);
}

void GDAPI rebel_pool_int_array_write_access_destroy(
    rebel_pool_int_array_write_access* p_write
) {
    memdelete((PoolVector<rebel_int>::Write*)p_write);
}

rebel_pool_real_array_write_access GDAPI*
rebel_pool_real_array_write_access_copy(
    const rebel_pool_real_array_write_access* p_other
) {
    PoolVector<rebel_real>::Write* other =
        (PoolVector<rebel_real>::Write*)p_other;
    return (rebel_pool_real_array_write_access*)memnew(
        PoolVector<rebel_real>::Write(*other)
    );
}

rebel_real GDAPI* rebel_pool_real_array_write_access_ptr(
    const rebel_pool_real_array_write_access* p_write
) {
    PoolVector<rebel_real>::Write* write =
        (PoolVector<rebel_real>::Write*)p_write;
    return write->ptr();
}

void GDAPI rebel_pool_real_array_write_access_operator_assign(
    rebel_pool_real_array_write_access* p_write,
    rebel_pool_real_array_write_access* p_other
) {
    PoolVector<rebel_real>::Write* write =
        (PoolVector<rebel_real>::Write*)p_write;
    PoolVector<rebel_real>::Write* other =
        (PoolVector<rebel_real>::Write*)p_other;
    write->operator=(*other);
}

void GDAPI rebel_pool_real_array_write_access_destroy(
    rebel_pool_real_array_write_access* p_write
) {
    memdelete((PoolVector<rebel_real>::Write*)p_write);
}

rebel_pool_string_array_write_access GDAPI*
rebel_pool_string_array_write_access_copy(
    const rebel_pool_string_array_write_access* p_other
) {
    PoolVector<String>::Write* other = (PoolVector<String>::Write*)p_other;
    return (rebel_pool_string_array_write_access*)memnew(
        PoolVector<String>::Write(*other)
    );
}

rebel_string GDAPI* rebel_pool_string_array_write_access_ptr(
    const rebel_pool_string_array_write_access* p_write
) {
    PoolVector<String>::Write* write = (PoolVector<String>::Write*)p_write;
    return (rebel_string*)write->ptr();
}

void GDAPI rebel_pool_string_array_write_access_operator_assign(
    rebel_pool_string_array_write_access* p_write,
    rebel_pool_string_array_write_access* p_other
) {
    PoolVector<String>::Write* write = (PoolVector<String>::Write*)p_write;
    PoolVector<String>::Write* other = (PoolVector<String>::Write*)p_other;
    write->operator=(*other);
}

void GDAPI rebel_pool_string_array_write_access_destroy(
    rebel_pool_string_array_write_access* p_write
) {
    memdelete((PoolVector<String>::Write*)p_write);
}

rebel_pool_vector2_array_write_access GDAPI*
rebel_pool_vector2_array_write_access_copy(
    const rebel_pool_vector2_array_write_access* p_other
) {
    PoolVector<Vector2>::Write* other = (PoolVector<Vector2>::Write*)p_other;
    return (rebel_pool_vector2_array_write_access*)memnew(
        PoolVector<Vector2>::Write(*other)
    );
}

rebel_vector2 GDAPI* rebel_pool_vector2_array_write_access_ptr(
    const rebel_pool_vector2_array_write_access* p_write
) {
    PoolVector<Vector2>::Write* write = (PoolVector<Vector2>::Write*)p_write;
    return (rebel_vector2*)write->ptr();
}

void GDAPI rebel_pool_vector2_array_write_access_operator_assign(
    rebel_pool_vector2_array_write_access* p_write,
    rebel_pool_vector2_array_write_access* p_other
) {
    PoolVector<Vector2>::Write* write = (PoolVector<Vector2>::Write*)p_write;
    PoolVector<Vector2>::Write* other = (PoolVector<Vector2>::Write*)p_other;
    write->operator=(*other);
}

void GDAPI rebel_pool_vector2_array_write_access_destroy(
    rebel_pool_vector2_array_write_access* p_write
) {
    memdelete((PoolVector<Vector2>::Write*)p_write);
}

rebel_pool_vector3_array_write_access GDAPI*
rebel_pool_vector3_array_write_access_copy(
    const rebel_pool_vector3_array_write_access* p_other
) {
    PoolVector<Vector3>::Write* other = (PoolVector<Vector3>::Write*)p_other;
    return (rebel_pool_vector3_array_write_access*)memnew(
        PoolVector<Vector3>::Write(*other)
    );
}

rebel_vector3 GDAPI* rebel_pool_vector3_array_write_access_ptr(
    const rebel_pool_vector3_array_write_access* p_write
) {
    PoolVector<Vector3>::Write* write = (PoolVector<Vector3>::Write*)p_write;
    return (rebel_vector3*)write->ptr();
}

void GDAPI rebel_pool_vector3_array_write_access_operator_assign(
    rebel_pool_vector3_array_write_access* p_write,
    rebel_pool_vector3_array_write_access* p_other
) {
    PoolVector<Vector3>::Write* write = (PoolVector<Vector3>::Write*)p_write;
    PoolVector<Vector3>::Write* other = (PoolVector<Vector3>::Write*)p_other;
    write->operator=(*other);
}

void GDAPI rebel_pool_vector3_array_write_access_destroy(
    rebel_pool_vector3_array_write_access* p_write
) {
    memdelete((PoolVector<Vector3>::Write*)p_write);
}

rebel_pool_color_array_write_access GDAPI*
rebel_pool_color_array_write_access_copy(
    const rebel_pool_color_array_write_access* p_other
) {
    PoolVector<Color>::Write* other = (PoolVector<Color>::Write*)p_other;
    return (rebel_pool_color_array_write_access*)memnew(
        PoolVector<Color>::Write(*other)
    );
}

rebel_color GDAPI* rebel_pool_color_array_write_access_ptr(
    const rebel_pool_color_array_write_access* p_write
) {
    PoolVector<Color>::Write* write = (PoolVector<Color>::Write*)p_write;
    return (rebel_color*)write->ptr();
}

void GDAPI rebel_pool_color_array_write_access_operator_assign(
    rebel_pool_color_array_write_access* p_write,
    rebel_pool_color_array_write_access* p_other
) {
    PoolVector<Color>::Write* write = (PoolVector<Color>::Write*)p_write;
    PoolVector<Color>::Write* other = (PoolVector<Color>::Write*)p_other;
    write->operator=(*other);
}

void GDAPI rebel_pool_color_array_write_access_destroy(
    rebel_pool_color_array_write_access* p_write
) {
    memdelete((PoolVector<Color>::Write*)p_write);
}

#ifdef __cplusplus
}
#endif
