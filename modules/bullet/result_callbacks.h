// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESULT_CALLBACKS_H
#define RESULT_CALLBACKS_H

#include "servers/physics_server.h"

#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <btBulletDynamicsCommon.h>

class RigidBodyBullet;

/// This callback is injected inside bullet server and allow me to smooth
/// contacts against trimesh
bool contactAddedCallback(
    btManifoldPoint& cp,
    const btCollisionObjectWrapper* colObj0Wrap,
    int partId0,
    int index0,
    const btCollisionObjectWrapper* colObj1Wrap,
    int partId1,
    int index1
);

/// This class is required to implement custom collision behaviour in the
/// broadphase
struct FilterCallback : public btOverlapFilterCallback {
    static bool test_collision_filters(
        uint32_t body0_collision_layer,
        uint32_t body0_collision_mask,
        uint32_t body1_collision_layer,
        uint32_t body1_collision_mask
    );

    // return true when pairs need collision
    virtual bool needBroadphaseCollision(
        btBroadphaseProxy* proxy0,
        btBroadphaseProxy* proxy1
    ) const;
};

/// It performs an additional check allow exclusions.
struct ClosestRayResultCallback :
    public btCollisionWorld::ClosestRayResultCallback {
    const Set<RID>* m_exclude;
    bool m_pickRay;
    int m_shapeId;

    bool collide_with_bodies;
    bool collide_with_areas;

public:
    ClosestRayResultCallback(
        const btVector3& rayFromWorld,
        const btVector3& rayToWorld,
        const Set<RID>* p_exclude,
        bool p_collide_with_bodies,
        bool p_collide_with_areas
    ) :
        btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld),
        m_exclude(p_exclude),
        m_pickRay(false),
        m_shapeId(0),
        collide_with_bodies(p_collide_with_bodies),
        collide_with_areas(p_collide_with_areas) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const;

    virtual btScalar addSingleResult(
        btCollisionWorld::LocalRayResult& rayResult,
        bool normalInWorldSpace
    ) {
        if (rayResult.m_localShapeInfo) {
            m_shapeId =
                rayResult.m_localShapeInfo
                    ->m_triangleIndex; // "m_triangleIndex" Is a odd name but
                                       // contains the compound shape ID
        } else {
            m_shapeId = 0;
        }
        return btCollisionWorld::ClosestRayResultCallback::addSingleResult(
            rayResult,
            normalInWorldSpace
        );
    }
};

// store all colliding object
struct AllConvexResultCallback : public btCollisionWorld::ConvexResultCallback {
public:
    PhysicsDirectSpaceState::ShapeResult* m_results;
    int m_resultMax;
    const Set<RID>* m_exclude;
    int count;

    AllConvexResultCallback(
        PhysicsDirectSpaceState::ShapeResult* p_results,
        int p_resultMax,
        const Set<RID>* p_exclude
    ) :
        m_results(p_results),
        m_resultMax(p_resultMax),
        m_exclude(p_exclude),
        count(0) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const;

    virtual btScalar addSingleResult(
        btCollisionWorld::LocalConvexResult& convexResult,
        bool normalInWorldSpace
    );
};

struct KinClosestConvexResultCallback :
    public btCollisionWorld::ClosestConvexResultCallback {
public:
    const RigidBodyBullet* m_self_object;
    const Set<RID>* m_exclude;
    const bool m_infinite_inertia;

    KinClosestConvexResultCallback(
        const btVector3& convexFromWorld,
        const btVector3& convexToWorld,
        const RigidBodyBullet* p_self_object,
        bool p_infinite_inertia,
        const Set<RID>* p_exclude
    ) :
        btCollisionWorld::ClosestConvexResultCallback(
            convexFromWorld,
            convexToWorld
        ),
        m_self_object(p_self_object),
        m_exclude(p_exclude),
        m_infinite_inertia(p_infinite_inertia) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const;
};

struct ClosestConvexResultCallback :
    public btCollisionWorld::ClosestConvexResultCallback {
public:
    const Set<RID>* m_exclude;
    int m_shapeId;

    bool collide_with_bodies;
    bool collide_with_areas;

    ClosestConvexResultCallback(
        const btVector3& convexFromWorld,
        const btVector3& convexToWorld,
        const Set<RID>* p_exclude,
        bool p_collide_with_bodies,
        bool p_collide_with_areas
    ) :
        btCollisionWorld::ClosestConvexResultCallback(
            convexFromWorld,
            convexToWorld
        ),
        m_exclude(p_exclude),
        m_shapeId(0),
        collide_with_bodies(p_collide_with_bodies),
        collide_with_areas(p_collide_with_areas) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const;

    virtual btScalar addSingleResult(
        btCollisionWorld::LocalConvexResult& convexResult,
        bool normalInWorldSpace
    );
};

