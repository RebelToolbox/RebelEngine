// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H

#include "scene/resources/shape.h"

class BoxShape : public Shape {
    GDCLASS(BoxShape, Shape);
    Vector3 extents;

protected:
    static void _bind_methods();

    virtual void _update_shape();

public:
    void set_extents(const Vector3& p_extents);
    Vector3 get_extents() const;

    virtual Vector<Vector3> get_debug_mesh_lines();

    BoxShape();
};

#endif // BOX_SHAPE_H
