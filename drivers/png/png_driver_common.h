// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PNG_DRIVER_COMMON_H
#define PNG_DRIVER_COMMON_H

#include "core/image.h"
#include "core/pool_vector.h"

namespace PNGDriverCommon {

// Attempt to load png from buffer (p_source, p_size) into p_image
Error png_to_image(
    const uint8_t* p_source,
    size_t p_size,
    bool p_force_linear,
    Ref<Image> p_image
);

// Append p_image, as a png, to p_buffer.
// Contents of p_buffer is unspecified if error returned.
Error image_to_png(const Ref<Image>& p_image, PoolVector<uint8_t>& p_buffer);

} // namespace PNGDriverCommon

#endif
