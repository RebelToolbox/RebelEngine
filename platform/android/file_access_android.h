// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FILE_ACCESS_ANDROID_H
#define FILE_ACCESS_ANDROID_H

#include "core/os/file_access.h"

#include <android/asset_manager.h>
#include <android/log.h>
#include <stdio.h>

// #include <android_native_app_glue.h>

class FileAccessAndroid : public FileAccess {
    static FileAccess* create_android();
    mutable AAsset* a;
    mutable uint64_t len;
    mutable uint64_t pos;
    mutable bool eof;

public:
    static AAssetManager* asset_manager;

    virtual Error _open(
        const String& p_path,
        int p_mode_flags
    );                            ///< open a file
    virtual void close();         ///< close a file
    virtual bool is_open() const; ///< true when file is open

    virtual void seek(uint64_t p_position); ///< seek to a given position
    virtual void seek_end(
        int64_t p_position = 0
    );                                     ///< seek from the end of file
    virtual uint64_t get_position() const; ///< get position in the file
    virtual uint64_t get_len() const;      ///< get size of the file

    virtual bool eof_reached() const; ///< reading passed EOF

    virtual uint8_t get_8() const; ///< get a byte
    virtual uint64_t get_buffer(uint8_t* p_dst, uint64_t p_length) const;

    virtual Error get_error() const; ///< get last error

    virtual void flush();
    virtual void store_8(uint8_t p_dest); ///< store a byte

    virtual bool file_exists(const String& p_path
    ); ///< return true if a file exists

    virtual uint64_t _get_modified_time(const String& p_file) {
        return 0;
    }

    virtual uint32_t _get_unix_permissions(const String& p_file) {
        return 0;
    }

    virtual Error _set_unix_permissions(
        const String& p_file,
        uint32_t p_permissions
    ) {
        return FAILED;
    }

    // static void make_default();

    FileAccessAndroid();
    ~FileAccessAndroid();
};

#endif // FILE_ACCESS_ANDROID_H
