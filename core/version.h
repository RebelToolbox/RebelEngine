// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VERSION_H
#define VERSION_H

#include "core/version_generated.gen.h"

// Rebel Engine uses Semantic Versioning. See https://semver.org
// Every major and minor version will have its own branch.
// Patch versions will be tagged on their major.minor branch.

#define VERSION_BRANCH _MKSTR(VERSION_MAJOR) "." _MKSTR(VERSION_MINOR)
#define VERSION_NUMBER VERSION_BRANCH "." _MKSTR(VERSION_PATCH)

// Version number encoded as hexadecimal int with one byte for each number.
// This makes it easy to make comparisons in code and scripts.
// Example: 1.2.3 will be 0x010203.
#define VERSION_HEX                                                            \
    0x10000 * VERSION_MAJOR + 0x100 * VERSION_MINOR + VERSION_PATCH

// If it's a pre-release, it includes the pre-release identifier after a '-'.
#ifdef VERSION_PRE_RELEASE
#define VERSION_FULL VERSION_NUMBER "-" VERSION_PRE_RELEASE
#else // ! VERSION_PRE_RELEASE
#define VERSION_FULL VERSION_NUMBER
#endif // VERSION_PRE_RELEASE

// Includes the ISO build date after a '+'.
// E.g. 1.0-dev+2025.8.22
#define VERSION_FULL_BUILD                                                     \
    VERSION_FULL "+" _MKSTR(VERSION_YEAR) "." _MKSTR(VERSION_MONTH             \
    ) "." _MKSTR(VERSION_DAY)

#define VERSION_FULL_NAME VERSION_NAME " v" VERSION_FULL

#endif // VERSION_H
