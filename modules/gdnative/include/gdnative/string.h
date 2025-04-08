// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_STRING_H
#define GDNATIVE_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <wchar.h>

typedef wchar_t rebel_char_type;

#define REBEL_STRING_SIZE      sizeof(void*)
#define REBEL_CHAR_STRING_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_STRING_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_STRING_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_STRING_SIZE];
} rebel_string;

#endif

#ifndef REBEL_CORE_API_REBEL_CHAR_STRING_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_CHAR_STRING_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_CHAR_STRING_SIZE];
} rebel_char_string;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/array.h>
#include <gdnative/gdnative.h>
#include <gdnative/variant.h>

#ifdef __cplusplus
extern "C" {
#endif

rebel_int GDAPI rebel_char_string_length(const rebel_char_string* p_cs);
const char GDAPI* rebel_char_string_get_data(const rebel_char_string* p_cs);
void GDAPI rebel_char_string_destroy(rebel_char_string* p_cs);

void GDAPI rebel_string_new(rebel_string* r_dest);
void GDAPI
rebel_string_new_copy(rebel_string* r_dest, const rebel_string* p_src);
void GDAPI rebel_string_new_with_wide_string(
    rebel_string* r_dest,
    const wchar_t* p_contents,
    const int p_size
);

const wchar_t GDAPI* rebel_string_operator_index(
    rebel_string* p_self,
    const rebel_int p_idx
);
wchar_t GDAPI rebel_string_operator_index_const(
    const rebel_string* p_self,
    const rebel_int p_idx
);
const wchar_t GDAPI* rebel_string_wide_str(const rebel_string* p_self);

rebel_bool GDAPI rebel_string_operator_equal(
    const rebel_string* p_self,
    const rebel_string* p_b
);
rebel_bool GDAPI
rebel_string_operator_less(const rebel_string* p_self, const rebel_string* p_b);
rebel_string GDAPI
rebel_string_operator_plus(const rebel_string* p_self, const rebel_string* p_b);

/* Standard size stuff */

rebel_int GDAPI rebel_string_length(const rebel_string* p_self);

/* Helpers */

signed char GDAPI
rebel_string_casecmp_to(const rebel_string* p_self, const rebel_string* p_str);
signed char GDAPI rebel_string_nocasecmp_to(
    const rebel_string* p_self,
    const rebel_string* p_str
);
signed char GDAPI rebel_string_naturalnocasecmp_to(
    const rebel_string* p_self,
    const rebel_string* p_str
);

rebel_bool GDAPI rebel_string_begins_with(
    const rebel_string* p_self,
    const rebel_string* p_string
);
rebel_bool GDAPI rebel_string_begins_with_char_array(
    const rebel_string* p_self,
    const char* p_char_array
);
rebel_array GDAPI rebel_string_bigrams(const rebel_string* p_self);
rebel_string GDAPI rebel_string_chr(wchar_t p_character);
rebel_bool GDAPI rebel_string_ends_with(
    const rebel_string* p_self,
    const rebel_string* p_string
);
rebel_int GDAPI rebel_string_count(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from,
    rebel_int p_to
);
rebel_int GDAPI rebel_string_countn(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from,
    rebel_int p_to
);
rebel_int GDAPI
rebel_string_find(const rebel_string* p_self, rebel_string p_what);
rebel_int GDAPI rebel_string_find_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
);
rebel_int GDAPI
rebel_string_findmk(const rebel_string* p_self, const rebel_array* p_keys);
rebel_int GDAPI rebel_string_findmk_from(
    const rebel_string* p_self,
    const rebel_array* p_keys,
    rebel_int p_from
);
rebel_int GDAPI rebel_string_findmk_from_in_place(
    const rebel_string* p_self,
    const rebel_array* p_keys,
    rebel_int p_from,
    rebel_int* r_key
);
rebel_int GDAPI
rebel_string_findn(const rebel_string* p_self, rebel_string p_what);
rebel_int GDAPI rebel_string_findn_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
);
rebel_int GDAPI
rebel_string_find_last(const rebel_string* p_self, rebel_string p_what);
rebel_string GDAPI
rebel_string_format(const rebel_string* p_self, const rebel_variant* p_values);
rebel_string GDAPI rebel_string_format_with_custom_placeholder(
    const rebel_string* p_self,
    const rebel_variant* p_values,
    const char* p_placeholder
);
rebel_string GDAPI
rebel_string_hex_encode_buffer(const uint8_t* p_buffer, rebel_int p_len);
rebel_int GDAPI rebel_string_hex_to_int(const rebel_string* p_self);
rebel_int GDAPI
rebel_string_hex_to_int_without_prefix(const rebel_string* p_self);
rebel_string GDAPI rebel_string_insert(
    const rebel_string* p_self,
    rebel_int p_at_pos,
    rebel_string p_string
);
rebel_bool GDAPI rebel_string_is_numeric(const rebel_string* p_self);
rebel_bool GDAPI rebel_string_is_subsequence_of(
    const rebel_string* p_self,
    const rebel_string* p_string
);
rebel_bool GDAPI rebel_string_is_subsequence_ofi(
    const rebel_string* p_self,
    const rebel_string* p_string
);
rebel_string GDAPI
rebel_string_lpad(const rebel_string* p_self, rebel_int p_min_length);
rebel_string GDAPI rebel_string_lpad_with_custom_character(
    const rebel_string* p_self,
    rebel_int p_min_length,
    const rebel_string* p_character
);
rebel_bool GDAPI
rebel_string_match(const rebel_string* p_self, const rebel_string* p_wildcard);
rebel_bool GDAPI
rebel_string_matchn(const rebel_string* p_self, const rebel_string* p_wildcard);
rebel_string GDAPI rebel_string_md5(const uint8_t* p_md5);
rebel_string GDAPI rebel_string_num(double p_num);
rebel_string GDAPI rebel_string_num_int64(int64_t p_num, rebel_int p_base);
rebel_string GDAPI rebel_string_num_int64_capitalized(
    int64_t p_num,
    rebel_int p_base,
    rebel_bool p_capitalize_hex
);
rebel_string GDAPI rebel_string_num_real(double p_num);
rebel_string GDAPI rebel_string_num_scientific(double p_num);
rebel_string GDAPI
rebel_string_num_with_decimals(double p_num, rebel_int p_decimals);
rebel_string GDAPI
rebel_string_pad_decimals(const rebel_string* p_self, rebel_int p_digits);
rebel_string GDAPI
rebel_string_pad_zeros(const rebel_string* p_self, rebel_int p_digits);
rebel_string GDAPI rebel_string_replace_first(
    const rebel_string* p_self,
    rebel_string p_key,
    rebel_string p_with
);
rebel_string GDAPI rebel_string_replace(
    const rebel_string* p_self,
    rebel_string p_key,
    rebel_string p_with
);
rebel_string GDAPI rebel_string_replacen(
    const rebel_string* p_self,
    rebel_string p_key,
    rebel_string p_with
);
rebel_int GDAPI
rebel_string_rfind(const rebel_string* p_self, rebel_string p_what);
rebel_int GDAPI
rebel_string_rfindn(const rebel_string* p_self, rebel_string p_what);
rebel_int GDAPI rebel_string_rfind_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
);
rebel_int GDAPI rebel_string_rfindn_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
);
rebel_string GDAPI
rebel_string_rpad(const rebel_string* p_self, rebel_int p_min_length);
rebel_string GDAPI rebel_string_rpad_with_custom_character(
    const rebel_string* p_self,
    rebel_int p_min_length,
    const rebel_string* p_character
);
rebel_real GDAPI rebel_string_similarity(
    const rebel_string* p_self,
    const rebel_string* p_string
);
rebel_string GDAPI rebel_string_sprintf(
    const rebel_string* p_self,
    const rebel_array* p_values,
    rebel_bool* p_error
);
rebel_string GDAPI rebel_string_substr(
    const rebel_string* p_self,
    rebel_int p_from,
    rebel_int p_chars
);
double GDAPI rebel_string_to_double(const rebel_string* p_self);
rebel_real GDAPI rebel_string_to_float(const rebel_string* p_self);
rebel_int GDAPI rebel_string_to_int(const rebel_string* p_self);

