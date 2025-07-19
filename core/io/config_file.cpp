// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "config_file.h"

#include "core/io/file_access_encrypted.h"
#include "core/os/keyboard.h"
#include "core/variant_parser.h"

namespace {
FileAccessEncrypted* get_file_access_encrypted(
    Error& error,
    FileAccess* file_access,
    const FileAccessEncrypted::Mode& mode,
    const String& password     = String(),
    const Vector<uint8_t>& key = Vector<uint8_t>()
) {
    FileAccessEncrypted* file_access_encrypted = memnew(FileAccessEncrypted);
    if (!key.empty()) {
        error = file_access_encrypted->open_and_parse(file_access, key, mode);
    } else {
        error = file_access_encrypted
                    ->open_and_parse_password(file_access, password, mode);
    }
    return file_access_encrypted;
}

Error load_stream(
    ConfigFile& config_file,
    VariantParser::Stream& stream,
    const String& source_name
) {
    String section;
    int line = 0;
    String error_string;
    Error error = OK;

    while (error == OK) {
        VariantParser::Tag tag;
        String assign;
        Variant value;

        error = VariantParser::parse_tag_assign_eof(
            &stream,
            line,
            error_string,
            tag,
            assign,
            value,
            nullptr,
            true
        );

        if (error == OK) {
            if (assign != String()) {
                config_file.set_value(section, assign, value);
            } else if (tag.name != String()) {
                section = tag.name;
            }
        }
    }

    if (error == ERR_FILE_EOF) {
        return OK;
    }
    ERR_PRINT(vformat(
        "ConfigFile parse error at %s:%d: %s.",
        source_name,
        line,
        error_string
    ));
    return error;
}

Error load_file_access(
    ConfigFile& config_file,
    FileAccess* file_access,
    const String& source_name
) {
    VariantParser::StreamFile stream;
    stream.f = file_access;
    return load_stream(config_file, stream, source_name);
}

void save_file_access(
    const OrderedHashMap<String, OrderedHashMap<String, Variant>>& values,
    FileAccess* file
) {
    for (auto section = values.front(); section; section = section.next()) {
        if (section != values.front()) {
            file->store_string("\n");
        }
        if (!section.key().empty()) {
            file->store_string(vformat("[%s]\n\n", section.key()));
        }

        for (auto key = section.get().front(); key; key = key.next()) {
            String value;
            VariantWriter::write_to_string(key.get(), value);
            file->store_string(
                vformat("%s=%s\n", key.key().property_name_encode(), value)
            );
        }
    }
}

Error load_file(
    ConfigFile& config_file,
    const String& path,
    const String& password     = String(),
    const Vector<uint8_t>& key = Vector<uint8_t>()
) {
    Error error;
    FileAccess* file_access = FileAccess::open(path, FileAccess::READ, &error);
    if (!file_access) {
        return error;
    }

    FileAccess* source = file_access;
    if (!password.empty() || !key.empty()) {
        source = get_file_access_encrypted(
            error,
            file_access,
            FileAccessEncrypted::MODE_READ,
            password,
            key
        );
        if (error) {
            memdelete(source);
            memdelete(file_access);
            return error;
        }
    }

    error = load_file_access(config_file, source, path);
    if (source != file_access) {
        memdelete(source);
    }
    memdelete(file_access);
    return error;
}

Error save_file(
    const OrderedHashMap<String, OrderedHashMap<String, Variant>>& values,
    const String& path,
    const String& password     = String(),
    const Vector<uint8_t>& key = Vector<uint8_t>()
) {
    Error error;
    FileAccess* file_access = FileAccess::open(path, FileAccess::WRITE, &error);
    if (!file_access) {
        return error;
    }

    FileAccess* destination = file_access;
    if (!password.empty() || !key.empty()) {
        destination = get_file_access_encrypted(
            error,
            file_access,
            FileAccessEncrypted::MODE_WRITE_AES256,
            password,
            key
        );
        if (error) {
            memdelete(destination);
            memdelete(file_access);
            return error;
        }
    }

    save_file_access(values, destination);
    if (destination != file_access) {
        memdelete(destination);
    }
    memdelete(file_access);
    return OK;
}
} // namespace

Variant ConfigFile::get_value(
    const String& section,
    const String& key,
    Variant default_value
) const {
    if (!values.has(section) || !values[section].has(key)) {
        ERR_FAIL_COND_V_MSG(
            default_value.get_type() == Variant::NIL,
            Variant(),
            vformat(
                "No key \"%s\" in section \"%s\", and no default given.",
                key,
                section
            )
        );
        return default_value;
    }
    return values[section][key];
}

void ConfigFile::set_value(
    const String& section,
    const String& key,
    const Variant& value
) {
    if (value.get_type() == Variant::NIL) {
        erase_section_key(section, key);
        return;
    }
    if (!values.has(section)) {
        values[section] = OrderedHashMap<String, Variant>();
    }
    values[section][key] = value;
}

