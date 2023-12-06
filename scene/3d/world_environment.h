// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SCENARIO_FX_H
#define SCENARIO_FX_H

#include "scene/3d/spatial.h"

class WorldEnvironment : public Node {
    GDCLASS(WorldEnvironment, Node);

    Ref<Environment> environment;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_environment(const Ref<Environment>& p_environment);
    Ref<Environment> get_environment() const;

    String get_configuration_warning() const;

    WorldEnvironment();
};

#endif
