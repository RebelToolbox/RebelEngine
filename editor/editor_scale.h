// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_SCALE_H
#define EDITOR_SCALE_H

void editor_set_scale(float p_scale);
float editor_get_scale();

#define EDSCALE (editor_get_scale())
#endif // EDITOR_SCALE_H
