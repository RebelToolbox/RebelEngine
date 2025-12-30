// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "noise_texture.h"

#include "core/core_string_names.h"

NoiseTexture::NoiseTexture() {
    texture = VS::get_singleton()->texture_create();
    _queue_update();
}

NoiseTexture::~NoiseTexture() {
    VS::get_singleton()->free(texture);
    noise_thread.wait_to_finish();
}

int NoiseTexture::get_width() const {
    return size.x;
}

int NoiseTexture::get_height() const {
    return size.y;
}

uint32_t NoiseTexture::get_flags() const {
    return flags;
}

void NoiseTexture::set_flags(const uint32_t new_flags) {
    flags = new_flags;
    VS::get_singleton()->texture_set_flags(texture, flags);
}

RID NoiseTexture::get_rid() const {
    return texture;
}

bool NoiseTexture::has_alpha() const {
    return false;
}

Ref<Image> NoiseTexture::get_data() const {
    return data;
}

void NoiseTexture::set_width(const int new_width) {
    if (size.x == new_width) {
        return;
    }
    size.x = new_width;
    _queue_update();
}

void NoiseTexture::set_height(const int new_height) {
    if (size.y == new_height) {
        return;
    }
    size.y = new_height;
    _queue_update();
}

Ref<OpenSimplexNoise> NoiseTexture::get_noise() {
    return noise;
}

void NoiseTexture::set_noise(Ref<OpenSimplexNoise> new_noise) {
    if (noise == new_noise) {
        return;
    }
    if (noise.is_valid()) {
        noise->disconnect(
            CoreStringNames::get_singleton()->changed,
            this,
            "_queue_update"
        );
    }
    noise = new_noise;
    if (noise.is_valid()) {
        noise->connect(
            CoreStringNames::get_singleton()->changed,
            this,
            "_queue_update"
        );
    }
    _queue_update();
}

Vector2 NoiseTexture::get_noise_offset() const {
    return noise_offset;
}

void NoiseTexture::set_noise_offset(const Vector2 new_noise_offset) {
    if (noise_offset == new_noise_offset) {
        return;
    }
    noise_offset = new_noise_offset;
    _queue_update();
}

bool NoiseTexture::get_seamless() const {
    return seamless;
}

void NoiseTexture::set_seamless(const bool new_seamless) {
    if (seamless == new_seamless) {
        return;
    }
    seamless = new_seamless;
    _queue_update();
}

bool NoiseTexture::is_normalmap() const {
    return normalmap;
}

void NoiseTexture::set_as_normalmap(const bool new_normalmap) {
    if (new_normalmap == normalmap) {
        return;
    }
    normalmap = new_normalmap;
    _queue_update();
    _change_notify();
}

float NoiseTexture::get_bump_strength() const {
    return bump_strength;
}

void NoiseTexture::set_bump_strength(const float new_bump_strength) {
    if (bump_strength == new_bump_strength) {
        return;
    }
    bump_strength = new_bump_strength;
    if (normalmap) {
        _queue_update();
    }
}

void NoiseTexture::_validate_property(PropertyInfo& property) const {
    if (property.name == "bump_strength") {
        if (!normalmap) {
            property.usage = PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL;
        }
    }
}

