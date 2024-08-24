// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONSTRAINT_BULLET_H
#define CONSTRAINT_BULLET_H

#include "bullet_utilities.h"
#include "rid_bullet.h"

#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

class RigidBodyBullet;
class SpaceBullet;
class btTypedConstraint;

class ConstraintBullet : public RIDBullet {
protected:
    SpaceBullet* space;
    btTypedConstraint* constraint;
    bool disabled_collisions_between_bodies;

public:
    ConstraintBullet();

    virtual void setup(btTypedConstraint* p_constraint);
    virtual void set_space(SpaceBullet* p_space);
    virtual void destroy_internal_constraint();

    void disable_collisions_between_bodies(const bool p_disabled);

    _FORCE_INLINE_ bool is_disabled_collisions_between_bodies() const {
        return disabled_collisions_between_bodies;
    }

public:
    virtual ~ConstraintBullet() {
        bulletdelete(constraint);
        constraint = nullptr;
    }

    _FORCE_INLINE_ btTypedConstraint* get_bt_constraint() {
        return constraint;
    }
};

#endif // CONSTRAINT_BULLET_H
