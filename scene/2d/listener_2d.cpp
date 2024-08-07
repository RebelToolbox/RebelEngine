// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "listener_2d.h"

bool Listener2D::_set(const StringName& p_name, const Variant& p_value) {
    if (p_name == "current") {
        if (p_value.operator bool()) {
            make_current();
        } else {
            clear_current();
        }
    } else {
        return false;
    }
    return true;
}

bool Listener2D::_get(const StringName& p_name, Variant& r_ret) const {
    if (p_name == "current") {
        if (is_inside_tree() && get_tree()->is_node_being_edited(this)) {
            r_ret = current;
        } else {
            r_ret = is_current();
        }
    } else {
        return false;
    }
    return true;
}

void Listener2D::_get_property_list(List<PropertyInfo>* p_list) const {
    p_list->push_back(PropertyInfo(Variant::BOOL, "current"));
}

void Listener2D::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            if (!get_tree()->is_node_being_edited(this) && current) {
                make_current();
            }
        } break;
        case NOTIFICATION_EXIT_TREE: {
            if (!get_tree()->is_node_being_edited(this)) {
                if (is_current()) {
                    clear_current();
                    current = true; // Keep it true.
                } else {
                    current = false;
                }
            }
        } break;
    }
}

void Listener2D::make_current() {
    current = true;
    if (!is_inside_tree()) {
        return;
    }
    get_viewport()->_listener_2d_set(this);
}

void Listener2D::clear_current() {
    current = false;
    if (!is_inside_tree()) {
        return;
    }
    get_viewport()->_listener_2d_remove(this);
}

bool Listener2D::is_current() const {
    if (is_inside_tree() && !get_tree()->is_node_being_edited(this)) {
        return get_viewport()->get_listener_2d() == this;
    } else {
        return current;
    }
    return false;
}

void Listener2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("make_current"), &Listener2D::make_current);
    ClassDB::bind_method(D_METHOD("clear_current"), &Listener2D::clear_current);
    ClassDB::bind_method(D_METHOD("is_current"), &Listener2D::is_current);
}
