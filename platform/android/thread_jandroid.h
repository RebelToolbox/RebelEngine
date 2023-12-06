// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef THREAD_JANDROID_H
#define THREAD_JANDROID_H

#include <jni.h>

void init_thread_jandroid(JavaVM* p_jvm, JNIEnv* p_env);

void setup_android_thread();
JNIEnv* get_jni_env();

#endif
