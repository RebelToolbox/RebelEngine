// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef UWP_OS_H
#define UWP_OS_H

#include "core/math/transform_2d.h"
#include "core/os/input.h"
#include "core/os/os.h"
#include "core/ustring.h"
#include "drivers/xaudio2/audio_driver_xaudio2.h"
#include "main/input_default.h"
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual_server.h"
#include "uwp_egl_context.h"
#include "uwp_joypad.h"
#include "uwp_power.h"

#include <fcntl.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// Windows system includes come after <windows.h>
#include <io.h>

class UwpOS : public OS {
public:
    struct KeyEvent {
        enum MessageType {
            KEY_EVENT_MESSAGE,
            CHAR_EVENT_MESSAGE
        };

        bool alt, shift, control;
        MessageType type;
        bool pressed;
        unsigned int scancode;
        unsigned int physical_scancode;
        unsigned int unicode;
        bool echo;
        CorePhysicalKeyStatus status;
    };

private:
    enum {
        JOYPADS_MAX           = 8,
        JOY_AXIS_COUNT        = 6,
        MAX_JOY_AXIS          = 32768, // I've no idea
        KEY_EVENT_BUFFER_SIZE = 512
    };

    FILE* stdo;

    KeyEvent key_event_buffer[KEY_EVENT_BUFFER_SIZE];
    int key_event_pos;

    uint64_t ticks_start;
    uint64_t ticks_per_second;

    bool minimized;
    bool old_invalid;
    bool outside;
    int old_x, old_y;
    Point2i center;
    VisualServer* visual_server;
    int pressrc;

    ContextEGL_UWP* gl_context;
    Windows::UI::Core::CoreWindow ^ window;

    VideoMode video_mode;
    int video_driver_index;

    MainLoop* main_loop;

    AudioDriverXAudio2 audio_driver;

    PowerUWP* power_manager;

    MouseMode mouse_mode;
    bool alt_mem;
    bool gr_mem;
    bool shift_mem;
    bool control_mem;
    bool meta_mem;
    bool force_quit;
    uint32_t last_button_state;

    CursorShape cursor_shape;

    InputDefault* input;

    JoypadUWP ^ joypad;

    Windows::System::Display::DisplayRequest ^ display_request;

    void _post_dpad(DWORD p_dpad, int p_device, bool p_pressed);

    void _drag_event(int idx, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void _touch_event(int idx, UINT uMsg, WPARAM wParam, LPARAM lParam);

    ref class ManagedType {
    public:
        property bool alert_close_handle;
        property Platform::String ^ clipboard;
        void alert_close(Windows::UI::Popups::IUICommand ^ command);
        void on_clipboard_changed(
            Platform::Object ^ sender,
            Platform::Object ^ ev
        );
        void update_clipboard();
        void on_accelerometer_reading_changed(
            Windows::Devices::Sensors::Accelerometer ^ sender,
            Windows::Devices::Sensors::AccelerometerReadingChangedEventArgs
                ^ args
        );
        void on_magnetometer_reading_changed(
            Windows::Devices::Sensors::Magnetometer ^ sender,
            Windows::Devices::Sensors::MagnetometerReadingChangedEventArgs
                ^ args
        );
        void on_gyroscope_reading_changed(
            Windows::Devices::Sensors::Gyrometer ^ sender,
            Windows::Devices::Sensors::GyrometerReadingChangedEventArgs ^ args
        );

        /** clang-format breaks this, it does not understand this token. */
        /* clang-format off */
	internal:
		ManagedType() { alert_close_handle = false; }
		property UwpOS* os;
        /* clang-format on */
    };

    ManagedType ^ managed_object;
    Windows::Devices::Sensors::Accelerometer ^ accelerometer;
    Windows::Devices::Sensors::Magnetometer ^ magnetometer;
    Windows::Devices::Sensors::Gyrometer ^ gyrometer;

    // functions used by main to initialize/deinitialize the OS

protected:
    int get_video_driver_count() const override;
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
    void finalize_core() override;

    void process_events();

    void process_key_events();

public:
    // Event to send to the app wrapper
    HANDLE mouse_mode_changed;

    void alert(
        const String& p_alert,
        const String& p_title = "ALERT!"
    ) override;
    String get_stdin_string(bool p_block);

    void set_mouse_mode(MouseMode p_mode);
    MouseMode get_mouse_mode() const;

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
    Size2 get_window_size() const override;
    void set_window_size(const Size2 p_size) override;
    void set_window_fullscreen(bool p_enabled) override;
    bool is_window_fullscreen() const override;
    void set_keep_screen_on(bool p_enabled) override;

    MainLoop* get_main_loop() const override;

    String get_name() const override;

    Date get_date(bool utc) const override;
    Time get_time(bool utc) const override;
    TimeZoneInfo get_time_zone_info() const override;
    uint64_t get_unix_time() const override;

    bool can_draw() const override;
    Error set_cwd(const String& p_cwd) override;

    void delay_usec(uint32_t p_usec) const override;
    uint64_t get_ticks_usec() const override;

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

    bool has_environment(const String& p_var) const override;
    String get_environment(const String& p_var) const override;
    bool set_environment(
        const String& p_var,
        const String& p_value
    ) const override;

    void set_clipboard(const String& p_text) override;
    String get_clipboard() const override;

    void set_cursor_shape(CursorShape p_shape);
    CursorShape get_cursor_shape() const;
    void set_custom_mouse_cursor(
        const RES& p_cursor,
        CursorShape p_shape,
        const Vector2& p_hotspot
    ) override;
    void set_icon(const Ref<Image>& p_icon);

    String get_executable_path() const override;

    String get_locale() const override;

    void move_window_to_foreground() override;
    String get_user_data_dir() const override;

    bool _check_internal_feature_support(const String& p_feature) override;

    void set_window(Windows::UI::Core::CoreWindow ^ p_window);
    void screen_size_changed();

    void release_rendering_thread() override;
    void make_rendering_thread() override;
    void swap_buffers() override;

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

    Error open_dynamic_library(
        const String p_path,
        void*& p_library_handle,
        bool p_also_set_library_path = false
    ) override;
    Error close_dynamic_library(void* p_library_handle) override;
    Error get_dynamic_library_symbol_handle(
        void* p_library_handle,
        const String p_name,
        void*& p_symbol_handle,
        bool p_optional = false
    ) override;

    Error shell_open(String p_uri) override;

    void run();

    bool get_swap_ok_cancel() override {
        return true;
    }

    void input_event(const Ref<InputEvent>& p_event);

    OS::PowerState get_power_state() override;
    int get_power_seconds_left() override;
    int get_power_percent_left() override;

    void queue_key_event(KeyEvent& p_event);

    UwpOS();
    ~UwpOS() override;
};

#endif // UWP_OS_H
