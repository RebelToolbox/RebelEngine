// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PARALLAX_LAYER_H
#define PARALLAX_LAYER_H

#include "scene/2d/node_2d.h"

class ParallaxLayer : public Node2D {
    GDCLASS(ParallaxLayer, Node2D);

    Point2 orig_offset;
    Point2 orig_scale;
    Size2 motion_scale;
    Vector2 motion_offset;
    Vector2 mirroring;
    void _update_mirroring();

    Point2 screen_offset;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_motion_offset(const Size2& p_offset);
    Size2 get_motion_offset() const;

    void set_motion_scale(const Size2& p_scale);
    Size2 get_motion_scale() const;

    void set_mirroring(const Size2& p_mirroring);
    Size2 get_mirroring() const;

    void set_base_offset_and_scale(
        const Point2& p_offset,
        float p_scale,
        const Point2& p_screen_offset
    );

    virtual String get_configuration_warning() const;
    ParallaxLayer();
};

#endif // PARALLAX_LAYER_H
