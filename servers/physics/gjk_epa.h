// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GJK_EPA_H
#define GJK_EPA_H

#include "collision_solver_sw.h"
#include "shape_sw.h"

bool gjk_epa_calculate_penetration(
    const ShapeSW* p_shape_A,
    const Transform& p_transform_A,
    const ShapeSW* p_shape_B,
    const Transform& p_transform_B,
    CollisionSolverSW::CallbackResult p_result_callback,
    void* p_userdata,
    bool p_swap       = false,
    real_t p_margin_A = 0.0,
    real_t p_margin_B = 0.0
);
bool gjk_epa_calculate_distance(
    const ShapeSW* p_shape_A,
    const Transform& p_transform_A,
    const ShapeSW* p_shape_B,
    const Transform& p_transform_B,
    Vector3& r_result_A,
    Vector3& r_result_B
);

#endif
