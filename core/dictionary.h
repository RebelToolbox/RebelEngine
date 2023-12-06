// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "core/array.h"
#include "core/list.h"
#include "core/ustring.h"

class Variant;

struct DictionaryPrivate;

class Dictionary {
    mutable DictionaryPrivate* _p;

    void _ref(const Dictionary& p_from) const;
    void _unref() const;

public:
    void get_key_list(List<Variant>* p_keys) const;
    Variant get_key_at_index(int p_index) const;
    Variant get_value_at_index(int p_index) const;

    Variant& operator[](const Variant& p_key);
    const Variant& operator[](const Variant& p_key) const;

    const Variant* getptr(const Variant& p_key) const;
    Variant* getptr(const Variant& p_key);

    Variant get_valid(const Variant& p_key) const;
    Variant get(const Variant& p_key, const Variant& p_default) const;

    int size() const;
    bool empty() const;
    void clear();

    bool has(const Variant& p_key) const;
    bool has_all(const Array& p_keys) const;

    bool erase(const Variant& p_key);

    bool operator==(const Dictionary& p_dictionary) const;
    bool operator!=(const Dictionary& p_dictionary) const;

    uint32_t hash() const;
    void operator=(const Dictionary& p_dictionary);

    const Variant* next(const Variant* p_key = nullptr) const;

    Array keys() const;
    Array values() const;

    Dictionary duplicate(bool p_deep = false) const;

    const void* id() const;

    Dictionary(const Dictionary& p_from);
    Dictionary();
    ~Dictionary();
};

#endif // DICTIONARY_H
