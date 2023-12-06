// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "managed_type.h"

#include "gd_mono.h"
#include "gd_mono_class.h"

ManagedType ManagedType::from_class(GDMonoClass* p_class) {
    return ManagedType(mono_type_get_type(p_class->get_mono_type()), p_class);
}

ManagedType ManagedType::from_class(MonoClass* p_mono_class) {
    GDMonoClass* tclass = GDMono::get_singleton()->get_class(p_mono_class);
    ERR_FAIL_COND_V(!tclass, ManagedType());

    return ManagedType(mono_type_get_type(tclass->get_mono_type()), tclass);
}

ManagedType ManagedType::from_type(MonoType* p_mono_type) {
    MonoClass* mono_class = mono_class_from_mono_type(p_mono_type);
    GDMonoClass* tclass   = GDMono::get_singleton()->get_class(mono_class);
    ERR_FAIL_COND_V(!tclass, ManagedType());

    return ManagedType(mono_type_get_type(p_mono_type), tclass);
}

ManagedType ManagedType::from_reftype(MonoReflectionType* p_mono_reftype) {
    MonoType* mono_type = mono_reflection_type_get_type(p_mono_reftype);
    return from_type(mono_type);
}
