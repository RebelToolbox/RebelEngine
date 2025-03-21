// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JNI_SINGLETON_H
#define JNI_SINGLETON_H

#ifdef ANDROID_ENABLED
#include "../android_jni_singleton.h"
#else // ! ANDROID_ENABLED

#include "core/engine.h"
#include "core/variant.h"

class JNISingleton : public Object {
    GDCLASS(JNISingleton, Object);

public:
    virtual Variant call(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    ) {
        return Object::call(p_method, p_args, p_argcount, r_error);
    }

    JNISingleton() {}
};

#endif // ! ANDROID_ENABLED

#endif // JNI_SINGLETON_H
