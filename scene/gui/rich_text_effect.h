// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RICH_TEXT_EFFECT_H
#define RICH_TEXT_EFFECT_H

#include "core/resource.h"

class RichTextEffect : public Resource {
    GDCLASS(RichTextEffect, Resource);
    OBJ_SAVE_TYPE(RichTextEffect);

protected:
    static void _bind_methods();

public:
    Variant get_bbcode() const;
    bool _process_effect_impl(Ref<class CharFXTransform> p_cfx);

    RichTextEffect();
};

class CharFXTransform : public Reference {
    GDCLASS(CharFXTransform, Reference);

protected:
    static void _bind_methods();

public:
    uint64_t relative_index;
    uint64_t absolute_index;
    bool visibility;
    Point2 offset;
    Color color;
    CharType character;
    float elapsed_time;
    Dictionary environment;

    CharFXTransform();
    ~CharFXTransform();

    uint64_t get_relative_index() {
        return relative_index;
    }

    void set_relative_index(uint64_t p_index) {
        relative_index = p_index;
    }

    uint64_t get_absolute_index() {
        return absolute_index;
    }

    void set_absolute_index(uint64_t p_index) {
        absolute_index = p_index;
    }

    float get_elapsed_time() {
        return elapsed_time;
    }

    void set_elapsed_time(float p_elapsed_time) {
        elapsed_time = p_elapsed_time;
    }

    bool is_visible() {
        return visibility;
    }

    void set_visibility(bool p_vis) {
        visibility = p_vis;
    }

    Point2 get_offset() {
        return offset;
    }

    void set_offset(Point2 p_offset) {
        offset = p_offset;
    }

    Color get_color() {
        return color;
    }

    void set_color(Color p_color) {
        color = p_color;
    }

    int get_character() {
        return (int)character;
    }

    void set_character(int p_char) {
        character = (CharType)p_char;
    }

    Dictionary get_environment() {
        return environment;
    }

    void set_environment(Dictionary p_environment) {
        environment = p_environment;
    }
};

#endif // RICH_TEXT_EFFECT_H
