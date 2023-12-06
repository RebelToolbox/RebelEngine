// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef OCCLUDER_H
#define OCCLUDER_H

#include "scene/3d/spatial.h"
#include "scene/resources/occluder_shape.h"

class Occluder : public Spatial {
    GDCLASS(Occluder, Spatial);

    friend class OccluderSpatialGizmo;
    friend class OccluderEditorPlugin;

    Ref<OccluderShape> _shape;

    void resource_changed(RES res);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_shape(const Ref<OccluderShape>& p_shape);
    Ref<OccluderShape> get_shape() const;

    String get_configuration_warning() const;

    Occluder();
    ~Occluder();
};

#endif // OCCLUDER_H
