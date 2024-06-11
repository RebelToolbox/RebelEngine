// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_file_access.h"

#include "core/print_string.h"

AAssetManager* AndroidFileAccess::asset_manager = NULL;

/*void AndroidFileAccess::make_default() {

    create_func=create_android;
}*/

FileAccess* AndroidFileAccess::create_android() {
    return memnew(AndroidFileAccess);
}

Error AndroidFileAccess::_open(const String& p_path, int p_mode_flags) {
    String path = fix_path(p_path).simplify_path();
    if (path.begins_with("/")) {
        path = path.substr(1, path.length());
    } else if (path.begins_with("res://")) {
        path = path.substr(6, path.length());
    }

    ERR_FAIL_COND_V(
        p_mode_flags & FileAccess::WRITE,
        ERR_UNAVAILABLE
    ); // can't write on android..
    a = AAssetManager_open(
        asset_manager,
        path.utf8().get_data(),
        AASSET_MODE_STREAMING
    );
    if (!a) {
        return ERR_CANT_OPEN;
    }
    // ERR_FAIL_COND_V(!a,ERR_FILE_NOT_FOUND);
    len = AAsset_getLength(a);
    pos = 0;
    eof = false;

    return OK;
}

void AndroidFileAccess::close() {
    if (!a) {
        return;
    }
    AAsset_close(a);
    a = NULL;
}

bool AndroidFileAccess::is_open() const {
    return a != NULL;
}

void AndroidFileAccess::seek(uint64_t p_position) {
    ERR_FAIL_COND(!a);

    AAsset_seek(a, p_position, SEEK_SET);
    pos = p_position;
    if (pos > len) {
        pos = len;
        eof = true;
    } else {
        eof = false;
    }
}

void AndroidFileAccess::seek_end(int64_t p_position) {
    ERR_FAIL_COND(!a);
    AAsset_seek(a, p_position, SEEK_END);
    pos = len + p_position;
}

uint64_t AndroidFileAccess::get_position() const {
    return pos;
}

uint64_t AndroidFileAccess::get_len() const {
    return len;
}

bool AndroidFileAccess::eof_reached() const {
    return eof;
}

uint8_t AndroidFileAccess::get_8() const {
    if (pos >= len) {
        eof = true;
        return 0;
    }

    uint8_t byte;
    AAsset_read(a, &byte, 1);
    pos++;
    return byte;
}

uint64_t AndroidFileAccess::get_buffer(uint8_t* p_dst, uint64_t p_length)
    const {
    ERR_FAIL_COND_V(!p_dst && p_length > 0, -1);

    int r = AAsset_read(a, p_dst, p_length);

    if (pos + p_length > len) {
        eof = true;
    }

    if (r >= 0) {
        pos += r;
        if (pos > len) {
            pos = len;
        }
    }
    return r;
}

Error AndroidFileAccess::get_error() const {
    return eof ? ERR_FILE_EOF : OK; // not sure what else it may happen
}

void AndroidFileAccess::flush() {
    ERR_FAIL();
}

void AndroidFileAccess::store_8(uint8_t p_dest) {
    ERR_FAIL();
}

bool AndroidFileAccess::file_exists(const String& p_path) {
    String path = fix_path(p_path).simplify_path();
    if (path.begins_with("/")) {
        path = path.substr(1, path.length());
    } else if (path.begins_with("res://")) {
        path = path.substr(6, path.length());
    }

    AAsset* at = AAssetManager_open(
        asset_manager,
        path.utf8().get_data(),
        AASSET_MODE_STREAMING
    );

    if (!at) {
        return false;
    }

    AAsset_close(at);
    return true;
}

AndroidFileAccess::AndroidFileAccess() {
    a   = NULL;
    eof = false;
}

AndroidFileAccess::~AndroidFileAccess() {
    close();
}
