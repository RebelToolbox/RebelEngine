// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "panel.h"

#include "core/print_string.h"

void Panel::_notification(int p_what) {
    if (p_what == NOTIFICATION_DRAW) {
        RID ci              = get_canvas_item();
        Ref<StyleBox> style = get_stylebox("panel");
        style->draw(ci, Rect2(Point2(), get_size()));
    }
}

Panel::Panel() {
    set_mouse_filter(MOUSE_FILTER_STOP);
}

Panel::~Panel() {}
