// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "audio_stream_mp3.h"

#ifdef TOOLS_ENABLED
#include "core/engine.h"
#include "resource_importer_mp3.h"
#endif

void register_minimp3_types() {
#ifdef TOOLS_ENABLED
    if (Engine::get_singleton()->is_editor_hint()) {
        Ref<ResourceImporterMP3> mp3_import;
        mp3_import.instance();
        ResourceFormatImporter::get_singleton()->add_importer(mp3_import);
    }
#endif
    ClassDB::register_class<AudioStreamMP3>();
}

void unregister_minimp3_types() {}
