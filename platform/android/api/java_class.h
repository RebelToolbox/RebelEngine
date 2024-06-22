// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JAVA_CLASS_H
#define JAVA_CLASS_H

#ifdef ANDROID_ENABLED
#include "../android_java_class.h"
#else // ! ANDROID_ENABLED

#include "core/reference.h"

class JavaClass : public Reference {
    GDCLASS(JavaClass, Reference);

public:
    virtual Variant call(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );
    JavaClass();
};

class JavaObject : public Reference {
    GDCLASS(JavaObject, Reference);

public:
    virtual Variant call(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );
};

class JavaClassWrapper : public Object {
    GDCLASS(JavaClassWrapper, Object);
    static JavaClassWrapper* singleton;

protected:
    static void _bind_methods();

public:
    static JavaClassWrapper* get_singleton() {
        return singleton;
    }

    Ref<JavaClass> wrap(const String& p_class);
    JavaClassWrapper();
};

#endif // ! ANDROID_ENABLED

#endif // JAVA_CLASS_H
