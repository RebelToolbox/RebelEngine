// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AREA_PAIR_SW_H
#define AREA_PAIR_SW_H

#include "area_sw.h"
#include "body_sw.h"
#include "constraint_sw.h"

class AreaPairSW : public ConstraintSW {
    BodySW* body;
    AreaSW* area;
    int body_shape;
    int area_shape;
    bool colliding;

public:
    bool setup(real_t p_step);
    void solve(real_t p_step);

    AreaPairSW(
        BodySW* p_body,
        int p_body_shape,
        AreaSW* p_area,
        int p_area_shape
    );
    ~AreaPairSW();
};

class Area2PairSW : public ConstraintSW {
    AreaSW* area_a;
    AreaSW* area_b;
    int shape_a;
    int shape_b;
    bool colliding;

public:
    bool setup(real_t p_step);
    void solve(real_t p_step);

    Area2PairSW(
        AreaSW* p_area_a,
        int p_shape_a,
        AreaSW* p_area_b,
        int p_shape_b
    );
    ~Area2PairSW();
};

#endif // AREA_PAIR__SW_H