rebel_string GDAPI
rebel_string_camelcase_to_underscore(const rebel_string* p_self);
rebel_string GDAPI
rebel_string_camelcase_to_underscore_lowercased(const rebel_string* p_self);
rebel_string GDAPI rebel_string_capitalize(const rebel_string* p_self);
double GDAPI rebel_string_char_to_double(const char* p_what);
rebel_int GDAPI rebel_string_char_to_int(const char* p_what);
int64_t GDAPI rebel_string_wchar_to_int(const wchar_t* p_str);
rebel_int GDAPI
rebel_string_char_to_int_with_len(const char* p_what, rebel_int p_len);
int64_t GDAPI
rebel_string_char_to_int64_with_len(const wchar_t* p_str, int p_len);
int64_t GDAPI rebel_string_hex_to_int64(const rebel_string* p_self);
int64_t GDAPI rebel_string_hex_to_int64_with_prefix(const rebel_string* p_self);
int64_t GDAPI rebel_string_to_int64(const rebel_string* p_self);
double GDAPI rebel_string_unicode_char_to_double(
    const wchar_t* p_str,
    const wchar_t** r_end
);

rebel_int GDAPI rebel_string_get_slice_count(
    const rebel_string* p_self,
    rebel_string p_splitter
);
rebel_string GDAPI rebel_string_get_slice(
    const rebel_string* p_self,
    rebel_string p_splitter,
    rebel_int p_slice
);
rebel_string GDAPI rebel_string_get_slicec(
    const rebel_string* p_self,
    wchar_t p_splitter,
    rebel_int p_slice
);

