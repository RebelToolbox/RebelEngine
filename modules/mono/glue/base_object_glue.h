// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BASE_OBJECT_GLUE_H
#define BASE_OBJECT_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "../mono_gd/gd_mono_marshal.h"
#include "core/class_db.h"
#include "core/object.h"

Object* rebel_icall_Object_Ctor(MonoObject* p_obj);

void rebel_icall_Object_Disposed(MonoObject* p_obj, Object* p_ptr);

void rebel_icall_Reference_Disposed(
    MonoObject* p_obj,
    Object* p_ptr,
    MonoBoolean p_is_finalizer
);

MethodBind* rebel_icall_Object_ClassDB_get_method(
    MonoString* p_type,
    MonoString* p_method
);

MonoObject* rebel_icall_Object_weakref(Object* p_obj);

int32_t rebel_icall_SignalAwaiter_connect(
    Object* p_source,
    MonoString* p_signal,
    Object* p_target,
    MonoObject* p_awaiter
);

// DynamicRebelObject

MonoArray* rebel_icall_DynamicRebelObject_SetMemberList(Object* p_ptr);

MonoBoolean rebel_icall_DynamicRebelObject_InvokeMember(
    Object* p_ptr,
    MonoString* p_name,
    MonoArray* p_args,
    MonoObject** r_result
);

MonoBoolean rebel_icall_DynamicRebelObject_GetMember(
    Object* p_ptr,
    MonoString* p_name,
    MonoObject** r_result
);

MonoBoolean rebel_icall_DynamicRebelObject_SetMember(
    Object* p_ptr,
    MonoString* p_name,
    MonoObject* p_value
);

MonoString* rebel_icall_Object_ToString(Object* p_ptr);

// Register internal calls

void rebel_register_object_icalls();

#endif // MONO_GLUE_ENABLED

#endif // BASE_OBJECT_GLUE_H
