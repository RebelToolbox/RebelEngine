// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PACKET_BUFFER_H
#define PACKET_BUFFER_H

#include "core/ring_buffer.h"

template <class T>
class PacketBuffer {
private:
    typedef struct {
        uint32_t size;
        T info;
    } _Packet;

    RingBuffer<_Packet> _packets;
    RingBuffer<uint8_t> _payload;

public:
    Error write_packet(
        const uint8_t* p_payload,
        uint32_t p_size,
        const T* p_info
    ) {
#ifdef TOOLS_ENABLED
        // Verbose buffer warnings
        if (p_payload && _payload.space_left() < (int32_t)p_size) {
            ERR_PRINT("Buffer payload full! Dropping data.");
            ERR_FAIL_V(ERR_OUT_OF_MEMORY);
        }
        if (p_info && _packets.space_left() < 1) {
            ERR_PRINT("Too many packets in queue! Dropping data.");
            ERR_FAIL_V(ERR_OUT_OF_MEMORY);
        }
#else
        ERR_FAIL_COND_V(
            p_payload && (uint32_t)_payload.space_left() < p_size,
            ERR_OUT_OF_MEMORY
        );
        ERR_FAIL_COND_V(p_info && _packets.space_left() < 1, ERR_OUT_OF_MEMORY);
#endif

        // If p_info is NULL, only the payload is written
        if (p_info) {
            _Packet p;
            p.size = p_size;
            memcpy(&p.info, p_info, sizeof(T));
            _packets.write(p);
        }

        // If p_payload is NULL, only the packet information is written.
        if (p_payload) {
            _payload.write((const uint8_t*)p_payload, p_size);
        }

        return OK;
    }

    Error read_packet(uint8_t* r_payload, int p_bytes, T* r_info, int& r_read) {
        ERR_FAIL_COND_V(_packets.data_left() < 1, ERR_UNAVAILABLE);
        _Packet p;
        _packets.read(&p, 1);
        ERR_FAIL_COND_V(_payload.data_left() < (int)p.size, ERR_BUG);
        ERR_FAIL_COND_V(p_bytes < (int)p.size, ERR_OUT_OF_MEMORY);

        r_read = p.size;
        memcpy(r_info, &p.info, sizeof(T));
        _payload.read(r_payload, p.size);
        return OK;
    }

    void discard_payload(int p_size) {
        _packets.decrease_write(p_size);
    }

    void resize(int p_pkt_shift, int p_buf_shift) {
        _packets.resize(p_pkt_shift);
        _payload.resize(p_buf_shift);
    }

    int packets_left() const {
        return _packets.data_left();
    }

    void clear() {
        _payload.resize(0);
        _packets.resize(0);
    }

    PacketBuffer() {
        clear();
    }

    ~PacketBuffer() {
        clear();
    }
};

#endif // PACKET_BUFFER_H
