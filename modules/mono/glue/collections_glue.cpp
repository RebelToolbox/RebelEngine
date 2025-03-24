// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "collections_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "../mono_gd/gd_mono_cache.h"
#include "../mono_gd/gd_mono_class.h"
#include "../mono_gd/gd_mono_utils.h"

#include <mono/metadata/exception.h>

Array* rebel_icall_Array_Ctor() {
    return memnew(Array);
}

void rebel_icall_Array_Dtor(Array* ptr) {
    memdelete(ptr);
}

MonoObject* rebel_icall_Array_At(Array* ptr, int32_t index) {
    if (index < 0 || index >= ptr->size()) {
        GDMonoUtils::set_pending_exception(
            mono_get_exception_index_out_of_range()
        );
        return NULL;
    }
    return GDMonoMarshal::variant_to_mono_object(ptr->operator[](index));
}

MonoObject* rebel_icall_Array_At_Generic(
    Array* ptr,
    int32_t index,
    uint32_t type_encoding,
    GDMonoClass* type_class
) {
    if (index < 0 || index >= ptr->size()) {
        GDMonoUtils::set_pending_exception(
            mono_get_exception_index_out_of_range()
        );
        return NULL;
    }
    return GDMonoMarshal::variant_to_mono_object(
        ptr->operator[](index),
        ManagedType(type_encoding, type_class)
    );
}

void rebel_icall_Array_SetAt(Array* ptr, int32_t index, MonoObject* value) {
    if (index < 0 || index >= ptr->size()) {
        GDMonoUtils::set_pending_exception(
            mono_get_exception_index_out_of_range()
        );
        return;
    }
    ptr->operator[](index) = GDMonoMarshal::mono_object_to_variant(value);
}

int32_t rebel_icall_Array_Count(Array* ptr) {
    return ptr->size();
}

int32_t rebel_icall_Array_Add(Array* ptr, MonoObject* item) {
    ptr->append(GDMonoMarshal::mono_object_to_variant(item));
    return ptr->size();
}

void rebel_icall_Array_Clear(Array* ptr) {
    ptr->clear();
}

MonoBoolean rebel_icall_Array_Contains(Array* ptr, MonoObject* item) {
    return ptr->find(GDMonoMarshal::mono_object_to_variant(item)) != -1;
}

void rebel_icall_Array_CopyTo(
    Array* ptr,
    MonoArray* array,
    int32_t array_index
) {
    unsigned int count = ptr->size();

    if (mono_array_length(array) < (array_index + count)) {
        MonoException* exc = mono_get_exception_argument(
            "",
            "Destination array was not long enough. Check destIndex and "
            "length, and the array's lower bounds."
        );
        GDMonoUtils::set_pending_exception(exc);
        return;
    }

    for (unsigned int i = 0; i < count; i++) {
        MonoObject* boxed =
            GDMonoMarshal::variant_to_mono_object(ptr->operator[](i));
        mono_array_setref(array, array_index, boxed);
        array_index++;
    }
}

Array* rebel_icall_Array_Ctor_MonoArray(MonoArray* mono_array) {
    Array* rebel_array = memnew(Array);
    unsigned int count = mono_array_length(mono_array);
    rebel_array->resize(count);
    for (unsigned int i = 0; i < count; i++) {
        MonoObject* item = mono_array_get(mono_array, MonoObject*, i);
        rebel_icall_Array_SetAt(rebel_array, i, item);
    }
    return rebel_array;
}

Array* rebel_icall_Array_Duplicate(Array* ptr, MonoBoolean deep) {
    return memnew(Array(ptr->duplicate(deep)));
}

Array* rebel_icall_Array_Concatenate(Array* left, Array* right) {
    int count        = left->size() + right->size();
    Array* new_array = memnew(Array(left->duplicate(false)));
    new_array->resize(count);
    for (unsigned int i = 0; i < (unsigned int)right->size(); i++) {
        new_array->operator[](i + left->size()) = right->operator[](i);
    }
    return new_array;
}

