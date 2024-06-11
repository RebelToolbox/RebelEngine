// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_jni_os.h"

#include "android_jni_thread.h"
#include "android_jni_utils.h"

// JNIEnv is only valid within the thread it belongs to, in a multi threading
// environment we can't cache it. For RebelFragment we call most access methods
// from our thread and we thus get a valid JNIEnv from ThreadAndroid. For one or
// two we expect to pass the environment

AndroidJNIOS::AndroidJNIOS(
    JNIEnv* p_env,
    jobject p_activity,
    jobject p_fragment
) {
    activity = p_env->NewGlobalRef(p_activity);
    fragment = p_env->NewGlobalRef(p_fragment);

    // Get the RebelFragment class that contains all the java methods
    fragment_class =
        p_env->FindClass("com/rebeltoolbox/rebelengine/RebelFragment");
    if (fragment_class) {
        fragment_class = (jclass)p_env->NewGlobalRef(fragment_class);
    } else {
        // this is a pretty serious fail.. bail... pointers will stay 0
        return;
    }
    activity_class = p_env->FindClass("android/app/Activity");
    if (activity_class) {
        activity_class = (jclass)p_env->NewGlobalRef(activity_class);
    } else {
        // this is a pretty serious fail.. bail... pointers will stay 0
        return;
    }

    // Set pointers to Android methods
    _on_video_init = p_env->GetMethodID(fragment_class, "onVideoInit", "()V");
    _restart       = p_env->GetMethodID(fragment_class, "restart", "()V");
    _finish        = p_env->GetMethodID(fragment_class, "forceQuit", "()V");
    _set_keep_screen_on =
        p_env->GetMethodID(fragment_class, "setKeepScreenOn", "(Z)V");
    _alert = p_env->GetMethodID(
        fragment_class,
        "alert",
        "(Ljava/lang/String;Ljava/lang/String;)V"
    );
    _get_GLES_version_code =
        p_env->GetMethodID(fragment_class, "getGLESVersionCode", "()I");
    _get_clipboard = p_env->GetMethodID(
        fragment_class,
        "getClipboard",
        "()Ljava/lang/String;"
    );
    _set_clipboard = p_env->GetMethodID(
        fragment_class,
        "setClipboard",
        "(Ljava/lang/String;)V"
    );
    _request_permission = p_env->GetMethodID(
        fragment_class,
        "requestPermission",
        "(Ljava/lang/String;)Z"
    );
    _request_permissions =
        p_env->GetMethodID(fragment_class, "requestPermissions", "()Z");
    _get_granted_permissions = p_env->GetMethodID(
        fragment_class,
        "getGrantedPermissions",
        "()[Ljava/lang/String;"
    );
    _init_input_devices =
        p_env->GetMethodID(fragment_class, "initInputDevices", "()V");
    _get_surface = p_env->GetMethodID(
        fragment_class,
        "getSurface",
        "()Landroid/view/Surface;"
    );
    _is_activity_resumed =
        p_env->GetMethodID(fragment_class, "isActivityResumed", "()Z");
    _vibrate = p_env->GetMethodID(fragment_class, "vibrate", "(I)V");
    _get_input_fallback_mapping = p_env->GetMethodID(
        fragment_class,
        "getInputFallbackMapping",
        "()Ljava/lang/String;"
    );
    _on_setup_completed =
        p_env->GetMethodID(fragment_class, "onSetupCompleted", "()V");
    _on_main_loop_started =
        p_env->GetMethodID(fragment_class, "onMainLoopStarted", "()V");

    // get some Activity method pointers...
    _get_class_loader = p_env->GetMethodID(
        activity_class,
        "getClassLoader",
        "()Ljava/lang/ClassLoader;"
    );
}

AndroidJNIOS::~AndroidJNIOS() {
    // nothing to do here for now
}

jobject AndroidJNIOS::get_activity() {
    return activity;
}

jobject AndroidJNIOS::get_member_object(
    const char* p_name,
    const char* p_class,
    JNIEnv* p_env
) {
    if (fragment_class) {
        if (p_env == NULL) {
            p_env = get_jni_env();
        }

        ERR_FAIL_COND_V(p_env == nullptr, nullptr);

        jfieldID fid = p_env->GetStaticFieldID(fragment_class, p_name, p_class);
        return p_env->GetStaticObjectField(fragment_class, fid);
    } else {
        return NULL;
    }
}

jobject AndroidJNIOS::get_class_loader() {
    if (_get_class_loader) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, nullptr);

        return env->CallObjectMethod(activity, _get_class_loader);
    } else {
        return NULL;
    }
}

void AndroidJNIOS::gfx_init(bool gl2) {
    // beats me what this once did, there was no code,
    // but we're getting false if our GLES3 driver is initialised
    // and true for our GLES2 driver
    // Maybe we're supposed to communicate this back or store it?
}

void AndroidJNIOS::on_video_init(JNIEnv* p_env) {
    if (_on_video_init) {
        if (p_env == NULL) {
            p_env = get_jni_env();
        }
        ERR_FAIL_COND(p_env == nullptr);

        p_env->CallVoidMethod(fragment, _on_video_init);
    }
}

