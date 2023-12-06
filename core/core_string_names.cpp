// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "core_string_names.h"

CoreStringNames* CoreStringNames::singleton = nullptr;

CoreStringNames::CoreStringNames() :
    _free(StaticCString::create("free")),
    changed(StaticCString::create("changed")),
    _meta(StaticCString::create("__meta__")),
    _script(StaticCString::create("script")),
    script_changed(StaticCString::create("script_changed")),
    ___pdcdata(StaticCString::create("___pdcdata")),
    __getvar(StaticCString::create("__getvar")),
    _iter_init(StaticCString::create("_iter_init")),
    _iter_next(StaticCString::create("_iter_next")),
    _iter_get(StaticCString::create("_iter_get")),
    get_rid(StaticCString::create("get_rid")),
    _to_string(StaticCString::create("_to_string")),
#ifdef TOOLS_ENABLED
    _sections_unfolded(StaticCString::create("_sections_unfolded")),
#endif
    _custom_features(StaticCString::create("_custom_features")),
    x(StaticCString::create("x")),
    y(StaticCString::create("y")),
    z(StaticCString::create("z")),
    w(StaticCString::create("w")),
    r(StaticCString::create("r")),
    g(StaticCString::create("g")),
    b(StaticCString::create("b")),
    a(StaticCString::create("a")),
    position(StaticCString::create("position")),
    size(StaticCString::create("size")),
    end(StaticCString::create("end")),
    basis(StaticCString::create("basis")),
    origin(StaticCString::create("origin")),
    normal(StaticCString::create("normal")),
    d(StaticCString::create("d")),
    h(StaticCString::create("h")),
    s(StaticCString::create("s")),
    v(StaticCString::create("v")),
    r8(StaticCString::create("r8")),
    g8(StaticCString::create("g8")),
    b8(StaticCString::create("b8")),
    a8(StaticCString::create("a8")) {
}
