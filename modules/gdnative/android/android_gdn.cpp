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
#include "platforms/android/android_jni_os.h"
#include "platforms/android/android_jni_thread.h"
#include "platforms/android/android_os.h"
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
    AndroidOS* android_os = (AndroidOS*)OS::get_singleton();
    return android_os->get_android_jni_os()->get_activity();
#else
    return nullptr;
#endif
}

jobject GDAPI godot_android_get_surface() {
#ifdef __ANDROID__
    AndroidOS* android_os = (AndroidOS*)OS::get_singleton();
    return android_os->get_android_jni_os()->get_surface();
#else
    return nullptr;
#endif
}

bool GDAPI godot_android_is_activity_resumed() {
#ifdef __ANDROID__
    AndroidOS* android_os = (AndroidOS*)OS::get_singleton();
    return android_os->get_android_jni_os()->is_activity_resumed();
#else
    return false;
#endif
}

#ifdef __cplusplus
}
#endif
