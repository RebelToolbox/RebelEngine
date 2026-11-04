// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_IMPORT_PLUGIN_H
#define EDITOR_IMPORT_PLUGIN_H

#include "core/io/resource_importer.h"

class EditorImportPlugin : public ResourceImporter {
    GDCLASS(EditorImportPlugin, ResourceImporter);

protected:
    static void _bind_methods();

public:
    EditorImportPlugin();
    String get_importer_name() const override;
    String get_visible_name() const override;
    void get_recognized_extensions(List<String>* p_extensions) const override;
    String get_preset_name(int p_idx) const override;
    int get_preset_count() const override;
    String get_save_extension() const override;
    String get_resource_type() const override;
    float get_priority() const override;
    int get_import_order() const override;
    void get_import_options(
        List<ImportOption>* r_options,
        int p_preset
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
        List<String>* r_gen_files,
        Variant* r_metadata = nullptr
    ) override;
};

#endif // EDITOR_IMPORT_PLUGIN_H
