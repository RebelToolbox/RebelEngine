// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/class_db.h"
#include "video_stream_gdnative.h"

static Ref<ResourceFormatLoaderVideoStreamGDNative> resource_loader_vsgdnative;

void register_videodecoder_types() {
    resource_loader_vsgdnative.instance();
    ResourceLoader::add_resource_format_loader(
        resource_loader_vsgdnative,
        true
    );

    ClassDB::register_class<VideoStreamGDNative>();
}

void unregister_videodecoder_types() {
    ResourceLoader::remove_resource_format_loader(resource_loader_vsgdnative);
    resource_loader_vsgdnative.unref();
}
