// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_JNI_IO_H
#define ANDROID_JNI_IO_H

#include "string_android.h"

#include <android/log.h>
#include <jni.h>

class AndroidJNIIO {
private:
    jobject io_object;
    jclass io_class;

    jmethodID _open_URI               = 0;
    jmethodID _get_cache_dir          = 0;
    jmethodID _get_data_dir           = 0;
    jmethodID _get_locale             = 0;
    jmethodID _get_model              = 0;
    jmethodID _get_screen_DPI         = 0;
    jmethodID _get_window_safe_area   = 0;
    jmethodID _get_unique_id          = 0;
    jmethodID _show_keyboard          = 0;
    jmethodID _hide_keyboard          = 0;
    jmethodID _set_screen_orientation = 0;
    jmethodID _get_screen_orientation = 0;
    jmethodID _get_system_dir         = 0;

public:
    AndroidJNIIO(JNIEnv* p_env, jobject io_object);
    ~AndroidJNIIO();

    jobject get_instance();

    Error open_uri(const String& p_uri);
    String get_cache_dir();
    String get_user_data_dir();
    String get_locale();
    String get_model();
    int get_screen_dpi();
    void get_window_safe_area(int (&p_rect_xywh)[4]);
    String get_unique_id();
    bool has_vk();
    void show_vk(
        const String& p_existing,
        bool p_multiline,
        int p_max_input_length,
        int p_cursor_start,
        int p_cursor_end
    );
    void hide_vk();
    int get_vk_height();
    void set_vk_height(int p_height);
    void set_screen_orientation(int p_orient);
    int get_screen_orientation() const;
    String get_system_dir(int p_dir, bool p_shared_storage);
};

#endif // ANDROID_JNI_IO_H
