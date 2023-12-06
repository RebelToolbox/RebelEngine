// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MATH_FIELDWISE_H
#define MATH_FIELDWISE_H

#ifdef TOOLS_ENABLED

#include "core/variant.h"

Variant fieldwise_assign(
    const Variant& p_target,
    const Variant& p_source,
    const String& p_field
);

#endif // TOOLS_ENABLED

#endif // MATH_FIELDWISE_H
