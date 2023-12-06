// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BOX_CONTAINER_H
#define BOX_CONTAINER_H

#include "scene/gui/container.h"

class BoxContainer : public Container {
    GDCLASS(BoxContainer, Container);

public:
    enum AlignMode {
        ALIGN_BEGIN,
        ALIGN_CENTER,
        ALIGN_END
    };

private:
    bool vertical;
    AlignMode align;

    void _resort();

protected:
    void _notification(int p_what);

    static void _bind_methods();

public:
    void add_spacer(bool p_begin = false);

    void set_alignment(AlignMode p_align);
    AlignMode get_alignment() const;

    virtual Size2 get_minimum_size() const;

    BoxContainer(bool p_vertical = false);
};

class HBoxContainer : public BoxContainer {
    GDCLASS(HBoxContainer, BoxContainer);

public:
    HBoxContainer() : BoxContainer(false) {}
};

class MarginContainer;

class VBoxContainer : public BoxContainer {
    GDCLASS(VBoxContainer, BoxContainer);

public:
    MarginContainer* add_margin_child(
        const String& p_label,
        Control* p_control,
        bool p_expand = false
    );

    VBoxContainer() : BoxContainer(true) {}
};

VARIANT_ENUM_CAST(BoxContainer::AlignMode);

#endif // BOX_CONTAINER_H
