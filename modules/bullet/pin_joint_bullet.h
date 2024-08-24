// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PIN_JOINT_BULLET_H
#define PIN_JOINT_BULLET_H

#include "joint_bullet.h"

class RigidBodyBullet;

class PinJointBullet : public JointBullet {
    class btPoint2PointConstraint* p2pConstraint;

public:
    PinJointBullet(
        RigidBodyBullet* p_body_a,
        const Vector3& p_pos_a,
        RigidBodyBullet* p_body_b,
        const Vector3& p_pos_b
    );
    ~PinJointBullet();

    virtual PhysicsServer::JointType get_type() const {
        return PhysicsServer::JOINT_PIN;
    }

    void set_param(PhysicsServer::PinJointParam p_param, real_t p_value);
    real_t get_param(PhysicsServer::PinJointParam p_param) const;

    void setPivotInA(const Vector3& p_pos);
    void setPivotInB(const Vector3& p_pos);

    Vector3 getPivotInA();
    Vector3 getPivotInB();
};

#endif // PIN_JOINT_BULLET_H
