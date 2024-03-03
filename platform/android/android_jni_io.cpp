// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_jni_io.h"

#include "core/error_list.h"

// JNIEnv is only valid within the thread it belongs to, in a multi threading
// environment we can't cache it. For IO we call all access methods from our
// thread and we thus get a valid JNIEnv from ThreadAndroid.

AndroidJNIIO::AndroidJNIIO(JNIEnv* p_env, jobject p_io_object) {
    io_object = p_env->NewGlobalRef(p_io_object);
    if (io_object) {
        io_class = p_env->GetObjectClass(io_object);
        if (io_class) {
            io_class = (jclass)p_env->NewGlobalRef(io_class);
        } else {
            // this is a pretty serious fail.. bail... pointers will stay 0
            return;
        }

        _open_URI =
            p_env->GetMethodID(io_class, "openURI", "(Ljava/lang/String;)I");
        _get_cache_dir =
            p_env->GetMethodID(io_class, "getCacheDir", "()Ljava/lang/String;");
        _get_data_dir =
            p_env->GetMethodID(io_class, "getDataDir", "()Ljava/lang/String;");
        _get_locale =
            p_env->GetMethodID(io_class, "getLocale", "()Ljava/lang/String;");
        _get_model =
            p_env->GetMethodID(io_class, "getModel", "()Ljava/lang/String;");
        _get_screen_DPI = p_env->GetMethodID(io_class, "getScreenDPI", "()I");
        _get_window_safe_area =
            p_env->GetMethodID(io_class, "getWindowSafeArea", "()[I"),
        _get_unique_id =
            p_env->GetMethodID(io_class, "getUniqueID", "()Ljava/lang/String;");
        _show_keyboard = p_env->GetMethodID(
            io_class,
            "showKeyboard",
            "(Ljava/lang/String;ZIII)V"
        );
        _hide_keyboard = p_env->GetMethodID(io_class, "hideKeyboard", "()V");
        _set_screen_orientation =
            p_env->GetMethodID(io_class, "setScreenOrientation", "(I)V");
        _get_screen_orientation =
            p_env->GetMethodID(io_class, "getScreenOrientation", "()I");
        _get_system_dir = p_env->GetMethodID(
            io_class,
            "getSystemDir",
            "(IZ)Ljava/lang/String;"
        );
    }
}

AndroidJNIIO::~AndroidJNIIO() {
    // nothing to do here for now
}

jobject AndroidJNIIO::get_instance() {
    return io_object;
}

Error AndroidJNIIO::open_uri(const String& p_uri) {
    if (_open_URI) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, ERR_UNAVAILABLE);
        jstring jStr = env->NewStringUTF(p_uri.utf8().get_data());
        return env->CallIntMethod(io_object, _open_URI, jStr) ? ERR_CANT_OPEN
                                                              : OK;
    } else {
        return ERR_UNAVAILABLE;
    }
}

String AndroidJNIIO::get_cache_dir() {
    if (_get_cache_dir) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String());
        jstring s = (jstring)env->CallObjectMethod(io_object, _get_cache_dir);
        return jstring_to_string(s, env);
    } else {
        return String();
    }
}

String AndroidJNIIO::get_user_data_dir() {
    if (_get_data_dir) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String());
        jstring s = (jstring)env->CallObjectMethod(io_object, _get_data_dir);
        return jstring_to_string(s, env);
    } else {
        return String();
    }
}

String AndroidJNIIO::get_locale() {
    if (_get_locale) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String());
        jstring s = (jstring)env->CallObjectMethod(io_object, _get_locale);
        return jstring_to_string(s, env);
    } else {
        return String();
    }
}

String AndroidJNIIO::get_model() {
    if (_get_model) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String());
        jstring s = (jstring)env->CallObjectMethod(io_object, _get_model);
        return jstring_to_string(s, env);
    } else {
        return String();
    }
}

int AndroidJNIIO::get_screen_dpi() {
    if (_get_screen_DPI) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, 160);
        return env->CallIntMethod(io_object, _get_screen_DPI);
    } else {
        return 160;
    }
}

void AndroidJNIIO::get_window_safe_area(int (&p_rect_xywh)[4]) {
    if (_get_window_safe_area) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);
        jintArray returnArray =
            (jintArray)env->CallObjectMethod(io_object, _get_window_safe_area);
        ERR_FAIL_COND(env->GetArrayLength(returnArray) != 4);
        jint* arrayBody = env->GetIntArrayElements(returnArray, JNI_FALSE);
        for (int i = 0; i < 4; i++) {
            p_rect_xywh[i] = arrayBody[i];
        }
        env->ReleaseIntArrayElements(returnArray, arrayBody, 0);
    }
}

String AndroidJNIIO::get_unique_id() {
    if (_get_unique_id) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String());
        jstring s = (jstring)env->CallObjectMethod(io_object, _get_unique_id);
        return jstring_to_string(s, env);
    } else {
        return String();
    }
}

bool AndroidJNIIO::has_vk() {
    return (_show_keyboard != 0) && (_hide_keyboard != 0);
}

void AndroidJNIIO::show_vk(
    const String& p_existing,
    bool p_multiline,
    int p_max_input_length,
    int p_cursor_start,
    int p_cursor_end
) {
    if (_show_keyboard) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);
        jstring jStr = env->NewStringUTF(p_existing.utf8().get_data());
        env->CallVoidMethod(
            io_object,
            _show_keyboard,
            jStr,
            p_multiline,
            p_max_input_length,
            p_cursor_start,
            p_cursor_end
        );
    }
}

void AndroidJNIIO::hide_vk() {
    if (_hide_keyboard) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);
        env->CallVoidMethod(io_object, _hide_keyboard);
    }
}

void AndroidJNIIO::set_screen_orientation(int p_orient) {
    if (_set_screen_orientation) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND(env == nullptr);
        env->CallVoidMethod(io_object, _set_screen_orientation, p_orient);
    }
}

int AndroidJNIIO::get_screen_orientation() const {
    if (_get_screen_orientation) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, 0);
        return env->CallIntMethod(io_object, _get_screen_orientation);
    } else {
        return 0;
    }
}

String AndroidJNIIO::get_system_dir(int p_dir, bool p_shared_storage) {
    if (_get_system_dir) {
        JNIEnv* env = get_jni_env();
        ERR_FAIL_COND_V(env == nullptr, String("."));
        jstring s = (jstring)env->CallObjectMethod(
            io_object,
            _get_system_dir,
            p_dir,
            p_shared_storage
        );
        return jstring_to_string(s, env);
    } else {
        return String(".");
    }
}

// SafeNumeric because it can be changed from non-main thread and we need to
// ensure the change is immediately visible to other threads.
static SafeNumeric<int> virtual_keyboard_height;

int AndroidJNIIO::get_vk_height() {
    return virtual_keyboard_height.get();
}

void AndroidJNIIO::set_vk_height(int p_height) {
    virtual_keyboard_height.set(p_height);
}
