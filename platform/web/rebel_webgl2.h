// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REBEL_WEBGL2_H
#define REBEL_WEBGL2_H

#include "GLES3/gl3.h"

// We could include "webgl/webgl2.h", but old (< 2.0.17) emscripten versions do
// not have it, so use our own wrapper instead.
void glGetBufferSubData(
    GLenum target,
    GLintptr offset,
    GLsizeiptr size,
    GLvoid* data
);

#endif // REBEL_WEBGL2_H
