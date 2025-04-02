// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/string.h"

#include "core/string_name.h"
#include "core/ustring.h"
#include "core/variant.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_char_string) == sizeof(CharString),
    "CharString size mismatch"
);
static_assert(sizeof(rebel_string) == sizeof(String), "String size mismatch");
static_assert(
    sizeof(rebel_char_type) == sizeof(CharType),
    "CharType size mismatch"
);

rebel_int GDAPI rebel_char_string_length(const rebel_char_string* p_cs) {
    const CharString* cs = (const CharString*)p_cs;

    return cs->length();
}

const char GDAPI* rebel_char_string_get_data(const rebel_char_string* p_cs) {
    const CharString* cs = (const CharString*)p_cs;

    return cs->get_data();
}

void GDAPI rebel_char_string_destroy(rebel_char_string* p_cs) {
    CharString* cs = (CharString*)p_cs;

    cs->~CharString();
}

void GDAPI rebel_string_new(rebel_string* r_dest) {
    String* dest = (String*)r_dest;
    memnew_placement(dest, String);
}

void GDAPI
rebel_string_new_copy(rebel_string* r_dest, const rebel_string* p_src) {
    String* dest      = (String*)r_dest;
    const String* src = (const String*)p_src;
    memnew_placement(dest, String(*src));
}

void GDAPI rebel_string_new_with_wide_string(
    rebel_string* r_dest,
    const wchar_t* p_contents,
    const int p_size
) {
    String* dest = (String*)r_dest;
    memnew_placement(dest, String(p_contents, p_size));
}

const wchar_t GDAPI* rebel_string_operator_index(
    rebel_string* p_self,
    const rebel_int p_idx
) {
    String* self = (String*)p_self;
    return &(self->operator[](p_idx));
}

wchar_t GDAPI rebel_string_operator_index_const(
    const rebel_string* p_self,
    const rebel_int p_idx
) {
    const String* self = (const String*)p_self;
    return self->operator[](p_idx);
}

const wchar_t GDAPI* rebel_string_wide_str(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    return self->c_str();
}

rebel_bool GDAPI rebel_string_operator_equal(
    const rebel_string* p_self,
    const rebel_string* p_b
) {
    const String* self = (const String*)p_self;
    const String* b    = (const String*)p_b;
    return *self == *b;
}

rebel_bool GDAPI rebel_string_operator_less(
    const rebel_string* p_self,
    const rebel_string* p_b
) {
    const String* self = (const String*)p_self;
    const String* b    = (const String*)p_b;
    return *self < *b;
}

rebel_string GDAPI rebel_string_operator_plus(
    const rebel_string* p_self,
    const rebel_string* p_b
) {
    rebel_string ret;
    const String* self = (const String*)p_self;
    const String* b    = (const String*)p_b;
    memnew_placement(&ret, String(*self + *b));
    return ret;
}

void GDAPI rebel_string_destroy(rebel_string* p_self) {
    String* self = (String*)p_self;
    self->~String();
}

/* Standard size stuff */

rebel_int GDAPI rebel_string_length(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->length();
}

/* Helpers */

signed char GDAPI
rebel_string_casecmp_to(const rebel_string* p_self, const rebel_string* p_str) {
    const String* self = (const String*)p_self;
    const String* str  = (const String*)p_str;

    return self->casecmp_to(*str);
}

signed char GDAPI rebel_string_nocasecmp_to(
    const rebel_string* p_self,
    const rebel_string* p_str
) {
    const String* self = (const String*)p_self;
    const String* str  = (const String*)p_str;

    return self->nocasecmp_to(*str);
}

signed char GDAPI rebel_string_naturalnocasecmp_to(
    const rebel_string* p_self,
    const rebel_string* p_str
) {
    const String* self = (const String*)p_self;
    const String* str  = (const String*)p_str;

    return self->naturalnocasecmp_to(*str);
}

rebel_bool GDAPI rebel_string_begins_with(
    const rebel_string* p_self,
    const rebel_string* p_string
) {
    const String* self   = (const String*)p_self;
    const String* string = (const String*)p_string;

    return self->begins_with(*string);
}

