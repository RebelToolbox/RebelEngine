// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_jni_plugin.h"

#include "api/jni_singleton.h"
#include "core/engine.h"
#include "core/error_macros.h"
#include "core/project_settings.h"
#include "jni_utils.h"

static HashMap<String, JNISingleton*> jni_singletons;

extern "C" {

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterSingleton(
    JNIEnv* env,
    jclass clazz,
    jstring name,
    jobject obj
) {
    String singname = string_from_jstring(env, name);
    JNISingleton* s = (JNISingleton*)ClassDB::instance("JNISingleton");
    s->set_instance(env->NewGlobalRef(obj));
    jni_singletons[singname] = s;

    Engine::get_singleton()->add_singleton(Engine::Singleton(singname, s));
    ProjectSettings::get_singleton()->set(singname, s);
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterMethod(
    JNIEnv* env,
    jclass clazz,
    jstring sname,
    jstring name,
    jstring ret,
    jobjectArray args
) {
    String singname = string_from_jstring(env, sname);

    ERR_FAIL_COND(!jni_singletons.has(singname));

    JNISingleton* s = jni_singletons.get(singname);

    String mname  = string_from_jstring(env, name);
    String retval = string_from_jstring(env, ret);
    Vector<Variant::Type> types;
    String cs = "(";

    int stringCount = env->GetArrayLength(args);

    for (int i = 0; i < stringCount; i++) {
        jstring string         = (jstring)env->GetObjectArrayElement(args, i);
        const String rawString = string_from_jstring(env, string);
        types.push_back(get_jni_type(rawString));
        cs += get_jni_sig(rawString);
    }

    cs         += ")";
    cs         += get_jni_sig(retval);
    jclass cls  = env->GetObjectClass(s->get_instance());
    jmethodID mid =
        env->GetMethodID(cls, mname.ascii().get_data(), cs.ascii().get_data());
    if (!mid) {
        print_line("Failed getting method ID " + mname);
    }

    s->add_method(mname, mid, types, get_jni_type(retval));
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterSignal(
    JNIEnv* env,
    jclass clazz,
    jstring j_plugin_name,
    jstring j_signal_name,
    jobjectArray j_signal_param_types
) {
    String singleton_name = string_from_jstring(env, j_plugin_name);

    ERR_FAIL_COND(!jni_singletons.has(singleton_name));

    JNISingleton* singleton = jni_singletons.get(singleton_name);

    String signal_name = string_from_jstring(env, j_signal_name);
    Vector<Variant::Type> types;

    int stringCount = env->GetArrayLength(j_signal_param_types);

    for (int i = 0; i < stringCount; i++) {
        jstring j_signal_param_type =
            (jstring)env->GetObjectArrayElement(j_signal_param_types, i);
        const String signal_param_type =
            string_from_jstring(env, j_signal_param_type);
        types.push_back(get_jni_type(signal_param_type));
    }

    singleton->add_signal(signal_name, types);
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeEmitSignal(
    JNIEnv* env,
    jclass clazz,
    jstring j_plugin_name,
    jstring j_signal_name,
    jobjectArray j_signal_params
) {
    String singleton_name = string_from_jstring(env, j_plugin_name);

    ERR_FAIL_COND(!jni_singletons.has(singleton_name));

    JNISingleton* singleton = jni_singletons.get(singleton_name);

    String signal_name = string_from_jstring(env, j_signal_name);

    int count = env->GetArrayLength(j_signal_params);
    ERR_FAIL_COND_MSG(
        count > VARIANT_ARG_MAX,
        "Maximum argument count exceeded!"
    );

    Variant variant_params[VARIANT_ARG_MAX];
    const Variant* args[VARIANT_ARG_MAX];

    for (int i = 0; i < count; i++) {
        jobject j_param   = env->GetObjectArrayElement(j_signal_params, i);
        variant_params[i] = _jobject_to_variant(env, j_param);
        args[i]           = &variant_params[i];
        env->DeleteLocalRef(j_param);
    };

    singleton->emit_signal(signal_name, args, count);
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_plugin_RebelPlugin_nativeRegisterGDNativeLibraries(
    JNIEnv* env,
    jclass clazz,
    jobjectArray gdnlib_paths
) {
    int gdnlib_count = env->GetArrayLength(gdnlib_paths);
    if (gdnlib_count == 0) {
        return;
    }

    // Retrieve the current list of gdnative libraries.
    Array singletons = Array();
    if (ProjectSettings::get_singleton()->has_setting("gdnative/singletons")) {
        singletons =
            ProjectSettings::get_singleton()->get("gdnative/singletons");
    }

    // Insert the libraries provided by the plugin
    for (int i = 0; i < gdnlib_count; i++) {
        jstring relative_path =
            (jstring)env->GetObjectArrayElement(gdnlib_paths, i);

        String path = "res://" + string_from_jstring(env, relative_path);
        if (!singletons.has(path)) {
            singletons.push_back(path);
        }
        env->DeleteLocalRef(relative_path);
    }

    // Insert the updated list back into project settings.
    ProjectSettings::get_singleton()->set("gdnative/singletons", singletons);
}
}
