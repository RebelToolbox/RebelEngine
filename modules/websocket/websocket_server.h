// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include "core/crypto/crypto.h"
#include "core/reference.h"
#include "websocket_multiplayer_peer.h"
#include "websocket_peer.h"

class WebSocketServer : public WebSocketMultiplayerPeer {
    GDCLASS(WebSocketServer, WebSocketMultiplayerPeer);
    GDCICLASS(WebSocketServer);

    IP_Address bind_ip;

protected:
    static void _bind_methods();

    Ref<CryptoKey> private_key;
    Ref<X509Certificate> ssl_cert;
    Ref<X509Certificate> ca_chain;
    uint32_t handshake_timeout = 3000;

public:
    virtual void poll() = 0;
    virtual Error listen(
        int p_port,
        const Vector<String> p_protocols = Vector<String>(),
        bool gd_mp_api                   = false
    )                                                   = 0;
    virtual void stop()                                 = 0;
    virtual bool is_listening() const                   = 0;
    virtual bool has_peer(int p_id) const               = 0;
    virtual Ref<WebSocketPeer> get_peer(int p_id) const = 0;
    virtual bool is_server() const;
    ConnectionStatus get_connection_status() const;

    virtual IP_Address get_peer_address(int p_peer_id) const = 0;
    virtual int get_peer_port(int p_peer_id) const           = 0;
    virtual void disconnect_peer(
        int p_peer_id,
        int p_code      = 1000,
        String p_reason = ""
    ) = 0;

    void _on_peer_packet(int32_t p_peer_id);
    void _on_connect(int32_t p_peer_id, String p_protocol);
    void _on_disconnect(int32_t p_peer_id, bool p_was_clean);
    void _on_close_request(int32_t p_peer_id, int p_code, String p_reason);

    IP_Address get_bind_ip() const;
    void set_bind_ip(const IP_Address& p_bind_ip);

    Ref<CryptoKey> get_private_key() const;
    void set_private_key(Ref<CryptoKey> p_key);

    Ref<X509Certificate> get_ssl_certificate() const;
    void set_ssl_certificate(Ref<X509Certificate> p_cert);

    Ref<X509Certificate> get_ca_chain() const;
    void set_ca_chain(Ref<X509Certificate> p_ca_chain);

    float get_handshake_timeout() const;
    void set_handshake_timeout(float p_timeout);

    virtual Error set_buffers(
        int p_in_buffer,
        int p_in_packets,
        int p_out_buffer,
        int p_out_packets
    ) = 0;

    WebSocketServer();
    ~WebSocketServer();
};

#endif // WEBSOCKET_H
