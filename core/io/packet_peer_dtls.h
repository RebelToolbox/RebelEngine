// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PACKET_PEER_DTLS_H
#define PACKET_PEER_DTLS_H

#include "core/crypto/crypto.h"
#include "core/io/packet_peer_udp.h"

class PacketPeerDTLS : public PacketPeer {
    GDCLASS(PacketPeerDTLS, PacketPeer);

protected:
    static PacketPeerDTLS* (*_create)();
    static void _bind_methods();

    static bool available;

public:
    enum Status {
        STATUS_DISCONNECTED,
        STATUS_HANDSHAKING,
        STATUS_CONNECTED,
        STATUS_ERROR,
        STATUS_ERROR_HOSTNAME_MISMATCH
    };

    virtual void poll() = 0;
    virtual Error connect_to_peer(
        Ref<PacketPeerUDP> p_base,
        bool p_validate_certs           = true,
        const String& p_for_hostname    = String(),
        Ref<X509Certificate> p_ca_certs = Ref<X509Certificate>()
    )                                   = 0;
    virtual void disconnect_from_peer() = 0;
    virtual Status get_status() const   = 0;

    static PacketPeerDTLS* create();
    static bool is_available();

    PacketPeerDTLS();
};

VARIANT_ENUM_CAST(PacketPeerDTLS::Status);

#endif // PACKET_PEER_DTLS_H
