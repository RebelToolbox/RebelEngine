// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_loader_tga.h"

static ImageLoaderTGA* image_loader_tga = nullptr;

void register_tga_types() {
    image_loader_tga = memnew(ImageLoaderTGA);
    ImageLoader::add_image_format_loader(image_loader_tga);
}

void unregister_tga_types() {
    memdelete(image_loader_tga);
}
