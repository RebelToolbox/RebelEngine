// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_input_handler.h"

#include "android_keys_utils.h"
#include "core/os/os.h"

void AndroidInputHandler::process_key_event(
    int p_keycode,
    int p_scancode,
    int p_unicode_char,
    bool p_pressed
) {
    unsigned int scancode          = android_get_keysym(p_keycode);
    unsigned int physical_scancode = android_get_keysym(p_scancode);
    switch (scancode) {
        case KEY_SHIFT: {
            shift_mem = p_pressed;
        } break;
        case KEY_ALT: {
            alt_mem = p_pressed;
        } break;
        case KEY_CONTROL: {
            control_mem = p_pressed;
        } break;
        case KEY_META: {
            meta_mem = p_pressed;
        } break;
    }

    Ref<InputEventKey> key_event;
    key_event.instance();
    key_event->set_scancode(scancode);
    key_event->set_physical_scancode(physical_scancode);
    key_event->set_unicode(p_unicode_char);
    key_event->set_pressed(p_pressed);
    set_key_modifier_state(key_event);

    if (p_unicode_char == '\n') {
        key_event->set_scancode(KEY_ENTER);
    } else if (p_unicode_char == 61448) {
        key_event->set_scancode(KEY_BACKSPACE);
        key_event->set_unicode(KEY_BACKSPACE);
    } else if (p_unicode_char == 61453) {
        key_event->set_scancode(KEY_ENTER);
        key_event->set_unicode(KEY_ENTER);
    } else if (p_scancode == 4) {
        if (MainLoop* main_loop = OS::get_singleton()->get_main_loop()) {
            main_loop->call_deferred(
                "notification",
                MainLoop::NOTIFICATION_WM_GO_BACK_REQUEST
            );
        }
    }

    input->parse_input_event(key_event);
}

void AndroidInputHandler::process_touch(
    int p_event,
    int p_pointer,
    const Vector<Touch>& p_touches
) {
    switch (p_event) {
        case AMOTION_EVENT_ACTION_DOWN: {
            if (prev_touches.size()) {
                // Ensure all previous touches are up.
                for (int i = 0; i < prev_touches.size(); i++) {
                    Ref<InputEventScreenTouch> touch_event;
                    touch_event.instance();
                    touch_event->set_index(prev_touches[i].id);
                    touch_event->set_pressed(false);
                    touch_event->set_position(prev_touches[i].pos);
                    input->parse_input_event(touch_event);
                }
            }

            prev_touches.resize(p_touches.size());
            for (int i = 0; i < p_touches.size(); i++) {
                prev_touches.write[i].id  = p_touches[i].id;
                prev_touches.write[i].pos = p_touches[i].pos;
                Ref<InputEventScreenTouch> touch_event;
                touch_event.instance();
                touch_event->set_index(prev_touches[i].id);
                touch_event->set_pressed(true);
                touch_event->set_position(prev_touches[i].pos);
                input->parse_input_event(touch_event);
            }
        } break;

        case AMOTION_EVENT_ACTION_MOVE: {
            ERR_FAIL_COND(prev_touches.size() != p_touches.size());
            for (int i = 0; i < prev_touches.size(); i++) {
                int idx = -1;
                for (int j = 0; j < p_touches.size(); j++) {
                    if (prev_touches[i].id == p_touches[j].id) {
                        idx = j;
                        break;
                    }
                }
                ERR_CONTINUE(idx == -1);

                if (prev_touches[i].pos == p_touches[idx].pos) {
                    // Don't create unnecessary drag events.
                    continue;
                }

                Ref<InputEventScreenDrag> drag_event;
                drag_event.instance();
                drag_event->set_index(prev_touches[i].id);
                drag_event->set_position(p_touches[idx].pos);
                drag_event->set_relative(
                    p_touches[idx].pos - prev_touches[i].pos
                );
                input->parse_input_event(drag_event);
                prev_touches.write[i].pos = p_touches[idx].pos;
            }
        } break;

        case AMOTION_EVENT_ACTION_CANCEL:
        case AMOTION_EVENT_ACTION_UP: {
            if (prev_touches.size()) {
                for (int i = 0; i < prev_touches.size(); i++) {
                    Ref<InputEventScreenTouch> touch_event;
                    touch_event.instance();
                    touch_event->set_index(prev_touches[i].id);
                    touch_event->set_pressed(false);
                    touch_event->set_position(prev_touches[i].pos);
                    input->parse_input_event(touch_event);
                }
                prev_touches.clear();
            }
        } break;

        case AMOTION_EVENT_ACTION_POINTER_DOWN: {
            // Add touch
            for (int i = 0; i < p_touches.size(); i++) {
                if (p_touches[i].id == p_pointer) {
                    Touch touch_pos = p_touches[i];
                    prev_touches.push_back(touch_pos);
                    Ref<InputEventScreenTouch> touch_event;
                    touch_event.instance();
                    touch_event->set_index(touch_pos.id);
                    touch_event->set_pressed(true);
                    touch_event->set_position(touch_pos.pos);
                    input->parse_input_event(touch_event);
                    break;
                }
            }
        } break;

        case AMOTION_EVENT_ACTION_POINTER_UP: {
            // Remove touch
            for (int i = 0; i < prev_touches.size(); i++) {
                if (prev_touches[i].id == p_pointer) {
                    Ref<InputEventScreenTouch> touch_event;
                    touch_event.instance();
                    touch_event->set_index(prev_touches[i].id);
                    touch_event->set_pressed(false);
                    touch_event->set_position(prev_touches[i].pos);
                    input->parse_input_event(touch_event);
                    prev_touches.remove(i);
                    break;
                }
            }
        } break;
    }
}

