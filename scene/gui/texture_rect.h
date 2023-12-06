// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TEXTURE_FRAME_H
#define TEXTURE_FRAME_H

#include "scene/gui/control.h"

class TextureRect : public Control {
    GDCLASS(TextureRect, Control);

public:
    enum StretchMode {
        STRETCH_SCALE_ON_EXPAND, // default, for backwards compatibility
        STRETCH_SCALE,
        STRETCH_TILE,
        STRETCH_KEEP,
        STRETCH_KEEP_CENTERED,
        STRETCH_KEEP_ASPECT,
        STRETCH_KEEP_ASPECT_CENTERED,
        STRETCH_KEEP_ASPECT_COVERED,
    };

private:
    bool expand;
    bool hflip;
    bool vflip;
    Ref<Texture> texture;
    StretchMode stretch_mode;

    void _texture_changed();

protected:
    void _notification(int p_what);
    virtual Size2 get_minimum_size() const;
    static void _bind_methods();

public:
    void set_texture(const Ref<Texture>& p_tex);
    Ref<Texture> get_texture() const;

    void set_expand(bool p_expand);
    bool has_expand() const;

    void set_stretch_mode(StretchMode p_mode);
    StretchMode get_stretch_mode() const;

    void set_flip_h(bool p_flip);
    bool is_flipped_h() const;

    void set_flip_v(bool p_flip);
    bool is_flipped_v() const;

    TextureRect();
    ~TextureRect();
};

VARIANT_ENUM_CAST(TextureRect::StretchMode);
#endif // TEXTURE_FRAME_H
