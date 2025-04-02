// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "string_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "core/ustring.h"
#include "core/variant.h"
#include "core/vector.h"

MonoArray* rebel_icall_String_md5_buffer(MonoString* p_str) {
    Vector<uint8_t> ret =
        GDMonoMarshal::mono_string_to_rebel(p_str).md5_buffer();
    // TODO Check possible Array/Vector<uint8_t> problem?
    return GDMonoMarshal::Array_to_mono_array(Variant(ret));
}

MonoString* rebel_icall_String_md5_text(MonoString* p_str) {
    String ret = GDMonoMarshal::mono_string_to_rebel(p_str).md5_text();
    return GDMonoMarshal::mono_string_from_rebel(ret);
}

int rebel_icall_String_rfind(
    MonoString* p_str,
    MonoString* p_what,
    int p_from
) {
    String what = GDMonoMarshal::mono_string_to_rebel(p_what);
    return GDMonoMarshal::mono_string_to_rebel(p_str).rfind(what, p_from);
}

int rebel_icall_String_rfindn(
    MonoString* p_str,
    MonoString* p_what,
    int p_from
) {
    String what = GDMonoMarshal::mono_string_to_rebel(p_what);
    return GDMonoMarshal::mono_string_to_rebel(p_str).rfindn(what, p_from);
}

MonoArray* rebel_icall_String_sha256_buffer(MonoString* p_str) {
    Vector<uint8_t> ret =
        GDMonoMarshal::mono_string_to_rebel(p_str).sha256_buffer();
    return GDMonoMarshal::Array_to_mono_array(Variant(ret));
}

MonoString* rebel_icall_String_sha256_text(MonoString* p_str) {
    String ret = GDMonoMarshal::mono_string_to_rebel(p_str).sha256_text();
    return GDMonoMarshal::mono_string_from_rebel(ret);
}

MonoString* rebel_icall_String_simplify_path(MonoString* p_str) {
    String ret = GDMonoMarshal::mono_string_to_rebel(p_str).simplify_path();
    return GDMonoMarshal::mono_string_from_rebel(ret);
}

void rebel_register_string_icalls() {
    GDMonoUtils::add_internal_call(
        "Rebel.StringExtensions::rebel_icall_String_md5_buffer",
        rebel_icall_String_md5_buffer
    );
    GDMonoUtils::add_internal_call(
        "Rebel.StringExtensions::rebel_icall_String_md5_text",
        rebel_icall_String_md5_text
    );
    GDMonoUtils::add_internal_call(
        "Rebel.StringExtensions::rebel_icall_String_rfind",
        rebel_icall_String_rfind
    );
    GDMonoUtils::add_internal_call(
        "Rebel.StringExtensions::rebel_icall_String_rfindn",
        rebel_icall_String_rfindn
    );
    GDMonoUtils::add_internal_call(
        "Rebel.StringExtensions::rebel_icall_String_sha256_buffer",
        rebel_icall_String_sha256_buffer
    );
    GDMonoUtils::add_internal_call(
        "Rebel.StringExtensions::rebel_icall_String_sha256_text",
        rebel_icall_String_sha256_text
    );
    GDMonoUtils::add_internal_call(
        "Rebel.StringExtensions::rebel_icall_String_simplify_path",
        rebel_icall_String_simplify_path
    );
}

#endif // MONO_GLUE_ENABLED
