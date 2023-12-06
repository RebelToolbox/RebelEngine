// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_compress_squish.h"

void register_squish_types() {
    Image::set_compress_bc_func(image_compress_squish);
    Image::_image_decompress_bc = image_decompress_squish;
}

void unregister_squish_types() {}
