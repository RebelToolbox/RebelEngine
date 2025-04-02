// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "nativescript/rebel_nativescript.h"

#include "core/class_db.h"
#include "core/error_macros.h"
#include "core/global_constants.h"
#include "core/project_settings.h"
#include "core/variant.h"
#include "gdnative/gdnative.h"
#include "nativescript.h"

#ifdef __cplusplus
extern "C" {
#endif

extern "C" void _native_script_hook() {}

#define NSL NativeScriptLanguage::get_singleton()

// Script API

void GDAPI rebel_nativescript_register_class(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_base,
    rebel_instance_create_func p_create_func,
    rebel_instance_destroy_func p_destroy_func
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>* classes = &NSL->library_classes[*s];

    NativeScriptDesc desc;

    desc.create_func  = p_create_func;
    desc.destroy_func = p_destroy_func;
    desc.is_tool      = false;

    desc.base = p_base;

    if (classes->has(p_base)) {
        desc.base_data        = &(*classes)[p_base];
        desc.base_native_type = desc.base_data->base_native_type;
    } else {
        desc.base_data        = nullptr;
        desc.base_native_type = p_base;
    }

    classes->insert(p_name, desc);
}

void GDAPI rebel_nativescript_register_tool_class(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_base,
    rebel_instance_create_func p_create_func,
    rebel_instance_destroy_func p_destroy_func
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>* classes = &NSL->library_classes[*s];

    NativeScriptDesc desc;

    desc.create_func  = p_create_func;
    desc.destroy_func = p_destroy_func;
    desc.is_tool      = true;
    desc.base         = p_base;

    if (classes->has(p_base)) {
        desc.base_data        = &(*classes)[p_base];
        desc.base_native_type = desc.base_data->base_native_type;
    } else {
        desc.base_data        = nullptr;
        desc.base_native_type = p_base;
    }

    classes->insert(p_name, desc);
}

void GDAPI rebel_nativescript_register_method(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_function_name,
    rebel_method_attributes p_attr,
    rebel_instance_method p_method
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to register method on non-existent class."
    );

    NativeScriptDesc::Method method;
    method.method   = p_method;
    method.rpc_mode = p_attr.rpc_type;
    method.info     = MethodInfo(p_function_name);

    E->get().methods.insert(p_function_name, method);
}

void GDAPI rebel_nativescript_register_property(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_path,
    rebel_property_attributes* p_attr,
    rebel_property_set_func p_set_func,
    rebel_property_get_func p_get_func
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to register method on non-existent class."
    );

    NativeScriptDesc::Property property;
    property.default_value = *(Variant*)&p_attr->default_value;
    property.getter        = p_get_func;
    property.rset_mode     = p_attr->rset_type;
    property.setter        = p_set_func;
    property.info          = PropertyInfo(
        (Variant::Type)p_attr->type,
        p_path,
        (PropertyHint)p_attr->hint,
        *(String*)&p_attr->hint_string,
        (PropertyUsageFlags)p_attr->usage
    );

    E->get().properties.insert(p_path, property);
}

void GDAPI rebel_nativescript_register_signal(
    void* p_gdnative_handle,
    const char* p_name,
    const rebel_signal* p_signal
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to register method on non-existent class."
    );

    List<PropertyInfo> args;
    Vector<Variant> default_args;

    for (int i = 0; i < p_signal->num_args; i++) {
        PropertyInfo info;

        rebel_signal_argument arg = p_signal->args[i];

        info.hint        = (PropertyHint)arg.hint;
        info.hint_string = *(String*)&arg.hint_string;
        info.name        = *(String*)&arg.name;
        info.type        = (Variant::Type)arg.type;
        info.usage       = (PropertyUsageFlags)arg.usage;

        args.push_back(info);
    }

    for (int i = 0; i < p_signal->num_default_args; i++) {
        Variant* v;
        rebel_signal_argument attrib = p_signal->args[i];

        v = (Variant*)&attrib.default_value;

        default_args.push_back(*v);
    }

    MethodInfo method_info;
    method_info.name              = *(String*)&p_signal->name;
    method_info.arguments         = args;
    method_info.default_arguments = default_args;

    NativeScriptDesc::Signal signal;
    signal.signal = method_info;

    E->get().signals_.insert(*(String*)&p_signal->name, signal);
}

void GDAPI* rebel_nativescript_get_userdata(rebel_object* p_instance) {
    Object* instance = (Object*)p_instance;
    if (!instance) {
        return nullptr;
    }
    if (instance->get_script_instance()
        && instance->get_script_instance()->get_language()
               == NativeScriptLanguage::get_singleton()) {
        return ((NativeScriptInstance*)instance->get_script_instance())
            ->userdata;
    }
    return nullptr;
}

/*
 *
 *
 * NativeScript 1.1
 *
 *
 */

