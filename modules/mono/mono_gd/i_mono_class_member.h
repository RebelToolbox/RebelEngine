// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef I_MONO_CLASS_MEMBER_H
#define I_MONO_CLASS_MEMBER_H

#include "gd_mono_header.h"

#include <mono/metadata/object.h>

class IMonoClassMember {
public:
    enum Visibility {
        PRIVATE,
        PROTECTED_AND_INTERNAL, // FAM_AND_ASSEM
        INTERNAL,               // ASSEMBLY
        PROTECTED,              // FAMILY
        PUBLIC
    };

    enum MemberType {
        MEMBER_TYPE_FIELD,
        MEMBER_TYPE_PROPERTY,
        MEMBER_TYPE_METHOD
    };

    virtual ~IMonoClassMember() {}

    virtual GDMonoClass* get_enclosing_class() const = 0;

    virtual MemberType get_member_type() const = 0;

    virtual StringName get_name() const = 0;

    virtual bool is_static() = 0;

    virtual Visibility get_visibility() = 0;

    virtual bool has_attribute(GDMonoClass* p_attr_class)        = 0;
    virtual MonoObject* get_attribute(GDMonoClass* p_attr_class) = 0;
};

#endif // I_MONO_CLASS_MEMBER_H
