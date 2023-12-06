// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PCK_PACKER_H
#define PCK_PACKER_H

#include "core/reference.h"

class FileAccess;

class PCKPacker : public Reference {
    GDCLASS(PCKPacker, Reference);

    FileAccess* file;
    int alignment;

    static void _bind_methods();

    struct File {
        String path;
        String src_path;
        uint64_t size;
        uint64_t offset_offset;
    };

    Vector<File> files;

public:
    Error pck_start(const String& p_file, int p_alignment = 0);
    Error add_file(const String& p_file, const String& p_src);
    Error flush(bool p_verbose = false);

    PCKPacker();
    ~PCKPacker();
};

#endif // PCK_PACKER_H
