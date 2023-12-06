// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef POPUP_H
#define POPUP_H

#include "scene/gui/control.h"

class Popup : public Control {
    GDCLASS(Popup, Control);

    bool exclusive;
    bool popped_up;

private:
    void _popup(const Rect2& p_bounds = Rect2(), const bool p_centered = false);

protected:
    virtual void _post_popup() {}

    void _gui_input(Ref<InputEvent> p_event);
    void _notification(int p_what);
    virtual void _fix_size();
    static void _bind_methods();

public:
    enum {
        NOTIFICATION_POST_POPUP = 80,
        NOTIFICATION_POPUP_HIDE = 81
    };

    void set_exclusive(bool p_exclusive);
    bool is_exclusive() const;

    void popup_centered_ratio(float p_screen_ratio = 0.75);
    void popup_centered(const Size2& p_size = Size2());
    void popup_centered_minsize(const Size2& p_minsize = Size2());
    void set_as_minsize();
    void popup_centered_clamped(
        const Size2& p_size    = Size2(),
        float p_fallback_ratio = 0.75
    );
    virtual void popup(const Rect2& p_bounds = Rect2());

    virtual String get_configuration_warning() const;

    Popup();
    ~Popup();
};

class PopupPanel : public Popup {
    GDCLASS(PopupPanel, Popup);

protected:
    void _update_child_rects();
    void _notification(int p_what);

public:
    void set_child_rect(Control* p_child);
    virtual Size2 get_minimum_size() const;
    PopupPanel();
};

#endif
