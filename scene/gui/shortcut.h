// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SHORTCUT_H
#define SHORTCUT_H

#include "core/os/input_event.h"
#include "core/resource.h"

class ShortCut : public Resource {
    GDCLASS(ShortCut, Resource);

    Ref<InputEvent> shortcut;

protected:
    static void _bind_methods();

public:
    void set_shortcut(const Ref<InputEvent>& p_shortcut);
    Ref<InputEvent> get_shortcut() const;
    bool is_shortcut(const Ref<InputEvent>& p_event) const;
    bool is_valid() const;

    String get_as_text() const;

    ShortCut();
};

#endif // SHORTCUT_H
