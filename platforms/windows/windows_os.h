// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WINDOWS_OS_H
#define WINDOWS_OS_H

#include "core/os/input.h"
#include "core/os/os.h"
#include "core/project_settings.h"
#include "drivers/network/default_ip.h"
#include "drivers/winmidi/midi_driver_winmidi.h"
#include "main/input_default.h"
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual_server.h"
#include "windows_audio_driver.h"
#include "windows_crash_handler.h"
#include "windows_key_mapping.h"
#include "windows_power.h"

#ifdef OPENGL_ENABLED
#include "windows_gl_context.h"
#endif // OPENGL_ENABLED

#ifdef XAUDIO2_ENABLED
#include "drivers/xaudio2/audio_driver_xaudio2.h"
#endif

#include <fcntl.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// Windows system includes come after <windows.h>
#include <dwmapi.h>
#include <io.h>
#include <shellapi.h>
#include <windowsx.h>

// WinTab API
#define WT_PACKET     0x7FF0
#define WT_PROXIMITY  0x7FF5
#define WT_INFOCHANGE 0x7FF6
#define WT_CSRCHANGE  0x7FF7

#define WTI_DEFSYSCTX   4
#define WTI_DEVICES     100
#define DVC_NPRESSURE   15
#define DVC_TPRESSURE   16
#define DVC_ORIENTATION 17
#define DVC_ROTATION    18

#define CXO_MESSAGES        0x0004
#define PK_NORMAL_PRESSURE  0x0400
#define PK_TANGENT_PRESSURE 0x0800
#define PK_ORIENTATION      0x1000

typedef struct tagLOGCONTEXTW {
    WCHAR lcName[40];
    UINT lcOptions;
    UINT lcStatus;
    UINT lcLocks;
    UINT lcMsgBase;
    UINT lcDevice;
    UINT lcPktRate;
    DWORD lcPktData;
    DWORD lcPktMode;
    DWORD lcMoveMask;
    DWORD lcBtnDnMask;
    DWORD lcBtnUpMask;
    LONG lcInOrgX;
    LONG lcInOrgY;
    LONG lcInOrgZ;
    LONG lcInExtX;
    LONG lcInExtY;
    LONG lcInExtZ;
    LONG lcOutOrgX;
    LONG lcOutOrgY;
    LONG lcOutOrgZ;
    LONG lcOutExtX;
    LONG lcOutExtY;
    LONG lcOutExtZ;
    DWORD lcSensX;
    DWORD lcSensY;
    DWORD lcSensZ;
    BOOL lcSysMode;
    int lcSysOrgX;
    int lcSysOrgY;
    int lcSysExtX;
    int lcSysExtY;
    DWORD lcSysSensX;
    DWORD lcSysSensY;
} LOGCONTEXTW;

typedef struct tagAXIS {
    LONG axMin;
    LONG axMax;
    UINT axUnits;
    DWORD axResolution;
} AXIS;

typedef struct tagORIENTATION {
    int orAzimuth;
    int orAltitude;
    int orTwist;
} ORIENTATION;

typedef struct tagPACKET {
    int pkNormalPressure;
    int pkTangentPressure;
    ORIENTATION pkOrientation;
} PACKET;

typedef HANDLE(WINAPI* WTOpenPtr)(
    HWND p_window,
    LOGCONTEXTW* p_ctx,
    BOOL p_enable
);
typedef BOOL(WINAPI* WTClosePtr)(HANDLE p_ctx);
typedef UINT(WINAPI* WTInfoPtr)(UINT p_category, UINT p_index, LPVOID p_output);
typedef BOOL(WINAPI* WTPacketPtr)(HANDLE p_ctx, UINT p_param, LPVOID p_packets);
typedef BOOL(WINAPI* WTEnablePtr)(HANDLE p_ctx, BOOL p_enable);

// Windows Ink API
#ifndef POINTER_STRUCTURES

#define POINTER_STRUCTURES

typedef DWORD POINTER_INPUT_TYPE;
typedef UINT32 POINTER_FLAGS;
typedef UINT32 PEN_FLAGS;
typedef UINT32 PEN_MASK;

