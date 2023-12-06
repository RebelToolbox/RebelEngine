// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_IMPORTER_BITMASK_H
#define RESOURCE_IMPORTER_BITMASK_H

#include "core/image.h"
#include "core/io/resource_importer.h"

class StreamBitMap;

class ResourceImporterBitMap : public ResourceImporter {
    GDCLASS(ResourceImporterBitMap, ResourceImporter);

public:
    virtual String get_importer_name() const;
    virtual String get_visible_name() const;
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual String get_save_extension() const;
    virtual String get_resource_type() const;

    virtual int get_preset_count() const;
    virtual String get_preset_name(int p_idx) const;

    virtual void get_import_options(
        List<ImportOption>* r_options,
        int p_preset = 0
    ) const;
    virtual bool get_option_visibility(
        const String& p_option,
        const Map<StringName, Variant>& p_options
    ) const;
    virtual Error import(
        const String& p_source_file,
        const String& p_save_path,
        const Map<StringName, Variant>& p_options,
        List<String>* r_platform_variants,
        List<String>* r_gen_files = nullptr,
        Variant* r_metadata       = nullptr
    );

    ResourceImporterBitMap();
    ~ResourceImporterBitMap();
};
#endif // RESOURCE_IMPORTER_BITMASK_H
