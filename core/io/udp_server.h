// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "core/io/net_socket.h"
#include "core/io/packet_peer_udp.h"

class UDPServer : public Reference {
    GDCLASS(UDPServer, Reference);

protected:
    enum {
        PACKET_BUFFER_SIZE = 65536
    };

    struct Peer {
        PacketPeerUDP* peer;
        IP_Address ip;
        uint16_t port = 0;

        bool operator==(const Peer& p_other) const {
            return (ip == p_other.ip && port == p_other.port);
        }
    };

    uint8_t recv_buffer[PACKET_BUFFER_SIZE];

    int bind_port = 0;
    IP_Address bind_address;

    List<Peer> peers;
    List<Peer> pending;
    int max_pending_connections = 16;

    Ref<NetSocket> _sock;

    static void _bind_methods();

public:
    void remove_peer(IP_Address p_ip, int p_port);
    Error listen(
        uint16_t p_port,
        const IP_Address& p_bind_address = IP_Address("*")
    );
    Error poll();
    bool is_listening() const;
    bool is_connection_available() const;
    void set_max_pending_connections(int p_max);
    int get_max_pending_connections() const;
    Ref<PacketPeerUDP> take_connection();

    void stop();

    UDPServer();
    ~UDPServer();
};

#endif // UDP_SERVER_H
