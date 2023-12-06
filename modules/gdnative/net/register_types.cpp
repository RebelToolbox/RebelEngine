// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "multiplayer_peer_gdnative.h"
#include "packet_peer_gdnative.h"
#include "stream_peer_gdnative.h"

void register_net_types() {
    ClassDB::register_class<MultiplayerPeerGDNative>();
    ClassDB::register_class<PacketPeerGDNative>();
    ClassDB::register_class<StreamPeerGDNative>();
}

void unregister_net_types() {}
