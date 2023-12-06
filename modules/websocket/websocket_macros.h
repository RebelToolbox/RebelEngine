// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEBSOCKETMACTOS_H
#define WEBSOCKETMACTOS_H

#define WSC_IN_BUF  "network/limits/websocket_client/max_in_buffer_kb"
#define WSC_IN_PKT  "network/limits/websocket_client/max_in_packets"
#define WSC_OUT_BUF "network/limits/websocket_client/max_out_buffer_kb"
#define WSC_OUT_PKT "network/limits/websocket_client/max_out_packets"

#define WSS_IN_BUF  "network/limits/websocket_server/max_in_buffer_kb"
#define WSS_IN_PKT  "network/limits/websocket_server/max_in_packets"
#define WSS_OUT_BUF "network/limits/websocket_server/max_out_buffer_kb"
#define WSS_OUT_PKT "network/limits/websocket_server/max_out_packets"

#define GDCICLASS(CNAME)                                                       \
                                                                               \
public:                                                                        \
    static CNAME* (*_create)();                                                \
                                                                               \
    static Ref<CNAME> create_ref() {                                           \
        if (!_create) return Ref<CNAME>();                                     \
        return Ref<CNAME>(_create());                                          \
    }                                                                          \
                                                                               \
    static CNAME* create() {                                                   \
        if (!_create) return NULL;                                             \
        return _create();                                                      \
    }                                                                          \
                                                                               \
protected:

#define GDCINULL(CNAME) CNAME* (*CNAME::_create)() = NULL;

#define GDCIIMPL(IMPNAME, CNAME)                                               \
                                                                               \
public:                                                                        \
    static CNAME* _create() {                                                  \
        return memnew(IMPNAME);                                                \
    }                                                                          \
    static void make_default() {                                               \
        CNAME::_create = IMPNAME::_create;                                     \
    }                                                                          \
                                                                               \
protected:

#endif // WEBSOCKETMACTOS_H
