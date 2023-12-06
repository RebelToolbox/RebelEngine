// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gltf_light.h"

void GLTFLight::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_color"), &GLTFLight::get_color);
    ClassDB::bind_method(D_METHOD("set_color", "color"), &GLTFLight::set_color);
    ClassDB::bind_method(D_METHOD("get_intensity"), &GLTFLight::get_intensity);
    ClassDB::bind_method(
        D_METHOD("set_intensity", "intensity"),
        &GLTFLight::set_intensity
    );
    ClassDB::bind_method(D_METHOD("get_type"), &GLTFLight::get_type);
    ClassDB::bind_method(D_METHOD("set_type", "type"), &GLTFLight::set_type);
    ClassDB::bind_method(D_METHOD("get_range"), &GLTFLight::get_range);
    ClassDB::bind_method(D_METHOD("set_range", "range"), &GLTFLight::set_range);
    ClassDB::bind_method(
        D_METHOD("get_inner_cone_angle"),
        &GLTFLight::get_inner_cone_angle
    );
    ClassDB::bind_method(
        D_METHOD("set_inner_cone_angle", "inner_cone_angle"),
        &GLTFLight::set_inner_cone_angle
    );
    ClassDB::bind_method(
        D_METHOD("get_outer_cone_angle"),
        &GLTFLight::get_outer_cone_angle
    );
    ClassDB::bind_method(
        D_METHOD("set_outer_cone_angle", "outer_cone_angle"),
        &GLTFLight::set_outer_cone_angle
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::COLOR, "color"),
        "set_color",
        "get_color"
    ); // Color
    ADD_PROPERTY(
        PropertyInfo(Variant::REAL, "intensity"),
        "set_intensity",
        "get_intensity"
    ); // float
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "type"),
        "set_type",
        "get_type"
    ); // String
    ADD_PROPERTY(
        PropertyInfo(Variant::REAL, "range"),
        "set_range",
        "get_range"
    ); // float
    ADD_PROPERTY(
        PropertyInfo(Variant::REAL, "inner_cone_angle"),
        "set_inner_cone_angle",
        "get_inner_cone_angle"
    ); // float
    ADD_PROPERTY(
        PropertyInfo(Variant::REAL, "outer_cone_angle"),
        "set_outer_cone_angle",
        "get_outer_cone_angle"
    ); // float
}

Color GLTFLight::get_color() {
    return color;
}

void GLTFLight::set_color(Color p_color) {
    color = p_color;
}

float GLTFLight::get_intensity() {
    return intensity;
}

void GLTFLight::set_intensity(float p_intensity) {
    intensity = p_intensity;
}

String GLTFLight::get_type() {
    return type;
}

void GLTFLight::set_type(String p_type) {
    type = p_type;
}

float GLTFLight::get_range() {
    return range;
}

void GLTFLight::set_range(float p_range) {
    range = p_range;
}

float GLTFLight::get_inner_cone_angle() {
    return inner_cone_angle;
}

void GLTFLight::set_inner_cone_angle(float p_inner_cone_angle) {
    inner_cone_angle = p_inner_cone_angle;
}

float GLTFLight::get_outer_cone_angle() {
    return outer_cone_angle;
}

void GLTFLight::set_outer_cone_angle(float p_outer_cone_angle) {
    outer_cone_angle = p_outer_cone_angle;
}