bool ConfigFile::has_section(const String& section) const {
    return values.has(section);
}

bool ConfigFile::has_section_key(const String& section, const String& key)
    const {
    if (!values.has(section)) {
        return false;
    }
    return values[section].has(key);
}

void ConfigFile::get_sections(List<String>* sections) const {
    for (auto section = values.front(); section; section = section.next()) {
        sections->push_back(section.key());
    }
}

void ConfigFile::get_section_keys(const String& section, List<String>* keys)
    const {
    ERR_FAIL_COND_MSG(
        !values.has(section),
        vformat("Cannot get keys from nonexistent section \"%s\".", section)
    );
    for (auto key = values[section].front(); key; key = key.next()) {
        keys->push_back(key.key());
    }
}

void ConfigFile::clear() {
    values.clear();
}

void ConfigFile::erase_section(const String& section) {
    ERR_FAIL_COND_MSG(
        !values.has(section),
        vformat("Cannot erase non-existent section \"%s\".", section)
    );
    values.erase(section);
}

void ConfigFile::erase_section_key(const String& section, const String& key) {
    ERR_FAIL_COND_MSG(
        !values.has(section),
        vformat(
            "Cannot erase key \"%s\" from non-existent section \"%s\".",
            key,
            section
        )
    );
    ERR_FAIL_COND_MSG(
        !values[section].has(key),
        vformat(
            "Cannot erase non-existent key \"%s\" from section \"%s\".",
            key,
            section
        )
    );
    values[section].erase(key);
}

Error ConfigFile::load(const String& path) {
    return load_file(*this, path);
}

Error ConfigFile::load_encrypted_pass(
    const String& path,
    const String& password
) {
    return load_file(*this, path, password);
}

Error ConfigFile::load_encrypted(
    const String& path,
    const Vector<uint8_t>& key
) {
    return load_file(*this, path, String(), key);
}

Error ConfigFile::parse(const String& data) {
    VariantParser::StreamString stream;
    stream.s = data;
    return load_stream(*this, stream, "<string>");
}

Error ConfigFile::save(const String& path) const {
    return save_file(values, path);
}

Error ConfigFile::save_encrypted_pass(
    const String& path,
    const String& password
) const {
    return save_file(values, path, password);
}

Error ConfigFile::save_encrypted(const String& path, const Vector<uint8_t>& key)
    const {
    return save_file(values, path, String(), key);
}

void ConfigFile::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("get_value", "section", "key", "default"),
        &ConfigFile::get_value,
        DEFVAL(Variant())
    );
    ClassDB::bind_method(
        D_METHOD("set_value", "section", "key", "value"),
        &ConfigFile::set_value
    );

    ClassDB::bind_method(
        D_METHOD("has_section", "section"),
        &ConfigFile::has_section
    );
    ClassDB::bind_method(
        D_METHOD("has_section_key", "section", "key"),
        &ConfigFile::has_section_key
    );

    ClassDB::bind_method(D_METHOD("get_sections"), &ConfigFile::_get_sections);
    ClassDB::bind_method(
        D_METHOD("get_section_keys", "section"),
        &ConfigFile::_get_section_keys
    );

    ClassDB::bind_method(D_METHOD("clear"), &ConfigFile::clear);
    ClassDB::bind_method(
        D_METHOD("erase_section", "section"),
        &ConfigFile::erase_section
    );
    ClassDB::bind_method(
        D_METHOD("erase_section_key", "section", "key"),
        &ConfigFile::erase_section_key
    );

    ClassDB::bind_method(D_METHOD("parse", "data"), &ConfigFile::parse);
    ClassDB::bind_method(D_METHOD("load", "path"), &ConfigFile::load);
    ClassDB::bind_method(
        D_METHOD("load_encrypted_pass", "path", "password"),
        &ConfigFile::load_encrypted_pass
    );
    ClassDB::bind_method(
        D_METHOD("load_encrypted", "path", "key"),
        &ConfigFile::load_encrypted
    );

    ClassDB::bind_method(D_METHOD("save", "path"), &ConfigFile::save);
    ClassDB::bind_method(
        D_METHOD("save_encrypted_pass", "path", "password"),
        &ConfigFile::save_encrypted_pass
    );
    ClassDB::bind_method(
        D_METHOD("save_encrypted", "path", "key"),
        &ConfigFile::save_encrypted
    );
}

PoolStringArray ConfigFile::_get_sections() const {
    List<String> sections;
    get_sections(&sections);
    PoolStringArray result;
    result.resize(sections.size());
    int idx = 0;
    for (auto section = sections.front(); section; section = section->next()) {
        result.set(idx++, section->get());
    }
    return result;
}

PoolStringArray ConfigFile::_get_section_keys(const String& section) const {
    List<String> keys;
    get_section_keys(section, &keys);
    PoolStringArray result;
    result.resize(keys.size());
    int idx = 0;
    for (auto key = keys.front(); key; key = key->next()) {
        result.set(idx++, key->get());
    }
    return result;
}
