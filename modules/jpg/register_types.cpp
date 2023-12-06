// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_loader_jpegd.h"

static ImageLoaderJPG* image_loader_jpg = nullptr;

void register_jpg_types() {
    image_loader_jpg = memnew(ImageLoaderJPG);
    ImageLoader::add_image_format_loader(image_loader_jpg);
}

void unregister_jpg_types() {
    memdelete(image_loader_jpg);
}
