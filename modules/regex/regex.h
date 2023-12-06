// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REGEX_H
#define REGEX_H

#include "core/array.h"
#include "core/dictionary.h"
#include "core/map.h"
#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

class RegExMatch : public Reference {
    GDCLASS(RegExMatch, Reference);

    struct Range {
        int start;
        int end;
    };

    String subject;
    Vector<Range> data;
    Map<String, int> names;

    friend class RegEx;

protected:
    static void _bind_methods();

    int _find(const Variant& p_name) const;

public:
    String get_subject() const;
    int get_group_count() const;
    Dictionary get_names() const;

    Array get_strings() const;
    String get_string(const Variant& p_name) const;
    int get_start(const Variant& p_name) const;
    int get_end(const Variant& p_name) const;
};

class RegEx : public Reference {
    GDCLASS(RegEx, Reference);

    void* general_ctx;
    void* code;
    String pattern;

    void _pattern_info(uint32_t what, void* where) const;

protected:
    static void _bind_methods();

public:
    void clear();
    Error compile(const String& p_pattern);
    void _init(const String& p_pattern = "");

    Ref<RegExMatch> search(
        const String& p_subject,
        int p_offset = 0,
        int p_end    = -1
    ) const;
    Array search_all(const String& p_subject, int p_offset = 0, int p_end = -1)
        const;
    String sub(
        const String& p_subject,
        const String& p_replacement,
        bool p_all   = false,
        int p_offset = 0,
        int p_end    = -1
    ) const;

    bool is_valid() const;
    String get_pattern() const;
    int get_group_count() const;
    Array get_names() const;

    RegEx();
    RegEx(const String& p_pattern);
    ~RegEx();
};

#endif // REGEX_H