int32_t rebel_icall_Array_IndexOf(Array* ptr, MonoObject* item) {
    return ptr->find(GDMonoMarshal::mono_object_to_variant(item));
}

void rebel_icall_Array_Insert(Array* ptr, int32_t index, MonoObject* item) {
    if (index < 0 || index > ptr->size()) {
        GDMonoUtils::set_pending_exception(
            mono_get_exception_index_out_of_range()
        );
        return;
    }
    ptr->insert(index, GDMonoMarshal::mono_object_to_variant(item));
}

MonoBoolean rebel_icall_Array_Remove(Array* ptr, MonoObject* item) {
    int idx = ptr->find(GDMonoMarshal::mono_object_to_variant(item));
    if (idx >= 0) {
        ptr->remove(idx);
        return true;
    }
    return false;
}

void rebel_icall_Array_RemoveAt(Array* ptr, int32_t index) {
    if (index < 0 || index >= ptr->size()) {
        GDMonoUtils::set_pending_exception(
            mono_get_exception_index_out_of_range()
        );
        return;
    }
    ptr->remove(index);
}

int32_t rebel_icall_Array_Resize(Array* ptr, int32_t new_size) {
    return (int32_t)ptr->resize(new_size);
}

void rebel_icall_Array_Shuffle(Array* ptr) {
    ptr->shuffle();
}

void rebel_icall_Array_Generic_GetElementTypeInfo(
    MonoReflectionType* refltype,
    uint32_t* type_encoding,
    GDMonoClass** type_class
) {
    MonoType* elem_type = mono_reflection_type_get_type(refltype);

    *type_encoding            = mono_type_get_type(elem_type);
    MonoClass* type_class_raw = mono_class_from_mono_type(elem_type);
    *type_class = GDMono::get_singleton()->get_class(type_class_raw);
}

MonoString* rebel_icall_Array_ToString(Array* ptr) {
    return GDMonoMarshal::mono_string_from_rebel(Variant(*ptr).operator String()
    );
}

Dictionary* rebel_icall_Dictionary_Ctor() {
    return memnew(Dictionary);
}

void rebel_icall_Dictionary_Dtor(Dictionary* ptr) {
    memdelete(ptr);
}

MonoObject* rebel_icall_Dictionary_GetValue(Dictionary* ptr, MonoObject* key) {
    Variant* ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
    if (ret == NULL) {
        MonoObject* exc = mono_object_new(
            mono_domain_get(),
            CACHED_CLASS(KeyNotFoundException)->get_mono_ptr()
        );
#ifdef DEBUG_ENABLED
        CRASH_COND(!exc);
#endif
        GDMonoUtils::runtime_object_init(
            exc,
            CACHED_CLASS(KeyNotFoundException)
        );
        GDMonoUtils::set_pending_exception((MonoException*)exc);
        return NULL;
    }
    return GDMonoMarshal::variant_to_mono_object(ret);
}

MonoObject* rebel_icall_Dictionary_GetValue_Generic(
    Dictionary* ptr,
    MonoObject* key,
    uint32_t type_encoding,
    GDMonoClass* type_class
) {
    Variant* ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
    if (ret == NULL) {
        MonoObject* exc = mono_object_new(
            mono_domain_get(),
            CACHED_CLASS(KeyNotFoundException)->get_mono_ptr()
        );
#ifdef DEBUG_ENABLED
        CRASH_COND(!exc);
#endif
        GDMonoUtils::runtime_object_init(
            exc,
            CACHED_CLASS(KeyNotFoundException)
        );
        GDMonoUtils::set_pending_exception((MonoException*)exc);
        return NULL;
    }
    return GDMonoMarshal::variant_to_mono_object(
        ret,
        ManagedType(type_encoding, type_class)
    );
}

void rebel_icall_Dictionary_SetValue(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
) {
    ptr->operator[](GDMonoMarshal::mono_object_to_variant(key)) =
        GDMonoMarshal::mono_object_to_variant(value);
}

