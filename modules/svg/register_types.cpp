// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_loader_svg.h"

static ImageLoaderSVG* image_loader_svg = nullptr;

void register_svg_types() {
    image_loader_svg = memnew(ImageLoaderSVG);
    ImageLoader::add_image_format_loader(image_loader_svg);
}

void unregister_svg_types() {
    memdelete(image_loader_svg);
}
