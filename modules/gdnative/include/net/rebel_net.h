// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_NET_H
#define GDNATIVE_NET_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

// For future versions of the API we should only add new functions at the end of
// the structure and use the version info to detect whether a call is available

// Use these to populate version in your plugin
#define REBEL_NET_API_MAJOR 3
#define REBEL_NET_API_MINOR 1

typedef struct {
    rebel_gdnative_api_version version; /* version of our API */
    rebel_object* data;                 /* User reference */

    /* This is StreamPeer */
    rebel_error (*get_data)(void* user, uint8_t* p_buffer, int p_bytes);
    rebel_error (*get_partial_data)(
        void* user,
        uint8_t* p_buffer,
        int p_bytes,
        int* r_received
    );
    rebel_error (*put_data)(void* user, const uint8_t* p_data, int p_bytes);
    rebel_error (*put_partial_data)(
        void* user,
        const uint8_t* p_data,
        int p_bytes,
        int* r_sent
    );

    int (*get_available_bytes)(const void* user);

    void* next; /* For extension? */
} rebel_net_stream_peer;

/* Binds a StreamPeerGDNative to the provided interface */
void rebel_net_bind_stream_peer(
    rebel_object* p_obj,
    const rebel_net_stream_peer* p_interface
);

typedef struct {
    rebel_gdnative_api_version version; /* version of our API */

    rebel_object* data; /* User reference */

    /* This is PacketPeer */
    rebel_error (*get_packet)(void*, const uint8_t**, int*);
    rebel_error (*put_packet)(void*, const uint8_t*, int);
    rebel_int (*get_available_packet_count)(const void*);
    rebel_int (*get_max_packet_size)(const void*);

    void* next; /* For extension? */
} rebel_net_packet_peer;

/* Binds a PacketPeerGDNative to the provided interface */
void GDAPI
rebel_net_bind_packet_peer(rebel_object* p_obj, const rebel_net_packet_peer*);

typedef struct {
    rebel_gdnative_api_version version; /* version of our API */

    rebel_object* data; /* User reference */

    /* This is PacketPeer */
    rebel_error (*get_packet)(void*, const uint8_t**, int*);
    rebel_error (*put_packet)(void*, const uint8_t*, int);
    rebel_int (*get_available_packet_count)(const void*);
    rebel_int (*get_max_packet_size)(const void*);

    /* This is NetworkedMultiplayerPeer */
    void (*set_transfer_mode)(void*, rebel_int);
    rebel_int (*get_transfer_mode)(const void*);
    // 0 = broadcast, 1 = server, <0 = all but abs(value)
    void (*set_target_peer)(void*, rebel_int);
    rebel_int (*get_packet_peer)(const void*);
    rebel_bool (*is_server)(const void*);
    void (*poll)(void*);
    // Must be > 0, 1 is for server
    int32_t (*get_unique_id)(const void*);
    void (*set_refuse_new_connections)(void*, rebel_bool);
    rebel_bool (*is_refusing_new_connections)(const void*);
    rebel_int (*get_connection_status)(const void*);

    void* next; /* For extension? Or maybe not... */
} rebel_net_multiplayer_peer;

/* Binds a MultiplayerPeerGDNative to the provided interface */
void GDAPI
rebel_net_bind_multiplayer_peer(rebel_object* p_obj, const rebel_net_multiplayer_peer*);

#ifdef __cplusplus
}
#endif

// WebRTC Bindings
#include "net/rebel_webrtc.h"

#endif // GDNATIVE_NET_H
