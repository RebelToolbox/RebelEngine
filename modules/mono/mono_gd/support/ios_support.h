// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IOS_SUPPORT_H
#define IOS_SUPPORT_H

#if defined(IOS_ENABLED)

#include "core/ustring.h"

namespace gdmono
{
namespace ios
{
namespace support
{

void initialize();
void cleanup();

} // namespace support
} // namespace ios
} // namespace gdmono

#endif // IOS_ENABLED

#endif // IOS_SUPPORT_H