Array* rebel_icall_Dictionary_Keys(Dictionary* ptr) {
    return memnew(Array(ptr->keys()));
}

Array* rebel_icall_Dictionary_Values(Dictionary* ptr) {
    return memnew(Array(ptr->values()));
}

int32_t rebel_icall_Dictionary_Count(Dictionary* ptr) {
    return ptr->size();
}

int32_t rebel_icall_Dictionary_KeyValuePairs(
    Dictionary* ptr,
    Array** keys,
    Array** values
) {
    *keys   = rebel_icall_Dictionary_Keys(ptr);
    *values = rebel_icall_Dictionary_Values(ptr);
    return rebel_icall_Dictionary_Count(ptr);
}

void rebel_icall_Dictionary_KeyValuePairAt(
    Dictionary* ptr,
    int index,
    MonoObject** key,
    MonoObject** value
) {
    *key = GDMonoMarshal::variant_to_mono_object(ptr->get_key_at_index(index));
    *value =
        GDMonoMarshal::variant_to_mono_object(ptr->get_value_at_index(index));
}

void rebel_icall_Dictionary_KeyValuePairAt_Generic(
    Dictionary* ptr,
    int index,
    MonoObject** key,
    MonoObject** value,
    uint32_t value_type_encoding,
    GDMonoClass* value_type_class
) {
    ManagedType type(value_type_encoding, value_type_class);
    *key = GDMonoMarshal::variant_to_mono_object(ptr->get_key_at_index(index));
    *value = GDMonoMarshal::variant_to_mono_object(
        ptr->get_value_at_index(index),
        type
    );
}

void rebel_icall_Dictionary_Add(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
) {
    Variant varKey = GDMonoMarshal::mono_object_to_variant(key);
    Variant* ret   = ptr->getptr(varKey);
    if (ret != NULL) {
        GDMonoUtils::set_pending_exception(mono_get_exception_argument(
            "key",
            "An element with the same key already exists"
        ));
        return;
    }
    ptr->operator[](varKey) = GDMonoMarshal::mono_object_to_variant(value);
}

void rebel_icall_Dictionary_Clear(Dictionary* ptr) {
    ptr->clear();
}

MonoBoolean rebel_icall_Dictionary_Contains(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
) {
    // no dupes
    Variant* ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
    return ret != NULL && *ret == GDMonoMarshal::mono_object_to_variant(value);
}

MonoBoolean rebel_icall_Dictionary_ContainsKey(
    Dictionary* ptr,
    MonoObject* key
) {
    return ptr->has(GDMonoMarshal::mono_object_to_variant(key));
}

Dictionary* rebel_icall_Dictionary_Duplicate(
    Dictionary* ptr,
    MonoBoolean deep
) {
    return memnew(Dictionary(ptr->duplicate(deep)));
}

MonoBoolean rebel_icall_Dictionary_RemoveKey(Dictionary* ptr, MonoObject* key) {
    return ptr->erase(GDMonoMarshal::mono_object_to_variant(key));
}

MonoBoolean rebel_icall_Dictionary_Remove(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject* value
) {
    Variant varKey = GDMonoMarshal::mono_object_to_variant(key);

    // no dupes
    Variant* ret = ptr->getptr(varKey);
    if (ret != NULL && *ret == GDMonoMarshal::mono_object_to_variant(value)) {
        ptr->erase(varKey);
        return true;
    }

    return false;
}

MonoBoolean rebel_icall_Dictionary_TryGetValue(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject** value
) {
    Variant* ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
    if (ret == NULL) {
        *value = NULL;
        return false;
    }
    *value = GDMonoMarshal::variant_to_mono_object(ret);
    return true;
}

MonoBoolean rebel_icall_Dictionary_TryGetValue_Generic(
    Dictionary* ptr,
    MonoObject* key,
    MonoObject** value,
    uint32_t type_encoding,
    GDMonoClass* type_class
) {
    Variant* ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
    if (ret == NULL) {
        *value = NULL;
        return false;
    }
    *value = GDMonoMarshal::variant_to_mono_object(
        ret,
        ManagedType(type_encoding, type_class)
    );
    return true;
}

