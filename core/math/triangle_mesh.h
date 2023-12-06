// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "core/math/face3.h"
#include "core/reference.h"

class TriangleMesh : public Reference {
    GDCLASS(TriangleMesh, Reference);

public:
    struct Triangle {
        Vector3 normal;
        int indices[3];
    };

private:
    PoolVector<Triangle> triangles;
    PoolVector<Vector3> vertices;

    struct BVH {
        AABB aabb;
        Vector3 center; // used for sorting
        int left;
        int right;

        int face_index;
    };

    struct BVHCmpX {
        bool operator()(const BVH* p_left, const BVH* p_right) const {
            return p_left->center.x < p_right->center.x;
        }
    };

    struct BVHCmpY {
        bool operator()(const BVH* p_left, const BVH* p_right) const {
            return p_left->center.y < p_right->center.y;
        }
    };

    struct BVHCmpZ {
        bool operator()(const BVH* p_left, const BVH* p_right) const {
            return p_left->center.z < p_right->center.z;
        }
    };

    int _create_bvh(
        BVH* p_bvh,
        BVH** p_bb,
        int p_from,
        int p_size,
        int p_depth,
        int& max_depth,
        int& max_alloc
    );

    PoolVector<BVH> bvh;
    int max_depth;
    bool valid;

public:
    bool is_valid() const;
    bool intersect_segment(
        const Vector3& p_begin,
        const Vector3& p_end,
        Vector3& r_point,
        Vector3& r_normal
    ) const;
    bool intersect_ray(
        const Vector3& p_begin,
        const Vector3& p_dir,
        Vector3& r_point,
        Vector3& r_normal
    ) const;
    bool intersect_convex_shape(
        const Plane* p_planes,
        int p_plane_count,
        const Vector3* p_points,
        int p_point_count
    ) const;
    bool inside_convex_shape(
        const Plane* p_planes,
        int p_plane_count,
        const Vector3* p_points,
        int p_point_count,
        Vector3 p_scale = Vector3(1, 1, 1)
    ) const;
    Vector3 get_area_normal(const AABB& p_aabb) const;
    PoolVector<Face3> get_faces() const;

    const PoolVector<Triangle>& get_triangles() const {
        return triangles;
    }

    const PoolVector<Vector3>& get_vertices() const {
        return vertices;
    }

    void get_indices(PoolVector<int>* r_triangles_indices) const;

    void create(const PoolVector<Vector3>& p_faces);
    TriangleMesh();
};

#endif // TRIANGLE_MESH_H
