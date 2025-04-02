// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_ANDROID_H
#define GDNATIVE_ANDROID_H
#include <gdnative/gdnative.h>

#ifdef __ANDROID__
#include <jni.h>
#else
#define JNIEnv  void
#define jobject void*
#endif

#ifdef __cplusplus
extern "C" {
#endif

JNIEnv* GDAPI rebel_android_get_env();
jobject GDAPI rebel_android_get_activity();
jobject GDAPI rebel_android_get_surface();
bool GDAPI rebel_android_is_activity_resumed();

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_ANDROID_H
