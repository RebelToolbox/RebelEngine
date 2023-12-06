// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JAVASCRIPT_SINGLETON_H
#define JAVASCRIPT_SINGLETON_H

#include "core/object.h"
#include "core/reference.h"

class JavaScriptObject : public Reference {
private:
    GDCLASS(JavaScriptObject, Reference);

protected:
    virtual bool _set(const StringName& p_name, const Variant& p_value) {
        return false;
    }

    virtual bool _get(const StringName& p_name, Variant& r_ret) const {
        return false;
    }

    virtual void _get_property_list(List<PropertyInfo>* p_list) const {}
};

class JavaScript : public Object {
private:
    GDCLASS(JavaScript, Object);

    static JavaScript* singleton;

protected:
    static void _bind_methods();

public:
    Variant eval(const String& p_code, bool p_use_global_exec_context = false);
    Ref<JavaScriptObject> get_interface(const String& p_interface);
    Ref<JavaScriptObject> create_callback(
        Object* p_ref,
        const StringName& p_method
    );
    Variant _create_object_bind(
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );
    void download_buffer(
        Vector<uint8_t> p_arr,
        const String& p_name,
        const String& p_mime = "application/octet-stream"
    );

    static JavaScript* get_singleton();
    JavaScript();
    ~JavaScript();
};

#endif // JAVASCRIPT_SINGLETON_H