void AndroidInputHandler::process_hover(int p_type, Point2 p_pos) {
    switch (p_type) {
        case AMOTION_EVENT_ACTION_HOVER_MOVE:
        case AMOTION_EVENT_ACTION_HOVER_ENTER:
        case AMOTION_EVENT_ACTION_HOVER_EXIT: {
            Ref<InputEventMouseMotion> mouse_motion_event;
            mouse_motion_event.instance();
            set_key_modifier_state(mouse_motion_event);
            mouse_motion_event->set_position(p_pos);
            mouse_motion_event->set_global_position(p_pos);
            mouse_motion_event->set_relative(p_pos - prev_hover_pos);
            input->parse_input_event(mouse_motion_event);
            prev_hover_pos = p_pos;
        } break;
    }
}

void AndroidInputHandler::process_mouse_event(
    int p_event_action,
    int p_event_android_buttons_mask,
    Point2 p_event_pos,
    float p_event_vertical_factor,
    float p_event_horizontal_factor
) {
    int event_buttons_mask = rebel_buttons_mask_from_android_buttons_mask(
        p_event_android_buttons_mask
    );
    switch (p_event_action) {
        case AMOTION_EVENT_ACTION_BUTTON_PRESS:
        case AMOTION_EVENT_ACTION_BUTTON_RELEASE: {
            Ref<InputEventMouseButton> mouse_button_event;
            mouse_button_event.instance();
            set_key_modifier_state(mouse_button_event);
            mouse_button_event->set_position(p_event_pos);
            mouse_button_event->set_global_position(p_event_pos);
            mouse_button_event->set_pressed(
                p_event_action == AMOTION_EVENT_ACTION_BUTTON_PRESS
            );
            int changed_buttons_mask = buttons_state ^ event_buttons_mask;

            buttons_state = event_buttons_mask;

            mouse_button_event->set_button_index(
                button_index_from_buttons_mask(changed_buttons_mask)
            );
            mouse_button_event->set_button_mask(event_buttons_mask);
            input->parse_input_event(mouse_button_event);
        } break;

        case AMOTION_EVENT_ACTION_MOVE: {
            Ref<InputEventMouseMotion> mouse_motion_event;
            mouse_motion_event.instance();
            set_key_modifier_state(mouse_motion_event);
            mouse_motion_event->set_position(p_event_pos);
            mouse_motion_event->set_global_position(p_event_pos);
            mouse_motion_event->set_relative(p_event_pos - prev_hover_pos);
            mouse_motion_event->set_button_mask(event_buttons_mask);
            input->parse_input_event(mouse_motion_event);
            prev_hover_pos = p_event_pos;
        } break;

        case AMOTION_EVENT_ACTION_SCROLL: {
            Ref<InputEventMouseButton> mouse_button_event;
            mouse_button_event.instance();
            set_key_modifier_state(mouse_button_event);
            mouse_button_event->set_position(p_event_pos);
            mouse_button_event->set_global_position(p_event_pos);
            mouse_button_event->set_pressed(true);
            buttons_state = event_buttons_mask;

            if (p_event_vertical_factor > 0) {
                wheel_button_click(
                    mouse_button_event,
                    BUTTON_WHEEL_UP,
                    event_buttons_mask,
                    p_event_vertical_factor
                );
            } else if (p_event_vertical_factor < 0) {
                wheel_button_click(
                    mouse_button_event,
                    BUTTON_WHEEL_DOWN,
                    event_buttons_mask,
                    -p_event_vertical_factor
                );
            }

            if (p_event_horizontal_factor > 0) {
                wheel_button_click(
                    mouse_button_event,
                    BUTTON_WHEEL_RIGHT,
                    event_buttons_mask,
                    p_event_horizontal_factor
                );
            } else if (p_event_horizontal_factor < 0) {
                wheel_button_click(
                    mouse_button_event,
                    BUTTON_WHEEL_LEFT,
                    event_buttons_mask,
                    -p_event_horizontal_factor
                );
            }
        } break;
    }
}

