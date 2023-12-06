// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_IMPORT_COLLADA_H
#define EDITOR_IMPORT_COLLADA_H

#include "editor/import/resource_importer_scene.h"

class EditorSceneImporterCollada : public EditorSceneImporter {
    GDCLASS(EditorSceneImporterCollada, EditorSceneImporter);

public:
    virtual uint32_t get_import_flags() const;
    virtual void get_extensions(List<String>* r_extensions) const;
    virtual Node* import_scene(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps,
        uint32_t p_compress_flags,
        List<String>* r_missing_deps = nullptr,
        Error* r_err                 = nullptr
    );
    virtual Ref<Animation> import_animation(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps
    );

    EditorSceneImporterCollada();
};

#endif
