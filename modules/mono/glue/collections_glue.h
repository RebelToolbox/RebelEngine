// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLECTIONS_GLUE_H
#define COLLECTIONS_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "../mono_gd/gd_mono_marshal.h"
#include "core/array.h"

// Array

Array* godot_icall_Array_Ctor();

void godot_icall_Array_Dtor(Array* ptr);

MonoObject* godot_icall_Array_At(Array* ptr, int index);

MonoObject* godot_icall_Array_At_Generic(
    Array* ptr,
    int index,
    uint32_t type_encoding,
    GDMonoClass* type_class
);

void godot_icall_Array_SetAt(Array* ptr, int index, MonoObject* value);

int godot_icall_Array_Count(Array* ptr);

int godot_icall_Array_Add(Array* ptr, MonoObject* item);

void godot_icall_Array_Clear(Array* ptr);

MonoBoolean godot_icall_Array_Contains(Array* ptr, MonoObject* item);

void godot_icall_Array_CopyTo(Array* ptr, MonoArray* array, int array_index);

Array* godot_icall_Array_Duplicate(Array* ptr, MonoBoolean deep);

int godot_icall_Array_IndexOf(Array* ptr, MonoObject* item);

void godot_icall_Array_Insert(Array* ptr, int index, MonoObject* item);

MonoBoolean godot_icall_Array_Remove(Array* ptr, MonoObject* item);

void godot_icall_Array_RemoveAt(Array* ptr, int index);

int32_t godot_icall_Array_Resize(Array* ptr, int new_size);

void godot_icall_Array_Generic_GetElementTypeInfo(
    MonoReflectionType* refltype,
    uint32_t* type_encoding,
    GDMonoClass** type_class
);

MonoString* godot_icall_Array_ToString(Array* ptr);

// Dictionary

Dictionary* godot_icall_Dictionary_Ctor();

void godot_icall_Dictionary_Dtor(Dictionary* ptr);

MonoObject* godot_icall_Dictionary_GetValue(Dictionary* ptr, MonoObject* key);

MonoObject* godot_icall_Dictionary_GetValue_Generic(
    Dictionary* ptr,
    MonoObject* key,
    uint32_t type_encoding,
    GDMonoClass* type_class
);

void godot_icall_Dictionary_SetValue(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

Array* godot_icall_Dictionary_Keys(Dictionary* ptr);

Array* godot_icall_Dictionary_Values(Dictionary* ptr);

int godot_icall_Dictionary_Count(Dictionary* ptr);

void godot_icall_Dictionary_Add(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

void godot_icall_Dictionary_Clear(Dictionary* ptr);

MonoBoolean godot_icall_Dictionary_Contains(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

MonoBoolean godot_icall_Dictionary_ContainsKey(
    Dictionary* ptr,
    MonoObject* key
);

Dictionary* godot_icall_Dictionary_Duplicate(Dictionary* ptr, MonoBoolean deep);

MonoBoolean godot_icall_Dictionary_RemoveKey(Dictionary* ptr, MonoObject* key);

MonoBoolean godot_icall_Dictionary_Remove(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

MonoBoolean godot_icall_Dictionary_TryGetValue(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject** value
);

MonoBoolean godot_icall_Dictionary_TryGetValue_Generic(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject** value,
    uint32_t type_encoding,
    GDMonoClass* type_class
);

void godot_icall_Dictionary_Generic_GetValueTypeInfo(
    MonoReflectionType* refltype,
    uint32_t* type_encoding,
    GDMonoClass** type_class
);

MonoString* godot_icall_Dictionary_ToString(Dictionary* ptr);

// Register internal calls

void godot_register_collections_icalls();

#endif // MONO_GLUE_ENABLED

#endif // COLLECTIONS_GLUE_H
