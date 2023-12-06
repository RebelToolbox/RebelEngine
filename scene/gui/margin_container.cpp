// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "margin_container.h"

Size2 MarginContainer::get_minimum_size() const {
    int margin_left   = get_constant("margin_left");
    int margin_top    = get_constant("margin_top");
    int margin_right  = get_constant("margin_right");
    int margin_bottom = get_constant("margin_bottom");

    Size2 max;

    for (int i = 0; i < get_child_count(); i++) {
        Control* c = Object::cast_to<Control>(get_child(i));
        if (!c) {
            continue;
        }
        if (c->is_set_as_toplevel()) {
            continue;
        }
        if (!c->is_visible()) {
            continue;
        }

        Size2 s = c->get_combined_minimum_size();
        if (s.width > max.width) {
            max.width = s.width;
        }
        if (s.height > max.height) {
            max.height = s.height;
        }
    }

    max.width  += (margin_left + margin_right);
    max.height += (margin_top + margin_bottom);

    return max;
}

void MarginContainer::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_SORT_CHILDREN: {
            int margin_left   = get_constant("margin_left");
            int margin_top    = get_constant("margin_top");
            int margin_right  = get_constant("margin_right");
            int margin_bottom = get_constant("margin_bottom");

            Size2 s = get_size();

            for (int i = 0; i < get_child_count(); i++) {
                Control* c = Object::cast_to<Control>(get_child(i));
                if (!c) {
                    continue;
                }
                if (c->is_set_as_toplevel()) {
                    continue;
                }

                int w = s.width - margin_left - margin_right;
                int h = s.height - margin_top - margin_bottom;
                fit_child_in_rect(c, Rect2(margin_left, margin_top, w, h));
            }
        } break;
        case NOTIFICATION_THEME_CHANGED: {
            minimum_size_changed();
        } break;
    }
}

MarginContainer::MarginContainer() {}
