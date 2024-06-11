// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_JNI_THREAD_H
#define ANDROID_JNI_THREAD_H

#include <jni.h>

void initialize_android_jni_thread(JavaVM* p_jvm, JNIEnv* p_env);

void setup_android_thread();
JNIEnv* get_jni_env();

#endif // ANDROID_JNI_THREAD_H