rebel_bool GDAPI rebel_string_begins_with_char_array(
    const rebel_string* p_self,
    const char* p_char_array
) {
    const String* self = (const String*)p_self;

    return self->begins_with(p_char_array);
}

rebel_array GDAPI rebel_string_bigrams(const rebel_string* p_self) {
    const String* self          = (const String*)p_self;
    Vector<String> return_value = self->bigrams();

    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy = (Array*)&result;
    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
};

rebel_string GDAPI rebel_string_chr(wchar_t p_character) {
    rebel_string result;
    memnew_placement(&result, String(String::chr(p_character)));

    return result;
}

rebel_bool GDAPI rebel_string_ends_with(
    const rebel_string* p_self,
    const rebel_string* p_string
) {
    const String* self   = (const String*)p_self;
    const String* string = (const String*)p_string;

    return self->ends_with(*string);
}

rebel_int GDAPI rebel_string_count(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from,
    rebel_int p_to
) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->count(*what, p_from, p_to);
}

rebel_int GDAPI rebel_string_countn(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from,
    rebel_int p_to
) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->countn(*what, p_from, p_to);
}

rebel_int GDAPI
rebel_string_find(const rebel_string* p_self, rebel_string p_what) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->find(*what);
}

rebel_int GDAPI rebel_string_find_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->find(*what, p_from);
}

rebel_int GDAPI
rebel_string_findmk(const rebel_string* p_self, const rebel_array* p_keys) {
    const String* self = (const String*)p_self;

    Vector<String> keys;
    Array* keys_proxy = (Array*)p_keys;
    keys.resize(keys_proxy->size());
    for (int i = 0; i < keys_proxy->size(); i++) {
        keys.write[i] = (*keys_proxy)[i];
    }

    return self->findmk(keys);
}

rebel_int GDAPI rebel_string_findmk_from(
    const rebel_string* p_self,
    const rebel_array* p_keys,
    rebel_int p_from
) {
    const String* self = (const String*)p_self;

    Vector<String> keys;
    Array* keys_proxy = (Array*)p_keys;
    keys.resize(keys_proxy->size());
    for (int i = 0; i < keys_proxy->size(); i++) {
        keys.write[i] = (*keys_proxy)[i];
    }

    return self->findmk(keys, p_from);
}

rebel_int GDAPI rebel_string_findmk_from_in_place(
    const rebel_string* p_self,
    const rebel_array* p_keys,
    rebel_int p_from,
    rebel_int* r_key
) {
    const String* self = (const String*)p_self;

    Vector<String> keys;
    Array* keys_proxy = (Array*)p_keys;
    keys.resize(keys_proxy->size());
    for (int i = 0; i < keys_proxy->size(); i++) {
        keys.write[i] = (*keys_proxy)[i];
    }

    return self->findmk(keys, p_from, r_key);
}

rebel_int GDAPI
rebel_string_findn(const rebel_string* p_self, rebel_string p_what) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->findn(*what);
}

rebel_int GDAPI rebel_string_findn_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->findn(*what, p_from);
}

rebel_int GDAPI
rebel_string_find_last(const rebel_string* p_self, rebel_string p_what) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->find_last(*what);
}

rebel_string GDAPI
rebel_string_format(const rebel_string* p_self, const rebel_variant* p_values) {
    const String* self    = (const String*)p_self;
    const Variant* values = (const Variant*)p_values;
    rebel_string result;
    memnew_placement(&result, String(self->format(*values)));

    return result;
}

rebel_string GDAPI rebel_string_format_with_custom_placeholder(
    const rebel_string* p_self,
    const rebel_variant* p_values,
    const char* p_placeholder
) {
    const String* self    = (const String*)p_self;
    const Variant* values = (const Variant*)p_values;
    String placeholder    = String(p_placeholder);
    rebel_string result;
    memnew_placement(&result, String(self->format(*values, placeholder)));

    return result;
}

