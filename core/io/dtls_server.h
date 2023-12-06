// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef DTLS_SERVER_H
#define DTLS_SERVER_H

#include "core/io/net_socket.h"
#include "core/io/packet_peer_dtls.h"

class DTLSServer : public Reference {
    GDCLASS(DTLSServer, Reference);

protected:
    static DTLSServer* (*_create)();
    static void _bind_methods();

    static bool available;

public:
    static bool is_available();
    static DTLSServer* create();

    virtual Error setup(
        Ref<CryptoKey> p_key,
        Ref<X509Certificate> p_cert,
        Ref<X509Certificate> p_ca_chain = Ref<X509Certificate>()
    )                                                                      = 0;
    virtual void stop()                                                    = 0;
    virtual Ref<PacketPeerDTLS> take_connection(Ref<PacketPeerUDP> p_peer) = 0;

    DTLSServer();
};

#endif // DTLS_SERVER_H
