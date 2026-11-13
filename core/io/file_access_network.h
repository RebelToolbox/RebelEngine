// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FILE_ACCESS_NETWORK_H
#define FILE_ACCESS_NETWORK_H

#include "core/io/stream_peer_tcp.h"
#include "core/os/file_access.h"
#include "core/os/semaphore.h"
#include "core/os/thread.h"

class FileAccessNetwork;

class FileAccessNetworkClient {
    struct BlockRequest {
        int32_t id;
        uint64_t offset;
        int32_t size;
    };

    List<BlockRequest> block_requests;

    Semaphore sem;
    Thread thread;
    bool quit;
    Mutex mutex;
    Mutex blockrequest_mutex;
    Map<int, FileAccessNetwork*> accesses;
    Ref<StreamPeerTCP> client;
    int32_t last_id;

    Vector<uint8_t> block;

    void _thread_func();
    static void _thread_func(void* s);

    void put_32(int32_t p_32);
    void put_64(int64_t p_64);
    int32_t get_32();
    int64_t get_64();
    int lockcount;
    void lock_mutex();
    void unlock_mutex();

    friend class FileAccessNetwork;
    static FileAccessNetworkClient* singleton;

public:
    static FileAccessNetworkClient* get_singleton() {
        return singleton;
    }

    Error connect(
        const String& p_host,
        int p_port,
        const String& p_password = ""
    );

    FileAccessNetworkClient();
    ~FileAccessNetworkClient();
};

class FileAccessNetwork : public FileAccess {
    Semaphore sem;
    Semaphore page_sem;
    Mutex buffer_mutex;
    bool opened;
    uint64_t total_size;
    mutable uint64_t pos;
    int32_t id;
    mutable bool eof_flag;
    mutable int32_t last_page;
    mutable uint8_t* last_page_buff;

    int32_t page_size;
    int32_t read_ahead;

    mutable int waiting_on_page;

    struct Page {
        int activity;
        bool queued;
        Vector<uint8_t> buffer;

        Page() {
            activity = 0;
            queued   = false;
        }
    };

    mutable Vector<Page> pages;

    mutable Error response;

    uint64_t exists_modtime;
    friend class FileAccessNetworkClient;
    void _queue_page(int32_t p_page) const;
    void _respond(uint64_t p_len, Error p_status);
    void _set_block(uint64_t p_offset, const Vector<uint8_t>& p_block);

public:
    enum Command {
        COMMAND_OPEN_FILE,
        COMMAND_READ_BLOCK,
        COMMAND_CLOSE,
        COMMAND_FILE_EXISTS,
        COMMAND_GET_MODTIME,
    };

    enum Response {
        RESPONSE_OPEN,
        RESPONSE_DATA,
        RESPONSE_FILE_EXISTS,
        RESPONSE_GET_MODTIME,
    };

    Error _open(const String& p_path, int p_mode_flags) override;
    void close() override;
    bool is_open() const override;

    void seek(uint64_t p_position) override;
    void seek_end(int64_t p_position = 0) override;
    uint64_t get_position() const override;
    uint64_t get_len() const override;

    bool eof_reached() const override;

    uint8_t get_8() const override;
    uint64_t get_buffer(uint8_t* p_dst, uint64_t p_length) const override;

    Error get_error() const override;

    void flush() override;
    void store_8(uint8_t p_dest) override;

    bool file_exists(const String& p_path) override;

    uint64_t _get_modified_time(const String& p_file) override;
    uint32_t _get_unix_permissions(const String& p_file) override;
    Error _set_unix_permissions(
        const String& p_file,
        uint32_t p_permissions
    ) override;

    static void configure();

    FileAccessNetwork();
    ~FileAccessNetwork() override;
};

#endif // FILE_ACCESS_NETWORK_H
