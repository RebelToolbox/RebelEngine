// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RAY_WORLD_ALGORITHM_H
#define RAY_WORLD_ALGORITHM_H

#include <BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionCreateFunc.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>

class btDiscreteDynamicsWorld;

class RayWorldAlgorithm : public btActivatingCollisionAlgorithm {
    const btDiscreteDynamicsWorld* m_world;
    btPersistentManifold* m_manifoldPtr;
    bool m_ownManifold;
    bool m_isSwapped;

public:
    RayWorldAlgorithm(
        const btDiscreteDynamicsWorld* world,
        btPersistentManifold* mf,
        const btCollisionAlgorithmConstructionInfo& ci,
        const btCollisionObjectWrapper* body0Wrap,
        const btCollisionObjectWrapper* body1Wrap,
        bool isSwapped
    );
    virtual ~RayWorldAlgorithm();

    virtual void processCollision(
        const btCollisionObjectWrapper* body0Wrap,
        const btCollisionObjectWrapper* body1Wrap,
        const btDispatcherInfo& dispatchInfo,
        btManifoldResult* resultOut
    );
    virtual btScalar calculateTimeOfImpact(
        btCollisionObject* body0,
        btCollisionObject* body1,
        const btDispatcherInfo& dispatchInfo,
        btManifoldResult* resultOut
    );

    virtual void getAllContactManifolds(btManifoldArray& manifoldArray) {
        /// should we use m_ownManifold to avoid adding duplicates?
        if (m_manifoldPtr && m_ownManifold) {
            manifoldArray.push_back(m_manifoldPtr);
        }
    }

    struct CreateFunc : public btCollisionAlgorithmCreateFunc {
        const btDiscreteDynamicsWorld* m_world;
        CreateFunc(const btDiscreteDynamicsWorld* world);

        virtual btCollisionAlgorithm* CreateCollisionAlgorithm(
            btCollisionAlgorithmConstructionInfo& ci,
            const btCollisionObjectWrapper* body0Wrap,
            const btCollisionObjectWrapper* body1Wrap
        ) {
            void* mem = ci.m_dispatcher1->allocateCollisionAlgorithm(
                sizeof(RayWorldAlgorithm)
            );
            return new (mem) RayWorldAlgorithm(
                m_world,
                ci.m_manifold,
                ci,
                body0Wrap,
                body1Wrap,
                false
            );
        }
    };

    struct SwappedCreateFunc : public btCollisionAlgorithmCreateFunc {
        const btDiscreteDynamicsWorld* m_world;
        SwappedCreateFunc(const btDiscreteDynamicsWorld* world);

        virtual btCollisionAlgorithm* CreateCollisionAlgorithm(
            btCollisionAlgorithmConstructionInfo& ci,
            const btCollisionObjectWrapper* body0Wrap,
            const btCollisionObjectWrapper* body1Wrap
        ) {
            void* mem = ci.m_dispatcher1->allocateCollisionAlgorithm(
                sizeof(RayWorldAlgorithm)
            );
            return new (mem) RayWorldAlgorithm(
                m_world,
                ci.m_manifold,
                ci,
                body0Wrap,
                body1Wrap,
                true
            );
        }
    };
};

#endif // RAY_WORLD_ALGORITHM_H
