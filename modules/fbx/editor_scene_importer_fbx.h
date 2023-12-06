// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_SCENE_IMPORTER_FBX_H
#define EDITOR_SCENE_IMPORTER_FBX_H

#ifdef TOOLS_ENABLED

#include "core/bind/core_bind.h"
#include "core/dictionary.h"
#include "core/io/resource_importer.h"
#include "core/local_vector.h"
#include "core/ustring.h"
#include "core/vector.h"
#include "data/import_state.h"
#include "editor/import/resource_importer_scene.h"
#include "editor/project_settings_editor.h"
#include "fbx_parser/FBXDocument.h"
#include "fbx_parser/FBXImportSettings.h"
#include "fbx_parser/FBXMeshGeometry.h"
#include "fbx_parser/FBXUtil.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/skeleton.h"
#include "scene/3d/spatial.h"
#include "scene/animation/animation_player.h"
#include "scene/resources/animation.h"
#include "scene/resources/surface_tool.h"
#include "tools/import_utils.h"

#define CONVERT_FBX_TIME(time) static_cast<double>(time) / 46186158000LL

class EditorSceneImporterFBX : public EditorSceneImporter {
private:
    GDCLASS(EditorSceneImporterFBX, EditorSceneImporter);

    struct AssetImportAnimation {
        enum Interpolation {
            INTERP_LINEAR,
            INTERP_STEP,
            INTERP_CATMULLROMSPLINE,
            INTERP_CUBIC_SPLINE
        };
    };

    // ------------------------------------------------------------------------------------------------
    template <typename T>
    const T* ProcessDOMConnection(
        const FBXDocParser::Document* doc,
        uint64_t current_element,
        bool reverse_lookup = false
    ) {
        const std::vector<const FBXDocParser::Connection*>& conns =
            reverse_lookup
                ? doc->GetConnectionsByDestinationSequenced(current_element)
                : doc->GetConnectionsBySourceSequenced(current_element);
        // print_verbose("[doc] looking for " + String(element_to_find));
        //  using the temp pattern here so we can debug before it returns
        //  in some cases we return too early, with 'deformer object base class'
        //  in wrong place in assimp this means we can accidentally return too
        //  early...
        const T* return_obj = nullptr;

        for (const FBXDocParser::Connection* con : conns) {
            const FBXDocParser::Object* source_object = con->SourceObject();
            const FBXDocParser::Object* dest_object = con->DestinationObject();
            if (source_object && dest_object != nullptr) {
                // print_verbose("[doc] connection name: " +
                // String(source_object->Name().c_str()) + ", dest: " +
                // String(dest_object->Name().c_str()));
                const T* temp = dynamic_cast<const T*>(
                    reverse_lookup ? source_object : dest_object
                );
                if (temp) {
                    return_obj = temp;
                }
            }
        }

        if (return_obj != nullptr) {
            // print_verbose("[doc] returned valid element");
            // print_verbose("Found object for bone");
            return return_obj;
        }

        // safe to return nothing, need to use nullptr here as nullptr is used
        // internally for FBX document.
        return nullptr;
    }

    void BuildDocumentBones(
        Ref<FBXBone> p_parent_bone,
        ImportState& state,
        const FBXDocParser::Document* p_doc,
        uint64_t p_id
    );

    void BuildDocumentNodes(
        Ref<PivotTransform> parent_transform,
        ImportState& state,
        const FBXDocParser::Document* doc,
        uint64_t id,
        Ref<FBXNode> fbx_parent
    );

    Spatial* _generate_scene(
        const String& p_path,
        const FBXDocParser::Document* p_document,
        const uint32_t p_flags,
        int p_bake_fps,
        const uint32_t p_compress_flags,
        const int32_t p_max_bone_weights,
        bool p_is_blender_fbx
    );

    template <class T>
    T _interpolate_track(
        const Vector<float>& p_times,
        const Vector<T>& p_values,
        float p_time,
        AssetImportAnimation::Interpolation p_interp
    );
    void _register_project_setting_import(
        const String generic,
        const String import_setting_string,
        const Vector<String>& exts,
        List<String>* r_extensions,
        const bool p_enabled
    ) const;

public:
    EditorSceneImporterFBX() {}

    ~EditorSceneImporterFBX() {}

    virtual void get_extensions(List<String>* r_extensions) const;
    virtual uint32_t get_import_flags() const;
    virtual Node* import_scene(
        const String& p_path,
        uint32_t p_flags,
        int p_bake_fps,
        uint32_t p_compress_flags,
        List<String>* r_missing_deps,
        Error* r_err = nullptr
    );
    void create_mesh_data_skin(
        ImportState& state,
        const Ref<FBXNode>& fbx_node,
        uint64_t mesh_id
    );
};

#endif // TOOLS_ENABLED
#endif // EDITOR_SCENE_IMPORTER_FBX_H
