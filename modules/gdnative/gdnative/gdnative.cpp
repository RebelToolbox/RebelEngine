// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/gdnative.h"

#include "core/class_db.h"
#include "core/engine.h"
#include "core/error_macros.h"
#include "core/global_constants.h"
#include "core/os/os.h"
#include "core/variant.h"
#include "modules/gdnative/gdnative.h"

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI rebel_object_destroy(rebel_object* p_o) {
    memdelete((Object*)p_o);
}

// Singleton API

rebel_object GDAPI* rebel_global_get_singleton(char* p_name) {
    return (rebel_object*)Engine::get_singleton()->get_singleton_object(
        String(p_name)
    );
} // result shouldn't be freed

// MethodBind API

rebel_method_bind GDAPI* rebel_method_bind_get_method(
    const char* p_classname,
    const char* p_methodname
) {
    MethodBind* mb =
        ClassDB::get_method(StringName(p_classname), StringName(p_methodname));
    // MethodBind *mb = ClassDB::get_method("Node", "get_name");
    return (rebel_method_bind*)mb;
}

void GDAPI rebel_method_bind_ptrcall(
    rebel_method_bind* p_method_bind,
    rebel_object* p_instance,
    const void** p_args,
    void* p_ret
) {
    MethodBind* mb = (MethodBind*)p_method_bind;
    Object* o      = (Object*)p_instance;
    mb->ptrcall(o, p_args, p_ret);
}

rebel_variant GDAPI rebel_method_bind_call(
    rebel_method_bind* p_method_bind,
    rebel_object* p_instance,
    const rebel_variant** p_args,
    const int p_arg_count,
    rebel_variant_call_error* p_call_error
) {
    MethodBind* mb       = (MethodBind*)p_method_bind;
    Object* o            = (Object*)p_instance;
    const Variant** args = (const Variant**)p_args;

    rebel_variant ret;
    rebel_variant_new_nil(&ret);

    Variant* ret_val = (Variant*)&ret;

    Variant::CallError r_error;
    *ret_val = mb->call(o, args, p_arg_count, r_error);

    if (p_call_error) {
        p_call_error->error    = (rebel_variant_call_error_error)r_error.error;
        p_call_error->argument = r_error.argument;
        p_call_error->expected = (rebel_variant_type)r_error.expected;
    }

    return ret;
}

rebel_class_constructor GDAPI
rebel_get_class_constructor(const char* p_classname) {
    ClassDB::ClassInfo* class_info =
        ClassDB::classes.getptr(StringName(p_classname));
    if (class_info) {
        return (rebel_class_constructor)class_info->creation_func;
    }
    return nullptr;
}

rebel_dictionary GDAPI rebel_get_global_constants() {
    rebel_dictionary constants;
    rebel_dictionary_new(&constants);
    Dictionary* p_constants   = (Dictionary*)&constants;
    const int constants_count = GlobalConstants::get_global_constant_count();
    for (int i = 0; i < constants_count; ++i) {
        const char* name     = GlobalConstants::get_global_constant_name(i);
        int value            = GlobalConstants::get_global_constant_value(i);
        (*p_constants)[name] = value;
    }
    return constants;
}

// System functions
void GDAPI rebel_register_native_call_type(
    const char* p_call_type,
    native_call_cb p_callback
) {
    GDNativeCallRegistry::get_singleton()->register_native_call_type(
        StringName(p_call_type),
        p_callback
    );
}

void GDAPI* rebel_alloc(int p_bytes) {
    return memalloc(p_bytes);
}

void GDAPI* rebel_realloc(void* p_ptr, int p_bytes) {
    return memrealloc(p_ptr, p_bytes);
}

void GDAPI rebel_free(void* p_ptr) {
    memfree(p_ptr);
}

void GDAPI rebel_print_error(
    const char* p_description,
    const char* p_function,
    const char* p_file,
    int p_line
) {
    _err_print_error(
        p_function,
        p_file,
        p_line,
        p_description,
        ERR_HANDLER_ERROR
    );
}

void GDAPI rebel_print_warning(
    const char* p_description,
    const char* p_function,
    const char* p_file,
    int p_line
) {
    _err_print_error(
        p_function,
        p_file,
        p_line,
        p_description,
        ERR_HANDLER_WARNING
    );
}

void GDAPI rebel_print(const rebel_string* p_message) {
    print_line(*(String*)p_message);
}

void _gdnative_report_version_mismatch(
    const rebel_object* p_library,
    const char* p_ext,
    rebel_gdnative_api_version p_want,
    rebel_gdnative_api_version p_have
) {
    String message           = "Error loading GDNative file ";
    GDNativeLibrary* library = (GDNativeLibrary*)p_library;

    message += library->get_current_library_path() + ": Extension \"" + p_ext
             + "\" can't be loaded.\n";

    Dictionary versions;
    versions["have_major"] = p_have.major;
    versions["have_minor"] = p_have.minor;
    versions["want_major"] = p_want.major;
    versions["want_minor"] = p_want.minor;

    message += String(
                   "Got version {have_major}.{have_minor} but needs "
                   "{want_major}.{want_minor}!"
    )
                   .format(versions);

    _err_print_error(
        "gdnative_init",
        library->get_current_library_path().utf8().ptr(),
        0,
        message.utf8().ptr()
    );
}

void _gdnative_report_loading_error(
    const rebel_object* p_library,
    const char* p_what
) {
    String message           = "Error loading GDNative file ";
    GDNativeLibrary* library = (GDNativeLibrary*)p_library;

    message += library->get_current_library_path() + ": " + p_what;

    _err_print_error(
        "gdnative_init",
        library->get_current_library_path().utf8().ptr(),
        0,
        message.utf8().ptr()
    );
}

bool GDAPI rebel_is_instance_valid(const rebel_object* p_object) {
    return ObjectDB::instance_validate((Object*)p_object);
}

rebel_object GDAPI* rebel_instance_from_id(rebel_int p_instance_id) {
    return (rebel_object*)ObjectDB::get_instance((ObjectID)p_instance_id);
}

void* rebel_get_class_tag(const rebel_string_name* p_class) {
    StringName class_name          = *(StringName*)p_class;
    ClassDB::ClassInfo* class_info = ClassDB::classes.getptr(class_name);
    return class_info ? class_info->class_ptr : nullptr;
}

rebel_object* rebel_object_cast_to(
    const rebel_object* p_object,
    void* p_class_tag
) {
    if (!p_object) {
        return nullptr;
    }
    Object* o = (Object*)p_object;

    return o->is_class_ptr(p_class_tag) ? (rebel_object*)o : nullptr;
}

#ifdef __cplusplus
}
#endif
