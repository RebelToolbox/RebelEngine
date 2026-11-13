// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FILE_ACCESS_COMPRESSED_H
#define FILE_ACCESS_COMPRESSED_H

#include "core/io/compression.h"
#include "core/os/file_access.h"

class FileAccessCompressed : public FileAccess {
    Compression::Mode cmode;
    bool writing;
    uint64_t write_pos;
    uint8_t* write_ptr;
    uint32_t write_buffer_size;
    uint64_t write_max;
    uint32_t block_size;
    mutable bool read_eof;
    mutable bool at_end;

    struct ReadBlock {
        uint32_t csize;
        uint64_t offset;
    };

    mutable Vector<uint8_t> comp_buffer;
    uint8_t* read_ptr;
    mutable uint32_t read_block;
    uint32_t read_block_count;
    mutable uint32_t read_block_size;
    mutable uint64_t read_pos;
    Vector<ReadBlock> read_blocks;
    uint64_t read_total;

    String magic;
    mutable Vector<uint8_t> buffer;
    FileAccess* f;

public:
    void configure(
        const String& p_magic,
        Compression::Mode p_mode = Compression::MODE_ZSTD,
        uint32_t p_block_size    = 4096
    );

    Error open_after_magic(FileAccess* p_base);

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

    bool file_exists(const String& p_name) override;

    uint64_t _get_modified_time(const String& p_file) override;
    uint32_t _get_unix_permissions(const String& p_file) override;
    Error _set_unix_permissions(
        const String& p_file,
        uint32_t p_permissions
    ) override;

    FileAccessCompressed();
    ~FileAccessCompressed() override;
};

#endif // FILE_ACCESS_COMPRESSED_H