rebel_array GDAPI
rebel_string_split(const rebel_string* p_self, const rebel_string* p_splitter);
rebel_array GDAPI rebel_string_split_allow_empty(
    const rebel_string* p_self,
    const rebel_string* p_splitter
);
rebel_array GDAPI rebel_string_split_floats(
    const rebel_string* p_self,
    const rebel_string* p_splitter
);
rebel_array GDAPI rebel_string_split_floats_allows_empty(
    const rebel_string* p_self,
    const rebel_string* p_splitter
);
rebel_array GDAPI rebel_string_split_floats_mk(
    const rebel_string* p_self,
    const rebel_array* p_splitters
);
rebel_array GDAPI rebel_string_split_floats_mk_allows_empty(
    const rebel_string* p_self,
    const rebel_array* p_splitters
);
rebel_array GDAPI rebel_string_split_ints(
    const rebel_string* p_self,
    const rebel_string* p_splitter
);
rebel_array GDAPI rebel_string_split_ints_allows_empty(
    const rebel_string* p_self,
    const rebel_string* p_splitter
);
rebel_array GDAPI rebel_string_split_ints_mk(
    const rebel_string* p_self,
    const rebel_array* p_splitters
);
rebel_array GDAPI rebel_string_split_ints_mk_allows_empty(
    const rebel_string* p_self,
    const rebel_array* p_splitters
);
rebel_array GDAPI rebel_string_split_spaces(const rebel_string* p_self);

wchar_t GDAPI rebel_string_char_lowercase(wchar_t p_char);
wchar_t GDAPI rebel_string_char_uppercase(wchar_t p_char);
rebel_string GDAPI rebel_string_to_lower(const rebel_string* p_self);
rebel_string GDAPI rebel_string_to_upper(const rebel_string* p_self);

rebel_string GDAPI rebel_string_get_basename(const rebel_string* p_self);
rebel_string GDAPI rebel_string_get_extension(const rebel_string* p_self);
rebel_string GDAPI
rebel_string_left(const rebel_string* p_self, rebel_int p_pos);
wchar_t GDAPI rebel_string_ord_at(const rebel_string* p_self, rebel_int p_idx);
rebel_string GDAPI
rebel_string_plus_file(const rebel_string* p_self, const rebel_string* p_file);
rebel_string GDAPI
rebel_string_right(const rebel_string* p_self, rebel_int p_pos);
rebel_string GDAPI rebel_string_strip_edges(
    const rebel_string* p_self,
    rebel_bool p_left,
    rebel_bool p_right
);
rebel_string GDAPI rebel_string_strip_escapes(const rebel_string* p_self);

void GDAPI
rebel_string_erase(rebel_string* p_self, rebel_int p_pos, rebel_int p_chars);

rebel_char_string GDAPI rebel_string_ascii(const rebel_string* p_self);
rebel_char_string GDAPI rebel_string_ascii_extended(const rebel_string* p_self);
rebel_char_string GDAPI rebel_string_utf8(const rebel_string* p_self);
rebel_bool GDAPI
rebel_string_parse_utf8(rebel_string* p_self, const char* p_utf8);
rebel_bool GDAPI rebel_string_parse_utf8_with_len(
    rebel_string* p_self,
    const char* p_utf8,
    rebel_int p_len
);
rebel_string GDAPI rebel_string_chars_to_utf8(const char* p_utf8);
rebel_string GDAPI
rebel_string_chars_to_utf8_with_len(const char* p_utf8, rebel_int p_len);

