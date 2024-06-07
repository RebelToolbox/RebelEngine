// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "macos_dir_access.h"

#if defined(UNIX_ENABLED) || defined(LIBC_FILEIO_ENABLED)

#include <AppKit/NSWorkspace.h>
#include <Foundation/Foundation.h>
#include <errno.h>

String MacOSDirAccess::fix_unicode_name(const char* p_name) const {
    String fname;
    NSString* nsstr = [[NSString stringWithUTF8String:p_name]
        precomposedStringWithCanonicalMapping];

    fname.parse_utf8([nsstr UTF8String]);

    return fname;
}

int MacOSDirAccess::get_drive_count() {
    NSArray* res_keys = [NSArray
        arrayWithObjects:NSURLVolumeURLKey, NSURLIsSystemImmutableKey, nil];
    NSArray* vols     = [[NSFileManager defaultManager]
        mountedVolumeURLsIncludingResourceValuesForKeys:res_keys
                                                options:
                                                    NSVolumeEnumerationSkipHiddenVolumes];

    return [vols count];
}

String MacOSDirAccess::get_drive(int p_drive) {
    NSArray* res_keys = [NSArray
        arrayWithObjects:NSURLVolumeURLKey, NSURLIsSystemImmutableKey, nil];
    NSArray* vols     = [[NSFileManager defaultManager]
        mountedVolumeURLsIncludingResourceValuesForKeys:res_keys
                                                options:
                                                    NSVolumeEnumerationSkipHiddenVolumes];
    int count         = [vols count];

    ERR_FAIL_INDEX_V(p_drive, count, "");

    String volname;
    NSString* path = [vols[p_drive] path];

    volname.parse_utf8([path UTF8String]);

    return volname;
}

bool MacOSDirAccess::is_hidden(const String& p_name) {
    String f         = get_current_dir().plus_file(p_name);
    NSURL* url       = [NSURL fileURLWithPath:@(f.utf8().get_data())];
    NSNumber* hidden = nil;
    if (![url getResourceValue:&hidden forKey:NSURLIsHiddenKey error:nil]) {
        return DirAccessUnix::is_hidden(p_name);
    }
    return [hidden boolValue];
}

#endif // posix_enabled
