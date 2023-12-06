// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "godot_collision_dispatcher.h"

#include "collision_object_bullet.h"

/**
    @author AndreaCatania
*/

const int GodotCollisionDispatcher::CASTED_TYPE_AREA =
    static_cast<int>(CollisionObjectBullet::TYPE_AREA);

GodotCollisionDispatcher::GodotCollisionDispatcher(
    btCollisionConfiguration* collisionConfiguration
) :
    btCollisionDispatcher(collisionConfiguration) {}

bool GodotCollisionDispatcher::needsCollision(
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

bool GodotCollisionDispatcher::needsResponse(
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
