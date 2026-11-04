// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_OS_H
#define ANDROID_OS_H

#include "android_audio_driver.h"
#include "core/os/main_loop.h"
#include "drivers/unix/unix_os.h"
#include "main/input_default.h"
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"

class AndroidJNIOS;
class AndroidJNIIO;

class AndroidOS : public UnixOS {
    bool use_gl2;
    bool use_apk_expansion;

    bool use_16bits_fbo;

    VisualServer* visual_server;

    mutable String data_dir_cache;

    AndroidAudioDriver android_audio_driver;

    const char* gl_extensions;

    InputDefault* input;
    VideoMode default_videomode;
    MainLoop* main_loop;

    AndroidJNIOS* android_jni_os;
    AndroidJNIIO* android_jni_io;

    int video_driver_index;

    bool transparency_enabled = false;

public:
    // functions used by main to initialize/deinitialize the OS
    int get_video_driver_count() const override;
    const char* get_video_driver_name(int p_driver) const override;

    int get_audio_driver_count() const override;
    const char* get_audio_driver_name(int p_driver) const override;

    int get_current_video_driver() const override;

    void initialize_core() override;
    Error initialize(
        const VideoMode& p_desired,
        int p_video_driver,
        int p_audio_driver
    ) override;

    void set_main_loop(MainLoop* p_main_loop) override;
    void delete_main_loop() override;

    void finalize() override;

    typedef int64_t ProcessID;

    static OS* get_singleton();
    AndroidJNIOS* get_android_jni_os();
    AndroidJNIIO* get_android_jni_io();

    void alert(
        const String& p_alert,
        const String& p_title = "ALERT!"
    ) override;
    bool request_permission(const String& p_name) override;
    bool request_permissions() override;
    Vector<String> get_granted_permissions() const override;

    Error open_dynamic_library(
        const String p_path,
        void*& p_library_handle,
        bool p_also_set_library_path = false
    ) override;

    void set_mouse_show(bool p_show);
    void set_mouse_grab(bool p_grab);
    bool is_mouse_grab_enabled() const;
    Point2 get_mouse_position() const override;
    int get_mouse_button_state() const override;
    void set_window_title(const String& p_title) override;

    void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    ) override;
    VideoMode get_video_mode(int p_screen = 0) const override;
    void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const override;

    void set_keep_screen_on(bool p_enabled) override;

    Size2 get_window_size() const override;
    Rect2 get_window_safe_area() const override;

    String get_name() const override;
    MainLoop* get_main_loop() const override;

    bool can_draw() const override;

    void main_loop_begin();
    bool main_loop_iterate();
    void main_loop_end();
    void main_loop_focusout();
    void main_loop_focusin();

    bool has_touchscreen_ui_hint() const override;

    bool has_virtual_keyboard() const override;
    void show_virtual_keyboard(
        const String& p_existing_text,
        const Rect2& p_screen_rect = Rect2(),
        bool p_multiline           = false,
        int p_max_input_length     = -1,
        int p_cursor_start         = -1,
        int p_cursor_end           = -1
    ) override;
    void hide_virtual_keyboard() override;
    int get_virtual_keyboard_height() const override;

    void set_opengl_extensions(const char* p_gl_extensions);
    void set_display_size(Size2 p_size);

    void set_context_is_16_bits(bool p_is_16);

    void set_screen_orientation(ScreenOrientation p_orientation) override;
    ScreenOrientation get_screen_orientation() const override;

    Error shell_open(String p_uri) override;
    String get_user_data_dir() const override;
    String get_data_path() const override;
    String get_cache_path() const override;
    String get_resource_dir() const override;
    String get_locale() const override;
    void set_clipboard(const String& p_text) override;
    String get_clipboard() const override;
    String get_model_name() const override;
    int get_screen_dpi(int p_screen = 0) const override;

    bool get_window_per_pixel_transparency_enabled() const override {
        return transparency_enabled;
    }

    void set_window_per_pixel_transparency_enabled(bool p_enabled) override {
        ERR_FAIL_MSG(
            "Setting per-pixel transparency is not supported at runtime, "
            "please set it in project settings instead."
        );
    }

    String get_unique_id() const override;

    String get_system_dir(
        SystemDir p_dir,
        bool p_shared_storage = true
    ) const override;

    void process_accelerometer(const Vector3& p_accelerometer);
    void process_gravity(const Vector3& p_gravity);
    void process_magnetometer(const Vector3& p_magnetometer);
    void process_gyroscope(const Vector3& p_gyroscope);
    void init_video_mode(int p_video_width, int p_video_height);

    bool is_joy_known(int p_device) override;
    String get_joy_guid(int p_device) const override;
    void vibrate_handheld(int p_duration_ms) override;

    bool _check_internal_feature_support(const String& p_feature) override;
    AndroidOS(
        AndroidJNIOS* p_android_jni_os,
        AndroidJNIIO* p_android_jni_io,
        bool p_use_apk_expansion
    );
    ~AndroidOS() override;
};

#endif // ANDROID_OS_H
