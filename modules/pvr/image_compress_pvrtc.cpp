// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "image_compress_pvrtc.h"

#include "core/image.h"
#include "core/reference.h"

#include <PvrTcEncoder.h>
#include <RgbaBitmap.h>

static void _compress_pvrtc4(Image* p_img) {
    Ref<Image> img = p_img->duplicate();

    bool make_mipmaps = false;
    if (!img->is_size_po2() || img->get_width() != img->get_height()) {
        make_mipmaps = img->has_mipmaps();
        img->resize_to_po2(true);
        // Resizing can fail for some formats
        if (!img->is_size_po2() || img->get_width() != img->get_height()) {
            ERR_FAIL_MSG("Failed to resize the image for compression.");
        }
    }
    img->convert(Image::FORMAT_RGBA8);
    if (!img->has_mipmaps() && make_mipmaps) {
        img->generate_mipmaps();
    }

    bool use_alpha = img->detect_alpha();

    Ref<Image> new_img;
    new_img.instance();
    new_img->create(
        img->get_width(),
        img->get_height(),
        img->has_mipmaps(),
        use_alpha ? Image::FORMAT_PVRTC4A : Image::FORMAT_PVRTC4
    );

    PoolVector<uint8_t> data = new_img->get_data();
    {
        PoolVector<uint8_t>::Write wr = data.write();
        PoolVector<uint8_t>::Read r   = img->get_data().read();

        for (int i = 0; i <= new_img->get_mipmap_count(); i++) {
            int ofs, size, w, h;
            img->get_mipmap_offset_size_and_dimensions(i, ofs, size, w, h);
            Javelin::RgbaBitmap bm(w, h);
            void* dst = (void*)bm.GetData();
            memcpy(dst, &r[ofs], size);
            Javelin::ColorRgba<unsigned char>* dp = bm.GetData();
            for (int j = 0; j < size / 4; j++) {
                // Red and blue colors are swapped.
                SWAP(dp[j].r, dp[j].b);
            }
            new_img->get_mipmap_offset_size_and_dimensions(i, ofs, size, w, h);
            Javelin::PvrTcEncoder::EncodeRgba4Bpp(&wr[ofs], bm);
        }
    }

    p_img->create(
        new_img->get_width(),
        new_img->get_height(),
        new_img->has_mipmaps(),
        new_img->get_format(),
        data
    );
}

void _register_pvrtc_compress_func() {
    // FIXME: We claim to support PVRTC2 but use the same method as for PVRTC4.
    Image::_image_compress_pvrtc2_func = _compress_pvrtc4;
    Image::_image_compress_pvrtc4_func = _compress_pvrtc4;
}