void AndroidJNIOS::on_setup_completed(JNIEnv* p_env) {
    if (_on_setup_completed) {
        if (p_env == NULL) {
            p_env = get_jni_env();
        }
        ERR_FAIL_COND(p_env == nullptr);
        p_env->CallVoidMethod(fragment, _on_setup_completed);
    }
}

void AndroidJNIOS::on_main_loop_started(JNIEnv* p_env) {
    if (_on_main_loop_started) {
        if (p_env == NULL) {
            p_env = get_jni_env();
        }
        ERR_FAIL_COND(p_env == nullptr);
        p_env->CallVoidMethod(fragment, _on_main_loop_started);
    }
}

void AndroidJNIOS::restart(JNIEnv* p_env) {
    if (_restart) {
        if (p_env == NULL) {
            p_env = get_jni_env();
        }
        ERR_FAIL_COND(p_env == nullptr);

        p_env->CallVoidMethod(fragment, _restart);
    }
}

void AndroidJNIOS::force_quit(JNIEnv* p_env) {
    if (_finish) {
        if (p_env == NULL) {
            p_env = get_jni_env();
        }
        ERR_FAIL_COND(p_env == nullptr);

        p_env->CallVoidMethod(fragment, _finish);
    }
}

void AndroidJNIOS::set_keep_screen_on(bool p_enabled) {
    if (_set_keep_screen_on) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);

        env->CallVoidMethod(fragment, _set_keep_screen_on, p_enabled);
    }
}

void AndroidJNIOS::alert(const String& p_message, const String& p_title) {
    if (_alert) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);

        jstring jStrMessage = env->NewStringUTF(p_message.utf8().get_data());
        jstring jStrTitle   = env->NewStringUTF(p_title.utf8().get_data());
        env->CallVoidMethod(fragment, _alert, jStrMessage, jStrTitle);
    }
}

int AndroidJNIOS::get_gles_version_code() {
    JNIEnv* env = get_jni_env();
    ERR_FAIL_COND_V(env == nullptr, 0);

    if (_get_GLES_version_code) {
        return env->CallIntMethod(fragment, _get_GLES_version_code);
    }

    return 0;
}

bool AndroidJNIOS::has_get_clipboard() {
    return _get_clipboard != 0;
}

String AndroidJNIOS::get_clipboard() {
    if (_get_clipboard) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String());

        jstring clipboard =
            (jstring)env->CallObjectMethod(fragment, _get_clipboard);
        return string_from_jstring(env, clipboard);
    } else {
        return String();
    }
}

String AndroidJNIOS::get_input_fallback_mapping() {
    if (_get_input_fallback_mapping) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String());

        jstring fallback_mapping = (jstring
        )env->CallObjectMethod(fragment, _get_input_fallback_mapping);
        return string_from_jstring(env, fallback_mapping);
    } else {
        return String();
    }
}

bool AndroidJNIOS::has_set_clipboard() {
    return _set_clipboard != 0;
}

void AndroidJNIOS::set_clipboard(const String& p_text) {
    if (_set_clipboard) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);

        jstring jStr = env->NewStringUTF(p_text.utf8().get_data());
        env->CallVoidMethod(fragment, _set_clipboard, jStr);
    }
}

bool AndroidJNIOS::request_permission(const String& p_name) {
    if (_request_permission) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, false);

        jstring jStrName = env->NewStringUTF(p_name.utf8().get_data());
        return env->CallBooleanMethod(fragment, _request_permission, jStrName);
    } else {
        return false;
    }
}

bool AndroidJNIOS::request_permissions() {
    if (_request_permissions) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, false);

        return env->CallBooleanMethod(fragment, _request_permissions);
    } else {
        return false;
    }
}

Vector<String> AndroidJNIOS::get_granted_permissions() const {
    Vector<String> permissions_list;
    if (_get_granted_permissions) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, permissions_list);

        jobject permissions_object =
            env->CallObjectMethod(fragment, _get_granted_permissions);
        jobjectArray* arr =
            reinterpret_cast<jobjectArray*>(&permissions_object);

        int i     = 0;
        jsize len = env->GetArrayLength(*arr);
        for (i = 0; i < len; i++) {
            jstring jstr  = (jstring)env->GetObjectArrayElement(*arr, i);
            String string = string_from_jstring(env, jstr);
            permissions_list.push_back(string);
            env->DeleteLocalRef(jstr);
        }
    }
    return permissions_list;
}

void AndroidJNIOS::init_input_devices() {
    if (_init_input_devices) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);

        env->CallVoidMethod(fragment, _init_input_devices);
    }
}

jobject AndroidJNIOS::get_surface() {
    if (_get_surface) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, nullptr);

        return env->CallObjectMethod(fragment, _get_surface);
    } else {
        return NULL;
    }
}

bool AndroidJNIOS::is_activity_resumed() {
    if (_is_activity_resumed) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, false);

        return env->CallBooleanMethod(fragment, _is_activity_resumed);
    } else {
        return false;
    }
}

void AndroidJNIOS::vibrate(int p_duration_ms) {
    if (_vibrate) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);

        env->CallVoidMethod(fragment, _vibrate, p_duration_ms);
    }
}
