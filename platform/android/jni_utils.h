// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JNI_UTILS_H
#define JNI_UTILS_H

#include "core/engine.h"
#include "core/variant.h"

#include <jni.h>

struct jvalret {
    jobject obj;
    jvalue val;

    jvalret() {
        obj = NULL;
    }
};

jvalret _variant_to_jvalue(
    JNIEnv* env,
    Variant::Type p_type,
    const Variant* p_arg,
    bool force_jobject = false
);

String _get_class_name(JNIEnv* env, jclass cls, bool* array);

Variant _jobject_to_variant(JNIEnv* env, jobject obj);

Variant::Type get_jni_type(const String& p_type);

const char* get_jni_sig(const String& p_type);

String string_from_jstring(JNIEnv* env, jstring string);

#endif // JNI_UTILS_H
