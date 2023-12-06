// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CENTER_CONTAINER_H
#define CENTER_CONTAINER_H

#include "scene/gui/container.h"

class CenterContainer : public Container {
    GDCLASS(CenterContainer, Container);

    bool use_top_left;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_use_top_left(bool p_enable);
    bool is_using_top_left() const;

    virtual Size2 get_minimum_size() const;

    CenterContainer();
};

#endif // CENTER_CONTAINER_H
