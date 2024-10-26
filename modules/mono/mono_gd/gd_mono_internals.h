// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GD_MONO_INTERNALS_H
#define GD_MONO_INTERNALS_H

#include "../utils/macros.h"
#include "core/object.h"

#include <mono/jit/jit.h>

namespace GDMonoInternals {
void tie_managed_to_unmanaged(MonoObject* managed, Object* unmanaged);

/**
 * Do not call this function directly.
 * Use GDMonoUtils::debug_unhandled_exception(MonoException *) instead.
 */
void unhandled_exception(MonoException* p_exc);

void gd_unhandled_exception_event(MonoException* p_exc);
} // namespace GDMonoInternals

#endif // GD_MONO_INTERNALS_H
