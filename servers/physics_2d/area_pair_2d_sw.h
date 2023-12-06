// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AREA_PAIR_2D_SW_H
#define AREA_PAIR_2D_SW_H

#include "area_2d_sw.h"
#include "body_2d_sw.h"
#include "constraint_2d_sw.h"

class AreaPair2DSW : public Constraint2DSW {
    Body2DSW* body;
    Area2DSW* area;
    int body_shape;
    int area_shape;
    bool colliding;

public:
    bool setup(real_t p_step);
    void solve(real_t p_step);

    AreaPair2DSW(
        Body2DSW* p_body,
        int p_body_shape,
        Area2DSW* p_area,
        int p_area_shape
    );
    ~AreaPair2DSW();
};

class Area2Pair2DSW : public Constraint2DSW {
    Area2DSW* area_a;
    Area2DSW* area_b;
    int shape_a;
    int shape_b;
    bool colliding;

public:
    bool setup(real_t p_step);
    void solve(real_t p_step);

    Area2Pair2DSW(
        Area2DSW* p_area_a,
        int p_shape_a,
        Area2DSW* p_area_b,
        int p_shape_b
    );
    ~Area2Pair2DSW();
};

#endif // AREA_PAIR_2D_SW_H
