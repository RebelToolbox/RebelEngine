// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SHAPE_OWNER_BULLET_H
#define SHAPE_OWNER_BULLET_H

#include "rid_bullet.h"

class ShapeBullet;
class btCollisionShape;
class CollisionObjectBullet;

/// Each class that want to use Shapes must inherit this class
/// E.G. BodyShape is a child of this
class ShapeOwnerBullet {
public:
    virtual int find_shape(ShapeBullet* p_shape) const         = 0;
    virtual void shape_changed(int p_shape_index)              = 0;
    virtual void reload_shapes()                               = 0;
    virtual void remove_shape_full(class ShapeBullet* p_shape) = 0;

    virtual ~ShapeOwnerBullet() {}
};

#endif // SHAPE_OWNER_BULLET_H
