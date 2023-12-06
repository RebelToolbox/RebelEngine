// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NAVIGATION_MESH_GENERATOR_H
#define NAVIGATION_MESH_GENERATOR_H

#include "editor/editor_node.h"
#include "scene/3d/navigation_mesh.h"

#include <Recast.h>

class EditorNavigationMeshGenerator : public Object {
    GDCLASS(EditorNavigationMeshGenerator, Object);

    static EditorNavigationMeshGenerator* singleton;

protected:
    static void _bind_methods();

    static void _add_vertex(const Vector3& p_vec3, Vector<float>& p_verticies);
    static void _add_mesh(
        const Ref<Mesh>& p_mesh,
        const Transform& p_xform,
        Vector<float>& p_verticies,
        Vector<int>& p_indices
    );
    static void _add_faces(
        const PoolVector3Array& p_faces,
        const Transform& p_xform,
        Vector<float>& p_verticies,
        Vector<int>& p_indices
    );
    static void _parse_geometry(
        Transform p_accumulated_transform,
        Node* p_node,
        Vector<float>& p_verticies,
        Vector<int>& p_indices,
        NavigationMesh::ParsedGeometryType p_generate_from,
        uint32_t p_collision_mask,
        bool p_recurse_children
    );

    static void _convert_detail_mesh_to_native_navigation_mesh(
        const rcPolyMeshDetail* p_detail_mesh,
        Ref<NavigationMesh> p_nav_mesh
    );
    static void _build_recast_navigation_mesh(
        Ref<NavigationMesh> p_nav_mesh,
        EditorProgress* ep,
        rcHeightfield* hf,
        rcCompactHeightfield* chf,
        rcContourSet* cset,
        rcPolyMesh* poly_mesh,
        rcPolyMeshDetail* detail_mesh,
        Vector<float>& vertices,
        Vector<int>& indices
    );

public:
    static EditorNavigationMeshGenerator* get_singleton();

    EditorNavigationMeshGenerator();
    ~EditorNavigationMeshGenerator();

    void bake(Ref<NavigationMesh> p_nav_mesh, Node* p_node);
    void clear(Ref<NavigationMesh> p_nav_mesh);
};

#endif // NAVIGATION_MESH_GENERATOR_H
