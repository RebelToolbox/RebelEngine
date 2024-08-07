// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REBEL_JS_H
#define REBEL_JS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

// Config
extern void rebel_js_config_locale_get(char* p_ptr, int p_ptr_max);
extern void rebel_js_config_canvas_id_get(char* p_ptr, int p_ptr_max);

// OS
extern void rebel_js_os_finish_async(void (*p_callback)());
extern void rebel_js_os_request_quit_cb(void (*p_callback)());
extern int rebel_js_os_fs_is_persistent();
extern void rebel_js_os_fs_sync(void (*p_callback)());
extern int rebel_js_os_execute(const char* p_json);
extern void rebel_js_os_shell_open(const char* p_uri);
extern int rebel_js_os_hw_concurrency_get();

// Input
extern void rebel_js_input_mouse_button_cb(int (*p_callback)(
    int p_pressed,
    int p_button,
    double p_x,
    double p_y,
    int p_modifiers
));
extern void rebel_js_input_mouse_move_cb(void (*p_callback)(
    double p_x,
    double p_y,
    double p_rel_x,
    double p_rel_y,
    int p_modifiers
));
extern void rebel_js_input_mouse_wheel_cb(
    int (*p_callback)(double p_delta_x, double p_delta_y)
);
extern void rebel_js_input_touch_cb(
    void (*p_callback)(int p_type, int p_count),
    uint32_t* r_identifiers,
    double* r_coords
);
extern void rebel_js_input_key_cb(
    void (*p_callback)(int p_type, int p_repeat, int p_modifiers),
    char r_code[32],
    char r_key[32]
);

// Input gamepad
extern void rebel_js_input_gamepad_cb(void (*p_on_change)(
    int p_index,
    int p_connected,
    const char* p_id,
    const char* p_guid
));
extern int rebel_js_input_gamepad_sample();
extern int rebel_js_input_gamepad_sample_count();
extern int rebel_js_input_gamepad_sample_get(
    int p_idx,
    float r_btns[16],
    int32_t* r_btns_num,
    float r_axes[10],
    int32_t* r_axes_num,
    int32_t* r_standard
);
extern void rebel_js_input_paste_cb(void (*p_callback)(const char* p_text));
extern void rebel_js_input_drop_files_cb(
    void (*p_callback)(char** p_filev, int p_filec)
);

// Display
extern int rebel_js_display_screen_dpi_get();
extern double rebel_js_display_pixel_ratio_get();
extern void rebel_js_display_alert(const char* p_text);
extern int rebel_js_display_touchscreen_is_available();
extern int rebel_js_display_is_swap_ok_cancel();
extern void rebel_js_display_setup_canvas(
    int p_width,
    int p_height,
    int p_fullscreen,
    int p_hidpi
);

// Display canvas
extern void rebel_js_display_canvas_focus();
extern int rebel_js_display_canvas_is_focused();

// Display window
extern void rebel_js_display_desired_size_set(int p_width, int p_height);
extern int rebel_js_display_size_update();
extern void rebel_js_display_window_size_get(int32_t* p_x, int32_t* p_y);
extern void rebel_js_display_screen_size_get(int32_t* p_x, int32_t* p_y);
extern int rebel_js_display_fullscreen_request();
extern int rebel_js_display_fullscreen_exit();
extern void rebel_js_display_window_title_set(const char* p_text);
extern void rebel_js_display_window_icon_set(const uint8_t* p_ptr, int p_len);
extern int rebel_js_display_has_webgl(int p_version);

// Display clipboard
extern int rebel_js_display_clipboard_set(const char* p_text);
extern int rebel_js_display_clipboard_get(void (*p_callback)(const char* p_text)
);

// Display cursor
extern void rebel_js_display_cursor_set_shape(const char* p_cursor);
extern int rebel_js_display_cursor_is_hidden();
extern void rebel_js_display_cursor_set_custom_shape(
    const char* p_shape,
    const uint8_t* p_ptr,
    int p_len,
    int p_hotspot_x,
    int p_hotspot_y
);
extern void rebel_js_display_cursor_set_visible(int p_visible);
extern void rebel_js_display_cursor_lock_set(int p_lock);
extern int rebel_js_display_cursor_is_locked();

// Display listeners
extern void rebel_js_display_fullscreen_cb(void (*p_callback)(int p_fullscreen)
);
extern void rebel_js_display_window_blur_cb(void (*p_callback)());
extern void rebel_js_display_notification_cb(
    void (*p_callback)(int p_notification),
    int p_enter,
    int p_exit,
    int p_in,
    int p_out
);

// Display Virtual Keyboard
extern int rebel_js_display_vk_available();
extern void rebel_js_display_vk_cb(
    void (*p_input)(const char* p_text, int p_cursor)
);
extern void rebel_js_display_vk_show(
    const char* p_text,
    int p_multiline,
    int p_start,
    int p_end
);
extern void rebel_js_display_vk_hide();

#ifdef __cplusplus
}
#endif

#endif // REBEL_JS_H
