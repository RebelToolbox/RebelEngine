// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include "core/ustring.h"
#include "core/vector.h"

class StringBuilder {
    uint32_t string_length;

    Vector<String> strings;
    Vector<const char*> c_strings;

    // -1 means it's a Rebel String
    // a natural number means C string.
    Vector<int32_t> appended_strings;

public:
    StringBuilder& append(const String& p_string);
    StringBuilder& append(const char* p_cstring);

    _FORCE_INLINE_ StringBuilder& operator+(const String& p_string) {
        return append(p_string);
    }

    _FORCE_INLINE_ StringBuilder& operator+(const char* p_cstring) {
        return append(p_cstring);
    }

    _FORCE_INLINE_ void operator+=(const String& p_string) {
        append(p_string);
    }

    _FORCE_INLINE_ void operator+=(const char* p_cstring) {
        append(p_cstring);
    }

    _FORCE_INLINE_ int num_strings_appended() const {
        return appended_strings.size();
    }

    _FORCE_INLINE_ uint32_t get_string_length() const {
        return string_length;
    }

    String as_string() const;

    _FORCE_INLINE_ operator String() const {
        return as_string();
    }

    StringBuilder() {
        string_length = 0;
    }
};

#endif // STRING_BUILDER_H
