// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONSTRAINT_SW_H
#define CONSTRAINT_SW_H

#include "body_sw.h"

class ConstraintSW : public RID_Data {
    BodySW** _body_ptr;
    int _body_count;
    uint64_t island_step;
    ConstraintSW* island_next;
    ConstraintSW* island_list_next;
    int priority;
    bool disabled_collisions_between_bodies;

    RID self;

protected:
    ConstraintSW(BodySW** p_body_ptr = nullptr, int p_body_count = 0) {
        _body_ptr                          = p_body_ptr;
        _body_count                        = p_body_count;
        island_step                        = 0;
        priority                           = 1;
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

    _FORCE_INLINE_ ConstraintSW* get_island_next() const {
        return island_next;
    }

    _FORCE_INLINE_ void set_island_next(ConstraintSW* p_next) {
        island_next = p_next;
    }

    _FORCE_INLINE_ ConstraintSW* get_island_list_next() const {
        return island_list_next;
    }

    _FORCE_INLINE_ void set_island_list_next(ConstraintSW* p_next) {
        island_list_next = p_next;
    }

    _FORCE_INLINE_ BodySW** get_body_ptr() const {
        return _body_ptr;
    }

    _FORCE_INLINE_ int get_body_count() const {
        return _body_count;
    }

    _FORCE_INLINE_ void set_priority(int p_priority) {
        priority = p_priority;
    }

    _FORCE_INLINE_ int get_priority() const {
        return priority;
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

    virtual ~ConstraintSW() {}
};

#endif // CONSTRAINT__SW_H
