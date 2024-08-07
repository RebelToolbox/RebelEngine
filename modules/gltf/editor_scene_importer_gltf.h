// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_SCENE_IMPORTER_GLTF_H
#define EDITOR_SCENE_IMPORTER_GLTF_H

#include "core/io/json.h"
#include "core/object.h"
#include "core/project_settings.h"
#include "core/vector.h"
#include "editor/import/resource_importer_scene.h"
#include "gltf_document.h"
#include "gltf_state.h"
#include "modules/csg/csg_shape.h"
#include "modules/gridmap/grid_map.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/multimesh_instance.h"
#include "scene/3d/skeleton.h"
#include "scene/3d/spatial.h"
#include "scene/animation/animation_player.h"
#include "scene/gui/check_box.h"
#include "scene/main/node.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/surface_tool.h"

class AnimationPlayer;
class BoneAttachment;
class EditorSceneImporterMeshNode3D;

#ifdef TOOLS_ENABLED
class EditorSceneImporterGLTF : public EditorSceneImporter {
    GDCLASS(EditorSceneImporterGLTF, EditorSceneImporter);

public:
    virtual uint32_t get_import_flags() const;
    virtual void get_extensions(List<String>* r_extensions) const;
    virtual Node* import_scene(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps,
        uint32_t p_compress_flags,
        List<String>* r_missing_deps = NULL,
        Error* r_err                 = NULL
    );
    virtual Ref<Animation> import_animation(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps
    );
};
#endif

class PackedSceneGLTF : public PackedScene {
    GDCLASS(PackedSceneGLTF, PackedScene);

protected:
    static void _bind_methods();

public:
    virtual void save_scene(
        Node* p_node,
        const String& p_path,
        const String& p_src_path,
        uint32_t p_flags,
        int p_bake_fps,
        List<String>* r_missing_deps,
        Error* r_err = NULL
    );
    virtual void _build_parent_hierachy(Ref<GLTFState> state);
    virtual Error export_gltf(
        Node* p_root,
        String p_path,
        int32_t p_flags   = 0,
        real_t p_bake_fps = 1000.0f
    );
    virtual Node* import_scene(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps,
        uint32_t p_compress_flags,
        List<String>* r_missing_deps,
        Error* r_err,
        Ref<GLTFState> r_state
    );
    virtual Node* import_gltf_scene(
        const String& p_path,
        uint32_t p_flags,
        float p_bake_fps,
        uint32_t p_compress_flags,
        Ref<GLTFState> r_state = Ref<GLTFState>()
    );
    virtual void pack_gltf(
        String p_path,
        int32_t p_flags           = 0,
        real_t p_bake_fps         = 1000.0f,
        uint32_t p_compress_flags = Mesh::ARRAY_COMPRESS_DEFAULT,
        Ref<GLTFState> r_state    = Ref<GLTFState>()
    );
};
#endif // EDITOR_SCENE_IMPORTER_GLTF_H
