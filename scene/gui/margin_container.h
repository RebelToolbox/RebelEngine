// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MARGIN_CONTAINER_H
#define MARGIN_CONTAINER_H

#include "scene/gui/container.h"

class MarginContainer : public Container {
    GDCLASS(MarginContainer, Container);

protected:
    void _notification(int p_what);

public:
    virtual Size2 get_minimum_size() const;

    MarginContainer();
};

#endif // MARGIN_CONTAINER_H
