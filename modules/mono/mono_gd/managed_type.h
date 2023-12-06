// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MANAGED_TYPE_H
#define MANAGED_TYPE_H

#include "gd_mono_header.h"

#include <mono/metadata/object.h>

struct ManagedType {
    int type_encoding;
    GDMonoClass* type_class;

    static ManagedType from_class(GDMonoClass* p_class);
    static ManagedType from_class(MonoClass* p_mono_class);
    static ManagedType from_type(MonoType* p_mono_type);
    static ManagedType from_reftype(MonoReflectionType* p_mono_reftype);

    ManagedType() : type_encoding(0), type_class(NULL) {}

    ManagedType(int p_type_encoding, GDMonoClass* p_type_class) :
        type_encoding(p_type_encoding),
        type_class(p_type_class) {}
};

#endif // MANAGED_TYPE_H
