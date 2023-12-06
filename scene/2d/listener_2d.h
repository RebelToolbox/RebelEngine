// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LISTENER_2D_H
#define LISTENER_2D_H

#include "scene/2d/node_2d.h"
#include "scene/main/viewport.h"

class Listener2D : public Node2D {
    GDCLASS(Listener2D, Node2D);

private:
    bool current = false;

    friend class Viewport;

protected:
    void _update_listener();

    bool _set(const StringName& p_name, const Variant& p_value);
    bool _get(const StringName& p_name, Variant& r_ret) const;
    void _get_property_list(List<PropertyInfo>* p_list) const;
    void _notification(int p_what);

    static void _bind_methods();

public:
    void make_current();
    void clear_current();
    bool is_current() const;
};

#endif
