// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IMAGE_LOADER_PNG_H
#define IMAGE_LOADER_PNG_H

#include "core/io/image_loader.h"

class ImageLoaderPNG : public ImageFormatLoader {
private:
    static PoolVector<uint8_t> lossless_pack_png(const Ref<Image>& p_image);
    static Ref<Image> lossless_unpack_png(const PoolVector<uint8_t>& p_data);
    static Ref<Image> load_mem_png(const uint8_t* p_png, int p_size);

public:
    virtual Error load_image(
        Ref<Image> p_image,
        FileAccess* f,
        bool p_force_linear,
        float p_scale
    );
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    ImageLoaderPNG();
};

#endif
