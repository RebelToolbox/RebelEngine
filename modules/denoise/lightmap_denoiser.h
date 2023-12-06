// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LIGHTMAP_DENOISER_H
#define LIGHTMAP_DENOISER_H

#include "core/class_db.h"
#include "scene/3d/lightmapper.h"

struct OIDNDeviceImpl;

class LightmapDenoiserOIDN : public LightmapDenoiser {
    GDCLASS(LightmapDenoiserOIDN, LightmapDenoiser);

protected:
    void* device = nullptr;

public:
    static LightmapDenoiser* create_oidn_denoiser();

    Ref<Image> denoise_image(const Ref<Image>& p_image);

    static void make_default_denoiser();

    LightmapDenoiserOIDN();
    ~LightmapDenoiserOIDN();
};

#endif // LIGHTMAP_DENOISER_H
