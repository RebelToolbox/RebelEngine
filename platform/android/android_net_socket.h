// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_NET_SOCKET_H
#define ANDROID_NET_SOCKET_H

#include "drivers/network/default_net_socket.h"

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
class AndroidNetSocket : public DefaultNetSocket {
private:
    static jobject wifi_multicast_lock;
    static jclass wifi_multicast_lock_class;
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
    static void setup(jobject p_wifi_multicast_lock);

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

    AndroidNetSocket();
    ~AndroidNetSocket();
};

#endif // ANDROID_NET_SOCKET_H
