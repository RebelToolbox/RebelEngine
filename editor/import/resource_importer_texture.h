// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_IMPORTER_TEXTURE_H
#define RESOURCE_IMPORTER_TEXTURE_H

#include "core/image.h"
#include "core/io/resource_importer.h"

class StreamTexture;

class ResourceImporterTexture : public ResourceImporter {
    GDCLASS(ResourceImporterTexture, ResourceImporter);

protected:
    enum {
        MAKE_3D_FLAG     = 1,
        MAKE_SRGB_FLAG   = 2,
        MAKE_NORMAL_FLAG = 4
    };

    Mutex mutex;
    Map<StringName, int> make_flags;

    static void _texture_reimport_srgb(const Ref<StreamTexture>& p_tex);
    static void _texture_reimport_3d(const Ref<StreamTexture>& p_tex);
    static void _texture_reimport_normal(const Ref<StreamTexture>& p_tex);

    static ResourceImporterTexture* singleton;
    static const char* compression_formats[];

public:
    static ResourceImporterTexture* get_singleton() {
        return singleton;
    }

    String get_importer_name() const override;
    String get_visible_name() const override;
    void get_recognized_extensions(List<String>* p_extensions) const override;
    String get_save_extension() const override;
    String get_resource_type() const override;

    enum Preset {
        PRESET_DETECT,
        PRESET_2D,
        PRESET_2D_PIXEL,
        PRESET_3D,
    };

    enum CompressMode {
        COMPRESS_LOSSLESS,
        COMPRESS_LOSSY,
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

    void _save_stex(
        const Ref<Image>& p_image,
        const String& p_to_path,
        int p_compress_mode,
        float p_lossy_quality,
        Image::CompressMode p_vram_compression,
        bool p_mipmaps,
        int p_texture_flags,
        bool p_streamable,
        bool p_detect_3d,
        bool p_detect_srgb,
        bool p_force_rgbe,
        bool p_detect_normal,
        bool p_force_normal,
        bool p_force_po2_for_compressed
    );

    Error import(
        const String& p_source_file,
        const String& p_save_path,
        const Map<StringName, Variant>& p_options,
        List<String>* r_platform_variants,
        List<String>* r_gen_files = nullptr,
        Variant* r_metadata       = nullptr
    ) override;

    void update_imports();

    bool are_import_settings_valid(const String& p_path) const override;
    String get_import_settings_string() const override;

    ResourceImporterTexture();
};

#endif // RESOURCE_IMPORTER_TEXTURE_H
