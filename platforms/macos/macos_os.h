// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MacOSOS_H
#define MacOSOS_H

#define BitMap _QDBitMap // Suppress deprecated QuickDraw definition.

#include "core/os/input.h"
#include "drivers/coreaudio/audio_driver_coreaudio.h"
#include "drivers/coremidi/midi_driver_coremidi.h"
#include "drivers/unix/unix_os.h"
#include "macos_crash_handler.h"
#include "macos_joypad.h"
#include "macos_power.h"
#include "main/input_default.h"
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual/visual_server_wrap_mt.h"
#include "servers/visual_server.h"

#include <AppKit/AppKit.h>
#include <AppKit/NSCursor.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreVideo/CoreVideo.h>

#undef BitMap
#undef CursorShape

class MacOSOS : public UnixOS {
public:
    struct KeyEvent {
        unsigned int macos_state;
        bool pressed;
        bool echo;
        bool raw;
        uint32_t scancode;
        uint32_t physical_scancode;
        uint32_t unicode;
    };

    struct WarpEvent {
        NSTimeInterval timestamp;
        NSPoint delta;
    };

    List<WarpEvent> warp_events;
    NSTimeInterval last_warp = 0;
    bool ignore_warp         = false;

    Vector<KeyEvent> key_event_buffer;
    int key_event_pos;

    bool force_quit;
    // rasterizer seems to no longer be given to visual server, its using GLES3
    // directly?
    // Rasterizer *rasterizer;
    VisualServer* visual_server;

    List<String> args;
    MainLoop* main_loop;

#ifdef COREAUDIO_ENABLED
    AudioDriverCoreAudio audio_driver;
#endif
#ifdef COREMIDI_ENABLED
    MIDIDriverCoreMidi midi_driver;
#endif

    InputDefault* input;
    MacOSJoypad* macos_joypad;

    /* objc */

    CGEventSourceRef eventSource;

    void process_events();
    void process_key_events();

    void* framework;
    // pthread_key_t   current;
    bool mouse_grab;
    Point2 mouse_pos;

    id delegate;
    id window_delegate;
    id window_object;
    id window_view;
    id autoreleasePool;
    id cursor;
    NSOpenGLPixelFormat* pixelFormat;
    NSOpenGLContext* context;

    Vector<Vector2> mpath;
    bool layered_window;

    CursorShape cursor_shape;
    NSCursor* cursors[CURSOR_MAX];
    Map<CursorShape, Vector<Variant>> cursors_cache;
    MouseMode mouse_mode;

    String title;
    bool minimized;
    bool maximized;
    bool zoomed;
    bool resizable;
    bool window_focused;
    bool on_top;

    Size2 window_size;
    Rect2 restore_rect;

    String open_with_filename;

    Point2 im_position;
    bool im_active;
    String im_text;
    Point2 im_selection;

    Size2 min_size;
    Size2 max_size;

    MacOSPower* power_manager;

    CrashHandler crash_handler;

    void _update_window();

    int video_driver_index;
    int get_current_video_driver() const override;

    struct GlobalMenuItem {
        String label;
        Variant signal;
        Variant meta;

        GlobalMenuItem() {
            // NOP
        }

        GlobalMenuItem(
            const String& p_label,
            const Variant& p_signal,
            const Variant& p_meta
        ) {
            label  = p_label;
            signal = p_signal;
            meta   = p_meta;
        }
    };

    Map<String, Vector<GlobalMenuItem>> global_menus;
    List<String> global_menus_order;

    void _update_global_menu();

protected:
    void initialize_core() override;
    Error initialize(
        const VideoMode& p_desired,
        int p_video_driver,
        int p_audio_driver
    ) override;
    void finalize() override;

    void set_main_loop(MainLoop* p_main_loop) override;
    void delete_main_loop() override;

public:
    static MacOSOS* singleton;

    void global_menu_add_item(
        const String& p_menu,
        const String& p_label,
        const Variant& p_signal,
        const Variant& p_meta
    ) override;
    void global_menu_add_separator(const String& p_menu) override;
    void global_menu_remove_item(const String& p_menu, int p_idx) override;
    void global_menu_clear(const String& p_menu) override;

    void wm_minimized(bool p_minimized);

    String get_name() const override;

    void alert(
        const String& p_alert,
        const String& p_title = "ALERT!"
    ) override;

    Error open_dynamic_library(
        const String p_path,
        void*& p_library_handle,
        bool p_also_set_library_path = false
    ) override;

    void set_cursor_shape(CursorShape p_shape) override;
    CursorShape get_cursor_shape() const override;
    void set_custom_mouse_cursor(
        const RES& p_cursor,
        CursorShape p_shape,
        const Vector2& p_hotspot
    ) override;

