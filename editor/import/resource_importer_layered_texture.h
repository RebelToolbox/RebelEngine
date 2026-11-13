// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_IMPORTER_LAYERED_TEXTURE_H
#define RESOURCE_IMPORTER_LAYERED_TEXTURE_H

#include "core/image.h"
#include "core/io/resource_importer.h"

class StreamTexture;

class ResourceImporterLayeredTexture : public ResourceImporter {
    GDCLASS(ResourceImporterLayeredTexture, ResourceImporter);

    bool is_3d;
    static const char* compression_formats[];

protected:
    static ResourceImporterLayeredTexture* singleton;

public:
    static ResourceImporterLayeredTexture* get_singleton() {
        return singleton;
    }

    String get_importer_name() const override;
    String get_visible_name() const override;
    void get_recognized_extensions(List<String>* p_extensions) const override;
    String get_save_extension() const override;
    String get_resource_type() const override;

    enum Preset {
        PRESET_3D,
        PRESET_2D,
        PRESET_COLOR_CORRECT,
    };

    enum CompressMode {
        COMPRESS_LOSSLESS,
        COMPRESS_VIDEO_RAM,
        COMPRESS_UNCOMPRESSED
    };

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

    void _save_tex(
        const Vector<Ref<Image>>& p_images,
        const String& p_to_path,
        int p_compress_mode,
        Image::CompressMode p_vram_compression,
        bool p_mipmaps,
        int p_texture_flags
    );

    Error import(
        const String& p_source_file,
        const String& p_save_path,
        const Map<StringName, Variant>& p_options,
        List<String>* r_platform_variants,
        List<String>* r_gen_files = nullptr,
        Variant* r_metadata       = nullptr
    ) override;

    bool are_import_settings_valid(const String& p_path) const override;
    String get_import_settings_string() const override;

    void set_3d(bool p_3d) {
        is_3d = p_3d;
    }

    ResourceImporterLayeredTexture();
    ~ResourceImporterLayeredTexture() override;
};
#endif // RESOURCE_IMPORTER_LAYERED_TEXTURE_H
