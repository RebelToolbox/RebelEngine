// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "modules/gdnative/gdnative.h"

// Code by Paritosh97 with minor tweaks by Mux213
// These entry points are only for the android platform and are simple stubs in
// all others.

#ifdef __ANDROID__
#include "platform/android/java_godot_wrapper.h"
#include "platform/android/os_android.h"
#include "platform/android/thread_jandroid.h"
#else
#define JNIEnv  void
#define jobject void*
#endif

#ifdef __cplusplus
extern "C" {
#endif

JNIEnv* GDAPI godot_android_get_env() {
#ifdef __ANDROID__
    return get_jni_env();
#else
    return nullptr;
#endif
}

jobject GDAPI godot_android_get_activity() {
#ifdef __ANDROID__
    OS_Android* os_android = (OS_Android*)OS::get_singleton();
    return os_android->get_godot_java()->get_activity();
#else
    return nullptr;
#endif
}

jobject GDAPI godot_android_get_surface() {
#ifdef __ANDROID__
    OS_Android* os_android = (OS_Android*)OS::get_singleton();
    return os_android->get_godot_java()->get_surface();
#else
    return nullptr;
#endif
}

bool GDAPI godot_android_is_activity_resumed() {
#ifdef __ANDROID__
    OS_Android* os_android = (OS_Android*)OS::get_singleton();
    return os_android->get_godot_java()->is_activity_resumed();
#else
    return false;
#endif
}

#ifdef __cplusplus
}
#endif
