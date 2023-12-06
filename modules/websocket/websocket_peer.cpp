// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "websocket_peer.h"

GDCINULL(WebSocketPeer);

WebSocketPeer::WebSocketPeer() {}

WebSocketPeer::~WebSocketPeer() {}

void WebSocketPeer::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("get_write_mode"),
        &WebSocketPeer::get_write_mode
    );
    ClassDB::bind_method(
        D_METHOD("set_write_mode", "mode"),
        &WebSocketPeer::set_write_mode
    );
    ClassDB::bind_method(
        D_METHOD("is_connected_to_host"),
        &WebSocketPeer::is_connected_to_host
    );
    ClassDB::bind_method(
        D_METHOD("was_string_packet"),
        &WebSocketPeer::was_string_packet
    );
    ClassDB::bind_method(
        D_METHOD("close", "code", "reason"),
        &WebSocketPeer::close,
        DEFVAL(1000),
        DEFVAL("")
    );
    ClassDB::bind_method(
        D_METHOD("get_connected_host"),
        &WebSocketPeer::get_connected_host
    );
    ClassDB::bind_method(
        D_METHOD("get_connected_port"),
        &WebSocketPeer::get_connected_port
    );
    ClassDB::bind_method(
        D_METHOD("set_no_delay", "enabled"),
        &WebSocketPeer::set_no_delay
    );
    ClassDB::bind_method(
        D_METHOD("get_current_outbound_buffered_amount"),
        &WebSocketPeer::get_current_outbound_buffered_amount
    );

    BIND_ENUM_CONSTANT(WRITE_MODE_TEXT);
    BIND_ENUM_CONSTANT(WRITE_MODE_BINARY);
}
