// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_FILE_ACCESS_H
#define ANDROID_FILE_ACCESS_H

#include "core/os/file_access.h"

#include <android/asset_manager.h>
#include <stdio.h>

// #include <android_native_app_glue.h>

class AndroidFileAccess : public FileAccess {
    static FileAccess* create_android();
    mutable AAsset* a;
    mutable uint64_t len;
    mutable uint64_t pos;
    mutable bool eof;

public:
    static AAssetManager* asset_manager;

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

    // static void make_default();

    AndroidFileAccess();
    ~AndroidFileAccess() override;
};

#endif // ANDROID_FILE_ACCESS_H
