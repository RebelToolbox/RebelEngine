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
    sizeof(godot_string_name) == sizeof(StringName),
    "StringName size mismatch"
);

void GDAPI
godot_string_name_new(godot_string_name* r_dest, const godot_string* p_name) {
    StringName* dest   = (StringName*)r_dest;
    const String* name = (const String*)p_name;
    memnew_placement(dest, StringName(*name));
}

void GDAPI
godot_string_name_new_data(godot_string_name* r_dest, const char* p_name) {
    StringName* dest = (StringName*)r_dest;
    memnew_placement(dest, StringName(p_name));
}

godot_string GDAPI godot_string_name_get_name(const godot_string_name* p_self) {
    godot_string ret;
    const StringName* self = (const StringName*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

uint32_t GDAPI godot_string_name_get_hash(const godot_string_name* p_self) {
    const StringName* self = (const StringName*)p_self;
    return self->hash();
}

const void GDAPI* godot_string_name_get_data_unique_pointer(
    const godot_string_name* p_self
) {
    const StringName* self = (const StringName*)p_self;
    return self->data_unique_pointer();
}

godot_bool GDAPI godot_string_name_operator_equal(
    const godot_string_name* p_self,
    const godot_string_name* p_other
) {
    const StringName* self  = (const StringName*)p_self;
    const StringName* other = (const StringName*)p_other;
    return self == other;
}

godot_bool GDAPI godot_string_name_operator_less(
    const godot_string_name* p_self,
    const godot_string_name* p_other
) {
    const StringName* self  = (const StringName*)p_self;
    const StringName* other = (const StringName*)p_other;
    return self < other;
}

void GDAPI godot_string_name_destroy(godot_string_name* p_self) {
    StringName* self = (StringName*)p_self;
    self->~StringName();
}

#ifdef __cplusplus
}
#endif
