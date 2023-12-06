// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "webrtc_peer_connection.h"

WebRTCPeerConnection* (*WebRTCPeerConnection::_create)() = nullptr;

Ref<WebRTCPeerConnection> WebRTCPeerConnection::create_ref() {
    return create();
}

WebRTCPeerConnection* WebRTCPeerConnection::create() {
    if (!_create) {
        return nullptr;
    }
    return _create();
}

void WebRTCPeerConnection::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("initialize", "configuration"),
        &WebRTCPeerConnection::initialize,
        DEFVAL(Dictionary())
    );
    ClassDB::bind_method(
        D_METHOD("create_data_channel", "label", "options"),
        &WebRTCPeerConnection::create_data_channel,
        DEFVAL(Dictionary())
    );
    ClassDB::bind_method(
        D_METHOD("create_offer"),
        &WebRTCPeerConnection::create_offer
    );
    ClassDB::bind_method(
        D_METHOD("set_local_description", "type", "sdp"),
        &WebRTCPeerConnection::set_local_description
    );
    ClassDB::bind_method(
        D_METHOD("set_remote_description", "type", "sdp"),
        &WebRTCPeerConnection::set_remote_description
    );
    ClassDB::bind_method(
        D_METHOD("add_ice_candidate", "media", "index", "name"),
        &WebRTCPeerConnection::add_ice_candidate
    );
    ClassDB::bind_method(D_METHOD("poll"), &WebRTCPeerConnection::poll);
    ClassDB::bind_method(D_METHOD("close"), &WebRTCPeerConnection::close);

    ClassDB::bind_method(
        D_METHOD("get_connection_state"),
        &WebRTCPeerConnection::get_connection_state
    );

    ADD_SIGNAL(MethodInfo(
        "session_description_created",
        PropertyInfo(Variant::STRING, "type"),
        PropertyInfo(Variant::STRING, "sdp")
    ));
    ADD_SIGNAL(MethodInfo(
        "ice_candidate_created",
        PropertyInfo(Variant::STRING, "media"),
        PropertyInfo(Variant::INT, "index"),
        PropertyInfo(Variant::STRING, "name")
    ));
    ADD_SIGNAL(MethodInfo(
        "data_channel_received",
        PropertyInfo(Variant::OBJECT, "channel")
    ));

    BIND_ENUM_CONSTANT(STATE_NEW);
    BIND_ENUM_CONSTANT(STATE_CONNECTING);
    BIND_ENUM_CONSTANT(STATE_CONNECTED);
    BIND_ENUM_CONSTANT(STATE_DISCONNECTED);
    BIND_ENUM_CONSTANT(STATE_FAILED);
    BIND_ENUM_CONSTANT(STATE_CLOSED);
}

WebRTCPeerConnection::WebRTCPeerConnection() {}

WebRTCPeerConnection::~WebRTCPeerConnection() {}
