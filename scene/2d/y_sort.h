// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef Y_SORT_H
#define Y_SORT_H

#include "scene/2d/node_2d.h"

class YSort : public Node2D {
    GDCLASS(YSort, Node2D);
    bool sort_enabled;
    static void _bind_methods();

public:
    void set_sort_enabled(bool p_enabled);
    bool is_sort_enabled() const;
    YSort();
};

#endif // Y_SORT_H
