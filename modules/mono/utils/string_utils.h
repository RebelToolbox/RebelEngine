// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STRING_FORMAT_H
#define STRING_FORMAT_H

#include "core/ustring.h"
#include "core/variant.h"

#include <stdarg.h>

String sformat(
    const String& p_text,
    const Variant& p1 = Variant(),
    const Variant& p2 = Variant(),
    const Variant& p3 = Variant(),
    const Variant& p4 = Variant(),
    const Variant& p5 = Variant()
);

#ifdef TOOLS_ENABLED
bool is_csharp_keyword(const String& p_name);

String escape_csharp_keyword(const String& p_name);
#endif

Error read_all_file_utf8(const String& p_path, String& r_content);

#if defined(__GNUC__)
#define _PRINTF_FORMAT_ATTRIBUTE_1_0 __attribute__((format(printf, 1, 0)))
#define _PRINTF_FORMAT_ATTRIBUTE_1_2 __attribute__((format(printf, 1, 2)))
#else
#define _PRINTF_FORMAT_ATTRIBUTE_1_0
#define _PRINTF_FORMAT_ATTRIBUTE_1_2
#endif

String str_format(const char* p_format, ...) _PRINTF_FORMAT_ATTRIBUTE_1_2;
String str_format(const char* p_format, va_list p_list)
    _PRINTF_FORMAT_ATTRIBUTE_1_0;
char* str_format_new(const char* p_format, ...) _PRINTF_FORMAT_ATTRIBUTE_1_2;
char* str_format_new(const char* p_format, va_list p_list)
    _PRINTF_FORMAT_ATTRIBUTE_1_0;

#endif // STRING_FORMAT_H
