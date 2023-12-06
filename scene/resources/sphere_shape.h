// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H

#include "scene/resources/shape.h"

class SphereShape : public Shape {
    GDCLASS(SphereShape, Shape);
    float radius;

protected:
    static void _bind_methods();

    virtual void _update_shape();

public:
    void set_radius(float p_radius);
    float get_radius() const;

    virtual Vector<Vector3> get_debug_mesh_lines();

    SphereShape();
};

#endif // SPHERE_SHAPE_H
