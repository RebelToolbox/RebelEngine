// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "collision_configuration.h"

#include "ray_world_algorithm.h"

#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

CollisionConfiguration::CollisionConfiguration(
    const btDiscreteDynamicsWorld* world,
    const btDefaultCollisionConstructionInfo& constructionInfo
) :
    btDefaultCollisionConfiguration(constructionInfo) {
    void* mem = nullptr;

    mem          = btAlignedAlloc(sizeof(RayWorldAlgorithm::CreateFunc), 16);
    m_rayWorldCF = new (mem) RayWorldAlgorithm::CreateFunc(world);

    mem = btAlignedAlloc(sizeof(RayWorldAlgorithm::SwappedCreateFunc), 16);
    m_swappedRayWorldCF = new (mem) RayWorldAlgorithm::SwappedCreateFunc(world);
}

CollisionConfiguration::~CollisionConfiguration() {
    m_rayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_rayWorldCF);

    m_swappedRayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_swappedRayWorldCF);
}

btCollisionAlgorithmCreateFunc* CollisionConfiguration::
    getCollisionAlgorithmCreateFunc(int proxyType0, int proxyType1) {
    if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0
        && CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        // This collision is not supported
        return m_emptyCreateFunc;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0) {
        return m_rayWorldCF;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        return m_swappedRayWorldCF;
    } else {
        return btDefaultCollisionConfiguration::getCollisionAlgorithmCreateFunc(
            proxyType0,
            proxyType1
        );
    }
}

btCollisionAlgorithmCreateFunc* CollisionConfiguration::
    getClosestPointsAlgorithmCreateFunc(int proxyType0, int proxyType1) {
    if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0
        && CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        // This collision is not supported
        return m_emptyCreateFunc;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0) {
        return m_rayWorldCF;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        return m_swappedRayWorldCF;
    } else {
        return btDefaultCollisionConfiguration::
            getClosestPointsAlgorithmCreateFunc(proxyType0, proxyType1);
    }
}

SoftCollisionConfiguration::SoftCollisionConfiguration(
    const btDiscreteDynamicsWorld* world,
    const btDefaultCollisionConstructionInfo& constructionInfo
) :
    btSoftBodyRigidBodyCollisionConfiguration(constructionInfo) {
    void* mem = nullptr;

    mem          = btAlignedAlloc(sizeof(RayWorldAlgorithm::CreateFunc), 16);
    m_rayWorldCF = new (mem) RayWorldAlgorithm::CreateFunc(world);

    mem = btAlignedAlloc(sizeof(RayWorldAlgorithm::SwappedCreateFunc), 16);
    m_swappedRayWorldCF = new (mem) RayWorldAlgorithm::SwappedCreateFunc(world);
}

SoftCollisionConfiguration::~SoftCollisionConfiguration() {
    m_rayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_rayWorldCF);

    m_swappedRayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_swappedRayWorldCF);
}

btCollisionAlgorithmCreateFunc* SoftCollisionConfiguration::
    getCollisionAlgorithmCreateFunc(int proxyType0, int proxyType1) {
    if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0
        && CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        // This collision is not supported
        return m_emptyCreateFunc;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0) {
        return m_rayWorldCF;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        return m_swappedRayWorldCF;
    } else {
        return btSoftBodyRigidBodyCollisionConfiguration::
            getCollisionAlgorithmCreateFunc(proxyType0, proxyType1);
    }
}

btCollisionAlgorithmCreateFunc* SoftCollisionConfiguration::
    getClosestPointsAlgorithmCreateFunc(int proxyType0, int proxyType1) {
    if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0
        && CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        // This collision is not supported
        return m_emptyCreateFunc;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType0) {
        return m_rayWorldCF;
    } else if (CUSTOM_CONVEX_SHAPE_TYPE == proxyType1) {
        return m_swappedRayWorldCF;
    } else {
        return btSoftBodyRigidBodyCollisionConfiguration::
            getClosestPointsAlgorithmCreateFunc(proxyType0, proxyType1);
    }
}
