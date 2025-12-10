// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef OPEN_SIMPLEX_NOISE_H
#define OPEN_SIMPLEX_NOISE_H

#include "core/image.h"
#include "core/resource.h"
#include "third-party/misc/open-simplex-noise.h"

class OpenSimplexNoise : public Resource {
    GDCLASS(OpenSimplexNoise, Resource);
    OBJ_SAVE_TYPE(OpenSimplexNoise);

public:
    OpenSimplexNoise();

    int get_seed() const;
    void set_seed(int new_seed);
    int get_octaves() const;
    void set_octaves(int new_octaves);
    float get_period() const;
    void set_period(float new_period);
    float get_persistence() const;
    void set_persistence(float new_persistence);
    float get_lacunarity() const;
    void set_lacunarity(float new_lacunarity);

    Ref<Image> get_image(
        int width,
        int height,
        const Vector2& noise_offset = Vector2()
    ) const;
    Ref<Image> get_seamless_image(int size) const;

    float get_noise_1d(float x) const;
    float get_noise_2d(float x, float y) const;
    float get_noise_2dv(const Vector2& values) const;
    float get_noise_3d(float x, float y, float z) const;
    float get_noise_3dv(const Vector3& values) const;
    float get_noise_4d(float x, float y, float z, float w) const;

protected:
    static void _bind_methods();

private:
    static constexpr int MAX_OCTAVES = 9;
    osn_context contexts[MAX_OCTAVES];
    int seed          = 0;
    int octaves       = 3;
    float persistence = 0.5f;
    float period      = 64;
    float lacunarity  = 2;

    void initialise_contexts();
};

#endif // OPEN_SIMPLEX_NOISE_H
