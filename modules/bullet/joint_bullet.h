// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JOINT_BULLET_H
#define JOINT_BULLET_H

#include "constraint_bullet.h"
#include "servers/physics_server.h"

class RigidBodyBullet;
class btTypedConstraint;

class JointBullet : public ConstraintBullet {
public:
    JointBullet();
    virtual ~JointBullet();

    virtual PhysicsServer::JointType get_type() const = 0;
};

#endif // JOINT_BULLET_H
