// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REMOTE_TRANSFORM_2D_H
#define REMOTE_TRANSFORM_2D_H

#include "scene/2d/node_2d.h"

class RemoteTransform2D : public Node2D {
    GDCLASS(RemoteTransform2D, Node2D);

    NodePath remote_node;

    ObjectID cache;

    bool use_global_coordinates;
    bool update_remote_position;
    bool update_remote_rotation;
    bool update_remote_scale;

    void _update_remote();
    void _update_cache();
    // void _node_exited_scene();

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    void set_remote_node(const NodePath& p_remote_node);
    NodePath get_remote_node() const;

    void set_use_global_coordinates(const bool p_enable);
    bool get_use_global_coordinates() const;

    void set_update_position(const bool p_update);
    bool get_update_position() const;

    void set_update_rotation(const bool p_update);
    bool get_update_rotation() const;

    void set_update_scale(const bool p_update);
    bool get_update_scale() const;

    void force_update_cache();

    virtual String get_configuration_warning() const;

    RemoteTransform2D();
};

#endif // REMOTE_TRANSFORM_2D_H