rebel_string GDAPI
rebel_string_hex_encode_buffer(const uint8_t* p_buffer, rebel_int p_len) {
    rebel_string result;
    memnew_placement(
        &result,
        String(String::hex_encode_buffer(p_buffer, p_len))
    );

    return result;
}

rebel_int GDAPI rebel_string_hex_to_int(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->hex_to_int();
}

rebel_int GDAPI
rebel_string_hex_to_int_without_prefix(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->hex_to_int(true);
}

rebel_string GDAPI rebel_string_insert(
    const rebel_string* p_self,
    rebel_int p_at_pos,
    rebel_string p_string
) {
    const String* self = (const String*)p_self;
    String* content    = (String*)&p_string;
    rebel_string result;
    memnew_placement(&result, String(self->insert(p_at_pos, *content)));

    return result;
}

rebel_bool GDAPI rebel_string_is_numeric(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_numeric();
}

rebel_bool GDAPI rebel_string_is_subsequence_of(
    const rebel_string* p_self,
    const rebel_string* p_string
) {
    const String* self   = (const String*)p_self;
    const String* string = (const String*)p_string;

    return self->is_subsequence_of(*string);
}

rebel_bool GDAPI rebel_string_is_subsequence_ofi(
    const rebel_string* p_self,
    const rebel_string* p_string
) {
    const String* self   = (const String*)p_self;
    const String* string = (const String*)p_string;

    return self->is_subsequence_ofi(*string);
}

rebel_string GDAPI
rebel_string_lpad(const rebel_string* p_self, rebel_int p_min_length) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->lpad(p_min_length)));

    return result;
}

rebel_string GDAPI rebel_string_lpad_with_custom_character(
    const rebel_string* p_self,
    rebel_int p_min_length,
    const rebel_string* p_character
) {
    const String* self      = (const String*)p_self;
    const String* character = (const String*)p_character;
    rebel_string result;
    memnew_placement(&result, String(self->lpad(p_min_length, *character)));

    return result;
}

rebel_bool GDAPI
rebel_string_match(const rebel_string* p_self, const rebel_string* p_wildcard) {
    const String* self     = (const String*)p_self;
    const String* wildcard = (const String*)p_wildcard;

    return self->match(*wildcard);
}

rebel_bool GDAPI rebel_string_matchn(
    const rebel_string* p_self,
    const rebel_string* p_wildcard
) {
    const String* self     = (const String*)p_self;
    const String* wildcard = (const String*)p_wildcard;

    return self->matchn(*wildcard);
}

rebel_string GDAPI rebel_string_md5(const uint8_t* p_md5) {
    rebel_string result;
    memnew_placement(&result, String(String::md5(p_md5)));

    return result;
}

rebel_string GDAPI rebel_string_num(double p_num) {
    rebel_string result;
    memnew_placement(&result, String(String::num(p_num)));

    return result;
}

rebel_string GDAPI rebel_string_num_int64(int64_t p_num, rebel_int p_base) {
    rebel_string result;
    memnew_placement(&result, String(String::num_int64(p_num, p_base)));

    return result;
}

rebel_string GDAPI rebel_string_num_int64_capitalized(
    int64_t p_num,
    rebel_int p_base,
    rebel_bool p_capitalize_hex
) {
    rebel_string result;
    memnew_placement(&result, String(String::num_int64(p_num, p_base, true)));

    return result;
}

rebel_string GDAPI rebel_string_num_real(double p_num) {
    rebel_string result;
    memnew_placement(&result, String(String::num_real(p_num)));

    return result;
}

rebel_string GDAPI rebel_string_num_scientific(double p_num) {
    rebel_string result;
    memnew_placement(&result, String(String::num_scientific(p_num)));

    return result;
}

rebel_string GDAPI
rebel_string_num_with_decimals(double p_num, rebel_int p_decimals) {
    rebel_string result;
    memnew_placement(&result, String(String::num(p_num, p_decimals)));

    return result;
}

rebel_string GDAPI
rebel_string_pad_decimals(const rebel_string* p_self, rebel_int p_digits) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->pad_decimals(p_digits)));

    return result;
}

