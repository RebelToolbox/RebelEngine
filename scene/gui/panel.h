// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PANEL_H
#define PANEL_H

#include "scene/gui/control.h"

class Panel : public Control {
    GDCLASS(Panel, Control);

protected:
    void _notification(int p_what);

public:
    Panel();
    ~Panel();
};

#endif
