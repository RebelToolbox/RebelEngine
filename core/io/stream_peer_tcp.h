// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STREAM_PEER_TCP_H
#define STREAM_PEER_TCP_H

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/io/net_socket.h"
#include "core/io/stream_peer.h"

class StreamPeerTCP : public StreamPeer {
    GDCLASS(StreamPeerTCP, StreamPeer);
    OBJ_CATEGORY("Networking");

public:
    enum Status {
        STATUS_NONE,
        STATUS_CONNECTING,
        STATUS_CONNECTED,
        STATUS_ERROR,
    };

protected:
    Ref<NetSocket> _sock;
    uint64_t timeout;
    Status status;
    IP_Address peer_host;
    uint16_t peer_port;

    Error _connect(const String& p_address, int p_port);
    Error _poll_connection();
    Error write(const uint8_t* p_data, int p_bytes, int& r_sent, bool p_block);
    Error read(uint8_t* p_buffer, int p_bytes, int& r_received, bool p_block);

    static void _bind_methods();

public:
    void accept_socket(
        Ref<NetSocket> p_sock,
        IP_Address p_host,
        uint16_t p_port
    );

    Error connect_to_host(const IP_Address& p_host, uint16_t p_port);
    bool is_connected_to_host() const;
    IP_Address get_connected_host() const;
    uint16_t get_connected_port() const;
    void disconnect_from_host();

    int get_available_bytes() const;
    Status get_status();

    void set_no_delay(bool p_enabled);

    // Read/Write from StreamPeer
    Error put_data(const uint8_t* p_data, int p_bytes);
    Error put_partial_data(const uint8_t* p_data, int p_bytes, int& r_sent);
    Error get_data(uint8_t* p_buffer, int p_bytes);
    Error get_partial_data(uint8_t* p_buffer, int p_bytes, int& r_received);

    StreamPeerTCP();
    ~StreamPeerTCP();
};

VARIANT_ENUM_CAST(StreamPeerTCP::Status);

#endif
