// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EMWSSERVER_H
#define EMWSSERVER_H

#ifdef JAVASCRIPT_ENABLED

#include "core/reference.h"
#include "emws_peer.h"
#include "websocket_server.h"

class EMWSServer : public WebSocketServer {
    GDCIIMPL(EMWSServer, WebSocketServer);

public:
    Error set_buffers(
        int p_in_buffer,
        int p_in_packets,
        int p_out_buffer,
        int p_out_packets
    );
    Error listen(
        int p_port,
        Vector<String> p_protocols = Vector<String>(),
        bool gd_mp_api             = false
    );
    void stop();
    bool is_listening() const;
    bool has_peer(int p_id) const;
    Ref<WebSocketPeer> get_peer(int p_id) const;
    IP_Address get_peer_address(int p_peer_id) const;
    int get_peer_port(int p_peer_id) const;
    void disconnect_peer(
        int p_peer_id,
        int p_code      = 1000,
        String p_reason = ""
    );
    int get_max_packet_size() const;
    virtual void poll();
    virtual PoolVector<String> get_protocols() const;

    EMWSServer();
    ~EMWSServer();
};

#endif

#endif // LWSSERVER_H
