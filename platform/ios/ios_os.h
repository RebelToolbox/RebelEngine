// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IOS_OS_H
#define IOS_OS_H

#ifdef IOS_ENABLED

#include "core/os/input.h"
#include "drivers/coreaudio/audio_driver_coreaudio.h"
#include "drivers/unix/unix_os.h"
#include "ios.h"
#include "ios_joypad.h"
#include "main/input_default.h"
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual_server.h"

class IosOS : public UnixOS {
private:
    static HashMap<String, void*> dynamic_symbol_lookup_table;
    friend void register_dynamic_symbol(char* name, void* address);

    VisualServer* visual_server;

    AudioDriverCoreAudio audio_driver;

    iOS* ios;

    IosJoypad* ios_joypad;

    MainLoop* main_loop;

    VideoMode video_mode;

    virtual int get_video_driver_count() const;
    virtual const char* get_video_driver_name(int p_driver) const;

    virtual int get_current_video_driver() const;

    virtual void initialize_core();
    virtual Error initialize(
        const VideoMode& p_desired,
        int p_video_driver,
        int p_audio_driver
    );

    virtual void set_main_loop(MainLoop* p_main_loop);
    virtual MainLoop* get_main_loop() const;

    virtual void delete_main_loop();

    virtual void finalize();

    void perform_event(const Ref<InputEvent>& p_event);

    void set_data_dir(String p_dir);

    String data_dir;

    InputDefault* input;

    int virtual_keyboard_height = 0;

    int video_driver_index;

    bool is_focused = false;

public:
    static IosOS* get_singleton();

    IosOS(String p_data_dir);
    ~IosOS();

    bool iterate();

    void start();

    virtual Error open_dynamic_library(
        const String p_path,
        void*& p_library_handle,
        bool p_also_set_library_path = false
    );
    virtual Error close_dynamic_library(void* p_library_handle);
    virtual Error get_dynamic_library_symbol_handle(
        void* p_library_handle,
        const String p_name,
        void*& p_symbol_handle,
        bool p_optional = false
    );

    virtual void alert(const String& p_alert, const String& p_title = "ALERT!");

    virtual String get_name() const;
    virtual String get_model_name() const;

    virtual void set_clipboard(const String& p_text);
    virtual String get_clipboard() const;

    Error shell_open(String p_uri);

    String get_user_data_dir() const;

    String get_locale() const;

    String get_unique_id() const;

    virtual void vibrate_handheld(int p_duration_ms = 500);

    virtual bool _check_internal_feature_support(const String& p_feature);

    virtual int get_screen_dpi(int p_screen = -1) const;

    void pencil_press(
        int p_idx,
        int p_x,
        int p_y,
        bool p_pressed,
        bool p_doubleclick
    );
    void touch_press(
        int p_idx,
        int p_x,
        int p_y,
        bool p_pressed,
        bool p_doubleclick
    );
    void pencil_drag(
        int p_idx,
        int p_prev_x,
        int p_prev_y,
        int p_x,
        int p_y,
        float p_force
    );
    void touch_drag(int p_idx, int p_prev_x, int p_prev_y, int p_x, int p_y);
    void touches_cancelled(int p_idx);
    void pencil_cancelled(int p_idx);
    void key(uint32_t p_key, bool p_pressed);
    void set_virtual_keyboard_height(int p_height);

    int set_base_framebuffer(int p_fb);

    void update_gravity(float p_x, float p_y, float p_z);
    void update_accelerometer(float p_x, float p_y, float p_z);
    void update_magnetometer(float p_x, float p_y, float p_z);
    void update_gyroscope(float p_x, float p_y, float p_z);

    int get_unused_joy_id();
    void joy_connection_changed(int p_idx, bool p_connected, String p_name);
    void joy_button(int p_device, int p_button, bool p_pressed);
    void joy_axis(
        int p_device,
        int p_axis,
        const InputDefault::JoyAxis& p_value
    );

    virtual void set_mouse_show(bool p_show);
    virtual void set_mouse_grab(bool p_grab);
    virtual bool is_mouse_grab_enabled() const;
    virtual Point2 get_mouse_position() const;
    virtual int get_mouse_button_state() const;

    virtual void set_window_title(const String& p_title);

    virtual void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    );
    virtual VideoMode get_video_mode(int p_screen = 0) const;

    virtual void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const;

    virtual void set_keep_screen_on(bool p_enabled);

    virtual bool can_draw() const;

    virtual bool has_virtual_keyboard() const;
    virtual void show_virtual_keyboard(
        const String& p_existing_text,
        const Rect2& p_screen_rect = Rect2(),
        bool p_multiline           = false,
        int p_max_input_length     = -1,
        int p_cursor_start         = -1,
        int p_cursor_end           = -1
    );
    virtual void hide_virtual_keyboard();
    virtual int get_virtual_keyboard_height() const;

    virtual Size2 get_window_size() const;
    virtual Rect2 get_window_safe_area() const;

    virtual bool has_touchscreen_ui_hint() const;

    virtual Error native_video_play(
        String p_path,
        float p_volume,
        String p_audio_track,
        String p_subtitle_track
    );
    virtual bool native_video_is_playing() const;
    virtual void native_video_pause();
    virtual void native_video_unpause();
    virtual void native_video_focus_out();
    virtual void native_video_stop();

    void on_focus_out();
    void on_focus_in();
};

#endif // IOS_ENABLED

#endif // IOS_OS_H
