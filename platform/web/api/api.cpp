// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "api.h"

#include "core/engine.h"
#include "web_javascript.h"
#include "web_tools_editor_plugin.h"

static JavaScript* web_eval;

void register_web_api() {
    WebToolsEditorPlugin::initialize();
    ClassDB::register_virtual_class<JavaScriptObject>();
    ClassDB::register_virtual_class<JavaScript>();
    web_eval = memnew(JavaScript);
    Engine::get_singleton()->add_singleton(
        Engine::Singleton("JavaScript", web_eval)
    );
}

void unregister_web_api() {
    memdelete(web_eval);
}

JavaScript* JavaScript::singleton = nullptr;

JavaScript* JavaScript::get_singleton() {
    return singleton;
}

JavaScript::JavaScript() {
    ERR_FAIL_COND_MSG(
        singleton != nullptr,
        "JavaScript singleton already exists."
    );
    singleton = this;
}

JavaScript::~JavaScript() {}

void JavaScript::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("eval", "code", "use_global_execution_context"),
        &JavaScript::eval,
        DEFVAL(false)
    );
    ClassDB::bind_method(
        D_METHOD("get_interface", "interface"),
        &JavaScript::get_interface
    );
    ClassDB::bind_method(
        D_METHOD("create_callback", "object", "method"),
        &JavaScript::create_callback
    );
    {
        MethodInfo mi;
        mi.name = "create_object";
        mi.arguments.push_back(PropertyInfo(Variant::STRING, "object"));
        ClassDB::bind_vararg_method(
            METHOD_FLAGS_DEFAULT,
            "create_object",
            &JavaScript::_create_object_bind,
            mi
        );
    }
    ClassDB::bind_method(
        D_METHOD("download_buffer", "buffer", "name", "mime"),
        &JavaScript::download_buffer,
        DEFVAL("application/octet-stream")
    );
}

#if !defined(WEB_ENABLED) || !defined(WEB_EVAL_ENABLED)
Variant JavaScript::eval(const String& p_code, bool p_use_global_exec_context) {
    return Variant();
}

Ref<JavaScriptObject> JavaScript::get_interface(const String& p_interface) {
    return Ref<JavaScriptObject>();
}

Ref<JavaScriptObject> JavaScript::create_callback(
    Object* p_ref,
    const StringName& p_method
) {
    return Ref<JavaScriptObject>();
}

Variant JavaScript::_create_object_bind(
    const Variant** p_args,
    int p_argcount,
    Variant::CallError& r_error
) {
    if (p_argcount < 1) {
        r_error.error    = Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
        r_error.argument = 0;
        return Ref<JavaScriptObject>();
    }
    if (p_args[0]->get_type() != Variant::STRING) {
        r_error.error    = Variant::CallError::CALL_ERROR_INVALID_ARGUMENT;
        r_error.argument = 0;
        r_error.expected = Variant::STRING;
        return Ref<JavaScriptObject>();
    }
    return Ref<JavaScriptObject>();
}
#endif
#if !defined(WEB_ENABLED)
void JavaScript::download_buffer(
    Vector<uint8_t> p_arr,
    const String& p_name,
    const String& p_mime
) {}
#endif
