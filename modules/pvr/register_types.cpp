// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "image_compress_pvrtc.h"
#include "texture_loader_pvr.h"

static Ref<ResourceFormatPVR> resource_loader_pvr;

void register_pvr_types() {
    resource_loader_pvr.instance();
    ResourceLoader::add_resource_format_loader(resource_loader_pvr);

    _register_pvrtc_compress_func();
}

void unregister_pvr_types() {
    ResourceLoader::remove_resource_format_loader(resource_loader_pvr);
    resource_loader_pvr.unref();
}
