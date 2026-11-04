// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FILE_ACCESS_ZIP_H
#define FILE_ACCESS_ZIP_H

#ifdef MINIZIP_ENABLED

#include "core/io/file_access_pack.h"
#include "core/map.h"
#include "third-party/minizip/unzip.h"

#include <stdlib.h>

class ZipArchive : public PackSource {
public:
    struct File {
        int package;
        unz_file_pos file_pos;

        File() {
            package = -1;
        };
    };

private:
    struct Package {
        String filename;
        unzFile zfile;
    };

    Vector<Package> packages;

    Map<String, File> files;

    static ZipArchive* instance;

    FileAccess::CreateFunc fa_create_func;

public:
    void close_handle(unzFile p_file) const;
    unzFile get_file_handle(String p_file) const;

    Error add_package(String p_name);

    bool file_exists(String p_name) const;

    bool try_open_pack(
        const String& p_path,
        bool p_replace_files,
        uint64_t p_offset
    ) override;
    FileAccess* get_file(
        const String& p_path,
        PackedData::PackedFile* p_file
    ) override;

    static ZipArchive* get_singleton();

    ZipArchive();
    ~ZipArchive() override;
};

class FileAccessZip : public FileAccess {
    unzFile zfile = nullptr;
    unz_file_info64 file_info;

    mutable bool at_eof;

public:
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

    uint64_t _get_modified_time(const String& p_file) override {
        return 0;
    } // todo

    uint32_t _get_unix_permissions(const String& p_file) override {
        return 0;
    }

    Error _set_unix_permissions(
        const String& p_file,
        uint32_t p_permissions
    ) override {
        return FAILED;
    }

    FileAccessZip(const String& p_path, const PackedData::PackedFile& p_file);
    ~FileAccessZip() override;
};

#endif // MINIZIP_ENABLED

#endif // FILE_ACCESS_ZIP_H