void NoiseTexture::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_width", "width"),
        &NoiseTexture::set_width
    );
    ClassDB::bind_method(
        D_METHOD("set_height", "height"),
        &NoiseTexture::set_height
    );
    ClassDB::bind_method(D_METHOD("get_noise"), &NoiseTexture::get_noise);
    ClassDB::bind_method(
        D_METHOD("set_noise", "noise"),
        &NoiseTexture::set_noise
    );
    ClassDB::bind_method(
        D_METHOD("get_noise_offset"),
        &NoiseTexture::get_noise_offset
    );
    ClassDB::bind_method(
        D_METHOD("set_noise_offset", "noise_offset"),
        &NoiseTexture::set_noise_offset
    );
    ClassDB::bind_method(D_METHOD("get_seamless"), &NoiseTexture::get_seamless);
    ClassDB::bind_method(
        D_METHOD("set_seamless", "seamless"),
        &NoiseTexture::set_seamless
    );
    ClassDB::bind_method(D_METHOD("is_normalmap"), &NoiseTexture::is_normalmap);
    ClassDB::bind_method(
        D_METHOD("set_as_normalmap", "as_normalmap"),
        &NoiseTexture::set_as_normalmap
    );
    ClassDB::bind_method(
        D_METHOD("get_bump_strength"),
        &NoiseTexture::get_bump_strength
    );
    ClassDB::bind_method(
        D_METHOD("set_bump_strength", "bump_strength"),
        &NoiseTexture::set_bump_strength
    );

    ClassDB::bind_method(
        D_METHOD("_generate_texture"),
        &NoiseTexture::_generate_texture
    );
    ClassDB::bind_method(
        D_METHOD("_queue_update"),
        &NoiseTexture::_queue_update
    );
    ClassDB::bind_method(
        D_METHOD("_thread_done", "image"),
        &NoiseTexture::_thread_done
    );
    ClassDB::bind_method(
        D_METHOD("_update_texture"),
        &NoiseTexture::_update_texture
    );

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "width",
            PROPERTY_HINT_RANGE,
            "1,2048,1,or_greater"
        ),
        "set_width",
        "get_width"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "height",
            PROPERTY_HINT_RANGE,
            "1,2048,1,or_greater"
        ),
        "set_height",
        "get_height"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::OBJECT,
            "noise",
            PROPERTY_HINT_RESOURCE_TYPE,
            "OpenSimplexNoise"
        ),
        "set_noise",
        "get_noise"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::VECTOR2, "noise_offset"),
        "set_noise_offset",
        "get_noise_offset"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "seamless"),
        "set_seamless",
        "get_seamless"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "as_normalmap"),
        "set_as_normalmap",
        "is_normalmap"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::REAL,
            "bump_strength",
            PROPERTY_HINT_RANGE,
            "0,32,0.1,or_greater"
        ),
        "set_bump_strength",
        "get_bump_strength"
    );
}

void NoiseTexture::_queue_update() {
    if (update_queued) {
        return;
    }
    update_queued = true;
    call_deferred("_update_texture");
}

void NoiseTexture::_update_texture() {
    bool use_thread = true;
    if (first_time) {
        use_thread = false;
        first_time = false;
    }
#ifdef NO_THREADS
    use_thread = false;
#endif
    if (use_thread) {
        if (!noise_thread.is_started()) {
            noise_thread.start(_thread_function, this);
            regeneration_queued = false;
        } else {
            regeneration_queued = true;
        }
    } else {
        const Ref<Image> image = _generate_texture();
        _set_texture_data(image);
    }
    update_queued = false;
}

Ref<Image> NoiseTexture::_generate_texture() const {
    // Prevent memdelete due to unref() on other thread.
    Ref<OpenSimplexNoise> noise_copy = noise;
    if (noise_copy.is_null()) {
        return {};
    }
    Ref<Image> image;
    if (seamless) {
        image = noise_copy->get_seamless_image(size.x);
    } else {
        image = noise_copy->get_image(size.x, size.y, noise_offset);
    }
    if (normalmap) {
        image->bumpmap_to_normalmap(bump_strength);
    }
    return image;
}

void NoiseTexture::_thread_done(const Ref<Image>& image) {
    _set_texture_data(image);
    noise_thread.wait_to_finish();
    if (regeneration_queued) {
        noise_thread.start(_thread_function, this);
        regeneration_queued = false;
    }
}

void NoiseTexture::_set_texture_data(const Ref<Image>& image) {
    if (image.is_valid()) {
        VS::get_singleton()->texture_allocate(
            texture,
            size.x,
            size.y,
            0,
            image->get_format(),
            VS::TEXTURE_TYPE_2D,
            flags
        );
        VS::get_singleton()->texture_set_data(texture, image);
    }
    emit_changed();
}

void NoiseTexture::_thread_function(void* user_data) {
    const auto noise_texture = static_cast<NoiseTexture*>(user_data);
    noise_texture->call_deferred(
        "_thread_done",
        noise_texture->_generate_texture()
    );
}
