// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BROAD_PHASE_SW_H
#define BROAD_PHASE_SW_H

#include "core/math/aabb.h"
#include "core/math/math_funcs.h"

class CollisionObjectSW;

class BroadPhaseSW {
public:
    typedef BroadPhaseSW* (*CreateFunction)();

    static CreateFunction create_func;

    typedef uint32_t ID;

    typedef void* (*PairCallback)(
        CollisionObjectSW* p_object_A,
        int p_subindex_A,
        CollisionObjectSW* p_object_B,
        int p_subindex_B,
        void* p_pair_data,
        void* p_user_data
    );
    typedef void (*UnpairCallback)(
        CollisionObjectSW* p_object_A,
        int p_subindex_A,
        CollisionObjectSW* p_object_B,
        int p_subindex_B,
        void* p_pair_data,
        void* p_user_data
    );

    // 0 is an invalid ID
    virtual ID create(
        CollisionObjectSW* p_object_,
        int p_subindex     = 0,
        const AABB& p_aabb = AABB(),
        bool p_static      = false
    )                                               = 0;
    virtual void move(ID p_id, const AABB& p_aabb)  = 0;
    virtual void recheck_pairs(ID p_id)             = 0;
    virtual void set_static(ID p_id, bool p_static) = 0;
    virtual void remove(ID p_id)                    = 0;

    virtual CollisionObjectSW* get_object(ID p_id) const = 0;
    virtual bool is_static(ID p_id) const                = 0;
    virtual int get_subindex(ID p_id) const              = 0;

    virtual int cull_point(
        const Vector3& p_point,
        CollisionObjectSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    ) = 0;
    virtual int cull_segment(
        const Vector3& p_from,
        const Vector3& p_to,
        CollisionObjectSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    ) = 0;
    virtual int cull_aabb(
        const AABB& p_aabb,
        CollisionObjectSW** p_results,
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

    virtual ~BroadPhaseSW();
};

#endif // BROAD_PHASE__SW_H
