// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "back_buffer_copy.h"

void BackBufferCopy::_update_copy_mode() {
    switch (copy_mode) {
        case COPY_MODE_DISABLED: {
            VS::get_singleton()->canvas_item_set_copy_to_backbuffer(
                get_canvas_item(),
                false,
                Rect2()
            );
        } break;
        case COPY_MODE_RECT: {
            VS::get_singleton()->canvas_item_set_copy_to_backbuffer(
                get_canvas_item(),
                true,
                rect
            );
        } break;
        case COPY_MODE_VIEWPORT: {
            VS::get_singleton()->canvas_item_set_copy_to_backbuffer(
                get_canvas_item(),
                true,
                Rect2()
            );

        } break;
    }
}

#ifdef TOOLS_ENABLED
Rect2 BackBufferCopy::_edit_get_rect() const {
    return rect;
}

bool BackBufferCopy::_edit_use_rect() const {
    return true;
}
#endif

Rect2 BackBufferCopy::get_anchorable_rect() const {
    return rect;
}

void BackBufferCopy::set_rect(const Rect2& p_rect) {
    rect = p_rect;
    _update_copy_mode();
}

Rect2 BackBufferCopy::get_rect() const {
    return rect;
}

void BackBufferCopy::set_copy_mode(CopyMode p_mode) {
    copy_mode = p_mode;
    _update_copy_mode();
}

BackBufferCopy::CopyMode BackBufferCopy::get_copy_mode() const {
    return copy_mode;
}

void BackBufferCopy::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_rect", "rect"),
        &BackBufferCopy::set_rect
    );
    ClassDB::bind_method(D_METHOD("get_rect"), &BackBufferCopy::get_rect);

    ClassDB::bind_method(
        D_METHOD("set_copy_mode", "copy_mode"),
        &BackBufferCopy::set_copy_mode
    );
    ClassDB::bind_method(
        D_METHOD("get_copy_mode"),
        &BackBufferCopy::get_copy_mode
    );

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "copy_mode",
            PROPERTY_HINT_ENUM,
            "Disabled,Rect,Viewport"
        ),
        "set_copy_mode",
        "get_copy_mode"
    );
    ADD_PROPERTY(PropertyInfo(Variant::RECT2, "rect"), "set_rect", "get_rect");

    BIND_ENUM_CONSTANT(COPY_MODE_DISABLED);
    BIND_ENUM_CONSTANT(COPY_MODE_RECT);
    BIND_ENUM_CONSTANT(COPY_MODE_VIEWPORT);
}

BackBufferCopy::BackBufferCopy() {
    rect      = Rect2(-100, -100, 200, 200);
    copy_mode = COPY_MODE_RECT;
    _update_copy_mode();
}

BackBufferCopy::~BackBufferCopy() {}
