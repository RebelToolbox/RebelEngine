// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NODEPATH_GLUE_H
#define NODEPATH_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "../mono_gd/gd_mono_marshal.h"
#include "core/node_path.h"

NodePath* godot_icall_NodePath_Ctor(MonoString* p_path);

void godot_icall_NodePath_Dtor(NodePath* p_ptr);

MonoString* godot_icall_NodePath_operator_String(NodePath* p_np);

MonoBoolean godot_icall_NodePath_is_absolute(NodePath* p_ptr);

uint32_t godot_icall_NodePath_get_name_count(NodePath* p_ptr);

MonoString* godot_icall_NodePath_get_name(NodePath* p_ptr, uint32_t p_idx);

uint32_t godot_icall_NodePath_get_subname_count(NodePath* p_ptr);

MonoString* godot_icall_NodePath_get_subname(NodePath* p_ptr, uint32_t p_idx);

MonoString* godot_icall_NodePath_get_concatenated_subnames(NodePath* p_ptr);

NodePath* godot_icall_NodePath_get_as_property_path(NodePath* p_ptr);

MonoBoolean godot_icall_NodePath_is_empty(NodePath* p_ptr);

// Register internal calls

void godot_register_nodepath_icalls();

#endif // MONO_GLUE_ENABLED

#endif // NODEPATH_GLUE_H
