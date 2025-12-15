// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "noise_texture.h"
#include "open_simplex_noise.h"

void register_noise_texture_types() {
    ClassDB::register_class<OpenSimplexNoise>();
    ClassDB::register_class<NoiseTexture>();
}

void unregister_noise_texture_types() {}
