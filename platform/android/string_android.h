// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STRING_ANDROID_H
#define STRING_ANDROID_H
#include "core/ustring.h"
#include "thread_jandroid.h"

#include <jni.h>

/**
 * Converts JNI jstring to Godot String.
 * @param source Source JNI string. If null an empty string is returned.
 * @param env JNI environment instance. If null obtained by get_jni_env().
 * @return Godot string instance.
 */
static inline String jstring_to_string(jstring source, JNIEnv* env = NULL) {
    String result;
    if (source) {
        if (!env) {
            env = get_jni_env();
        }
        const char* const source_utf8 = env->GetStringUTFChars(source, NULL);
        if (source_utf8) {
            result.parse_utf8(source_utf8);
            env->ReleaseStringUTFChars(source, source_utf8);
        }
    }
    return result;
}

#endif // STRING_ANDROID_H
