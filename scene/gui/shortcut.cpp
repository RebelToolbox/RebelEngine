// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "shortcut.h"

#include "core/os/keyboard.h"

void ShortCut::set_shortcut(const Ref<InputEvent>& p_shortcut) {
    shortcut = p_shortcut;
    emit_changed();
}

Ref<InputEvent> ShortCut::get_shortcut() const {
    return shortcut;
}

bool ShortCut::is_shortcut(const Ref<InputEvent>& p_event) const {
    return shortcut.is_valid() && shortcut->shortcut_match(p_event);
}

String ShortCut::get_as_text() const {
    if (shortcut.is_valid()) {
        return shortcut->as_text();
    } else {
        return "None";
    }
}

bool ShortCut::is_valid() const {
    return shortcut.is_valid();
}

void ShortCut::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_shortcut", "event"),
        &ShortCut::set_shortcut
    );
    ClassDB::bind_method(D_METHOD("get_shortcut"), &ShortCut::get_shortcut);

    ClassDB::bind_method(D_METHOD("is_valid"), &ShortCut::is_valid);

    ClassDB::bind_method(
        D_METHOD("is_shortcut", "event"),
        &ShortCut::is_shortcut
    );
    ClassDB::bind_method(D_METHOD("get_as_text"), &ShortCut::get_as_text);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::OBJECT,
            "shortcut",
            PROPERTY_HINT_RESOURCE_TYPE,
            "InputEvent"
        ),
        "set_shortcut",
        "get_shortcut"
    );
}

ShortCut::ShortCut() {}
