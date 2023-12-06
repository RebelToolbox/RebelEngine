// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEBRTC_DATA_CHANNEL_GDNATIVE_H
#define WEBRTC_DATA_CHANNEL_GDNATIVE_H

#ifdef WEBRTC_GDNATIVE_ENABLED

#include "modules/gdnative/include/net/godot_net.h"
#include "webrtc_data_channel.h"

class WebRTCDataChannelGDNative : public WebRTCDataChannel {
    GDCLASS(WebRTCDataChannelGDNative, WebRTCDataChannel);

protected:
    static void _bind_methods();

private:
    const godot_net_webrtc_data_channel* interface;

public:
    void set_native_webrtc_data_channel(
        const godot_net_webrtc_data_channel* p_impl
    );

    virtual void set_write_mode(WriteMode mode);
    virtual WriteMode get_write_mode() const;
    virtual bool was_string_packet() const;

    virtual ChannelState get_ready_state() const;
    virtual String get_label() const;
    virtual bool is_ordered() const;
    virtual int get_id() const;
    virtual int get_max_packet_life_time() const;
    virtual int get_max_retransmits() const;
    virtual String get_protocol() const;
    virtual bool is_negotiated() const;
    virtual int get_buffered_amount() const;

    virtual Error poll();
    virtual void close();

    /** Inherited from PacketPeer: **/
    virtual int get_available_packet_count() const;
    virtual Error get_packet(
        const uint8_t** r_buffer,
        int& r_buffer_size
    ); ///< buffer is GONE after next get_packet
    virtual Error put_packet(const uint8_t* p_buffer, int p_buffer_size);

    virtual int get_max_packet_size() const;

    WebRTCDataChannelGDNative();
    ~WebRTCDataChannelGDNative();
};

#endif // WEBRTC_GDNATIVE_ENABLED

#endif // WEBRTC_DATA_CHANNEL_GDNATIVE_H
