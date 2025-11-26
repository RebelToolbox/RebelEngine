#ifndef REBEL_ENET_EXT_H
#define REBEL_ENET_EXT_H

/** Sets the host field in the address parameter from ip struct.
    @param address destination to store resolved address
    @param ip the ip struct to read from
    @param size the size of the ip struct.
    @retval 0 on success
    @retval != 0 on failure
    @returns the address of the given ip in address on success.
*/
ENET_API void enet_address_set_ip(ENetAddress * address, const uint8_t * ip, size_t size);

ENET_API void enet_host_dtls_server_setup (ENetHost *, void *, void *);
ENET_API void enet_host_dtls_client_setup (ENetHost *, void *, uint8_t, const char *);
ENET_API void enet_host_refuse_new_connections (ENetHost *, int);

#endif // REBEL_ENET_EXT_H
