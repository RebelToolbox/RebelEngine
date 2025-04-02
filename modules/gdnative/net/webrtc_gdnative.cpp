// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "modules/gdnative/gdnative.h"
#include "modules/gdnative/include/net/rebel_net.h"

#ifdef WEBRTC_GDNATIVE_ENABLED
#include "modules/webrtc/webrtc_data_channel_gdnative.h"
#include "modules/webrtc/webrtc_peer_connection_gdnative.h"
#endif

extern "C" {

void GDAPI rebel_net_bind_webrtc_peer_connection(
    rebel_object* p_obj,
    const rebel_net_webrtc_peer_connection* p_impl
) {
#ifdef WEBRTC_GDNATIVE_ENABLED
    ((WebRTCPeerConnectionGDNative*)p_obj)
        ->set_native_webrtc_peer_connection(p_impl);
#endif
}

void GDAPI rebel_net_bind_webrtc_data_channel(
    rebel_object* p_obj,
    const rebel_net_webrtc_data_channel* p_impl
) {
#ifdef WEBRTC_GDNATIVE_ENABLED
    ((WebRTCDataChannelGDNative*)p_obj)->set_native_webrtc_data_channel(p_impl);
#endif
}

rebel_error GDAPI
rebel_net_set_webrtc_library(const rebel_net_webrtc_library* p_lib) {
#ifdef WEBRTC_GDNATIVE_ENABLED
    return (rebel_error)WebRTCPeerConnectionGDNative::set_default_library(p_lib
    );
#else
    return (rebel_error)ERR_UNAVAILABLE;
#endif
}
}
