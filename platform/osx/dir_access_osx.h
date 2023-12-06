// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef DIR_ACCESS_OSX_H
#define DIR_ACCESS_OSX_H

#if defined(UNIX_ENABLED) || defined(LIBC_FILEIO_ENABLED)

#include "core/os/dir_access.h"
#include "drivers/unix/dir_access_unix.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class DirAccessOSX : public DirAccessUnix {
protected:
    virtual String fix_unicode_name(const char* p_name) const;

    virtual int get_drive_count();
    virtual String get_drive(int p_drive);

    virtual bool is_hidden(const String& p_name);
};

#endif // UNIX ENABLED
#endif
