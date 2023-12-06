// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "dtls_server_mbedtls.h"

#include "packet_peer_mbed_dtls.h"

Error DTLSServerMbedTLS::setup(
    Ref<CryptoKey> p_key,
    Ref<X509Certificate> p_cert,
    Ref<X509Certificate> p_ca_chain
) {
    ERR_FAIL_COND_V(_cookies->setup() != OK, ERR_ALREADY_IN_USE);
    _key      = p_key;
    _cert     = p_cert;
    _ca_chain = p_ca_chain;
    return OK;
}

void DTLSServerMbedTLS::stop() {
    _cookies->clear();
}

Ref<PacketPeerDTLS> DTLSServerMbedTLS::take_connection(
    Ref<PacketPeerUDP> p_udp_peer
) {
    Ref<PacketPeerMbedDTLS> out;
    out.instance();

    ERR_FAIL_COND_V(!out.is_valid(), out);
    ERR_FAIL_COND_V(!p_udp_peer.is_valid(), out);
    out->accept_peer(p_udp_peer, _key, _cert, _ca_chain, _cookies);
    return out;
}

DTLSServer* DTLSServerMbedTLS::_create_func() {
    return memnew(DTLSServerMbedTLS);
}

void DTLSServerMbedTLS::initialize() {
    _create   = _create_func;
    available = true;
}

void DTLSServerMbedTLS::finalize() {
    _create   = nullptr;
    available = false;
}

DTLSServerMbedTLS::DTLSServerMbedTLS() {
    _cookies.instance();
}

DTLSServerMbedTLS::~DTLSServerMbedTLS() {
    stop();
}
