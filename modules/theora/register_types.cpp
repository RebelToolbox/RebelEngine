// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "video_stream_theora.h"

static Ref<ResourceFormatLoaderTheora> resource_loader_theora;

void register_theora_types() {
    resource_loader_theora.instance();
    ResourceLoader::add_resource_format_loader(resource_loader_theora, true);

    ClassDB::register_class<VideoStreamTheora>();
}

void unregister_theora_types() {
    ResourceLoader::remove_resource_format_loader(resource_loader_theora);
    resource_loader_theora.unref();
}
