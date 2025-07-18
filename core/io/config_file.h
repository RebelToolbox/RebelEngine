// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include "core/ordered_hash_map.h"
#include "core/reference.h"

template <class T, class A>
class List;
template <class T>
class PoolVector;
class String;
class Variant;
template <class T>
class Vector;

enum Error;

typedef PoolVector<String> PoolStringArray;

class ConfigFile : public Reference {
    GDCLASS(ConfigFile, Reference);

public:
    Variant get_value(
        const String& section,
        const String& key,
        Variant default_value = Variant()
    ) const;
    void set_value(
        const String& section,
        const String& key,
        const Variant& value
    );

    bool has_section(const String& section) const;
    bool has_section_key(const String& section, const String& key) const;

    void get_sections(List<String>* sections) const;
    void get_section_keys(const String& section, List<String>* keys) const;

    void clear();
    void erase_section(const String& section);
    void erase_section_key(const String& section, const String& key);

    Error parse(const String& data);
    Error load(const String& path);
    Error load_encrypted_pass(const String& path, const String& pass);
    Error load_encrypted(const String& path, const Vector<uint8_t>& key);

    Error save(const String& path) const;
    Error save_encrypted_pass(const String& path, const String& password) const;
    Error save_encrypted(const String& path, const Vector<uint8_t>& key) const;

private:
    static void _bind_methods();

    PoolStringArray _get_sections() const;
    PoolStringArray _get_section_keys(const String& section) const;

    OrderedHashMap<String, OrderedHashMap<String, Variant>> values;
};

#endif // CONFIG_FILE_H
