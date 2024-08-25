// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "constraint_bullet.h"

#include "collision_object_bullet.h"
#include "space_bullet.h"

ConstraintBullet::ConstraintBullet() :
    space(nullptr),
    constraint(nullptr),
    disabled_collisions_between_bodies(true) {}

void ConstraintBullet::setup(btTypedConstraint* p_constraint) {
    constraint = p_constraint;
    constraint->setUserConstraintPtr(this);
}

void ConstraintBullet::set_space(SpaceBullet* p_space) {
    space = p_space;
}

void ConstraintBullet::destroy_internal_constraint() {
    space->remove_constraint(this);
}

void ConstraintBullet::disable_collisions_between_bodies(const bool p_disabled
) {
    disabled_collisions_between_bodies = p_disabled;

    if (space) {
        space->remove_constraint(this);
        space->add_constraint(this, disabled_collisions_between_bodies);
    }
}
