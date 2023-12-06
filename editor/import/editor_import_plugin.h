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
    virtual String get_importer_name() const;
    virtual String get_visible_name() const;
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual String get_preset_name(int p_idx) const;
    virtual int get_preset_count() const;
    virtual String get_save_extension() const;
    virtual String get_resource_type() const;
    virtual float get_priority() const;
    virtual int get_import_order() const;
    virtual void get_import_options(List<ImportOption>* r_options, int p_preset)
        const;
    virtual bool get_option_visibility(
        const String& p_option,
        const Map<StringName, Variant>& p_options
    ) const;
    virtual Error import(
        const String& p_source_file,
        const String& p_save_path,
        const Map<StringName, Variant>& p_options,
        List<String>* r_platform_variants,
        List<String>* r_gen_files,
        Variant* r_metadata = nullptr
    );
};

#endif // EDITOR_IMPORT_PLUGIN_H
