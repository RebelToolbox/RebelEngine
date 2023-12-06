// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FUNC_REF_H
#define FUNC_REF_H

#include "core/reference.h"

class FuncRef : public Reference {
    GDCLASS(FuncRef, Reference);
    ObjectID id;
    StringName function;

protected:
    static void _bind_methods();

public:
    Variant call_func(
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );
    Variant call_funcv(const Array& p_args);
    void set_instance(Object* p_obj);
    void set_function(const StringName& p_func);
    StringName get_function();
    bool is_valid() const;
    FuncRef();
};

#endif // FUNC_REF_H
