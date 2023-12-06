// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "separator.h"

Size2 Separator::get_minimum_size() const {
    Size2 ms(3, 3);
    if (orientation == VERTICAL) {
        ms.x = get_constant("separation");
    } else { // HORIZONTAL
        ms.y = get_constant("separation");
    }
    return ms;
}

void Separator::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_DRAW: {
            Size2i size         = get_size();
            Ref<StyleBox> style = get_stylebox("separator");
            Size2i ssize = style->get_minimum_size() + style->get_center_size();

            if (orientation == VERTICAL) {
                style->draw(
                    get_canvas_item(),
                    Rect2((size.x - ssize.x) / 2, 0, ssize.x, size.y)
                );
            } else {
                style->draw(
                    get_canvas_item(),
                    Rect2(0, (size.y - ssize.y) / 2, size.x, ssize.y)
                );
            }

        } break;
    }
}

Separator::Separator() {}

Separator::~Separator() {}

HSeparator::HSeparator() {
    orientation = HORIZONTAL;
}

VSeparator::VSeparator() {
    orientation = VERTICAL;
}
