// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_FILE_SERVER_H
#define EDITOR_FILE_SERVER_H

#include "core/io/file_access_network.h"
#include "core/io/packet_peer.h"
#include "core/io/tcp_server.h"
#include "core/object.h"
#include "core/os/thread.h"

class EditorFileServer : public Object {
    GDCLASS(EditorFileServer, Object);

    enum Command {
        CMD_NONE,
        CMD_ACTIVATE,
        CMD_STOP,
    };

    struct ClientData {
        Thread* thread;
        Ref<StreamPeerTCP> connection;
        Map<int, FileAccess*> files;
        EditorFileServer* efs;
        bool quit;
    };

    Ref<TCP_Server> server;
    Set<Thread*> to_wait;

    static void _close_client(ClientData* cd);
    static void _subthread_start(void* s);

    Mutex wait_mutex;
    Thread thread;
    static void _thread_start(void*);
    bool quit;
    Command cmd;

    String password;
    int port;
    bool active;

public:
    void start();
    void stop();

    bool is_active() const;

    EditorFileServer();
    ~EditorFileServer();
};

#endif // EDITOR_FILE_SERVER_H
