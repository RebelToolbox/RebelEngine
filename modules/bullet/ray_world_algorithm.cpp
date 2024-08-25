// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "ray_world_algorithm.h"

#include "btRayShape.h"
#include "collision_object_bullet.h"

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

// Epsilon to account for floating point inaccuracies
#define RAY_PENETRATION_DEPTH_EPSILON 0.01

RayWorldAlgorithm::CreateFunc::CreateFunc(const btDiscreteDynamicsWorld* world
) :
    m_world(world) {}

RayWorldAlgorithm::SwappedCreateFunc::SwappedCreateFunc(
    const btDiscreteDynamicsWorld* world
) :
    m_world(world) {}

RayWorldAlgorithm::RayWorldAlgorithm(
    const btDiscreteDynamicsWorld* world,
    btPersistentManifold* mf,
    const btCollisionAlgorithmConstructionInfo& ci,
    const btCollisionObjectWrapper* body0Wrap,
    const btCollisionObjectWrapper* body1Wrap,
    bool isSwapped
) :
    btActivatingCollisionAlgorithm(ci, body0Wrap, body1Wrap),
    m_world(world),
    m_manifoldPtr(mf),
    m_ownManifold(false),
    m_isSwapped(isSwapped) {}

RayWorldAlgorithm::~RayWorldAlgorithm() {
    if (m_ownManifold && m_manifoldPtr) {
        m_dispatcher->releaseManifold(m_manifoldPtr);
    }
}

void RayWorldAlgorithm::processCollision(
    const btCollisionObjectWrapper* body0Wrap,
    const btCollisionObjectWrapper* body1Wrap,
    const btDispatcherInfo& dispatchInfo,
    btManifoldResult* resultOut
) {
    if (!m_manifoldPtr) {
        if (m_isSwapped) {
            m_manifoldPtr = m_dispatcher->getNewManifold(
                body1Wrap->getCollisionObject(),
                body0Wrap->getCollisionObject()
            );
        } else {
            m_manifoldPtr = m_dispatcher->getNewManifold(
                body0Wrap->getCollisionObject(),
                body1Wrap->getCollisionObject()
            );
        }
        m_ownManifold = true;
    }
    m_manifoldPtr->clearManifold();
    resultOut->setPersistentManifold(m_manifoldPtr);

    const btRayShape* ray_shape;
    btTransform ray_transform;

    const btCollisionObjectWrapper* other_co_wrapper;

    if (m_isSwapped) {
        ray_shape =
            static_cast<const btRayShape*>(body1Wrap->getCollisionShape());
        ray_transform = body1Wrap->getWorldTransform();

        other_co_wrapper = body0Wrap;
    } else {
        ray_shape =
            static_cast<const btRayShape*>(body0Wrap->getCollisionShape());
        ray_transform = body0Wrap->getWorldTransform();

        other_co_wrapper = body1Wrap;
    }

    btTransform to(ray_transform * ray_shape->getSupportPoint());

    btCollisionWorld::ClosestRayResultCallback btResult(
        ray_transform.getOrigin(),
        to.getOrigin()
    );

    m_world
        ->rayTestSingleInternal(ray_transform, to, other_co_wrapper, btResult);

    if (btResult.hasHit()) {
        btScalar depth(
            ray_shape->getScaledLength() * (btResult.m_closestHitFraction - 1)
        );

        if (depth > -RAY_PENETRATION_DEPTH_EPSILON) {
            depth = 0.0;
        }

        if (ray_shape->getSlipsOnSlope()) {
            resultOut->addContactPoint(
                btResult.m_hitNormalWorld,
                btResult.m_hitPointWorld,
                depth
            );
        } else {
            resultOut->addContactPoint(
                (ray_transform.getOrigin() - to.getOrigin()).normalize(),
                btResult.m_hitPointWorld,
                depth
            );
        }
    }
}

btScalar RayWorldAlgorithm::calculateTimeOfImpact(
    btCollisionObject* body0,
    btCollisionObject* body1,
    const btDispatcherInfo& dispatchInfo,
    btManifoldResult* resultOut
) {
    return 1;
}