    void set_mouse_show(bool p_show);
    void set_mouse_grab(bool p_grab);
    bool is_mouse_grab_enabled() const;
    void warp_mouse_position(const Point2& p_to) override;
    Point2 get_mouse_position() const override;
    int get_mouse_button_state() const override;
    void update_real_mouse_position();
    void set_window_title(const String& p_title) override;
    void set_window_mouse_passthrough(
        const PoolVector2Array& p_region
    ) override;

    Size2 get_window_size() const override;
    Size2 get_real_window_size() const override;

    void set_native_icon(const String& p_filename) override;
    void set_icon(const Ref<Image>& p_icon) override;

    MainLoop* get_main_loop() const override;

    String get_config_path() const override;
    String get_data_path() const override;
    String get_cache_path() const override;
    String get_bundle_resource_dir() const override;
    String get_bundle_icon_path() const override;
    String get_rebel_dir_name() const override;

    String get_system_dir(
        SystemDir p_dir,
        bool p_shared_storage = true
    ) const override;

    bool can_draw() const override;

    void set_clipboard(const String& p_text) override;
    String get_clipboard() const override;

    void release_rendering_thread() override;
    void make_rendering_thread() override;
    void swap_buffers() override;

    Error shell_open(String p_uri) override;
    void push_input(const Ref<InputEvent>& p_event);

    String get_locale() const override;

    void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    ) override;
    VideoMode get_video_mode(int p_screen = 0) const override;
    void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const override;

    String get_executable_path() const override;
    Error execute(
        const String& p_path,
        const List<String>& p_arguments,
        bool p_blocking       = true,
        ProcessID* r_child_id = nullptr,
        String* r_pipe        = nullptr,
        int* r_exitcode       = nullptr,
        bool read_stderr      = false,
        Mutex* p_pipe_mutex   = nullptr
    ) override;

    LatinKeyboardVariant get_latin_keyboard_variant() const override;
    int keyboard_get_layout_count() const override;
    int keyboard_get_current_layout() const override;
    void keyboard_set_current_layout(int p_index) override;
    String keyboard_get_layout_language(int p_index) const override;
    String keyboard_get_layout_name(int p_index) const override;

    void move_window_to_foreground() override;

    int get_screen_count() const override;
    int get_current_screen() const override;
    void set_current_screen(int p_screen) override;
    Point2 get_screen_position(int p_screen = -1) const override;
    Size2 get_screen_size(int p_screen = -1) const override;
    int get_screen_dpi(int p_screen = -1) const override;
    float get_screen_scale(int p_screen = -1) const override;
    float get_screen_max_scale() const override;

    Point2 get_window_position() const override;
    void set_window_position(const Point2& p_position) override;
    Size2 get_max_window_size() const override;
    Size2 get_min_window_size() const override;
    void set_min_window_size(const Size2 p_size) override;
    void set_max_window_size(const Size2 p_size) override;
    void set_window_size(const Size2 p_size) override;
    void set_window_fullscreen(bool p_enabled) override;
    bool is_window_fullscreen() const override;
    void set_window_resizable(bool p_enabled) override;
    bool is_window_resizable() const override;
    void set_window_minimized(bool p_enabled) override;
    bool is_window_minimized() const override;
    void set_window_maximized(bool p_enabled) override;
    bool is_window_maximized() const override;
    void set_window_always_on_top(bool p_enabled) override;
    bool is_window_always_on_top() const override;
    bool is_window_focused() const override;
    void request_attention() override;
    String get_joy_guid(int p_device) const override;

    void set_borderless_window(bool p_borderless) override;
    bool get_borderless_window() override;

    bool get_window_per_pixel_transparency_enabled() const override;
    void set_window_per_pixel_transparency_enabled(bool p_enabled) override;

    void set_ime_active(const bool p_active) override;
    void set_ime_position(const Point2& p_pos) override;
    Point2 get_ime_selection() const override;
    String get_ime_text() const override;

    String get_unique_id() const override;

    OS::PowerState get_power_state() override;
    int get_power_seconds_left() override;
    int get_power_percent_left() override;

    bool _check_internal_feature_support(const String& p_feature) override;

    void _set_use_vsync(bool p_enable) override;

    void run();

    void set_mouse_mode(MouseMode p_mode) override;
    MouseMode get_mouse_mode() const override;

    void disable_crash_handler() override;
    bool is_disable_crash_handler() const override;

    Error move_to_trash(const String& p_path) override;

    void force_process_input() override;

    MacOSOS();

private:
    Point2 get_native_screen_position(int p_screen) const;
    Point2 get_native_window_position() const;
    void set_native_window_position(const Point2& p_position);
    Point2 get_screens_origin() const;
};

#endif
