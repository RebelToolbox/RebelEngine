// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CHECK_BUTTON_H
#define CHECK_BUTTON_H

#include "scene/gui/button.h"

/**
@author Juan Linietsky <reduzio@gmail.com>
*/
class CheckButton : public Button {
    GDCLASS(CheckButton, Button);

protected:
    Size2 get_icon_size() const;
    virtual Size2 get_minimum_size() const;
    void _notification(int p_what);

public:
    CheckButton();
    ~CheckButton();
};

#endif
