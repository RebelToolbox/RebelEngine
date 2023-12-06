// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "packet_peer_dtls.h"

#include "core/os/file_access.h"
#include "core/project_settings.h"

PacketPeerDTLS* (*PacketPeerDTLS::_create)() = nullptr;
bool PacketPeerDTLS::available               = false;

PacketPeerDTLS* PacketPeerDTLS::create() {
    if (_create) {
        return _create();
    }
    return nullptr;
}

bool PacketPeerDTLS::is_available() {
    return available;
}

void PacketPeerDTLS::_bind_methods() {
    ClassDB::bind_method(D_METHOD("poll"), &PacketPeerDTLS::poll);
    ClassDB::bind_method(
        D_METHOD(
            "connect_to_peer",
            "packet_peer",
            "validate_certs",
            "for_hostname",
            "valid_certificate"
        ),
        &PacketPeerDTLS::connect_to_peer,
        DEFVAL(true),
        DEFVAL(String()),
        DEFVAL(Ref<X509Certificate>())
    );
    ClassDB::bind_method(D_METHOD("get_status"), &PacketPeerDTLS::get_status);
    ClassDB::bind_method(
        D_METHOD("disconnect_from_peer"),
        &PacketPeerDTLS::disconnect_from_peer
    );

    BIND_ENUM_CONSTANT(STATUS_DISCONNECTED);
    BIND_ENUM_CONSTANT(STATUS_HANDSHAKING);
    BIND_ENUM_CONSTANT(STATUS_CONNECTED);
    BIND_ENUM_CONSTANT(STATUS_ERROR);
    BIND_ENUM_CONSTANT(STATUS_ERROR_HOSTNAME_MISMATCH);
}

PacketPeerDTLS::PacketPeerDTLS() {}
