// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/string_name.h"

#include "core/string_name.h"
#include "core/ustring.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_string_name) == sizeof(StringName),
    "StringName size mismatch"
);

void GDAPI
rebel_string_name_new(rebel_string_name* r_dest, const rebel_string* p_name) {
    StringName* dest   = (StringName*)r_dest;
    const String* name = (const String*)p_name;
    memnew_placement(dest, StringName(*name));
}

void GDAPI
rebel_string_name_new_data(rebel_string_name* r_dest, const char* p_name) {
    StringName* dest = (StringName*)r_dest;
    memnew_placement(dest, StringName(p_name));
}

rebel_string GDAPI rebel_string_name_get_name(const rebel_string_name* p_self) {
    rebel_string ret;
    const StringName* self = (const StringName*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

uint32_t GDAPI rebel_string_name_get_hash(const rebel_string_name* p_self) {
    const StringName* self = (const StringName*)p_self;
    return self->hash();
}

const void GDAPI* rebel_string_name_get_data_unique_pointer(
    const rebel_string_name* p_self
) {
    const StringName* self = (const StringName*)p_self;
    return self->data_unique_pointer();
}

rebel_bool GDAPI rebel_string_name_operator_equal(
    const rebel_string_name* p_self,
    const rebel_string_name* p_other
) {
    const StringName* self  = (const StringName*)p_self;
    const StringName* other = (const StringName*)p_other;
    return self == other;
}

rebel_bool GDAPI rebel_string_name_operator_less(
    const rebel_string_name* p_self,
    const rebel_string_name* p_other
) {
    const StringName* self  = (const StringName*)p_self;
    const StringName* other = (const StringName*)p_other;
    return self < other;
}

void GDAPI rebel_string_name_destroy(rebel_string_name* p_self) {
    StringName* self = (StringName*)p_self;
    self->~StringName();
}

#ifdef __cplusplus
}
#endif
