// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_driver_types.h"

#include "drivers/png/image_loader_png.h"
#include "drivers/png/resource_saver_png.h"

static ImageLoaderPNG* image_loader_png;
static Ref<ResourceSaverPNG> resource_saver_png;

void register_core_driver_types() {
    image_loader_png = memnew(ImageLoaderPNG);
    ImageLoader::add_image_format_loader(image_loader_png);

    resource_saver_png.instance();
    ResourceSaver::add_resource_format_saver(resource_saver_png);
}

void unregister_core_driver_types() {
    if (image_loader_png) {
        memdelete(image_loader_png);
    }

    ResourceSaver::remove_resource_format_saver(resource_saver_png);
    resource_saver_png.unref();
}

void register_driver_types() {}

void unregister_driver_types() {}
