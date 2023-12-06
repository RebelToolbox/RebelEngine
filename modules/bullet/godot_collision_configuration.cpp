// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "godot_collision_configuration.h"

#include "godot_ray_world_algorithm.h"

#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

/**
    @author AndreaCatania
*/

GodotCollisionConfiguration::GodotCollisionConfiguration(
    const btDiscreteDynamicsWorld* world,
    const btDefaultCollisionConstructionInfo& constructionInfo
) :
    btDefaultCollisionConfiguration(constructionInfo) {
    void* mem = nullptr;

    mem = btAlignedAlloc(sizeof(GodotRayWorldAlgorithm::CreateFunc), 16);
    m_rayWorldCF = new (mem) GodotRayWorldAlgorithm::CreateFunc(world);

    mem = btAlignedAlloc(sizeof(GodotRayWorldAlgorithm::SwappedCreateFunc), 16);
    m_swappedRayWorldCF =
        new (mem) GodotRayWorldAlgorithm::SwappedCreateFunc(world);
}

GodotCollisionConfiguration::~GodotCollisionConfiguration() {
    m_rayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_rayWorldCF);

    m_swappedRayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_swappedRayWorldCF);
}

btCollisionAlgorithmCreateFunc* GodotCollisionConfiguration::
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

btCollisionAlgorithmCreateFunc* GodotCollisionConfiguration::
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

GodotSoftCollisionConfiguration::GodotSoftCollisionConfiguration(
    const btDiscreteDynamicsWorld* world,
    const btDefaultCollisionConstructionInfo& constructionInfo
) :
    btSoftBodyRigidBodyCollisionConfiguration(constructionInfo) {
    void* mem = nullptr;

    mem = btAlignedAlloc(sizeof(GodotRayWorldAlgorithm::CreateFunc), 16);
    m_rayWorldCF = new (mem) GodotRayWorldAlgorithm::CreateFunc(world);

    mem = btAlignedAlloc(sizeof(GodotRayWorldAlgorithm::SwappedCreateFunc), 16);
    m_swappedRayWorldCF =
        new (mem) GodotRayWorldAlgorithm::SwappedCreateFunc(world);
}

GodotSoftCollisionConfiguration::~GodotSoftCollisionConfiguration() {
    m_rayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_rayWorldCF);

    m_swappedRayWorldCF->~btCollisionAlgorithmCreateFunc();
    btAlignedFree(m_swappedRayWorldCF);
}

btCollisionAlgorithmCreateFunc* GodotSoftCollisionConfiguration::
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

btCollisionAlgorithmCreateFunc* GodotSoftCollisionConfiguration::
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
