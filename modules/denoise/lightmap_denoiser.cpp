// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "lightmap_denoiser.h"

#include "denoise_wrapper.h"

LightmapDenoiser* LightmapDenoiserOIDN::create_oidn_denoiser() {
    return memnew(LightmapDenoiserOIDN);
}

void LightmapDenoiserOIDN::make_default_denoiser() {
    create_function = create_oidn_denoiser;
}

Ref<Image> LightmapDenoiserOIDN::denoise_image(const Ref<Image>& p_image) {
    Ref<Image> img = p_image->duplicate();

    img->convert(Image::FORMAT_RGBF);

    PoolByteArray data = img->get_data();
    {
        PoolByteArray::Write w = data.write();
        if (!oidn_denoise(
                device,
                (float*)w.ptr(),
                img->get_width(),
                img->get_height()
            )) {
            return p_image;
        }
    }

    img->create(
        img->get_width(),
        img->get_height(),
        false,
        img->get_format(),
        data
    );
    return img;
}

LightmapDenoiserOIDN::LightmapDenoiserOIDN() {
    device = oidn_denoiser_init();
}

LightmapDenoiserOIDN::~LightmapDenoiserOIDN() {
    oidn_denoiser_finish(device);
}
