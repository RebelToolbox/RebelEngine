// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef DIR_ACCESS_WINDOWS_H
#define DIR_ACCESS_WINDOWS_H

#ifdef WINDOWS_ENABLED

#include "core/os/dir_access.h"

/**
    @author Juan Linietsky <reduz@gmail.com>
*/

struct DirAccessWindowsPrivate;

class DirAccessWindows : public DirAccess {
    enum {
        MAX_DRIVES = 26
    };

    DirAccessWindowsPrivate* p;
    /* Windows stuff */

    char drives[MAX_DRIVES]; // a-z:
    int drive_count;

    String current_dir;

    bool _cisdir;
    bool _cishidden;

public:
    virtual Error list_dir_begin(); ///< This starts dir listing
    virtual String get_next();
    virtual bool current_is_dir() const;
    virtual bool current_is_hidden() const;
    virtual void list_dir_end(); ///<

    virtual int get_drive_count();
    virtual String get_drive(int p_drive);

    virtual Error change_dir(String p_dir
    ); ///< can be relative or absolute, return false on success
    virtual String get_current_dir(); ///< return current dir location
    virtual String get_current_dir_without_drive();

    virtual bool file_exists(String p_file);
    virtual bool dir_exists(String p_dir);

    virtual Error make_dir(String p_dir);

    virtual Error rename(String p_path, String p_new_path);
    virtual Error remove(String p_path);

    virtual bool is_link(String p_file) {
        return false;
    };

    virtual String read_link(String p_file) {
        return p_file;
    };

    virtual Error create_link(String p_source, String p_target) {
        return FAILED;
    };

    uint64_t get_space_left();

    virtual String get_filesystem_type() const;

    DirAccessWindows();
    ~DirAccessWindows();
};

#endif // WINDOWS_ENABLED

#endif
