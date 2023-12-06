// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MESH_DATA_TOOL_H
#define MESH_DATA_TOOL_H

#include "scene/resources/mesh.h"

class MeshDataTool : public Reference {
    GDCLASS(MeshDataTool, Reference);

    int format;

    struct Vertex {
        Vector3 vertex;
        Color color;
        Vector3 normal; // normal, binormal, tangent
        Plane tangent;
        Vector2 uv;
        Vector2 uv2;
        Vector<int> bones;
        Vector<float> weights;
        Vector<int> edges;
        Vector<int> faces;
        Variant meta;
    };

    Vector<Vertex> vertices;

    struct Edge {
        int vertex[2];
        Vector<int> faces;
        Variant meta;
    };

    Vector<Edge> edges;

    struct Face {
        int v[3];
        int edges[3];
        Variant meta;
    };

    Vector<Face> faces;

    Ref<Material> material;

protected:
    static void _bind_methods();

public:
    void clear();
    Error create_from_surface(const Ref<ArrayMesh>& p_mesh, int p_surface);
    Error commit_to_surface(const Ref<ArrayMesh>& p_mesh);

    int get_format() const;

    int get_vertex_count() const;
    int get_edge_count() const;
    int get_face_count() const;

    Vector3 get_vertex(int p_idx) const;
    void set_vertex(int p_idx, const Vector3& p_vertex);

    Vector3 get_vertex_normal(int p_idx) const;
    void set_vertex_normal(int p_idx, const Vector3& p_normal);

    Plane get_vertex_tangent(int p_idx) const;
    void set_vertex_tangent(int p_idx, const Plane& p_tangent);

    Vector2 get_vertex_uv(int p_idx) const;
    void set_vertex_uv(int p_idx, const Vector2& p_uv);

    Vector2 get_vertex_uv2(int p_idx) const;
    void set_vertex_uv2(int p_idx, const Vector2& p_uv2);

    Color get_vertex_color(int p_idx) const;
    void set_vertex_color(int p_idx, const Color& p_color);

    Vector<int> get_vertex_bones(int p_idx) const;
    void set_vertex_bones(int p_idx, const Vector<int>& p_bones);

    Vector<float> get_vertex_weights(int p_idx) const;
    void set_vertex_weights(int p_idx, const Vector<float>& p_weights);

    Variant get_vertex_meta(int p_idx) const;
    void set_vertex_meta(int p_idx, const Variant& p_meta);

    Vector<int> get_vertex_edges(int p_idx) const;
    Vector<int> get_vertex_faces(int p_idx) const;

    int get_edge_vertex(int p_edge, int p_vertex) const;
    Vector<int> get_edge_faces(int p_edge) const;
    Variant get_edge_meta(int p_idx) const;
    void set_edge_meta(int p_idx, const Variant& p_meta);

    int get_face_vertex(int p_face, int p_vertex) const;
    int get_face_edge(int p_face, int p_vertex) const;
    Variant get_face_meta(int p_face) const;
    void set_face_meta(int p_face, const Variant& p_meta);
    Vector3 get_face_normal(int p_face) const;

    Ref<Material> get_material() const;
    void set_material(const Ref<Material>& p_material);

    MeshDataTool();
};

#endif // MESH_DATA_TOOL_H
