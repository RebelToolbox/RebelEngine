// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STREAM_PEER_SSL_H
#define STREAM_PEER_SSL_H

#include "core/crypto/crypto.h"
#include "core/io/stream_peer.h"

class StreamPeerSSL : public StreamPeer {
    GDCLASS(StreamPeerSSL, StreamPeer);

protected:
    static StreamPeerSSL* (*_create)();
    static void _bind_methods();

    static bool available;

    bool blocking_handshake;

public:
    enum Status {
        STATUS_DISCONNECTED,
        STATUS_HANDSHAKING,
        STATUS_CONNECTED,
        STATUS_ERROR,
        STATUS_ERROR_HOSTNAME_MISMATCH
    };

    void set_blocking_handshake_enabled(bool p_enabled);
    bool is_blocking_handshake_enabled() const;

    virtual void poll() = 0;
    virtual Error accept_stream(
        Ref<StreamPeer> p_base,
        Ref<CryptoKey> p_key,
        Ref<X509Certificate> p_cert,
        Ref<X509Certificate> p_ca_chain = Ref<X509Certificate>()
    ) = 0;
    virtual Error connect_to_stream(
        Ref<StreamPeer> p_base,
        bool p_validate_certs             = false,
        const String& p_for_hostname      = String(),
        Ref<X509Certificate> p_valid_cert = Ref<X509Certificate>()
    )                                 = 0;
    virtual Status get_status() const = 0;

    virtual void disconnect_from_stream() = 0;

    static StreamPeerSSL* create();

    static bool is_available();

    StreamPeerSSL();
};

VARIANT_ENUM_CAST(StreamPeerSSL::Status);

#endif // STREAM_PEER_SSL_H