void AndroidInputHandler::process_double_tap(
    int p_event_android_buttons_mask,
    Point2 p_pos
) {
    int event_buttons_mask = rebel_buttons_mask_from_android_buttons_mask(
        p_event_android_buttons_mask
    );
    Ref<InputEventMouseButton> ev;
    ev.instance();
    set_key_modifier_state(ev);
    ev->set_position(p_pos);
    ev->set_global_position(p_pos);
    ev->set_pressed(event_buttons_mask != 0);
    ev->set_button_index(button_index_from_buttons_mask(event_buttons_mask));
    ev->set_button_mask(event_buttons_mask);
    ev->set_doubleclick(true);
    input->parse_input_event(ev);
}

void AndroidInputHandler::process_scroll(Point2 p_pos) {
    Ref<InputEventPanGesture> ev;
    ev.instance();
    set_key_modifier_state(ev);
    ev->set_position(p_pos);
    ev->set_delta(p_pos - prev_scroll_pos);
    input->parse_input_event(ev);
    prev_scroll_pos = p_pos;
}

void AndroidInputHandler::process_joy_event(const JoypadEvent& p_event) {
    switch (p_event.type) {
        case JOY_EVENT_BUTTON:
            input->joy_button(p_event.device, p_event.index, p_event.pressed);
            break;
        case JOY_EVENT_AXIS:
            InputDefault::JoyAxis value;
            value.min   = -1;
            value.value = p_event.value;
            input->joy_axis(p_event.device, p_event.index, value);
            break;
        case JOY_EVENT_HAT:
            input->joy_hat(p_event.device, p_event.hat);
            break;
        default:
            return;
    }
}

void AndroidInputHandler::joy_connection_changed(
    int p_device,
    bool p_connected,
    String p_name
) {
    input->joy_connection_changed(p_device, p_connected, p_name, "");
}

void AndroidInputHandler::set_key_modifier_state(
    Ref<InputEventWithModifiers> r_event_with_modifiers
) const {
    r_event_with_modifiers->set_shift(shift_mem);
    r_event_with_modifiers->set_alt(alt_mem);
    r_event_with_modifiers->set_metakey(meta_mem);
    r_event_with_modifiers->set_control(control_mem);
}

void AndroidInputHandler::wheel_button_click(
    const Ref<InputEventMouseButton>& p_mouse_button_event,
    int p_wheel_button,
    int p_event_buttons_mask,
    float p_factor
) {
    Ref<InputEventMouseButton> mouse_button_pressed =
        p_mouse_button_event->duplicate();
    mouse_button_pressed->set_button_index(p_wheel_button);
    mouse_button_pressed->set_button_mask(
        p_event_buttons_mask ^ (1 << (p_wheel_button - 1))
    );
    mouse_button_pressed->set_factor(p_factor);
    input->parse_input_event(mouse_button_pressed);

    Ref<InputEventMouseButton> mouse_button_released =
        mouse_button_pressed->duplicate();
    mouse_button_released->set_pressed(false);
    mouse_button_released->set_button_mask(p_event_buttons_mask);
    input->parse_input_event(mouse_button_released);
}

int AndroidInputHandler::button_index_from_buttons_mask(int p_buttons_mask) {
    switch (p_buttons_mask) {
        case BUTTON_MASK_LEFT:
            return BUTTON_LEFT;
        case BUTTON_MASK_RIGHT:
            return BUTTON_RIGHT;
        case BUTTON_MASK_MIDDLE:
            return BUTTON_MIDDLE;
        case BUTTON_MASK_XBUTTON1:
            return BUTTON_XBUTTON1;
        case BUTTON_MASK_XBUTTON2:
            return BUTTON_XBUTTON2;
        default:
            return 0;
    }
}

int AndroidInputHandler::rebel_buttons_mask_from_android_buttons_mask(
    int p_android_buttons_mask
) {
    int rebel_buttons_mask = 0;
    if (p_android_buttons_mask & AMOTION_EVENT_BUTTON_PRIMARY) {
        rebel_buttons_mask |= BUTTON_MASK_LEFT;
    }
    if (p_android_buttons_mask & AMOTION_EVENT_BUTTON_SECONDARY) {
        rebel_buttons_mask |= BUTTON_MASK_RIGHT;
    }
    if (p_android_buttons_mask & AMOTION_EVENT_BUTTON_TERTIARY) {
        rebel_buttons_mask |= BUTTON_MASK_MIDDLE;
    }
    if (p_android_buttons_mask & AMOTION_EVENT_BUTTON_BACK) {
        rebel_buttons_mask |= BUTTON_MASK_XBUTTON1;
    }
    if (p_android_buttons_mask & AMOTION_EVENT_BUTTON_FORWARD) {
        rebel_buttons_mask |= BUTTON_MASK_XBUTTON2;
    }
    return rebel_buttons_mask;
}
