// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CANVASMODULATE_H
#define CANVASMODULATE_H

#include "scene/2d/node_2d.h"

class CanvasModulate : public Node2D {
    GDCLASS(CanvasModulate, Node2D);

    Color color;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_color(const Color& p_color);
    Color get_color() const;

    String get_configuration_warning() const;

    CanvasModulate();
    ~CanvasModulate();
};

#endif // CANVASMODULATE_H
