// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "skin.h"

void Skin::set_bind_count(int p_size) {
    ERR_FAIL_COND(p_size < 0);
    binds.resize(p_size);
    binds_ptr  = binds.ptrw();
    bind_count = p_size;
    emit_changed();
}

void Skin::add_bind(int p_bone, const Transform& p_pose) {
    uint32_t index = bind_count;
    set_bind_count(bind_count + 1);
    set_bind_bone(index, p_bone);
    set_bind_pose(index, p_pose);
}

void Skin::add_named_bind(const String& p_name, const Transform& p_pose) {
    uint32_t index = bind_count;
    set_bind_count(bind_count + 1);
    set_bind_name(index, p_name);
    set_bind_pose(index, p_pose);
}

void Skin::set_bind_name(int p_index, const StringName& p_name) {
    ERR_FAIL_INDEX(p_index, bind_count);
    bool notify_change =
        (binds_ptr[p_index].name != StringName()) != (p_name != StringName());
    binds_ptr[p_index].name = p_name;
    emit_changed();
    if (notify_change) {
        _change_notify();
    }
}

void Skin::set_bind_bone(int p_index, int p_bone) {
    ERR_FAIL_INDEX(p_index, bind_count);
    binds_ptr[p_index].bone = p_bone;
    emit_changed();
}

void Skin::set_bind_pose(int p_index, const Transform& p_pose) {
    ERR_FAIL_INDEX(p_index, bind_count);
    binds_ptr[p_index].pose = p_pose;
    emit_changed();
}

void Skin::clear_binds() {
    binds.clear();
    binds_ptr  = nullptr;
    bind_count = 0;
    emit_changed();
}

bool Skin::_set(const StringName& p_name, const Variant& p_value) {
    String name = p_name;
    if (name == "bind_count") {
        set_bind_count(p_value);
        return true;
    } else if (name.begins_with("bind/")) {
        int index   = name.get_slicec('/', 1).to_int();
        String what = name.get_slicec('/', 2);
        if (what == "bone") {
            set_bind_bone(index, p_value);
            return true;
        } else if (what == "name") {
            set_bind_name(index, p_value);
            return true;
        } else if (what == "pose") {
            set_bind_pose(index, p_value);
            return true;
        }
    }
    return false;
}

bool Skin::_get(const StringName& p_name, Variant& r_ret) const {
    String name = p_name;
    if (name == "bind_count") {
        r_ret = get_bind_count();
        return true;
    } else if (name.begins_with("bind/")) {
        int index   = name.get_slicec('/', 1).to_int();
        String what = name.get_slicec('/', 2);
        if (what == "bone") {
            r_ret = get_bind_bone(index);
            return true;
        } else if (what == "name") {
            r_ret = get_bind_name(index);
            return true;
        } else if (what == "pose") {
            r_ret = get_bind_pose(index);
            return true;
        }
    }
    return false;
}

void Skin::_get_property_list(List<PropertyInfo>* p_list) const {
    p_list->push_back(PropertyInfo(
        Variant::INT,
        "bind_count",
        PROPERTY_HINT_RANGE,
        "0,16384,1,or_greater"
    ));
    for (int i = 0; i < get_bind_count(); i++) {
        p_list->push_back(
            PropertyInfo(Variant::STRING, "bind/" + itos(i) + "/name")
        );
        p_list->push_back(PropertyInfo(
            Variant::INT,
            "bind/" + itos(i) + "/bone",
            PROPERTY_HINT_RANGE,
            "0,16384,1,or_greater",
            get_bind_name(i) != StringName() ? PROPERTY_USAGE_NOEDITOR
                                             : PROPERTY_USAGE_DEFAULT
        ));
        p_list->push_back(
            PropertyInfo(Variant::TRANSFORM, "bind/" + itos(i) + "/pose")
        );
    }
}

void Skin::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_bind_count", "bind_count"),
        &Skin::set_bind_count
    );
    ClassDB::bind_method(D_METHOD("get_bind_count"), &Skin::get_bind_count);

    ClassDB::bind_method(D_METHOD("add_bind", "bone", "pose"), &Skin::add_bind);

    ClassDB::bind_method(
        D_METHOD("set_bind_pose", "bind_index", "pose"),
        &Skin::set_bind_pose
    );
    ClassDB::bind_method(
        D_METHOD("get_bind_pose", "bind_index"),
        &Skin::get_bind_pose
    );

    ClassDB::bind_method(
        D_METHOD("set_bind_name", "bind_index", "name"),
        &Skin::set_bind_name
    );
    ClassDB::bind_method(
        D_METHOD("get_bind_name", "bind_index"),
        &Skin::get_bind_name
    );

    ClassDB::bind_method(
        D_METHOD("set_bind_bone", "bind_index", "bone"),
        &Skin::set_bind_bone
    );
    ClassDB::bind_method(
        D_METHOD("get_bind_bone", "bind_index"),
        &Skin::get_bind_bone
    );

    ClassDB::bind_method(D_METHOD("clear_binds"), &Skin::clear_binds);
}

Skin::Skin() {
    bind_count = 0;
    binds_ptr  = nullptr;
}
