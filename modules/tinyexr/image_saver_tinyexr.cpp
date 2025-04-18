// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "image_saver_tinyexr.h"

#include "core/math/math_funcs.h"
#include "thirdparty/tinyexr/tinyexr.h"

#include <cstdlib>

static bool is_supported_format(Image::Format p_format) {
    // This is checked before anything else.
    // Mostly uncompressed formats are considered.
    switch (p_format) {
        case Image::FORMAT_RF:
        case Image::FORMAT_RGF:
        case Image::FORMAT_RGBF:
        case Image::FORMAT_RGBAF:
        case Image::FORMAT_RH:
        case Image::FORMAT_RGH:
        case Image::FORMAT_RGBH:
        case Image::FORMAT_RGBAH:
        case Image::FORMAT_R8:
        case Image::FORMAT_RG8:
        case Image::FORMAT_RGB8:
        case Image::FORMAT_RGBA8:
            return true;
        default:
            return false;
    }
}

enum SrcPixelType {
    SRC_FLOAT,
    SRC_HALF,
    SRC_BYTE,
    SRC_UNSUPPORTED
};

static SrcPixelType get_source_pixel_type(Image::Format p_format) {
    switch (p_format) {
        case Image::FORMAT_RF:
        case Image::FORMAT_RGF:
        case Image::FORMAT_RGBF:
        case Image::FORMAT_RGBAF:
            return SRC_FLOAT;
        case Image::FORMAT_RH:
        case Image::FORMAT_RGH:
        case Image::FORMAT_RGBH:
        case Image::FORMAT_RGBAH:
            return SRC_HALF;
        case Image::FORMAT_R8:
        case Image::FORMAT_RG8:
        case Image::FORMAT_RGB8:
        case Image::FORMAT_RGBA8:
            return SRC_BYTE;
        default:
            return SRC_UNSUPPORTED;
    }
}

static int get_target_pixel_type(Image::Format p_format) {
    switch (p_format) {
        case Image::FORMAT_RF:
        case Image::FORMAT_RGF:
        case Image::FORMAT_RGBF:
        case Image::FORMAT_RGBAF:
            return TINYEXR_PIXELTYPE_FLOAT;
        case Image::FORMAT_RH:
        case Image::FORMAT_RGH:
        case Image::FORMAT_RGBH:
        case Image::FORMAT_RGBAH:
        // EXR doesn't support 8-bit channels so in that case we'll convert
        case Image::FORMAT_R8:
        case Image::FORMAT_RG8:
        case Image::FORMAT_RGB8:
        case Image::FORMAT_RGBA8:
            return TINYEXR_PIXELTYPE_HALF;
        default:
            return -1;
    }
}

static int get_pixel_type_size(int p_pixel_type) {
    switch (p_pixel_type) {
        case TINYEXR_PIXELTYPE_HALF:
            return 2;
        case TINYEXR_PIXELTYPE_FLOAT:
            return 4;
    }
    return -1;
}

static int get_channel_count(Image::Format p_format) {
    switch (p_format) {
        case Image::FORMAT_RF:
        case Image::FORMAT_RH:
        case Image::FORMAT_R8:
            return 1;
        case Image::FORMAT_RGF:
        case Image::FORMAT_RGH:
        case Image::FORMAT_RG8:
            return 2;
        case Image::FORMAT_RGBF:
        case Image::FORMAT_RGBH:
        case Image::FORMAT_RGB8:
            return 3;
        case Image::FORMAT_RGBAF:
        case Image::FORMAT_RGBAH:
        case Image::FORMAT_RGBA8:
            return 4;
        default:
            return -1;
    }
}

