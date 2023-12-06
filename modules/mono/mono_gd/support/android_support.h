// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_SUPPORT_H
#define ANDROID_SUPPORT_H

#if defined(ANDROID_ENABLED)

#include "core/ustring.h"

namespace gdmono
{
namespace android
{
namespace support
{

String get_app_native_lib_dir();

void initialize();
void cleanup();

void register_internal_calls();

} // namespace support
} // namespace android
} // namespace gdmono

#endif // ANDROID_ENABLED

#endif // ANDROID_SUPPORT_H
