// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "open_simplex_noise.h"

namespace {
constexpr uint8_t byte_from_noise(const double noise) {
    // Convert noise value [-1:1] to byte [0:255].
    return static_cast<uint8_t>(CLAMP((noise + 1) / 2 * 255, 0, 255));
}
} // namespace

OpenSimplexNoise::OpenSimplexNoise() {
    initialise_contexts();
}

int OpenSimplexNoise::get_seed() const {
    return seed;
}

void OpenSimplexNoise::set_seed(const int new_seed) {
    if (seed == new_seed) {
        return;
    }
    seed = new_seed;
    initialise_contexts();
    emit_changed();
}

int OpenSimplexNoise::get_octaves() const {
    return octaves;
}

void OpenSimplexNoise::set_octaves(const int new_octaves) {
    if (octaves == new_octaves) {
        return;
    }
    ERR_FAIL_COND_MSG(
        new_octaves > MAX_OCTAVES,
        vformat(
            "The number of OpenSimplexNoise octaves is limited to %d; "
            " ignoring the new value.",
            MAX_OCTAVES
        )
    );
    octaves = CLAMP(new_octaves, 1, MAX_OCTAVES);
    emit_changed();
}

float OpenSimplexNoise::get_period() const {
    return period;
}

void OpenSimplexNoise::set_period(const float new_period) {
    if (period == new_period) {
        return;
    }
    period = new_period;
    emit_changed();
}

float OpenSimplexNoise::get_persistence() const {
    return persistence;
}

void OpenSimplexNoise::set_persistence(const float new_persistence) {
    if (persistence == new_persistence) {
        return;
    }
    persistence = new_persistence;
    emit_changed();
}

float OpenSimplexNoise::get_lacunarity() const {
    return lacunarity;
}

void OpenSimplexNoise::set_lacunarity(const float new_lacunarity) {
    if (lacunarity == new_lacunarity) {
        return;
    }
    lacunarity = new_lacunarity;
    emit_changed();
}

Ref<Image> OpenSimplexNoise::get_image(
    const int width,
    const int height,
    const Vector2& noise_offset
) const {
    PoolVector<uint8_t> data;
    data.resize(width * height);
    const PoolVector<uint8_t>::Write data_write = data.write();

    int index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const float noise = get_noise_2d(
                static_cast<real_t>(x) + noise_offset.x,
                static_cast<real_t>(y) + noise_offset.y
            );
            data_write[index++] = byte_from_noise(noise);
        }
    }
    Ref<Image> image =
        memnew(Image(width, height, false, Image::FORMAT_L8, data));
    return image;
}

Ref<Image> OpenSimplexNoise::get_seamless_image(const int size) const {
    PoolVector<uint8_t> data;
    data.resize(size * size);
    const PoolVector<uint8_t>::Write data_write = data.write();
    const auto radius       = static_cast<float>(size / (2 * Math_PI));
    const auto angle_factor = static_cast<float>(2 * Math_PI / size);

    int index = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            const float x_factor = static_cast<float>(y) * angle_factor;
            const float y_factor = static_cast<float>(x) * angle_factor;
            const float noise    = get_noise_4d(
                radius * Math::sin(y_factor),
                radius * Math::cos(y_factor),
                radius * Math::sin(x_factor),
                radius * Math::cos(x_factor)
            );
            data_write[index++] = byte_from_noise(noise);
        }
    }
    Ref<Image> image = memnew(Image(size, size, false, Image::FORMAT_L8, data));
    return image;
}

float OpenSimplexNoise::get_noise_1d(const float x) const {
    return get_noise_2d(x, 1.0);
}

float OpenSimplexNoise::get_noise_2d(const float x, const float y) const {
    float x_interval = x / period;
    float y_interval = y / period;
    float amplitude  = 1;
    float maximum    = 0;
    double sum       = 0;
    for (int i = 0; i < octaves; i++) {
        sum += amplitude
             * open_simplex_noise2(&contexts[i], x_interval, y_interval);
        maximum    += amplitude;
        x_interval *= lacunarity;
        y_interval *= lacunarity;
        amplitude  *= persistence;
    }
    if (maximum == 0) {
        return 0;
    }
    return static_cast<float>(sum / maximum);
}

float OpenSimplexNoise::get_noise_2dv(const Vector2& values) const {
    return get_noise_2d(values.x, values.y);
}

float OpenSimplexNoise::get_noise_3d(
    const float x,
    const float y,
    const float z
) const {
    float x_interval = x / period;
    float y_interval = y / period;
    float z_interval = z / period;
    float amplitude  = 1;
    float maximum    = 0;
    double sum       = 0;
    for (int i = 0; i < octaves; i++) {
        sum += amplitude
             * open_simplex_noise3(
                   &contexts[i],
                   x_interval,
                   y_interval,
                   z_interval
             );
        maximum    += amplitude;
        x_interval *= lacunarity;
        y_interval *= lacunarity;
        z_interval *= lacunarity;
        amplitude  *= persistence;
    }
    if (maximum == 0) {
        return 0;
    }
    return static_cast<float>(sum / maximum);
}

