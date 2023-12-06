// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "core/ustring.h"

#ifndef OSX_UTILS_H
#define OSX_UTILS_H

#ifdef OSX_ENABLED

bool osx_is_app_bundle_installed(const String& p_bundle_id);

#endif

#endif // OSX_UTILS_H
