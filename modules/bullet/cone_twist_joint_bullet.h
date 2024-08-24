// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONE_TWIST_JOINT_BULLET_H
#define CONE_TWIST_JOINT_BULLET_H

#include "joint_bullet.h"

class RigidBodyBullet;

class ConeTwistJointBullet : public JointBullet {
    class btConeTwistConstraint* coneConstraint;

public:
    ConeTwistJointBullet(
        RigidBodyBullet* rbA,
        RigidBodyBullet* rbB,
        const Transform& rbAFrame,
        const Transform& rbBFrame
    );

    virtual PhysicsServer::JointType get_type() const {
        return PhysicsServer::JOINT_CONE_TWIST;
    }

    void set_param(PhysicsServer::ConeTwistJointParam p_param, real_t p_value);
    real_t get_param(PhysicsServer::ConeTwistJointParam p_param) const;
};

#endif // CONE_TWIST_JOINT_BULLET_H
