// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_IMPORTER_OGG_VORBIS_H
#define RESOURCE_IMPORTER_OGG_VORBIS_H

#include "audio_stream_ogg_vorbis.h"
#include "core/io/resource_importer.h"

class ResourceImporterOGGVorbis : public ResourceImporter {
    GDCLASS(ResourceImporterOGGVorbis, ResourceImporter);

public:
    String get_importer_name() const override;
    String get_visible_name() const override;
    void get_recognized_extensions(List<String>* p_extensions) const override;
    String get_save_extension() const override;
    String get_resource_type() const override;

    int get_preset_count() const override;
    String get_preset_name(int p_idx) const override;

    void get_import_options(
        List<ImportOption>* r_options,
        int p_preset = 0
    ) const override;
    bool get_option_visibility(
        const String& p_option,
        const Map<StringName, Variant>& p_options
    ) const override;

    Error import(
        const String& p_source_file,
        const String& p_save_path,
        const Map<StringName, Variant>& p_options,
        List<String>* r_platform_variants,
        List<String>* r_gen_files = nullptr,
        Variant* r_metadata       = nullptr
    ) override;

    ResourceImporterOGGVorbis();
};

#endif // RESOURCE_IMPORTER_OGG_VORBIS_H