Error save_exr(
    const String& p_path,
    const Ref<Image>& p_img,
    bool p_grayscale
) {
    Image::Format format = p_img->get_format();

    if (!is_supported_format(format)) {
        // Format not supported
        print_error(
            "Image format not supported for saving as EXR. Consider saving as "
            "PNG."
        );
        return ERR_UNAVAILABLE;
    }

    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    // To store header information on the stack, the maximum number of channels
    // is hard-coded to 4 and the information is limited to the subset needed.
    const int max_channels = 4;
    PoolByteArray channels[max_channels];
    unsigned char* channels_ptrs[max_channels];
    EXRChannelInfo channel_infos[max_channels];
    int pixel_types[max_channels];
    int requested_pixel_types[max_channels] = {-1};

    // Gimp and Blender are a bit annoying so order of channels isn't
    // straightforward.
    const int channel_mappings[4][4] = {
        {0}, // R
        {1, 0}, // GR
        {2, 1, 0}, // BGR
        {3, 2, 1, 0}  // ABGR
    };

    int channel_count = get_channel_count(format);
    ERR_FAIL_COND_V(channel_count < 0, ERR_UNAVAILABLE);
    ERR_FAIL_COND_V(p_grayscale && channel_count != 1, ERR_INVALID_PARAMETER);

    int target_pixel_type = get_target_pixel_type(format);
    ERR_FAIL_COND_V(target_pixel_type < 0, ERR_UNAVAILABLE);
    int target_pixel_type_size = get_pixel_type_size(target_pixel_type);
    ERR_FAIL_COND_V(target_pixel_type_size < 0, ERR_UNAVAILABLE);
    SrcPixelType src_pixel_type = get_source_pixel_type(format);
    ERR_FAIL_COND_V(src_pixel_type == SRC_UNSUPPORTED, ERR_UNAVAILABLE);
    const int pixel_count = p_img->get_width() * p_img->get_height();

    const int* channel_mapping = channel_mappings[channel_count - 1];

    {
        PoolByteArray src_data    = p_img->get_data();
        PoolByteArray::Read src_r = src_data.read();

        for (int channel_index = 0; channel_index < channel_count;
             ++channel_index) {
            // De-interleave channels

            PoolByteArray& dst = channels[channel_index];
            dst.resize(pixel_count * target_pixel_type_size);

            PoolByteArray::Write dst_w = dst.write();

            if (src_pixel_type == SRC_FLOAT
                && target_pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
                // Note: we don't save mipmaps
                CRASH_COND(
                    src_data.size()
                    < pixel_count * channel_count * target_pixel_type_size
                );

                const float* src_rp = (float*)src_r.ptr();
                float* dst_wp       = (float*)dst_w.ptr();

                for (int i = 0; i < pixel_count; ++i) {
                    dst_wp[i] = src_rp[channel_index + i * channel_count];
                }

            } else if (src_pixel_type == SRC_HALF && target_pixel_type == TINYEXR_PIXELTYPE_HALF) {
                CRASH_COND(
                    src_data.size()
                    < pixel_count * channel_count * target_pixel_type_size
                );

                const uint16_t* src_rp = (uint16_t*)src_r.ptr();
                uint16_t* dst_wp       = (uint16_t*)dst_w.ptr();

                for (int i = 0; i < pixel_count; ++i) {
                    dst_wp[i] = src_rp[channel_index + i * channel_count];
                }

            } else if (src_pixel_type == SRC_BYTE && target_pixel_type == TINYEXR_PIXELTYPE_HALF) {
                CRASH_COND(src_data.size() < pixel_count * channel_count);

                const uint8_t* src_rp = (uint8_t*)src_r.ptr();
                uint16_t* dst_wp      = (uint16_t*)dst_w.ptr();

                for (int i = 0; i < pixel_count; ++i) {
                    dst_wp[i] = Math::make_half_float(
                        src_rp[channel_index + i * channel_count] / 255.f
                    );
                }

            } else {
                CRASH_NOW();
            }

            int remapped_index = channel_mapping[channel_index];

            channels_ptrs[remapped_index] = dst_w.ptr();

            // No conversion
            pixel_types[remapped_index]           = target_pixel_type;
            requested_pixel_types[remapped_index] = target_pixel_type;

            // Write channel name
            if (p_grayscale) {
                channel_infos[remapped_index].name[0] = 'Y';
                channel_infos[remapped_index].name[1] = '\0';
            } else {
                const char* rgba                      = "RGBA";
                channel_infos[remapped_index].name[0] = rgba[channel_index];
                channel_infos[remapped_index].name[1] = '\0';
            }
        }
    }

    image.images       = channels_ptrs;
    image.num_channels = channel_count;
    image.width        = p_img->get_width();
    image.height       = p_img->get_height();

    header.num_channels          = image.num_channels;
    header.channels              = channel_infos;
    header.pixel_types           = pixel_types;
    header.requested_pixel_types = requested_pixel_types;
    header.compression_type      = TINYEXR_COMPRESSIONTYPE_PIZ;

    unsigned char* mem = nullptr;
    const char* err    = nullptr;

    size_t bytes = SaveEXRImageToMemory(&image, &header, &mem, &err);

    if (bytes == 0) {
        print_error(String("Saving EXR failed. Error: {0}").format(varray(err))
        );
        return ERR_FILE_CANT_WRITE;
    } else {
        FileAccessRef ref = FileAccess::open(p_path, FileAccess::WRITE);
        ERR_FAIL_COND_V(!ref, ERR_FILE_CANT_WRITE);
        ref->store_buffer(mem, bytes);
        free(mem);
    }

    return OK;
}
