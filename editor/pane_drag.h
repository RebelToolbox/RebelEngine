// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PANE_DRAG_H
#define PANE_DRAG_H

#include "scene/gui/control.h"

class PaneDrag : public Control {
    GDCLASS(PaneDrag, Control);

    bool mouse_over;

protected:
    void _gui_input(const Ref<InputEvent>& p_input);
    void _notification(int p_what);
    virtual Size2 get_minimum_size() const;
    static void _bind_methods();

public:
    PaneDrag();
};

#endif // PANE_DRAG_H
