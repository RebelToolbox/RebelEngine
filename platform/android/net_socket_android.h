// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NET_SOCKET_ANDROID_H
#define NET_SOCKET_ANDROID_H

#include "drivers/unix/net_socket_posix.h"

#include <jni.h>

/**
 * Specialized NetSocket implementation for Android.
 *
 * Some devices requires Android-specific code to acquire a MulticastLock
 * before sockets are allowed to receive broadcast and multicast packets.
 * This implementation calls into Java code and automatically acquire/release
 * the lock when broadcasting is enabled/disabled on a socket, or that socket
 * joins/leaves a multicast group.
 */
class NetSocketAndroid : public NetSocketPosix {
private:
    static jobject net_utils;
    static jclass cls;
    static jmethodID _multicast_lock_acquire;
    static jmethodID _multicast_lock_release;

    bool wants_broadcast;
    int multicast_groups;

    static void multicast_lock_acquire();
    static void multicast_lock_release();

protected:
    static NetSocket* _create_func();

public:
    static void make_default();
    static void setup(jobject p_net_utils);

    virtual void close();

    virtual Error set_broadcasting_enabled(bool p_enabled);
    virtual Error join_multicast_group(
        const IP_Address& p_multi_address,
        String p_if_name
    );
    virtual Error leave_multicast_group(
        const IP_Address& p_multi_address,
        String p_if_name
    );

    NetSocketAndroid();
    ~NetSocketAndroid();
};

#endif