#ifndef PEN_MASK_PRESSURE
#define PEN_MASK_PRESSURE 0x00000001
#endif

#ifndef PEN_MASK_TILT_X
#define PEN_MASK_TILT_X 0x00000004
#endif

#ifndef PEN_MASK_TILT_Y
#define PEN_MASK_TILT_Y 0x00000008
#endif

#ifndef POINTER_MESSAGE_FLAG_FIRSTBUTTON
#define POINTER_MESSAGE_FLAG_FIRSTBUTTON 0x00000010
#endif

enum tagPOINTER_INPUT_TYPE {
    PT_POINTER  = 0x00000001,
    PT_TOUCH    = 0x00000002,
    PT_PEN      = 0x00000003,
    PT_MOUSE    = 0x00000004,
    PT_TOUCHPAD = 0x00000005
};

typedef enum tagPOINTER_BUTTON_CHANGE_TYPE {
    POINTER_CHANGE_NONE,
    POINTER_CHANGE_FIRSTBUTTON_DOWN,
    POINTER_CHANGE_FIRSTBUTTON_UP,
    POINTER_CHANGE_SECONDBUTTON_DOWN,
    POINTER_CHANGE_SECONDBUTTON_UP,
    POINTER_CHANGE_THIRDBUTTON_DOWN,
    POINTER_CHANGE_THIRDBUTTON_UP,
    POINTER_CHANGE_FOURTHBUTTON_DOWN,
    POINTER_CHANGE_FOURTHBUTTON_UP,
    POINTER_CHANGE_FIFTHBUTTON_DOWN,
    POINTER_CHANGE_FIFTHBUTTON_UP,
} POINTER_BUTTON_CHANGE_TYPE;

typedef struct tagPOINTER_INFO {
    POINTER_INPUT_TYPE pointerType;
    UINT32 pointerId;
    UINT32 frameId;
    POINTER_FLAGS pointerFlags;
    HANDLE sourceDevice;
    HWND hwndTarget;
    POINT ptPixelLocation;
    POINT ptHimetricLocation;
    POINT ptPixelLocationRaw;
    POINT ptHimetricLocationRaw;
    DWORD dwTime;
    UINT32 historyCount;
    INT32 InputData;
    DWORD dwKeyStates;
    UINT64 PerformanceCount;
    POINTER_BUTTON_CHANGE_TYPE ButtonChangeType;
} POINTER_INFO;

typedef struct tagPOINTER_PEN_INFO {
    POINTER_INFO pointerInfo;
    PEN_FLAGS penFlags;
    PEN_MASK penMask;
    UINT32 pressure;
    UINT32 rotation;
    INT32 tiltX;
    INT32 tiltY;
} POINTER_PEN_INFO;

#endif

#ifndef WM_POINTERUPDATE
#define WM_POINTERUPDATE 0x0245
#endif

#ifndef WM_POINTERENTER
#define WM_POINTERENTER 0x0249
#endif

#ifndef WM_POINTERLEAVE
#define WM_POINTERLEAVE 0x024A
#endif

typedef BOOL(WINAPI* GetPointerTypePtr)(
    uint32_t p_id,
    POINTER_INPUT_TYPE* p_type
);
typedef BOOL(WINAPI* GetPointerPenInfoPtr)(
    uint32_t p_id,
    POINTER_PEN_INFO* p_pen_info
);

