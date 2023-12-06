// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GD_GLUE_H
#define GD_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "../mono_gd/gd_mono_marshal.h"

MonoObject* godot_icall_GD_bytes2var(
    MonoArray* p_bytes,
    MonoBoolean p_allow_objects
);

MonoObject* godot_icall_GD_convert(MonoObject* p_what, int32_t p_type);

int godot_icall_GD_hash(MonoObject* p_var);

MonoObject* godot_icall_GD_instance_from_id(uint64_t p_instance_id);

void godot_icall_GD_print(MonoArray* p_what);

void godot_icall_GD_printerr(MonoArray* p_what);

void godot_icall_GD_printraw(MonoArray* p_what);

void godot_icall_GD_prints(MonoArray* p_what);

void godot_icall_GD_printt(MonoArray* p_what);

float godot_icall_GD_randf();

uint32_t godot_icall_GD_randi();

void godot_icall_GD_randomize();

double godot_icall_GD_rand_range(double from, double to);

uint32_t godot_icall_GD_rand_seed(uint64_t seed, uint64_t* newSeed);

void godot_icall_GD_seed(uint64_t p_seed);

MonoString* godot_icall_GD_str(MonoArray* p_what);

MonoObject* godot_icall_GD_str2var(MonoString* p_str);

MonoBoolean godot_icall_GD_type_exists(MonoString* p_type);

MonoArray* godot_icall_GD_var2bytes(
    MonoObject* p_var,
    MonoBoolean p_full_objects
);

MonoString* godot_icall_GD_var2str(MonoObject* p_var);

MonoObject* godot_icall_DefaultGodotTaskScheduler();

// Register internal calls

void godot_register_gd_icalls();

#endif // MONO_GLUE_ENABLED

#endif // GD_GLUE_H
