// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BROAD_PHASE_2D_BVH_H
#define BROAD_PHASE_2D_BVH_H

#include "broad_phase_2d_sw.h"
#include "core/math/bvh.h"
#include "core/math/rect2.h"
#include "core/math/vector2.h"

class BroadPhase2DBVH : public BroadPhase2DSW {
    BVH_Manager<CollisionObject2DSW, true, 128, Rect2, Vector2> bvh;

    static void* _pair_callback(
        void* p_self,
        uint32_t p_id_A,
        CollisionObject2DSW* p_object_A,
        int p_subindex_A,
        uint32_t p_id_B,
        CollisionObject2DSW* p_object_B,
        int p_subindex_B
    );
    static void _unpair_callback(
        void* p_self,
        uint32_t p_id_A,
        CollisionObject2DSW* p_object_A,
        int p_subindex_A,
        uint32_t p_id_B,
        CollisionObject2DSW* p_object_B,
        int p_subindex_B,
        void* p_pair_data
    );
    static void* _check_pair_callback(
        void* p_self,
        uint32_t p_id_A,
        CollisionObject2DSW* p_object_A,
        int p_subindex_A,
        uint32_t p_id_B,
        CollisionObject2DSW* p_object_B,
        int p_subindex_B,
        void* p_pair_data
    );

    PairCallback pair_callback;
    void* pair_userdata;
    UnpairCallback unpair_callback;
    void* unpair_userdata;

public:
    // 0 is an invalid ID
    virtual ID create(
        CollisionObject2DSW* p_object,
        int p_subindex      = 0,
        const Rect2& p_aabb = Rect2(),
        bool p_static       = false
    );
    virtual void move(ID p_id, const Rect2& p_aabb);
    virtual void recheck_pairs(ID p_id);
    virtual void set_static(ID p_id, bool p_static);
    virtual void remove(ID p_id);

    virtual CollisionObject2DSW* get_object(ID p_id) const;
    virtual bool is_static(ID p_id) const;
    virtual int get_subindex(ID p_id) const;

    virtual int cull_segment(
        const Vector2& p_from,
        const Vector2& p_to,
        CollisionObject2DSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    );
    virtual int cull_aabb(
        const Rect2& p_aabb,
        CollisionObject2DSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    );

    virtual void set_pair_callback(
        PairCallback p_pair_callback,
        void* p_userdata
    );
    virtual void set_unpair_callback(
        UnpairCallback p_unpair_callback,
        void* p_userdata
    );

    virtual void update();

    static BroadPhase2DSW* _create();
    BroadPhase2DBVH();
};

#endif // BROAD_PHASE_2D_BVH_H
