// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "net_socket_android.h"

#include "thread_jandroid.h"

jobject NetSocketAndroid::wifi_multicast_lock       = 0;
jclass NetSocketAndroid::wifi_multicast_lock_class  = 0;
jmethodID NetSocketAndroid::_multicast_lock_acquire = 0;
jmethodID NetSocketAndroid::_multicast_lock_release = 0;

void NetSocketAndroid::setup(jobject p_wifi_multicast_lock) {
    JNIEnv* env = get_jni_env();

    wifi_multicast_lock       = env->NewGlobalRef(p_wifi_multicast_lock);
    jclass lock_class         = env->GetObjectClass(wifi_multicast_lock);
    wifi_multicast_lock_class = (jclass)env->NewGlobalRef(lock_class);
    _multicast_lock_acquire   = env->GetMethodID(
        wifi_multicast_lock_class,
        "multicastLockAcquire",
        "()V"
    );
    _multicast_lock_release = env->GetMethodID(
        wifi_multicast_lock_class,
        "multicastLockRelease",
        "()V"
    );
}

void NetSocketAndroid::multicast_lock_acquire() {
    if (_multicast_lock_acquire) {
        JNIEnv* env = get_jni_env();
        env->CallVoidMethod(wifi_multicast_lock, _multicast_lock_acquire);
    }
}

void NetSocketAndroid::multicast_lock_release() {
    if (_multicast_lock_release) {
        JNIEnv* env = get_jni_env();
        env->CallVoidMethod(wifi_multicast_lock, _multicast_lock_release);
    }
}

NetSocket* NetSocketAndroid::_create_func() {
    return memnew(NetSocketAndroid);
}

void NetSocketAndroid::make_default() {
    _create = _create_func;
}

NetSocketAndroid::NetSocketAndroid() :
    wants_broadcast(false),
    multicast_groups(0) {}

NetSocketAndroid::~NetSocketAndroid() {
    close();
}

void NetSocketAndroid::close() {
    NetSocketPosix::close();
    if (wants_broadcast) {
        multicast_lock_release();
    }
    if (multicast_groups) {
        multicast_lock_release();
    }
    wants_broadcast  = false;
    multicast_groups = 0;
}

Error NetSocketAndroid::set_broadcasting_enabled(bool p_enabled) {
    Error err = NetSocketPosix::set_broadcasting_enabled(p_enabled);
    if (err != OK) {
        return err;
    }

    if (p_enabled != wants_broadcast) {
        if (p_enabled) {
            multicast_lock_acquire();
        } else {
            multicast_lock_release();
        }

        wants_broadcast = p_enabled;
    }

    return OK;
}

Error NetSocketAndroid::join_multicast_group(
    const IP_Address& p_multi_address,
    String p_if_name
) {
    Error err =
        NetSocketPosix::join_multicast_group(p_multi_address, p_if_name);
    if (err != OK) {
        return err;
    }

    if (!multicast_groups) {
        multicast_lock_acquire();
    }
    multicast_groups++;

    return OK;
}

Error NetSocketAndroid::leave_multicast_group(
    const IP_Address& p_multi_address,
    String p_if_name
) {
    Error err =
        NetSocketPosix::leave_multicast_group(p_multi_address, p_if_name);
    if (err != OK) {
        return err;
    }

    ERR_FAIL_COND_V(multicast_groups == 0, ERR_BUG);

    multicast_groups--;
    if (!multicast_groups) {
        multicast_lock_release();
    }

    return OK;
}
