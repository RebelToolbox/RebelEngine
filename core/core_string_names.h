// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CORE_STRING_NAMES_H
#define CORE_STRING_NAMES_H

#include "core/string_name.h"

class CoreStringNames {
    friend void register_core_types();
    friend void unregister_core_types();

    static void create() {
        singleton = memnew(CoreStringNames);
    }

    static void free() {
        memdelete(singleton);
        singleton = nullptr;
    }

    CoreStringNames();

public:
    _FORCE_INLINE_ static CoreStringNames* get_singleton() {
        return singleton;
    }

    static CoreStringNames* singleton;

    StringName _free;
    StringName changed;
    StringName _meta;
    StringName _script;
    StringName script_changed;
    StringName ___pdcdata;
    StringName __getvar;
    StringName _iter_init;
    StringName _iter_next;
    StringName _iter_get;
    StringName get_rid;
    StringName _to_string;
#ifdef TOOLS_ENABLED
    StringName _sections_unfolded;
#endif
    StringName _custom_features;

    StringName x;
    StringName y;
    StringName z;
    StringName w;
    StringName r;
    StringName g;
    StringName b;
    StringName a;
    StringName position;
    StringName size;
    StringName end;
    StringName basis;
    StringName origin;
    StringName normal;
    StringName d;
    StringName h;
    StringName s;
    StringName v;
    StringName r8;
    StringName g8;
    StringName b8;
    StringName a8;
};

#endif // SCENE_STRING_NAMES_H
