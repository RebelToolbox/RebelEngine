// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_loader_webp.h"

static ImageLoaderWEBP* image_loader_webp = nullptr;

void register_webp_types() {
    image_loader_webp = memnew(ImageLoaderWEBP);
    ImageLoader::add_image_format_loader(image_loader_webp);
}

void unregister_webp_types() {
    memdelete(image_loader_webp);
}
