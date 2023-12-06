// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EMWSCLIENT_H
#define EMWSCLIENT_H

#ifdef JAVASCRIPT_ENABLED

#include "core/error_list.h"
#include "emws_peer.h"
#include "websocket_client.h"

class EMWSClient : public WebSocketClient {
    GDCIIMPL(EMWSClient, WebSocketClient);

private:
    int _js_id;
    bool _is_connecting;
    int _in_buf_size;
    int _in_pkt_size;
    int _out_buf_size;

    static void _esws_on_connect(void* obj, char* proto);
    static void _esws_on_message(
        void* obj,
        const uint8_t* p_data,
        int p_data_size,
        int p_is_string
    );
    static void _esws_on_error(void* obj);
    static void _esws_on_close(
        void* obj,
        int code,
        const char* reason,
        int was_clean
    );

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
    Ref<WebSocketPeer> get_peer(int p_peer_id) const;
    void disconnect_from_host(int p_code = 1000, String p_reason = "");
    IP_Address get_connected_host() const;
    uint16_t get_connected_port() const;
    virtual ConnectionStatus get_connection_status() const;
    int get_max_packet_size() const;
    virtual void poll();
    EMWSClient();
    ~EMWSClient();
};

#endif // JAVASCRIPT_ENABLED

#endif // EMWSCLIENT_H
