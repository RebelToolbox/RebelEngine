// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_DISPATCHER_H
#define COLLISION_DISPATCHER_H

#include "core/int_types.h"

#include <btBulletDynamicsCommon.h>

// This class is required to implement custom collision behaviour in the
// narrowphase
class CollisionDispatcher : public btCollisionDispatcher {
private:
    static const int CASTED_TYPE_AREA;

public:
    CollisionDispatcher(btCollisionConfiguration* collisionConfiguration);
    virtual bool needsCollision(
        const btCollisionObject* body0,
        const btCollisionObject* body1
    );
    virtual bool needsResponse(
        const btCollisionObject* body0,
        const btCollisionObject* body1
    );
};

#endif // COLLISION_DISPATCHER_H
