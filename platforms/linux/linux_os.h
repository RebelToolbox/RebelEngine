// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LINUX_OS_H
#define LINUX_OS_H

#include "core/local_vector.h"
#include "core/os/input.h"
#include "drivers/alsa/audio_driver_alsa.h"
#include "drivers/alsamidi/midi_driver_alsamidi.h"
#include "drivers/pulseaudio/audio_driver_pulseaudio.h"
#include "drivers/unix/unix_os.h"
#include "linux_crash_handler.h"
#include "linux_gl_context.h"
#include "linux_joypad.h"
#include "linux_power.h"
#include "main/input_default.h"
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual_server.h"

#include <X11/Xcursor/Xcursor.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xrandr.h>
#include <X11/keysym.h>

// Hints for X11 fullscreen
typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long inputMode;
    unsigned long status;
} Hints;

typedef struct _xrr_monitor_info {
    Atom name;
    Bool primary;
    Bool automatic;
    int noutput;
    int x;
    int y;
    int width;
    int height;
    int mwidth;
    int mheight;
    RROutput* outputs;
} xrr_monitor_info;

#undef CursorShape

class LinuxOS : public UnixOS {
    Atom wm_delete;
    Atom xdnd_enter;
    Atom xdnd_position;
    Atom xdnd_status;
    Atom xdnd_action_copy;
    Atom xdnd_drop;
    Atom xdnd_finished;
    Atom xdnd_selection;
    Atom requested;

    int xdnd_version;

#if defined(OPENGL_ENABLED)
    LinuxGLContext* gl_context;
#endif
    // Rasterizer *rasterizer;
    VisualServer* visual_server;
    VideoMode current_videomode;
    List<String> args;
    Window x11_window;
    Window xdnd_source_window;
    MainLoop* main_loop;
    ::Display* x11_display;
    char* xmbstring;
    int xmblen;
    unsigned long last_timestamp;
    ::Time last_keyrelease_time;
    ::XIC xic;
    ::XIM xim;
    ::XIMStyle xim_style;
    static void xim_destroy_callback(
        ::XIM im,
        ::XPointer client_data,
        ::XPointer call_data
    );

    // IME
    bool im_active;
    Vector2 im_position;
    Vector2 last_position_before_fs;

    Size2 min_size;
    Size2 max_size;

    Point2 last_mouse_pos;
    bool last_mouse_pos_valid;
    Point2i last_click_pos;
    uint64_t last_click_ms;
    int last_click_button_index;
    uint32_t last_button_state;

    struct {
        int opcode;
        Vector<int> touch_devices;
        Map<int, Vector2> absolute_devices;
        Map<int, Vector2> pen_pressure_range;
        Map<int, Vector2> pen_tilt_x_range;
        Map<int, Vector2> pen_tilt_y_range;
        XIEventMask all_event_mask;
        XIEventMask all_master_event_mask;
        Map<int, Vector2> state;
        double pressure;
        bool pressure_supported;
        Vector2 tilt;
        Vector2 mouse_pos_to_filter;
        Vector2 relative_motion;
        Vector2 raw_pos;
        Vector2 old_raw_pos;
        ::Time last_relative_time;
    } xi;

    bool refresh_device_info();

    unsigned int get_mouse_button_state(
        unsigned int p_x11_button,
        int p_x11_type
    );
    void get_key_modifier_state(
        unsigned int p_x11_state,
        Ref<InputEventWithModifiers> state
    );
    void flush_mouse_motion();

    MouseMode mouse_mode;
    Point2i center;

    void _handle_key_event(
        XKeyEvent* p_event,
        LocalVector<XEvent>& p_events,
        uint32_t& p_event_index,
        bool p_echo = false
    );

    Atom _process_selection_request_target(
        Atom p_target,
        Window p_requestor,
        Atom p_property
    ) const;
    void _handle_selection_request_event(XSelectionRequestEvent* p_event) const;

    String _get_clipboard_impl(
        Atom p_source,
        Window x11_window,
        Atom target
    ) const;
    String _get_clipboard(Atom p_source, Window x11_window) const;
    void _clipboard_transfer_ownership(Atom p_source, Window x11_window) const;

    mutable Mutex events_mutex;
    Thread events_thread;
    bool events_thread_done = false;
    LocalVector<XEvent> polled_events;
    static void _poll_events_thread(void* ud);
    bool _wait_for_events() const;
    void _poll_events();
    void _check_pending_events(LocalVector<XEvent>& r_events);

    static Bool _predicate_all_events(
        Display* display,
        XEvent* event,
        XPointer arg
    );
    static Bool _predicate_clipboard_selection(
        Display* display,
        XEvent* event,
        XPointer arg
    );
    static Bool _predicate_clipboard_incr(
        Display* display,
        XEvent* event,
        XPointer arg
    );
    static Bool _predicate_clipboard_save_targets(
        Display* display,
        XEvent* event,
        XPointer arg
    );

    void process_xevents();
    void delete_main_loop() override;

    bool force_quit;
    bool minimized;
    bool window_has_focus;
    bool do_mouse_warp;

