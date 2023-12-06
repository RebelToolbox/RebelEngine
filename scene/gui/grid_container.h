// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GRID_CONTAINER_H
#define GRID_CONTAINER_H

#include "scene/gui/container.h"

class GridContainer : public Container {
    GDCLASS(GridContainer, Container);

    int columns;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_columns(int p_columns);
    int get_columns() const;
    virtual Size2 get_minimum_size() const;

    GridContainer();
};

#endif // GRID_CONTAINER_H
