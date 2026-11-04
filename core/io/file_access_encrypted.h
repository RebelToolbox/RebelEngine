// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FILE_ACCESS_ENCRYPTED_H
#define FILE_ACCESS_ENCRYPTED_H

#include "core/os/file_access.h"

class FileAccessEncrypted : public FileAccess {
public:
    enum Mode {
        MODE_READ,
        MODE_WRITE_AES256,
        MODE_MAX
    };

private:
    Mode mode;
    Vector<uint8_t> key;
    bool writing;
    FileAccess* file;
    uint64_t base;
    uint64_t length;
    Vector<uint8_t> data;
    mutable uint64_t pos;
    mutable bool eofed;

public:
    Error open_and_parse(
        FileAccess* p_base,
        const Vector<uint8_t>& p_key,
        Mode p_mode
    );
    Error open_and_parse_password(
        FileAccess* p_base,
        const String& p_key,
        Mode p_mode
    );

    Error _open(const String& p_path, int p_mode_flags) override;
    void close() override;
    bool is_open() const override;

    String get_path() const override;
    String get_path_absolute() const override;

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
    void store_buffer(const uint8_t* p_src, uint64_t p_length) override;

    bool file_exists(const String& p_name) override;

    uint64_t _get_modified_time(const String& p_file) override;
    uint32_t _get_unix_permissions(const String& p_file) override;
    Error _set_unix_permissions(
        const String& p_file,
        uint32_t p_permissions
    ) override;

    FileAccessEncrypted();
    ~FileAccessEncrypted() override;
};

#endif // FILE_ACCESS_ENCRYPTED_H