    const char* cursor_theme;
    int cursor_size;
    XcursorImage* img[CURSOR_MAX];
    Cursor cursors[CURSOR_MAX];
    Cursor null_cursor;
    CursorShape current_cursor;
    Map<CursorShape, Vector<Variant>> cursors_cache;

    InputDefault* input;

#ifdef JOYDEV_ENABLED
    LinuxJoypad* joypad;
#endif

#ifdef ALSA_ENABLED
    AudioDriverALSA driver_alsa;
#endif

#ifdef ALSAMIDI_ENABLED
    MIDIDriverALSAMidi driver_alsamidi;
#endif

#ifdef PULSEAUDIO_ENABLED
    AudioDriverPulseAudio driver_pulseaudio;
#endif

    LinuxPower* power_manager;

    bool layered_window;

    CrashHandler crash_handler;

    int video_driver_index;
    bool maximized;
    bool window_focused;
    // void set_wm_border(bool p_enabled);
    void set_wm_fullscreen(bool p_enabled);
    void set_wm_above(bool p_enabled);

    typedef xrr_monitor_info* (*xrr_get_monitors_t)(
        Display* dpy,
        Window window,
        Bool get_active,
        int* nmonitors
    );
    typedef void (*xrr_free_monitors_t)(xrr_monitor_info* monitors);
    xrr_get_monitors_t xrr_get_monitors;
    xrr_free_monitors_t xrr_free_monitors;
    void* xrandr_handle;
    Bool xrandr_ext_ok;

protected:
    int get_current_video_driver() const override;

    void initialize_core() override;
    Error initialize(
        const VideoMode& p_desired,
        int p_video_driver,
        int p_audio_driver
    ) override;
    void finalize() override;

    void set_main_loop(MainLoop* p_main_loop) override;

    void _window_changed(XEvent* event);

    bool window_maximize_check(const char* p_atom_name) const;
    bool window_fullscreen_check() const;
    bool is_window_maximize_allowed() const;

public:
    String get_name() const override;

    void set_cursor_shape(CursorShape p_shape) override;
    CursorShape get_cursor_shape() const override;
    void set_custom_mouse_cursor(
        const RES& p_cursor,
        CursorShape p_shape,
        const Vector2& p_hotspot
    ) override;

    void set_mouse_mode(MouseMode p_mode) override;
    MouseMode get_mouse_mode() const override;

    void warp_mouse_position(const Point2& p_to) override;
    Point2 get_mouse_position() const override;
    int get_mouse_button_state() const override;
    void set_window_title(const String& p_title) override;
    void set_window_mouse_passthrough(
        const PoolVector2Array& p_region
    ) override;

    void set_icon(const Ref<Image>& p_icon) override;

    MainLoop* get_main_loop() const override;

    bool can_draw() const override;

    void set_clipboard(const String& p_text) override;
    String get_clipboard() const override;

    void release_rendering_thread() override;
    void make_rendering_thread() override;
    void swap_buffers() override;

    String get_config_path() const override;
    String get_data_path() const override;
    String get_cache_path() const override;

    String get_system_dir(
        SystemDir p_dir,
        bool p_shared_storage = true
    ) const override;

    Error shell_open(String p_uri) override;

    void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    ) override;
    VideoMode get_video_mode(int p_screen = 0) const override;
    void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const override;

    int get_screen_count() const override;
    int get_current_screen() const override;
    void set_current_screen(int p_screen) override;
    Point2 get_screen_position(int p_screen = -1) const override;
    Size2 get_screen_size(int p_screen = -1) const override;
    int get_screen_dpi(int p_screen = -1) const override;
    Point2 get_window_position() const override;
    void set_window_position(const Point2& p_position) override;
    Size2 get_window_size() const override;
    Size2 get_real_window_size() const override;
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
    void* get_native_handle(int p_handle_type) override;

    void set_borderless_window(bool p_borderless) override;
    bool get_borderless_window() override;

    bool get_window_per_pixel_transparency_enabled() const override;
    void set_window_per_pixel_transparency_enabled(bool p_enabled) override;

    void set_ime_active(const bool p_active) override;
    void set_ime_position(const Point2& p_pos) override;

    String get_unique_id() const override;

    void move_window_to_foreground() override;
    void alert(
        const String& p_alert,
        const String& p_title = "ALERT!"
    ) override;

    bool is_joy_known(int p_device) override;
    String get_joy_guid(int p_device) const override;

    void set_context(int p_context) override;

    void _set_use_vsync(bool p_enable) override;

    OS::PowerState get_power_state() override;
    int get_power_seconds_left() override;
    int get_power_percent_left() override;

    bool _check_internal_feature_support(const String& p_feature) override;

    void force_process_input() override;
    void run();

    void disable_crash_handler() override;
    bool is_disable_crash_handler() const override;

    Error move_to_trash(const String& p_path) override;

    LatinKeyboardVariant get_latin_keyboard_variant() const override;
    int keyboard_get_layout_count() const override;
    int keyboard_get_current_layout() const override;
    void keyboard_set_current_layout(int p_index) override;
    String keyboard_get_layout_language(int p_index) const override;
    String keyboard_get_layout_name(int p_index) const override;

    void update_real_mouse_position();
    LinuxOS();
};

#endif // LINUX_OS_H
