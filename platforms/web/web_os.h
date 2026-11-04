// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEB_OS_H
#define WEB_OS_H

#include "drivers/unix/unix_os.h"
#include "main/input_default.h"
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"
#include "web_audio_driver.h"

#include <emscripten/html5.h>

class WebOS : public UnixOS {
private:
    struct JSTouchEvent {
        uint32_t identifier[32] = {0};
        double coords[64]       = {0};
    };

    JSTouchEvent touch_event;

    struct JSKeyEvent {
        char code[32]        = {0};
        char key[32]         = {0};
        uint8_t modifiers[4] = {0};
    };

    JSKeyEvent key_event;

    VideoMode video_mode;
    bool transparency_enabled;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE webgl_ctx;

    InputDefault* input;
    CursorShape cursor_shape;
    Point2 touches[32];

    char canvas_id[256];
    bool cursor_inside_canvas;
    Point2i last_click_pos;
    uint64_t last_click_ms;
    int last_click_button_index;

    MainLoop* main_loop;
    int video_driver_index;
    List<WebAudioDriver*> audio_drivers;
    VisualServer* visual_server;

    bool swap_ok_cancel;
    bool idb_available;
    bool idb_needs_sync;
    bool idb_is_syncing;

    static void fullscreen_change_callback(int p_fullscreen);
    static int mouse_button_callback(
        int p_pressed,
        int p_button,
        double p_x,
        double p_y,
        int p_modifiers
    );
    static void mouse_move_callback(
        double p_x,
        double p_y,
        double p_rel_x,
        double p_rel_y,
        int p_modifiers
    );
    static int mouse_wheel_callback(double p_delta_x, double p_delta_y);
    static void key_callback(int p_pressed, int p_repeat, int p_modifiers);
    static void touch_callback(int p_type, int p_count);

    static void gamepad_callback(
        int p_index,
        int p_connected,
        const char* p_id,
        const char* p_guid
    );
    static void input_text_callback(const char* p_text, int p_cursor);
    void process_joypads();

    static void file_access_close_callback(const String& p_file, int p_flags);

    static void request_quit_callback();
    static void window_blur_callback();
    static void drop_files_callback(char** p_filev, int p_filec);
    static void send_notification_callback(int p_notification);
    static void fs_sync_callback();
    static void update_clipboard_callback(const char* p_text);

protected:
    void resume_audio();

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

    bool _check_internal_feature_support(const String& p_feature) override;

public:
    bool check_size_force_redraw();

    // Override return type to make writing static callbacks less tedious.
    static WebOS* get_singleton();

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

    bool get_swap_ok_cancel() override;
    void swap_buffers() override;
    void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    ) override;
    VideoMode get_video_mode(int p_screen = 0) const override;
    void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const override;

    void set_window_size(const Size2) override;
    Size2 get_window_size() const override;
    void set_window_maximized(bool p_enabled) override;
    bool is_window_maximized() const override;
    void set_window_fullscreen(bool p_enabled) override;
    bool is_window_fullscreen() const override;
    Size2 get_screen_size(int p_screen = -1) const override;
    int get_screen_dpi(int p_screen = -1) const override;
    float get_screen_scale(int p_screen = -1) const override;
    float get_screen_max_scale() const override;

    Point2 get_mouse_position() const override;
    int get_mouse_button_state() const override;
    void set_cursor_shape(CursorShape p_shape) override;
    void set_custom_mouse_cursor(
        const RES& p_cursor,
        CursorShape p_shape,
        const Vector2& p_hotspot
    ) override;
    void set_mouse_mode(MouseMode p_mode) override;
    MouseMode get_mouse_mode() const override;

    bool get_window_per_pixel_transparency_enabled() const override;
    void set_window_per_pixel_transparency_enabled(bool p_enabled) override;

    bool has_touchscreen_ui_hint() const override;

    bool is_joy_known(int p_device) override;
    String get_joy_guid(int p_device) const override;

    int get_video_driver_count() const override;
    const char* get_video_driver_name(int p_driver) const override;

    int get_audio_driver_count() const override;
    const char* get_audio_driver_name(int p_driver) const override;

    void set_clipboard(const String& p_text) override;
    String get_clipboard() const override;

    MainLoop* get_main_loop() const override;
    bool main_loop_iterate();

    Error execute(
        const String& p_path,
        const List<String>& p_arguments,
        bool p_blocking       = true,
        ProcessID* r_child_id = NULL,
        String* r_pipe        = NULL,
        int* r_exitcode       = NULL,
        bool read_stderr      = false,
        Mutex* p_pipe_mutex   = NULL
    ) override;
    Error kill(const ProcessID& p_pid) override;
    int get_process_id() const override;
    int get_processor_count() const override;

    void alert(
        const String& p_alert,
        const String& p_title = "ALERT!"
    ) override;
    void set_window_title(const String& p_title) override;
    void set_icon(const Ref<Image>& p_icon) override;
    String get_executable_path() const override;
    Error shell_open(String p_uri) override;
    String get_name() const override;

    void add_frame_delay(bool p_can_draw) override {}

    bool can_draw() const override;

    String get_cache_path() const override;
    String get_config_path() const override;
    String get_data_path() const override;
    String get_user_data_dir() const override;

    OS::PowerState get_power_state() override;
    int get_power_seconds_left() override;
    int get_power_percent_left() override;

    bool is_userfs_persistent() const override;
    Error open_dynamic_library(
        const String p_path,
        void*& p_library_handle,
        bool p_also_set_library_path
    ) override;
    WebOS();
};

#endif // WEB_OS_H
