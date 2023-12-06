// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "reference_rect.h"

#include "core/engine.h"

void ReferenceRect::_notification(int p_what) {
    if (p_what == NOTIFICATION_DRAW) {
        if (!is_inside_tree()) {
            return;
        }
        if (Engine::get_singleton()->is_editor_hint() || !editor_only) {
            draw_rect(
                Rect2(Point2(), get_size()),
                border_color,
                false,
                border_width
            );
        }
    }
}

void ReferenceRect::set_border_color(const Color& p_color) {
    border_color = p_color;
    update();
}

Color ReferenceRect::get_border_color() const {
    return border_color;
}

void ReferenceRect::set_border_width(float p_width) {
    border_width = MAX(0.0, p_width);
    update();
}

float ReferenceRect::get_border_width() const {
    return border_width;
}

void ReferenceRect::set_editor_only(const bool& p_enabled) {
    editor_only = p_enabled;
    update();
}

bool ReferenceRect::get_editor_only() const {
    return editor_only;
}

void ReferenceRect::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("get_border_color"),
        &ReferenceRect::get_border_color
    );
    ClassDB::bind_method(
        D_METHOD("set_border_color", "color"),
        &ReferenceRect::set_border_color
    );

    ClassDB::bind_method(
        D_METHOD("get_border_width"),
        &ReferenceRect::get_border_width
    );
    ClassDB::bind_method(
        D_METHOD("set_border_width", "width"),
        &ReferenceRect::set_border_width
    );

    ClassDB::bind_method(
        D_METHOD("get_editor_only"),
        &ReferenceRect::get_editor_only
    );
    ClassDB::bind_method(
        D_METHOD("set_editor_only", "enabled"),
        &ReferenceRect::set_editor_only
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::COLOR, "border_color"),
        "set_border_color",
        "get_border_color"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::REAL,
            "border_width",
            PROPERTY_HINT_RANGE,
            "0.0,5.0,0.1,or_greater"
        ),
        "set_border_width",
        "get_border_width"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "editor_only"),
        "set_editor_only",
        "get_editor_only"
    );
}
