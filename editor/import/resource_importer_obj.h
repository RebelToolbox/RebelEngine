// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCEIMPORTEROBJ_H
#define RESOURCEIMPORTEROBJ_H

#include "resource_importer_scene.h"

class EditorOBJImporter : public EditorSceneImporter {
    GDCLASS(EditorOBJImporter, EditorSceneImporter);

public:
    virtual uint32_t get_import_flags() const;
    virtual void get_extensions(List<String>* r_extensions) const;
    virtual Node* import_scene(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps,
        uint32_t p_compress_flags,
        List<String>* r_missing_deps,
        Error* r_err = nullptr
    );
    virtual Ref<Animation> import_animation(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps
    );

    EditorOBJImporter();
};

class ResourceImporterOBJ : public ResourceImporter {
    GDCLASS(ResourceImporterOBJ, ResourceImporter);

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

    ResourceImporterOBJ();
};

#endif // RESOURCEIMPORTEROBJ_H
