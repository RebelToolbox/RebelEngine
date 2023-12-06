// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PANEL_CONTAINER_H
#define PANEL_CONTAINER_H

#include "scene/gui/container.h"

class PanelContainer : public Container {
    GDCLASS(PanelContainer, Container);

protected:
    void _notification(int p_what);

public:
    virtual Size2 get_minimum_size() const;

    PanelContainer();
};

#endif // PANEL_CONTAINER_H
