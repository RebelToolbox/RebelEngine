/*************************************************************************/
/*  register_types.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           REBEL ENGINE                                */
/*************************************************************************/
/* Copyright (c) 2022-Present Rebel Engine contributors                  */
/* Copyright (c) 2014-2022 Godot Engine contributors                     */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur                  */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#define ENABLE_VHACD_IMPLEMENTATION 1

#include "register_types.h"
#include "scene/resources/mesh.h"
#include "thirdparty/vhacd/include/VHACD.h"

static Vector<PoolVector<Vector3>> convex_decompose(const real_t *p_vertices, int p_vertex_count, const uint32_t *p_triangles, int p_triangle_count, int p_max_convex_hulls = -1, Vector<PoolVector<uint32_t>> *r_convex_indices = nullptr) {
    VHACD::IVHACD::Parameters params;
    if (p_max_convex_hulls > 0) {
        params.m_maxConvexHulls = p_max_convex_hulls;
    }

    VHACD::IVHACD *decomposer = VHACD::CreateVHACD();
    decomposer->Compute(p_vertices, p_vertex_count, p_triangles, p_triangle_count, params);

    int hull_count = decomposer->GetNConvexHulls();

    Vector<PoolVector<Vector3>> ret;
    ret.resize(hull_count);

    if (r_convex_indices) {
        r_convex_indices->resize(hull_count);
    }

    for (int i = 0; i < hull_count; i++) {
        VHACD::IVHACD::ConvexHull hull;
        decomposer->GetConvexHull(i, hull);

        PoolVector<Vector3> &points = ret.write[i];
        points.resize(hull.m_points.size());

        PoolVector<Vector3>::Write write_points = points.write();
        for (uint32_t j = 0; j < hull.m_points.size(); ++j) {
            write_points[j] = Vector3(hull.m_points[j].mX, hull.m_points[j].mY, hull.m_points[j].mZ);
        }

        if (r_convex_indices) {
            PoolVector<uint32_t> &indices = r_convex_indices->write[i];
            PoolVector<uint32_t>::Write write_indices = indices.write();
            indices.resize(hull.m_triangles.size() * 3);
            for (uint32_t j = 0; j < hull.m_triangles.size(); ++j) {
                write_indices[j * 3 + 0] = hull.m_triangles[j].mI0;
                write_indices[j * 3 + 1] = hull.m_triangles[j].mI1;
                write_indices[j * 3 + 2] = hull.m_triangles[j].mI2;
            }
        }
    }

    decomposer->Clean();
    decomposer->Release();

    return ret;
}

void register_vhacd_types() {
    Mesh::convex_decomposition_function = convex_decompose;
}

void unregister_vhacd_types() {
    Mesh::convex_decomposition_function = nullptr;
}
