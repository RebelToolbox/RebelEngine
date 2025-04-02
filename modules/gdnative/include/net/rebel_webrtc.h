// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_WEBRTC_H
#define GDNATIVE_WEBRTC_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REBEL_NET_WEBRTC_API_MAJOR 3
#define REBEL_NET_WEBRTC_API_MINOR 4

/* Library Interface (used to set default GDNative WebRTC implementation */
typedef struct {
    rebel_gdnative_api_version version; /* version of our API */

    /* Called when the library is unset as default interface via
     * rebel_net_set_webrtc_library */
    void (*unregistered)();

    /* Used by WebRTCPeerConnection create when GDNative is the default
     * implementation. */
    /* Takes a pointer to WebRTCPeerConnectionGDNative, should bind and return
     * OK, failure if binding was unsuccessful. */
    rebel_error (*create_peer_connection)(rebel_object*);

    void* next; /* For extension */
} rebel_net_webrtc_library;

/* WebRTCPeerConnection interface */
typedef struct {
    rebel_gdnative_api_version version; /* version of our API */

    rebel_object* data; /* User reference */

    /* This is WebRTCPeerConnection */
    rebel_int (*get_connection_state)(const void*);

    rebel_error (*initialize)(void*, const rebel_dictionary*);
    rebel_object* (*create_data_channel)(void*, const char* p_channel_name, const rebel_dictionary*);
    rebel_error (*create_offer)(void*);
    rebel_error (*create_answer)(void*); /* unused for now, should be done
                                            automatically on
                                            set_local_description */
    rebel_error (*set_remote_description)(void*, const char*, const char*);
    rebel_error (*set_local_description)(void*, const char*, const char*);
    rebel_error (*add_ice_candidate)(void*, const char*, int, const char*);
    rebel_error (*poll)(void*);
    void (*close)(void*);

    void* next; /* For extension? */
} rebel_net_webrtc_peer_connection;

/* WebRTCDataChannel interface */
typedef struct {
    rebel_gdnative_api_version version; /* version of our API */

    rebel_object* data; /* User reference */

    /* This is PacketPeer */
    rebel_error (*get_packet)(void*, const uint8_t**, int*);
    rebel_error (*put_packet)(void*, const uint8_t*, int);
    rebel_int (*get_available_packet_count)(const void*);
    rebel_int (*get_max_packet_size)(const void*);

    /* This is WebRTCDataChannel */
    void (*set_write_mode)(void*, rebel_int);
    rebel_int (*get_write_mode)(const void*);
    bool (*was_string_packet)(const void*);

    rebel_int (*get_ready_state)(const void*);
    const char* (*get_label)(const void*);
    bool (*is_ordered)(const void*);
    int (*get_id)(const void*);
    int (*get_max_packet_life_time)(const void*);
    int (*get_max_retransmits)(const void*);
    const char* (*get_protocol)(const void*);
    bool (*is_negotiated)(const void*);

    rebel_error (*poll)(void*);
    void (*close)(void*);

    void* next; /* For extension? */
} rebel_net_webrtc_data_channel;

/* Extensions to WebRTCDataChannel */
typedef struct {
    int (*get_buffered_amount)(const void*);

    void* next; /* For extension? */
} rebel_net_webrtc_data_channel_ext;

/* Set the default GDNative library */
rebel_error GDAPI rebel_net_set_webrtc_library(const rebel_net_webrtc_library*);
/* Binds a WebRTCPeerConnectionGDNative to the provided interface */
void GDAPI
rebel_net_bind_webrtc_peer_connection(rebel_object* p_obj, const rebel_net_webrtc_peer_connection*);
/* Binds a WebRTCDataChannelGDNative to the provided interface */
void GDAPI
rebel_net_bind_webrtc_data_channel(rebel_object* p_obj, const rebel_net_webrtc_data_channel*);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_WEBRTC_H
