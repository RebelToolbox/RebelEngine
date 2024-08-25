// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "collision_dispatcher.h"

#include "collision_object_bullet.h"

const int CollisionDispatcher::CASTED_TYPE_AREA =
    static_cast<int>(CollisionObjectBullet::TYPE_AREA);

CollisionDispatcher::CollisionDispatcher(
    btCollisionConfiguration* collisionConfiguration
) :
    btCollisionDispatcher(collisionConfiguration) {}

bool CollisionDispatcher::needsCollision(
    const btCollisionObject* body0,
    const btCollisionObject* body1
) {
    if (body0->getUserIndex() == CASTED_TYPE_AREA
        || body1->getUserIndex() == CASTED_TYPE_AREA) {
        // Avoide area narrow phase
        return false;
    }
    return btCollisionDispatcher::needsCollision(body0, body1);
}

bool CollisionDispatcher::needsResponse(
    const btCollisionObject* body0,
    const btCollisionObject* body1
) {
    if (body0->getUserIndex() == CASTED_TYPE_AREA
        || body1->getUserIndex() == CASTED_TYPE_AREA) {
        // Avoide area narrow phase
        return false;
    }
    return btCollisionDispatcher::needsResponse(body0, body1);
}
