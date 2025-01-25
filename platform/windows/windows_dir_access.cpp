// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "windows_dir_access.h"

#include "core/os/memory.h"
#include "core/print_string.h"

#include <stdio.h>
#include <wchar.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct WindowsPrivateDirAccess {
    HANDLE h; // handle for findfirstfile
    WIN32_FIND_DATA f;
    WIN32_FIND_DATAW fu; // unicode version
};

// CreateFolderAsync

Error WindowsDirAccess::list_dir_begin() {
    _cisdir    = false;
    _cishidden = false;

    list_dir_end();
    p->h = FindFirstFileExW(
        (current_dir + "\\*").c_str(),
        FindExInfoStandard,
        &p->fu,
        FindExSearchNameMatch,
        NULL,
        0
    );

    return (p->h == INVALID_HANDLE_VALUE) ? ERR_CANT_OPEN : OK;
}

String WindowsDirAccess::get_next() {
    if (p->h == INVALID_HANDLE_VALUE) {
        return "";
    }

    _cisdir    = (p->fu.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    _cishidden = (p->fu.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);

    String name = p->fu.cFileName;

    if (FindNextFileW(p->h, &p->fu) == 0) {
        FindClose(p->h);
        p->h = INVALID_HANDLE_VALUE;
    }

    return name;
}

bool WindowsDirAccess::current_is_dir() const {
    return _cisdir;
}

bool WindowsDirAccess::current_is_hidden() const {
    return _cishidden;
}

void WindowsDirAccess::list_dir_end() {
    if (p->h != INVALID_HANDLE_VALUE) {
        FindClose(p->h);
        p->h = INVALID_HANDLE_VALUE;
    }
}

int WindowsDirAccess::get_drive_count() {
    return drive_count;
}

String WindowsDirAccess::get_drive(int p_drive) {
    if (p_drive < 0 || p_drive >= drive_count) {
        return "";
    }

    return String::chr(drives[p_drive]) + ":";
}

Error WindowsDirAccess::change_dir(String p_dir) {
    GLOBAL_LOCK_FUNCTION

    p_dir = fix_path(p_dir);

    wchar_t real_current_dir_name[2048];
    GetCurrentDirectoryW(2048, real_current_dir_name);
    String prev_dir = real_current_dir_name;

    SetCurrentDirectoryW(current_dir.c_str());
    bool worked = (SetCurrentDirectoryW(p_dir.c_str()) != 0);

    String base = _get_root_path();
    if (base != "") {
        GetCurrentDirectoryW(2048, real_current_dir_name);
        String new_dir;
        new_dir = String(real_current_dir_name).replace("\\", "/");
        if (!new_dir.begins_with(base)) {
            worked = false;
        }
    }

    if (worked) {
        GetCurrentDirectoryW(2048, real_current_dir_name);
        current_dir = real_current_dir_name; // TODO, utf8 parser
        current_dir = current_dir.replace("\\", "/");

    } // else {

    SetCurrentDirectoryW(prev_dir.c_str());
    //}

    return worked ? OK : ERR_INVALID_PARAMETER;
}

Error WindowsDirAccess::make_dir(String p_dir) {
    GLOBAL_LOCK_FUNCTION

    p_dir = fix_path(p_dir);
    if (p_dir.is_rel_path()) {
        p_dir = current_dir.plus_file(p_dir);
    }

    p_dir = p_dir.replace("/", "\\");

    bool success;
    int err;

    p_dir = "\\\\?\\" + p_dir; // done according to
    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa363855(v=vs.85).aspx

    success = CreateDirectoryW(p_dir.c_str(), NULL);
    err     = GetLastError();

    if (success) {
        return OK;
    };

    if (err == ERROR_ALREADY_EXISTS || err == ERROR_ACCESS_DENIED) {
        return ERR_ALREADY_EXISTS;
    };

    return ERR_CANT_CREATE;
}

String WindowsDirAccess::get_current_dir() {
    String base = _get_root_path();
    if (base != "") {
        String bd = current_dir.replace("\\", "/").replace_first(base, "");
        if (bd.begins_with("/")) {
            return _get_root_string() + bd.substr(1, bd.length());
        } else {
            return _get_root_string() + bd;
        }

    } else {
    }

    return current_dir;
}

String WindowsDirAccess::get_current_dir_without_drive() {
    String dir = get_current_dir();

    if (_get_root_string() == "") {
        int p = current_dir.find(":");
        if (p != -1) {
            dir = dir.right(p + 1);
        }
    }

    return dir;
}

bool WindowsDirAccess::file_exists(String p_file) {
    GLOBAL_LOCK_FUNCTION

    if (!p_file.is_abs_path()) {
        p_file = get_current_dir().plus_file(p_file);
    }

    p_file = fix_path(p_file);

    // p_file.replace("/","\\");

    // WIN32_FILE_ATTRIBUTE_DATA    fileInfo;

    DWORD fileAttr;

    fileAttr = GetFileAttributesW(p_file.c_str());
    if (INVALID_FILE_ATTRIBUTES == fileAttr) {
        return false;
    }

    return !(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
}

bool WindowsDirAccess::dir_exists(String p_dir) {
    GLOBAL_LOCK_FUNCTION

    if (p_dir.is_rel_path()) {
        p_dir = get_current_dir().plus_file(p_dir);
    }

    p_dir = fix_path(p_dir);

    // p_dir.replace("/","\\");

    // WIN32_FILE_ATTRIBUTE_DATA    fileInfo;

    DWORD fileAttr;

    fileAttr = GetFileAttributesW(p_dir.c_str());
    if (INVALID_FILE_ATTRIBUTES == fileAttr) {
        return false;
    }
    return (fileAttr & FILE_ATTRIBUTE_DIRECTORY);
}

Error WindowsDirAccess::rename(String p_path, String p_new_path) {
    if (p_path.is_rel_path()) {
        p_path = get_current_dir().plus_file(p_path);
    }

    p_path = fix_path(p_path);

    if (p_new_path.is_rel_path()) {
        p_new_path = get_current_dir().plus_file(p_new_path);
    }

    p_new_path = fix_path(p_new_path);

    // If we're only changing file name case we need to do a little juggling
    if (p_path.to_lower() == p_new_path.to_lower()) {
        if (dir_exists(p_path)) {
            // The path is a dir; just rename
            return ::_wrename(p_path.c_str(), p_new_path.c_str()) == 0 ? OK
                                                                       : FAILED;
        }
        // The path is a file; juggle
        WCHAR tmpfile[MAX_PATH];

        if (!GetTempFileNameW(
                fix_path(get_current_dir()).c_str(),
                NULL,
                0,
                tmpfile
            )) {
            return FAILED;
        }

        if (!::ReplaceFileW(tmpfile, p_path.c_str(), NULL, 0, NULL, NULL)) {
            DeleteFileW(tmpfile);
            return FAILED;
        }

        return ::_wrename(tmpfile, p_new_path.c_str()) == 0 ? OK : FAILED;

    } else {
        if (file_exists(p_new_path)) {
            if (remove(p_new_path) != OK) {
                return FAILED;
            }
        }

        return ::_wrename(p_path.c_str(), p_new_path.c_str()) == 0 ? OK
                                                                   : FAILED;
    }
}

Error WindowsDirAccess::remove(String p_path) {
    if (p_path.is_rel_path()) {
        p_path = get_current_dir().plus_file(p_path);
    }

    p_path = fix_path(p_path);

    DWORD fileAttr;

    fileAttr = GetFileAttributesW(p_path.c_str());
    if (INVALID_FILE_ATTRIBUTES == fileAttr) {
        return FAILED;
    }
    if ((fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
        return ::_wrmdir(p_path.c_str()) == 0 ? OK : FAILED;
    } else {
        return ::_wunlink(p_path.c_str()) == 0 ? OK : FAILED;
    }
}

uint64_t WindowsDirAccess::get_space_left() {
    uint64_t bytes = 0;
    if (!GetDiskFreeSpaceEx(NULL, (PULARGE_INTEGER)&bytes, NULL, NULL)) {
        return 0;
    }

    // this is either 0 or a value in bytes.
    return bytes;
}

String WindowsDirAccess::get_filesystem_type() const {
    String path =
        fix_path(const_cast<WindowsDirAccess*>(this)->get_current_dir());

    int unit_end = path.find(":");
    ERR_FAIL_COND_V(unit_end == -1, String());
    String unit = path.substr(0, unit_end + 1) + "\\";

    WCHAR szVolumeName[100];
    WCHAR szFileSystemName[10];
    DWORD dwSerialNumber      = 0;
    DWORD dwMaxFileNameLength = 0;
    DWORD dwFileSystemFlags   = 0;

    if (::GetVolumeInformationW(
            unit.c_str(),
            szVolumeName,
            sizeof(szVolumeName),
            &dwSerialNumber,
            &dwMaxFileNameLength,
            &dwFileSystemFlags,
            szFileSystemName,
            sizeof(szFileSystemName)
        )
        == TRUE) {
        return String(szFileSystemName);
    }

    ERR_FAIL_V("");
    return String();
}

WindowsDirAccess::WindowsDirAccess() {
    p           = memnew(WindowsPrivateDirAccess);
    p->h        = INVALID_HANDLE_VALUE;
    current_dir = ".";

    drive_count = 0;

#ifdef UWP_ENABLED
    Windows::Storage::StorageFolder ^ install_folder =
        Windows::ApplicationModel::Package::Current->InstalledLocation;
    change_dir(install_folder->Path->Data());

#else

    DWORD mask = GetLogicalDrives();

    for (int i = 0; i < MAX_DRIVES; i++) {
        if (mask & (1 << i)) { // DRIVE EXISTS

            drives[drive_count] = 'A' + i;
            drive_count++;
        }
    }

    change_dir(".");
#endif
}

WindowsDirAccess::~WindowsDirAccess() {
    memdelete(p);
}
