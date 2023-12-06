// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "pane_drag.h"

void PaneDrag::_gui_input(const Ref<InputEvent>& p_input) {
    Ref<InputEventMouseMotion> mm = p_input;
    if (mm.is_valid() && mm->get_button_mask() & BUTTON_MASK_LEFT) {
        emit_signal(
            "dragged",
            Point2(mm->get_relative().x, mm->get_relative().y)
        );
    }
}

void PaneDrag::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_DRAW: {
            Ref<Texture> icon = mouse_over
                                  ? get_icon("PaneDragHover", "EditorIcons")
                                  : get_icon("PaneDrag", "EditorIcons");
            if (!icon.is_null()) {
                icon->draw(get_canvas_item(), Point2(0, 0));
            }

        } break;
        case NOTIFICATION_MOUSE_ENTER:
            mouse_over = true;
            update();
            break;
        case NOTIFICATION_MOUSE_EXIT:
            mouse_over = false;
            update();
            break;
    }
}

Size2 PaneDrag::get_minimum_size() const {
    Ref<Texture> icon = get_icon("PaneDrag", "EditorIcons");
    if (!icon.is_null()) {
        return icon->get_size();
    }
    return Size2();
}

void PaneDrag::_bind_methods() {
    ClassDB::bind_method("_gui_input", &PaneDrag::_gui_input);
    ADD_SIGNAL(MethodInfo("dragged", PropertyInfo(Variant::VECTOR2, "amount")));
}

PaneDrag::PaneDrag() {
    mouse_over = false;
}
