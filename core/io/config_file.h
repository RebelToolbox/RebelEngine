// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include "core/ordered_hash_map.h"
#include "core/os/file_access.h"
#include "core/reference.h"
#include "core/variant_parser.h"

class ConfigFile : public Reference {
    GDCLASS(ConfigFile, Reference);

    OrderedHashMap<String, OrderedHashMap<String, Variant>> values;

    PoolStringArray _get_sections() const;
    PoolStringArray _get_section_keys(const String& p_section) const;
    Error _internal_load(const String& p_path, FileAccess* f);
    Error _internal_save(FileAccess* file);

    Error _parse(const String& p_path, VariantParser::Stream* p_stream);

protected:
    static void _bind_methods();

public:
    void set_value(
        const String& p_section,
        const String& p_key,
        const Variant& p_value
    );
    Variant get_value(
        const String& p_section,
        const String& p_key,
        Variant p_default = Variant()
    ) const;

    bool has_section(const String& p_section) const;
    bool has_section_key(const String& p_section, const String& p_key) const;

    void get_sections(List<String>* r_sections) const;
    void get_section_keys(const String& p_section, List<String>* r_keys) const;

    void erase_section(const String& p_section);
    void erase_section_key(const String& p_section, const String& p_key);

    Error save(const String& p_path);
    Error load(const String& p_path);
    Error parse(const String& p_data);

    void clear();

    Error load_encrypted(const String& p_path, const Vector<uint8_t>& p_key);
    Error load_encrypted_pass(const String& p_path, const String& p_pass);

    Error save_encrypted(const String& p_path, const Vector<uint8_t>& p_key);
    Error save_encrypted_pass(const String& p_path, const String& p_pass);
};

#endif // CONFIG_FILE_H
