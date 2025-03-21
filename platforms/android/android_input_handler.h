// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_INPUT_HANDLER_H
#define ANDROID_INPUT_HANDLER_H

#include "main/input_default.h"

// This handler handles all input events from the Android UI thread.
// It is not thread-safe. Therefore:
// - All methods must be called on the Android UI thread.
// - Methods must only call thread-safe methods.

class AndroidInputHandler {
public:
    struct Touch {
        int id;
        Point2 pos;
    };

    enum {
        JOY_EVENT_BUTTON = 0,
        JOY_EVENT_AXIS   = 1,
        JOY_EVENT_HAT    = 2
    };

    struct JoypadEvent {
        int device;
        int type;
        int index;
        bool pressed;
        float value;
        int hat;
    };

    void process_key_event(
        int p_keycode,
        int p_scancode,
        int p_unicode_char,
        bool p_pressed
    );
    void process_touch(
        int p_event,
        int p_pointer,
        const Vector<Touch>& p_touches
    );
    void process_hover(int p_type, Point2 p_pos);
    void process_mouse_event(
        int p_event_action,
        int p_event_android_buttons_mask,
        Point2 p_event_pos,
        float p_event_vertical_factor,
        float p_event_horizontal_factor
    );
    void process_double_tap(int p_event_android_buttons_mask, Point2 p_pos);
    void process_scroll(Point2 p_pos);
    void process_joy_event(const JoypadEvent& p_event);
    void joy_connection_changed(int p_device, bool p_connected, String p_name);

private:
    Vector<Touch> prev_touches;
    Point2 prev_hover_pos;
    Point2 prev_scroll_pos;

    bool alt_mem     = false;
    bool shift_mem   = false;
    bool control_mem = false;
    bool meta_mem    = false;

    int buttons_state = 0;

    InputDefault* input =
        static_cast<InputDefault*>(InputDefault::get_singleton());

    void set_key_modifier_state(
        Ref<InputEventWithModifiers> r_event_with_modifiers
    ) const;
    void wheel_button_click(
        const Ref<InputEventMouseButton>& p_mouse_button_event,
        int p_wheel_button,
        int p_event_buttons_mask,
        float p_factor
    );

    static int button_index_from_buttons_mask(int p_buttons_mask);
    static int rebel_buttons_mask_from_android_buttons_mask(
        int p_android_buttons_mask
    );
};

#endif
