// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_SOLVER_2D_SAT_H
#define COLLISION_SOLVER_2D_SAT_H

#include "collision_solver_2d_sw.h"

bool sat_2d_calculate_penetration(
    const Shape2DSW* p_shape_A,
    const Transform2D& p_transform_A,
    const Vector2& p_motion_A,
    const Shape2DSW* p_shape_B,
    const Transform2D& p_transform_B,
    const Vector2& p_motion_B,
    CollisionSolver2DSW::CallbackResult p_result_callback,
    void* p_userdata,
    bool p_swap       = false,
    Vector2* sep_axis = nullptr,
    real_t p_margin_A = 0,
    real_t p_margin_B = 0
);

#endif // COLLISION_SOLVER_2D_SAT_H