typedef struct {
    BYTE bWidth;         // Width, in pixels, of the image
    BYTE bHeight;        // Height, in pixels, of the image
    BYTE bColorCount;    // Number of colors in image (0 if >=8bpp)
    BYTE bReserved;      // Reserved ( must be 0)
    WORD wPlanes;        // Color Planes
    WORD wBitCount;      // Bits per pixel
    DWORD dwBytesInRes;  // How many bytes in this resource?
    DWORD dwImageOffset; // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct {
    WORD idReserved;           // Reserved (must be 0)
    WORD idType;               // Resource Type (1 for icons)
    WORD idCount;              // How many images?
    ICONDIRENTRY idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;

class WindowsJoypad;

class WindowsOS : public OS {
    String tablet_driver;
    Vector<String> tablet_drivers;

    // WinTab API
    static bool wintab_available;
    static WTOpenPtr wintab_WTOpen;
    static WTClosePtr wintab_WTClose;
    static WTInfoPtr wintab_WTInfo;
    static WTPacketPtr wintab_WTPacket;
    static WTEnablePtr wintab_WTEnable;

    // Windows Ink API
    static bool winink_available;
    static GetPointerTypePtr win8p_GetPointerType;
    static GetPointerPenInfoPtr win8p_GetPointerPenInfo;

    HANDLE wtctx;
    LOGCONTEXTW wtlc;
    int min_pressure;
    int max_pressure;
    bool tilt_supported;
    bool block_mm = false;

    int last_pressure_update;
    float last_pressure;
    Vector2 last_tilt;

    enum {
        KEY_EVENT_BUFFER_SIZE = 512
    };

#ifdef STDOUT_FILE
    FILE* stdo;
#endif

    struct KeyEvent {
        bool alt, shift, control, meta;
        UINT uMsg;
        WPARAM wParam;
        LPARAM lParam;
    };

    KeyEvent key_event_buffer[KEY_EVENT_BUFFER_SIZE];
    int key_event_pos;

    uint64_t ticks_start;
    uint64_t ticks_per_second;

    bool old_invalid;
    bool outside;
    int old_x, old_y;
    Point2i center;
#ifdef OPENGL_ENABLED
    WindowsGLContext* gl_context;
#endif // OPENGL_ENABLED
    VisualServer* visual_server;
    int pressrc;
    HINSTANCE hInstance; // Holds The Instance Of The Application
    HWND hWnd;

    Vector<Vector2> mpath;
    Point2 last_pos;

    bool layered_window;

    uint32_t move_timer_id;

    HCURSOR hCursor;

    Size2 min_size;
    Size2 max_size;

    Size2 window_rect;
    VideoMode video_mode;
    bool preserve_window_size = false;

    MainLoop* main_loop;

    WNDPROC user_proc;

    // IME
    HIMC im_himc;
    Vector2 im_position;

    MouseMode mouse_mode;
    int restore_mouse_trails;
    bool alt_mem;
    bool gr_mem;
    bool shift_mem;
    bool control_mem;
    bool meta_mem;
    bool force_quit;
    bool window_has_focus;
    uint32_t last_button_state;
    bool use_raw_input;
    bool drop_events;

    HCURSOR cursors[CURSOR_MAX] = {NULL};
    CursorShape cursor_shape;
    Map<CursorShape, Vector<Variant>> cursors_cache;

    InputDefault* input;
    WindowsJoypad* joypad;
    Map<int, Vector2> touch_state;

    WindowsPower* power_manager;

    int video_driver_index;
    WindowsAudioDriver windows_audio_driver;
#ifdef XAUDIO2_ENABLED
    AudioDriverXAudio2 driver_xaudio2;
#endif
#ifdef WINMIDI_ENABLED
    MIDIDriverWinMidi driver_midi;
#endif

    CrashHandler crash_handler;

    void _drag_event(float p_x, float p_y, int idx);
    void _touch_event(bool p_pressed, float p_x, float p_y, int idx);
    void _update_window_style(bool p_repaint = true, bool p_maximized = false);
    void _update_window_mouse_passthrough();
    void _set_mouse_mode_impl(MouseMode p_mode);

    // functions used by main to initialize/deinitialize the OS

protected:
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

    String _quote_command_line_argument(const String& p_text) const;

    struct ProcessInfo {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
    };

    Map<ProcessID, ProcessInfo>* process_map;

    bool pre_fs_valid;
    RECT pre_fs_rect;
    bool maximized;
    bool minimized;
    bool borderless;
    bool window_focused;
    bool console_visible;
    bool was_maximized;

public:
    LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void alert(
        const String& p_alert,
        const String& p_title = "ALERT!"
    ) override;
    String get_stdin_string(bool p_block) override;

    void set_mouse_mode(MouseMode p_mode) override;
    MouseMode get_mouse_mode() const override;

    void warp_mouse_position(const Point2& p_to) override;
    Point2 get_mouse_position() const override;
    void update_real_mouse_position();
    int get_mouse_button_state() const override;
    void set_window_title(const String& p_title) override;
    void set_window_mouse_passthrough(
        const PoolVector2Array& p_region
    ) override;

    void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    ) override;
    VideoMode get_video_mode(int p_screen = 0) const override;
    void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const override;

    int get_tablet_driver_count() const override;
    String get_tablet_driver_name(int p_driver) const override;
    String get_current_tablet_driver() const override;
    void set_current_tablet_driver(const String& p_driver) override;

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
    void set_console_visible(bool p_enabled) override;
    bool is_console_visible() const override;
    void request_attention() override;
    void* get_native_handle(int p_handle_type) override;

    void set_borderless_window(bool p_borderless) override;
    bool get_borderless_window() override;

    bool get_window_per_pixel_transparency_enabled() const override;
    void set_window_per_pixel_transparency_enabled(bool p_enabled) override;

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

    MainLoop* get_main_loop() const override;

    String get_name() const override;

    Date get_date(bool utc) const override;
    Time get_time(bool utc) const override;
    TimeZoneInfo get_time_zone_info() const override;
    uint64_t get_unix_time() const override;
    uint64_t get_system_time_secs() const override;
    uint64_t get_system_time_msecs() const override;

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
    int get_process_id() const override;

    bool has_environment(const String& p_var) const override;
    String get_environment(const String& p_var) const override;
    bool set_environment(
        const String& p_var,
        const String& p_value
    ) const override;

    void set_clipboard(const String& p_text) override;
    String get_clipboard() const override;

    void set_cursor_shape(CursorShape p_shape) override;
    CursorShape get_cursor_shape() const override;
    void set_custom_mouse_cursor(
        const RES& p_cursor,
        CursorShape p_shape,
        const Vector2& p_hotspot
    ) override;
    void GetMaskBitmaps(
        HBITMAP hSourceBitmap,
        COLORREF clrTransparent,
        OUT HBITMAP& hAndMaskBitmap,
        OUT HBITMAP& hXorMaskBitmap
    );

    void set_native_icon(const String& p_filename) override;
    void set_icon(const Ref<Image>& p_icon) override;

    String get_executable_path() const override;

    String get_locale() const override;

    int get_processor_count() const override;

    LatinKeyboardVariant get_latin_keyboard_variant() const override;
    int keyboard_get_layout_count() const override;
    int keyboard_get_current_layout() const override;
    void keyboard_set_current_layout(int p_index) override;
    String keyboard_get_layout_language(int p_index) const override;
    String keyboard_get_layout_name(int p_index) const override;

    void enable_for_stealing_focus(ProcessID pid) override;
    void move_window_to_foreground() override;

    String get_config_path() const override;
    String get_data_path() const override;
    String get_cache_path() const override;
    String get_rebel_dir_name() const override;

    String get_system_dir(
        SystemDir p_dir,
        bool p_shared_storage = true
    ) const override;
    String get_user_data_dir() const override;

    String get_unique_id() const override;

    void set_ime_active(const bool p_active) override;
    void set_ime_position(const Point2& p_pos) override;

    void release_rendering_thread() override;
    void make_rendering_thread() override;
    void swap_buffers() override;

    Error shell_open(String p_uri) override;

    void run();

    bool get_swap_ok_cancel() override {
        return true;
    }

    bool is_joy_known(int p_device) override;
    String get_joy_guid(int p_device) const override;

    void _set_use_vsync(bool p_enable) override;

    OS::PowerState get_power_state() override;
    int get_power_seconds_left() override;
    int get_power_percent_left() override;

    bool _check_internal_feature_support(const String& p_feature) override;

    void disable_crash_handler() override;
    bool is_disable_crash_handler() const override;
    void initialize_debugging() override;

    void force_process_input() override;

    Error move_to_trash(const String& p_path) override;

    void process_and_drop_events() override;

    WindowsOS(HINSTANCE _hInstance);
    ~WindowsOS() override;
};

#endif // WINDOWS_OS_H
