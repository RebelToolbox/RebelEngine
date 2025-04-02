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

Array* rebel_icall_Array_Ctor();

void rebel_icall_Array_Dtor(Array* ptr);

MonoObject* rebel_icall_Array_At(Array* ptr, int index);

MonoObject* rebel_icall_Array_At_Generic(
    Array* ptr,
    int index,
    uint32_t type_encoding,
    GDMonoClass* type_class
);

void rebel_icall_Array_SetAt(Array* ptr, int index, MonoObject* value);

int rebel_icall_Array_Count(Array* ptr);

int rebel_icall_Array_Add(Array* ptr, MonoObject* item);

void rebel_icall_Array_Clear(Array* ptr);

MonoBoolean rebel_icall_Array_Contains(Array* ptr, MonoObject* item);

void rebel_icall_Array_CopyTo(Array* ptr, MonoArray* array, int array_index);

Array* rebel_icall_Array_Duplicate(Array* ptr, MonoBoolean deep);

int rebel_icall_Array_IndexOf(Array* ptr, MonoObject* item);

void rebel_icall_Array_Insert(Array* ptr, int index, MonoObject* item);

MonoBoolean rebel_icall_Array_Remove(Array* ptr, MonoObject* item);

void rebel_icall_Array_RemoveAt(Array* ptr, int index);

int32_t rebel_icall_Array_Resize(Array* ptr, int new_size);

void rebel_icall_Array_Generic_GetElementTypeInfo(
    MonoReflectionType* refltype,
    uint32_t* type_encoding,
    GDMonoClass** type_class
);

MonoString* rebel_icall_Array_ToString(Array* ptr);

// Dictionary

Dictionary* rebel_icall_Dictionary_Ctor();

void rebel_icall_Dictionary_Dtor(Dictionary* ptr);

MonoObject* rebel_icall_Dictionary_GetValue(Dictionary* ptr, MonoObject* key);

MonoObject* rebel_icall_Dictionary_GetValue_Generic(
    Dictionary* ptr,
    MonoObject* key,
    uint32_t type_encoding,
    GDMonoClass* type_class
);

void rebel_icall_Dictionary_SetValue(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

Array* rebel_icall_Dictionary_Keys(Dictionary* ptr);

Array* rebel_icall_Dictionary_Values(Dictionary* ptr);

int rebel_icall_Dictionary_Count(Dictionary* ptr);

void rebel_icall_Dictionary_Add(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

void rebel_icall_Dictionary_Clear(Dictionary* ptr);

MonoBoolean rebel_icall_Dictionary_Contains(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

MonoBoolean rebel_icall_Dictionary_ContainsKey(
    Dictionary* ptr,
    MonoObject* key
);

Dictionary* rebel_icall_Dictionary_Duplicate(Dictionary* ptr, MonoBoolean deep);

MonoBoolean rebel_icall_Dictionary_RemoveKey(Dictionary* ptr, MonoObject* key);

MonoBoolean rebel_icall_Dictionary_Remove(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
);

MonoBoolean rebel_icall_Dictionary_TryGetValue(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject** value
);

MonoBoolean rebel_icall_Dictionary_TryGetValue_Generic(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject** value,
    uint32_t type_encoding,
    GDMonoClass* type_class
);

void rebel_icall_Dictionary_Generic_GetValueTypeInfo(
    MonoReflectionType* refltype,
    uint32_t* type_encoding,
    GDMonoClass** type_class
);

MonoString* rebel_icall_Dictionary_ToString(Dictionary* ptr);

// Register internal calls

void rebel_register_collections_icalls();

#endif // MONO_GLUE_ENABLED

#endif // COLLECTIONS_GLUE_H
