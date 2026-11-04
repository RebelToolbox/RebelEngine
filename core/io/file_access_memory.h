// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FILE_ACCESS_MEMORY_H
#define FILE_ACCESS_MEMORY_H

#include "core/os/file_access.h"

class FileAccessMemory : public FileAccess {
    uint8_t* data;
    uint64_t length;
    mutable uint64_t pos;

    static FileAccess* create();

public:
    static void register_file(String p_name, Vector<uint8_t> p_data);
    static void cleanup();

    virtual Error open_custom(
        const uint8_t* p_data,
        uint64_t p_len
    ); ///< open a file
    Error _open(const String& p_path, int p_mode_flags) override;
    void close() override;
    bool is_open() const override;

    void seek(uint64_t p_position) override;
    void seek_end(int64_t p_position) override;
    uint64_t get_position() const override;
    uint64_t get_len() const override;

    bool eof_reached() const override;

    uint8_t get_8() const override;

    uint64_t get_buffer(uint8_t* p_dst, uint64_t p_length) const override;

    Error get_error() const override;

    void flush() override;
    void store_8(uint8_t p_byte) override;
    void store_buffer(const uint8_t* p_src, uint64_t p_length) override;

    bool file_exists(const String& p_name) override;

    uint64_t _get_modified_time(const String& p_file) override {
        return 0;
    }

    uint32_t _get_unix_permissions(const String& p_file) override {
        return 0;
    }

    Error _set_unix_permissions(
        const String& p_file,
        uint32_t p_permissions
    ) override {
        return FAILED;
    }

    FileAccessMemory();
};

#endif // FILE_ACCESS_MEMORY_H
