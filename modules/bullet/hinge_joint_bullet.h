// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef HINGE_JOINT_BULLET_H
#define HINGE_JOINT_BULLET_H

#include "joint_bullet.h"

/**
    @author AndreaCatania
*/

class HingeJointBullet : public JointBullet {
    class btHingeConstraint* hingeConstraint;

public:
    HingeJointBullet(
        RigidBodyBullet* rbA,
        RigidBodyBullet* rbB,
        const Transform& frameA,
        const Transform& frameB
    );
    HingeJointBullet(
        RigidBodyBullet* rbA,
        RigidBodyBullet* rbB,
        const Vector3& pivotInA,
        const Vector3& pivotInB,
        const Vector3& axisInA,
        const Vector3& axisInB
    );

    virtual PhysicsServer::JointType get_type() const {
        return PhysicsServer::JOINT_HINGE;
    }

    real_t get_hinge_angle();

    void set_param(PhysicsServer::HingeJointParam p_param, real_t p_value);
    real_t get_param(PhysicsServer::HingeJointParam p_param) const;

    void set_flag(PhysicsServer::HingeJointFlag p_flag, bool p_value);
    bool get_flag(PhysicsServer::HingeJointFlag p_flag) const;
};
#endif
