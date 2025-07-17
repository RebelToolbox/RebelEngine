// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SCROLL_CONTAINER_H
#define SCROLL_CONTAINER_H

#include "container.h"
#include "scroll_bar.h"

class ScrollContainer : public Container {
    GDCLASS(ScrollContainer, Container);

    HScrollBar* h_scroll;
    VScrollBar* v_scroll;

    Size2 child_max_size;
    Size2 scroll;

    void update_scrollbars();

    Vector2 drag_speed;
    Vector2 drag_accum;
    Vector2 drag_from;
    Vector2 last_drag_accum;
    float last_drag_time;
    float time_since_motion;
    bool drag_touching;
    bool drag_touching_deaccel;
    bool click_handled;
    bool beyond_deadzone;

    bool scroll_h;
    bool scroll_v;

    int deadzone;
    bool follow_focus;

    void _cancel_drag();

protected:
    Size2 get_minimum_size() const;

    void _gui_input(const Ref<InputEvent>& p_gui_input);
    void _gui_focus_changed(Control* p_control);
    void _notification(int p_what);

    void _scroll_moved(float);
    static void _bind_methods();

    void _update_scrollbar_position();

public:
    int get_v_scroll() const;
    void set_v_scroll(int p_pos);

    int get_h_scroll() const;
    void set_h_scroll(int p_pos);

    void set_enable_h_scroll(bool p_enable);
    bool is_h_scroll_enabled() const;

    void set_enable_v_scroll(bool p_enable);
    bool is_v_scroll_enabled() const;

    int get_deadzone() const;
    void set_deadzone(int p_deadzone);

    bool is_following_focus() const;
    void set_follow_focus(bool p_follow);

    HScrollBar* get_h_scrollbar();
    VScrollBar* get_v_scrollbar();
    void ensure_control_visible(const Control* p_control);

    virtual bool clips_input() const;

    virtual String get_configuration_warning() const;

    ScrollContainer();
};

#endif
