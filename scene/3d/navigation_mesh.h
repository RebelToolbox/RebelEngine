// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NAVIGATION_MESH_H
#define NAVIGATION_MESH_H

#include "scene/3d/spatial.h"
#include "scene/resources/mesh.h"

class Mesh;

class NavigationMesh : public Resource {
    GDCLASS(NavigationMesh, Resource);

    PoolVector<Vector3> vertices;

    struct Polygon {
        Vector<int> indices;
    };

    Vector<Polygon> polygons;
    Ref<ArrayMesh> debug_mesh;

    struct _EdgeKey {
        Vector3 from;
        Vector3 to;

        bool operator<(const _EdgeKey& p_with) const {
            return from == p_with.from ? to < p_with.to : from < p_with.from;
        }
    };

protected:
    static void _bind_methods();
    virtual void _validate_property(PropertyInfo& property) const;

    void _set_polygons(const Array& p_array);
    Array _get_polygons() const;

public:
    enum SamplePartitionType {
        SAMPLE_PARTITION_WATERSHED = 0,
        SAMPLE_PARTITION_MONOTONE,
        SAMPLE_PARTITION_LAYERS,
        SAMPLE_PARTITION_MAX
    };

    enum ParsedGeometryType {
        PARSED_GEOMETRY_MESH_INSTANCES = 0,
        PARSED_GEOMETRY_STATIC_COLLIDERS,
        PARSED_GEOMETRY_BOTH,
        PARSED_GEOMETRY_MAX
    };

    enum SourceGeometryMode {
        SOURCE_GEOMETRY_NAVMESH_CHILDREN = 0,
        SOURCE_GEOMETRY_GROUPS_WITH_CHILDREN,
        SOURCE_GEOMETRY_GROUPS_EXPLICIT,
        SOURCE_GEOMETRY_MAX
    };

protected:
    float cell_size;
    float cell_height;
    float agent_height;
    float agent_radius;
    float agent_max_climb;
    float agent_max_slope;
    float region_min_size;
    float region_merge_size;
    float edge_max_length;
    float edge_max_error;
    float verts_per_poly;
    float detail_sample_distance;
    float detail_sample_max_error;

    SamplePartitionType partition_type;
    ParsedGeometryType parsed_geometry_type;
    uint32_t collision_mask;

    SourceGeometryMode source_geometry_mode;
    StringName source_group_name;

    bool filter_low_hanging_obstacles;
    bool filter_ledge_spans;
    bool filter_walkable_low_height_spans;

public:
    // Recast settings
    void set_sample_partition_type(SamplePartitionType p_value);
    SamplePartitionType get_sample_partition_type() const;

    void set_parsed_geometry_type(ParsedGeometryType p_value);
    ParsedGeometryType get_parsed_geometry_type() const;

    void set_collision_mask(uint32_t p_mask);
    uint32_t get_collision_mask() const;

    void set_collision_mask_bit(int p_bit, bool p_value);
    bool get_collision_mask_bit(int p_bit) const;

    void set_source_geometry_mode(SourceGeometryMode p_geometry_mode);
    SourceGeometryMode get_source_geometry_mode() const;

    void set_source_group_name(StringName p_group_name);
    StringName get_source_group_name() const;

    void set_cell_size(float p_value);
    float get_cell_size() const;

    void set_cell_height(float p_value);
    float get_cell_height() const;

    void set_agent_height(float p_value);
    float get_agent_height() const;

    void set_agent_radius(float p_value);
    float get_agent_radius();

    void set_agent_max_climb(float p_value);
    float get_agent_max_climb() const;

    void set_agent_max_slope(float p_value);
    float get_agent_max_slope() const;

    void set_region_min_size(float p_value);
    float get_region_min_size() const;

    void set_region_merge_size(float p_value);
    float get_region_merge_size() const;

    void set_edge_max_length(float p_value);
    float get_edge_max_length() const;

    void set_edge_max_error(float p_value);
    float get_edge_max_error() const;

    void set_verts_per_poly(float p_value);
    float get_verts_per_poly() const;

    void set_detail_sample_distance(float p_value);
    float get_detail_sample_distance() const;

    void set_detail_sample_max_error(float p_value);
    float get_detail_sample_max_error() const;

    void set_filter_low_hanging_obstacles(bool p_value);
    bool get_filter_low_hanging_obstacles() const;

    void set_filter_ledge_spans(bool p_value);
    bool get_filter_ledge_spans() const;

    void set_filter_walkable_low_height_spans(bool p_value);
    bool get_filter_walkable_low_height_spans() const;

    void create_from_mesh(const Ref<Mesh>& p_mesh);

    void set_vertices(const PoolVector<Vector3>& p_vertices);
    PoolVector<Vector3> get_vertices() const;

    void add_polygon(const Vector<int>& p_polygon);
    int get_polygon_count() const;
    Vector<int> get_polygon(int p_idx);
    void clear_polygons();

    Ref<Mesh> get_debug_mesh();

    NavigationMesh();
};

VARIANT_ENUM_CAST(NavigationMesh::SamplePartitionType);
VARIANT_ENUM_CAST(NavigationMesh::ParsedGeometryType);
VARIANT_ENUM_CAST(NavigationMesh::SourceGeometryMode);

class Navigation;

class NavigationMeshInstance : public Spatial {
    GDCLASS(NavigationMeshInstance, Spatial);

    bool enabled;
    int nav_id;
    Navigation* navigation;
    Ref<NavigationMesh> navmesh;

    Node* debug_view;

protected:
    void _notification(int p_what);
    static void _bind_methods();
    void _changed_callback(Object* p_changed, const char* p_prop);

public:
    void set_enabled(bool p_enabled);
    bool is_enabled() const;

    void set_navigation_mesh(const Ref<NavigationMesh>& p_navmesh);
    Ref<NavigationMesh> get_navigation_mesh() const;

    String get_configuration_warning() const;

    NavigationMeshInstance();
    ~NavigationMeshInstance();
};

#endif // NAVIGATION_MESH_H
