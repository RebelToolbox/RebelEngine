// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_IMPORTER_OBJ_H
#define RESOURCE_IMPORTER_OBJ_H

#include "resource_importer_scene.h"

class EditorOBJImporter : public EditorSceneImporter {
    GDCLASS(EditorOBJImporter, EditorSceneImporter);

public:
    uint32_t get_import_flags() const override;
    void get_extensions(List<String>* r_extensions) const override;
    Node* import_scene(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps,
        uint32_t p_compress_flags,
        List<String>* r_missing_deps,
        Error* r_err = nullptr
    ) override;
    Ref<Animation> import_animation(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps
    ) override;

    EditorOBJImporter();
};

class ResourceImporterOBJ : public ResourceImporter {
    GDCLASS(ResourceImporterOBJ, ResourceImporter);

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

    ResourceImporterOBJ();
};

#endif // RESOURCE_IMPORTER_OBJ_H
