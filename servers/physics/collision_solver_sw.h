// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_SOLVER_SW_H
#define COLLISION_SOLVER_SW_H

#include "shape_sw.h"

class CollisionSolverSW {
public:
    typedef void (*CallbackResult)(
        const Vector3& p_point_A,
        const Vector3& p_point_B,
        void* p_userdata
    );

private:
    static bool concave_callback(void* p_userdata, ShapeSW* p_convex);
    static bool solve_static_plane(
        const ShapeSW* p_shape_A,
        const Transform& p_transform_A,
        const ShapeSW* p_shape_B,
        const Transform& p_transform_B,
        CallbackResult p_result_callback,
        void* p_userdata,
        bool p_swap_result
    );
    static bool solve_ray(
        const ShapeSW* p_shape_A,
        const Transform& p_transform_A,
        const ShapeSW* p_shape_B,
        const Transform& p_transform_B,
        CallbackResult p_result_callback,
        void* p_userdata,
        bool p_swap_result,
        real_t p_margin = 0
    );
    static bool solve_concave(
        const ShapeSW* p_shape_A,
        const Transform& p_transform_A,
        const ShapeSW* p_shape_B,
        const Transform& p_transform_B,
        CallbackResult p_result_callback,
        void* p_userdata,
        bool p_swap_result,
        real_t p_margin_A = 0,
        real_t p_margin_B = 0
    );
    static bool concave_distance_callback(void* p_userdata, ShapeSW* p_convex);
    static bool solve_distance_plane(
        const ShapeSW* p_shape_A,
        const Transform& p_transform_A,
        const ShapeSW* p_shape_B,
        const Transform& p_transform_B,
        Vector3& r_point_A,
        Vector3& r_point_B
    );

public:
    static bool solve_static(
        const ShapeSW* p_shape_A,
        const Transform& p_transform_A,
        const ShapeSW* p_shape_B,
        const Transform& p_transform_B,
        CallbackResult p_result_callback,
        void* p_userdata,
        Vector3* r_sep_axis = nullptr,
        real_t p_margin_A   = 0,
        real_t p_margin_B   = 0
    );
    static bool solve_distance(
        const ShapeSW* p_shape_A,
        const Transform& p_transform_A,
        const ShapeSW* p_shape_B,
        const Transform& p_transform_B,
        Vector3& r_point_A,
        Vector3& r_point_B,
        const AABB& p_concave_hint,
        Vector3* r_sep_axis = nullptr
    );
};

#endif // COLLISION_SOLVER__SW_H
