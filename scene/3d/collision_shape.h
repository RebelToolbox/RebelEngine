// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include "scene/3d/spatial.h"
#include "scene/resources/shape.h"
class CollisionObject;

class CollisionShape : public Spatial {
    GDCLASS(CollisionShape, Spatial);
    OBJ_CATEGORY("3D Physics Nodes");

    Ref<Shape> shape;

    uint32_t owner_id;
    CollisionObject* parent;

    void resource_changed(RES res);
    bool disabled;

protected:
    void _update_in_shape_owner(bool p_xform_only = false);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void make_convex_from_brothers();

    void set_shape(const Ref<Shape>& p_shape);
    Ref<Shape> get_shape() const;

    void set_disabled(bool p_disabled);
    bool is_disabled() const;

    String get_configuration_warning() const;

    CollisionShape();
    ~CollisionShape();
};

#endif // BODY_VOLUME_H
