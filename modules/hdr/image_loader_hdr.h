// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IMAGE_LOADER_HDR_H
#define IMAGE_LOADER_HDR_H

#include "core/io/image_loader.h"

class ImageLoaderHDR : public ImageFormatLoader {
public:
    virtual Error load_image(
        Ref<Image> p_image,
        FileAccess* f,
        bool p_force_linear,
        float p_scale
    );
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    ImageLoaderHDR();
};

#endif
