// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STEP_2D_SW_H
#define STEP_2D_SW_H

#include "space_2d_sw.h"

class Step2DSW {
    uint64_t _step;

    void _populate_island(
        Body2DSW* p_body,
        Body2DSW** p_island,
        Constraint2DSW** p_constraint_island
    );
    bool _setup_island(Constraint2DSW* p_island, real_t p_delta);
    void _solve_island(
        Constraint2DSW* p_island,
        int p_iterations,
        real_t p_delta
    );
    void _check_suspend(Body2DSW* p_island, real_t p_delta);

public:
    void step(Space2DSW* p_space, real_t p_delta, int p_iterations);
    Step2DSW();
};

#endif // STEP_2D_SW_H
