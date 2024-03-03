// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_JNI_OS_H
#define ANDROID_JNI_OS_H

#include "string_android.h"

#include <android/log.h>
#include <jni.h>

class AndroidJNIOS {
private:
    jobject activity;
    jobject fragment;

    jclass activity_class;
    jclass fragment_class;

    jmethodID _on_video_init              = 0;
    jmethodID _restart                    = 0;
    jmethodID _finish                     = 0;
    jmethodID _set_keep_screen_on         = 0;
    jmethodID _alert                      = 0;
    jmethodID _get_GLES_version_code      = 0;
    jmethodID _get_clipboard              = 0;
    jmethodID _set_clipboard              = 0;
    jmethodID _request_permission         = 0;
    jmethodID _request_permissions        = 0;
    jmethodID _get_granted_permissions    = 0;
    jmethodID _init_input_devices         = 0;
    jmethodID _get_surface                = 0;
    jmethodID _is_activity_resumed        = 0;
    jmethodID _vibrate                    = 0;
    jmethodID _get_input_fallback_mapping = 0;
    jmethodID _on_setup_completed         = 0;
    jmethodID _on_main_loop_started       = 0;
    jmethodID _get_class_loader           = 0;

public:
    AndroidJNIOS(JNIEnv* p_env, jobject p_activity, jobject p_fragment);
    ~AndroidJNIOS();

    jobject get_activity();
    jobject get_member_object(
        const char* p_name,
        const char* p_class,
        JNIEnv* p_env = NULL
    );

    jobject get_class_loader();

    void gfx_init(bool gl2);
    void on_video_init(JNIEnv* p_env = NULL);
    void on_setup_completed(JNIEnv* p_env = NULL);
    void on_main_loop_started(JNIEnv* p_env = NULL);
    void restart(JNIEnv* p_env = NULL);
    void force_quit(JNIEnv* p_env = NULL);
    void set_keep_screen_on(bool p_enabled);
    void alert(const String& p_message, const String& p_title);
    int get_gles_version_code();
    bool has_get_clipboard();
    String get_clipboard();
    bool has_set_clipboard();
    void set_clipboard(const String& p_text);
    bool request_permission(const String& p_name);
    bool request_permissions();
    Vector<String> get_granted_permissions() const;
    void init_input_devices();
    jobject get_surface();
    bool is_activity_resumed();
    void vibrate(int p_duration_ms);
    String get_input_fallback_mapping();
};

#endif // ANDROID_JNI_OS_H
