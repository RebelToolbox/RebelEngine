// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDSCRIPT_PROTOCAL_SERVER_H
#define GDSCRIPT_PROTOCAL_SERVER_H

#include "core/io/stream_peer.h"
#include "core/io/stream_peer_tcp.h"
#include "core/io/tcp_server.h"
#include "gdscript_text_document.h"
#include "gdscript_workspace.h"
#include "lsp.hpp"
#include "modules/jsonrpc/jsonrpc.h"

#define LSP_MAX_BUFFER_SIZE 4194304
#define LSP_MAX_CLIENTS     8

class GDScriptLanguageProtocol : public JSONRPC {
    GDCLASS(GDScriptLanguageProtocol, JSONRPC)

private:
    struct LSPeer : Reference {
        Ref<StreamPeerTCP> connection;

        uint8_t req_buf[LSP_MAX_BUFFER_SIZE];
        int req_pos        = 0;
        bool has_header    = false;
        bool has_content   = false;
        int content_length = 0;
        Vector<CharString> res_queue;
        int res_sent = 0;

        Error handle_data();
        Error send_data();
    };

    enum LSPErrorCode {
        RequestCancelled = -32800,
        ContentModified  = -32801,
    };

    static GDScriptLanguageProtocol* singleton;

    HashMap<int, Ref<LSPeer>> clients;
    Ref<TCP_Server> server;
    int latest_client_id = 0;
    int next_client_id   = 0;
    int next_server_id   = 0;

    Ref<GDScriptTextDocument> text_document;
    Ref<GDScriptWorkspace> workspace;

    Error on_client_connected();
    void on_client_disconnected(const int& p_client_id);

    String process_message(const String& p_text);
    String format_output(const String& p_text);

    bool _initialized = false;

protected:
    static void _bind_methods();

    Dictionary initialize(const Dictionary& p_params);
    void initialized(const Variant& p_params);

public:
    _FORCE_INLINE_ static GDScriptLanguageProtocol* get_singleton() {
        return singleton;
    }

    _FORCE_INLINE_ Ref<GDScriptWorkspace> get_workspace() {
        return workspace;
    }

    _FORCE_INLINE_ Ref<GDScriptTextDocument> get_text_document() {
        return text_document;
    }

    _FORCE_INLINE_ bool is_initialized() const {
        return _initialized;
    }

    void poll();
    Error start(int p_port, const IP_Address& p_bind_ip);
    void stop();

    void notify_client(
        const String& p_method,
        const Variant& p_params = Variant(),
        int p_client_id         = -1
    );
    void request_client(
        const String& p_method,
        const Variant& p_params = Variant(),
        int p_client_id         = -1
    );

    bool is_smart_resolve_enabled() const;
    bool is_goto_native_symbols_enabled() const;

    GDScriptLanguageProtocol();
};

#endif
