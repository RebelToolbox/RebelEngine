// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JSON_H
#define JSON_H

#include "core/variant.h"

class JSON {
    enum TokenType {
        TK_CURLY_BRACKET_OPEN,
        TK_CURLY_BRACKET_CLOSE,
        TK_BRACKET_OPEN,
        TK_BRACKET_CLOSE,
        TK_IDENTIFIER,
        TK_STRING,
        TK_NUMBER,
        TK_COLON,
        TK_COMMA,
        TK_EOF,
        TK_MAX
    };

    enum Expecting {
        EXPECT_OBJECT,
        EXPECT_OBJECT_KEY,
        EXPECT_COLON,
        EXPECT_OBJECT_VALUE,
    };

    struct Token {
        TokenType type;
        Variant value;
    };

    static const char* tk_name[TK_MAX];

    static String _print_var(
        const Variant& p_var,
        const String& p_indent,
        int p_cur_indent,
        bool p_sort_keys,
        Set<const void*>& p_markers
    );

    static Error _get_token(
        const CharType* p_str,
        int& index,
        int p_len,
        Token& r_token,
        int& line,
        String& r_err_str
    );
    static Error _parse_value(
        Variant& value,
        Token& token,
        const CharType* p_str,
        int& index,
        int p_len,
        int& line,
        String& r_err_str
    );
    static Error _parse_array(
        Array& array,
        const CharType* p_str,
        int& index,
        int p_len,
        int& line,
        String& r_err_str
    );
    static Error _parse_object(
        Dictionary& object,
        const CharType* p_str,
        int& index,
        int p_len,
        int& line,
        String& r_err_str
    );

public:
    static String print(
        const Variant& p_var,
        const String& p_indent = "",
        bool p_sort_keys       = true
    );
    static Error parse(
        const String& p_json,
        Variant& r_ret,
        String& r_err_str,
        int& r_err_line
    );
};

#endif // JSON_H
