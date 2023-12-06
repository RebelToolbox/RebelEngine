// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WSLCLIENT_H
#define WSLCLIENT_H

#ifndef JAVASCRIPT_ENABLED

#include "core/error_list.h"
#include "core/io/stream_peer_ssl.h"
#include "core/io/stream_peer_tcp.h"
#include "websocket_client.h"
#include "wsl_peer.h"
#include "wslay/wslay.h"

class WSLClient : public WebSocketClient {
    GDCIIMPL(WSLClient, WebSocketClient);

private:
    int _in_buf_size;
    int _in_pkt_size;
    int _out_buf_size;
    int _out_pkt_size;

    Ref<WSLPeer> _peer;
    Ref<StreamPeerTCP> _tcp;
    Ref<StreamPeer> _connection;

    CharString _request;
    int _requested;

    uint8_t _resp_buf[WSL_MAX_HEADER_SIZE];
    int _resp_pos;

    String _response;

    String _key;
    String _host;
    int _port;
    Array ip_candidates;
    Vector<String> _protocols;
    bool _use_ssl;

    void _do_handshake();
    bool _verify_headers(String& r_protocol);

public:
    Error set_buffers(
        int p_in_buffer,
        int p_in_packets,
        int p_out_buffer,
        int p_out_packets
    );
    Error connect_to_host(
        String p_host,
        String p_path,
        uint16_t p_port,
        bool p_ssl,
        const Vector<String> p_protocol       = Vector<String>(),
        const Vector<String> p_custom_headers = Vector<String>()
    );
    int get_max_packet_size() const;
    Ref<WebSocketPeer> get_peer(int p_peer_id) const;
    void disconnect_from_host(int p_code = 1000, String p_reason = "");
    IP_Address get_connected_host() const;
    uint16_t get_connected_port() const;
    virtual ConnectionStatus get_connection_status() const;
    virtual void poll();

    WSLClient();
    ~WSLClient();
};

#endif // JAVASCRIPT_ENABLED

#endif // WSLCLIENT_H
