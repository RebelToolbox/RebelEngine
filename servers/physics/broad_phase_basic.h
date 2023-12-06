// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BROAD_PHASE_BASIC_H
#define BROAD_PHASE_BASIC_H

#include "broad_phase_sw.h"
#include "core/map.h"

class BroadPhaseBasic : public BroadPhaseSW {
    struct Element {
        CollisionObjectSW* owner;
        bool _static;
        AABB aabb;
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
    virtual ID create(
        CollisionObjectSW* p_object,
        int p_subindex     = 0,
        const AABB& p_aabb = AABB(),
        bool p_static      = false
    );
    virtual void move(ID p_id, const AABB& p_aabb);
    virtual void recheck_pairs(ID p_id);
    virtual void set_static(ID p_id, bool p_static);
    virtual void remove(ID p_id);

    virtual CollisionObjectSW* get_object(ID p_id) const;
    virtual bool is_static(ID p_id) const;
    virtual int get_subindex(ID p_id) const;

    virtual int cull_point(
        const Vector3& p_point,
        CollisionObjectSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    );
    virtual int cull_segment(
        const Vector3& p_from,
        const Vector3& p_to,
        CollisionObjectSW** p_results,
        int p_max_results,
        int* p_result_indices = nullptr
    );
    virtual int cull_aabb(
        const AABB& p_aabb,
        CollisionObjectSW** p_results,
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

    static BroadPhaseSW* _create();
    BroadPhaseBasic();
};

#endif // BROAD_PHASE_BASIC_H
