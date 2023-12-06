// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/engine.h"
#include "lightmap_denoiser.h"

void register_denoise_types() {
    LightmapDenoiserOIDN::make_default_denoiser();
}

void unregister_denoise_types() {}
