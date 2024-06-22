// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef WEB_ENABLED

#include "emws_peer.h"

#include "core/io/ip.h"

void EMWSPeer::set_sock(
    int p_sock,
    unsigned int p_in_buf_size,
    unsigned int p_in_pkt_size,
    unsigned int p_out_buf_size
) {
    peer_sock = p_sock;
    _in_buffer.resize(p_in_pkt_size, p_in_buf_size);
    _packet_buffer.resize((1 << p_in_buf_size));
    _out_buf_size = p_out_buf_size;
}

void EMWSPeer::set_write_mode(WriteMode p_mode) {
    write_mode = p_mode;
}

EMWSPeer::WriteMode EMWSPeer::get_write_mode() const {
    return write_mode;
}

Error EMWSPeer::read_msg(
    const uint8_t* p_data,
    uint32_t p_size,
    bool p_is_string
) {
    uint8_t is_string = p_is_string ? 1 : 0;
    return _in_buffer.write_packet(p_data, p_size, &is_string);
}

Error EMWSPeer::put_packet(const uint8_t* p_buffer, int p_buffer_size) {
    ERR_FAIL_COND_V(
        _out_buf_size
            && ((uint64_t)rebel_js_websocket_buffered_amount(peer_sock)
                    + p_buffer_size
                >= (1ULL << _out_buf_size)),
        ERR_OUT_OF_MEMORY
    );

    int is_bin = write_mode == WebSocketPeer::WRITE_MODE_BINARY ? 1 : 0;

    if (rebel_js_websocket_send(peer_sock, p_buffer, p_buffer_size, is_bin)
        != 0) {
        return FAILED;
    }

    return OK;
};

Error EMWSPeer::get_packet(const uint8_t** r_buffer, int& r_buffer_size) {
    if (_in_buffer.packets_left() == 0) {
        return ERR_UNAVAILABLE;
    }

    PoolVector<uint8_t>::Write rw = _packet_buffer.write();
    int read                      = 0;
    Error err =
        _in_buffer
            .read_packet(rw.ptr(), _packet_buffer.size(), &_is_string, read);
    ERR_FAIL_COND_V(err != OK, err);

    *r_buffer     = rw.ptr();
    r_buffer_size = read;

    return OK;
};

int EMWSPeer::get_available_packet_count() const {
    return _in_buffer.packets_left();
};

int EMWSPeer::get_current_outbound_buffered_amount() const {
    if (peer_sock != -1) {
        return rebel_js_websocket_buffered_amount(peer_sock);
    }
    return 0;
}

bool EMWSPeer::was_string_packet() const {
    return _is_string;
};

bool EMWSPeer::is_connected_to_host() const {
    return peer_sock != -1;
};

void EMWSPeer::close(int p_code, String p_reason) {
    if (peer_sock != -1) {
        rebel_js_websocket_close(peer_sock, p_code, p_reason.utf8().get_data());
    }
    _is_string = 0;
    _in_buffer.clear();
    peer_sock = -1;
};

IP_Address EMWSPeer::get_connected_host() const {
    ERR_FAIL_V_MSG(IP_Address(), "Not supported in Web exports.");
};

uint16_t EMWSPeer::get_connected_port() const {
    ERR_FAIL_V_MSG(0, "Not supported in Web exports.");
};

void EMWSPeer::set_no_delay(bool p_enabled) {
    ERR_FAIL_MSG("'set_no_delay' is not supported in Web exports.");
}

EMWSPeer::EMWSPeer() {
    _out_buf_size = 0;
    peer_sock     = -1;
    write_mode    = WRITE_MODE_BINARY;
    close();
};

EMWSPeer::~EMWSPeer() {
    close();
};

#endif // WEB_ENABLED
