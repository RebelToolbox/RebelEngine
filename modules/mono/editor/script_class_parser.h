// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SCRIPT_CLASS_PARSER_H
#define SCRIPT_CLASS_PARSER_H

#include "core/ustring.h"
#include "core/variant.h"
#include "core/vector.h"

class ScriptClassParser {
public:
    struct NameDecl {
        enum Type {
            NAMESPACE_DECL,
            CLASS_DECL,
            STRUCT_DECL
        };

        String name;
        Type type;
    };

    struct ClassDecl {
        String name;
        String namespace_;
        Vector<String> base;
        bool nested;
        bool has_script_attr;
    };

private:
    String code;
    int idx;
    int line;
    String error_str;
    bool error;
    Variant value;

    Vector<ClassDecl> classes;

    enum Token {
        TK_BRACKET_OPEN,
        TK_BRACKET_CLOSE,
        TK_CURLY_BRACKET_OPEN,
        TK_CURLY_BRACKET_CLOSE,
        TK_PERIOD,
        TK_COLON,
        TK_COMMA,
        TK_SYMBOL,
        TK_IDENTIFIER,
        TK_STRING,
        TK_NUMBER,
        TK_OP_LESS,
        TK_OP_GREATER,
        TK_EOF,
        TK_ERROR,
        TK_MAX
    };

    static const char* token_names[TK_MAX];
    static String get_token_name(Token p_token);

    Token get_token();

    Error _skip_generic_type_params();

    Error _parse_type_full_name(String& r_full_name);
    Error _parse_class_base(Vector<String>& r_base);
    Error _parse_type_constraints();
    Error _parse_namespace_name(String& r_name, int& r_curly_stack);

public:
    Error parse(const String& p_code);
    Error parse_file(const String& p_filepath);

    String get_error();

    Vector<ClassDecl> get_classes();
};

#endif // SCRIPT_CLASS_PARSER_H
