// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_jni_thread.h"

#include "core/os/thread.h"

static JavaVM* java_vm          = nullptr;
static thread_local JNIEnv* env = nullptr;

// The logic here need to improve, init_thread/term_tread are designed to work
// with Thread::callback Calling init_thread from setup_android_thread and
// get_jni_env to setup an env we're keeping and not detaching could cause
// issues on app termination.
//
// We should be making sure that any thread started calls a nice cleanup
// function when it's done, especially now that we use many more threads.

static void init_thread() {
    if (env) {
        // thread never detached! just keep using...
        return;
    }

    java_vm->AttachCurrentThread(&env, nullptr);
}

static void term_thread() {
    java_vm->DetachCurrentThread();

    // this is no longer valid, must called init_thread to re-establish
    env = nullptr;
}

void initialize_android_jni_thread(JavaVM* p_jvm, JNIEnv* p_env) {
    java_vm = p_jvm;
    env     = p_env;
    Thread::_set_platform_funcs(nullptr, nullptr, &init_thread, &term_thread);
}

void setup_android_thread() {
    if (!env) {
        // !BAS! see remarks above
        init_thread();
    }
}

JNIEnv* get_jni_env() {
    if (!env) {
        // !BAS! see remarks above
        init_thread();
    }

    return env;
}
