// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IMAGE_COMPRESS_SQUISH_H
#define IMAGE_COMPRESS_SQUISH_H

#include "core/image.h"

void image_compress_squish(
    Image* p_image,
    float p_lossy_quality,
    Image::CompressSource p_source
);
void image_decompress_squish(Image* p_image);

#endif // IMAGE_COMPRESS_SQUISH_H