void GDAPI rebel_nativescript_set_method_argument_information(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_function_name,
    int p_num_args,
    const rebel_method_arg* p_args
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to add argument information for a method on a non-existent "
        "class."
    );

    Map<StringName, NativeScriptDesc::Method>::Element* method =
        E->get().methods.find(p_function_name);
    ERR_FAIL_COND_MSG(
        !method,
        "Attempted to add argument information to non-existent method."
    );

    MethodInfo* method_information = &method->get().info;

    List<PropertyInfo> args;

    for (int i = 0; i < p_num_args; i++) {
        rebel_method_arg arg = p_args[i];
        String name          = *(String*)&arg.name;
        String hint_string   = *(String*)&arg.hint_string;

        Variant::Type type = (Variant::Type)arg.type;
        PropertyHint hint  = (PropertyHint)arg.hint;

        args.push_back(PropertyInfo(type, p_name, hint, hint_string));
    }

    method_information->arguments = args;
}

void GDAPI rebel_nativescript_set_class_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    rebel_string p_documentation
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to add documentation to a non-existent class."
    );

    E->get().documentation = *(String*)&p_documentation;
}

void GDAPI rebel_nativescript_set_method_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_function_name,
    rebel_string p_documentation
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to add documentation to a method on a non-existent class."
    );

    Map<StringName, NativeScriptDesc::Method>::Element* method =
        E->get().methods.find(p_function_name);
    ERR_FAIL_COND_MSG(
        !method,
        "Attempted to add documentation to non-existent method."
    );

    method->get().documentation = *(String*)&p_documentation;
}

void GDAPI rebel_nativescript_set_property_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_path,
    rebel_string p_documentation
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to add documentation to a property on a non-existent class."
    );

    OrderedHashMap<StringName, NativeScriptDesc::Property>::Element property =
        E->get().properties.find(p_path);
    ERR_FAIL_COND_MSG(
        !property,
        "Attempted to add documentation to non-existent property."
    );

    property.get().documentation = *(String*)&p_documentation;
}

void GDAPI rebel_nativescript_set_signal_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_signal_name,
    rebel_string p_documentation
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(
        !E,
        "Attempted to add documentation to a signal on a non-existent class."
    );

    Map<StringName, NativeScriptDesc::Signal>::Element* signal =
        E->get().signals_.find(p_signal_name);
    ERR_FAIL_COND_MSG(
        !signal,
        "Attempted to add documentation to non-existent signal."
    );

    signal->get().documentation = *(String*)&p_documentation;
}

void GDAPI rebel_nativescript_set_global_type_tag(
    int p_idx,
    const char* p_name,
    const void* p_type_tag
) {
    NativeScriptLanguage::get_singleton()
        ->set_global_type_tag(p_idx, StringName(p_name), p_type_tag);
}

const void GDAPI* rebel_nativescript_get_global_type_tag(
    int p_idx,
    const char* p_name
) {
    return NativeScriptLanguage::get_singleton()->get_global_type_tag(
        p_idx,
        StringName(p_name)
    );
}

void GDAPI rebel_nativescript_set_type_tag(
    void* p_gdnative_handle,
    const char* p_name,
    const void* p_type_tag
) {
    String* s = (String*)p_gdnative_handle;

    Map<StringName, NativeScriptDesc>::Element* E =
        NSL->library_classes[*s].find(p_name);
    ERR_FAIL_COND_MSG(!E, "Attempted to set type tag on a non-existent class.");

    E->get().type_tag = p_type_tag;
}

const void GDAPI* rebel_nativescript_get_type_tag(const rebel_object* p_object
) {
    const Object* o = (Object*)p_object;

    if (!o->get_script_instance()) {
        return nullptr;
    } else {
        NativeScript* script = Object::cast_to<NativeScript>(
            o->get_script_instance()->get_script().ptr()
        );
        if (!script) {
            return nullptr;
        }

        if (script->get_script_desc()) {
            return script->get_script_desc()->type_tag;
        }
    }

    return nullptr;
}

int GDAPI rebel_nativescript_register_instance_binding_data_functions(
    rebel_instance_binding_functions p_binding_functions
) {
    return NativeScriptLanguage::get_singleton()->register_binding_functions(
        p_binding_functions
    );
}

void GDAPI
rebel_nativescript_unregister_instance_binding_data_functions(int p_idx) {
    NativeScriptLanguage::get_singleton()->unregister_binding_functions(p_idx);
}

void GDAPI* rebel_nativescript_get_instance_binding_data(
    int p_idx,
    rebel_object* p_object
) {
    return NativeScriptLanguage::get_singleton()->get_instance_binding_data(
        p_idx,
        (Object*)p_object
    );
}

void GDAPI rebel_nativescript_profiling_add_data(
    const char* p_signature,
    uint64_t p_time
) {
    NativeScriptLanguage::get_singleton()->profiling_add_data(
        StringName(p_signature),
        p_time
    );
}

#ifdef __cplusplus
}
#endif
