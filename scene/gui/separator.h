// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SEPARATOR_H
#define SEPARATOR_H

#include "scene/gui/control.h"

class Separator : public Control {
    GDCLASS(Separator, Control);

protected:
    Orientation orientation;
    void _notification(int p_what);

public:
    virtual Size2 get_minimum_size() const;

    Separator();
    ~Separator();
};

class VSeparator : public Separator {
    GDCLASS(VSeparator, Separator);

public:
    VSeparator();
};

class HSeparator : public Separator {
    GDCLASS(HSeparator, Separator);

public:
    HSeparator();
};

#endif
