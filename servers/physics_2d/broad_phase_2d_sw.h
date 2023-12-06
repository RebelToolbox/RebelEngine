// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BROAD_PHASE_2D_SW_H
#define BROAD_PHASE_2D_SW_H

#include "core/math/math_funcs.h"
#include "core/math/rect2.h"

class CollisionObject2DSW;

class BroadPhase2DSW {
public:
    typedef BroadPhase2DSW* (*CreateFunction)();

    static CreateFunction create_func;

    typedef uint32_t ID;

    typedef void* (*PairCallback)(
        CollisionObject2DSW* p_object_A,
        int p_subindex_A,
        CollisionObject2DSW* p_object_B,
        int p_subindex_B,
        void* p_pair_data,
        void* p_user_data
    );
    typedef void (*UnpairCallback)(
        CollisionObject2DSW* p_object_A,
        int p_subindex_A,
        CollisionObject2DSW* p_object_B,
        int p_subindex_B,
        void* p_pair_data,
        void* p_user_data
    );

    // 0 is an invalid ID
    virtual ID create(
        CollisionObject2DSW* p_object_,
        int p_subindex      = 0,
        const Rect2& p_aabb = Rect2(),
        bool p_static       = false
    )                                               = 0;
    virtual void move(ID p_id, const Rect2& p_aabb) = 0;
    virtual void recheck_pairs(ID p_id)             = 0;
    virtual void set_static(ID p_id, bool p_static) = 0;
    virtual void remove(ID p_id)                    = 0;

    virtual CollisionObject2DSW* get_object(ID p_id) const = 0;
    virtual bool is_static(ID p_id) const                  = 0;
    virtual int get_subindex(ID p_id) const                = 0;

    virtual int cull_segment(
        const Vector2& p_from,
        const Vector2& p_to,
        CollisionObject2DSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    ) = 0;
    virtual int cull_aabb(
        const Rect2& p_aabb,
        CollisionObject2DSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    ) = 0;

    virtual void set_pair_callback(
        PairCallback p_pair_callback,
        void* p_userdata
    ) = 0;
    virtual void set_unpair_callback(
        UnpairCallback p_unpair_callback,
        void* p_userdata
    ) = 0;

    virtual void update() = 0;

    virtual ~BroadPhase2DSW();
};

#endif // BROAD_PHASE_2D_SW_H
