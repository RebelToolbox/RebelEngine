// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#ifdef TOOLS_ENABLED

#include "image_compress_cvtt.h"

void register_cvtt_types() {
    Image::set_compress_bptc_func(image_compress_cvtt);
    Image::_image_decompress_bptc = image_decompress_cvtt;
}

void unregister_cvtt_types() {}

#endif
