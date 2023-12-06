// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STRING_GLUE_H
#define STRING_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "../mono_gd/gd_mono_marshal.h"

MonoArray* godot_icall_String_md5_buffer(MonoString* p_str);

MonoString* godot_icall_String_md5_text(MonoString* p_str);

int godot_icall_String_rfind(MonoString* p_str, MonoString* p_what, int p_from);

int godot_icall_String_rfindn(
    MonoString* p_str,
    MonoString* p_what,
    int p_from
);

MonoArray* godot_icall_String_sha256_buffer(MonoString* p_str);

MonoString* godot_icall_String_sha256_text(MonoString* p_str);

// Register internal calls

void godot_register_string_icalls();

#endif // MONO_GLUE_ENABLED

#endif // STRING_GLUE_H
