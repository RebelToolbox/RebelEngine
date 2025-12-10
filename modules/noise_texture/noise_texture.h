// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#include "core/image.h"
#include "core/reference.h"
#include "core/rid.h"
#include "open_simplex_noise.h"
#include "scene/resources/texture.h"

class NoiseTexture : public Texture {
    GDCLASS(NoiseTexture, Texture);

public:
    NoiseTexture();
    virtual ~NoiseTexture();

    virtual int get_width() const;
    virtual int get_height() const;
    virtual uint32_t get_flags() const;
    virtual void set_flags(uint32_t new_flags);
    virtual RID get_rid() const;
    virtual bool has_alpha() const;
    virtual Ref<Image> get_data() const;

    void set_width(int new_width);
    void set_height(int new_height);
    Ref<OpenSimplexNoise> get_noise();
    void set_noise(Ref<OpenSimplexNoise> new_noise);
    Vector2 get_noise_offset() const;
    void set_noise_offset(Vector2 new_noise_offset);
    bool get_seamless() const;
    void set_seamless(bool new_seamless);
    bool is_normalmap() const;
    void set_as_normalmap(bool new_normalmap);
    float get_bump_strength() const;
    void set_bump_strength(float new_bump_strength);

protected:
    virtual void _validate_property(PropertyInfo& property) const;

    static void _bind_methods();

private:
    Thread noise_thread;

    RID texture;
    Ref<Image> data;
    Ref<OpenSimplexNoise> noise;

    Vector2i size{512, 512};
    Vector2 noise_offset;

    uint32_t flags           = FLAGS_DEFAULT;
    float bump_strength      = 8;
    bool first_time          = true;
    bool regeneration_queued = false;
    bool update_queued       = false;
    bool normalmap           = false;
    bool seamless            = false;

    void _queue_update();
    void _update_texture();
    Ref<Image> _generate_texture() const;
    void _thread_done(const Ref<Image>& image);
    void _set_texture_data(const Ref<Image>& image);

    static void _thread_function(void* user_data);
};

#endif // NOISE_TEXTURE_H
