// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOT_COLLISION_CONFIGURATION_H
#define GODOT_COLLISION_CONFIGURATION_H

#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>

/**
    @author AndreaCatania
*/

class btDiscreteDynamicsWorld;

class GodotCollisionConfiguration : public btDefaultCollisionConfiguration {
    btCollisionAlgorithmCreateFunc* m_rayWorldCF;
    btCollisionAlgorithmCreateFunc* m_swappedRayWorldCF;

public:
    GodotCollisionConfiguration(
        const btDiscreteDynamicsWorld* world,
        const btDefaultCollisionConstructionInfo& constructionInfo =
            btDefaultCollisionConstructionInfo()
    );
    virtual ~GodotCollisionConfiguration();

    virtual btCollisionAlgorithmCreateFunc* getCollisionAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
    virtual btCollisionAlgorithmCreateFunc* getClosestPointsAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
};

class GodotSoftCollisionConfiguration :
    public btSoftBodyRigidBodyCollisionConfiguration {
    btCollisionAlgorithmCreateFunc* m_rayWorldCF;
    btCollisionAlgorithmCreateFunc* m_swappedRayWorldCF;

public:
    GodotSoftCollisionConfiguration(
        const btDiscreteDynamicsWorld* world,
        const btDefaultCollisionConstructionInfo& constructionInfo =
            btDefaultCollisionConstructionInfo()
    );
    virtual ~GodotSoftCollisionConfiguration();

    virtual btCollisionAlgorithmCreateFunc* getCollisionAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
    virtual btCollisionAlgorithmCreateFunc* getClosestPointsAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
};
#endif
