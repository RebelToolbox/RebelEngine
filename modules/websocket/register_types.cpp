// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/error_macros.h"
#include "core/project_settings.h"
#ifdef JAVASCRIPT_ENABLED
#include "emscripten.h"
#include "emws_client.h"
#include "emws_peer.h"
#include "emws_server.h"
#else
#include "wsl_client.h"
#include "wsl_server.h"
#endif

void register_websocket_types() {
#define _SET_HINT(NAME, _VAL_, _MAX_)                                          \
    GLOBAL_DEF(NAME, _VAL_);                                                   \
    ProjectSettings::get_singleton()->set_custom_property_info(                \
        NAME,                                                                  \
        PropertyInfo(                                                          \
            Variant::INT,                                                      \
            NAME,                                                              \
            PROPERTY_HINT_RANGE,                                               \
            "2," #_MAX_ ",1,or_greater"                                        \
        )                                                                      \
    );

    // Client buffers project settings
    _SET_HINT(WSC_IN_BUF, 64, 4096);
    _SET_HINT(WSC_IN_PKT, 1024, 16384);
    _SET_HINT(WSC_OUT_BUF, 64, 4096);
    _SET_HINT(WSC_OUT_PKT, 1024, 16384);

    // Server buffers project settings
    _SET_HINT(WSS_IN_BUF, 64, 4096);
    _SET_HINT(WSS_IN_PKT, 1024, 16384);
    _SET_HINT(WSS_OUT_BUF, 64, 4096);
    _SET_HINT(WSS_OUT_PKT, 1024, 16384);

#ifdef JAVASCRIPT_ENABLED
    EMWSPeer::make_default();
    EMWSClient::make_default();
    EMWSServer::make_default();
#else
    WSLPeer::make_default();
    WSLClient::make_default();
    WSLServer::make_default();
#endif

    ClassDB::register_virtual_class<WebSocketMultiplayerPeer>();
    ClassDB::register_custom_instance_class<WebSocketServer>();
    ClassDB::register_custom_instance_class<WebSocketClient>();
    ClassDB::register_custom_instance_class<WebSocketPeer>();
}

void unregister_websocket_types() {}