void rebel_icall_Dictionary_Generic_GetValueTypeInfo(
    MonoReflectionType* refltype,
    uint32_t* type_encoding,
    GDMonoClass** type_class
) {
    MonoType* value_type = mono_reflection_type_get_type(refltype);

    *type_encoding            = mono_type_get_type(value_type);
    MonoClass* type_class_raw = mono_class_from_mono_type(value_type);
    *type_class = GDMono::get_singleton()->get_class(type_class_raw);
}

MonoString* rebel_icall_Dictionary_ToString(Dictionary* ptr) {
    return GDMonoMarshal::mono_string_from_rebel(Variant(*ptr).operator String()
    );
}

void rebel_register_collections_icalls() {
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Ctor",
        rebel_icall_Array_Ctor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Ctor_MonoArray",
        rebel_icall_Array_Ctor_MonoArray
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Dtor",
        rebel_icall_Array_Dtor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_At",
        rebel_icall_Array_At
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_At_Generic",
        rebel_icall_Array_At_Generic
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_SetAt",
        rebel_icall_Array_SetAt
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Count",
        rebel_icall_Array_Count
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Add",
        rebel_icall_Array_Add
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Clear",
        rebel_icall_Array_Clear
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Concatenate",
        rebel_icall_Array_Concatenate
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Contains",
        rebel_icall_Array_Contains
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_CopyTo",
        rebel_icall_Array_CopyTo
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Duplicate",
        rebel_icall_Array_Duplicate
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_IndexOf",
        rebel_icall_Array_IndexOf
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Insert",
        rebel_icall_Array_Insert
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Remove",
        rebel_icall_Array_Remove
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_RemoveAt",
        rebel_icall_Array_RemoveAt
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Resize",
        rebel_icall_Array_Resize
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Shuffle",
        rebel_icall_Array_Shuffle
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_Generic_GetElementTypeInfo",
        rebel_icall_Array_Generic_GetElementTypeInfo
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Array::rebel_icall_Array_ToString",
        rebel_icall_Array_ToString
    );

    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Ctor",
        rebel_icall_Dictionary_Ctor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Dtor",
        rebel_icall_Dictionary_Dtor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_GetValue",
        rebel_icall_Dictionary_GetValue
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_GetValue_Generic",
        rebel_icall_Dictionary_GetValue_Generic
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_SetValue",
        rebel_icall_Dictionary_SetValue
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Keys",
        rebel_icall_Dictionary_Keys
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Values",
        rebel_icall_Dictionary_Values
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Count",
        rebel_icall_Dictionary_Count
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_KeyValuePairs",
        rebel_icall_Dictionary_KeyValuePairs
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_KeyValuePairAt",
        rebel_icall_Dictionary_KeyValuePairAt
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_KeyValuePairAt_"
        "Generic",
        rebel_icall_Dictionary_KeyValuePairAt_Generic
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Add",
        rebel_icall_Dictionary_Add
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Clear",
        rebel_icall_Dictionary_Clear
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Contains",
        rebel_icall_Dictionary_Contains
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_ContainsKey",
        rebel_icall_Dictionary_ContainsKey
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Duplicate",
        rebel_icall_Dictionary_Duplicate
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_RemoveKey",
        rebel_icall_Dictionary_RemoveKey
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Remove",
        rebel_icall_Dictionary_Remove
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_TryGetValue",
        rebel_icall_Dictionary_TryGetValue
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_TryGetValue_"
        "Generic",
        rebel_icall_Dictionary_TryGetValue_Generic
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_Generic_"
        "GetValueTypeInfo",
        rebel_icall_Dictionary_Generic_GetValueTypeInfo
    );
    GDMonoUtils::add_internal_call(
        "Rebel.Collections.Dictionary::rebel_icall_Dictionary_ToString",
        rebel_icall_Dictionary_ToString
    );
}

#endif // MONO_GLUE_ENABLED
