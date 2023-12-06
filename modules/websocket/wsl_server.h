// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WSLSERVER_H
#define WSLSERVER_H

#ifndef JAVASCRIPT_ENABLED

#include "core/io/stream_peer_ssl.h"
#include "core/io/stream_peer_tcp.h"
#include "core/io/tcp_server.h"
#include "websocket_server.h"
#include "wsl_peer.h"

class WSLServer : public WebSocketServer {
    GDCIIMPL(WSLServer, WebSocketServer);

private:
    class PendingPeer : public Reference {
    private:
        bool _parse_request(const Vector<String> p_protocols);

    public:
        Ref<StreamPeerTCP> tcp;
        Ref<StreamPeer> connection;
        bool use_ssl;

        uint64_t time;
        uint8_t req_buf[WSL_MAX_HEADER_SIZE];
        int req_pos;
        String key;
        String protocol;
        bool has_request;
        CharString response;
        int response_sent;

        PendingPeer();

        Error do_handshake(
            const Vector<String> p_protocols,
            uint64_t p_timeout
        );
    };

    int _in_buf_size;
    int _in_pkt_size;
    int _out_buf_size;
    int _out_pkt_size;

    List<Ref<PendingPeer>> _pending;
    Ref<TCP_Server> _server;
    Vector<String> _protocols;

public:
    Error set_buffers(
        int p_in_buffer,
        int p_in_packets,
        int p_out_buffer,
        int p_out_packets
    );
    Error listen(
        int p_port,
        const Vector<String> p_protocols = Vector<String>(),
        bool gd_mp_api                   = false
    );
    void stop();
    bool is_listening() const;
    int get_max_packet_size() const;
    bool has_peer(int p_id) const;
    Ref<WebSocketPeer> get_peer(int p_id) const;
    IP_Address get_peer_address(int p_peer_id) const;
    int get_peer_port(int p_peer_id) const;
    void disconnect_peer(
        int p_peer_id,
        int p_code      = 1000,
        String p_reason = ""
    );
    virtual void poll();

    WSLServer();
    ~WSLServer();
};

#endif // JAVASCRIPT_ENABLED

#endif // WSLSERVER_H
