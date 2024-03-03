// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_JNI_PLUGIN_H
#define ANDROID_JNI_PLUGIN_H

#include <android/log.h>
#include <jni.h>

extern "C" {
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterSingleton(
    JNIEnv* env,
    jclass clazz,
    jstring name,
    jobject obj
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterMethod(
    JNIEnv* env,
    jclass clazz,
    jstring sname,
    jstring name,
    jstring ret,
    jobjectArray args
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterSignal(
    JNIEnv* env,
    jclass clazz,
    jstring j_plugin_name,
    jstring j_signal_name,
    jobjectArray j_signal_param_types
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeEmitSignal(
    JNIEnv* env,
    jclass clazz,
    jstring j_plugin_name,
    jstring j_signal_name,
    jobjectArray j_signal_params
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterGDNativeLibraries(
    JNIEnv* env,
    jclass clazz,
    jobjectArray gdnlib_paths
);
}

#endif // ANDROID_JNI_PLUGIN_H