rebel_string GDAPI
rebel_string_pad_zeros(const rebel_string* p_self, rebel_int p_digits) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->pad_zeros(p_digits)));

    return result;
}

rebel_string GDAPI rebel_string_replace(
    const rebel_string* p_self,
    rebel_string p_key,
    rebel_string p_with
) {
    const String* self = (const String*)p_self;
    String* key        = (String*)&p_key;
    String* with       = (String*)&p_with;
    rebel_string result;
    memnew_placement(&result, String(self->replace(*key, *with)));

    return result;
}

rebel_string GDAPI rebel_string_replacen(
    const rebel_string* p_self,
    rebel_string p_key,
    rebel_string p_with
) {
    const String* self = (const String*)p_self;
    String* key        = (String*)&p_key;
    String* with       = (String*)&p_with;
    rebel_string result;
    memnew_placement(&result, String(self->replacen(*key, *with)));

    return result;
}

rebel_int GDAPI
rebel_string_rfind(const rebel_string* p_self, rebel_string p_what) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->rfind(*what);
}

rebel_int GDAPI
rebel_string_rfindn(const rebel_string* p_self, rebel_string p_what) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->rfindn(*what);
}

rebel_int GDAPI rebel_string_rfind_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->rfind(*what, p_from);
}

rebel_int GDAPI rebel_string_rfindn_from(
    const rebel_string* p_self,
    rebel_string p_what,
    rebel_int p_from
) {
    const String* self = (const String*)p_self;
    String* what       = (String*)&p_what;

    return self->rfindn(*what, p_from);
}

rebel_string GDAPI rebel_string_replace_first(
    const rebel_string* p_self,
    rebel_string p_key,
    rebel_string p_with
) {
    const String* self = (const String*)p_self;
    String* key        = (String*)&p_key;
    String* with       = (String*)&p_with;
    rebel_string result;
    memnew_placement(&result, String(self->replace_first(*key, *with)));

    return result;
}

rebel_string GDAPI
rebel_string_rpad(const rebel_string* p_self, rebel_int p_min_length) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->rpad(p_min_length)));

    return result;
}

rebel_string GDAPI rebel_string_rpad_with_custom_character(
    const rebel_string* p_self,
    rebel_int p_min_length,
    const rebel_string* p_character
) {
    const String* self      = (const String*)p_self;
    const String* character = (const String*)p_character;
    rebel_string result;
    memnew_placement(&result, String(self->rpad(p_min_length, *character)));

    return result;
}

rebel_real GDAPI rebel_string_similarity(
    const rebel_string* p_self,
    const rebel_string* p_string
) {
    const String* self   = (const String*)p_self;
    const String* string = (const String*)p_string;

    return self->similarity(*string);
}

rebel_string GDAPI rebel_string_sprintf(
    const rebel_string* p_self,
    const rebel_array* p_values,
    rebel_bool* p_error
) {
    const String* self  = (const String*)p_self;
    const Array* values = (const Array*)p_values;

    rebel_string result;
    String return_value = self->sprintf(*values, p_error);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_substr(
    const rebel_string* p_self,
    rebel_int p_from,
    rebel_int p_chars
) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->substr(p_from, p_chars)));

    return result;
}

double GDAPI rebel_string_to_double(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->to_double();
}

rebel_real GDAPI rebel_string_to_float(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->to_float();
}

rebel_int GDAPI rebel_string_to_int(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->to_int();
}

rebel_string GDAPI rebel_string_capitalize(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->capitalize()));

    return result;
}

rebel_string GDAPI
rebel_string_camelcase_to_underscore(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->camelcase_to_underscore(false)));

    return result;
}

rebel_string GDAPI
rebel_string_camelcase_to_underscore_lowercased(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->camelcase_to_underscore()));

    return result;
}

double GDAPI rebel_string_char_to_double(const char* p_what) {
    return String::to_double(p_what);
}

rebel_int GDAPI rebel_string_char_to_int(const char* p_what) {
    return String::to_int(p_what);
}

