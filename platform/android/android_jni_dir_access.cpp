// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_jni_dir_access.h"

#include "android_file_access.h"
#include "android_jni_thread.h"
#include "android_jni_utils.h"
#include "core/print_string.h"

jobject AndroidJNIDirAccess::io            = NULL;
jclass AndroidJNIDirAccess::cls            = NULL;
jmethodID AndroidJNIDirAccess::_dir_open   = NULL;
jmethodID AndroidJNIDirAccess::_dir_next   = NULL;
jmethodID AndroidJNIDirAccess::_dir_close  = NULL;
jmethodID AndroidJNIDirAccess::_dir_is_dir = NULL;

DirAccess* AndroidJNIDirAccess::create_fs() {
    return memnew(AndroidJNIDirAccess);
}

Error AndroidJNIDirAccess::list_dir_begin() {
    list_dir_end();
    JNIEnv* env = get_jni_env();

    jstring js = env->NewStringUTF(current_dir.utf8().get_data());
    int res    = env->CallIntMethod(io, _dir_open, js);
    if (res <= 0) {
        return ERR_CANT_OPEN;
    }

    id = res;

    return OK;
}

String AndroidJNIDirAccess::get_next() {
    ERR_FAIL_COND_V(id == 0, "");

    JNIEnv* env = get_jni_env();
    jstring str = (jstring)env->CallObjectMethod(io, _dir_next, id);
    if (!str) {
        return "";
    }

    String ret = string_from_jstring(env, str);
    env->DeleteLocalRef((jobject)str);
    return ret;
}

bool AndroidJNIDirAccess::current_is_dir() const {
    JNIEnv* env = get_jni_env();

    return env->CallBooleanMethod(io, _dir_is_dir, id);
}

bool AndroidJNIDirAccess::current_is_hidden() const {
    return current != "." && current != ".." && current.begins_with(".");
}

void AndroidJNIDirAccess::list_dir_end() {
    if (id == 0) {
        return;
    }

    JNIEnv* env = get_jni_env();
    env->CallVoidMethod(io, _dir_close, id);
    id = 0;
}

int AndroidJNIDirAccess::get_drive_count() {
    return 0;
}

String AndroidJNIDirAccess::get_drive(int p_drive) {
    return "";
}

Error AndroidJNIDirAccess::change_dir(String p_dir) {
    JNIEnv* env = get_jni_env();

    if (p_dir == "" || p_dir == "." || (p_dir == ".." && current_dir == "")) {
        return OK;
    }

    String new_dir;

    if (p_dir != "res://" && p_dir.length() > 1 && p_dir.ends_with("/")) {
        p_dir = p_dir.substr(0, p_dir.length() - 1);
    }

    if (p_dir.begins_with("/")) {
        new_dir = p_dir.substr(1, p_dir.length());
    } else if (p_dir.begins_with("res://")) {
        new_dir = p_dir.substr(6, p_dir.length());
    } else if (current_dir == "") {
        new_dir = p_dir;
    } else {
        new_dir = current_dir.plus_file(p_dir);
    }

    // test if newdir exists
    new_dir = new_dir.simplify_path();

    jstring js = env->NewStringUTF(new_dir.utf8().get_data());
    int res    = env->CallIntMethod(io, _dir_open, js);
    env->DeleteLocalRef(js);
    if (res <= 0) {
        return ERR_INVALID_PARAMETER;
    }

    env->CallVoidMethod(io, _dir_close, res);

    current_dir = new_dir;

    return OK;
}

String AndroidJNIDirAccess::get_current_dir() {
    return "res://" + current_dir;
}

bool AndroidJNIDirAccess::file_exists(String p_file) {
    String sd;
    if (current_dir == "") {
        sd = p_file;
    } else {
        sd = current_dir.plus_file(p_file);
    }

    AndroidFileAccess* f = memnew(AndroidFileAccess);
    bool exists          = f->file_exists(sd);
    memdelete(f);

    return exists;
}

bool AndroidJNIDirAccess::dir_exists(String p_dir) {
    JNIEnv* env = get_jni_env();

    String sd;

    if (current_dir == "") {
        sd = p_dir;
    } else {
        if (p_dir.is_rel_path()) {
            sd = current_dir.plus_file(p_dir);
        } else {
            sd = fix_path(p_dir);
        }
    }

    String path = sd.simplify_path();

    if (path.begins_with("/")) {
        path = path.substr(1, path.length());
    } else if (path.begins_with("res://")) {
        path = path.substr(6, path.length());
    }

    jstring js = env->NewStringUTF(path.utf8().get_data());
    int res    = env->CallIntMethod(io, _dir_open, js);
    env->DeleteLocalRef(js);
    if (res <= 0) {
        return false;
    }

    env->CallVoidMethod(io, _dir_close, res);

    return true;
}

Error AndroidJNIDirAccess::make_dir(String p_dir) {
    ERR_FAIL_V(ERR_UNAVAILABLE);
}

Error AndroidJNIDirAccess::rename(String p_from, String p_to) {
    ERR_FAIL_V(ERR_UNAVAILABLE);
}

Error AndroidJNIDirAccess::remove(String p_name) {
    ERR_FAIL_V(ERR_UNAVAILABLE);
}

String AndroidJNIDirAccess::get_filesystem_type() const {
    return "APK";
}

uint64_t AndroidJNIDirAccess::get_space_left() {
    return 0;
}

void AndroidJNIDirAccess::setup(jobject p_io) {
    JNIEnv* env = get_jni_env();
    io          = p_io;

    jclass c = env->GetObjectClass(io);
    cls      = (jclass)env->NewGlobalRef(c);

    _dir_open   = env->GetMethodID(cls, "dir_open", "(Ljava/lang/String;)I");
    _dir_next   = env->GetMethodID(cls, "dir_next", "(I)Ljava/lang/String;");
    _dir_close  = env->GetMethodID(cls, "dir_close", "(I)V");
    _dir_is_dir = env->GetMethodID(cls, "dir_is_dir", "(I)Z");
}

AndroidJNIDirAccess::AndroidJNIDirAccess() {
    id = 0;
}

AndroidJNIDirAccess::~AndroidJNIDirAccess() {
    list_dir_end();
}
