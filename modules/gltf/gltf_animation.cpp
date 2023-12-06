// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gltf_animation.h"

void GLTFAnimation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_loop"), &GLTFAnimation::get_loop);
    ClassDB::bind_method(
        D_METHOD("set_loop", "loop"),
        &GLTFAnimation::set_loop
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "loop"),
        "set_loop",
        "get_loop"
    ); // bool
}

bool GLTFAnimation::get_loop() const {
    return loop;
}

void GLTFAnimation::set_loop(bool p_val) {
    loop = p_val;
}

Map<int, GLTFAnimation::Track>& GLTFAnimation::get_tracks() {
    return tracks;
}

GLTFAnimation::GLTFAnimation() {}
