// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "core/ustring.h"

#ifndef MACOS_UTILS_H
#define MACOS_UTILS_H

#ifdef MACOS_ENABLED

bool macos_is_app_bundle_installed(const String& p_bundle_id);

#endif

#endif // MACOS_UTILS_H
