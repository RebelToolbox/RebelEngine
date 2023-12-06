// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "osx_utils.h"

#ifdef OSX_ENABLED

#include "core/print_string.h"

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>

bool osx_is_app_bundle_installed(const String& p_bundle_id) {
    CFStringRef bundle_id = CFStringCreateWithCString(
        nullptr,
        p_bundle_id.utf8(),
        kCFStringEncodingUTF8
    );
    CFArrayRef result =
        LSCopyApplicationURLsForBundleIdentifier(bundle_id, nullptr);
    CFRelease(bundle_id);

    if (result) {
        if (CFArrayGetCount(result) > 0) {
            CFRelease(result);
            return true;
        } else {
            CFRelease(result);
            return false;
        }
    } else {
        return false;
    }
}

#endif
