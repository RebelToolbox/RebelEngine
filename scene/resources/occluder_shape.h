// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef OCCLUDER_SHAPE_H
#define OCCLUDER_SHAPE_H

#include "core/math/plane.h"
#include "core/resource.h"
#include "core/vector.h"

class OccluderShape : public Resource {
    GDCLASS(OccluderShape, Resource);
    OBJ_SAVE_TYPE(OccluderShape);
    RES_BASE_EXTENSION("occ");
    RID _shape;

protected:
    static void _bind_methods();

    RID get_shape() const {
        return _shape;
    }

    OccluderShape(RID p_shape);

public:
    virtual RID get_rid() const {
        return _shape;
    }

    ~OccluderShape();

    virtual void notification_enter_world(RID p_scenario) = 0;
    virtual void update_shape_to_visual_server()          = 0;
    void update_transform_to_visual_server(const Transform& p_global_xform);
    void update_active_to_visual_server(bool p_active);
    void notification_exit_world();
    virtual Transform center_node(
        const Transform& p_global_xform,
        const Transform& p_parent_xform,
        real_t p_snap
    ) = 0;
};

class OccluderShapeSphere : public OccluderShape {
    GDCLASS(OccluderShapeSphere, OccluderShape);

    // We bandit a plane to store position / radius
    Vector<Plane> _spheres;
    const real_t _min_radius = 0.1;

protected:
    static void _bind_methods();

public:
    void set_spheres(const Vector<Plane>& p_spheres);

    Vector<Plane> get_spheres() const {
        return _spheres;
    }

    void set_sphere_position(int p_idx, const Vector3& p_position);
    void set_sphere_radius(int p_idx, real_t p_radius);

    virtual void notification_enter_world(RID p_scenario);
    virtual void update_shape_to_visual_server();
    virtual Transform center_node(
        const Transform& p_global_xform,
        const Transform& p_parent_xform,
        real_t p_snap
    );

    OccluderShapeSphere();
};

#endif // OCCLUDER_SHAPE_H
