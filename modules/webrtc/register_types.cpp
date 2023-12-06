// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/project_settings.h"
#include "webrtc_data_channel.h"
#include "webrtc_peer_connection.h"

#ifdef JAVASCRIPT_ENABLED
#include "emscripten.h"
#include "webrtc_peer_connection_js.h"
#endif
#ifdef WEBRTC_GDNATIVE_ENABLED
#include "webrtc_data_channel_gdnative.h"
#include "webrtc_peer_connection_gdnative.h"
#endif
#include "webrtc_multiplayer.h"

void register_webrtc_types() {
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

    _SET_HINT(WRTC_IN_BUF, 64, 4096);

#ifdef JAVASCRIPT_ENABLED
    WebRTCPeerConnectionJS::make_default();
#elif defined(WEBRTC_GDNATIVE_ENABLED)
    WebRTCPeerConnectionGDNative::make_default();
#endif

    ClassDB::register_custom_instance_class<WebRTCPeerConnection>();
#ifdef WEBRTC_GDNATIVE_ENABLED
    ClassDB::register_class<WebRTCPeerConnectionGDNative>();
    ClassDB::register_class<WebRTCDataChannelGDNative>();
#endif
    ClassDB::register_virtual_class<WebRTCDataChannel>();
    ClassDB::register_class<WebRTCMultiplayer>();
}

void unregister_webrtc_types() {}