int64_t GDAPI rebel_string_wchar_to_int(const wchar_t* p_str) {
    return String::to_int(p_str);
}

rebel_int GDAPI
rebel_string_char_to_int_with_len(const char* p_what, rebel_int p_len) {
    return String::to_int(p_what, p_len);
}

int64_t GDAPI
rebel_string_char_to_int64_with_len(const wchar_t* p_str, int p_len) {
    return String::to_int(p_str, p_len);
}

int64_t GDAPI rebel_string_hex_to_int64(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->hex_to_int64(false);
}

int64_t GDAPI rebel_string_hex_to_int64_with_prefix(const rebel_string* p_self
) {
    const String* self = (const String*)p_self;

    return self->hex_to_int64();
}

int64_t GDAPI rebel_string_to_int64(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->to_int64();
}

double GDAPI rebel_string_unicode_char_to_double(
    const wchar_t* p_str,
    const wchar_t** r_end
) {
    return String::to_double(p_str, r_end);
}

rebel_string GDAPI rebel_string_get_slice(
    const rebel_string* p_self,
    rebel_string p_splitter,
    rebel_int p_slice
) {
    const String* self = (const String*)p_self;
    String* splitter   = (String*)&p_splitter;
    rebel_string result;
    memnew_placement(&result, String(self->get_slice(*splitter, p_slice)));

    return result;
}

rebel_string GDAPI rebel_string_get_slicec(
    const rebel_string* p_self,
    wchar_t p_splitter,
    rebel_int p_slice
) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->get_slicec(p_splitter, p_slice)));

    return result;
}

