// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "audio_stream_ogg_vorbis.h"

#ifdef TOOLS_ENABLED
#include "core/engine.h"
#include "resource_importer_ogg_vorbis.h"
#endif

void register_stb_vorbis_types() {
#ifdef TOOLS_ENABLED
    if (Engine::get_singleton()->is_editor_hint()) {
        Ref<ResourceImporterOGGVorbis> ogg_import;
        ogg_import.instance();
        ResourceFormatImporter::get_singleton()->add_importer(ogg_import);
    }
#endif
    ClassDB::register_class<AudioStreamOGGVorbis>();
}

void unregister_stb_vorbis_types() {}
