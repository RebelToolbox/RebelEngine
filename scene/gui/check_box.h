// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "scene/gui/button.h"

/**
@author Mariano Suligoy <marianognu.esyrpg@gmail.com>
*/
class CheckBox : public Button {
    GDCLASS(CheckBox, Button);

protected:
    Size2 get_icon_size() const;
    Size2 get_minimum_size() const;
    void _notification(int p_what);

    bool is_radio();

public:
    CheckBox(const String& p_text = String());
    ~CheckBox();
};

#endif
