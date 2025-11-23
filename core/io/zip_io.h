// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ZIP_IO_H
#define ZIP_IO_H

#include "core/os/file_access.h"

// Not directly used in this header, but assumed available in downstream users
// like platforms/*/export/export.cpp. Could be fixed, but probably better to
// have third-party includes in as little headers as possible.
#include "third-party/minizip/unzip.h"
#include "third-party/minizip/zip.h"

void* zipio_open(void* data, const char* p_fname, int mode);
uLong zipio_read(void* data, void* fdata, void* buf, uLong size);
uLong zipio_write(voidpf opaque, voidpf stream, const void* buf, uLong size);

long zipio_tell(voidpf opaque, voidpf stream);
long zipio_seek(voidpf opaque, voidpf stream, uLong offset, int origin);

int zipio_close(voidpf opaque, voidpf stream);

int zipio_testerror(voidpf opaque, voidpf stream);

voidpf zipio_alloc(voidpf opaque, uInt items, uInt size);
void zipio_free(voidpf opaque, voidpf address);

zlib_filefunc_def zipio_create_io_from_file(FileAccess** p_file);

#endif // ZIP_IO_H
