// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_SPIN_SLIDER_H
#define EDITOR_SPIN_SLIDER_H

#include "scene/gui/line_edit.h"
#include "scene/gui/range.h"
#include "scene/gui/texture_rect.h"

class EditorSpinSlider : public Range {
    GDCLASS(EditorSpinSlider, Range);

    String label;
    int updown_offset;
    bool hover_updown;
    bool mouse_hover;

    TextureRect* grabber;
    int grabber_range;

    bool mouse_over_spin;
    bool mouse_over_grabber;
    bool mousewheel_over_grabber;

    bool grabbing_grabber;
    int grabbing_from;
    float grabbing_ratio;

    bool grabbing_spinner_attempt;
    bool grabbing_spinner;

    bool read_only;
    float grabbing_spinner_dist_cache;
    Vector2 grabbing_spinner_mouse_pos;
    double pre_grab_value;

    LineEdit* value_input;
    bool value_input_just_closed;

    void _grabber_gui_input(const Ref<InputEvent>& p_event);
    void _value_input_closed();
    void _value_input_entered(const String&);
    void _value_focus_exited();
    void _value_input_gui_input(const Ref<InputEvent>& p_event);
    bool hide_slider;
    bool flat;

    bool use_custom_label_color;
    Color custom_label_color;

    void _evaluate_input_text();

    void _draw_spin_slider();

protected:
    void _notification(int p_what);
    void _gui_input(const Ref<InputEvent>& p_event);
    static void _bind_methods();
    void _grabber_mouse_entered();
    void _grabber_mouse_exited();
    void _focus_entered();

public:
    String get_tooltip(const Point2& p_pos) const;

    String get_text_value() const;
    void set_label(const String& p_label);
    String get_label() const;

    void set_hide_slider(bool p_hide);
    bool is_hiding_slider() const;

    void set_read_only(bool p_enable);
    bool is_read_only() const;

    void set_flat(bool p_enable);
    bool is_flat() const;

    void set_custom_label_color(
        bool p_use_custom_label_color,
        Color p_custom_label_color
    );

    void setup_and_show() {
        _focus_entered();
    }

    LineEdit* get_line_edit() {
        return value_input;
    }

    virtual Size2 get_minimum_size() const;
    EditorSpinSlider();
};

#endif // EDITOR_SPIN_SLIDER_H
