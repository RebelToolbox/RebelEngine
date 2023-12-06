// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "editor_scale.h"

#include "core/os/os.h"

static float scale = 1.0;

void editor_set_scale(float p_scale) {
    scale = p_scale;
}

float editor_get_scale() {
    return scale;
}
