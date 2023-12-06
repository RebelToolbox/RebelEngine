// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef JAVASCRIPT_ENABLED

#include "emws_server.h"

#include "core/os/os.h"

Error EMWSServer::listen(
    int p_port,
    Vector<String> p_protocols,
    bool gd_mp_api
) {
    return FAILED;
}

bool EMWSServer::is_listening() const {
    return false;
}

void EMWSServer::stop() {}

bool EMWSServer::has_peer(int p_id) const {
    return false;
}

Ref<WebSocketPeer> EMWSServer::get_peer(int p_id) const {
    return NULL;
}

PoolVector<String> EMWSServer::get_protocols() const {
    PoolVector<String> out;

    return out;
}

IP_Address EMWSServer::get_peer_address(int p_peer_id) const {
    return IP_Address();
}

int EMWSServer::get_peer_port(int p_peer_id) const {
    return 0;
}

void EMWSServer::disconnect_peer(int p_peer_id, int p_code, String p_reason) {}

void EMWSServer::poll() {}

int EMWSServer::get_max_packet_size() const {
    return 0;
}

Error EMWSServer::set_buffers(
    int p_in_buffer,
    int p_in_packets,
    int p_out_buffer,
    int p_out_packets
) {
    return OK;
}

EMWSServer::EMWSServer() {}

EMWSServer::~EMWSServer() {}

#endif // JAVASCRIPT_ENABLED