rebel_array GDAPI
rebel_string_split(const rebel_string* p_self, const rebel_string* p_splitter) {
    const String* self     = (const String*)p_self;
    const String* splitter = (const String*)p_splitter;
    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy                = (Array*)&result;
    Vector<String> return_value = self->split(*splitter, false);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_allow_empty(
    const rebel_string* p_self,
    const rebel_string* p_splitter
) {
    const String* self     = (const String*)p_self;
    const String* splitter = (const String*)p_splitter;
    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy                = (Array*)&result;
    Vector<String> return_value = self->split(*splitter);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_floats(
    const rebel_string* p_self,
    const rebel_string* p_splitter
) {
    const String* self     = (const String*)p_self;
    const String* splitter = (const String*)p_splitter;
    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy               = (Array*)&result;
    Vector<float> return_value = self->split_floats(*splitter, false);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_floats_allows_empty(
    const rebel_string* p_self,
    const rebel_string* p_splitter
) {
    const String* self     = (const String*)p_self;
    const String* splitter = (const String*)p_splitter;
    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy               = (Array*)&result;
    Vector<float> return_value = self->split_floats(*splitter);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_floats_mk(
    const rebel_string* p_self,
    const rebel_array* p_splitters
) {
    const String* self = (const String*)p_self;

    Vector<String> splitters;
    Array* splitter_proxy = (Array*)p_splitters;
    splitters.resize(splitter_proxy->size());
    for (int i = 0; i < splitter_proxy->size(); i++) {
        splitters.write[i] = (*splitter_proxy)[i];
    }

    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy               = (Array*)&result;
    Vector<float> return_value = self->split_floats_mk(splitters, false);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_floats_mk_allows_empty(
    const rebel_string* p_self,
    const rebel_array* p_splitters
) {
    const String* self = (const String*)p_self;

    Vector<String> splitters;
    Array* splitter_proxy = (Array*)p_splitters;
    splitters.resize(splitter_proxy->size());
    for (int i = 0; i < splitter_proxy->size(); i++) {
        splitters.write[i] = (*splitter_proxy)[i];
    }

    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy               = (Array*)&result;
    Vector<float> return_value = self->split_floats_mk(splitters);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_ints(
    const rebel_string* p_self,
    const rebel_string* p_splitter
) {
    const String* self     = (const String*)p_self;
    const String* splitter = (const String*)p_splitter;
    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy             = (Array*)&result;
    Vector<int> return_value = self->split_ints(*splitter, false);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_ints_allows_empty(
    const rebel_string* p_self,
    const rebel_string* p_splitter
) {
    const String* self     = (const String*)p_self;
    const String* splitter = (const String*)p_splitter;
    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy             = (Array*)&result;
    Vector<int> return_value = self->split_ints(*splitter);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_ints_mk(
    const rebel_string* p_self,
    const rebel_array* p_splitters
) {
    const String* self = (const String*)p_self;

    Vector<String> splitters;
    Array* splitter_proxy = (Array*)p_splitters;
    splitters.resize(splitter_proxy->size());
    for (int i = 0; i < splitter_proxy->size(); i++) {
        splitters.write[i] = (*splitter_proxy)[i];
    }

    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy             = (Array*)&result;
    Vector<int> return_value = self->split_ints_mk(splitters, false);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_ints_mk_allows_empty(
    const rebel_string* p_self,
    const rebel_array* p_splitters
) {
    const String* self = (const String*)p_self;

    Vector<String> splitters;
    Array* splitter_proxy = (Array*)p_splitters;
    splitters.resize(splitter_proxy->size());
    for (int i = 0; i < splitter_proxy->size(); i++) {
        splitters.write[i] = (*splitter_proxy)[i];
    }

    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy             = (Array*)&result;
    Vector<int> return_value = self->split_ints_mk(splitters);

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_array GDAPI rebel_string_split_spaces(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_array result;
    memnew_placement(&result, Array);
    Array* proxy                = (Array*)&result;
    Vector<String> return_value = self->split_spaces();

    proxy->resize(return_value.size());
    for (int i = 0; i < return_value.size(); i++) {
        (*proxy)[i] = return_value[i];
    }

    return result;
}

rebel_int GDAPI rebel_string_get_slice_count(
    const rebel_string* p_self,
    rebel_string p_splitter
) {
    const String* self = (const String*)p_self;
    String* splitter   = (String*)&p_splitter;

    return self->get_slice_count(*splitter);
}

wchar_t GDAPI rebel_string_char_lowercase(wchar_t p_char) {
    return String::char_lowercase(p_char);
}

wchar_t GDAPI rebel_string_char_uppercase(wchar_t p_char) {
    return String::char_uppercase(p_char);
}

rebel_string GDAPI rebel_string_to_lower(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->to_lower()));

    return result;
}

rebel_string GDAPI rebel_string_to_upper(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->to_upper()));

    return result;
}

rebel_string GDAPI rebel_string_get_basename(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->get_basename()));

    return result;
}

rebel_string GDAPI rebel_string_get_extension(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->get_extension()));

    return result;
}

rebel_string GDAPI
rebel_string_left(const rebel_string* p_self, rebel_int p_pos) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->left(p_pos)));

    return result;
}

wchar_t GDAPI rebel_string_ord_at(const rebel_string* p_self, rebel_int p_idx) {
    const String* self = (const String*)p_self;

    return self->ord_at(p_idx);
}

rebel_string GDAPI
rebel_string_plus_file(const rebel_string* p_self, const rebel_string* p_file) {
    const String* self = (const String*)p_self;
    const String* file = (const String*)p_file;
    rebel_string result;
    memnew_placement(&result, String(self->plus_file(*file)));

    return result;
}

rebel_string GDAPI
rebel_string_right(const rebel_string* p_self, rebel_int p_pos) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->right(p_pos)));

    return result;
}

rebel_string GDAPI rebel_string_strip_edges(
    const rebel_string* p_self,
    rebel_bool p_left,
    rebel_bool p_right
) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->strip_edges(p_left, p_right)));

    return result;
}

rebel_string GDAPI rebel_string_strip_escapes(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->strip_escapes()));

    return result;
}

void GDAPI
rebel_string_erase(rebel_string* p_self, rebel_int p_pos, rebel_int p_chars) {
    String* self = (String*)p_self;

    return self->erase(p_pos, p_chars);
}

