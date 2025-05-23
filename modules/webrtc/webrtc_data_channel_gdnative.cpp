// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef WEBRTC_GDNATIVE_ENABLED

#include "webrtc_data_channel_gdnative.h"

#include "core/io/resource_loader.h"
#include "modules/gdnative/nativescript/nativescript.h"

void WebRTCDataChannelGDNative::_bind_methods() {
    ADD_PROPERTY_DEFAULT("write_mode", WRITE_MODE_BINARY);
}

WebRTCDataChannelGDNative::WebRTCDataChannelGDNative() {
    interface = nullptr;
}

WebRTCDataChannelGDNative::~WebRTCDataChannelGDNative() {}

Error WebRTCDataChannelGDNative::poll() {
    ERR_FAIL_COND_V(interface == nullptr, ERR_UNCONFIGURED);
    return (Error)interface->poll(interface->data);
}

void WebRTCDataChannelGDNative::close() {
    ERR_FAIL_COND(interface == nullptr);
    interface->close(interface->data);
}

void WebRTCDataChannelGDNative::set_write_mode(WriteMode p_mode) {
    ERR_FAIL_COND(interface == nullptr);
    interface->set_write_mode(interface->data, p_mode);
}

WebRTCDataChannel::WriteMode WebRTCDataChannelGDNative::get_write_mode() const {
    ERR_FAIL_COND_V(interface == nullptr, WRITE_MODE_BINARY);
    return (WriteMode)interface->get_write_mode(interface->data);
}

bool WebRTCDataChannelGDNative::was_string_packet() const {
    ERR_FAIL_COND_V(interface == nullptr, false);
    return interface->was_string_packet(interface->data);
}

WebRTCDataChannel::ChannelState WebRTCDataChannelGDNative::get_ready_state(
) const {
    ERR_FAIL_COND_V(interface == nullptr, STATE_CLOSED);
    return (ChannelState)interface->get_ready_state(interface->data);
}

String WebRTCDataChannelGDNative::get_label() const {
    ERR_FAIL_COND_V(interface == nullptr, "");
    return String(interface->get_label(interface->data));
}

bool WebRTCDataChannelGDNative::is_ordered() const {
    ERR_FAIL_COND_V(interface == nullptr, false);
    return interface->is_ordered(interface->data);
}

int WebRTCDataChannelGDNative::get_id() const {
    ERR_FAIL_COND_V(interface == nullptr, -1);
    return interface->get_id(interface->data);
}

int WebRTCDataChannelGDNative::get_max_packet_life_time() const {
    ERR_FAIL_COND_V(interface == nullptr, -1);
    return interface->get_max_packet_life_time(interface->data);
}

int WebRTCDataChannelGDNative::get_max_retransmits() const {
    ERR_FAIL_COND_V(interface == nullptr, -1);
    return interface->get_max_retransmits(interface->data);
}

String WebRTCDataChannelGDNative::get_protocol() const {
    ERR_FAIL_COND_V(interface == nullptr, "");
    return String(interface->get_protocol(interface->data));
}

bool WebRTCDataChannelGDNative::is_negotiated() const {
    ERR_FAIL_COND_V(interface == nullptr, false);
    return interface->is_negotiated(interface->data);
}

int WebRTCDataChannelGDNative::get_buffered_amount() const {
    ERR_FAIL_COND_V(interface == nullptr, 0);
    ERR_FAIL_COND_V(interface->next == nullptr, 0);

    return ((rebel_net_webrtc_data_channel_ext*)interface->next)
        ->get_buffered_amount(interface->data);
}

Error WebRTCDataChannelGDNative::get_packet(
    const uint8_t** r_buffer,
    int& r_buffer_size
) {
    ERR_FAIL_COND_V(interface == nullptr, ERR_UNCONFIGURED);
    return (Error
    )interface->get_packet(interface->data, r_buffer, &r_buffer_size);
}

Error WebRTCDataChannelGDNative::put_packet(
    const uint8_t* p_buffer,
    int p_buffer_size
) {
    ERR_FAIL_COND_V(interface == nullptr, ERR_UNCONFIGURED);
    return (Error
    )interface->put_packet(interface->data, p_buffer, p_buffer_size);
}

int WebRTCDataChannelGDNative::get_max_packet_size() const {
    ERR_FAIL_COND_V(interface == nullptr, 0);
    return interface->get_max_packet_size(interface->data);
}

int WebRTCDataChannelGDNative::get_available_packet_count() const {
    ERR_FAIL_COND_V(interface == nullptr, 0);
    return interface->get_available_packet_count(interface->data);
}

void WebRTCDataChannelGDNative::set_native_webrtc_data_channel(
    const rebel_net_webrtc_data_channel* p_impl
) {
    interface = p_impl;
}

#endif // WEBRTC_GDNATIVE_ENABLED
