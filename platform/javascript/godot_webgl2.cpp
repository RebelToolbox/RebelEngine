// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "godot_webgl2.h"

extern "C" {
extern void rebel_js_display_glGetBufferSubData(
    GLenum target,
    GLintptr offset,
    GLsizeiptr size,
    GLvoid* data
);
}

void glGetBufferSubData(
    GLenum target,
    GLintptr offset,
    GLsizeiptr size,
    GLvoid* data
) {
    rebel_js_display_glGetBufferSubData(target, offset, size, data);
}