struct AllContactResultCallback :
    public btCollisionWorld::ContactResultCallback {
public:
    const btCollisionObject* m_self_object;
    PhysicsDirectSpaceState::ShapeResult* m_results;
    int m_resultMax;
    const Set<RID>* m_exclude;
    int m_count;

    bool collide_with_bodies;
    bool collide_with_areas;

    AllContactResultCallback(
        btCollisionObject* p_self_object,
        PhysicsDirectSpaceState::ShapeResult* p_results,
        int p_resultMax,
        const Set<RID>* p_exclude,
        bool p_collide_with_bodies,
        bool p_collide_with_areas
    ) :
        m_self_object(p_self_object),
        m_results(p_results),
        m_resultMax(p_resultMax),
        m_exclude(p_exclude),
        m_count(0),
        collide_with_bodies(p_collide_with_bodies),
        collide_with_areas(p_collide_with_areas) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const;

    virtual btScalar addSingleResult(
        btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1
    );
};

/// Returns the list of contacts pairs in this order: Local contact, other body
/// contact
struct ContactPairContactResultCallback :
    public btCollisionWorld::ContactResultCallback {
public:
    const btCollisionObject* m_self_object;
    Vector3* m_results;
    int m_resultMax;
    const Set<RID>* m_exclude;
    int m_count;

    bool collide_with_bodies;
    bool collide_with_areas;

    ContactPairContactResultCallback(
        btCollisionObject* p_self_object,
        Vector3* p_results,
        int p_resultMax,
        const Set<RID>* p_exclude,
        bool p_collide_with_bodies,
        bool p_collide_with_areas
    ) :
        m_self_object(p_self_object),
        m_results(p_results),
        m_resultMax(p_resultMax),
        m_exclude(p_exclude),
        m_count(0),
        collide_with_bodies(p_collide_with_bodies),
        collide_with_areas(p_collide_with_areas) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const;

    virtual btScalar addSingleResult(
        btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1
    );
};

struct RestInfoContactResultCallback :
    public btCollisionWorld::ContactResultCallback {
public:
    const btCollisionObject* m_self_object;
    PhysicsDirectSpaceState::ShapeRestInfo* m_result;
    const Set<RID>* m_exclude;
    bool m_collided;
    real_t m_min_distance;
    const btCollisionObject* m_rest_info_collision_object;
    btVector3 m_rest_info_bt_point;
    bool collide_with_bodies;
    bool collide_with_areas;

    RestInfoContactResultCallback(
        btCollisionObject* p_self_object,
        PhysicsDirectSpaceState::ShapeRestInfo* p_result,
        const Set<RID>* p_exclude,
        bool p_collide_with_bodies,
        bool p_collide_with_areas
    ) :
        m_self_object(p_self_object),
        m_result(p_result),
        m_exclude(p_exclude),
        m_collided(false),
        m_min_distance(0),
        collide_with_bodies(p_collide_with_bodies),
        collide_with_areas(p_collide_with_areas) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const;

    virtual btScalar addSingleResult(
        btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1
    );
};

struct DeepPenetrationContactResultCallback : public btManifoldResult {
    btVector3 m_pointNormalWorld;
    btVector3 m_pointWorld;
    btScalar m_penetration_distance;
    int m_other_compound_shape_index;

    DeepPenetrationContactResultCallback(
        const btCollisionObjectWrapper* body0Wrap,
        const btCollisionObjectWrapper* body1Wrap
    ) :
        btManifoldResult(body0Wrap, body1Wrap),
        m_penetration_distance(0),
        m_other_compound_shape_index(0) {}

    void reset() {
        m_penetration_distance = 0;
    }

    bool hasHit() {
        return m_penetration_distance < 0;
    }

    virtual void addContactPoint(
        const btVector3& normalOnBInWorld,
        const btVector3& pointInWorldOnB,
        btScalar depth
    );
};

#endif // RESULT_CALLBACKS_H
