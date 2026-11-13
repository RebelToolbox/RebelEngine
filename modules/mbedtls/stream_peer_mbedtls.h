// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STREAM_PEER_OPEN_SSL_H
#define STREAM_PEER_OPEN_SSL_H

#include "core/io/stream_peer_ssl.h"
#include "ssl_context_mbedtls.h"

class StreamPeerMbedTLS : public StreamPeerSSL {
private:
    Status status;
    String hostname;

    Ref<StreamPeer> base;

    static StreamPeerSSL* _create_func();

    static int bio_recv(void* ctx, unsigned char* buf, size_t len);
    static int bio_send(void* ctx, const unsigned char* buf, size_t len);
    void _cleanup();

protected:
    Ref<SSLContextMbedTLS> ssl_ctx;

    static void _bind_methods();

    Error _do_handshake();

public:
    void poll() override;
    Error accept_stream(
        Ref<StreamPeer> p_base,
        Ref<CryptoKey> p_key,
        Ref<X509Certificate> p_cert,
        Ref<X509Certificate> p_ca_chain = Ref<X509Certificate>()
    ) override;
    Error connect_to_stream(
        Ref<StreamPeer> p_base,
        bool p_validate_certs             = false,
        const String& p_for_hostname      = String(),
        Ref<X509Certificate> p_valid_cert = Ref<X509Certificate>()
    ) override;
    Status get_status() const override;

    void disconnect_from_stream() override;

    Error put_data(const uint8_t* p_data, int p_bytes) override;
    Error put_partial_data(
        const uint8_t* p_data,
        int p_bytes,
        int& r_sent
    ) override;

    Error get_data(uint8_t* p_buffer, int p_bytes) override;
    Error get_partial_data(
        uint8_t* p_buffer,
        int p_bytes,
        int& r_received
    ) override;

    int get_available_bytes() const override;

    static void initialize_ssl();
    static void finalize_ssl();

    StreamPeerMbedTLS();
    ~StreamPeerMbedTLS() override;
};

#endif // STREAM_PEER_SSL_H