rebel_char_string GDAPI rebel_string_ascii(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_char_string result;

    memnew_placement(&result, CharString(self->ascii()));

    return result;
}

rebel_char_string GDAPI rebel_string_ascii_extended(const rebel_string* p_self
) {
    const String* self = (const String*)p_self;

    rebel_char_string result;

    memnew_placement(&result, CharString(self->ascii(true)));

    return result;
}

rebel_char_string GDAPI rebel_string_utf8(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    rebel_char_string result;

    memnew_placement(&result, CharString(self->utf8()));

    return result;
}

rebel_bool GDAPI
rebel_string_parse_utf8(rebel_string* p_self, const char* p_utf8) {
    String* self = (String*)p_self;

    return self->parse_utf8(p_utf8);
}

rebel_bool GDAPI rebel_string_parse_utf8_with_len(
    rebel_string* p_self,
    const char* p_utf8,
    rebel_int p_len
) {
    String* self = (String*)p_self;

    return self->parse_utf8(p_utf8, p_len);
}

rebel_string GDAPI rebel_string_chars_to_utf8(const char* p_utf8) {
    rebel_string result;
    memnew_placement(&result, String(String::utf8(p_utf8)));

    return result;
}

rebel_string GDAPI
rebel_string_chars_to_utf8_with_len(const char* p_utf8, rebel_int p_len) {
    rebel_string result;
    memnew_placement(&result, String(String::utf8(p_utf8, p_len)));

    return result;
}

uint32_t GDAPI rebel_string_hash(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->hash();
}

uint64_t GDAPI rebel_string_hash64(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->hash64();
}

uint32_t GDAPI rebel_string_hash_chars(const char* p_cstr) {
    return String::hash(p_cstr);
}

uint32_t GDAPI
rebel_string_hash_chars_with_len(const char* p_cstr, rebel_int p_len) {
    return String::hash(p_cstr, p_len);
}

uint32_t GDAPI rebel_string_hash_utf8_chars(const wchar_t* p_str) {
    return String::hash(p_str);
}

uint32_t GDAPI
rebel_string_hash_utf8_chars_with_len(const wchar_t* p_str, rebel_int p_len) {
    return String::hash(p_str, p_len);
}

rebel_pool_byte_array GDAPI rebel_string_md5_buffer(const rebel_string* p_self
) {
    const String* self         = (const String*)p_self;
    Vector<uint8_t> tmp_result = self->md5_buffer();

    rebel_pool_byte_array result;
    memnew_placement(&result, PoolByteArray);
    PoolByteArray* proxy              = (PoolByteArray*)&result;
    PoolByteArray::Write proxy_writer = proxy->write();
    proxy->resize(tmp_result.size());

    for (int i = 0; i < tmp_result.size(); i++) {
        proxy_writer[i] = tmp_result[i];
    }

    return result;
}

rebel_string GDAPI rebel_string_md5_text(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->md5_text()));

    return result;
}

rebel_pool_byte_array GDAPI
rebel_string_sha256_buffer(const rebel_string* p_self) {
    const String* self         = (const String*)p_self;
    Vector<uint8_t> tmp_result = self->sha256_buffer();

    rebel_pool_byte_array result;
    memnew_placement(&result, PoolByteArray);
    PoolByteArray* proxy              = (PoolByteArray*)&result;
    PoolByteArray::Write proxy_writer = proxy->write();
    proxy->resize(tmp_result.size());

    for (int i = 0; i < tmp_result.size(); i++) {
        proxy_writer[i] = tmp_result[i];
    }

    return result;
}

rebel_string GDAPI rebel_string_sha256_text(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    memnew_placement(&result, String(self->sha256_text()));

    return result;
}

rebel_bool rebel_string_empty(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->empty();
}

