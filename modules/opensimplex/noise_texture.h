// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#include "core/image.h"
#include "core/reference.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/property_editor.h"
#include "open_simplex_noise.h"

class NoiseTexture : public Texture {
    GDCLASS(NoiseTexture, Texture);

private:
    Ref<Image> data;

    Thread noise_thread;

    bool first_time;
    bool update_queued;
    bool regen_queued;

    RID texture;
    uint32_t flags;

    Ref<OpenSimplexNoise> noise;
    Vector2i size;
    Vector2 noise_offset;
    bool seamless;
    bool as_normalmap;
    float bump_strength;

    void _thread_done(const Ref<Image>& p_image);
    static void _thread_function(void* p_ud);

    void _queue_update();
    Ref<Image> _generate_texture();
    void _update_texture();
    void _set_texture_data(const Ref<Image>& p_image);

protected:
    static void _bind_methods();
    virtual void _validate_property(PropertyInfo& property) const;

public:
    void set_noise(Ref<OpenSimplexNoise> p_noise);
    Ref<OpenSimplexNoise> get_noise();

    void set_width(int p_width);
    void set_height(int p_height);

    void set_noise_offset(Vector2 p_noise_offset);
    Vector2 get_noise_offset() const;

    void set_seamless(bool p_seamless);
    bool get_seamless();

    void set_as_normalmap(bool p_as_normalmap);
    bool is_normalmap();

    void set_bump_strength(float p_bump_strength);
    float get_bump_strength();

    int get_width() const;
    int get_height() const;

    virtual void set_flags(uint32_t p_flags);
    virtual uint32_t get_flags() const;

    virtual RID get_rid() const {
        return texture;
    }

    virtual bool has_alpha() const {
        return false;
    }

    virtual Ref<Image> get_data() const;

    NoiseTexture();
    virtual ~NoiseTexture();
};

#endif // NOISE_TEXTURE_H
