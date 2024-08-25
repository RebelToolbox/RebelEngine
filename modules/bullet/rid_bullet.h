// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RID_BULLET_H
#define RID_BULLET_H

#include "core/rid.h"

class BulletPhysicsServer;

class RIDBullet : public RID_Data {
    RID self;
    BulletPhysicsServer* physicsServer;

public:
    _FORCE_INLINE_ void set_self(const RID& p_self) {
        self = p_self;
    }

    _FORCE_INLINE_ RID get_self() const {
        return self;
    }

    _FORCE_INLINE_ void _set_physics_server(BulletPhysicsServer* p_physicsServer
    ) {
        physicsServer = p_physicsServer;
    }

    _FORCE_INLINE_ BulletPhysicsServer* get_physics_server() const {
        return physicsServer;
    }
};

#endif // RID_BULLET_H
