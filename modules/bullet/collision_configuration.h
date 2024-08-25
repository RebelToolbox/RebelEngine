// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_CONFIGURATION_H
#define COLLISION_CONFIGURATION_H

#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>

class btDiscreteDynamicsWorld;

class CollisionConfiguration : public btDefaultCollisionConfiguration {
    btCollisionAlgorithmCreateFunc* m_rayWorldCF;
    btCollisionAlgorithmCreateFunc* m_swappedRayWorldCF;

public:
    CollisionConfiguration(
        const btDiscreteDynamicsWorld* world,
        const btDefaultCollisionConstructionInfo& constructionInfo =
            btDefaultCollisionConstructionInfo()
    );
    virtual ~CollisionConfiguration();

    virtual btCollisionAlgorithmCreateFunc* getCollisionAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
    virtual btCollisionAlgorithmCreateFunc* getClosestPointsAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
};

class SoftCollisionConfiguration :
    public btSoftBodyRigidBodyCollisionConfiguration {
    btCollisionAlgorithmCreateFunc* m_rayWorldCF;
    btCollisionAlgorithmCreateFunc* m_swappedRayWorldCF;

public:
    SoftCollisionConfiguration(
        const btDiscreteDynamicsWorld* world,
        const btDefaultCollisionConstructionInfo& constructionInfo =
            btDefaultCollisionConstructionInfo()
    );
    virtual ~SoftCollisionConfiguration();

    virtual btCollisionAlgorithmCreateFunc* getCollisionAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
    virtual btCollisionAlgorithmCreateFunc* getClosestPointsAlgorithmCreateFunc(
        int proxyType0,
        int proxyType1
    );
};

#endif // COLLISION_CONFIGURATION_H
