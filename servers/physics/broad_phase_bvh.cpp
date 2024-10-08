// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "broad_phase_bvh.h"

#include "collision_object_sw.h"
#include "core/project_settings.h"

BroadPhaseSW::ID BroadPhaseBVH::create(
    CollisionObjectSW* p_object,
    int p_subindex,
    const AABB& p_aabb,
    bool p_static
) {
    ID oid = bvh.create(
        p_object,
        true,
        p_aabb,
        p_subindex,
        !p_static,
        1 << p_object->get_type(),
        p_static ? 0 : 0xFFFFF
    ); // Pair everything, don't care?
    return oid + 1;
}

void BroadPhaseBVH::move(ID p_id, const AABB& p_aabb) {
    bvh.move(p_id - 1, p_aabb);
}

void BroadPhaseBVH::recheck_pairs(ID p_id) {
    bvh.recheck_pairs(p_id - 1);
}

void BroadPhaseBVH::set_static(ID p_id, bool p_static) {
    CollisionObjectSW* it = bvh.get(p_id - 1);
    bvh.set_pairable(
        p_id - 1,
        !p_static,
        1 << it->get_type(),
        p_static ? 0 : 0xFFFFF,
        false
    ); // Pair everything, don't care?
}

void BroadPhaseBVH::remove(ID p_id) {
    bvh.erase(p_id - 1);
}

CollisionObjectSW* BroadPhaseBVH::get_object(ID p_id) const {
    CollisionObjectSW* it = bvh.get(p_id - 1);
    ERR_FAIL_COND_V(!it, nullptr);
    return it;
}

bool BroadPhaseBVH::is_static(ID p_id) const {
    return !bvh.is_pairable(p_id - 1);
}

int BroadPhaseBVH::get_subindex(ID p_id) const {
    return bvh.get_subindex(p_id - 1);
}

int BroadPhaseBVH::cull_point(
    const Vector3& p_point,
    CollisionObjectSW** p_results,
    int p_max_results,
    int* p_result_indices
) {
    return bvh.cull_point(p_point, p_results, p_max_results, p_result_indices);
}

int BroadPhaseBVH::cull_segment(
    const Vector3& p_from,
    const Vector3& p_to,
    CollisionObjectSW** p_results,
    int p_max_results,
    int* p_result_indices
) {
    return bvh
        .cull_segment(p_from, p_to, p_results, p_max_results, p_result_indices);
}

int BroadPhaseBVH::cull_aabb(
    const AABB& p_aabb,
    CollisionObjectSW** p_results,
    int p_max_results,
    int* p_result_indices
) {
    return bvh.cull_aabb(p_aabb, p_results, p_max_results, p_result_indices);
}

void* BroadPhaseBVH::_pair_callback(
    void* p_self,
    uint32_t p_id_A,
    CollisionObjectSW* p_object_A,
    int p_subindex_A,
    uint32_t p_id_B,
    CollisionObjectSW* p_object_B,
    int p_subindex_B
) {
    BroadPhaseBVH* bpo = (BroadPhaseBVH*)(p_self);
    if (!bpo->pair_callback) {
        return nullptr;
    }

    return bpo->pair_callback(
        p_object_A,
        p_subindex_A,
        p_object_B,
        p_subindex_B,
        nullptr,
        bpo->pair_userdata
    );
}

void BroadPhaseBVH::_unpair_callback(
    void* p_self,
    uint32_t p_id_A,
    CollisionObjectSW* p_object_A,
    int p_subindex_A,
    uint32_t p_id_B,
    CollisionObjectSW* p_object_B,
    int p_subindex_B,
    void* p_pair_data
) {
    BroadPhaseBVH* bpo = (BroadPhaseBVH*)(p_self);
    if (!bpo->unpair_callback) {
        return;
    }

    bpo->unpair_callback(
        p_object_A,
        p_subindex_A,
        p_object_B,
        p_subindex_B,
        p_pair_data,
        bpo->unpair_userdata
    );
}

void* BroadPhaseBVH::_check_pair_callback(
    void* p_self,
    uint32_t p_id_A,
    CollisionObjectSW* p_object_A,
    int p_subindex_A,
    uint32_t p_id_B,
    CollisionObjectSW* p_object_B,
    int p_subindex_B,
    void* p_pair_data
) {
    BroadPhaseBVH* bpo = (BroadPhaseBVH*)(p_self);
    if (!bpo->pair_callback) {
        return nullptr;
    }

    return bpo->pair_callback(
        p_object_A,
        p_subindex_A,
        p_object_B,
        p_subindex_B,
        p_pair_data,
        bpo->pair_userdata
    );
}

void BroadPhaseBVH::set_pair_callback(
    PairCallback p_pair_callback,
    void* p_userdata
) {
    pair_callback = p_pair_callback;
    pair_userdata = p_userdata;
}

void BroadPhaseBVH::set_unpair_callback(
    UnpairCallback p_unpair_callback,
    void* p_userdata
) {
    unpair_callback = p_unpair_callback;
    unpair_userdata = p_userdata;
}

void BroadPhaseBVH::update() {
    bvh.update();
}

BroadPhaseSW* BroadPhaseBVH::_create() {
    return memnew(BroadPhaseBVH);
}

BroadPhaseBVH::BroadPhaseBVH() {
    bvh.params_set_thread_safe(GLOBAL_GET("rendering/threads/thread_safe_bvh"));
    bvh.params_set_pairing_expansion(
        GLOBAL_GET("physics/3d/rebel_physics/bvh_collision_margin")
    );
    bvh.set_pair_callback(_pair_callback, this);
    bvh.set_unpair_callback(_unpair_callback, this);
    bvh.set_check_pair_callback(_check_pair_callback, this);
    pair_callback   = nullptr;
    pair_userdata   = nullptr;
    unpair_userdata = nullptr;
}
