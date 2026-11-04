// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BROAD_PHASE_2D_BASIC_H
#define BROAD_PHASE_2D_BASIC_H

#include "core/map.h"
#include "space_2d_sw.h"

class BroadPhase2DBasic : public BroadPhase2DSW {
    struct Element {
        CollisionObject2DSW* owner;
        bool _static;
        Rect2 aabb;
        int subindex;
    };

    Map<ID, Element> element_map;

    ID current;

    struct PairKey {
        union {
            struct {
                ID a;
                ID b;
            };

            uint64_t key;
        };

        _FORCE_INLINE_ bool operator<(const PairKey& p_key) const {
            return key < p_key.key;
        }

        PairKey() {
            key = 0;
        }

        PairKey(ID p_a, ID p_b) {
            if (p_a > p_b) {
                a = p_b;
                b = p_a;
            } else {
                a = p_a;
                b = p_b;
            }
        }
    };

    Map<PairKey, void*> pair_map;

    PairCallback pair_callback;
    void* pair_userdata;
    UnpairCallback unpair_callback;
    void* unpair_userdata;

public:
    // 0 is an invalid ID
    ID create(
        CollisionObject2DSW* p_object_,
        int p_subindex      = 0,
        const Rect2& p_aabb = Rect2(),
        bool p_static       = false
    ) override;
    void move(ID p_id, const Rect2& p_aabb) override;
    void recheck_pairs(ID p_id) override;
    void set_static(ID p_id, bool p_static) override;
    void remove(ID p_id) override;

    CollisionObject2DSW* get_object(ID p_id) const override;
    bool is_static(ID p_id) const override;
    int get_subindex(ID p_id) const override;

    int cull_segment(
        const Vector2& p_from,
        const Vector2& p_to,
        CollisionObject2DSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    ) override;
    int cull_aabb(
        const Rect2& p_aabb,
        CollisionObject2DSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    ) override;

    void set_pair_callback(
        PairCallback p_pair_callback,
        void* p_userdata
    ) override;
    void set_unpair_callback(
        UnpairCallback p_unpair_callback,
        void* p_userdata
    ) override;

    void update() override;

    static BroadPhase2DSW* _create();
    BroadPhase2DBasic();
};

#endif // BROAD_PHASE_2D_BASIC_H
