// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_loader_hdr.h"

static ImageLoaderHDR* image_loader_hdr = nullptr;

void register_hdr_types() {
    image_loader_hdr = memnew(ImageLoaderHDR);
    ImageLoader::add_image_format_loader(image_loader_hdr);
}

void unregister_hdr_types() {
    memdelete(image_loader_hdr);
}
