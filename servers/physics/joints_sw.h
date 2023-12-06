// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JOINTS_SW_H
#define JOINTS_SW_H

#include "body_sw.h"
#include "constraint_sw.h"

class JointSW : public ConstraintSW {
public:
    virtual PhysicsServer::JointType get_type() const = 0;

    _FORCE_INLINE_ JointSW(
        BodySW** p_body_ptr = nullptr,
        int p_body_count    = 0
    ) :
        ConstraintSW(p_body_ptr, p_body_count) {}
};

#endif // JOINTS_SW_H
