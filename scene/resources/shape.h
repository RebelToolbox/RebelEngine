// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SHAPE_H
#define SHAPE_H

#include "core/resource.h"
class ArrayMesh;

class Shape : public Resource {
    GDCLASS(Shape, Resource);
    OBJ_SAVE_TYPE(Shape);
    RES_BASE_EXTENSION("shape");
    RID shape;
    real_t margin;

    Ref<ArrayMesh> debug_mesh_cache;

protected:
    static void _bind_methods();

    _FORCE_INLINE_ RID get_shape() const {
        return shape;
    }

    Shape(RID p_shape);

    virtual void _update_shape();

public:
    virtual RID get_rid() const {
        return shape;
    }

    Ref<ArrayMesh> get_debug_mesh();
    virtual Vector<Vector3> get_debug_mesh_lines(
    ) = 0; // { return Vector<Vector3>(); }

    void add_vertices_to_array(
        PoolVector<Vector3>& array,
        const Transform& p_xform
    );

    real_t get_margin() const;
    void set_margin(real_t p_margin);

    Shape();
    ~Shape();
};

#endif // SHAPE_H
