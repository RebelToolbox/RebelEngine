// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VARIANT_PARSER_H
#define VARIANT_PARSER_H

#include "core/os/file_access.h"
#include "core/resource.h"
#include "core/variant.h"

class VariantParser {
public:
    struct Stream {
        virtual CharType get_char()  = 0;
        virtual bool is_utf8() const = 0;
        virtual bool is_eof() const  = 0;

        CharType saved;

        Stream() : saved(0) {}

        virtual ~Stream() {}
    };

    struct StreamFile : public Stream {
        FileAccess* f;

        virtual CharType get_char();
        virtual bool is_utf8() const;
        virtual bool is_eof() const;

        StreamFile() {
            f = nullptr;
        }
    };

    struct StreamString : public Stream {
        String s;
        int pos;

        virtual CharType get_char();
        virtual bool is_utf8() const;
        virtual bool is_eof() const;

        StreamString() {
            pos = 0;
        }
    };

    typedef Error (*ParseResourceFunc)(
        void* p_self,
        Stream* p_stream,
        Ref<Resource>& r_res,
        int& line,
        String& r_err_str
    );

    struct ResourceParser {
        void* userdata             = nullptr;
        ParseResourceFunc func     = nullptr;
        ParseResourceFunc ext_func = nullptr;
        ParseResourceFunc sub_func = nullptr;
    };

    enum TokenType {
        TK_CURLY_BRACKET_OPEN,
        TK_CURLY_BRACKET_CLOSE,
        TK_BRACKET_OPEN,
        TK_BRACKET_CLOSE,
        TK_PARENTHESIS_OPEN,
        TK_PARENTHESIS_CLOSE,
        TK_IDENTIFIER,
        TK_STRING,
        TK_NUMBER,
        TK_COLOR,
        TK_COLON,
        TK_COMMA,
        TK_PERIOD,
        TK_EQUAL,
        TK_EOF,
        TK_ERROR,
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

    struct Tag {
        String name;
        Map<String, Variant> fields;
    };

private:
    static const char* tk_name[TK_MAX];

    template <class T>
    static Error _parse_construct(
        Stream* p_stream,
        Vector<T>& r_construct,
        int& line,
        String& r_err_str
    );
    static Error _parse_enginecfg(
        Stream* p_stream,
        Vector<String>& strings,
        int& line,
        String& r_err_str
    );
    static Error _parse_dictionary(
        Dictionary& object,
        Stream* p_stream,
        int& line,
        String& r_err_str,
        ResourceParser* p_res_parser = nullptr
    );
    static Error _parse_array(
        Array& array,
        Stream* p_stream,
        int& line,
        String& r_err_str,
        ResourceParser* p_res_parser = nullptr
    );
    static Error _parse_tag(
        Token& token,
        Stream* p_stream,
        int& line,
        String& r_err_str,
        Tag& r_tag,
        ResourceParser* p_res_parser = nullptr,
        bool p_simple_tag            = false
    );

public:
    static Error parse_tag(
        Stream* p_stream,
        int& line,
        String& r_err_str,
        Tag& r_tag,
        ResourceParser* p_res_parser = nullptr,
        bool p_simple_tag            = false
    );
    static Error parse_tag_assign_eof(
        Stream* p_stream,
        int& line,
        String& r_err_str,
        Tag& r_tag,
        String& r_assign,
        Variant& r_value,
        ResourceParser* p_res_parser = nullptr,
        bool p_simple_tag            = false
    );

    static Error parse_value(
        Token& token,
        Variant& value,
        Stream* p_stream,
        int& line,
        String& r_err_str,
        ResourceParser* p_res_parser = nullptr
    );
    static Error get_token(
        Stream* p_stream,
        Token& r_token,
        int& line,
        String& r_err_str
    );
    static Error parse(
        Stream* p_stream,
        Variant& r_ret,
        String& r_err_str,
        int& r_err_line,
        ResourceParser* p_res_parser = nullptr
    );
};

class VariantWriter {
public:
    typedef Error (*StoreStringFunc)(void* ud, const String& p_string);
    typedef String (*EncodeResourceFunc)(void* ud, const RES& p_resource);

    static Error write(
        const Variant& p_variant,
        StoreStringFunc p_store_string_func,
        void* p_store_string_ud,
        EncodeResourceFunc p_encode_res_func,
        void* p_encode_res_ud
    );
    static Error write_to_string(
        const Variant& p_variant,
        String& r_string,
        EncodeResourceFunc p_encode_res_func = nullptr,
        void* p_encode_res_ud                = nullptr
    );
};

#endif // VARIANT_PARSER_H
