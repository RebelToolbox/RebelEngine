// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONSTRAINT_2D_SW_H
#define CONSTRAINT_2D_SW_H

#include "body_2d_sw.h"

class Constraint2DSW : public RID_Data {
    Body2DSW** _body_ptr;
    int _body_count;
    uint64_t island_step;
    Constraint2DSW* island_next;
    Constraint2DSW* island_list_next;
    bool disabled_collisions_between_bodies;

    RID self;

protected:
    Constraint2DSW(Body2DSW** p_body_ptr = nullptr, int p_body_count = 0) {
        _body_ptr                          = p_body_ptr;
        _body_count                        = p_body_count;
        island_step                        = 0;
        disabled_collisions_between_bodies = true;
    }

public:
    _FORCE_INLINE_ void set_self(const RID& p_self) {
        self = p_self;
    }

    _FORCE_INLINE_ RID get_self() const {
        return self;
    }

    _FORCE_INLINE_ uint64_t get_island_step() const {
        return island_step;
    }

    _FORCE_INLINE_ void set_island_step(uint64_t p_step) {
        island_step = p_step;
    }

    _FORCE_INLINE_ Constraint2DSW* get_island_next() const {
        return island_next;
    }

    _FORCE_INLINE_ void set_island_next(Constraint2DSW* p_next) {
        island_next = p_next;
    }

    _FORCE_INLINE_ Constraint2DSW* get_island_list_next() const {
        return island_list_next;
    }

    _FORCE_INLINE_ void set_island_list_next(Constraint2DSW* p_next) {
        island_list_next = p_next;
    }

    _FORCE_INLINE_ Body2DSW** get_body_ptr() const {
        return _body_ptr;
    }

    _FORCE_INLINE_ int get_body_count() const {
        return _body_count;
    }

    _FORCE_INLINE_ void disable_collisions_between_bodies(const bool p_disabled
    ) {
        disabled_collisions_between_bodies = p_disabled;
    }

    _FORCE_INLINE_ bool is_disabled_collisions_between_bodies() const {
        return disabled_collisions_between_bodies;
    }

    virtual bool setup(real_t p_step) = 0;
    virtual void solve(real_t p_step) = 0;

    virtual ~Constraint2DSW() {}
};

#endif // CONSTRAINT_2D_SW_H