uint32_t GDAPI rebel_string_hash(const rebel_string* p_self);
uint64_t GDAPI rebel_string_hash64(const rebel_string* p_self);
uint32_t GDAPI rebel_string_hash_chars(const char* p_cstr);
uint32_t GDAPI
rebel_string_hash_chars_with_len(const char* p_cstr, rebel_int p_len);
uint32_t GDAPI rebel_string_hash_utf8_chars(const wchar_t* p_str);
uint32_t GDAPI
rebel_string_hash_utf8_chars_with_len(const wchar_t* p_str, rebel_int p_len);
rebel_pool_byte_array GDAPI rebel_string_md5_buffer(const rebel_string* p_self);
rebel_string GDAPI rebel_string_md5_text(const rebel_string* p_self);
rebel_pool_byte_array GDAPI
rebel_string_sha256_buffer(const rebel_string* p_self);
rebel_string GDAPI rebel_string_sha256_text(const rebel_string* p_self);

rebel_bool rebel_string_empty(const rebel_string* p_self);

// path functions
rebel_string GDAPI rebel_string_get_base_dir(const rebel_string* p_self);
rebel_string GDAPI rebel_string_get_file(const rebel_string* p_self);
rebel_string GDAPI rebel_string_humanize_size(uint64_t p_size);
rebel_bool GDAPI rebel_string_is_abs_path(const rebel_string* p_self);
rebel_bool GDAPI rebel_string_is_rel_path(const rebel_string* p_self);
rebel_bool GDAPI rebel_string_is_resource_file(const rebel_string* p_self);
rebel_string GDAPI
rebel_string_path_to(const rebel_string* p_self, const rebel_string* p_path);
rebel_string GDAPI rebel_string_path_to_file(
    const rebel_string* p_self,
    const rebel_string* p_path
);
rebel_string GDAPI rebel_string_simplify_path(const rebel_string* p_self);

rebel_string GDAPI rebel_string_c_escape(const rebel_string* p_self);
rebel_string GDAPI rebel_string_c_escape_multiline(const rebel_string* p_self);
rebel_string GDAPI rebel_string_c_unescape(const rebel_string* p_self);
rebel_string GDAPI rebel_string_http_escape(const rebel_string* p_self);
rebel_string GDAPI rebel_string_http_unescape(const rebel_string* p_self);
rebel_string GDAPI rebel_string_json_escape(const rebel_string* p_self);
rebel_string GDAPI
rebel_string_word_wrap(const rebel_string* p_self, rebel_int p_chars_per_line);
rebel_string GDAPI rebel_string_xml_escape(const rebel_string* p_self);
rebel_string GDAPI
rebel_string_xml_escape_with_quotes(const rebel_string* p_self);
rebel_string GDAPI rebel_string_xml_unescape(const rebel_string* p_self);

rebel_string GDAPI rebel_string_percent_decode(const rebel_string* p_self);
rebel_string GDAPI rebel_string_percent_encode(const rebel_string* p_self);

rebel_bool GDAPI rebel_string_is_valid_float(const rebel_string* p_self);
rebel_bool GDAPI rebel_string_is_valid_hex_number(
    const rebel_string* p_self,
    rebel_bool p_with_prefix
);
rebel_bool GDAPI rebel_string_is_valid_html_color(const rebel_string* p_self);
rebel_bool GDAPI rebel_string_is_valid_identifier(const rebel_string* p_self);
rebel_bool GDAPI rebel_string_is_valid_integer(const rebel_string* p_self);
rebel_bool GDAPI rebel_string_is_valid_ip_address(const rebel_string* p_self);

rebel_string GDAPI rebel_string_dedent(const rebel_string* p_self);
rebel_string GDAPI rebel_string_trim_prefix(
    const rebel_string* p_self,
    const rebel_string* p_prefix
);
rebel_string GDAPI rebel_string_trim_suffix(
    const rebel_string* p_self,
    const rebel_string* p_suffix
);
rebel_string GDAPI
rebel_string_rstrip(const rebel_string* p_self, const rebel_string* p_chars);
rebel_pool_string_array GDAPI rebel_string_rsplit(
    const rebel_string* p_self,
    const rebel_string* p_divisor,
    const rebel_bool p_allow_empty,
    const rebel_int p_maxsplit
);

void GDAPI rebel_string_destroy(rebel_string* p_self);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_STRING_H
