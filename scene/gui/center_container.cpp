// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "center_container.h"

Size2 CenterContainer::get_minimum_size() const {
    if (use_top_left) {
        return Size2();
    }
    Size2 ms;
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
        Size2 minsize = c->get_combined_minimum_size();
        ms.width      = MAX(ms.width, minsize.width);
        ms.height     = MAX(ms.height, minsize.height);
    }

    return ms;
}

void CenterContainer::set_use_top_left(bool p_enable) {
    if (use_top_left == p_enable) {
        return;
    }

    use_top_left = p_enable;

    minimum_size_changed();
    queue_sort();
}

bool CenterContainer::is_using_top_left() const {
    return use_top_left;
}

void CenterContainer::_notification(int p_what) {
    if (p_what == NOTIFICATION_SORT_CHILDREN) {
        Size2 size = get_size();
        for (int i = 0; i < get_child_count(); i++) {
            Control* c = Object::cast_to<Control>(get_child(i));
            if (!c) {
                continue;
            }
            if (c->is_set_as_toplevel()) {
                continue;
            }

            Size2 minsize = c->get_combined_minimum_size();
            Point2 ofs    = use_top_left ? (-minsize * 0.5).floor()
                                         : ((size - minsize) / 2.0).floor();
            fit_child_in_rect(c, Rect2(ofs, minsize));
        }
    }
}

void CenterContainer::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_use_top_left", "enable"),
        &CenterContainer::set_use_top_left
    );
    ClassDB::bind_method(
        D_METHOD("is_using_top_left"),
        &CenterContainer::is_using_top_left
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "use_top_left"),
        "set_use_top_left",
        "is_using_top_left"
    );
}

CenterContainer::CenterContainer() {
    use_top_left = false;
}
