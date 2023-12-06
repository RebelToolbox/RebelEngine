// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include <alloca.h>

#define GLES2_INCLUDE_H <ES2/gl.h>
#define GLES3_INCLUDE_H <ES3/gl.h>

#define PLATFORM_REFCOUNT

#define PTHREAD_RENAME_SELF

#define _weakify(var) __weak typeof(var) GDWeak_##var = var;
#define _strongify(var)                                                        \
    _Pragma("clang diagnostic push")                                           \
        _Pragma("clang diagnostic ignored \"-Wshadow\"")                       \
            __strong typeof(var) var = GDWeak_##var;                           \
    _Pragma("clang diagnostic pop")
