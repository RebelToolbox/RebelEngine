// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "panel_container.h"

Size2 PanelContainer::get_minimum_size() const {
    Ref<StyleBox> style;

    if (has_stylebox("panel")) {
        style = get_stylebox("panel");
    } else {
        style = get_stylebox("panel", "PanelContainer");
    }

    Size2 ms;
    for (int i = 0; i < get_child_count(); i++) {
        Control* c = Object::cast_to<Control>(get_child(i));
        if (!c || !c->is_visible_in_tree()) {
            continue;
        }
        if (c->is_set_as_toplevel()) {
            continue;
        }

        Size2 minsize = c->get_combined_minimum_size();
        ms.width      = MAX(ms.width, minsize.width);
        ms.height     = MAX(ms.height, minsize.height);
    }

    if (style.is_valid()) {
        ms += style->get_minimum_size();
    }
    return ms;
}

void PanelContainer::_notification(int p_what) {
    if (p_what == NOTIFICATION_DRAW) {
        RID ci = get_canvas_item();
        Ref<StyleBox> style;

        if (has_stylebox("panel")) {
            style = get_stylebox("panel");
        } else {
            style = get_stylebox("panel", "PanelContainer");
        }

        style->draw(ci, Rect2(Point2(), get_size()));
    }

    if (p_what == NOTIFICATION_SORT_CHILDREN) {
        Ref<StyleBox> style;

        if (has_stylebox("panel")) {
            style = get_stylebox("panel");
        } else {
            style = get_stylebox("panel", "PanelContainer");
        }

        Size2 size = get_size();
        Point2 ofs;
        if (style.is_valid()) {
            size -= style->get_minimum_size();
            ofs  += style->get_offset();
        }

        for (int i = 0; i < get_child_count(); i++) {
            Control* c = Object::cast_to<Control>(get_child(i));
            if (!c || !c->is_visible_in_tree()) {
                continue;
            }
            if (c->is_set_as_toplevel()) {
                continue;
            }

            fit_child_in_rect(c, Rect2(ofs, size));
        }
    }
}

PanelContainer::PanelContainer() {}
