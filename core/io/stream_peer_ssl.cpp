// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "stream_peer_ssl.h"

#include "core/engine.h"

StreamPeerSSL* (*StreamPeerSSL::_create)() = nullptr;

StreamPeerSSL* StreamPeerSSL::create() {
    if (_create) {
        return _create();
    }
    return nullptr;
}

bool StreamPeerSSL::available = false;

bool StreamPeerSSL::is_available() {
    return available;
}

void StreamPeerSSL::set_blocking_handshake_enabled(bool p_enabled) {
    blocking_handshake = p_enabled;
}

bool StreamPeerSSL::is_blocking_handshake_enabled() const {
    return blocking_handshake;
}

void StreamPeerSSL::_bind_methods() {
    ClassDB::bind_method(D_METHOD("poll"), &StreamPeerSSL::poll);
    ClassDB::bind_method(
        D_METHOD(
            "accept_stream",
            "stream",
            "private_key",
            "certificate",
            "chain"
        ),
        &StreamPeerSSL::accept_stream,
        DEFVAL(Ref<X509Certificate>())
    );
    ClassDB::bind_method(
        D_METHOD(
            "connect_to_stream",
            "stream",
            "validate_certs",
            "for_hostname",
            "valid_certificate"
        ),
        &StreamPeerSSL::connect_to_stream,
        DEFVAL(false),
        DEFVAL(String()),
        DEFVAL(Ref<X509Certificate>())
    );
    ClassDB::bind_method(D_METHOD("get_status"), &StreamPeerSSL::get_status);
    ClassDB::bind_method(
        D_METHOD("disconnect_from_stream"),
        &StreamPeerSSL::disconnect_from_stream
    );
    ClassDB::bind_method(
        D_METHOD("set_blocking_handshake_enabled", "enabled"),
        &StreamPeerSSL::set_blocking_handshake_enabled
    );
    ClassDB::bind_method(
        D_METHOD("is_blocking_handshake_enabled"),
        &StreamPeerSSL::is_blocking_handshake_enabled
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "blocking_handshake"),
        "set_blocking_handshake_enabled",
        "is_blocking_handshake_enabled"
    );

    BIND_ENUM_CONSTANT(STATUS_DISCONNECTED);
    BIND_ENUM_CONSTANT(STATUS_HANDSHAKING);
    BIND_ENUM_CONSTANT(STATUS_CONNECTED);
    BIND_ENUM_CONSTANT(STATUS_ERROR);
    BIND_ENUM_CONSTANT(STATUS_ERROR_HOSTNAME_MISMATCH);
}

StreamPeerSSL::StreamPeerSSL() {
    blocking_handshake = true;
}
