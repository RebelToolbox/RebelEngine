// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_IMPORTER_TEXTURE_ATLAS_H
#define RESOURCE_IMPORTER_TEXTURE_ATLAS_H

#include "core/image.h"
#include "core/io/resource_importer.h"

class ResourceImporterTextureAtlas : public ResourceImporter {
    GDCLASS(ResourceImporterTextureAtlas, ResourceImporter);

    struct PackData {
        Rect2 region;
        bool is_cropped;
        bool is_mesh;
        Vector<int> chart_pieces;               // one for region, many for mesh
        Vector<Vector<Vector2>> chart_vertices; // for mesh
        Ref<Image> image;
    };

public:
    enum ImportMode {
        IMPORT_MODE_REGION,
        IMPORT_MODE_2D_MESH
    };

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
    String get_option_group_file() const override;

    Error import(
        const String& p_source_file,
        const String& p_save_path,
        const Map<StringName, Variant>& p_options,
        List<String>* r_platform_variants,
        List<String>* r_gen_files = nullptr,
        Variant* r_metadata       = nullptr
    ) override;
    Error import_group_file(
        const String& p_group_file,
        const Map<String, Map<StringName, Variant>>& p_source_file_options,
        const Map<String, String>& p_base_paths
    ) override;

    ResourceImporterTextureAtlas();
};

#endif // RESOURCE_IMPORTER_TEXTURE_ATLAS_H
