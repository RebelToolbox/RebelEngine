// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef POSITION_2D_H
#define POSITION_2D_H

#include "scene/2d/node_2d.h"

class Position2D : public Node2D {
    GDCLASS(Position2D, Node2D);

    void _draw_cross();

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
    virtual Rect2 _edit_get_rect() const;
    virtual bool _edit_use_rect() const;
#endif

    void set_gizmo_extents(float p_extents);
    float get_gizmo_extents() const;

    Position2D();
};

#endif // POSITION_2D_H