// path functions
rebel_string GDAPI rebel_string_get_base_dir(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->get_base_dir();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_get_file(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->get_file();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_humanize_size(uint64_t p_size) {
    rebel_string result;
    String return_value = String::humanize_size(p_size);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_bool GDAPI rebel_string_is_abs_path(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_abs_path();
}

rebel_bool GDAPI rebel_string_is_rel_path(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_rel_path();
}

rebel_bool GDAPI rebel_string_is_resource_file(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_resource_file();
}

rebel_string GDAPI
rebel_string_path_to(const rebel_string* p_self, const rebel_string* p_path) {
    const String* self = (const String*)p_self;
    String* path       = (String*)p_path;
    rebel_string result;
    String return_value = self->path_to(*path);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_path_to_file(
    const rebel_string* p_self,
    const rebel_string* p_path
) {
    const String* self = (const String*)p_self;
    String* path       = (String*)p_path;
    rebel_string result;
    String return_value = self->path_to_file(*path);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_simplify_path(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->simplify_path();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_c_escape(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->c_escape();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_c_escape_multiline(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->c_escape_multiline();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_c_unescape(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->c_unescape();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_http_escape(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->http_escape();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_http_unescape(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->http_unescape();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_json_escape(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->json_escape();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI
rebel_string_word_wrap(const rebel_string* p_self, rebel_int p_chars_per_line) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->word_wrap(p_chars_per_line);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_xml_escape(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->xml_escape();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI
rebel_string_xml_escape_with_quotes(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->xml_escape(true);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_xml_unescape(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->xml_unescape();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_percent_decode(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->percent_decode();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_percent_encode(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->percent_encode();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_bool GDAPI rebel_string_is_valid_float(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_valid_float();
}

rebel_bool GDAPI rebel_string_is_valid_hex_number(
    const rebel_string* p_self,
    rebel_bool p_with_prefix
) {
    const String* self = (const String*)p_self;

    return self->is_valid_hex_number(p_with_prefix);
}

rebel_bool GDAPI rebel_string_is_valid_html_color(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_valid_html_color();
}

rebel_bool GDAPI rebel_string_is_valid_identifier(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_valid_identifier();
}

rebel_bool GDAPI rebel_string_is_valid_integer(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_valid_integer();
}

rebel_bool GDAPI rebel_string_is_valid_ip_address(const rebel_string* p_self) {
    const String* self = (const String*)p_self;

    return self->is_valid_ip_address();
}

rebel_string GDAPI rebel_string_dedent(const rebel_string* p_self) {
    const String* self = (const String*)p_self;
    rebel_string result;
    String return_value = self->dedent();
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_trim_prefix(
    const rebel_string* p_self,
    const rebel_string* p_prefix
) {
    const String* self = (const String*)p_self;
    String* prefix     = (String*)p_prefix;
    rebel_string result;
    String return_value = self->trim_prefix(*prefix);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI rebel_string_trim_suffix(
    const rebel_string* p_self,
    const rebel_string* p_suffix
) {
    const String* self = (const String*)p_self;
    String* suffix     = (String*)p_suffix;
    rebel_string result;
    String return_value = self->trim_suffix(*suffix);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_string GDAPI
rebel_string_rstrip(const rebel_string* p_self, const rebel_string* p_chars) {
    const String* self = (const String*)p_self;
    String* chars      = (String*)p_chars;
    rebel_string result;
    String return_value = self->rstrip(*chars);
    memnew_placement(&result, String(return_value));

    return result;
}

rebel_pool_string_array GDAPI rebel_string_rsplit(
    const rebel_string* p_self,
    const rebel_string* p_divisor,
    const rebel_bool p_allow_empty,
    const rebel_int p_maxsplit
) {
    const String* self = (const String*)p_self;
    String* divisor    = (String*)p_divisor;

    rebel_pool_string_array result;
    memnew_placement(&result, PoolStringArray);
    PoolStringArray* proxy              = (PoolStringArray*)&result;
    PoolStringArray::Write proxy_writer = proxy->write();
    Vector<String> tmp_result =
        self->rsplit(*divisor, p_allow_empty, p_maxsplit);
    proxy->resize(tmp_result.size());

    for (int i = 0; i < tmp_result.size(); i++) {
        proxy_writer[i] = tmp_result[i];
    }

    return result;
}

#ifdef __cplusplus
}
#endif
