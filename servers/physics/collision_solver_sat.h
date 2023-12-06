// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_SOLVER_SAT_H
#define COLLISION_SOLVER_SAT_H

#include "collision_solver_sw.h"

bool sat_calculate_penetration(
    const ShapeSW* p_shape_A,
    const Transform& p_transform_A,
    const ShapeSW* p_shape_B,
    const Transform& p_transform_B,
    CollisionSolverSW::CallbackResult p_result_callback,
    void* p_userdata,
    bool p_swap          = false,
    Vector3* r_prev_axis = nullptr,
    real_t p_margin_a    = 0,
    real_t p_margin_b    = 0
);

#endif // COLLISION_SOLVER_SAT_H
