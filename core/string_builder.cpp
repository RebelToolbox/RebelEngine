// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "string_builder.h"

#include <string.h>

StringBuilder& StringBuilder::append(const String& p_string) {
    if (p_string == String()) {
        return *this;
    }

    strings.push_back(p_string);
    appended_strings.push_back(-1);

    string_length += p_string.length();

    return *this;
}

StringBuilder& StringBuilder::append(const char* p_cstring) {
    int32_t len = strlen(p_cstring);

    c_strings.push_back(p_cstring);
    appended_strings.push_back(len);

    string_length += len;

    return *this;
}

String StringBuilder::as_string() const {
    if (string_length == 0) {
        return "";
    }

    CharType* buffer = memnew_arr(CharType, string_length);

    int current_position = 0;
    int string_elem      = 0;
    int c_string_elem    = 0;

    for (int i = 0; i < appended_strings.size(); i++) {
        if (appended_strings[i] == -1) {
            const String& s = strings[string_elem];

            memcpy(
                buffer + current_position,
                s.ptr(),
                s.length() * sizeof(CharType)
            );

            current_position += s.length();

            string_elem++;
        } else {
            const char* s = c_strings[c_string_elem];

            for (int32_t j = 0; j < appended_strings[i]; j++) {
                buffer[current_position + j] = s[j];
            }

            current_position += appended_strings[i];

            c_string_elem++;
        }
    }

    String final_string = String(buffer, string_length);

    memdelete_arr(buffer);

    return final_string;
}
