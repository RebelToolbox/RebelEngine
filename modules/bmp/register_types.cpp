// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_loader_bmp.h"

static ImageLoaderBMP* image_loader_bmp = nullptr;

void register_bmp_types() {
    image_loader_bmp = memnew(ImageLoaderBMP);
    ImageLoader::add_image_format_loader(image_loader_bmp);
}

void unregister_bmp_types() {
    memdelete(image_loader_bmp);
}
