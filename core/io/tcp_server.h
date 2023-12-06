// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "core/io/ip.h"
#include "core/io/net_socket.h"
#include "core/io/stream_peer.h"
#include "core/io/stream_peer_tcp.h"

class TCP_Server : public Reference {
    GDCLASS(TCP_Server, Reference);

protected:
    enum {
        MAX_PENDING_CONNECTIONS = 8
    };

    Ref<NetSocket> _sock;
    static void _bind_methods();

public:
    Error listen(
        uint16_t p_port,
        const IP_Address& p_bind_address = IP_Address("*")
    );
    bool is_listening() const;
    bool is_connection_available() const;
    Ref<StreamPeerTCP> take_connection();

    void stop(); // Stop listening

    TCP_Server();
    ~TCP_Server();
};

#endif // TCP_SERVER_H
