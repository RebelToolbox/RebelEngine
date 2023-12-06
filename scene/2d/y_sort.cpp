// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "y_sort.h"

void YSort::set_sort_enabled(bool p_enabled) {
    sort_enabled = p_enabled;
    VS::get_singleton()->canvas_item_set_sort_children_by_y(
        get_canvas_item(),
        sort_enabled
    );
}

bool YSort::is_sort_enabled() const {
    return sort_enabled;
}

void YSort::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_sort_enabled", "enabled"),
        &YSort::set_sort_enabled
    );
    ClassDB::bind_method(D_METHOD("is_sort_enabled"), &YSort::is_sort_enabled);

    ADD_GROUP("Sort", "sort_");
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "sort_enabled"),
        "set_sort_enabled",
        "is_sort_enabled"
    );
}

YSort::YSort() {
    sort_enabled = true;
    VS::get_singleton()->canvas_item_set_sort_children_by_y(
        get_canvas_item(),
        true
    );
}
