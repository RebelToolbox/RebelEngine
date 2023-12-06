// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GD_MONO_HEADER_H
#define GD_MONO_HEADER_H

#include "core/int_types.h"

#ifdef WIN32
#define GD_MONO_STDCALL __stdcall
#else
#define GD_MONO_STDCALL
#endif

class GDMonoAssembly;
class GDMonoClass;
class GDMonoField;
class GDMonoMethod;
class GDMonoProperty;

class IMonoClassMember;

#include "managed_type.h"

#endif // GD_MONO_HEADER_H
