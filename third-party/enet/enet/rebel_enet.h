// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REBEL_ENET_H
#define REBEL_ENET_H

#ifdef WINDOWS_ENABLED
#include <stdint.h>
#include <winsock2.h>
#endif
#ifdef UNIX_ENABLED
#include <arpa/inet.h>
#endif

#ifdef MSG_MAXIOVLEN
#define ENET_BUFFER_MAXIMUM MSG_MAXIOVLEN
#endif

typedef void *ENetSocket;

#define ENET_SOCKET_NULL NULL

#define ENET_HOST_TO_NET_16(value) (htons(value)) /**< macro that converts host to net byte-order of a 16-bit value */
#define ENET_HOST_TO_NET_32(value) (htonl(value)) /**< macro that converts host to net byte-order of a 32-bit value */

#define ENET_NET_TO_HOST_16(value) (ntohs(value)) /**< macro that converts net to host byte-order of a 16-bit value */
#define ENET_NET_TO_HOST_32(value) (ntohl(value)) /**< macro that converts net to host byte-order of a 32-bit value */

typedef struct
{
	void *data;
	size_t dataLength;
} ENetBuffer;

#define ENET_CALLBACK

#define ENET_API extern

typedef void ENetSocketSet;

/**
 * Portable internet address structure.
 *
 * The host must be specified in network byte-order, and the port must be in host
 * byte-order. The constant ENET_HOST_ANY may be used to specify the default
 * server host. The constant ENET_HOST_BROADCAST may be used to specify the
 * broadcast address (255.255.255.255).  This makes sense for enet_host_connect,
 * but not for enet_host_create.  Once a server responds to a broadcast, the
 * address is updated from ENET_HOST_BROADCAST to the server's actual IP address.
 */
typedef struct _ENetAddress
{
   uint8_t host[16];
   uint16_t port;
   uint8_t wildcard;
} ENetAddress;

#define enet_host_equal(host_a, host_b) (memcmp(&host_a, &host_b,16) == 0)

#endif // REBEL_ENET_H
