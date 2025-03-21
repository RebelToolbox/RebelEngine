// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STREAM_PEER_GDNATIVE_H
#define STREAM_PEER_GDNATIVE_H

#include "core/io/stream_peer.h"
#include "modules/gdnative/gdnative.h"
#include "modules/gdnative/include/net/rebel_net.h"

class StreamPeerGDNative : public StreamPeer {
    GDCLASS(StreamPeerGDNative, StreamPeer);

protected:
    static void _bind_methods();
    const rebel_net_stream_peer* interface;

public:
    StreamPeerGDNative();
    ~StreamPeerGDNative();

    /* Sets the interface implementation from GDNative */
    void set_native_stream_peer(const rebel_net_stream_peer* p_interface);

    /* Specific to StreamPeer */
    Error put_data(const uint8_t* p_data, int p_bytes);
    Error put_partial_data(const uint8_t* p_data, int p_bytes, int& r_sent);
    Error get_data(uint8_t* p_buffer, int p_bytes);
    Error get_partial_data(uint8_t* p_buffer, int p_bytes, int& r_received);
    int get_available_bytes() const;
};

#endif // STREAM_PEER_GDNATIVE_H