float OpenSimplexNoise::get_noise_3dv(const Vector3& values) const {
    return get_noise_3d(values.x, values.y, values.z);
}

float OpenSimplexNoise::get_noise_4d(
    const float x,
    const float y,
    const float z,
    const float w
) const {
    float x_interval = x / period;
    float y_interval = y / period;
    float z_interval = z / period;
    float w_interval = w / period;
    float amplitude  = 1;
    float maximum    = 0;
    double sum       = 0;
    for (int i = 0; i < octaves; i++) {
        sum += amplitude
             * open_simplex_noise4(
                   &contexts[i],
                   x_interval,
                   y_interval,
                   z_interval,
                   w_interval
             );
        maximum    += amplitude;
        x_interval *= lacunarity;
        y_interval *= lacunarity;
        z_interval *= lacunarity;
        w_interval *= lacunarity;
        amplitude  *= persistence;
    }
    if (maximum == 0) {
        return 0;
    }
    return static_cast<float>(sum / maximum);
}

void OpenSimplexNoise::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_seed"), &OpenSimplexNoise::get_seed);
    ClassDB::bind_method(
        D_METHOD("set_seed", "seed"),
        &OpenSimplexNoise::set_seed
    );
    ClassDB::bind_method(
        D_METHOD("get_octaves"),
        &OpenSimplexNoise::get_octaves
    );
    ClassDB::bind_method(
        D_METHOD("set_octaves", "octave_count"),
        &OpenSimplexNoise::set_octaves
    );
    ClassDB::bind_method(D_METHOD("get_period"), &OpenSimplexNoise::get_period);
    ClassDB::bind_method(
        D_METHOD("set_period", "period"),
        &OpenSimplexNoise::set_period
    );
    ClassDB::bind_method(
        D_METHOD("get_persistence"),
        &OpenSimplexNoise::get_persistence
    );
    ClassDB::bind_method(
        D_METHOD("set_persistence", "persistence"),
        &OpenSimplexNoise::set_persistence
    );
    ClassDB::bind_method(
        D_METHOD("get_lacunarity"),
        &OpenSimplexNoise::get_lacunarity
    );
    ClassDB::bind_method(
        D_METHOD("set_lacunarity", "lacunarity"),
        &OpenSimplexNoise::set_lacunarity
    );

    ClassDB::bind_method(
        D_METHOD("get_image", "width", "height", "noise_offset"),
        &OpenSimplexNoise::get_image,
        DEFVAL(Vector2())
    );
    ClassDB::bind_method(
        D_METHOD("get_seamless_image", "size"),
        &OpenSimplexNoise::get_seamless_image
    );

    ClassDB::bind_method(
        D_METHOD("get_noise_1d", "x"),
        &OpenSimplexNoise::get_noise_1d
    );
    ClassDB::bind_method(
        D_METHOD("get_noise_2d", "x", "y"),
        &OpenSimplexNoise::get_noise_2d
    );
    ClassDB::bind_method(
        D_METHOD("get_noise_2dv", "pos"),
        &OpenSimplexNoise::get_noise_2dv
    );
    ClassDB::bind_method(
        D_METHOD("get_noise_3d", "x", "y", "z"),
        &OpenSimplexNoise::get_noise_3d
    );
    ClassDB::bind_method(
        D_METHOD("get_noise_3dv", "pos"),
        &OpenSimplexNoise::get_noise_3dv
    );
    ClassDB::bind_method(
        D_METHOD("get_noise_4d", "x", "y", "z", "w"),
        &OpenSimplexNoise::get_noise_4d
    );

    ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "octaves",
            PROPERTY_HINT_RANGE,
            vformat("1,%d,1", MAX_OCTAVES)
        ),
        "set_octaves",
        "get_octaves"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::REAL,
            "period",
            PROPERTY_HINT_RANGE,
            "0.1,256.0,0.1"
        ),
        "set_period",
        "get_period"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::REAL,
            "persistence",
            PROPERTY_HINT_RANGE,
            "0.0,1.0,0.001"
        ),
        "set_persistence",
        "get_persistence"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::REAL,
            "lacunarity",
            PROPERTY_HINT_RANGE,
            "0.1,4.0,0.01"
        ),
        "set_lacunarity",
        "get_lacunarity"
    );
}

void OpenSimplexNoise::initialise_contexts() {
    for (int i = 0; i < MAX_OCTAVES; ++i) {
        open_simplex_noise(seed + i * 2, &contexts[i]);
    }
}
