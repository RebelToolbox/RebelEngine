// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IMAGE_LOADER_SVG_H
#define IMAGE_LOADER_SVG_H

#include "core/io/image_loader.h"
#include "core/ustring.h"

/**
    @author Daniel Ramirez <djrmuv@gmail.com>
*/

// Forward declare and include third-party headers in .cpp.
struct NSVGrasterizer;
struct NSVGimage;

class SVGRasterizer {
    NSVGrasterizer* rasterizer;

public:
    void rasterize(
        NSVGimage* p_image,
        float p_tx,
        float p_ty,
        float p_scale,
        unsigned char* p_dst,
        int p_w,
        int p_h,
        int p_stride
    );

    SVGRasterizer();
    ~SVGRasterizer();
};

class ImageLoaderSVG : public ImageFormatLoader {
    static struct ReplaceColors {
        List<uint32_t> old_colors;
        List<uint32_t> new_colors;
    } replace_colors;

    static SVGRasterizer rasterizer;
    static void _convert_colors(NSVGimage* p_svg_image);
    static Error _create_image(
        Ref<Image> p_image,
        const PoolVector<uint8_t>* p_data,
        float p_scale,
        bool upsample,
        bool convert_colors = false
    );

public:
    static void set_convert_colors(Dictionary* p_replace_color = nullptr);
    static Error create_image_from_string(
        Ref<Image> p_image,
        const char* p_svg_str,
        float p_scale,
        bool upsample,
        bool convert_colors = false
    );

    virtual Error load_image(
        Ref<Image> p_image,
        FileAccess* f,
        bool p_force_linear,
        float p_scale
    );
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    ImageLoaderSVG();
};

#endif
