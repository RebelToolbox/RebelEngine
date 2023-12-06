// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "text_file.h"

#include "core/os/file_access.h"

bool TextFile::has_text() const {
    return text != "";
}

String TextFile::get_text() const {
    return text;
}

void TextFile::set_text(const String& p_code) {
    text = p_code;
}

void TextFile::reload_from_file() {
    load_text(path);
}

Error TextFile::load_text(const String& p_path) {
    PoolVector<uint8_t> sourcef;
    Error err;
    FileAccess* f = FileAccess::open(p_path, FileAccess::READ, &err);

    ERR_FAIL_COND_V_MSG(err, err, "Cannot open TextFile '" + p_path + "'.");

    uint64_t len = f->get_len();
    sourcef.resize(len + 1);
    PoolVector<uint8_t>::Write w = sourcef.write();
    uint64_t r                   = f->get_buffer(w.ptr(), len);
    f->close();
    memdelete(f);
    ERR_FAIL_COND_V(r != len, ERR_CANT_OPEN);
    w[len] = 0;

    String s;
    ERR_FAIL_COND_V_MSG(s.parse_utf8((const char *)w.ptr()), ERR_INVALID_DATA, "Script '" + p_path + "' contains invalid unicode (UTF-8), so it was not loaded. Please ensure that scripts are saved in valid UTF-8 unicode.");
    text = s;
    path = p_path;
    return OK;
}
