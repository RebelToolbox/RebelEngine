// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MESH_INSTANCE_2D_H
#define MESH_INSTANCE_2D_H

#include "scene/2d/node_2d.h"

class MeshInstance2D : public Node2D {
    GDCLASS(MeshInstance2D, Node2D);

    Ref<Mesh> mesh;

    Ref<Texture> texture;
    Ref<Texture> normal_map;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
    virtual Rect2 _edit_get_rect() const;
    virtual bool _edit_use_rect() const;
#endif

    void set_mesh(const Ref<Mesh>& p_mesh);
    Ref<Mesh> get_mesh() const;

    void set_texture(const Ref<Texture>& p_texture);
    Ref<Texture> get_texture() const;

    void set_normal_map(const Ref<Texture>& p_texture);
    Ref<Texture> get_normal_map() const;

    MeshInstance2D();
};

#endif // MESH_INSTANCE_2D_H
